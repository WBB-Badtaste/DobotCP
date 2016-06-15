/*
  CConfigUserDefDialog.cpp - User defined parameter config dialog
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

#include "CConfigUserDefDialog.h"
#include <QtWidgets>
#include "DobotDll.h"

CConfigUserDefDialog::CConfigUserDefDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("User defined");
    setFixedSize(412, 250);
    initLayout();
}

CConfigUserDefDialog::~CConfigUserDefDialog()
{

}

void CConfigUserDefDialog::initLayout(void)
{
    static const char *strLut[] = {
        "Joint1Angle",
        "Joint2Angle",
        "Joint3Angle",
        "Joint4Angle"
    };
    static const char *unitLut[] = {
        "°",
        "°",
        "°",
        "°"
    };
    static const float paramLut[] = {
        0, 45, 45, 0
    };

    QFont font("微软雅黑", 8);

    int i = 0;
    for (i = 0; i < 4; i++) {
        QLabel *label = new QLabel(this);
        label->move(10, 10 + i * 40);
        label->resize(150, 30);
        label->setFont(font);
        label->setText(strLut[i]);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->move(180, 10 + i * 40);
        lineEdit->resize(100, 30);
        lineEdit->setFont(font);
        lineEdit->setText(QString("%1").arg(paramLut[i]));
        paramWidgetList << lineEdit;

        label = new QLabel(this);
        label->move(300, 10 + i * 40);
        label->resize(50, 20);
        label->setFont(font);
        label->setText(unitLut[i]);
    }

    QPushButton *sendBtn = new QPushButton(this);
    sendBtn->move(280, 10 + i * 40);
    sendBtn->resize(120, 50);
    sendBtn->setFont(font);
    sendBtn->setText("Send");
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(onSendBtnClicked()));
}

void CConfigUserDefDialog::onSendBtnClicked(void)
{
    QVector<float> params;

    int i = 0;
    for (i = 0; i < 4; i++) {
        QLineEdit *lineEdit = (QLineEdit *)paramWidgetList.at(i);
        params << lineEdit->text().toFloat();
    }
    InitialPose initialPose;
    initialPose.jong1Angle = params.at(0);
    initialPose.jong2Angle = params.at(1);
    initialPose.jong3Angle = params.at(2);
    initialPose.jong4Angle = params.at(3);
    SetInitialPose(&initialPose);

    accept();
}
