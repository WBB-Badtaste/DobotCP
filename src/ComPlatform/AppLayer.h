/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           AppLayer.h
** Latest modified Date:2016-03-30
** Latest Version:      V2.0.0
** Descriptions:        Transform from c file to cpp file
**
**--------------------------------------------------------------------------------------------------------
** Created by:
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef APPLICATION_LAYER_H
#define APPLICATION_LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/*********************************************************************************************************
** Function name:       ProtocolGetMessage
** Descriptions:        Get message from message queue
** Input parameters:    ComCtrl pointer, message pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
extern uint32_t ProtocolGetMessage(ComCtrl *const comCtrl, Message *message);

/*********************************************************************************************************
** Function name:       ProtocolPostMessage
** Descriptions:        Send message to message queue
** Input parameters:    ComCtrl pointer, message pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
extern void ProtocolPostMessage(ComCtrl *const comCtrl, const Message *message);

#ifdef __cplusplus
}
#endif

#endif
