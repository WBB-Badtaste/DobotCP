/*
  DobotClient.h - Dobot client
  Copyright (c) 2015-2016 Yuejiang Technology Co., Ltd. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DOBOTCLIENT_H
#define DOBOTCLIENT_H

#include <QObject>
#include <QPair>
#include <QVector>

#include "CComPM.h"

class DobotClient : public QObject
{
    Q_OBJECT
public:
    // Dobot status
    enum DobotStatus {
        Idle,
        Busy
    };

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

    // For end type
    enum EndType {
        Custom,
        Pump,
        Gripper,
        Laser
    };

    // For play back
    enum MotionStyle {
        JUMP,
        MOVJ,
        MOVL
    };

    typedef struct tagPlaybackPoint {
        MotionStyle motionStyle;
        float x;
        float y;
        float z;
        float rHead;
        float gripper;
        float pauseTime;
        unsigned int ioState;
    }PlaybackPoint;

    enum PlaybackInstantCmd {
        PlaybackStart,
        PlaybackPause,
        PlaybackStop
    };

    enum CPInstantCmd {
        CPStart,
        CPPause,
        CPStop
    };

private:
    DobotClient(QObject *parent = 0);
    ~DobotClient();
public:
    static DobotClient *instance(void);
    void init(void);

    // Dobot status
    enum DobotStatus getDobotStatus(void);

    // Public functions
    void sendSetEndTypeCmd(EndType endType, float xBias = 0, float yBias = 0, float zBias = 0);
    void sendGetEndTypeCmd(void);

    void sendSetInitialPoseCmd(float joint1Angle, float joint2Angle, float joint3Angle, float joint4Angle = 0);
    void sendGetInitialPoseCmd(void);

    void sendGetPoseCmd(void);

    void sendGetKinematicsCmd(void);

    void sendGetAlarmsStateCmd(void);

    void sendSetIOStateCmd(unsigned int ioState);
    void sendGetIOStateCmd(void);

    // Jog functions
    void sendSetJogJointParams(float joint1Velocity, float joint1Acceleration,
                                float joint2Velocity, float joint2Acceleration,
                                float joint3Velocity, float joint3Acceleration,
                                float joint4Velocity, float joint4Acceleration);
    void sendGetJogJointParams(void);
    void sendSetJogCoordinateParams(float xVelocity, float xAcceleration,
                                    float yVelocity, float yAcceleration,
                                    float zVelocity, float zAcceleration,
                                    float rVelocity, float rAcceleration);
    void sendGetJogCoordinateParams(void);
    void sendSetJogCommonParams(float velocityRatio, float accelerationRatio);
    void sendGetJogCommonParams(void);
    void sendSetJogCmd(bool isJointJog, enum JogCmd jogCmd);

    // Play back functions
    void sendSetPlaybackSpeedParams(float joint1Velocity, float joint1Acceleration,
                                                 float joint2Velocity, float joint2Acceleration,
                                                 float joint3Velocity, float joint3Acceleration,
                                                 float joint4Velocity, float joint4Acceleration,
                                                 float linearVelocity, float linearAcceleration);
    void sendGetPlaybackSpeedParams(void);

    void sendSetPlaybackJumpParams(float jumpHeight, float maxJumpHeight);
    void sendGetPlaybackJumpParams(void);
    void sendSetPlaybackCommonParams(float velocityRatio, float accelerationRatio);
    void sendGetPlaybackCommonParams(void);

    void sendGetPlaybackCurrentLoopLine(void);

    void sendSetPlaybackInstantCmd(enum PlaybackInstantCmd cmd);
    void sendGetPlaybackBufferSizeCmd(void);
    void sendSetPlaybackBufferCmd(quint32 currentLoop, quint32 currentLine, MotionStyle motionStyle,
                            float x, float y, float z, float rHead, float gripper/*reserved*/,
                            float pauseTime, quint32 ioState);

    // Write functions
    void sendSetCPParams(float acceleration, float cornerVelocity, float actualAcceleration);
    void sendGetCPParams(void);

    void sendGetCPCurrentLoopLine(void);

    void sendSetCPInstantCmd(enum CPInstantCmd cmd);
    void sendGetCPBufferSizeCmd(void);
    void sendSetCPBufferCmd(quint32 currentLoop, quint32 currentLine, float deltaX, float deltaY, float deltaZ, float velocity);

signals:
    void commandResult(const Message &message, bool isTimeout = false);
private slots:
    void onMessageReady(const Message &message);
    void onTimer(void);
private:
    DobotStatus dobotStatus;
    QVector<Message> bufferCmd;
    QVector<QPair<qint64, Message> > waitingCmd;

    CComPM *comPM;

    bool isJointJog;
};

#endif // DOBOTCLIENT_H
