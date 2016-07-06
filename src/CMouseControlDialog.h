#ifndef CMOUSECONTROL_H
#define CMOUSECONTROL_H

#include <QDialog>
#include <QVector>
#include <QTime>
#include "TrajectoryOperator.h"
#include "DobotDll.h"

#define MOUSE_SAMPLE_CYC 100    //鼠标采样周期
#define SPLINE_SAMPLE_CYC 20    //样条采样周期
#define COMMAND_DELAY 5         //下发周期
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

    QTime m_sysTime;//用来记录运行时间
    int m_lastSampleTime;//上一次采集时间戳,计算时间差用
    int m_startSampleTime;//开始采样时间
    int m_startCmdTime;//开始下发时间

    TRAJ_E4_POINT m_lastSamplePoint;
    TRAJ_E4_POINT m_lastCmdPoint;

    TrajectoryOperator m_trajOp;

    bool m_bReSendCmd;
    CPBufferCmd m_cmd;
};

#endif // CMOUSECONTROL_H
