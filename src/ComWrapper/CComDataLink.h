#ifndef CCOMDATALINK_H
#define CCOMDATALINK_H

#include <QObject>

class CComData;

class CComDataLink : public QObject
{
    Q_OBJECT
public:
    CComDataLink(CComData *comData, QObject *parent = 0);
    ~CComDataLink();
protected:
    void timerEvent(QTimerEvent *event);
private:
    CComData *const comData;
    int timer;
};

#endif // CCOMDATALINK_H
