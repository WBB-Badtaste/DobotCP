/*
  MainWindow.h - Dobot client main window
  Copyright (c) 2015-2016 Yuejiang Technology Co., Ltd. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include "DobotDll.h"

class QLineEdit;
class QTableWidget;
class QButtonGroup;
class QProgressBar;
class CPaintWidget;
class QLabel;


class MainWindow : public QDialog
{
    Q_OBJECT
private:
    typedef struct tagWriteData {
        float deltaX;
        float deltaY;
        float deltaZ;
        bool isLaserOn;

    }WriteData;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void wheelEvent(QWheelEvent *event);

private:
    void initLayout(void);
private slots:
    void onPenTypeChanged(int penType);

    void onLoadBtnClicked(void);
    void transformAllPoints(void);

    void onTextDesignBtnClicked(void);

    void onGetPoseTimer(void);
    void onPlaybackStartBtnVerify(void);
    void onStartBtnClicked(void);
    void onStopBtnClicked(void);
    void onConfigBtnClicked(void);
    void onUserDefBtnClicked(void);
    void onMouseBtnClicked(void);

    void onSendPlaybackTimer(void);

//    void onMouseTimer(void);

private slots:
    void delayedInit(void);
    void onPeriodicTaskTimer(void);

private:
    CPaintWidget *paintWidget;

    QButtonGroup *penType;
    QLineEdit *brushDown;
    QPushButton *loadBtn;
    QPushButton *textDesignBtn;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPushButton *configBtn;
    QPushButton *userDefBtn;
    QPushButton *mouseBtn;

    QProgressBar *sendProgress;
    QProgressBar *executeProgress;
private:
    QTimer *getPoseTimer;
    QTimer *getCPLoopLineTimer;
    QTimer *getCPBufferSizeTimer;
private:
    QList<QPolygonF> routeList;
    QVector<WriteData> writeList;

    // Playback loop line
    quint32 playbackTotalLoop;
    quint32 playbackTotalLine;
    quint32 playbackCurrentLoop;
    quint32 playbackCurrentLine;

    quint32 playbackSendStatus;
    quint32 mouseSendStatus;

    quint32 exeCurrentLoop, exeCurrentLine, exeCurrentLineState;

private:
    bool rightBtnPressed;
    QPoint lastPoint;
    QPoint deltaPoint;
    int lastX;
    int lastY;
    int deltaX;
    int deltaY;
    int deltaZ;
    QPoint z;
    QVector<CPBufferCmd> mousePoint;
};

#endif // MAINWINDOW_H
