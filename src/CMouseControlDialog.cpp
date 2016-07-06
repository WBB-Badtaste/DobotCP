#include "CMouseControlDialog.h"
#include <QtWidgets>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <fstream>
#include <windows.h>
#include <windef.h>
#include <QDateTime>


CMouseControlDialog::CMouseControlDialog(QWidget *parent)
  : QDialog(parent),
    timer(nullptr),
    rightBtnPressed(false),
    m_ratio_x((float)(DOBOT_WORK_X) / (float)(SCREEN_HEIGHT) / MANUAL_RATIO),
    m_ratio_y((float)(DOBOT_WORK_Y) / (float)(SCREEN_WIDTH) / MANUAL_RATIO),
    m_sysTime(),
    m_lastSampleTime(0),
    m_startSampleTime(0),
    m_startCmdTime(0),
    m_lastSamplePoint(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
    m_lastCmdPoint(0, 0, 0, 0),
    m_trajOp(),
    m_bReSendCmd(false)
{
    setWindowTitle("Mouse control panel");
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setMouseTracking(true);

    QRect rect = QApplication::desktop()->screenGeometry();
    QCursor p;
    p.setPos(rect.width()/2, rect.height()/2);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->setSingleShot(true);
    timer->start(COMMAND_DELAY);

    RECT mainWinRect; //RECT在windef.h中被定义
    mainWinRect.left = (LONG)this->geometry().left() + 560;
    mainWinRect.right = (LONG)this->geometry().right() + 560;
    mainWinRect.top = (LONG)this->geometry().top() + 200 ;
    mainWinRect.bottom = (LONG)this->geometry().bottom() + 200;
    ClipCursor(&mainWinRect);

    m_sysTime.start();//开始计算系统运行时间
    m_lastSampleTime = m_startSampleTime = m_sysTime.elapsed();

    m_trajOp.EnterPoint(m_lastSamplePoint);
}

CMouseControlDialog::~CMouseControlDialog()
{
    timer->stop();
    delete timer;
    timer = nullptr;

    setMouseTracking(false);
    ClipCursor(NULL);
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
    QPoint mousePoint(event->pos());//获取鼠标位置
    int currentSampleTime(m_sysTime.elapsed());//获取时间戳

    //判断是否达到或超过一个采样周期，并设置m_lastSampleTime
    if(!m_lastSampleTime || currentSampleTime - m_lastSampleTime > MOUSE_SAMPLE_CYC)
        m_lastSampleTime = currentSampleTime;
    else
        return;

    m_lastSamplePoint.t = m_lastSampleTime - m_startSampleTime;
    m_lastSamplePoint.x = mousePoint.x();
    m_lastSamplePoint.y = mousePoint.y();

    m_trajOp.EnterPoint(m_lastSamplePoint);
}

void CMouseControlDialog::wheelEvent(QWheelEvent *event)
{
    m_lastSamplePoint.z = event->angleDelta().y() / 30;
}

void CMouseControlDialog::onTimer(void)
{
    if(!m_bReSendCmd) {
        double x(0.0), y(0.0), z(0.0);

        m_lastCmdPoint.t += SPLINE_SAMPLE_CYC;

        m_trajOp.GetDataByTimeStamp(m_lastCmdPoint.t, x, y, z);

        m_cmd.x = m_lastCmdPoint.y - y;
        m_cmd.y = m_lastCmdPoint.x - x;
        m_cmd.z = z - m_lastCmdPoint.z;
        m_cmd.velocity = 0;

        m_lastCmdPoint.x = x;
        m_lastCmdPoint.y = y;
        m_lastCmdPoint.z = z;
    }
    if (SetCPCmd(&m_cmd, true, 0) == DobotCommunicate_NoError) {
        m_bReSendCmd = false;
    } else {
        m_bReSendCmd = true;
    }
    timer->start(COMMAND_DELAY);
}
