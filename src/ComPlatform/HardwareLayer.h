/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           HardwareLayer.h
** Latest modified Date:2016-03-30
** Latest Version:      V2.0.0
** Descriptions:        Transform from c file to cpp file
**
**--------------------------------------------------------------------------------------------------------
** Created by:
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:        Hardware layer
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef HARDWARELAYER_H
#define HARDWARELAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/*********************************************************************************************************
** Function name:       IsCommEnoughForOneFrame
** Descriptions:        judge serial port buff is enough to Send one frame data or not
** Input parameters:
        pCommBuffCtrl:serial port buff structure pointer
** Output parameters:   None
** Returned value:      enough return TRUE,Otherwise return FALSE
*********************************************************************************************************/
extern unsigned char IsCommEnoughForOneFrame(ComBuffCtrl* pCommBuffCtrl);

/*********************************************************************************************************
** Function name:       ReadDataFromCommBuff
** Descriptions:        read data from serial buff
** Input parameters:
        pCommBuffCtrl:serial buff structure pointer
        pBuff:data buffer
        len:data length
** Output parameters:   None
** Returned value:      return the length of the read data
*********************************************************************************************************/
extern unsigned int ReadDataFromCommBuff(ComBuffCtrl *pCommBuffCtrl,unsigned char* pBuff,unsigned int len);

/*********************************************************************************************************
** Function name:       WriteDataIntoCommBuff
** Descriptions:        write data into serial buff
** Input parameters:
        pHardware:hardware structure pointer
        pBuff:data buffer
        len:data length
** Output parameters:   None
** Returned value:      return the length of the written data
*********************************************************************************************************/
extern unsigned int WriteDataIntoCommBuff(HardwareCtrl* pHardware,unsigned char* pBuff,unsigned int iDataLen);

/*********************************************************************************************************
** Function name:       WriteDataIntoRxBuff
** Descriptions:        write data into serial com Rx buff
** Input parameters:
        pHardware:  hardware structure pointer
        pBuff:      data  buffer
        len:        data length
** Output parameters:   None
** Returned value:      return the real length of the written data
*********************************************************************************************************/
extern unsigned int WriteDataIntoRxBuff(HardwareCtrl* pHardware,unsigned char* pRxBuff,unsigned int iRxDataLen);

#ifdef __cplusplus
}
#endif

#endif
