/*
  CPLTProcess.cpp - PLT process
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

#include "CPLTProcess.h"
#include <QTextStream>

CPLTProcess::CPLTProcess()
  : plottersPerInch(1016.0),
    mmPerInch(25.4),
    plotters2mmRatio(mmPerInch / plottersPerInch)
{
    /*
     * PLT文件中设备单位与标准单位的换算关系如下：
     * 1016个绘图仪单位 = 1英寸
     * 40个绘图仪单位 = 1mm
     */
}

double CPLTProcess::plotters2mm(double plotters)
{
    return plotters * plotters2mmRatio;
}

void CPLTProcess::plt2pt(const QByteArray &line, QPointF &point)
{
    QByteArray validLine = line.right(line.count() - 2);
    QTextStream ts(validLine);

    ts >> point.rx() >> point.ry();
    point.rx() = plotters2mm(point.rx());
    point.ry() = plotters2mm(point.ry());
}

void CPLTProcess::operator()(QFile &file, QList<QPolygonF> &routeList)
{
    routeList.clear();
    QPolygonF pointList;
    QPointF previousPoint(0, 0);
    QPointF currentPoint(0, 0);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        if (line.contains("PD")) {
            plt2pt(line, currentPoint);
            if (currentPoint != previousPoint) {
                // 只添加长度不为零的点
                pointList.push_back(currentPoint);
            }
            previousPoint = currentPoint;
        } else if (line.contains("PU")) {
            plt2pt(line, currentPoint);
            if (pointList.count() > 1) {
                // 两个以下的点不能成为路径
                routeList.push_back(pointList);
            }
            pointList.clear();
            pointList.push_back(currentPoint);
            previousPoint = currentPoint;
        }
    }
    if (pointList.count() > 1) {
        // 添加最后一次的点成为路径
        routeList.push_back(pointList);
    }
}

