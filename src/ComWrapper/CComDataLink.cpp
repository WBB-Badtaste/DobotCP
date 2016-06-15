#include "CComDataLink.h"
#include <QTimerEvent>
#include <QMutexLocker>
#include "CComData.h"
#include "DataLinkLayer.h"

CComDataLink::CComDataLink(CComData *comData, QObject *parent)
  : QObject(parent),
    comData(comData)
{
    timer = startTimer(0);
}

void CComDataLink::timerEvent(QTimerEvent *event)
{
    int timerId = event->timerId();

    if (timerId == timer) {
        QMutexLocker locker(comData->mutex());
        ComDataProcess(comData->data());
    } else {
        QObject::timerEvent(event);
    }
}

CComDataLink::~CComDataLink()
{

}
