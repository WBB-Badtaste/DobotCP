/*
  CPaintWidget.h - Dobot client main window
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

#ifndef CPAINTWIDGET_H
#define CPAINTWIDGET_H

#include <QWidget>

class CPaintWidget : public QWidget
{
    Q_OBJECT
public:
    CPaintWidget(QWidget *parent = 0);
public:
    void setData(const QList<QPolygonF> &routeList);
private:
    void offsetAllPoint(void);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    QList<QPolygonF> routeList;
    double routeHeight;
};

#endif // CPAINTWIDGET_H
