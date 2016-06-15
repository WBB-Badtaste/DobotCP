/*
  CPLTProcess.h - PLT process
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

#ifndef CPLTPROCESS_H
#define CPLTPROCESS_H

#include <QFile>
#include <QPolygonF>

class CPLTProcess
{
public:
    CPLTProcess();
    void operator()(QFile &file, QList<QPolygonF> &routeList);
private:
    double plotters2mm(double plotters);
    void plt2pt(const QByteArray &line, QPointF &point);
private:
    // Parameters used to transform plotters to mm
    const double plottersPerInch;
    const double mmPerInch;
    const double plotters2mmRatio;
private:

};

#endif // CPLTPROCESS_H
