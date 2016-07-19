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
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vtools/tools/vtextmanager.h"

#include <QMatrix>
#include <QPointF>
#include <QDate>

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
    void SetSeamAllowencePoints(const QVector<QPointF> &points, bool seamAllowence = true, bool closed = true);

    QVector<QPointF> GetLayoutAllowencePoints() const;
    void SetLayoutAllowencePoints();

    void SetDetail(const QString &qsName, const VPatternPieceData& data, const QFont& font);

    void SetPatternInfo(const QString& qsPattern, const QString& qsNumber, const QString& qsSize,
                        const QString& qsCompany, const QString& qsCustomer, const QDate& date,
                        const VPatternInfoGeometry& geom, const QFont& font);

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

    QRectF DetailBoundingRect() const;
    QRectF LayoutBoundingRect() const;
    qreal  Diagonal() const;

    bool isNull() const;
    qint64 Square() const;
    QPainterPath ContourPath() const;
    void ClearTextItems();
    void CreateTextItems();
    int GetTextItemsCount() const;
    QGraphicsItem* GetTextItem(int i) const;
    QPainterPath LayoutAllowencePath() const;
    QGraphicsItem *GetItem() const Q_REQUIRED_RESULT;

private:
    QSharedDataPointer<VLayoutDetailData>   d;
    VTextManager                            m_tmDetail;
    VTextManager                            m_tmPattern;
    QList<QPainterPath>                     m_liPP;

    QVector<QPointF> Map(const QVector<QPointF> &points) const;
    static QVector<QPointF> RoundPoints(const QVector<QPointF> &points);

    QPointF RotatePoint(const QPointF& ptCenter, const QPointF& pt, qreal dAng) const;
    QVector<QPointF> Mirror(const QVector<QPointF>& points) const;
    qreal GetDistance(const QPointF& pt1, const QPointF& pt2) const;
};

Q_DECLARE_TYPEINFO(VLayoutDetail, Q_MOVABLE_TYPE);

#endif // VLAYOUTDETAIL_H
