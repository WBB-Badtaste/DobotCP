#ifndef SPLINEOPERATOR_H
#define SPLINEOPERATOR_H

#include "E2CubicSpline.h"
#include <deque>


/*
@brief: 2 dimensional point
@author: JoMar
@date: 2016-06-16
*/
typedef class _traj_e2_point
{
public:
    double t;
    double x;
    _traj_e2_point() : t(0), x(0){}
    virtual ~_traj_e2_point(){}
    _traj_e2_point(const double newT, const double newX) : t(newT), x(newX){}
    _traj_e2_point(const _traj_e2_point& object) : t(object.t), x(object.x){}
    _traj_e2_point& operator=(const _traj_e2_point& object)
    {
        t = object.t;
        x = object.x;
        return *this;
    }
}
/*
@brief: 2 dimensional point
@author: JoMar
@date: 2016-07-02
*/
TRAJ_E2_POINT;

/*
@brief: 3 dimensional point
@author: JoMar
@date: 2016-07-02
*/
typedef class _traj_e3_point : public TRAJ_E2_POINT
{
public:
    double y;
    _traj_e3_point()
        : TRAJ_E2_POINT()
        , y(0)
    {}
    virtual ~_traj_e3_point(){}
    _traj_e3_point(const double newT, const double newX, const double newY)
        : TRAJ_E2_POINT(newT, newX)
        , y(newY)
    {}
    _traj_e3_point(const _traj_e3_point& object)
        : TRAJ_E2_POINT(object.t, object.x)
        , y(object.y)
    {}
    _traj_e3_point& operator=(const _traj_e3_point& object)
    {
        t = object.t;
        x = object.x;
        y = object.y;
        return *this;
    }
}
/*
@brief: 3 dimensional point
@author: JoMar
@date: 2016-07-02
*/
TRAJ_E3_POINT;

/*
@brief: 4 dimensional point
@author: JoMar
@date: 2016-07-02
*/
typedef class _traj_e4_point : public TRAJ_E3_POINT
{
public:
    double z;
    _traj_e4_point()
        : TRAJ_E3_POINT()
        , z(0)
    {}
    virtual ~_traj_e4_point(){}
    _traj_e4_point(const double newT, const double newX, const double newY, const double newZ)
        : TRAJ_E3_POINT(newT, newX, newY)
        , z(newZ)
    {}
    _traj_e4_point(const _traj_e4_point& object)
        : TRAJ_E3_POINT(object.t, object.x, object.y)
        , z(object.z)
    {}
    _traj_e4_point& operator=(const _traj_e4_point& object)
    {
        t = object.t;
        x = object.x;
        y = object.y;
        z = object.z;
        return *this;
    }
}
/*
@brief: 4 dimensional point
@author: JoMar
@date: 2016-07-02
*/
TRAJ_E4_POINT;

/*
@brief: 一个用来规划轨迹的类，自动划分轨迹类型
@author: JoMar
@date: 2016-07-04
*/
class TrajectoryOperator
{
public:
    TrajectoryOperator();

    bool EnterPoint(TRAJ_E4_POINT &point);

    bool GetDataByTimeStamp(const int time, double &x, double &y, double &z);

    TRAJ_E4_POINT m_lastEnterPoint;
    TRAJ_E4_POINT m_lastTrajPoint;

private:
    //性能参数
    unsigned numberOfTrajectoryCreating;//最少控制点数
    unsigned timeOfTrajectoryCreating;//最少时间间隔

    //轨迹数据，待运动
    std::deque<Trajectory*> m_trajX;
    std::deque<Trajectory*> m_trajY;
    std::deque<Trajectory*> m_trajZ;


    //采集数据，待转化为轨迹
    std::deque<double> m_sampleT;
    std::deque<double> m_sampleX;
    std::deque<double> m_sampleY;
    std::deque<double> m_sampleZ;

    //获取轨迹开始标志
    bool m_bStartTraj;

    //保存采样点
    inline void SaveSamplePoint(TRAJ_E4_POINT &point);

    inline void PopSamplePoint(const unsigned num);

    void CreateTrajectory();

    void CreateOneAxisTarjectory(const unsigned &num, std::deque<double> &sampleDatas, std::deque<Trajectory*> &traj);

    inline bool GetOneAxisData(const int time, double &data, std::deque<Trajectory*> &trajs);
};

#endif // SPLINEOPERATOR_H
