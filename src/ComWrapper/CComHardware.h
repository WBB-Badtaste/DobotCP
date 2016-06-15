#ifndef CCOMHARDWARE_H
#define CCOMHARDWARE_H

#include <QObject>
#include <QScopedArrayPointer>

class QIODevice;
class CComData;

class CComHardware : public QObject
{
    Q_OBJECT
public:
    CComHardware(QIODevice *device, CComData *comData, QObject *parent = 0);
    ~CComHardware();
protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void onReadyRead(void);
    void onBytesWritten(qint64 bytes);
private:
    QIODevice *const device;
    CComData *const comData;
    int timer;
    QScopedArrayPointer<quint8> *const writeBuffer;

    bool readyWrite;
};

#endif // CCOMHARDWARE_H
