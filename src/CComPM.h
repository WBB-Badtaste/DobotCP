#ifndef CCOMPM_H
#define CCOMPM_H

#include <QObject>
#include "PlatformDef.h"

class CComApp;

class CComPM : public QObject
{
    Q_OBJECT
public:
    CComPM(QObject *parent = 0);
    ~CComPM();
    void packMessage(const Message &message);
signals:
    void messageReady(const Message &message);
private:
    void initConnect(void);
private:
    CComApp *comApp;
};

#endif // CCOMPM_H
