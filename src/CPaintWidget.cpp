/*
  CPaintWidget.cpp - Dobot client main window
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

#include "CPaintWidget.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <math.h>
#include <QGraphicsScene>
#include <QGraphicsView>

CPaintWidget::CPaintWidget(QWidget *parent)
  : QWidget(parent),
    routeHeight(0)
{
    QList<QScreen *> screens = QGuiApplication::screens();
    foreach(QScreen *screen, screens) {
        qDebug() << screen->physicalDotsPerInch() << screen->logicalDotsPerInch() << screen->physicalSize();
    }
}

void CPaintWidget::setData(const QList<QPolygonF> &routeList)
{
    this->routeList = routeList;
    offsetAllPoint();
    update();
}

void CPaintWidget::offsetAllPoint(void)
{
    // Offset all points!
    float minX = 1.0E10;
    float minY = 1.0E10;
    float maxX = -1.0E10;
    float maxY = -1.0E10;

    foreach (QPolygonF pointList, routeList) {
        foreach(QPointF point, pointList) {
            if (point.x() < minX) {
                minX = point.x();
            } else if (point.x() > maxX) {
                maxX = point.x();
            }
            if (point.y() < minY) {
                minY = point.y();
            } else if (point.y() > maxY) {
                maxY = point.y();
            }
        }
    }
    for (int i = 0; i < routeList.count(); i++) {
        QPointF *pointList = (QPointF *)routeList.at(i).data();
        for (int j = 0; j < routeList.at(i).count(); j++) {
            pointList[j].setX(pointList[j].x() - minX);
            pointList[j].setY(pointList[j].y() - minY);
        }
    }
    routeHeight = maxY - minY;
}

void CPaintWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    painter.fillRect(this->rect(), QColor(255, 255, 255));

    // 160mm / 25.4 = 6.25英寸, 6.25英寸 x DPI
    int screenNumber = QApplication::desktop()->screenNumber(this);
    float mmToInch = 25.4;
    float ratio = 1.0 / mmToInch * QGuiApplication::screens().at(screenNumber)->physicalDotsPerInch();
    double heightOffset = routeHeight * ratio;
    painter.translate(QPoint(0, (this->height() - heightOffset) / 2));
    painter.scale(ratio, ratio);

    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(255, 0, 0));
    pen.setWidthF(0.5f);
    painter.setPen(pen);

    foreach(QPolygonF line, routeList) {
        painter.drawPolyline(line);
    }
}
