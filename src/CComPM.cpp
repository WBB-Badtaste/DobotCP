#include "CComPM.h"
#include "CComApp.h"

#include <QSerialPort>
#include <QSerialPortInfo>

CComPM::CComPM(QObject *parent)
  : QObject(parent)
{
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    QString portName;

    foreach(QSerialPortInfo portInfo, availablePorts) {
        if (portInfo.description().contains("USB-to-Serial")) {
            portName = portInfo.portName();
            break;
        }
    }
    QSerialPort *serial = new QSerialPort(portName);

    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->open(QIODevice::ReadWrite);

    comApp = new CComApp(serial, this);

    initConnect();
}

CComPM::~CComPM()
{

}

void CComPM::initConnect(void)
{
    connect(comApp, SIGNAL(messageReady(Message)), this, SIGNAL(messageReady(Message)));
}

void CComPM::packMessage(const Message &message)
{
    comApp->postMessage(message);
}
