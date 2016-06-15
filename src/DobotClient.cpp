/*
  DobotClient.cpp - Dobot client
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

#include "DobotClient.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "ProtocolID.h"

DobotClient::DobotClient(QObject *parent)
  : QObject(parent),
    comPM(new CComPM(this)),
    isJointJog(false)
{
    bufferCmd.reserve(128);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(10);

    connect(comPM, SIGNAL(messageReady(Message)), this, SLOT(onMessageReady(Message)));
}

DobotClient::~DobotClient()
{

}

DobotClient *DobotClient::instance(void)
{
    static DobotClient *instance = 0;
    if (instance == 0) {
        instance = new DobotClient();
    }
    return instance;
}

void DobotClient::init(void)
{

}

enum DobotClient::DobotStatus DobotClient::getDobotStatus(void)
{
    return dobotStatus;
}

void DobotClient::sendSetEndTypeCmd(EndType endType, float xBias, float yBias, float zBias)
{
    Message message;

    message.id = ProtocolSetEndType;
    message.params[0] = endType;
    if (endType == Custom) {
        memcpy(&message.params[1], &xBias, 4);
        memcpy(&message.params[5], &yBias, 4);
        memcpy(&message.params[9], &zBias, 4);
        message.paramsLen = 13;
    } else {
        message.paramsLen = 1;
    }
    bufferCmd.push_back(message);
}

void DobotClient::sendGetEndTypeCmd(void)
{
    Message message;

    message.id = ProtocolGetEndType;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetInitialPoseCmd(float joint1Angle, float joint2Angle, float joint3Angle, float joint4Angle)
{
    Message message;

    message.id = ProtocolSetInitialPose;
    memcpy(&message.params[0], &joint1Angle, 4);
    memcpy(&message.params[4], &joint2Angle, 4);
    memcpy(&message.params[8], &joint3Angle, 4);
    memcpy(&message.params[12], &joint4Angle, 4);

    message.paramsLen = 16;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetInitialPoseCmd(void)
{
    Message message;

    message.id = ProtocolGetInitialPose;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPoseCmd(void)
{
    Message message;

    message.id = ProtocolGetPose;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetKinematicsCmd(void)
{
    Message message;

    message.id = ProtocolGetKinematics;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetAlarmsStateCmd(void)
{
    Message message;

    message.id = ProtocolGetAlarmsState;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetIOStateCmd(unsigned int ioState)
{
    Message message;

    message.id = ProtocolSetIOState;
    memcpy(&message.params[0], &ioState, 4);

    message.paramsLen = 4;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetIOStateCmd(void)
{
    Message message;

    message.id = ProtocolGetIOState;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

// Jog functions
void DobotClient::sendSetJogJointParams(float joint1Velocity, float joint1Acceleration,
                            float joint2Velocity, float joint2Acceleration,
                            float joint3Velocity, float joint3Acceleration,
                            float joint4Velocity, float joint4Acceleration)
{
    Message message;

    message.id = ProtocolSetJogJointParameters;
    memcpy(&message.params[0], &joint1Velocity, 4);
    memcpy(&message.params[4], &joint1Acceleration, 4);
    memcpy(&message.params[8], &joint2Velocity, 4);
    memcpy(&message.params[12], &joint2Acceleration, 4);
    memcpy(&message.params[16], &joint3Velocity, 4);
    memcpy(&message.params[20], &joint3Acceleration, 4);
    memcpy(&message.params[24], &joint4Velocity, 4);
    memcpy(&message.params[28], &joint4Acceleration, 4);

    message.paramsLen = 32;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetJogJointParams(void)
{
    Message message;

    message.id = ProtocolGetJogJointParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetJogCoordinateParams(float xVelocity, float xAcceleration,
                                float yVelocity, float yAcceleration,
                                float zVelocity, float zAcceleration,
                                float rVelocity, float rAcceleration)
{
    Message message;

    message.id = ProtocolSetJogCoordinateParameters;
    memcpy(&message.params[0], &xVelocity, 4);
    memcpy(&message.params[4], &xAcceleration, 4);
    memcpy(&message.params[8], &yVelocity, 4);
    memcpy(&message.params[12], &yAcceleration, 4);
    memcpy(&message.params[16], &zVelocity, 4);
    memcpy(&message.params[20], &zAcceleration, 4);
    memcpy(&message.params[24], &rVelocity, 4);
    memcpy(&message.params[28], &rAcceleration, 4);

    message.paramsLen = 32;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetJogCoordinateParams(void)
{
    Message message;

    message.id = ProtocolGetJogCoordinateParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetJogCommonParams(float velocityRatio, float accelerationRatio)
{
    Message message;

    message.id = ProtocolSetJogCommonParameters;
    memcpy(&message.params[0], &velocityRatio, 4);
    memcpy(&message.params[4], &accelerationRatio, 4);

    message.paramsLen = 8;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetJogCommonParams(void)
{
    Message message;

    message.id = ProtocolGetJogCommonParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetJogCmd(bool isJointJog, enum JogCmd jogCmd)
{
    Message message;

    if (jogCmd != JogIdle) {
        this->isJointJog = isJointJog;
    }
    message.id = ProtocolSetJogCmd;
    message.params[0] = this->isJointJog;
    message.params[1] = jogCmd;

    message.paramsLen = 2;
    bufferCmd.push_back(message);
}

// Playback
void DobotClient::sendSetPlaybackSpeedParams(float joint1Velocity, float joint1Acceleration,
                                             float joint2Velocity, float joint2Acceleration,
                                             float joint3Velocity, float joint3Acceleration,
                                             float joint4Velocity, float joint4Acceleration,
                                             float linearVelocity, float linearAcceleration)
{
    Message message;

    message.id = ProtocolSetPlaybackSpeedParameters;
    memcpy(&message.params[0], &joint1Velocity, 4);
    memcpy(&message.params[4], &joint1Acceleration, 4);
    memcpy(&message.params[8], &joint2Velocity, 4);
    memcpy(&message.params[12], &joint2Acceleration, 4);
    memcpy(&message.params[16], &joint3Velocity, 4);
    memcpy(&message.params[20], &joint3Acceleration, 4);
    memcpy(&message.params[24], &joint4Velocity, 4);
    memcpy(&message.params[28], &joint4Acceleration, 4);

    memcpy(&message.params[32], &linearVelocity, 4);
    memcpy(&message.params[36], &linearAcceleration, 4);


    message.paramsLen = 40;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPlaybackSpeedParams(void)
{
    Message message;

    message.id = ProtocolGetPlaybackSpeedParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetPlaybackJumpParams(float jumpHeight, float maxJumpHeight)
{
    Message message;

    message.id = ProtocolSetPlaybackJumpParameters;
    memcpy(&message.params[0], &jumpHeight, 4);
    memcpy(&message.params[4], &maxJumpHeight, 4);

    message.paramsLen = 8;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPlaybackJumpParams(void)
{
    Message message;

    message.id = ProtocolGetPlaybackJumpParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetPlaybackCommonParams(float velocityRatio, float accelerationRatio)
{
    Message message;

    message.id = ProtocolSetPlaybackCommonParameters;
    memcpy(&message.params[0], &velocityRatio, 4);
    memcpy(&message.params[4], &accelerationRatio, 4);

    message.paramsLen = 8;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPlaybackCommonParams(void)
{
    Message message;

    message.id = ProtocolGetPlaybackCommonParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPlaybackCurrentLoopLine(void)
{
    Message message;

    message.id = ProtocolGetPlaybackCurrentLoopLine;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetPlaybackInstantCmd(enum PlaybackInstantCmd cmd)
{
    Message message;

    message.id = ProtocolSetPlaybackInstantCmd;
    message.params[0] = cmd;
    message.paramsLen = 1;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetPlaybackBufferSizeCmd(void)
{
    Message message;

    message.id = ProtocolGetPlaybackBufferSize;
    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetPlaybackBufferCmd(quint32 currentLoop, quint32 currentLine, MotionStyle motionStyle,
                        float x, float y, float z, float rHead, float gripper/*reserved*/,
                        float pauseTime, quint32 ioState)
{
    Message message;

    message.id = ProtocolSetPlaybackBufferCmd;
    memcpy(&message.params[0], &currentLoop, 4);
    memcpy(&message.params[4], &currentLine, 4);
    message.params[8] = motionStyle;
    memcpy(&message.params[9], &x, 4);
    memcpy(&message.params[13], &y, 4);
    memcpy(&message.params[17], &z, 4);
    memcpy(&message.params[21], &rHead, 4);
    memcpy(&message.params[25], &gripper, 4);
    memcpy(&message.params[29], &pauseTime, 4);
    memcpy(&message.params[33], &ioState, 4);

    message.paramsLen = 37;
    bufferCmd.push_back(message);
}

///

void DobotClient::sendSetCPParams(float acceleration, float cornerVelocity, float actualAcceleration)
{
    Message message;

    message.id = ProtocolSetCPParameters;
    memcpy(&message.params[0], &acceleration, 4);
    memcpy(&message.params[4], &cornerVelocity, 4);
    memcpy(&message.params[8], &actualAcceleration, 4);

    message.paramsLen = 12;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetCPParams(void)
{
    Message message;

    message.id = ProtocolGetCPParameters;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetCPCurrentLoopLine(void)
{
    Message message;

    message.id = ProtocolGetCPCurrentLoopLine;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetCPInstantCmd(enum CPInstantCmd cmd)
{
    Message message;

    message.id = ProtocolSetCPInstantCmd;
    message.params[0] = cmd;
    message.paramsLen = 1;
    bufferCmd.push_back(message);
}

void DobotClient::sendGetCPBufferSizeCmd(void)
{
    Message message;

    message.id = ProtocolGetCPBufferSize;

    message.paramsLen = 0;
    bufferCmd.push_back(message);
}

void DobotClient::sendSetCPBufferCmd(quint32 currentLoop, quint32 currentLine, float deltaX, float deltaY, float deltaZ, float velocity)
{
    Message message;

    message.id = ProtocolSetCPBufferCmd;
    memcpy(&message.params[0], &currentLoop, 4);
    memcpy(&message.params[4], &currentLine, 4);
    memcpy(&message.params[8], &deltaX, 4);
    memcpy(&message.params[12], &deltaY, 4);
    memcpy(&message.params[16], &deltaZ, 4);
    memcpy(&message.params[20], &velocity, 4);

    message.paramsLen = 24;
    bufferCmd.push_back(message);
}

void DobotClient::onMessageReady(const Message &message)
{
    // Erase those waiting ones that their ids are the same, and emit the signals
    if (not waitingCmd.isEmpty()) {
        if (waitingCmd.at(0).second.id == message.id) {
            emit commandResult(message);
            waitingCmd.removeFirst();
            dobotStatus = Idle;
            // Send next command at once
            onTimer();
        }
    }
}

void DobotClient::onTimer(void)
{
    // Check whether any waiting command is timeout
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (not waitingCmd.isEmpty()) {
        if (currentTime - waitingCmd.at(0).first > 1000) {
            emit commandResult(waitingCmd.at(0).second, true);
            waitingCmd.removeFirst();
            dobotStatus = Idle;
        }
    }
    // Send new command
    if (waitingCmd.isEmpty() and
        not bufferCmd.isEmpty()) {
        comPM->packMessage(bufferCmd.at(0));
        waitingCmd.push_back(qMakePair(currentTime, bufferCmd.at(0)));
        bufferCmd.removeFirst();
        dobotStatus = Busy;
    }
}
