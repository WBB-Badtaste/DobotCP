#ifndef CMOUSECONTROL_H
#define CMOUSECONTROL_H

#include <QDialog>
#include <QVector>

//#define EXPORT_MOUSE_DATA

#define COMMAND_DELAY 50
#define ENDPOINT_SMOOTH_DELAY 2

#define MANUAL_RATIO 2
#define POINTS_THRESHOLD 5

//#define SMOOTH_MODEL
#define SMOOTH_SEC_NUM 70

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
//    typedef struct tagPoint {
//        float deltaX;
//        float deltaY;
//        float deltaZ;
//    }Point;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void onTimer(void);
    void onEndPointSmooth(void);
private:
    bool rightBtnPressed;

    Point m_lastPoint;
    Point m_currentPoint;
    Point m_deltaPoint;

    static const unsigned HISTORY_POINTS_SIZE = SMOOTH_SEC_NUM;
    unsigned m_historyPointsChangeIndex;
    unsigned m_historyPointsSizeMask;
    Point m_historyPoints[HISTORY_POINTS_SIZE];
    Point m_sumOfPoints;
    inline const void SmoothPoint(Point &point);
    Point m_smoothPoint;

    static const int DOBOT_WORK_X = 320;
    static const int DOBOT_WORK_Y = 400;
    static const int DOBOT_WORK_Z = 320;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 640;

    float m_ratio_x;
    float m_ratio_y;

    unsigned startEndPointSingal = 3;

#ifdef EXPORT_MOUSE_DATA
    unsigned m_indexOfData;
    unsigned m_indexOfSmoothData;
    static const int EXPORT_DATA_SIZE = 1000;
    int m_lastSysTime;
    int m_exportData[EXPORT_DATA_SIZE][3];
    int m_smoothData[EXPORT_DATA_SIZE][3];
#endif
};




#endif // CMOUSECONTROL_H
