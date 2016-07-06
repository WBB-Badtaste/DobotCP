#include "Trajectory.h"

Trajectory::Trajectory() : m_type(LINE)
{

}

Trajectory::~Trajectory()
{

}

Trajectory::Trajectory(const Trajectory &object):m_type(object.m_type)
{

}

Trajectory::Trajectory(const TRAJ_TYPE t):m_type(t)
{

}

Trajectory& Trajectory::operator =(const Trajectory& object)
{
    m_type = object.m_type;
    return *this;
}

TRAJ_TYPE Trajectory::Type()
{
    return m_type;
}

NODE_TYPE Trajectory::LastNodeType()
{
    return END_NODE;
}

int Trajectory::FirstNodeTimeStamp()
{
    return 0;
}

int Trajectory::LastNodeTimeStamp()
{
    return 0;
}
