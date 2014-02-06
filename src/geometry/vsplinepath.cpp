/************************************************************************
 **
 **  @file   vsplinepath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vsplinepath.h"
#include "../exception/vexception.h"

VSplinePath::VSplinePath(qreal kCurve, qint64 idObject, Draw::Draws mode)
    : VGObject(GObject::SplinePath, idObject, mode), path(QVector<VSplinePoint>()), kCurve(kCurve), maxCountPoints(0)
{
}

VSplinePath::VSplinePath(const VSplinePath &splPath)
    : VGObject(splPath), path(*splPath.GetPoint()), kCurve(splPath.getKCurve()),
      maxCountPoints(splPath.getMaxCountPoints())
{
}

void VSplinePath::append(const VSplinePoint &point)
{
    path.append(point);
    _name = QString("SplPath");
    _name.append(QString("_%1").arg(path.first().P().name()));
    if (path.size() > 1)
    {
        _name.append(QString("_%1").arg(path.last().P().name()));
    }
}

qint32 VSplinePath::Count() const
{
    if (path.size() == 0)
    {
        return 0;
    }
    else
    {
        return path.size() - 1;
    }
}

VSpline VSplinePath::GetSpline(qint32 index) const
{
    if (Count()<1)
    {
        throw VException(tr("Not enough points to create the spline."));
    }
    if (index < 1 || index > Count())
    {
        throw VException(tr("This spline does not exist."));
    }
    VSpline spl(path[index-1].P(), path[index].P(), path[index-1].Angle2(), path[index].Angle1(),
            path[index-1].KAsm2(), path[index].KAsm1(), this->kCurve);
    return spl;
}

QPainterPath VSplinePath::GetPath() const
{
    QPainterPath painterPath;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(),
                path[i-1].KAsm2(), path[i].KAsm1(), this->kCurve);
        painterPath.addPath(spl.GetPath());
    }
    return painterPath;
}

QVector<QPointF> VSplinePath::GetPathPoints() const
{
    QVector<QPointF> pathPoints;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(),
                path[i-1].KAsm2(), path[i].KAsm1(), this->kCurve);
        pathPoints += spl.GetPoints();
    }
    return pathPoints;
}

qreal VSplinePath::GetLength() const
{
    qreal length = 0;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(), path[i-1].KAsm2(),
                path[i].KAsm1(), kCurve);
        length += spl.GetLength();
    }
    return length;
}

void VSplinePath::UpdatePoint(qint32 indexSpline, const SplinePoint::Position &pos, const VSplinePoint &point)
{
    if (indexSpline < 1 || indexSpline > Count())
    {
        throw VException(tr("This spline does not exist."));
    }
    if (pos == SplinePoint::FirstPoint)
    {
        path[indexSpline-1] = point;
    }
    else
    {
        path[indexSpline] = point;
    }
}

VSplinePoint VSplinePath::GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos) const
{
    if (indexSpline < 1 || indexSpline > Count())
    {
        throw VException(tr("This spline does not exist."));
    }
    if (pos == SplinePoint::FirstPoint)
    {
        return path.at(indexSpline-1);
    }
    else
    {
        return path.at(indexSpline);
    }
}

VSplinePath &VSplinePath::operator =(const VSplinePath &path)
{
    VGObject::operator=(path);
    this->path = path.GetSplinePath();
    this->kCurve = path.getKCurve();
    this->maxCountPoints = path.getMaxCountPoints();
    return *this;
}

VSplinePoint & VSplinePath::operator[](ptrdiff_t indx)
{
    return path[indx];
}

const VSplinePoint &VSplinePath::at(ptrdiff_t indx) const
{
    return path[indx];
}

QPointF VSplinePath::CutSplinePath(qreal length, qint32 &p1, qint32 &p2, QPointF &spl1p2, QPointF &spl1p3,
                                   QPointF &spl2p2, QPointF &spl2p3) const
{
    if (Count() < 2)
    {
        throw VException(tr("Can't cut spline path with one point"));
    }

    //Always need return two spline paths, so we must correct wrong length.
    qreal fullLength = GetLength();
    if (length < fullLength * 0.02)
    {
        length = fullLength * 0.02;
    }
    else if ( length > fullLength * 0.98)
    {
        length = fullLength * 0.98;
    }

    fullLength = 0;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl = VSpline(path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(), path[i-1].KAsm2(),
                path[i].KAsm1(), kCurve);
        fullLength += spl.GetLength();
        if (fullLength > length)
        {
            p1 = i-1;
            p2 = i;
            return spl.CutSpline(length - (fullLength - spl.GetLength()), spl1p2, spl1p3, spl2p2, spl2p3);
        }
    }
    return QPointF();
}
qint32 VSplinePath::getMaxCountPoints() const
{
    return maxCountPoints;
}

void VSplinePath::setMaxCountPoints(const qint32 &value)
{
    maxCountPoints = value;
}
