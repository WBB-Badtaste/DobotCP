#include "CMouseControlDialog.h"
#include <QtWidgets>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include "DobotDll.h"

CMouseControlDialog::CMouseControlDialog(QWidget *parent)
  : QDialog(parent),
    rightBtnPressed(false)
{
    setWindowTitle("Mouse control panel");
    setFixedSize(800, 800);
    setMouseTracking(true);

    startupFlag = 1;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->setSingleShot(true);
    timer->start();
}

CMouseControlDialog::~CMouseControlDialog()
{
    setMouseTracking(false);
    xyzPoint.clear();
}

void CMouseControlDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightBtnPressed = true;
        qDebug() << "Right button pressed!";
    }
}

void CMouseControlDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightBtnPressed = false;
        qDebug() << "Right button released!";
    }
}

void CMouseControlDialog::mouseMoveEvent(QMouseEvent *event)
{
   QPoint currentPoint = event->pos();
   if (startupFlag) {
       startupFlag = 0;
       lastPoint = currentPoint;
   }
   deltaPoint = lastPoint - currentPoint;
   lastPoint = currentPoint;

   point.deltaX = deltaPoint.x();
   point.deltaY = deltaPoint.y();
   point.deltaZ = 0;
   xyzPoint << point;

}

void CMouseControlDialog::wheelEvent(QWheelEvent *event)
{
   point.deltaX = 0;
   point.deltaY = 0;
   point.deltaZ = event->angleDelta().y();
   xyzPoint << point;
}

void CMouseControlDialog::onTimer(void)
{
    do {
        if (xyzPoint.isEmpty()) {
            break;
        }

        uint32_t bufferSize = 0;
        GetCPBufferSize(&bufferSize);   
        if (bufferSize == 0) {
            qDebug() << "Buffer full!";
            break;
        }

        Point point = xyzPoint.first();
        CPBufferCmd cpBufferCmd;
        cpBufferCmd.loop = 0;
        cpBufferCmd.line = 0;
        cpBufferCmd.x = point.deltaX;
        cpBufferCmd.y = point.deltaY;
        cpBufferCmd.z = point.deltaZ / 30;
        cpBufferCmd.velocity = 100;
        cpBufferCmd.ioState = 0;

        if ((cpBufferCmd.x == 0  and  cpBufferCmd.y == 0  and  cpBufferCmd.z == 0) ||
            ( -3 > cpBufferCmd.x || cpBufferCmd.x > 3 ) ||
            ( -3 > cpBufferCmd.y || cpBufferCmd.y > 3 ))
        {
            xyzPoint.pop_front();
            break;
        }

        int result = SetCPBufferCmd(&cpBufferCmd);
        if (result == DobotResult_Timeout) {
            qDebug() << "timeout";
            break;
        }
        if (result == DobotResult_NoError) {
            qDebug() << "NoError:" << cpBufferCmd.x << cpBufferCmd.y << cpBufferCmd.z;
        }
        xyzPoint.pop_front();
    } while (0);
    timer->start();
}
