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
#include "DobotDll.h"

class QLineEdit;
class QTableWidget;
class QButtonGroup;
class QProgressBar;
class CPaintWidget;

class MainWindow : public QDialog
{
    Q_OBJECT
private:
    typedef struct tagWriteData {
        float deltaX;
        float deltaY;
        float deltaZ;
        bool isLaserChange;
        bool isLaserOn;
    }WriteData;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initLayout(void);
private slots:
    void delayedInit(void);
    void secondDelayedInit(void);
    void onPeriodicTaskTimer(void);
private slots:
    void onPenTypeChanged(int penType);

    void onLoadBtnClicked(void);
    void transformAllPoints(void);

    void onTextDesignBtnClicked(void);

    void onGetPoseTimer(void);
    void onPlaybackStartBtnVerify(void);
    void onStartBtnClicked(void);
    void onStopBtnClicked(void);
    void onSendCPTimer(void);

    void onConfigBtnClicked(void);
    void onUserDefBtnClicked(void);
    void onMouseBtnClicked(void);
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
private:
    QList<QPolygonF> routeList;
    QVector<WriteData> writeList;

    quint32 queuedCmdExecCtrl;
};

#endif // MAINWINDOW_H
