/************************************************************************
 **
 **  @file   vistoolpointfromcircleandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VISTOOLPOINTFROMCIRCLEANDTANGENT_H
#define VISTOOLPOINTFROMCIRCLEANDTANGENT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "visline.h"

class VisToolPointFromCircleAndTangent : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolPointFromCircleAndTangent(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPointFromCircleAndTangent() = default;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void         setObject2Id(const quint32 &value);
    void         setCRadius(const QString &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointFromCircleAndTangent)};

    virtual void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget = nullptr) Q_DECL_OVERRIDE;
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VisToolPointFromCircleAndTangent)
    quint32              object2Id;
    qreal                cRadius;
    CrossCirclesPoint    crossPoint;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *tangent;
    QGraphicsEllipseItem *cCenter;
    QGraphicsEllipseItem *cPath;
    QGraphicsLineItem    *tangent2;

    void FindRays(const QPointF &p, const QPointF &center, qreal radius);
};

#endif // VISTOOLPOINTFROMCIRCLEANDTANGENT_H
