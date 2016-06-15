/*
  CTextDesignDialog.cpp - Text design dialog
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

#include "CTextDesignDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFontDialog>

CTextDesignDialog::CTextDesignDialog(QWidget *parent)
  : QDialog(parent),
    designedText(""),
    selectedFont("微软雅黑", 36, QFont::Normal, false)
{
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setText(designedText);
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onDesignedTextChanged(QString)));
    hLayout1->addWidget(lineEdit);
    QPushButton *fontBtn = new QPushButton(this);
    fontBtn->setText("Font");
    connect(fontBtn, SIGNAL(clicked()), this, SLOT(onFontBtnClicked()));
    hLayout1->addWidget(fontBtn);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton(this);
    okBtn->setText("OK");
    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    hLayout2->addWidget(okBtn);

    QPushButton *cancelBtn = new QPushButton(this);
    cancelBtn->setText("Cancel");
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
    hLayout2->addWidget(cancelBtn);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(hLayout1);
    layout->addLayout(hLayout2);
    setLayout(layout);
}

void CTextDesignDialog::onDesignedTextChanged(const QString &str)
{
    designedText = str;
}

void CTextDesignDialog::onFontBtnClicked(void)
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, selectedFont, this, "Select a font");
    if (ok) {
        selectedFont = font;
    }
}

QList<QPolygonF> CTextDesignDialog::getSubPathPolygons(void)
{
    QPainterPath path;
    QPoint point(0, 0);
    path.addText(point, selectedFont, designedText);
    QList<QPolygonF> routeList = path.toSubpathPolygons();
    // Transfrom point to mm
    double pointsPerInch = 72;
    double mmPerInch = 25.4;
    double ratio = mmPerInch / pointsPerInch;
    for (int i = 0; i < routeList.count(); i++) {
        for (int j = 0; j < routeList.at(i).count(); j++) {
            routeList[i][j].setX(routeList[i][j].x() * ratio);
            routeList[i][j].setY(routeList[i][j].y() * ratio);
        }
    }
    return routeList;
}
