#ifndef CMOUSECONTROL_H
#define CMOUSECONTROL_H

#include <QDialog>
#include <QVector>

class CMouseControlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMouseControlDialog(QWidget *parent = 0);
    ~CMouseControlDialog();
    quint32 startupFlag;
    QTimer *timer;
    int i;
    typedef struct tagPoint {
        float deltaX;
        float deltaY;
        float deltaZ;
    }Point;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void onTimer(void);
private:
    bool rightBtnPressed;
    QPoint lastPoint;
    QPoint deltaPoint;


    Point point;
    QVector<Point> xyzPoint;

};

#endif // CMOUSECONTROL_H
