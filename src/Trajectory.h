#ifndef TRAJECTORY_H
#define TRAJECTORY_H

enum TRAJ_TYPE
{
    LINE,
    SPLINE
};

/*
@brief: node type
@author: JoMar
@date: 2016-07-05
*/
enum NODE_TYPE
{
    START_NODE,
    MIDDLE_NODE,
    END_NODE
};

class Trajectory
{
public:
    Trajectory();
    Trajectory(const TRAJ_TYPE &type);
    Trajectory(const Trajectory &object);
    Trajectory &operator=(const Trajectory &object);
    virtual ~Trajectory();
    virtual NODE_TYPE LastNodeType();
    virtual int LastNodeTimeStamp();
    virtual int FirstNodeTimeStamp();

    TRAJ_TYPE Type();

private:
    TRAJ_TYPE m_type;

};

#endif // TRAJECTORY_H
