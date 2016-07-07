#include "TrajectoryOperator.h"
#include <qdebug.h>


TrajectoryOperator::TrajectoryOperator()
    : m_lastEnterPoint()
    , m_lastTrajPoint()
    , numberOfTrajectoryCreating(50)
    , timeOfTrajectoryCreating(5000)
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
    if( traj.size() < 1 ||//轨迹已经开始生成，不然会出现野指针
        traj.back()->LastNodeType() == END_NODE )//上一段结束
    {
        //！！添加轨迹类型判断！！

        E2_CUB_SPLINE *spline = new E2_CUB_SPLINE();

        if(1)//判断是否应该结束，现在没有合适方案，不结束spline
            spline->Create(num, m_sampleT, sampleDatas, START_SEC);
        else//该段就是完整的spline曲线
            spline->Create(num, m_sampleT, sampleDatas, STATIC_SEC);

        traj.push_back(spline);

    }
    else//接上一段
    {
        //判断类型
        if(traj.back()->Type() == SPLINE)//最后一段轨迹是spline
        {
            E2_CUB_SPLINE* spline = (E2_CUB_SPLINE*)traj.back();

            if(1)//判断是否应该结束，现在没有合适方案，不结束spline
                spline->Create(num, m_sampleT, sampleDatas, MIDDLE_SEC);
            else
                spline->Create(num, m_sampleT, sampleDatas, END_SEC);
        }
        else//其他类型没有算法
        {

        }
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

    //存点到临时区域
    m_lastEnterPoint = point;

    //存入缓冲区
    SaveSamplePoint(point);

    if(point.t - m_lastTrajPoint.t >= timeOfTrajectoryCreating &&
       m_sampleT.size() > numberOfTrajectoryCreating)//判断是否达到生成新轨迹条件
    {
        CreateTrajectory();//生成轨迹
        m_lastTrajPoint = point;
    }

    return true;
}

bool TrajectoryOperator::IsEmpty()
{
    if(m_trajX.size() < 1 &&
       m_trajY.size() < 1 &&
       m_trajZ.size() < 1 )
        return true;
    else
        return false;
}

bool TrajectoryOperator::GetOneAxisData(const int time, double &data, std::deque<Trajectory*> &trajs)
{

    if(trajs.size() < 1)
        return false;

//    if(!m_bStartTraj)//还没有开始走轨迹
//        beginNewTraj = true;

    if(time > trajs.front()->LastNodeTimeStamp())
    {//时间戳超过第一个轨迹区间的最后一个结点
        if(trajs.front()->LastNodeType() == END_NODE)
        {//第一个轨迹区间是完整的，则删除该轨迹
            delete trajs.front();
            trajs.pop_front();
            qDebug()<< "A tragectory has been deleted!";
        }
        else
            return false;

//        beginNewTraj = true;
    }

    if(m_trajX.front()->Type() == SPLINE)//区分轨迹区间类型
    {
        E2_CUB_SPLINE *spline = (E2_CUB_SPLINE *)trajs.front();
        spline->GetDataByTimeStamp(time, &data, nullptr, nullptr);
        /*
        if(beginNewTraj)
            spline->GetDataByTimeStamp(time, &data, nullptr, nullptr, ERGODIC_START);
        else
            spline->GetDataByTimeStamp(time, &data, nullptr, nullptr, ERGODIC_CONTINUE);
        */
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
