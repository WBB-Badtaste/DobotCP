#ifndef DOBOTDLL_H
#define DOBOTDLL_H

#include "dobotdll_global.h"
#include "DobotType.h"

extern "C" DOBOTDLLSHARED_EXPORT int ConnectDobot(int baudrate);
extern "C" DOBOTDLLSHARED_EXPORT void DisconnectDobot(void);
extern "C" DOBOTDLLSHARED_EXPORT void PeriodicTask(void);
extern "C" DOBOTDLLSHARED_EXPORT int SetCmdTimeout(uint32_t cmdTimeout);

// Public functions
// EndType Parameters
extern "C" DOBOTDLLSHARED_EXPORT int SetEndTypeParams(EndTypeParams *endTypeParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetEndTypeParams(EndTypeParams *endTypeParams);

// Initial pose
extern "C" DOBOTDLLSHARED_EXPORT int SetInitialPose(InitialPose *initialPose);
extern "C" DOBOTDLLSHARED_EXPORT int GetInitialPose(InitialPose *initialPose);

// Pose and Kinematics parameters are automatically get
extern "C" DOBOTDLLSHARED_EXPORT int GetPose(Pose *pose);
extern "C" DOBOTDLLSHARED_EXPORT int GetKinematics(Kinematics *kinematics);

// I/O
extern "C" DOBOTDLLSHARED_EXPORT int SetIOState(uint32_t ioState);
extern "C" DOBOTDLLSHARED_EXPORT int GetIOState(uint32_t *ioState);

// Jog functions
extern "C" DOBOTDLLSHARED_EXPORT int SetJogJointParams(JointJogParams *jointJogParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogJointParams(JointJogParams *jointJogParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetJogCoordinateParams(CoordinateJogParams *coordinateJogParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogCoordinateParams(CoordinateJogParams *coordinateJogParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetJogCommonParams(JogCommonParams *jogCommonParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogCommonParams(JogCommonParams *jogCommonParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetJogInstantCmd(JogInstantCmd *jogInstantCmd);

// Playback functions
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackSpeedParams(PlaybackSpeedParams *playbackSpeedParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackSpeedParams(PlaybackSpeedParams *playbackSpeedParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackJumpParams(PlaybackJumpParams *playbackJumpParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackJumpParams(PlaybackJumpParams *playbackJumpParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackCommonParams(PlaybackCommonParams *playbackCommonParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackCommonParams(PlaybackCommonParams *playbackCommonParams);

extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackCurrentLoopLine(uint32_t *currentLoop, uint32_t *currentLine, uint32_t *currentLineState);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackInstantCmd(enum PlaybackInstantCmd cmd);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackBufferSize(uint32_t *bufferSize);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackBufferCmd(PlaybackBufferCmd *playbackBufferCmd);

// CP functions
extern "C" DOBOTDLLSHARED_EXPORT int SetCPParams(CPParams *cpParams);
extern "C" DOBOTDLLSHARED_EXPORT int GetCPParams(CPParams *cpParams);

extern "C" DOBOTDLLSHARED_EXPORT int GetCPCurrentLoopLine(uint32_t *currentLoop, uint32_t *currentLine, uint32_t *currentLineState);
extern "C" DOBOTDLLSHARED_EXPORT int setCPInstantCmd(enum CPInstantCmd cmd);
extern "C" DOBOTDLLSHARED_EXPORT int GetCPBufferSize(uint32_t *bufferSize);
extern "C" DOBOTDLLSHARED_EXPORT int SetCPBufferCmd(CPBufferCmd *cpBufferCmd);

// User params
extern "C" DOBOTDLLSHARED_EXPORT int GetUserParams(UserParams *userParams);

#endif // DOBOTDLL_H
