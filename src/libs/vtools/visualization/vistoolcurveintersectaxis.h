/************************************************************************
 **
 **  @file   vistoolcurveintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
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

#ifndef VISTOOLCURVEINTERSECTAXIS_H
#define VISTOOLCURVEINTERSECTAXIS_H

#include "visline.h"

class QGraphicsEllipseItem;

class VisToolCurveIntersectAxis : public VisLine
{
    Q_OBJECT
public:
    VisToolCurveIntersectAxis(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolCurveIntersectAxis() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    QString      Angle() const;
    void         SetAngle(const QString &expression);
    void         setAxisPointId(const quint32 &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolLineIntersectAxis)};
private:
    Q_DISABLE_COPY(VisToolCurveIntersectAxis)
    quint32              axisPointId;
    qreal                angle;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *basePoint;
    QGraphicsLineItem    *baseLine;
    QGraphicsLineItem    *axisLine;
    QGraphicsPathItem    *visCurve;
};

#endif // VISTOOLCURVEINTERSECTAXIS_H
