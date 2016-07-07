#ifndef CMOUSECONTROL_H
#define CMOUSECONTROL_H


#include <QDialog>
#include <QVector>
#include <QTime>
#include "TrajectoryOperator.h"
#include "DobotDll.h"

//#define EXPORT_CMD_DATAS      //输出数据标志
#ifdef EXPORT_CMD_DATAS
#include "excelengine.h"
#endif

#define DO_NOT_USE_TRAJECTORY   //不使用轨迹生成器标志
#ifdef DO_NOT_USE_TRAJECTORY
#define MOUSE_SAMPLE_CYC 20    //鼠标采样周期
#else
#define MOUSE_SAMPLE_CYC 100    //鼠标采样周期
#endif
#define SPLINE_SAMPLE_CYC 20    //样条采样周期
#define COMMAND_DELAY 0         //下发周期
#define MANUAL_RATIO 2          //手动坐标比例
#define POINTS_THRESHOLD 5      //坐标点阈值





class CMouseControlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMouseControlDialog(QWidget *parent = 0);
    ~CMouseControlDialog();
    QTimer *timer;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void onTimer(void);
private:
    bool rightBtnPressed;

    static const int DOBOT_WORK_X = 320;
    static const int DOBOT_WORK_Y = 400;
    static const int DOBOT_WORK_Z = 320;
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 640;

    float m_ratio_x;
    float m_ratio_y;
    float m_ratio_z;

    QTime m_sysTime;//用来记录运行时间
    int m_lastSampleTime;//上一次采集时间戳,计算时间差用
    int m_startSampleTime;//开始采样时间
    int m_startCmdTime;//开始下发时间
    int m_startSampleSignal;//开始采集标志

#ifdef DO_NOT_USE_TRAJECTORY
    std::deque<QPoint> m_mousePoint;
#else
    TRAJ_E4_POINT m_lastSamplePoint;//上一个采集数据
    TRAJ_E4_POINT m_lastCmdPoint;//上一个下发数据
    TrajectoryOperator m_trajOp;
#endif

    bool m_bReSendCmd;//重发标志
    CPCmd m_cmd;//下发数据

#ifdef EXPORT_CMD_DATAS
    ExcelEngine m_excel;
    unsigned m_excel_row;
#endif
};

#endif // CMOUSECONTROL_H
