#ifndef CMOUSECONTROL_H
#define CMOUSECONTROL_H

#include <QDialog>
#include <QVector>
#include <QTime>
#include "E2CubicSpline.h"

#define EXPORT_MOUSE_DATA
#define EXPORT_SAMPLE_NUM 20
#define EXPORT_SAMPLE_CYC 100

#define PRINT_SPLINE_CYC 3

#define COMMAND_DELAY 20

#define MANUAL_RATIO 2
#define POINTS_THRESHOLD 5




typedef struct tagPoint
{
    float deltaX;
    float deltaY;
    float deltaZ;
    tagPoint():deltaX(0.0),deltaY(0.0),deltaZ(0.0){};

    tagPoint(const float x, const float y, const float z):deltaX(x),deltaY(y),deltaZ(z){};

    tagPoint(const tagPoint& object):deltaX(object.deltaX),deltaY(object.deltaY),deltaZ(object.deltaZ){};

    tagPoint& operator=(const tagPoint& object)
    {
        deltaX = object.deltaX;
        deltaY = object.deltaY;
        deltaZ = object.deltaZ;
        return *this;
    };

    tagPoint& operator+=(const tagPoint& object)
    {
        deltaX += object.deltaX;
        deltaY += object.deltaY;
        deltaZ += object.deltaZ;
        return *this;
    };

    tagPoint& operator-=(const tagPoint& object)
    {
        deltaX -= object.deltaX;
        deltaY -= object.deltaY;
        deltaZ -= object.deltaZ;
        return *this;
    };

    friend tagPoint operator/(const tagPoint& object, const float number)
    {
        return tagPoint(object.deltaX / number, object.deltaY / number, object.deltaZ / number);
    };

    friend tagPoint operator-(const tagPoint& objectL, const tagPoint& objectR)
    {
        return tagPoint(objectL.deltaX - objectR.deltaX, objectL.deltaY - objectR.deltaY, objectL.deltaZ - objectR.deltaZ);
    };

}Point;

class CMouseControlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMouseControlDialog(QWidget *parent = 0);
    ~CMouseControlDialog();
    quint32 startupFlag;
    QTimer *timer;
    QTimer *m_endPointTimer;
    int i;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void onTimer(void);
private:
    bool rightBtnPressed;

    Point m_lastPoint;
    Point m_currentPoint;
    Point m_deltaPoint;

    static const int DOBOT_WORK_X = 320;
    static const int DOBOT_WORK_Y = 400;
    static const int DOBOT_WORK_Z = 320;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 640;

    float m_ratio_x;
    float m_ratio_y;

    QTime m_sysTime;//用来记录运行时间

    int m_lastSampleTime;//上一次采集时间戳,计算时间差用
    int m_startSampleTime;//第一次采样时间
    bool m_startCommand;//下发标志
    int m_startCommandTime;//第一次下发时间

    unsigned m_commandMask;

    int m_commandTime[10000];
    double m_commandX[10000];
    double m_commandY[10000];

    double m_deltaCommandX[10000];
    double m_deltaCommandY[10000];

    E2_CUB_SPLINE *m_splineX;
    E2_CUB_SPLINE *m_splineY;
#ifdef EXPORT_MOUSE_DATA
    unsigned m_sampleDataMask;//指示当前的采集数组水位
    int m_sampleData[EXPORT_SAMPLE_NUM][4];
#endif
};




#endif // CMOUSECONTROL_H
