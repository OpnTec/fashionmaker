/************************************************************************
 **
 **  @file   vcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vcubicbezierpath.h"

#include <QLineF>
#include <QPoint>

#include "../ifc/exception/vexception.h"
#include "../vmisc/vmath.h"
#include "vabstractcurve.h"
#include "vcubicbezierpath_p.h"
#include "vspline.h"
#include "vsplinepoint.h"

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(quint32 idObject, Draw mode)
    : VAbstractCubicBezierPath(GOType::CubicBezierPath, idObject, mode),
      d(new VCubicBezierPathData())
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(const VCubicBezierPath &curve)
    : VAbstractCubicBezierPath(curve),
      d(curve.d)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(const QVector<VPointF> &points, quint32 idObject, Draw mode)
    : VAbstractCubicBezierPath(GOType::CubicBezierPath, idObject, mode),
      d(new VCubicBezierPathData())
{
    if (points.isEmpty())
    {
        return;
    }

    d->path = points;
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath &VCubicBezierPath::operator=(const VCubicBezierPath &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(curve);
    d = curve.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath VCubicBezierPath::Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix) const
{
    const QVector<VPointF> points = GetCubicPath();
    VCubicBezierPath curve;
    for(int i=0; i < points.size(); ++i)
    {
        curve.append(points.at(i).Rotate(originPoint, degrees));
    }
    curve.setName(name() + prefix);
    return curve;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath VCubicBezierPath::Flip(const QLineF &axis, const QString &prefix) const
{
    const QVector<VPointF> points = GetCubicPath();
    VCubicBezierPath curve;
    for(int i=0; i < points.size(); ++i)
    {
        curve.append(points.at(i).Flip(axis));
    }
    curve.setName(name() + prefix);
    return curve;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::~VCubicBezierPath()
{
}

//---------------------------------------------------------------------------------------------------------------------
VPointF &VCubicBezierPath::operator[](int indx)
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
const VPointF &VCubicBezierPath::at(int indx) const
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezierPath::append(const VPointF &point)
{
    if (d->path.size() > 0 && d->path.last() != point)
    {
        return;
    }

    d->path.append(point);
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::CountSubSpl() const
{
    return CountSubSpl(d->path.size());
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::CountPoints() const
{
    return d->path.size();
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezierPath::Clear()
{
    d->path.clear();
    SetDuplicate(0);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VCubicBezierPath::GetSpline(qint32 index) const
{
    if (CountPoints() < 4)
    {
        throw VException(tr("Not enough points to create the spline."));
    }

    if (index < 1 || index > CountSubSpl())
    {
        throw VException(tr("This spline does not exist."));
    }

    const qint32 base = SubSplOffset(index);

    // Correction the first control point of each next spline curve except for the first.
    QPointF p2 = d->path.at(base + 1);
    if (base + 1 > 1)
    {
        const QPointF b = d->path.at(base);
        QLineF foot1(b, d->path.at(base - 1));
        QLineF foot2(b, p2);

        foot2.setAngle(foot1.angle() + 180);
        p2 = foot2.p2();
    }

    VSpline spl(d->path.at(base), p2, d->path.at(base + 2), d->path.at(base + 3));
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezierPath::GetStartAngle() const
{
    if (CountSubSpl() > 0)
    {
        return GetSpline(1).GetStartAngle();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezierPath::GetEndAngle() const
{
    const qint32 count = CountSubSpl();
    if (count > 0)
    {
        return GetSpline(count).GetEndAngle();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSplinePoint> VCubicBezierPath::GetSplinePath() const
{
    const int size = CountSubSpl();
    QVector<VSplinePoint> splPoints(size+1);

    for (qint32 i = 1; i <= size; ++i)
    {
        const VSpline spl = GetSpline(i);

        {
            VSplinePoint p = splPoints.at(i-1);
            p.SetP(spl.GetP1());
            p.SetAngle2(spl.GetStartAngle(), spl.GetStartAngleFormula());
            p.SetLength2(spl.GetC1Length(), spl.GetC1LengthFormula());
            splPoints[i-1] = p;
        }

        {
            VSplinePoint p = splPoints.at(i);
            p.SetP(spl.GetP4());
            p.SetAngle1(spl.GetEndAngle(), spl.GetEndAngleFormula());
            p.SetLength1(spl.GetC2Length(), spl.GetC2LengthFormula());
            splPoints[i] = p;
        }
    }
    return splPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPointF> VCubicBezierPath::GetCubicPath() const
{
    return d->path;
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::CountSubSpl(qint32 size)
{
    if (size <= 0)
    {
        return 0;
    }
    return qFloor(qAbs((size - 4) / 3.0 + 1));
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::SubSplOffset(qint32 subSplIndex)
{
    if (subSplIndex <= 0)
    {
        return -1;
    }

    return (subSplIndex - 1) * 3;
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::SubSplPointsCount(qint32 countSubSpl)
{
    if (countSubSpl <= 0)
    {
        return 0;
    }

    return ((countSubSpl - 1) * 3 + 4);
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezierPath::FirstPoint() const
{
    if (not d->path.isEmpty())
    {
        return d->path.first();
    }
    else
    {
        return VPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezierPath::LastPoint() const
{
    const qint32 count = CountSubSpl();
    if (count >= 1)
    {
        return d->path.at(SubSplOffset(count) + 3);// Take last point of the last real spline
    }
    else
    {
        return VPointF();
    }
}
