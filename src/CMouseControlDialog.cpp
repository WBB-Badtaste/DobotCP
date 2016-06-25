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
#include "excelengine.h"



CMouseControlDialog::CMouseControlDialog(QWidget *parent)
  : QDialog(parent),
    rightBtnPressed(false),
    m_ratio_x((float)(DOBOT_WORK_X) / (float)(SCREEN_HEIGHT) / MANUAL_RATIO),
    m_ratio_y((float)(DOBOT_WORK_Y) / (float)(SCREEN_WIDTH) / MANUAL_RATIO),
    m_lastSampleTime(0),
    m_sampleDataMask(0),
    m_startCommand(false),
    m_splineX(nullptr),
    m_splineY(nullptr),
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

#ifdef EXPORT_MOUSE_DATA
        ExcelEngine excel; //创建excl对象
        excel.Open(QObject::tr("C:\\Users\\sos90\\Desktop\\SP_Mouse_Data.xls"), 1, false); //打开指定的xls文件的指定sheet，且指定是否可见

        excel.SetCellData(1, 1, "采集时间戳");
        excel.SetCellData(1, 2, "采集X");
        excel.SetCellData(1, 3, "采集Y");
        excel.SetCellData(1, 4, "采集Z");

        for (int i = 0; i < EXPORT_SAMPLE_NUM; i++)
        {
             excel.SetCellData(i + 2, 1, m_sampleData[i][0]);
             excel.SetCellData(i + 2, 2, m_sampleData[i][1]);
             excel.SetCellData(i + 2, 3, m_sampleData[i][2]);
             excel.SetCellData(i + 2, 4, m_sampleData[i][3]);
        }

        excel.SetCellData(1, 6, "spline时间戳");
        excel.SetCellData(1, 7, "splineX");
        excel.SetCellData(1, 8, "splineX'");
        excel.SetCellData(1, 9, "splineX''");
        excel.SetCellData(1, 10, "splineY");
        excel.SetCellData(1, 11, "splineY'");
        excel.SetCellData(1, 12, "splineY''");


        double t(m_splineX->nodeArray[0].x);
        SP_PROIFILE x;
        SP_PROIFILE y;

        for(unsigned i = 0; true; ++i)
        {
            excel.SetCellData(i + 2,6,t);

            m_splineX->GetYFull(t, x);
            excel.SetCellData(i + 2, 7, x.y);
            excel.SetCellData(i + 2, 8, x.dy);
            excel.SetCellData(i + 2, 9, x.ddy);

            m_splineY->GetYFull(t, y);
            excel.SetCellData(i + 2, 10, y.y);
            excel.SetCellData(i + 2, 11, y.dy);
            excel.SetCellData(i + 2, 12, y.ddy);

            if(t == m_splineX->nodeArray[m_splineX->pointAmount - 1].x)
                break;

            t += PRINT_SPLINE_CYC;
            if(t > m_splineX->nodeArray[m_splineX->pointAmount - 1].x)
                t = m_splineX->nodeArray[m_splineX->pointAmount - 1].x;
        }

        excel.SetCellData(1, 14, "command时间戳");
        excel.SetCellData(1, 15, "commandX");
        excel.SetCellData(1, 16, "commandY");
        excel.SetCellData(1, 17, "deltaCommandX");
        excel.SetCellData(1, 18, "deltaCommandY");

        for(unsigned i = 0; i < m_commandMask; ++i)
        {
            excel.SetCellData(i + 2, 14, m_commandTime[i]);
            excel.SetCellData(i + 2, 15, m_commandX[i]);
            excel.SetCellData(i + 2, 16, m_commandY[i]);
            excel.SetCellData(i + 2, 17, m_deltaCommandX[i]);
            excel.SetCellData(i + 2, 18, m_deltaCommandY[i]);
        }


        excel.Save(); //保存
        excel.Close();
#endif

    setMouseTracking(false);
    ClipCursor(NULL);

    if(!m_splineX)
    {
        delete m_splineX;
        m_splineX = nullptr;
    }

    if(!m_splineY)
    {
        delete m_splineY;
        m_splineY = nullptr;
    }

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

#ifdef EXPORT_MOUSE_DATA
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

    //采集样本足够，关闭窗口
    if(++m_sampleDataMask == EXPORT_SAMPLE_NUM)
    {
        m_startCommand = true;

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

        //生成spline
        m_splineX = new E2_CUB_SPLINE(m_sampleDataMask, sampleX);
        m_splineY = new E2_CUB_SPLINE(m_sampleDataMask, sampleY);
    }

#endif
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

#ifndef EXPORT_MOUSE_DATA
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
        bufferSize = 20;
    }
    else
    {
#endif
        double currentTime(m_sysTime.elapsed());
        if(!m_startCommandTime)
            m_startCommandTime = currentTime;


        m_commandTime[m_commandMask] = currentTime - m_startCommandTime;
        if(m_commandTime[m_commandMask] > m_splineX->nodeArray[m_splineX->pointAmount - 1].x)
        {
            m_commandTime[m_commandMask] = m_splineX->nodeArray[m_splineX->pointAmount - 1].x;
            m_startCommand = false;
        }

        if(!m_splineX->GetY( m_commandTime[m_commandMask], m_commandX[m_commandMask]))
        {
            m_startCommand = false;
            timer->start(COMMAND_DELAY);
            return;
        }

        if(!m_splineY->GetY( m_commandTime[m_commandMask], m_commandY[m_commandMask]))
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
            cpBufferCmd.z = 0;//point.deltaZ / 30;
            cpBufferCmd.velocity = 0;
            cpBufferCmd.ioState = 0;

            int result = SetCPBufferCmd(&cpBufferCmd);
            if (result == DobotResult_Timeout) qDebug() << "timeout";
            else
                if (result == DobotResult_NoError) qDebug() << "NoError:" << cpBufferCmd.x << cpBufferCmd.y << cpBufferCmd.z;
        }

        m_commandMask++;
#ifndef EXPORT_MOUSE_DATA
    }
#endif

    timer->start(COMMAND_DELAY);
}


