#include "CComHardware.h"
#include <QIODevice>
#include <QTimerEvent>
#include <QMutexLocker>
#include "CComData.h"
#include "HardwareLayer.h"

CComHardware::CComHardware(QIODevice *device, CComData *comData, QObject *parent)
  : QObject(parent),
    device(device),
    comData(comData),
    writeBuffer(new QScopedArrayPointer<quint8>),
    readyWrite(true)
{
    writeBuffer->reset(new quint8[COM_TX_COMBUFF_SIZE]);
    connect(device, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(device, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    timer = startTimer(0);
}

void CComHardware::timerEvent(QTimerEvent *event)
{
    int timerId = event->timerId();

    if (timerId == timer) {
        if (not readyWrite) {
            return;
        }
        ComCtrl *comCtrl = comData->data();
        quint32 len = COM_TX_COMBUFF_SIZE;
        do {
            QMutexLocker locker(comData->mutex());
            len = ReadDataFromCommBuff(&(comCtrl->hardwareCtrl.txComBuffCtrl), writeBuffer->data(), len);
        } while (0);
        if (len) {
            readyWrite = false;
            device->write((const char *)writeBuffer->data(), len);
        }
    } else {
        QObject::timerEvent(event);
    }
}

void CComHardware::onReadyRead(void)
{
    HardwareCtrl *hwCtrl = &(comData->data()->hardwareCtrl);
    const QByteArray array = device->readAll();
    QMutexLocker locker(comData->mutex());
    WriteDataIntoRxBuff(hwCtrl, (unsigned char *)array.data(), array.size());
}

void CComHardware::onBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes)
    readyWrite = true;
}

CComHardware::~CComHardware()
{

}
