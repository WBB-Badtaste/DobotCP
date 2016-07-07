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
  : QDialog(parent)
  ,  timer(nullptr)
  ,  rightBtnPressed(false)
  ,  m_ratio_x((float)(DOBOT_WORK_X) / (float)(SCREEN_HEIGHT) / MANUAL_RATIO)
  ,  m_ratio_y((float)(DOBOT_WORK_Y) / (float)(SCREEN_WIDTH) / MANUAL_RATIO)
  ,  m_ratio_z(1 / 30)
  ,  m_sysTime()
  ,  m_lastSampleTime(0)
  ,  m_startSampleTime(0)
  ,  m_startCmdTime(0)
  ,  m_startSampleSignal(2)
  ,  m_lastSamplePoint()
  ,  m_lastCmdPoint()
  ,  m_trajOp()
  ,  m_bReSendCmd(false)
#ifdef EXPORT_CMD_DATAS
  ,  m_excel()
  ,  m_excel_row(1)
#endif
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

    m_trajOp.EnterPoint(m_lastSamplePoint);

#ifdef EXPORT_CMD_DATAS
    m_excel.Open(QObject::tr("C:\\Users\\sos90\\Desktop\\splineDatas.xls"),1,false);

    m_excel.SetCellData(m_excel_row, 1, "系统时间戳");
    m_excel.SetCellData(m_excel_row, 2, "下发时间戳");

    m_excel.SetCellData(m_excel_row, 4, "X坐标");
    m_excel.SetCellData(m_excel_row, 5, "Y坐标");
    m_excel.SetCellData(m_excel_row, 6, "Z坐标");

    m_excel.SetCellData(m_excel_row, 8, "X下发");
    m_excel.SetCellData(m_excel_row, 9, "Y下发");
    m_excel.SetCellData(m_excel_row, 10, "Z下发");
#endif
}

CMouseControlDialog::~CMouseControlDialog()
{
    timer->stop();
    delete timer;
    timer = nullptr;

    setMouseTracking(false);
    ClipCursor(NULL);

#ifdef EXPORT_CMD_DATAS
    m_excel.Save();
    m_excel.Close();
#endif
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

    if(m_startSampleSignal)
    {
        if(m_startSampleSignal == 2)
        {
            //第一次鼠标事件是自动去到窗口中心,也会存入轨迹生成器
            m_lastSamplePoint.t = 0;
            m_lastSamplePoint.x = mousePoint.x();
            m_lastSamplePoint.y = mousePoint.y();

            m_lastCmdPoint = m_lastSamplePoint;//将初始点也存到命令位置中
        }
        else if(m_startSampleSignal == 1)
        {
            //第二次鼠标事件才是开始运动
            m_startSampleTime = currentSampleTime - 100;
            m_lastSampleTime = currentSampleTime;

            m_lastSamplePoint.t = currentSampleTime - m_startSampleTime;
            m_lastSamplePoint.x = mousePoint.x();
            m_lastSamplePoint.y = mousePoint.y();

            m_lastCmdPoint = m_lastSamplePoint;//将初始点也存到命令位置中
        }

        m_startSampleSignal--;
    }
    else
    {
        //判断是否达到或超过一个采样周期，并设置m_lastSampleTime
        if(currentSampleTime - m_lastSampleTime > MOUSE_SAMPLE_CYC)
        {
            m_lastSampleTime = currentSampleTime;

            m_lastSamplePoint.t = currentSampleTime - m_startSampleTime;
            m_lastSamplePoint.x = mousePoint.x();
            m_lastSamplePoint.y = mousePoint.y();
        }
        else
            return;
    }

    qDebug() << "Sample time:" << m_lastSamplePoint.t;
    m_trajOp.EnterPoint(m_lastSamplePoint);
}

void CMouseControlDialog::wheelEvent(QWheelEvent *event)
{
    m_lastSamplePoint.z = event->angleDelta().y();
}

void CMouseControlDialog::onTimer(void)
{
    if(!m_trajOp.IsEmpty())//判断是否还有未完成的轨迹
    {
        //不重发才重置下发数据
        if(!m_bReSendCmd)
        {
            double x(0.0), y(0.0), z(0.0);

            //设置当前下发数据的时间戳
            int time(m_lastCmdPoint.t + SPLINE_SAMPLE_CYC);

            //读取对应的坐标
            if(!m_trajOp.GetDataByTimeStamp(m_lastCmdPoint.t, x, y, z))
            {
                qDebug() << "All trajectory datas have been read!";
                timer->start(COMMAND_DELAY);
                return;
            }

            //设置下发结构体
            m_cmd.x = (m_lastCmdPoint.y - y) * m_ratio_x;
            m_cmd.y = (m_lastCmdPoint.x - x) * m_ratio_y;
            m_cmd.z = (z - m_lastCmdPoint.z) * m_ratio_z;
            m_cmd.velocity = 0;

            //保存当前数据，供下次使用
            m_lastCmdPoint.t = time;
            m_lastCmdPoint.x = x;
            m_lastCmdPoint.y = y;
            m_lastCmdPoint.z = z;
        }
#ifdef EXPORT_CMD_DATAS
        m_excel_row++;

        m_excel.SetCellData(m_excel_row, 1, m_sysTime.elapsed());
        m_excel.SetCellData(m_excel_row, 2, m_lastCmdPoint.t);

        m_excel.SetCellData(m_excel_row, 4, m_lastCmdPoint.x);
        m_excel.SetCellData(m_excel_row, 5, m_lastCmdPoint.y);
        m_excel.SetCellData(m_excel_row, 6, m_lastCmdPoint.z);

        m_excel.SetCellData(m_excel_row, 8, m_cmd.x);
        m_excel.SetCellData(m_excel_row, 9, m_cmd.y);
        m_excel.SetCellData(m_excel_row, 10, m_cmd.z);
#else
        //下发数据
        int result = SetCPCmd(&m_cmd, true, 0);
        if (result == DobotCommunicate_NoError)
        {
            qDebug() << "CP:" << m_lastCmdPoint.x << m_lastCmdPoint.y << m_lastCmdPoint.z;
            m_bReSendCmd = false;
        }
        else
        {
            if(result == DobotCommunicate_BufferFull)
                qDebug() << "Communicate buffer full!";
            else
                qDebug() << "Communicate timeout!";
            m_bReSendCmd = true;
        }
#endif
    }
    timer->start(COMMAND_DELAY);

}
