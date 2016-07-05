#include "CMouseControlDialog.h"
#include <QtWidgets>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include "DobotDll.h"
#include <fstream>
#include <windows.h>
#include <windef.h>
#include <QDateTime>




CMouseControlDialog::CMouseControlDialog(QWidget *parent)
  : QDialog(parent),
    rightBtnPressed(false),
    m_ratio_x((float)(DOBOT_WORK_X) / (float)(SCREEN_HEIGHT) / MANUAL_RATIO),
    m_ratio_y((float)(DOBOT_WORK_Y) / (float)(SCREEN_WIDTH) / MANUAL_RATIO),
    m_lastSampleTime(0),
    m_sampleDataMask(0),
    m_startCommand(false),
    m_startCommandTime(0),
    m_commandMask(0)
{
    ZeroMemory(m_sampleData, EXPORT_SAMPLE_NUM * 4 * sizeof(int));
    ZeroMemory(m_commandX, 10000 * sizeof(double));
    ZeroMemory(m_commandY, 10000 * sizeof(double));
    ZeroMemory(m_deltaCommandX, 10000 * sizeof(double));
    ZeroMemory(m_deltaCommandY, 10000 * sizeof(double));

    setWindowTitle("Mouse control panel");
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setMouseTracking(true);

    QRect rect = QApplication::desktop()->screenGeometry();
    QCursor p;
    p.setPos(rect.width()/2, rect.height()/2);

    startupFlag = 1;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->setSingleShot(true);
    timer->start(COMMAND_DELAY);

    RECT mainWinRect; //RECT在windef.h中被定义
    mainWinRect.left = (LONG)this->geometry().left() + 560;
    mainWinRect.right = (LONG)this->geometry().right() + 560;
    mainWinRect.top = (LONG)this->geometry().top() + 200;
    mainWinRect.bottom = (LONG)this->geometry().bottom() + 200;
    ClipCursor(&mainWinRect);

    m_sysTime.start();//开始计算系统运行时间
}

CMouseControlDialog::~CMouseControlDialog()
{

    setMouseTracking(false);
    ClipCursor(NULL);

    timer->stop();
    delete timer;
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


    if(m_startCommand)
        return;

    //判断是否达到或超过一个采样周期，并设置m_lastSampleTime
    if(!m_lastSampleTime || currentSampleTime - m_lastSampleTime > EXPORT_SAMPLE_CYC)
    {
        if(!m_lastSampleTime)
            m_startSampleTime = currentSampleTime;
        m_lastSampleTime = currentSampleTime;
    }
    else
    {
        return;
    }

    //储存采集数据
    m_sampleData[m_sampleDataMask][0] = currentSampleTime -  m_startSampleTime;
    m_sampleData[m_sampleDataMask][1] = mousePoint.x();
    m_sampleData[m_sampleDataMask][2] = mousePoint.y();

    //采集样本足够，开始下发
    if(++m_sampleDataMask == EXPORT_SAMPLE_NUM)
    {
        //转存到接口数据结构
        E2_POINT sampleX[m_sampleDataMask];
        E2_POINT sampleY[m_sampleDataMask];

        ZeroMemory(sampleX, m_sampleDataMask * sizeof(E2_POINT));
        ZeroMemory(sampleY, m_sampleDataMask * sizeof(E2_POINT));

        for(unsigned i= 0; i < m_sampleDataMask; ++i)
        {
            sampleX[i].x = m_sampleData[i][0];
            sampleX[i].y = m_sampleData[i][1];
            sampleY[i].x = m_sampleData[i][0];
            sampleY[i].y = m_sampleData[i][2];
        }

        m_splineX.CubicSplineInterpolation(m_sampleDataMask, sampleX);
        m_splineY.CubicSplineInterpolation(m_sampleDataMask, sampleY);

        m_startCommand = true;
    }

    m_currentPoint.deltaX = mousePoint.x();
    m_currentPoint.deltaY = mousePoint.y();

    if (startupFlag)
    {
        startupFlag = 0;
        m_lastPoint.deltaX = SCREEN_WIDTH / 2;
        m_lastPoint.deltaY = SCREEN_HEIGHT / 2;
    }

}

void CMouseControlDialog::wheelEvent(QWheelEvent *event)
{
//   point.deltaX = 0;
//   point.deltaY = 0;
//   point.deltaZ = event->angleDelta().y();
//   xyzPoint << point;
}

void CMouseControlDialog::onTimer(void)
{
    if(!m_startCommand)
    {
        timer->start(COMMAND_DELAY);
        return;
    }

    //检查buffer水位
    uint32_t bufferSize = 0;
    int result = GetCPBufferSize(&bufferSize);

    if (result != 0)
    {
        qDebug() << "Timeout!";
    }
    if (bufferSize == 0)
    {
        qDebug() << "Buffer full!";
    }
    else
    {
        if(m_commandMask == 0)
            m_commandTime[m_commandMask] = 0;
        else
            m_commandTime[m_commandMask] = m_commandTime[m_commandMask - 1] + 20;

        if(m_commandTime[m_commandMask] > m_splineX.dequeOfNodes.back().x)
        {
            m_commandTime[m_commandMask] = m_splineX.dequeOfNodes.back().x;
            m_startCommand = false;
        }

        if(!m_splineX.GetY(m_commandTime[m_commandMask], m_commandX[m_commandMask]))
        {
            m_startCommand = false;
            timer->start(COMMAND_DELAY);
            return;
        }

        if(!m_splineY.GetY(m_commandTime[m_commandMask], m_commandY[m_commandMask]))
        {
            m_startCommand = false;
            timer->start(COMMAND_DELAY);
            return;
        }

        if(m_commandMask)
        {
            m_deltaCommandX[m_commandMask] = (m_commandX[m_commandMask] - m_commandX[m_commandMask - 1]) * m_ratio_x;
            m_deltaCommandY[m_commandMask] = (m_commandY[m_commandMask] - m_commandY[m_commandMask - 1]) * m_ratio_y;
        }
        else
        {
            m_deltaCommandX[m_commandMask] = 0;
            m_deltaCommandY[m_commandMask] = 0;
        }

        if (!( -POINTS_THRESHOLD < m_deltaCommandX[m_commandMask] && m_deltaCommandX[m_commandMask] < POINTS_THRESHOLD
            && -POINTS_THRESHOLD < m_deltaCommandY[m_commandMask] && m_deltaCommandY[m_commandMask] < POINTS_THRESHOLD
          //&& -POINTS_THRESHOLD < m_deltaPoint.deltaZ && m_deltaPoint.deltaZ < POINTS_THRESHOLD
               ))
        {
            //设置下发值
            CPBufferCmd cpBufferCmd;
            cpBufferCmd.loop = 0;
            cpBufferCmd.line = 0;
            cpBufferCmd.x = -m_deltaCommandY[m_commandMask];
            cpBufferCmd.y = -m_deltaCommandX[m_commandMask];
            cpBufferCmd.z = 0; //point.deltaZ / 30;
            cpBufferCmd.velocity = 0;
            cpBufferCmd.ioState = 0;

            int result = SetCPBufferCmd(&cpBufferCmd);
            if (result == DobotResult_Timeout) qDebug() << "timeout";
            else
                if (result == DobotResult_NoError) qDebug() << "NoError:" << cpBufferCmd.x << cpBufferCmd.y << cpBufferCmd.z;
        }

        m_commandMask++;
    }


    timer->start(COMMAND_DELAY);
}


