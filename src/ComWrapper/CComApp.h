#ifndef CCOMAPP_H
#define CCOMAPP_H

#include <QObject>
#include "PlatformDef.h"

class QIODevice;
class CComData;
class CComDataLink;
class CComHardware;

class CComApp : public QObject
{
    Q_OBJECT
public:
    CComApp(QIODevice *device, QObject *parent);
    ~CComApp();
protected:
    void timerEvent(QTimerEvent *event);
signals:
    void messageReady(const Message &message);
public slots:
    void postMessage(const Message &message);
private:
    QIODevice *const device;
    CComData *const comData;
    CComDataLink *const comDataLink;
    CComHardware *const comHardware;
    int appTimer;
};

#endif // CCOMAPP_H
