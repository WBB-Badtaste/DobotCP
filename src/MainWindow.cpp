/*
  MainWindow.cpp - Dobot client main window
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

#include "MainWindow.h"
#include <QtWidgets>
#include <QDebug>
#include "CPaintWidget.h"
#include "CTextDesignDialog.h"
#include <QProcess>
#include <QApplication>
#include "ProtocolID.h"
#include "CConfigCPDialog.h"
#include "CConfigUserDefDialog.h"
#include "CPLTProcess.h"
#include "CMouseControlDialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    initLayout();

    QTimer *delayedInitTimer = new QTimer(this);
    connect(delayedInitTimer, SIGNAL(timeout()), this, SLOT(delayedInit()));
    delayedInitTimer->setSingleShot(true);
    delayedInitTimer->start(0);

}

MainWindow::~MainWindow()
{

}

void MainWindow::delayedInit(void)
{
    if (ConnectDobot(115200) != DobotResult_NoError) {
        QMessageBox::information(this, "Error", "Failed to connect!");

        return;
    }

    QTimer *periodicTaskTimer = new QTimer(this);
    connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
    periodicTaskTimer->start(5);

    // Timer used to get pose timer
    getPoseTimer = new QTimer(this);
    getPoseTimer->setObjectName("getPoseTimer");
    connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
    getPoseTimer->start(500);

    CConfigCPDialog cpDlg;
    cpDlg.onSendBtnClicked();
    CConfigUserDefDialog userDefDlg;
    userDefDlg.onSendBtnClicked();
}

void MainWindow::onPeriodicTaskTimer(void)
{
    PeriodicTask();
}

void MainWindow::initLayout(void)
{
    setFixedSize(1000, 600);

    QFont font(QFont("微软雅黑", 8));

    // Init the paint widget
    paintWidget = new CPaintWidget(this);
    paintWidget->move(5, 5);
    paintWidget->resize(800, 500);

    // Init the end type selection
    QRadioButton *normalPen = new QRadioButton(this);
    normalPen->move(835, 5);
    normalPen->resize(130, 25);
    normalPen->setFont(font);
    normalPen->setText("Normal pen");

    QRadioButton *brushPen = new QRadioButton(this);
    brushPen->move(835, 35);
    brushPen->resize(130, 25);
    brushPen->setFont(font);
    brushPen->setText("Brush pen");

    QRadioButton *laserPen = new QRadioButton(this);
    laserPen->move(835, 65);
    laserPen->resize(130, 25);
    laserPen->setFont(font);
    laserPen->setText("Laser pen");
    laserPen->setChecked(true);

    penType = new QButtonGroup(this);
    penType->setObjectName("penType");
    penType->addButton(normalPen, 0);
    penType->addButton(brushPen, 1);
    penType->addButton(laserPen, 2);

    connect(penType, SIGNAL(buttonClicked(int)),  this, SLOT(onPenTypeChanged(int)));

    // Brush down parameters
    QLabel *nameLabel = new QLabel(this);
    nameLabel->move(835, 95);
    nameLabel->resize(130, 25);
    nameLabel->setAlignment(Qt::AlignLeft);
    nameLabel->setText("Brush down");

    brushDown = new QLineEdit(this);
    brushDown->setObjectName("brushDown");
    brushDown->move(835, 125);
    brushDown->resize(100, 25);
    brushDown->setText("20");
    QRegExp regExp("^(-)?([0-9]|[1-4][0-9])(\\.\\d{1,2})?$");
    brushDown->setValidator(new QRegExpValidator(regExp, brushDown));

    QLabel *unitLabel = new QLabel(this);
    unitLabel->move(935, 125);
    unitLabel->resize(30, 25);
    unitLabel->setAlignment(Qt::AlignCenter);
    unitLabel->setText("mm");

    onPenTypeChanged(penType->checkedId());

    // Init the buttons
    // Load button
    loadBtn = new QPushButton(this);
    loadBtn->move(835, 155);
    loadBtn->resize(130, 50);
    loadBtn->setFont(font);
    loadBtn->setText("Load plt...");
    loadBtn->setEnabled(true);
    connect(loadBtn, SIGNAL(clicked()), this, SLOT(onLoadBtnClicked()));

    // TextDesign
    textDesignBtn = new QPushButton(this);
    textDesignBtn->move(835, 210);
    textDesignBtn->resize(130, 50);
    textDesignBtn->setFont(font);
    textDesignBtn->setText("TextDesign...");
    textDesignBtn->setEnabled(true);
    connect(textDesignBtn, SIGNAL(clicked()), this, SLOT(onTextDesignBtnClicked()));

    // Start button
    startBtn = new QPushButton(this);
    startBtn->setObjectName("startBtn");
    startBtn->move(835, 265);
    startBtn->resize(130, 50);
    startBtn->setFont(font);
    startBtn->setText("Start");
    startBtn->setEnabled(false);
    connect(startBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));

    // Stop button
    stopBtn = new QPushButton(this);
    stopBtn->setObjectName("stopBtn");
    stopBtn->move(835, 320);
    stopBtn->resize(130, 50);
    stopBtn->setFont(font);
    stopBtn->setText("Stop");
    stopBtn->setEnabled(false);
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(onStopBtnClicked()));

    // Config button
    configBtn = new QPushButton(this);
    configBtn->setObjectName("configBtn");
    configBtn->move(835, 375);
    configBtn->resize(130, 50);
    configBtn->setFont(font);
    configBtn->setText("Config...");
    connect(configBtn, SIGNAL(clicked()), this, SLOT(onConfigBtnClicked()));

    // User defined button
    userDefBtn = new QPushButton(this);
    userDefBtn->setObjectName("userDefBtn");
    userDefBtn->move(835, 430);
    userDefBtn->resize(130, 50);
    userDefBtn->setFont(font);
    userDefBtn->setText("UserDef...");
    connect(userDefBtn, SIGNAL(clicked()), this, SLOT(onUserDefBtnClicked()));

    //Mouse control button
    mouseBtn = new QPushButton(this);
    mouseBtn->setObjectName("mouseBtn");
    mouseBtn->move(835, 485);
    mouseBtn->resize(130, 50);
    mouseBtn->setFont(font);
    mouseBtn->setText("&mouse");
    connect(mouseBtn, SIGNAL(clicked()), this, SLOT(onMouseBtnClicked()));


    // Progress bar
    // Send Progress
    QLabel *label = new QLabel(this);
    label->move(5, 510);
    label->resize(100, 35);
    label->setText("Send");

    sendProgress = new QProgressBar(this);
    sendProgress->setObjectName("sendProgress");
    sendProgress->move(110, 510);
    sendProgress->resize(730, 35);

    // Execute Progress
    label = new QLabel(this);
    label->move(5, 550);
    label->resize(100, 35);
    label->setText("Execute");

    executeProgress = new QProgressBar(this);
    executeProgress->setObjectName("executeProgress");
    executeProgress->move(110, 550);
    executeProgress->resize(730, 35);
}

void MainWindow::onPenTypeChanged(int penType)
{
    brushDown->setEnabled(penType == 1);
}

void MainWindow::onLoadBtnClicked(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open plt files"), "", tr("PLT file(*.plt)"));
    if (fileName == QString("")) {
        qDebug() << "Open file canceled!";
        return;
    }
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly) == false) {
        QMessageBox::warning(this, tr("Warning"), "Failed to open file!");
        return;
    }
    CPLTProcess pltProcess;
    pltProcess(file, routeList);
    if (routeList.count() == 0) {
        QMessageBox::warning(this, tr("Warning"), "No valid point!");
        return;
    }
    paintWidget->setData(routeList);
    onPlaybackStartBtnVerify();
}

void MainWindow::transformAllPoints(void)
{
    bool isLaser = penType->checkedId() == 2;

    writeList.clear();

    WriteData lastData;
    WriteData writeData;

    switch (penType->checkedId()) {
        case 0:
            // Normal pen
            lastData.deltaX = routeList.at(0).at(0).x();
            lastData.deltaY = routeList.at(0).at(0).y();
            lastData.deltaZ = 0;
            lastData.isLaserOn = false;
            writeList.push_back(lastData);
        break;

        case 1:
            // Brush, 先下压
            lastData.deltaX = routeList.at(0).at(0).x();
            lastData.deltaY = routeList.at(0).at(0).y();
            lastData.deltaZ = brushDown->text().toFloat();
            lastData.isLaserOn = false;
            writeList.push_back(lastData);
        break;

        case 2:
            // 激光
            lastData.deltaX = routeList.at(0).at(0).x();
            lastData.deltaY = routeList.at(0).at(0).y();
            lastData.deltaZ = 0;
            lastData.isLaserOn = true;
            writeList.push_back(lastData);
        break;
    }

    // First point
    lastData.deltaX = routeList.at(0).at(0).x();
    lastData.deltaY = routeList.at(0).at(0).y();
    lastData.deltaZ = 0;
    lastData.isLaserOn = false;

    int routeNum = routeList.count();
    for (int i = 0; i < routeNum; i++) {
        if (i != 0) {
            if (penType->checkedId() == 2) {
                writeData.deltaX = routeList.at(i).at(0).x();
                writeData.deltaY = routeList.at(i).at(0).y();
                writeData.deltaZ = 0;
                writeData.isLaserOn = false;
                writeList.push_back(writeData);
            } else {
                // 换路径了，抬笔
                writeData.deltaX = lastData.deltaX;
                writeData.deltaY = lastData.deltaY;
                writeData.deltaZ = 10;
                writeData.isLaserOn = false;
                writeList.push_back(writeData);

                lastData = writeData;

                // 换到XYZ
                writeData.deltaX = routeList.at(i).at(0).x();
                writeData.deltaY = routeList.at(i).at(0).y();
                writeData.deltaZ = 10;
                writeData.isLaserOn = false;
                writeList.push_back(writeData);

                lastData = writeData;
            }
        }
        foreach (QPointF point, routeList.at(i)) {
            writeData.deltaX = point.x();
            writeData.deltaY = point.y();
            writeData.deltaZ = 0;
            writeData.isLaserOn = isLaser;
            writeList.push_back(writeData);

            lastData = writeData;
        }
    }
    lastData = writeList.at(0);
    writeList.removeFirst();
    MainWindow::WriteData *data = (MainWindow::WriteData *)writeList.data();
    WriteData temp;
    for (int i = 0; i < writeList.count(); i++) {
        temp = data[i];
        data[i].deltaX -= lastData.deltaX;
        data[i].deltaY -= lastData.deltaY;
        data[i].deltaZ -= lastData.deltaZ;

        lastData = temp;
    }
    // 最后的抬笔，如果是激光，则关闭激光即可
    switch (penType->checkedId()) {
        case 0:
            writeData.deltaX = 0;
            writeData.deltaY = 0;
            writeData.deltaZ = 10;
            writeData.isLaserOn = false;
            writeList.push_back(writeData);
        break;

        case 1:
            writeData.deltaX = 0;
            writeData.deltaY = 0;
            writeData.deltaZ = 10 + brushDown->text().toFloat();
            writeData.isLaserOn = false;
            writeList.push_back(writeData);
        break;

        case 2:
            writeData.deltaX = 0;
            writeData.deltaY = 0;
            writeData.deltaZ = 0;
            writeData.isLaserOn = false;
            writeList.push_back(writeData);
        break;
    }

    // 去除太小的点
    QVector<WriteData> tempWriteList;
    foreach(WriteData writeData, writeList) {
        double size = writeData.deltaX * writeData.deltaX + writeData.deltaY * writeData.deltaY + writeData.deltaZ * writeData.deltaZ;
        size = sqrt(size);
        if (size > 0.1) {
            tempWriteList.push_back(writeData);
        }
    }
    writeList = tempWriteList;
}

void MainWindow::onTextDesignBtnClicked(void)
{
    CTextDesignDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        routeList = dlg.getSubPathPolygons();
        if (routeList.count() == 0) {
            return;
        }
        paintWidget->setData(routeList);
        onPlaybackStartBtnVerify();
    }
}

// Always running now
void MainWindow::onGetPoseTimer(void)
{
    Pose pose;
    GetPose(&pose);
    //qDebug() <<
}

void MainWindow::onPlaybackStartBtnVerify(void)
{
    startBtn->setEnabled(routeList.count() != 0);
}

void MainWindow::onStartBtnClicked(void)
{
    QPushButton *startBtn = findChild<QPushButton *>("startBtn");

    if(startBtn->text() == QString(tr("Start"))){
        playbackSendStatus = CPStart;
        setCPInstantCmd((CPInstantCmd)playbackSendStatus);
        onSendPlaybackTimer();
    }else if(startBtn->text() == QString(tr("pause"))){
        startBtn->setText(tr("Resume"));
        playbackSendStatus = CPPause;
        setCPInstantCmd((CPInstantCmd)playbackSendStatus);
        bool isLaser = penType->checkedId() == 2;
        if(isLaser){
            SetIOState(0);
        }
    }else if(startBtn->text() == QString(tr("Resume"))){
        startBtn->setText(tr("Pause"));
        playbackSendStatus = CPStart;
        setCPInstantCmd((CPInstantCmd)playbackSendStatus);
        bool isLaser = penType->checkedId() == 2;
        if(isLaser){
            SetIOState(1);
        }
    }
}

void MainWindow::onSendPlaybackTimer()
{
    qDebug() << startBtn->text() << "button pressed";

    startBtn->setText(tr("Pause"));
    stopBtn->setEnabled(true);

    transformAllPoints();

    // Calculate the total count of loop & line
    playbackTotalLoop = 1;
    playbackTotalLine = writeList.count();
    playbackCurrentLoop = 0;
    playbackCurrentLine = 0;

    // Set the rang of progressbar
    sendProgress->setValue(0);
    sendProgress->setRange(0, playbackTotalLoop * playbackTotalLine);

    executeProgress->setValue(0);
    executeProgress->setRange(0, playbackTotalLoop * playbackTotalLine);

    // !!!! Start communication
    quint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while (playbackSendStatus != CPStop) {
        do {
            // Check send status
            if (playbackSendStatus == CPPause) {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
                break;
            }
            //send the command then start the control
            qDebug() << "Check buffer size";
            uint32_t bufferSize;
            GetCPBufferSize(&bufferSize);
            if (bufferSize == 0) {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
                break;
            }
            if (playbackCurrentLoop == playbackTotalLoop) {
                break;
            }
            qDebug() << "Total line:" << playbackTotalLine << playbackCurrentLine;
            //get the points
            WriteData writeData = writeList.at(playbackCurrentLine);
            CPBufferCmd cpBufferCmd;
            cpBufferCmd.loop = playbackCurrentLoop;
            cpBufferCmd.line = playbackCurrentLine;
            cpBufferCmd.x = writeData.deltaX;
            cpBufferCmd.y = writeData.deltaY;
            cpBufferCmd.z = writeData.deltaZ;
            cpBufferCmd.velocity = 100;
            cpBufferCmd.ioState = writeData.isLaserOn;
            qDebug() << "Before sending...";
            SetCPBufferCmd(&cpBufferCmd);
            qDebug() << "After sending...";
            playbackCurrentLine++;

            if(playbackCurrentLine == playbackTotalLine){
                playbackCurrentLine = 0;
                playbackCurrentLoop++;
            }

            sendProgress->setValue(playbackCurrentLoop * playbackTotalLine + playbackCurrentLine);
        }while(0);
        quint64 lastestTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                if (lastestTime - currentTime > 250/*ms*/) {
                    currentTime = lastestTime;
                    // Current loop line state

                    GetCPCurrentLoopLine(&exeCurrentLoop, &exeCurrentLine, &exeCurrentLineState);

                    executeProgress->setValue(exeCurrentLoop * playbackTotalLine + (exeCurrentLine + 1));

                    if (exeCurrentLoop == playbackTotalLoop - 1 and
                        exeCurrentLine == playbackTotalLine - 1 and
                        exeCurrentLineState == 1) {
                        // True stop
                        break;
                    }
                }
    }


    startBtn->setText("Start");
    stopBtn->setEnabled(false);

    // Set the value of progressbar
    sendProgress->setValue(playbackTotalLoop * playbackTotalLine);
    executeProgress->setValue(playbackTotalLoop * playbackTotalLine);


}

void MainWindow::onStopBtnClicked(void)
{
    startBtn->setText("Start");
    stopBtn->setEnabled(false);

    // ProgressBar
    sendProgress->setValue(0);

    executeProgress->setValue(0);

    playbackSendStatus = CPStop;
    setCPInstantCmd((CPInstantCmd)playbackSendStatus);
}

void MainWindow::onConfigBtnClicked(void)
{
    CConfigCPDialog dlg;
    dlg.exec();
}

void MainWindow::onUserDefBtnClicked(void)
{
    CConfigUserDefDialog dlg;
    dlg.exec();
}

void MainWindow::onMouseBtnClicked(void)
{
//    if(mouseBtn->text() == QString("mouse")){
//        mouseBtn->setText("stop");
//        mouseSendStatus = CPStart;
//    }else if(mouseBtn->text() == QString("stop")){
//        mouseBtn->setText("mouse");
//        mouseSendStatus = CPStop;
//        setCPInstantCmd((CPInstantCmd)mouseSendStatus);
//    }

    setCPInstantCmd((CPInstantCmd)CPStart);
    CMouseControlDialog dlg;
    dlg.exec();

}

#if 0

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightBtnPressed = true;
        qDebug() << "Right button pressed!";
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightBtnPressed = false;
        qDebug() << "Right button released!";
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    z = event->angleDelta() / 8;
    deltaZ = z.y();
    qDebug() << deltaZ;

}

void MainWindow::onMouseTimer()
{
    uint32_t cpBufferCmd;
//    GetCPBufferSize(&cpBufferCmd);
    if(cpBufferCmd == 0){
        qDebug() << "cpBufferCmd == 0";
    }

    qDebug() << "11";
    QPoint pos = QCursor::pos();
    int currentX = pos.x();
    int currentY = pos.y();

    static quint32 startupFlag = 1;
    if(startupFlag)
    {
        startupFlag = 0;
        lastX = currentX;
        lastY = currentY;
    }


    qDebug() << lastX;
    qDebug() << currentX;
    deltaX += lastX - currentX;
    qDebug() << deltaX;

    qDebug() << lastY;
    qDebug() << currentY;
    deltaY += lastY - currentY;
    qDebug() << deltaY;
    lastX = currentX;
    lastY = currentY;

    if(deltaX != 0 || deltaY != 0 || deltaZ != 0)
    {
        qDebug() << "x:" << currentX << "\ny:" << currentY;
        qDebug() << deltaX << deltaY << deltaZ;
        CPBufferCmd cpBufferCmd;
        cpBufferCmd.loop = 0;
        cpBufferCmd.line = 0;
        cpBufferCmd.x = deltaX / 10;
        cpBufferCmd.y = deltaY / 10;
        cpBufferCmd.z = deltaZ;
        cpBufferCmd.velocity = 100;
        cpBufferCmd.ioState = 0;

//        SetCPBufferCmd(&cpBufferCmd);
    }

}
#endif
