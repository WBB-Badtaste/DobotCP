/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           DataLinkLayer.h
** Latest modified Date:2016-03-30
** Latest Version:      V2.0.0
** Descriptions:        Transform from c file to cpp file
**
**--------------------------------------------------------------------------------------------------------
** Created by:
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:        DataLink ComPlatForm's message code or decode
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/*********************************************************************************************************
** Function name:       MsgBuffRead
** Descriptions:        Read one msg from the msg buffer
** Input parameters:    pMsgBuffCtrl:Msg buff structure pointer
** Output parameters:   Message:Msg structure pointer
** Returned value:      Success read return TRUE,Otherwise return FALSE
*********************************************************************************************************/
extern uint8_t MsgBuffRead(MsgBuffCtrl* pMsgBuffCtrl, Message *pMessage);

/*********************************************************************************************************
** Function name:       MsgBuffWrite
** Descriptions:        Write one msg to the msg buffer
** Input parameters:    pMsgBuffCtrl:Msg buff structure pointer;Message:Msg structure pointer
** Output parameters:   None
** Returned value:      Success read return TRUE,Otherwise return FALSE
*********************************************************************************************************/
extern uint8_t MsgBuffWrite(MsgBuffCtrl* pMsgBuffCtrl, const Message *pMessage);

/*********************************************************************************************************
** Function name:       ComDataProcess
** Descriptions:        Comm data process
** Input parameters:
        Pcomctrl:Comm platform structure pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
extern void ComDataProcess(ComCtrl *pcomctrl);

#ifdef __cplusplus
}
#endif
#endif
