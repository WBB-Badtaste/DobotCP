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
    m_historyPointsChangeIndex(0),
    m_historyPointsSizeMask(0),
    m_ratio_x((float)(DOBOT_WORK_X) / (float)(SCREEN_HEIGHT) / MANUAL_RATIO),
    m_ratio_y((float)(DOBOT_WORK_Y) / (float)(SCREEN_WIDTH) / MANUAL_RATIO)
#ifdef EXPORT_MOUSE_DATA
    , m_indexOfData(0)
    , m_lastSysTime(0)
    , m_indexOfSmoothData(0)
#endif
{
    ZeroMemory(m_historyPoints, HISTORY_POINTS_SIZE * sizeof(Point));

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

//    m_endPointTimer = new QTimer(this);
//    connect(m_endPointTimer, SIGNAL(timeout()), this, SLOT(onEndPointSmooth()));
//    m_endPointTimer->setSingleShot(true);
//    m_endPointTimer->start(ENDPOINT_SMOOTH_DELAY);

    RECT mainWinRect; //RECT在windef.h中被定义
    mainWinRect.left = (LONG)this->geometry().left() + 560;
    mainWinRect.right = (LONG)this->geometry().right() + 560;
    mainWinRect.top = (LONG)this->geometry().top() + 200;
    mainWinRect.bottom = (LONG)this->geometry().bottom() + 200;
    ClipCursor(&mainWinRect);

}

CMouseControlDialog::~CMouseControlDialog()
{

#ifdef EXPORT_MOUSE_DATA
        ExcelEngine excel; //创建excl对象
        excel.Open(QObject::tr("C:\\Users\\sos90\\Desktop\\Export_Mouse_Data.xls"),1,false); //打开指定的xls文件的指定sheet，且指定是否可见

        for (int i=1; i<= EXPORT_DATA_SIZE; i++)
        {
            for (int j=1; j<= 3; j++)
            {
               excel.SetCellData(i,j,m_exportData[i - 1][j - 1]); //修改指定单元数据
               excel.SetCellData(i,j+3,m_smoothData[i - 1][j - 1]);
            }
        }

        excel.Save(); //保存
        excel.Close();
#endif

    setMouseTracking(false);
    //xyzPoint.clear();
    ClipCursor(NULL);

//    QDateTime::currentDateTime().sta

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
    QPoint mousePoint(event->pos());

#ifdef EXPORT_MOUSE_DATA
    int currentTime(QDateTime::currentDateTime().toTime_t());
    if(!m_lastSysTime) m_lastSysTime = currentTime;

    if(m_indexOfData < EXPORT_DATA_SIZE)
    {
         m_exportData[m_indexOfData][0] = mousePoint.x();
         m_exportData[m_indexOfData][1] = mousePoint.y();
         m_exportData[m_indexOfData][2] = currentTime - m_lastSysTime;

         m_lastSysTime = currentTime;
         m_indexOfData++;

         if(m_indexOfData == EXPORT_DATA_SIZE)
             this->close();
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

    startEndPointSingal = 3;
    SmoothPoint(m_currentPoint);

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
    //检查buffer水位
    uint32_t bufferSize = 0;
    int result = GetCPBufferSize(&bufferSize);
    qDebug() << "Buffer size:" << bufferSize;

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
#ifdef SMOOTH_MODEL
        m_deltaPoint = m_smoothPoint - m_lastPoint;
        m_lastPoint = m_smoothPoint;
#else
        m_deltaPoint = m_currentPoint - m_lastPoint;
        m_lastPoint = m_currentPoint;
#endif
        m_deltaPoint.deltaX *= m_ratio_x;
        m_deltaPoint.deltaY *= m_ratio_y;

        if (!( -POINTS_THRESHOLD < m_deltaPoint.deltaX && m_deltaPoint.deltaX < POINTS_THRESHOLD
            && -POINTS_THRESHOLD < m_deltaPoint.deltaY && m_deltaPoint.deltaY < POINTS_THRESHOLD
            && -POINTS_THRESHOLD < m_deltaPoint.deltaZ && m_deltaPoint.deltaZ < POINTS_THRESHOLD ))
        {
#ifdef EXPORT_MOUSE_DATA
            m_smoothData[m_indexOfSmoothData][0] = -m_deltaPoint.deltaY;
            m_smoothData[m_indexOfSmoothData][1] = -m_deltaPoint.deltaX;
            m_smoothData[m_indexOfSmoothData][2] = 0;
            m_indexOfSmoothData++;
#else
            //设置下发值
            CPBufferCmd cpBufferCmd;
            cpBufferCmd.loop = 0;
            cpBufferCmd.line = 0;
            cpBufferCmd.x = -m_deltaPoint.deltaY;
            cpBufferCmd.y = -m_deltaPoint.deltaX;
            cpBufferCmd.z = 0;//point.deltaZ / 30;
            cpBufferCmd.velocity = 0;
            cpBufferCmd.ioState = 0;

     //     std::ofstream poi1;
     //     poi1.open("C:\\Users\\sos90\\Desktop\\point_O.txt", std::ios::out|std::ios::app);
     //     poi1.setf(std::ios::fixed, std::ios::floatfield);
     //     poi1.setf(std::ios::showpoint);
     //     poi1<< "X:"<< cpBufferCmd.x  << "           Y:" << cpBufferCmd.y << "\n"<< endl;
     //     poi1.close();

            int result = SetCPBufferCmd(&cpBufferCmd);
            if (result == DobotResult_Timeout) qDebug() << "timeout";
            else
                if (result == DobotResult_NoError) qDebug() << "NoError:" << cpBufferCmd.x << cpBufferCmd.y << cpBufferCmd.z;
#endif
        }
    }

    timer->start(COMMAND_DELAY);
}

const void CMouseControlDialog::SmoothPoint(Point &point)
{
    //总值加上最新的点
    m_sumOfPoints += point;

    //数组是否已满
    if(m_historyPointsSizeMask == HISTORY_POINTS_SIZE)
    {
        //总值减去准备出数组的点
        m_sumOfPoints -= m_historyPoints[m_historyPointsChangeIndex];

        //出数组并存入新的点
        m_historyPoints[m_historyPointsChangeIndex] = point;

        //出组标志移位
        if(m_historyPointsChangeIndex == HISTORY_POINTS_SIZE - 1)
            m_historyPointsChangeIndex = 0;
        else
            m_historyPointsChangeIndex++;
    }
    else
        m_historyPoints[m_historyPointsSizeMask++] = point;

  //取均值
  m_smoothPoint = m_sumOfPoints / m_historyPointsSizeMask;
}

void CMouseControlDialog::onEndPointSmooth(void)
{
    if(startEndPointSingal)
        startEndPointSingal--;
    else
        {
        SmoothPoint(m_currentPoint);
        SmoothPoint(m_currentPoint);
        SmoothPoint(m_currentPoint);
        SmoothPoint(m_currentPoint);
        SmoothPoint(m_currentPoint);
        SmoothPoint(m_currentPoint);
        }

    m_endPointTimer->start(COMMAND_DELAY);
}
