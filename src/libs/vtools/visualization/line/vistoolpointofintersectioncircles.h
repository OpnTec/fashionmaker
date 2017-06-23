/************************************************************************
 **
 **  @file   vistoolpointofintersectioncircles.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
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

#ifndef VISTOOLPOINTOFINTERSECTIONCIRCLES_H
#define VISTOOLPOINTOFINTERSECTIONCIRCLES_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "visline.h"

class VisToolPointOfIntersectionCircles : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolPointOfIntersectionCircles(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPointOfIntersectionCircles() = default;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    virtual void VisualMode(const quint32 &id) Q_DECL_OVERRIDE;

    void         setObject2Id(const quint32 &value);
    void         setC1Radius(const QString &value);
    void         setC2Radius(const QString &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointOfIntersectionCircles)};
private:
    Q_DISABLE_COPY(VisToolPointOfIntersectionCircles)
    quint32            object2Id;
    qreal              c1Radius;
    qreal              c2Radius;
    CrossCirclesPoint  crossPoint;
    VScaledEllipse    *point;
    VScaledEllipse    *c1Center;
    VScaledEllipse    *c2Center;
    VScaledEllipse    *c1Path;
    VScaledEllipse    *c2Path;
};

#endif // VISTOOLPOINTOFINTERSECTIONCIRCLES_H
