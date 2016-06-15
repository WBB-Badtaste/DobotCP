#include "CComApp.h"
#include <QIODevice>
#include <QTimerEvent>
#include <QMutexLocker>
#include "CComData.h"
#include "CComDataLink.h"
#include "CComHardware.h"
#include "AppLayer.h"

CComApp::CComApp(QIODevice *device, QObject *parent)
  : QObject(parent),
    device(device),
    comData(new CComData(this)),
    comDataLink(new CComDataLink(comData, this)),
    comHardware(new CComHardware(device, comData, this))
{
    appTimer = startTimer(20);
}

void CComApp::timerEvent(QTimerEvent *event)
{
    int timerId = event->timerId();

    if (timerId == appTimer) {
        Message message;
        QMutexLocker locker(comData->mutex());
        while (ProtocolGetMessage(comData->data(), &message)) {
            emit messageReady(message);
        }
    } else {
        QObject::timerEvent(event);
    }
}

void CComApp::postMessage(const Message &message)
{
    QMutexLocker locker(comData->mutex());
    ProtocolPostMessage(comData->data(), &message);
}

CComApp::~CComApp()
{

}
