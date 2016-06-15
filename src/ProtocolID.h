/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           ProtocolID.h
** Latest modified Date:
** Latest Version:      V1.0.0
** Descriptions:        Protocol ID definition
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Liu Zhufu
** Created date:        2016-03-15
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef PROTOCOLID_H
#define PROTOCOLID_H

// About the state of setting command

typedef enum {
    // Public protocol IDs---------//

    // End type select
    ProtocolSetEndType,
    ProtocolGetEndType,
    // Initial positions
    ProtocolSetInitialPose,
    ProtocolGetInitialPose,

    // Position and orientation
    ProtocolGetPose,

    // Kinematics:Speed and acceleration
    ProtocolGetKinematics,

    // Alarms state
    ProtocolGetAlarmsState,

    // I/O
    ProtocolSetIOState,
    ProtocolGetIOState,

    // Jog protocol IDs--------//
    // Joint parameters:Joint jog parameters & Coordinate jog parameters
    ProtocolSetJogJointParameters,
    ProtocolGetJogJointParameters,

    ProtocolSetJogCoordinateParameters,
    ProtocolGetJogCoordinateParameters,

    ProtocolSetJogCommonParameters,
    ProtocolGetJogCommonParameters,

    // Jog command
    ProtocolSetJogCmd,

    // Playback protocol IDs--------//
    // Playback parameters
    ProtocolSetPlaybackSpeedParameters,
    ProtocolGetPlaybackSpeedParameters,

    ProtocolSetPlaybackJumpParameters,
    ProtocolGetPlaybackJumpParameters,

    ProtocolSetPlaybackCommonParameters,
    ProtocolGetPlaybackCommonParameters,

    ProtocolGetPlaybackCurrentLoopLine,

    // Playback command
    ProtocolSetPlaybackInstantCmd,
    ProtocolGetPlaybackBufferSize,
    ProtocolSetPlaybackBufferCmd,

    // Continuous path protocol IDs--------//
    ProtocolSetCPParameters,
    ProtocolGetCPParameters,

    ProtocolGetCPCurrentLoopLine,

    // Continuous path command
    ProtocolSetCPInstantCmd,
    ProtocolGetCPBufferSize,
    ProtocolSetCPBufferCmd,

} ProtocolID;

#endif
