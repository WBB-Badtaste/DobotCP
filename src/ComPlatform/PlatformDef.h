/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           PlatformDef.h
** Latest modified Date:2016-03-30
** Latest Version:      V1.0.0
** Descriptions:        communication platform common struct define
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Liu Zhufu
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef PLATFORMDEF_H
#define PLATFORMDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

/*********************************************************************************************************
** Platform independent macro definition
*********************************************************************************************************/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*********************************************************************************************************
** Protocol specific definitions
*********************************************************************************************************/
#define PROTOCOL_VER    0x01                                            //The version number

/*********************************************************************************************************
** Protocol buffer definitions
*********************************************************************************************************/
#define  COM_TX_COMBUFF_SIZE 2048                                       // Hardware send buffer
#define  COM_RX_COMBUFF_SIZE 2048                                       // Hardware receive buffer
#define  COM_TX_MSGBUFF_SIZE 64                                         // Message send queue buffer
#define  COM_RX_MSGBUFF_SIZE 64                                         // Message receive queue buffer

#define AFTER_ESCAPE_DATA_SIZE      (MESSAGE_DATA_SIZE * 2)

/*********************************************************************************************************
** Protocol escape character definitions
*********************************************************************************************************/
#define FRAME_END           (0xC0)
#define FRAME_DB            (0xDB)

#define TRANS_END_DB        (0xDB)
#define TRANS_END_DC        (0xDC)

#define TRANS_DB_DB         (0xDB)
#define TRANS_DB_DD         (0xDD)

/*********************************************************************************************************
** Protocol specific structure definitions
*********************************************************************************************************/

// Message structure, which is for application layer
#define MESSAGE_PAYLOAD_SIZE        62

typedef struct tagMessage {
    uint8_t id;
    uint8_t paramsLen;
    uint8_t params[MESSAGE_PAYLOAD_SIZE];
}Message;

#define MESSAGE_SIZE    sizeof(Message)

// Frame structure, which is for datalink layer
typedef struct tagFrame {
    uint8_t payloadLen;
    uint8_t payload[MESSAGE_SIZE + 1];                                     // Contains the checksum
}Frame;

#define FRAME_SIZE  sizeof(Frame)

typedef struct tagComBuffCtrl {                                         // Ring buffer for hardware!
    // Static property
    uint8_t *addr;
    uint32_t len;
    // Dynamic property
    uint32_t writeIndex;
    uint32_t readIndex;
}ComBuffCtrl;

//Hardware layer information structure
typedef struct tagHardwareComCtrl {
    ComBuffCtrl txComBuffCtrl;
    ComBuffCtrl rxComBuffCtrl;
    uint8_t uartId;
    uint8_t reserve[3];
    void (*send)(uint8_t uartId);
}HardwareCtrl;

// Data-link layer information

typedef struct tagAfterRevEscape {
    uint8_t len;
    uint8_t data[FRAME_SIZE * 2];
}AfterRevEscape;

typedef struct tagBeforeRevEscape {
    uint8_t index;                                                //Record the position of the current analytical
    uint8_t len;                                                  //The length of the data before escape
    uint8_t data[FRAME_SIZE * 2];                        //the data before escape
}BeforeRevEscape;

//application layer Message structure buff
typedef struct  tagMsgBuffCtrl {
    // Static property
    Message* addr;
    uint32_t len;
    // Dynamic property
    uint32_t writeIndex;
    uint32_t readIndex;
}MsgBuffCtrl;

typedef struct tagApplicationCtrl
{
    MsgBuffCtrl txMsgBuffCtrl;    //Message send buffer
    MsgBuffCtrl rxMsgBuffCtrl;    //Message receive buffer
}AppCtrl;

//Communication platform structure
typedef struct tagComCtrl {
    HardwareCtrl   hardwareCtrl;    // Ring buffer!
    // 接收的原始的经过转义的，不包含0xC0
    AfterRevEscape*   backupFrame;
    AppCtrl        appCtrl;
}ComCtrl;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
#endif
