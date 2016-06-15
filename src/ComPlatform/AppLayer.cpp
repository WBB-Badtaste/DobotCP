/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           AppLayer.cpp
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
#include "AppLayer.h"
#include "DataLinkLayer.h"

/*********************************************************************************************************
** Function name:       ProtocolGetMessage
** Descriptions:        Get message from message queue
** Input parameters:    ComCtrl pointer, message pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
uint32_t ProtocolGetMessage(ComCtrl *comCtrl, Message *message)
{
    MsgBuffCtrl *rxMsgBufCtrl = &comCtrl->appCtrl.rxMsgBuffCtrl;

    return MsgBuffRead(rxMsgBufCtrl, message);
}

/*********************************************************************************************************
** Function name:       ProtocolPostMessage
** Descriptions:        Send message to message queue
** Input parameters:    ComCtrl pointer, message pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void ProtocolPostMessage(ComCtrl *comCtrl, const Message *message)
{
    MsgBuffCtrl *txMsgBufCtrl = &comCtrl->appCtrl.txMsgBuffCtrl;

    MsgBuffWrite(txMsgBufCtrl, message);
}
