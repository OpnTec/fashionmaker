/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
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

#ifndef VABSTRACTPIECE_H
#define VABSTRACTPIECE_H

#include <QtGlobal>
#include <QSharedDataPointer>

template <class T> class QVector;
class QPointF;
class VAbstractPieceData;
class QLineF;

class VAbstractPiece
{
public:
    VAbstractPiece();
    VAbstractPiece(const VAbstractPiece &piece);
    VAbstractPiece &operator=(const VAbstractPiece &piece);
    virtual ~VAbstractPiece();

    QString GetName() const;
    void    SetName(const QString &value);

    bool IsForbidFlipping() const;
    void SetForbidFlipping(bool value);

    bool IsSeamAllowance() const;
    void SetSeamAllowance(bool value);

    qreal GetSAWidth() const;
    void  SetSAWidth(qreal value);

    static QVector<QPointF> Equidistant(const QVector<QPointF> &points, qreal width);
    static qreal            SumTrapezoids(const QVector<QPointF> &points);
    static QVector<QPointF> CheckLoops(const QVector<QPointF> &points);
    static QVector<QPointF> CorrectEquidistantPoints(const QVector<QPointF> &points, bool removeFirstAndLast = true);

protected:
    static QVector<QPointF> RemoveDublicates(const QVector<QPointF> &points, bool removeFirstAndLast = true);

private:
    QSharedDataPointer<VAbstractPieceData> d;

    static QVector<QPointF> EkvPoint(const QLineF &line1, const QLineF &line2, qreal width);
    static QLineF           ParallelLine(const QLineF &line, qreal width);
    static QPointF          SingleParallelPoint(const QLineF &line, qreal angle, qreal width);
    static int              BisectorAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3);
};

#endif // VABSTRACTPIECE_H
