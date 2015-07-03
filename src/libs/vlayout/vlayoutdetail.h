/************************************************************************
 **
 **  @file   vlayoutdetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
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

#ifndef VLAYOUTDETAIL_H
#define VLAYOUTDETAIL_H

#include "vabstractdetail.h"

#include <QMatrix>
#include <QPointF>

class VLayoutDetailData;
class QGraphicsItem;

class VLayoutDetail :public VAbstractDetail
{
public:
    VLayoutDetail();
    VLayoutDetail(const VLayoutDetail &detail);
    VLayoutDetail &operator=(const VLayoutDetail &detail);
    virtual ~VLayoutDetail() Q_DECL_OVERRIDE;

    QVector<QPointF> GetContourPoints() const;
    void SetCountourPoints(const QVector<QPointF> &points);

    QVector<QPointF> GetSeamAllowencePoints() const;
    void SetSeamAllowencePoints(const QVector<QPointF> &points, bool seamAllowence = true);

    QVector<QPointF> GetLayoutAllowencePoints() const;
    void SetLayoutAllowencePoints();

    QTransform GetMatrix() const;
    void    SetMatrix(const QTransform &matrix);

    qreal GetLayoutWidth() const;
    void  SetLayoutWidth(const qreal &value);

    bool IsMirror() const;
    void SetMirror(bool value);

    void Translate(qreal dx, qreal dy);
    void Rotate(const QPointF &originPoint, qreal degrees);
    void Mirror(const QLineF &edge);

    int    EdgesCount() const;
    QLineF Edge(int i) const;
    int    EdgeByPoint(const QPointF &p1) const;

    QRectF BoundingRect() const;

    bool isNull() const;
    qint64 Square() const;
    QPainterPath ContourPath() const;
    QGraphicsItem *GetItem() const;

private:
    QSharedDataPointer<VLayoutDetailData> d;

    QVector<QPointF> Map(const QVector<QPointF> &points) const;
    static QVector<QPointF> RoundPoints(const QVector<QPointF> &points);
};

#endif // VLAYOUTDETAIL_H
