#include "CComData.h"
#include <QScopedArrayPointer>
#include <QScopedPointer>
#include <QMutex>

class CComDataPrivate
{
public:
    QScopedArrayPointer<Message> TxMsgBuffM4;
    QScopedArrayPointer<Message> RxMsgBuffM4;
    QScopedArrayPointer<quint8> TxComBuffM4;
    QScopedArrayPointer<quint8> RxComBuffM4;
    QScopedArrayPointer<AfterRevEscape> RevBackupFrameM4;
    QScopedArrayPointer<ComCtrl> CommToM4;
    QScopedPointer<QMutex> mutex;
public:
    CComDataPrivate()
    {
        // Init the buffer
        TxMsgBuffM4.reset(new Message[COM_TX_MSGBUFF_SIZE]);
        memset((void *)&TxMsgBuffM4[0], 0, sizeof(Message) * COM_TX_MSGBUFF_SIZE);
        RxMsgBuffM4.reset(new Message[COM_RX_MSGBUFF_SIZE]);
        memset((void *)&RxMsgBuffM4[0], 0, sizeof(Message) * COM_RX_MSGBUFF_SIZE);

        TxComBuffM4.reset(new quint8[COM_TX_COMBUFF_SIZE]);
        memset((void *)&TxComBuffM4[0], 0, sizeof(quint8) * COM_TX_COMBUFF_SIZE);
        RxComBuffM4.reset(new quint8[COM_RX_COMBUFF_SIZE]);
        memset((void *)&RxComBuffM4[0], 0, sizeof(quint8) * COM_RX_COMBUFF_SIZE);

        RevBackupFrameM4.reset(new AfterRevEscape[1]);
        memset((void *)&RevBackupFrameM4[0], 0, sizeof(AfterRevEscape));

        CommToM4.reset(new ComCtrl[1]);

        memset((void *)&CommToM4[0], 0, sizeof(ComCtrl));
        CommToM4[0].hardwareCtrl.txComBuffCtrl.writeIndex = 0;
        CommToM4[0].hardwareCtrl.txComBuffCtrl.readIndex = 0;
        CommToM4[0].hardwareCtrl.txComBuffCtrl.len = COM_TX_COMBUFF_SIZE;
        CommToM4[0].hardwareCtrl.txComBuffCtrl.addr = &TxComBuffM4[0];

        CommToM4[0].hardwareCtrl.rxComBuffCtrl.writeIndex = 0;
        CommToM4[0].hardwareCtrl.rxComBuffCtrl.readIndex = 0;
        CommToM4[0].hardwareCtrl.rxComBuffCtrl.len = COM_RX_COMBUFF_SIZE;
        CommToM4[0].hardwareCtrl.rxComBuffCtrl.addr = &RxComBuffM4[0];

        CommToM4[0].appCtrl.txMsgBuffCtrl.writeIndex = 0;
        CommToM4[0].appCtrl.txMsgBuffCtrl.readIndex = 0;
        CommToM4[0].appCtrl.txMsgBuffCtrl.len = COM_TX_MSGBUFF_SIZE;
        CommToM4[0].appCtrl.txMsgBuffCtrl.addr = &TxMsgBuffM4[0];

        CommToM4[0].appCtrl.rxMsgBuffCtrl.writeIndex = 0;
        CommToM4[0].appCtrl.rxMsgBuffCtrl.readIndex = 0;
        CommToM4[0].appCtrl.rxMsgBuffCtrl.len = COM_RX_MSGBUFF_SIZE;
        CommToM4[0].appCtrl.rxMsgBuffCtrl.addr = &RxMsgBuffM4[0];

        CommToM4[0].backupFrame = &RevBackupFrameM4[0];

        mutex.reset(new QMutex(QMutex::Recursive));
    }
    ~CComDataPrivate()
    {

    }
};

CComData::CComData(QObject *parent)
  : QObject(parent),
    privateData(new CComDataPrivate())
{

}

ComCtrl &CComData::operator*() const
{
    return (ComCtrl &)privateData->CommToM4[0];
}

ComCtrl *CComData::operator->() const
{
    return (ComCtrl *)&(privateData->CommToM4[0]);
}

ComCtrl *CComData::data() const
{
    return (ComCtrl *)&(privateData->CommToM4[0]);
}

QMutex *CComData::mutex() const
{
    return privateData->mutex.data();
}

CComData::~CComData()
{
    delete privateData;
}
