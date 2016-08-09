/************************************************************************
 **
 **  @file   vistoolrotation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2016
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

#ifndef VISTOOLROTATION_H
#define VISTOOLROTATION_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "visline.h"

class QPointF;
class VContainer;

class VisToolRotation : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolRotation(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolRotation();

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void SetObjects(QVector<quint32> objects);
    void SetOriginPointId(quint32 value);

    QString Angle() const;
    void    SetAngle(const QString &expression);

    virtual void VisualMode(const quint32 &pointId = NULL_ID) Q_DECL_OVERRIDE;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolRotation)};
private:
    Q_DISABLE_COPY(VisToolRotation)
    qreal                angle;
    QVector<quint32>     objects;
    QGraphicsEllipseItem *point;
    QGraphicsPathItem    *angleArc;
    QGraphicsLineItem    *xAxis;
    QColor               supportColor2;

    QVector<QGraphicsEllipseItem *> points;
    QVector<QGraphicsPathItem *>    curves;

    QGraphicsEllipseItem * GetPoint(quint32 i, const QColor &color);
    QGraphicsPathItem    * GetCurve(quint32 i, const QColor &color);

    template <class Item>
    int AddCurve(qreal angle, const QPointF &origin, quint32 id, int i);
};

#endif // VISTOOLROTATION_H
