#ifndef DOBOTDLL_H
#define DOBOTDLL_H

#include "dobotdll_global.h"
#include "DobotType.h"

extern "C" DOBOTDLLSHARED_EXPORT int ConnectDobot(int baudrate);
extern "C" DOBOTDLLSHARED_EXPORT void DisconnectDobot(void);
extern "C" DOBOTDLLSHARED_EXPORT void PeriodicTask(void);
extern "C" DOBOTDLLSHARED_EXPORT int SetCmdTimeout(uint32_t cmdTimeout);
extern "C" DOBOTDLLSHARED_EXPORT int DobotExec(void);

// Queued command control!
extern "C" DOBOTDLLSHARED_EXPORT int SetQueuedCmdExecMode(QueuedCmdExecMode queuedCmdExecMode);
extern "C" DOBOTDLLSHARED_EXPORT int GetQueuedCmdExecMode(QueuedCmdExecMode *queuedCmdExecMode);
extern "C" DOBOTDLLSHARED_EXPORT int GetQueuedCmdExecState(uint32_t *queuedCmdIndex, bool *isFinished);
extern "C" DOBOTDLLSHARED_EXPORT int SetQueuedCmdExecCtrl(QueuedCmdExecCtrl queuedCmdExecCtrl);
extern "C" DOBOTDLLSHARED_EXPORT int SetQueuedCmdOfflineLoopLine(uint32_t totalLoop, uint32_t linePerLoop);

// Device information
extern "C" DOBOTDLLSHARED_EXPORT int SetDeviceSN(const char *deviceSN);
extern "C" DOBOTDLLSHARED_EXPORT int GetDeviceSN(char *deviceSN);

extern "C" DOBOTDLLSHARED_EXPORT int SetDeviceName(const char *deviceName);
extern "C" DOBOTDLLSHARED_EXPORT int GetDeviceName(char *deviceName);

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
// User params
extern "C" DOBOTDLLSHARED_EXPORT int GetUserParams(UserParams *userParams);

// HOME
extern "C" DOBOTDLLSHARED_EXPORT int SetHOMEParams(HOMEParams *homeParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetHOMEParams(HOMEParams *homeParams);

extern "C" DOBOTDLLSHARED_EXPORT int SetHOMECmd(HOMECmd *homeCmd, bool isQueued, uint32_t queuedCmdIndex);

// Handheld teach
extern "C" DOBOTDLLSHARED_EXPORT int SetHandHeldTeachEnabled(bool isEnabled);
extern "C" DOBOTDLLSHARED_EXPORT int GetHandHeldTeachEnabled(bool *isEnabled);

extern "C" DOBOTDLLSHARED_EXPORT int GetHasHandHeldTeachPoint(bool *isAvailable);

// Jog functions
extern "C" DOBOTDLLSHARED_EXPORT int SetJogJointParams(JointJogParams *jointJogParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogJointParams(JointJogParams *jointJogParams);

extern "C" DOBOTDLLSHARED_EXPORT int SetJogCoordinateParams(CoordinateJogParams *coordinateJogParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogCoordinateParams(CoordinateJogParams *coordinateJogParams);

extern "C" DOBOTDLLSHARED_EXPORT int SetJogCommonParams(JogCommonParams *jogCommonParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetJogCommonParams(JogCommonParams *jogCommonParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetJogCmd(JogCmd *jogCmd, bool isQueued, uint32_t queuedCmdIndex);

// Playback functions
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackSpeedParams(PlaybackSpeedParams *playbackSpeedParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackSpeedParams(PlaybackSpeedParams *playbackSpeedParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackJumpParams(PlaybackJumpParams *playbackJumpParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackJumpParams(PlaybackJumpParams *playbackJumpParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackCommonParams(PlaybackCommonParams *playbackCommonParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetPlaybackCommonParams(PlaybackCommonParams *playbackCommonParams);

extern "C" DOBOTDLLSHARED_EXPORT int SetPlaybackCmd(PlaybackCmd *playbackCmd, bool isQueued, uint32_t queuedCmdIndex);

// CP functions
extern "C" DOBOTDLLSHARED_EXPORT int SetCPParams(CPParams *cpParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetCPParams(CPParams *cpParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetCPCmd(CPCmd *cpCmd, bool isQueued, uint32_t queuedCmdIndex);

// TRACK
extern "C" DOBOTDLLSHARED_EXPORT int SetTRACKParams(TRACKParams *trackParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetTRACKParams(TRACKParams *trackParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetTRACKCmd(TRACKCmd *trackCmd, bool isQueued, uint32_t queuedCmdIndex);

// WAIT
extern "C" DOBOTDLLSHARED_EXPORT int SetWAITParams(WAITParams *waitParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetWAITParams(WAITParams *waitParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetWAITCmd(WAITCmd *waitCmd, bool isQueued, uint32_t queuedCmdIndex);

// TRIG
extern "C" DOBOTDLLSHARED_EXPORT int SetTRIGParams(TRIGParams *trigParams, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetTRIGParams(TRIGParams *trigParams);
extern "C" DOBOTDLLSHARED_EXPORT int SetTRIGCmd(TRIGCmd *trigCmd, bool isQueued, uint32_t queuedCmdIndex);

// I/O
extern "C" DOBOTDLLSHARED_EXPORT int SetGPIOFunction(uint32_t index, GPIOFunction gpioFunction, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetGPIOFunction(uint32_t index, GPIOFunction *gpioFunction);

extern "C" DOBOTDLLSHARED_EXPORT int SetOutputIOState(uint32_t index, bool isOn, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetOutputIOState(uint32_t index, bool *isOn);

extern "C" DOBOTDLLSHARED_EXPORT int SetPWMValue(uint32_t index, float pwm, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetPWMValue(uint32_t index, float *pwm);

extern "C" DOBOTDLLSHARED_EXPORT int GetInputIOState(uint32_t index, bool *isOn);
extern "C" DOBOTDLLSHARED_EXPORT int GetADCValue(uint32_t index, float *adc);

extern "C" DOBOTDLLSHARED_EXPORT int SetGripperValue(float gripper, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetGripperValue(float *gripper);

extern "C" DOBOTDLLSHARED_EXPORT int SetStepperValue(float stepper, bool isQueued, uint32_t queuedCmdIndex);
extern "C" DOBOTDLLSHARED_EXPORT int GetStepperValue(float *stepper);

#endif // DOBOTDLL_H
