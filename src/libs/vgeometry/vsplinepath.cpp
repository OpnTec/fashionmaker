/************************************************************************
 **
 **  @file   vsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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
#include "vsplinepath_p.h"
#include "../ifc/exception/vexception.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../../utils/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::VSplinePath(qreal kCurve, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::SplinePath, idObject, mode), d(new VSplinePathData(kCurve))
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::VSplinePath(const VSplinePath &splPath)
    : VAbstractCurve(splPath), d(splPath.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::~VSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePath::append(const VSplinePoint &point)
{
    if (d->path.size() > 0 && d->path.last().P().toQPointF() == point.P().toQPointF())
    {
        return;
    }

    d->path.append(point);
    QString name = splPath;
    name.append(QString("_%1").arg(d->path.first().P().name()));
    if (d->path.size() > 1)
    {
        name.append(QString("_%1").arg(d->path.last().P().name()));
    }
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VSplinePath::Count() const
{
    if (d->path.size() == 0)
    {
        return 0;
    }
    else
    {
        return d->path.size() - 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------
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
    VSpline spl(d->path.at(index-1).P(), d->path.at(index).P(), d->path.at(index-1).Angle2(),
                d->path.at(index).Angle1(), d->path.at(index-1).KAsm2(), d->path.at(index).KAsm1(), d->kCurve);
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VSplinePath::GetPath(PathDirection direction) const
{
    QPainterPath painterPath;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(d->path.at(i-1).P(), d->path.at(i).P(), d->path.at(i-1).Angle2(), d->path.at(i).Angle1(),
                    d->path.at(i-1).KAsm2(), d->path.at(i).KAsm1(), d->kCurve);
        painterPath.addPath(spl.GetPath(direction));
    }
    return painterPath;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VSplinePath::GetPoints() const
{
    QVector<QPointF> pathPoints;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(d->path.at(i-1).P(), d->path.at(i).P(), d->path.at(i-1).Angle2(), d->path.at(i).Angle1(),
                    d->path.at(i-1).KAsm2(), d->path.at(i).KAsm1(), d->kCurve);
        pathPoints += spl.GetPoints();
    }
    return pathPoints;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePath::GetLength() const
{
    qreal length = 0;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl(d->path.at(i-1).P(), d->path.at(i).P(), d->path.at(i-1).Angle2(), d->path.at(i).Angle1(),
                    d->path.at(i-1).KAsm2(), d->path.at(i).KAsm1(), d->kCurve);
        length += spl.GetLength();
    }
    return length;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePath::UpdatePoint(qint32 indexSpline, const SplinePointPosition &pos, const VSplinePoint &point)
{
    if (indexSpline < 1 || indexSpline > Count())
    {
        throw VException(tr("This spline does not exist."));
    }
    if (pos == SplinePointPosition::FirstPoint)
    {
        d->path[indexSpline-1] = point;
    }
    else
    {
        d->path[indexSpline] = point;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint VSplinePath::GetSplinePoint(qint32 indexSpline, SplinePointPosition pos) const
{
    if (indexSpline < 1 || indexSpline > Count())
    {
        throw VException(tr("This spline does not exist."));
    }
    if (pos == SplinePointPosition::FirstPoint)
    {
        return d->path.at(indexSpline-1);
    }
    else
    {
        return d->path.at(indexSpline);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath &VSplinePath::operator =(const VSplinePath &path)
{
    if ( &path == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(path);
    d = path.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint & VSplinePath::operator[](int indx)
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
const VSplinePoint &VSplinePath::at(int indx) const
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
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
        VSpline spl = VSpline(d->path.at(i-1).P(), d->path.at(i).P(), d->path.at(i-1).Angle2(), d->path.at(i).Angle1(),
                              d->path.at(i-1).KAsm2(), d->path.at(i).KAsm1(), d->kCurve);
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

//---------------------------------------------------------------------------------------------------------------------
int VSplinePath::Segment(const QPointF &p) const
{
    int index = -1;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        VSpline spl = VSpline(d->path.at(i-1).P(), d->path.at(i).P(), d->path.at(i-1).Angle2(), d->path.at(i).Angle1(),
                              d->path.at(i-1).KAsm2(), d->path.at(i).KAsm1(), d->kCurve);

        const qreal t = spl.ParamT(p);

        if (qFloor(t) == -1)
        {
            continue;
        }
        else
        {
            index = i;
            break;
        }
    }
    return index;
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VSplinePath::CountPoint() const
{
    return d->path.size();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSplinePoint> VSplinePath::GetSplinePath() const
{
    return d->path;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePath::Clear()
{
    d->path.clear();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePath::GetKCurve() const
{
    return d->kCurve;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePath::SetKCurve(const qreal &value)
{
    if (value > 0)
    {
        d->kCurve = value;
    }
}

//---------------------------------------------------------------------------------------------------------------------
const QVector<VSplinePoint> *VSplinePath::GetPoint() const
{
    return &d->path;
}
