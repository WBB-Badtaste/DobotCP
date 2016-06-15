/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           HardwareLayer.cpp
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
#include "HardwareLayer.h"

/*********************************************************************************************************
** Function name:       IsCommEnoughForOneFrame
** Descriptions:        judge serial port buff is enough to Send one frame data or not
** Input parameters:
        pCommBuffCtrl:serial port buff structure pointer
** Output parameters:   None
** Returned value:      enough return TRUE,Otherwise return FALSE
*********************************************************************************************************/
uint8_t IsCommEnoughForOneFrame(ComBuffCtrl* pCommBuffCtrl)
{
    uint32_t len = 0;

    if(pCommBuffCtrl->readIndex <= pCommBuffCtrl->writeIndex) {
        len = pCommBuffCtrl->len - (pCommBuffCtrl->writeIndex - pCommBuffCtrl->readIndex) - 1;
    } else {
        len = (pCommBuffCtrl->readIndex - pCommBuffCtrl->writeIndex) - 1;
    }
    return len >= FRAME_SIZE * 2;
}

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
uint32_t ReadDataFromCommBuff(ComBuffCtrl *pCommBuffCtrl, uint8_t *pBuff, uint32_t len)
{
    uint32_t iDataLen = 0;
    uint32_t iCanReadLen = 0;
    uint32_t iTempWriteIndex = pCommBuffCtrl->writeIndex;
    uint32_t iTempReadIndex = pCommBuffCtrl->readIndex;
    uint32_t iLen = pCommBuffCtrl->len;

    if(iTempReadIndex <= iTempWriteIndex) {
        iDataLen = iTempWriteIndex-iTempReadIndex;
    } else {
        iDataLen = iLen-(iTempReadIndex-iTempWriteIndex);
    }

    if(len <= iDataLen) {
        iCanReadLen = len;
    } else {
        iCanReadLen = iDataLen;
    }

    for(uint32_t index = 0; index < iCanReadLen; index++) {
        pBuff[index] = pCommBuffCtrl->addr[iTempReadIndex];
        iTempReadIndex = (iTempReadIndex + 1) % iLen;
    }
    pCommBuffCtrl->readIndex = iTempReadIndex;

    return iCanReadLen;
}

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
uint32_t WriteDataIntoCommBuff(HardwareCtrl* pHardware,uint8_t* pBuff,uint32_t iDataLen)
{
    uint32_t canWriteSize;

    if(pHardware->txComBuffCtrl.readIndex <= pHardware->txComBuffCtrl.writeIndex) {
        canWriteSize = pHardware->txComBuffCtrl.len-(pHardware->txComBuffCtrl.writeIndex-pHardware->txComBuffCtrl.readIndex)-1;
    } else {
        canWriteSize = (pHardware->txComBuffCtrl.readIndex - pHardware->txComBuffCtrl.writeIndex) - 1;
    }
    if(canWriteSize >= iDataLen) {
        canWriteSize = iDataLen;
    }

    for(uint32_t index = 0; index < canWriteSize; index++) {
        pHardware->txComBuffCtrl.addr[pHardware->txComBuffCtrl.writeIndex] = pBuff[index];

        pHardware->txComBuffCtrl.writeIndex = (pHardware->txComBuffCtrl.writeIndex + 1) % pHardware->txComBuffCtrl.len;
    }
    if(pHardware->send) {
        pHardware->send(pHardware->uartId);
    }
    return canWriteSize;
}

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
uint32_t WriteDataIntoRxBuff(HardwareCtrl* pHardware,uint8_t* pRxBuff,uint32_t iRxDataLen)
{
    uint32_t canWriteSize;

    if(pHardware->rxComBuffCtrl.readIndex <= pHardware->rxComBuffCtrl.writeIndex) {
        canWriteSize = pHardware->rxComBuffCtrl.len - (pHardware->rxComBuffCtrl.writeIndex - pHardware->rxComBuffCtrl.readIndex) - 1;
    } else {
        canWriteSize = (pHardware->rxComBuffCtrl.readIndex - pHardware->rxComBuffCtrl.writeIndex) - 1;
    }
    if(canWriteSize >= iRxDataLen) {
        canWriteSize = iRxDataLen;
    }
    for(uint32_t index = 0; index < canWriteSize; index++) {
        pHardware->rxComBuffCtrl.addr[pHardware->rxComBuffCtrl.writeIndex] = pRxBuff[index];

        pHardware->rxComBuffCtrl.writeIndex = (pHardware->rxComBuffCtrl.writeIndex + 1) % pHardware->rxComBuffCtrl.len;
    }

    return canWriteSize;
}
