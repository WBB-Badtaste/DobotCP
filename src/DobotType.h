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
** HOME参数
*********************************************************************************************************/
typedef struct tagHOMEParams {
    float temp;
}HOMEParams;

typedef struct tagHOMECmd {
    uint32_t temp;
}HOMECmd;

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
enum {
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

typedef struct tagJogCmd {
    uint8_t isJoint;
    uint8_t cmd;
}JogCmd;

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

// For play back
enum PTPMode {
    PTPJUMPXYZMode,
    PTPMOVJXYZMode,
    PTPMOVLXYZMode,

    PTPJUMPANGLEMode,
    PTPMOVJANGLEMode,
    PTPMOVLANGLEMode,

    PTPMOVJXYZINCMode,
    PTPMOVLXYZINCMode,
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

typedef struct tagPlaybackCmd {
    uint8_t ptpMode;
    float x;
    float y;
    float z;
    float rHead;
    float gripper;
}PlaybackCmd;

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

enum ContinuousPathMode {
    CPRelativeMode,
    CPAbsoluteMode
};

typedef struct tagCPCmd {
    uint8_t cpMode;
    float x;
    float y;
    float z;
    float velocity;
}CPCmd;

/*********************************************************************************************************
** User parameters
*********************************************************************************************************/
typedef struct tagTRACKParams {
    uint32_t temp;
}TRACKParams;

typedef struct tagTRACKCmd {
    uint32_t temp;
}TRACKCmd;

typedef struct tagWAITParams {
    uint8_t unitType;
}WAITParams;

typedef struct tagWAITCmd {
    float waitTime;
}WAITCmd;

typedef struct tagTRIGParams {
    uint32_t temp;
}TRIGParams;

typedef struct tagTRIGCmd {
    uint8_t inputPin;
    uint8_t inputLevel;
}TRIGCmd;

typedef enum tagGPIOFunction {
    GPIOFunction_GPIO,
    GPIOFunction_PWM,
    GPIOFunction_ADC
}GPIOFunction;

/*********************************************************************************************************
** User parameters
*********************************************************************************************************/
typedef struct tagUserParams {
    float params[8];
}UserParams;

/*********************************************************************************************************
** Queued command related type
*********************************************************************************************************/
typedef enum tagQueuedCmdExecMode {
    QueuedCmdExecOnlineMode,
    QueuedCmdExecOfflineMode
}QueuedCmdExecMode;

typedef enum tagQueuedCmdExecCtrl {
    QueuedCmdExecStartCtrl,
    QueuedCmdExecPauseCtrl,
    QueuedCmdExecStopCtrl,
}QueuedCmdExecCtrl;

/*********************************************************************************************************
** API result
*********************************************************************************************************/
enum {
    DobotConnect_NoError,
    DobotConnect_NotFound,
    DobotConnect_Occupied
};

enum {
    DobotCommunicate_NoError,
    DobotCommunicate_BufferFull,
    DobotCommunicate_Timeout
};

#pragma pack(pop)
#endif
