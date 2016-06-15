/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           DataLinkLayer.cpp
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
#include "DataLinkLayer.h"
#include "HardwareLayer.h"
#include <string.h>

// 理论上这个是独立的队列！！！

/*********************************************************************************************************
** Function name:       MsgBuffRead
** Descriptions:        Read one msg from the msg buffer
** Input parameters:    pMsgBuffCtrl:Msg buff structure pointer
** Output parameters:   Message:Msg structure pointer
** Returned value:      Success read return TRUE,Otherwise return FALSE
*********************************************************************************************************/
unsigned char MsgBuffRead(MsgBuffCtrl* pMsgBuffCtrl,Message* pMessage)
{
    if(pMsgBuffCtrl->writeIndex == pMsgBuffCtrl->readIndex) {
        return FALSE;
    }
    *pMessage = pMsgBuffCtrl->addr[pMsgBuffCtrl->readIndex];
    pMsgBuffCtrl->readIndex = (pMsgBuffCtrl->readIndex+1) % pMsgBuffCtrl->len;

    return TRUE;
}

/*********************************************************************************************************
** Function name:       MsgBuffWrite
** Descriptions:        Write one msg to the msg buffer
** Input parameters:    pMsgBuffCtrl:Msg buff structure pointer;Message:Msg structure pointer
** Output parameters:   None
** Returned value:      Success read return TRUE,Otherwise return FALSE
*********************************************************************************************************/
unsigned char MsgBuffWrite(MsgBuffCtrl* pMsgBuffCtrl, const Message* pMessage)
{
    if((pMsgBuffCtrl->writeIndex+1)%pMsgBuffCtrl->len == pMsgBuffCtrl->readIndex) {
        return FALSE;
    }
    pMsgBuffCtrl->addr[pMsgBuffCtrl->writeIndex] = *pMessage;
    pMsgBuffCtrl->writeIndex = (pMsgBuffCtrl->writeIndex + 1) % pMsgBuffCtrl->len;

    return TRUE;
}

/*********************************************************************************************************
** Function name:       ChecksumCalc
** Descriptions:        Calculate the checksum
** Input parameters:    Source and the length
** Output parameters:   None
** Returned value:      Check sum
*********************************************************************************************************/
static uint8_t ChecksumCalc(uint8_t *src, uint32_t len)
{
    uint8_t checksum = 0;

    for(uint32_t index = 0; index < len; index++) {
        checksum += src[index];
    }
    return checksum;
}

/*********************************************************************************************************
** Function name:       Message2Frame
** Descriptions:        Message to frame
** Input parameters:    Transform the message to frame
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static void Message2Frame(Frame *frame, Message *message)
{
    // Payload length = id + paramsLen + params
    frame->payloadLen = 2 + message->paramsLen;
    memcpy(frame->payload, message, frame->payloadLen);
    // Only the payload checksum
    frame->payload[frame->payloadLen] = ChecksumCalc(frame->payload, frame->payloadLen);
}

/*********************************************************************************************************
** Function name:       Frame2Message
** Descriptions:        Frame to message
** Input parameters:    Transform the frame to message
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static void Frame2Message(Message *message, Frame *frame)
{
    memcpy(message, frame->payload, frame->payloadLen);
}

/*********************************************************************************************************
** Function name:       EscapeCharacter
** Descriptions:        Escape the frame data
** Input parameters:
        dst:After escape data buff
        src:before escape data buff
        srcLen:before escape data length
** Output parameters:   None
** Returned value:      After escape data length
*********************************************************************************************************/
uint8_t EscapeCharacter(uint8_t *dst, uint8_t *src, uint8_t srcLen)
{
    if (srcLen == 0) {
        return 0;
    }
    uint8_t srcIndex = 0, dstIndex = 0;

    for(srcIndex = 0; srcIndex < srcLen; srcIndex++) {
        if (src[srcIndex] == FRAME_END) {
            dst[dstIndex++] = TRANS_END_DB;
            dst[dstIndex++] = TRANS_END_DC;
        } else if (src[srcIndex] == FRAME_DB) {
            dst[dstIndex++] = TRANS_DB_DB;
            dst[dstIndex++] = TRANS_DB_DD;
        } else {
            dst[dstIndex++] = src[srcIndex];
        }
    }
    dst[dstIndex++] = FRAME_END;

    return dstIndex;
}

/*********************************************************************************************************
** Function name:       RevEscapeCharacter
** Descriptions:        revescape data into the frame
** Input parameters:
        dst:After escape data buff
        src:before escape data buff
        srcLen:before escape data length
** Output parameters:   None
** Returned value:      Before escape data length
*********************************************************************************************************/
uint8_t RevEscapeCharacter(uint8_t *dst, uint8_t *src, uint8_t srcLen)
{
    uint8_t srcIndex = 0, dstIndex = 0;
    uint8_t lastData = src[srcIndex];

    for(srcIndex = 0; srcIndex < srcLen; srcIndex++) {
        if(lastData == TRANS_END_DB) {
            if(src[srcIndex] == TRANS_END_DC) {
                dst[dstIndex++] = FRAME_END;
            } else if (src[srcIndex] == TRANS_DB_DD) {
                dst[dstIndex++] = FRAME_DB;
            }
        } else {
            if (src[srcIndex] != TRANS_END_DB) {
                dst[dstIndex++] = src[srcIndex];
            }
        }
        lastData = src[srcIndex];
    }
    return dstIndex;
}

/*********************************************************************************************************
** Function name:       Receive
** Descriptions:        Receive process
** Input parameters:
        comCtrl:Comm platform structure pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static void Receive(ComCtrl *comCtrl)
{
    uint32_t iAfterRevEscapeLen;
    Message message;
    Frame frame;

    BeforeRevEscape beforeRevEscapeFrame;
    // 拷贝数据进行处理
    beforeRevEscapeFrame.len = ReadDataFromCommBuff(&(comCtrl->hardwareCtrl.rxComBuffCtrl), beforeRevEscapeFrame.data, sizeof(beforeRevEscapeFrame.data));

    for(beforeRevEscapeFrame.index = 0; beforeRevEscapeFrame.index < beforeRevEscapeFrame.len;beforeRevEscapeFrame.index++) {
        if (beforeRevEscapeFrame.data[beforeRevEscapeFrame.index] == FRAME_END) {
            iAfterRevEscapeLen = RevEscapeCharacter(comCtrl->backupFrame->data, comCtrl->backupFrame->data, comCtrl->backupFrame->len);
            if(iAfterRevEscapeLen < 4 && 0 != iAfterRevEscapeLen) {
                // Shortest: len id functionPayloadLen checkSum
            } else if(comCtrl->backupFrame->data[0] == iAfterRevEscapeLen - 2) {
                // Copy the frame
                memcpy((uint8_t *)&frame, comCtrl->backupFrame->data, iAfterRevEscapeLen);
                // Check the checksum
                if(frame.payload[frame.payloadLen] == ChecksumCalc(frame.payload, frame.payloadLen)) {
                    // Transform the frame to message
                    Frame2Message(&message, &frame);

                    if(!MsgBuffWrite(&(comCtrl->appCtrl.rxMsgBuffCtrl), &message)) {
                        // Message buffer full now!
                        break;
                    }
                } else {
                    // Checksum error!
                }
            }
            comCtrl->backupFrame->len = 0;
        } else {
            comCtrl->backupFrame->data[comCtrl->backupFrame->len++] = beforeRevEscapeFrame.data[beforeRevEscapeFrame.index];
            if (comCtrl->backupFrame->len >= sizeof(comCtrl->backupFrame->data)) {
                comCtrl->backupFrame->len = 0;
                // Frame backup buffer full error!
            }
        }
    }
}

/*********************************************************************************************************
** Function name:       Send
** Descriptions:        Send process
** Input parameters:
        comCtrl:Comm platform structure pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static void Send(ComCtrl *comCtrl)
{
    Message message;
    Frame frame;
    uint8_t buffer[FRAME_SIZE * 2] = {0};

    // 提前判断是否有空间可以放一帧数据！
    while(IsCommEnoughForOneFrame(&(comCtrl->hardwareCtrl.txComBuffCtrl))) {
        if (MsgBuffRead(&(comCtrl->appCtrl.txMsgBuffCtrl), &message) == FALSE) {
            break;
        }
        Message2Frame(&frame, &message);
        uint32_t len = EscapeCharacter(buffer, (uint8_t*)&frame, frame.payloadLen + 2);
        // 已经提前判断是否可以放数据了！
        WriteDataIntoCommBuff(&(comCtrl->hardwareCtrl), buffer, len);
    }
}

/*********************************************************************************************************
** Function name:       ComDataProcess
** Descriptions:        Comm data process
** Input parameters:
        comCtrl:Comm platform structure pointer
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void ComDataProcess(ComCtrl *pcomctrl)
{
    Receive(pcomctrl);
    Send(pcomctrl);
}
