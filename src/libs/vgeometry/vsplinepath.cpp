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
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSplinePath constructor.
 * @param idObject parent id.
 * @param mode mode creation spline path.
 */
VSplinePath::VSplinePath(quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::SplinePath, idObject, mode),
      d(new VSplinePathData())
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::VSplinePath(const QVector<VFSplinePoint> &points, qreal kCurve, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::SplinePath, idObject, mode),
      d(new VSplinePathData())
{
    if (points.size() < 3)
    {
        return;
    }

    QVector<VSplinePoint> newPoints;
    for (int i=0; i < points.size(); ++i)
    {
        newPoints.append(VSplinePoint());
    }

    for (qint32 i = 1; i <= points.size()-1; ++i)
    {
        const VFSplinePoint &p1 = points.at(i-1);
        const VFSplinePoint &p2 = points.at(i);
        VSpline spl(p1.P(), p2.P(), p1.Angle2(), p2.Angle1(), p1.KAsm2(), p2.KAsm1(), kCurve);

        newPoints[i-1].SetP(p1.P());
        newPoints[i-1].SetAngle2(p1.Angle2(), spl.GetStartAngleFormula());
        newPoints[i-1].SetLength2(spl.GetC1Length(), spl.GetC1LengthFormula());

        newPoints[i].SetP(p2.P());
        newPoints[i].SetAngle1(p2.Angle1(), spl.GetEndAngleFormula());
        newPoints[i].SetLength1(spl.GetC2Length(), spl.GetC2LengthFormula());
    }

    d->path = newPoints;
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::VSplinePath(const QVector<VSplinePoint> &points, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::SplinePath, idObject, mode),
      d(new VSplinePathData())
{
    if (points.isEmpty())
    {
        return;
    }

    d->path = points;
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSplinePath copy constructor.
 * @param splPath spline path.
 */
VSplinePath::VSplinePath(const VSplinePath &splPath)
    : VAbstractCurve(splPath),
      d(splPath.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath::~VSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief append add point in the end of list points.
 * @param point new point.
 */
void VSplinePath::append(const VSplinePoint &point)
{
    if (d->path.size() > 0 && d->path.last().P().toQPointF() == point.P().toQPointF()) //-V807
    {
        return;
    }

    d->path.append(point);
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Count return count point.
 * @return count.
 */
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
/**
 * @brief GetSpline return spline by index.
 * @param index index spline in spline path.
 * @return spline
 */
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
    const VSplinePoint &p1 = d->path.at(index-1);
    const VSplinePoint &p2 = d->path.at(index);
    VSpline spl(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(), p1.Length2(),
                p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1);
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPath return QPainterPath which reprezent spline path.
 * @return path.
 */
QPainterPath VSplinePath::GetPath(PathDirection direction) const
{
    QPainterPath painterPath;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        const VSplinePoint &p1 = d->path.at(i-1);
        const VSplinePoint &p2 = d->path.at(i);
        VSpline spl(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(), p1.Length2(),
                    p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1);
        painterPath.addPath(spl.GetPath(direction));
    }
    return painterPath;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPathPoints return list of points what located on path.
 * @return list.
 */
QVector<QPointF> VSplinePath::GetPoints() const
{
    QVector<QPointF> pathPoints;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        const VSplinePoint &p1 = d->path.at(i-1);
        const VSplinePoint &p2 = d->path.at(i);
        VSpline spl(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(), p1.Length2(),
                    p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1);
        pathPoints += spl.GetPoints();
    }
    return pathPoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return length of spline path.
 * @return length.
 */
qreal VSplinePath::GetLength() const
{
    qreal length = 0;
    for (qint32 i = 1; i <= Count(); ++i)
    {
        const VSplinePoint &p1 = d->path.at(i-1);
        const VSplinePoint &p2 = d->path.at(i);
        VSpline spl(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(), p1.Length2(),
                    p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1);
        length += spl.GetLength();
    }
    return length;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePoint update spline point in list.
 * @param indexSpline spline index in list.
 * @param pos position point in spline.
 * @param point point.
 */
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
/**
 * @brief GetSplinePoint return spline point from list.
 * @param indexSpline spline index in list.
 * @param pos position point in spline.
 * @return spline point.
 */
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
/**
 * @brief operator = assignment operator.
 * @param path spline path.
 * @return spline path.
 */
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
/**
 * @brief operator [] return spline point by index.
 * @param indx index in list.
 * @return spline point.
 */
VSplinePoint & VSplinePath::operator[](int indx)
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief at return spline point by index.
 * @param indx index in list.
 * @return spline point.
 */
const VSplinePoint &VSplinePath::at(int indx) const
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CutSplinePath cut spline path into two. This method don't return two spline path. You must create spline
 * paths by yourself.
 * Example:
 * QPointF spl1p2, spl1p3, spl2p2, spl2p3;
 * qint32 p1 = 0, p2 = 0;
 * QPointF point = splPath->CutSplinePath(length, p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
 *
 * VSplinePoint splP1 = splPath->at(p1);
 * VSplinePoint splP2 = splPath->at(p2);
 * VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->GetKCurve());
 * VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->GetKCurve());
 * @param length length first spline path.
 * @param p1 index first spline point in list.
 * @param p2 index second spline point in list.
 * @param spl1p2 first control point first spline.
 * @param spl1p3 second control point first spline.
 * @param spl2p2 first control point second spline.
 * @param spl2p3 second control point second spline.
 * @return cutting point.
 */
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
        const VSplinePoint &point1 = d->path.at(i-1);
        const VSplinePoint &point2 = d->path.at(i);
        VSpline spl = VSpline(point1.P(), point2.P(), point1.Angle2(), point1.Angle2Formula(), point2.Angle1(),
                              point2.Angle1Formula(), point1.Length2(), point1.Length2Formula(), point2.Length1(),
                              point2.Length1Formula(), 1);
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
        const VSplinePoint &p1 = d->path.at(i-1);
        const VSplinePoint &p2 = d->path.at(i);
        VSpline spl = VSpline(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(),
                              p1.Length2(), p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1.0);

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
qreal VSplinePath::GetStartAngle() const
{
    if (CountPoint() > 0)
    {
        return GetSplinePath().first().Angle1();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePath::GetEndAngle() const
{
    if (CountPoint() > 0)
    {
        return GetSplinePath().last().Angle2();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePath::CreateName()
{
    QString name;
    if (not d->path.isEmpty())
    {
        name = splPath;
        name.append(QString("_%1").arg(d->path.first().P().name()));
        if (d->path.size() > 1)
        {
            name.append(QString("_%1").arg(d->path.last().P().name()));
        }

        if (GetDuplicate() > 0)
        {
            name += QString("_%1").arg(GetDuplicate());
        }
    }
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CountPoint return count point.
 * @return count.
 */
qint32 VSplinePath::CountPoint() const
{
    return d->path.size();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSplinePath return list with spline points.
 * @return list.
 */
QVector<VSplinePoint> VSplinePath::GetSplinePath() const
{
    return d->path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFSplinePoint> VSplinePath::GetFSplinePath() const
{
    QVector<VFSplinePoint> points;
    points.reserve(d->path.size());

    for (qint32 i = 1; i <= Count(); ++i)
    {
        const VSplinePoint &p1 = d->path.at(i-1);
        const VSplinePoint &p2 = d->path.at(i);
        VSpline spl(p1.P(), p2.P(), p1.Angle2(), p1.Angle2Formula(), p2.Angle1(), p2.Angle1Formula(), p1.Length2(),
                    p1.Length2Formula(), p2.Length1(), p2.Length1Formula(), 1);

        points[i-1].SetP(p1.P());
        points[i-1].SetAngle2(p1.Angle2());
        points[i-1].SetKAsm2(spl.GetKasm1());

        points[i].SetP(p2.P());
        points[i].SetAngle1(p2.Angle1());
        points[i].SetKAsm1(spl.GetKasm2());
    }

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear list of points.
 */
void VSplinePath::Clear()
{
    d->path.clear();
    SetDuplicate(0);
}
