#ifndef DOBOTTYPE_H
#define DOBOTTYPE_H

#include <stdint.h>

/*********************************************************************************************************
** 数据结构等定义
*********************************************************************************************************/

/*********************************************************************************************************
** 公共部分
*********************************************************************************************************/
#pragma pack(push)
#pragma pack(1)

/*
 * 末端选择
 */
typedef enum tagEndType {
    EndTypeCustom,
    EndTypeSuctionCap,
    EndTypeGripper,
    EndTypeLaser,
    EndTypeMax,
    EndTypeNum = EndTypeMax
}EndType;

typedef struct tagEndTypeParams {
    uint8_t endType;
    float xBias;
    float yBias;
    float zBias;
}EndTypeParams;

/*
 * 初始位置
 */
typedef struct tagInitialPose {
    float jong1Angle;
    float jong2Angle;
    float jong3Angle;
    float jong4Angle;
}InitialPose;

/*
 * 位姿
 */
typedef struct tagPose {
    float x;
    float y;
    float z;
    float rHead;
    float jointAngle[4];
}Pose;

/*
 * 运动学参数
 */
typedef struct tagKinematics {
    float velocity;
    float acceleration;
}Kinematics;

/*
 * 报警状态：暂时支持32种报警状态
 */
typedef struct tagAlarmsState {
    uint32_t alarmsState;
}AlarmsState;

/*********************************************************************************************************
** 点动示教部分
*********************************************************************************************************/
/*
 * 单关节点动示教参数
 */
typedef struct tagJointJogParams {
    float joint1Velocity;
    float joint1Acceleration;
    float joint2Velocity;
    float joint2Acceleration;
    float joint3Velocity;
    float joint3Acceleration;
    float joint4Velocity;
    float joint4Acceleration;
}JointJogParams;

/*
 * 单坐标轴点动示教参数
 */
typedef struct tagCoordinateJogParams {
    float xVelocity;
    float xAcceleration;
    float yVelocity;
    float yAcceleration;
    float zVelocity;
    float zAcceleration;
    float rVelocity;
    float rAcceleration;
}CoordinateJogParams;

/*
 * 点动示教公共参数
 */
typedef struct tagJogCommonParams {
    float velocityRatio;
    float accelerationRatio;
}JogCommonParams;

/*
 * Jog Cmd
 */
enum JogCmd {
    JogIdle,
    JogAPPressed,
    JogANPressed,
    JogBPPressed,
    JogBNPressed,
    JogCPPressed,
    JogCNPressed,
    JogDPPressed,
    JogDNPressed,
    JogEPPressed,
    JogENPressed
};

/*
 * Jog instant cmd
 */

typedef struct tagJogInstantCmd {
    uint8_t isJoint;
    enum JogCmd cmd;
}JogInstantCmd;

/*********************************************************************************************************
** 再现运动部分
*********************************************************************************************************/
/*
 * 再现运动参数
 */
typedef struct tagPlaybackSpeedParams {
    float joint1Velocity;
    float joint1Acceleration;
    float joint2Velocity;
    float joint2Acceleration;
    float joint3Velocity;
    float joint3Acceleration;
    float joint4Velocity;
    float joint4Acceleration;
    float linearVelocity;
    float linearAcceleration;
}PlaybackSpeedParams;

typedef struct tagPlaybackJumpParams {
    float jumpHeight;
    float maxJumpHeight;
}PlaybackJumpParams;

typedef struct tagPlaybackCommonParams {
    float velocityRatio;
    float accelerationRatio;
}PlaybackCommonParams;

enum PlaybackInstantCmd {
    PlaybackStart,
    PlaybackPause,
    PlaybackStop
};

// For play back
enum MotionStyle {
    JUMP,
    MOVJ,
    MOVL
};

typedef enum tagInputPin {
    InputPinNone,
    InputPin1,
    InputPin2,
    InputPin3,
    InputPin4,
    InputPin5,
    InputPin6,
    InputPin7,
    InputPin8
}InputPin;

typedef enum tagInputLevel {
    InputLevelBoth,
    InputLevelLow,
    InputLevelHigh
}InputLevel;

typedef struct tagPlaybackPoint {
    uint8_t inputPin;
    uint8_t inputLevel;
    uint8_t motionStyle;
    float x;
    float y;
    float z;
    float rHead;
    float gripper;
    float pauseTime;
    unsigned int ioState;
}PlaybackPoint;

typedef struct tagPlaybackBufferCmd {
    uint32_t loop;
    uint32_t line;
    PlaybackPoint playbackPoint;
}PlaybackBufferCmd;

/*********************************************************************************************************
** 连续轨迹：Continuous path
*********************************************************************************************************/
/*
 * CP参数
 */
typedef struct tagCPParams
{
    float planAcc;
    float juncitionVel;
    float acc;
}CPParams;

enum CPInstantCmd {
    CPStart,
    CPPause,
    CPStop
};

typedef struct tagCPBufferCmd {
    uint32_t loop;
    uint32_t line;

    float x;
    float y;
    float z;
    float velocity;
    uint32_t ioState;
}CPBufferCmd;

/*********************************************************************************************************
** User parameters
*********************************************************************************************************/
typedef struct tagUserParams {
    float params[8];
}UserParams;

/*********************************************************************************************************
** API result
*********************************************************************************************************/

enum DobotResult {
    DobotResult_NoError,
    DobotResult_NotFound,
    DobotResult_Occupied,
    DobotResult_Timeout,

};

#pragma pack(pop)

#endif
