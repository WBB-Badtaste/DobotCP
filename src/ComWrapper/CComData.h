#ifndef CCOMDATA_H
#define CCOMDATA_H

#include <QObject>
#include "PlatformDef.h"

class CComDataPrivate;
class QMutex;

class CComData : public QObject
{
    Q_OBJECT
public:
    CComData(QObject *parent);
    ~CComData();

    ComCtrl &operator*() const;
    ComCtrl *operator->() const;
    ComCtrl *data() const;
    QMutex *mutex() const;
private:
    CComDataPrivate *const privateData;
};

#endif // CCOMDATA_H
