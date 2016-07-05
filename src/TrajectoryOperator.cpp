#include "TrajectoryOperator.h"


TrajectoryOperator::TrajectoryOperator()
    : m_lastEnterPoint()
    , m_lastTrajPoint()
    , numberOfTrajectoryCreating(5)
    , timeOfTrajectoryCreating(500)
    , m_trajX()
    , m_trajY()
    , m_trajZ()
    , m_sampleT()
    , m_sampleX()
    , m_sampleY()
    , m_sampleZ()
    , m_bStartTraj(false)
{

}

void TrajectoryOperator::SaveSamplePoint(TRAJ_E4_POINT &point)
{
    m_sampleT.push_back(point.t);
    m_sampleX.push_back(point.x);
    m_sampleY.push_back(point.y);
    m_sampleZ.push_back(point.z);
}

void TrajectoryOperator::PopSamplePoint(const unsigned num)
{
    for(unsigned i = 0; i < num; ++i)
    {
        m_sampleT.pop_front();
        m_sampleX.pop_front();
        m_sampleY.pop_front();
        m_sampleZ.pop_front();
    }
}

void TrajectoryOperator::CreateOneAxisTarjectory(const unsigned &num, std::deque<double> &sampleDatas, std::deque<Trajectory *> &traj)
{
    //判断是否连接上一段
    if( traj.size() > 0 //轨迹已经开始，不然出现野指针
     && traj.back()->Type() == SPLINE // 最后一段轨迹是spline
     && traj.back()->LastNodeType() != END_NODE)//spline轨迹没有结束
    {
        E2_CUB_SPLINE* splineX = (E2_CUB_SPLINE*)traj.back();
        if(1)//判断是否结束
            splineX->Create(num, m_sampleT, sampleDatas, MIDDLE_SEC);
        else
            splineX->Create(num, m_sampleT, sampleDatas, END_SEC);
    }
    else
    {
        E2_CUB_SPLINE *splineX = new E2_CUB_SPLINE();

        if(1)//判断是否结束
            splineX->Create(num, m_sampleT, sampleDatas, START_SEC);
        else
            splineX->Create(num, m_sampleT, sampleDatas, STATIC_SEC);
    }
}

void TrajectoryOperator::CreateTrajectory()
{
    unsigned num(m_sampleT.size());

    CreateOneAxisTarjectory(num, m_sampleX, m_trajX);
    CreateOneAxisTarjectory(num, m_sampleY, m_trajY);
    CreateOneAxisTarjectory(num, m_sampleZ, m_trajZ);

    PopSamplePoint(num);//删除已经生成轨迹的采样点
}


bool TrajectoryOperator::EnterPoint(TRAJ_E4_POINT &point)
{
    //添加过滤，识别功能
    if(point.t <= m_lastEnterPoint.t)//时间戳有误
        return false;
    else
        m_lastEnterPoint = point;

    //存入缓冲区
    SaveSamplePoint(point);

    if(point.t - m_lastEnterPoint.t >= timeOfTrajectoryCreating &&
       m_sampleT.size() > numberOfTrajectoryCreating)//判断是否达到生成新轨迹条件
    {
        CreateTrajectory();
    }
    else
        return true;//没达到轨迹生成条件

    return true;
}

bool TrajectoryOperator::GetOneAxisData(const int time, double &data, std::deque<Trajectory*> &trajs)
{
    bool beginNewTraj(false);

    if(trajs.size() < 1)
        return false;

    if(m_bStartTraj)
        beginNewTraj = true;

    if(time > trajs.front()->LastNodeTimeStamp())
    {//时间戳超过第一个轨迹区间的最后一个结点
        delete trajs.front();
        trajs.pop_front();

        beginNewTraj = true;
    }

    if(m_trajX.front()->Type() == SPLINE)//区分轨迹区间类型
    {
        E2_CUB_SPLINE *spline = (E2_CUB_SPLINE *)trajs.front();
        if(beginNewTraj)
            spline->GetDataByTimeStamp(time, &data, nullptr, nullptr, ERGODIC_START);
        else
            spline->GetDataByTimeStamp(time, &data, nullptr, nullptr, ERGODIC_CONTINUE);
    }

    return true;
}

bool TrajectoryOperator::GetDataByTimeStamp(const int time, double &x, double &y, double &z)
{
    if(!GetOneAxisData(time, x, m_trajX)) return false;
    if(!GetOneAxisData(time, y, m_trajY)) return false;
    if(!GetOneAxisData(time, z, m_trajZ)) return false;

    m_bStartTraj = true;
    return true;
}
