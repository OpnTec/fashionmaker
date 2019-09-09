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
#include "vsapoint.h"
#include "testpath.h"

class VAbstractPieceData;
class QPainterPath;
class VGrainlineData;
class VContainer;
enum class LayoutGravity : qint8;
enum class CuttingTime :  unsigned char;

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

    uint GetPriority() const;
    void SetPriority(uint value);

    static QVector<QPointF> Equidistant(QVector<VSAPoint> points, qreal width, const QString &name);
    static qreal            SumTrapezoids(const QVector<QPointF> &points);
    static QVector<QPointF> CheckLoops(const QVector<QPointF> &points);
    static QVector<QPointF> EkvPoint(QVector<QPointF> points, const VSAPoint &p1Line1, const VSAPoint &p2Line1,
                                     const VSAPoint &p1Line2, const VSAPoint &p2Line2, qreal width,
                                     bool *needRollback = nullptr);
    static QLineF           ParallelLine(const VSAPoint &p1, const VSAPoint &p2, qreal width);
    static bool             IsAllowanceValid(const QVector<QPointF> &base, const QVector<QPointF> &allowance);

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
/**
 * @brief CorrectEquidistantPoints clear equivalent points and remove point on line from equdistant.
 * @param points list of points equdistant.
 * @return corrected list.
 */
template <class T>
QVector<T> VAbstractPiece::CorrectEquidistantPoints(const QVector<T> &points, bool removeFirstAndLast)
{
//    DumpVector(points, QStringLiteral("input.json.XXXXXX")); // Uncomment for dumping test data
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
    int next = -1;

    QVector<T> buf2;
    //Remove point on line
    for (qint32 i = 0; i < buf1.size(); ++i)
    {// In this case we alwayse will have bounded intersection, so all is need is to check if point i is on line.
     // Unfortunatelly QLineF::intersect can't be used in this case because of the floating-point accuraccy problem.
        if (prev == -1)
        {
            if (i == 0)
            {
                prev = buf1.size() - 1;
                const T &prevPoint = buf1.at(prev);
                const T &iPoint = buf1.at(i);
                if (iPoint == prevPoint)
                {
                    prev = buf1.size() - 2;
                }
            }
            else
            {
                prev = i-1;
            }
        }

        if (i == buf1.size() - 1)
        {
            next = 0;
            const T &nextPoint = buf1.at(next);
            const T &iPoint = buf1.at(i);
            if (iPoint == nextPoint)
            {
                next = 1;
            }
        }
        else
        {
            next = i+1;
        }

        const T &iPoint = buf1.at(i);
        const T &prevPoint = buf1.at(prev);
        const T &nextPoint = buf1.at(next);

        if (not IsEkvPointOnLine(iPoint, prevPoint, nextPoint))
        {
            buf2.append(iPoint);
            prev = -1;
        }
    }

    if (not buf2.isEmpty() && buf2.first() != buf2.last())
    {
        buf2.append(buf2.first());
    }

    buf2 = RemoveDublicates(buf2, false);

//    DumpVector(buf2, QStringLiteral("output.json.XXXXXX")); // Uncomment for dumping test data
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
