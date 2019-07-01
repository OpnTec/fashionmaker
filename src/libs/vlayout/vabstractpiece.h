/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VABSTRACTPIECE_H
#define VABSTRACTPIECE_H

#include <QtGlobal>
#include <QSharedDataPointer>
#include <QPointF>
#include <QDebug>

#include "../vmisc/diagnostic.h"
#include "../vmisc/def.h"
#include "../vgeometry/vgobject.h"

template <class T> class QVector;

class VAbstractPieceData;
class QPainterPath;
class VGrainlineData;
class VContainer;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

/**
 * @brief The VSAPoint class seam allowance point
 */
class VSAPoint : public QPointF
{
public:
    Q_DECL_CONSTEXPR VSAPoint();
    Q_DECL_CONSTEXPR VSAPoint(qreal xpos, qreal ypos);
    Q_DECL_CONSTEXPR explicit VSAPoint(QPointF p);

    Q_DECL_CONSTEXPR qreal GetSABefore() const;
                     qreal GetSABefore(qreal width) const;
                     void  SetSABefore(qreal value);

    Q_DECL_CONSTEXPR qreal GetSAAfter() const;
                     qreal GetSAAfter(qreal width) const;
                     void  SetSAAfter(qreal value);

    Q_DECL_CONSTEXPR PieceNodeAngle GetAngleType() const;
                     void           SetAngleType(PieceNodeAngle value);

    Q_DECL_CONSTEXPR bool IsManualPasskmarkLength() const;
    Q_DECL_RELAXED_CONSTEXPR void SetManualPasskmarkLength(bool value);

    Q_DECL_CONSTEXPR qreal GetPasskmarkLength() const;
    Q_DECL_RELAXED_CONSTEXPR void  SetPasskmarkLength(qreal value);

    qreal MaxLocalSA(qreal width) const;
    qreal PassmarkLength(qreal width) const;

    static const qreal passmarkFactor;
    static const qreal maxPassmarkLength;

private:
    qreal          m_before{-1};
    qreal          m_after{-1};
    PieceNodeAngle m_angle{PieceNodeAngle::ByLength};
    bool           m_manualPassmarkLength{false};
    qreal          m_passmarkLength{0};
};

Q_DECLARE_METATYPE(VSAPoint)
Q_DECLARE_TYPEINFO(VSAPoint, Q_MOVABLE_TYPE);

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos)
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint(QPointF p)
    : QPointF(p)
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetSABefore() const
{
    return m_before;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSABefore(qreal value)
{
    value < 0 ? m_before = -1 : m_before = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetSAAfter() const
{
    return m_after;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSAAfter(qreal value)
{
    value < 0 ? m_after = -1 : m_after = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline PieceNodeAngle VSAPoint::GetAngleType() const
{
    return m_angle;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetAngleType(PieceNodeAngle value)
{
    m_angle = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline bool VSAPoint::IsManualPasskmarkLength() const
{
    return m_manualPassmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_RELAXED_CONSTEXPR inline void VSAPoint::SetManualPasskmarkLength(bool value)
{
    m_manualPassmarkLength = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetPasskmarkLength() const
{
    return m_passmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_RELAXED_CONSTEXPR inline void VSAPoint::SetPasskmarkLength(qreal value)
{
    m_passmarkLength = value;
}

QT_WARNING_POP

class VAbstractPiece
{
public:
    VAbstractPiece();
    VAbstractPiece(const VAbstractPiece &piece);
    virtual ~VAbstractPiece();

    VAbstractPiece &operator=(const VAbstractPiece &piece);
#ifdef Q_COMPILER_RVALUE_REFS
    VAbstractPiece &operator=(VAbstractPiece &&piece) Q_DECL_NOTHROW { Swap(piece); return *this; }
#endif

    inline void Swap(VAbstractPiece &piece) Q_DECL_NOTHROW
    { std::swap(d, piece.d); }

    QString GetName() const;
    void    SetName(const QString &value);

    bool IsForbidFlipping() const;
    void SetForbidFlipping(bool value);

    bool IsForceFlipping() const;
    void SetForceFlipping(bool value);

    bool IsSeamAllowance() const;
    void SetSeamAllowance(bool value);

    bool IsSeamAllowanceBuiltIn() const;
    void SetSeamAllowanceBuiltIn(bool value);

    bool IsHideMainPath() const;
    void SetHideMainPath(bool value);

    qreal GetSAWidth() const;
    void  SetSAWidth(qreal value);

    qreal GetMx() const;
    void  SetMx(qreal value);

    qreal GetMy() const;
    void  SetMy(qreal value);

    static QVector<QPointF> Equidistant(QVector<VSAPoint> points, qreal width, const QString &name);
    static qreal            SumTrapezoids(const QVector<QPointF> &points);
    static QVector<QPointF> CheckLoops(const QVector<QPointF> &points);
    static QVector<QPointF> EkvPoint(QVector<QPointF> points, const VSAPoint &p1Line1, VSAPoint p2Line1,
                                     const VSAPoint &p1Line2, VSAPoint p2Line2, qreal width,
                                     bool *needRollback = nullptr);
    static QLineF           ParallelLine(const VSAPoint &p1, const VSAPoint &p2, qreal width);
    static bool             IsAllowanceValid(const QVector<QPointF> &base, const QVector<QPointF> &allowance);

    template <class T>
    static QVector<T> CorrectPathDistortion(QVector<T> path);

    template <class T>
    static QVector<T> CorrectEquidistantPoints(const QVector<T> &points, bool removeFirstAndLast = true);

    static QVector<QPointF> RollbackSeamAllowance(QVector<QPointF> points, const QLineF &cuttingEdge, bool *success);

    static QVector<QPointF> GrainlinePoints(const VGrainlineData &geom, const VContainer *pattern,
                                            const QRectF &boundingRect, qreal &dAng);

    friend QDataStream& operator<< (QDataStream& dataStream, const VAbstractPiece& piece);
    friend QDataStream& operator>> (QDataStream& dataStream, VAbstractPiece& piece);

protected:
    template <class T>
    static QVector<T> RemoveDublicates(const QVector<T> &points, bool removeFirstAndLast = true);
    static bool       IsEkvPointOnLine(const QPointF &iPoint, const QPointF &prevPoint, const QPointF &nextPoint);
    static bool       IsEkvPointOnLine(const VSAPoint &iPoint, const VSAPoint &prevPoint, const VSAPoint &nextPoint);

    static bool             IsItemContained(const QRectF &parentBoundingRect, const QVector<QPointF> &shape, qreal &dX,
                                        qreal &dY);
    static QVector<QPointF> CorrectPosition(const QRectF &parentBoundingRect, QVector<QPointF> points);
    static bool             FindGrainlineGeometry(const VGrainlineData& geom, const VContainer *pattern, qreal &length,
                                                  qreal &rotationAngle, QPointF &pos);
private:
    QSharedDataPointer<VAbstractPieceData> d;
};

Q_DECLARE_TYPEINFO(VAbstractPiece, Q_MOVABLE_TYPE);

//---------------------------------------------------------------------------------------------------------------------
template<class T>
QVector<T> VAbstractPiece::CorrectPathDistortion(QVector<T> path)
{
    if (path.size() < 3)
    {
        return path;
    }

    int prev = -1;
    for (qint32 i = 0; i < path.size(); ++i)
    {
        if (prev == -1)
        {
            i == 0 ? prev = path.size() - 1 : prev = i-1;
        }

        int next = i+1;
        if (i == path.size() - 1)
        {
            next = 0;
        }

        const QPointF &iPoint = path.at(i);
        const QPointF &prevPoint = path.at(prev);
        const QPointF &nextPoint = path.at(next);

        if (VGObject::IsPointOnLineSegment(iPoint, prevPoint, nextPoint))
        {
            const QPointF p = VGObject::CorrectDistortion(iPoint, prevPoint, nextPoint);
            path[i].setX(p.x());
            path[i].setY(p.y());
        }
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CorrectEquidistantPoints clear equivalent points and remove point on line from equdistant.
 * @param points list of points equdistant.
 * @return corrected list.
 */
template <class T>
QVector<T> VAbstractPiece::CorrectEquidistantPoints(const QVector<T> &points, bool removeFirstAndLast)
{
    if (points.size()<4)//Better don't check if only three points. We can destroy equidistant.
    {
        qDebug()<<"Only three points.";
        return points;
    }

    //Clear equivalent points
    QVector<T> buf1 = RemoveDublicates(points, removeFirstAndLast);

    if (buf1.size()<3)
    {
        return buf1;
    }

    int prev = -1;

    QVector<T> buf2;
    //Remove point on line
    for (qint32 i = 0; i < buf1.size(); ++i)
    {// In this case we alwayse will have bounded intersection, so all is need is to check if point i is on line.
     // Unfortunatelly QLineF::intersect can't be used in this case because of the floating-point accuraccy problem.
        if (prev == -1)
        {
            prev = (i == 0) ?  buf1.size() - 1 : i-1;
        }

        const int next = (i == buf1.size() - 1) ? 0 : i+1;

        const T &iPoint = buf1.at(i);
        const T &prevPoint = buf1.at(prev);
        const T &nextPoint = buf1.at(next);

        if (not (IsEkvPointOnLine(iPoint, prevPoint, nextPoint) && prevPoint == nextPoint/*not zigzag*/)
                // If RemoveDublicates does not remove these points it is a valid case.
                // Case where last point equal first point
                || ((i == 0 || i == buf1.size() - 1) && (iPoint == prevPoint || iPoint == nextPoint)))
        {
            buf2.append(iPoint);
            prev = -1;
        }
    }

    buf2 = RemoveDublicates(buf2, false);

    return buf2;
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
QVector<T> VAbstractPiece::RemoveDublicates(const QVector<T> &points, bool removeFirstAndLast)
{
    QVector<T> p = points;

    if (removeFirstAndLast)
    {
        if (not p.isEmpty() && p.size() > 1)
        {
            // Path can't be closed
            // See issue #686
            if (VFuzzyComparePoints(p.first(), p.last()))
            {
                p.removeLast();
            }
        }
    }

    for (int i = 0; i < p.size()-1; ++i)
    {
        if (VFuzzyComparePoints(p.at(i), p.at(i+1)))
        {
            if (not removeFirstAndLast && (i == p.size()-1))
            {
                continue;
            }

            p.erase(p.begin() + i + 1);
            --i;
            continue;
        }
    }

    return p;
}

#endif // VABSTRACTPIECE_H
