/************************************************************************
 **
 **  @file   vistoolpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 1, 2016
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

#ifndef VISTOOLPOINTOFINTERSECTIONCURVES_H
#define VISTOOLPOINTOFINTERSECTIONCURVES_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vispath.h"

class VisToolPointOfIntersectionCurves : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolPointOfIntersectionCurves(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPointOfIntersectionCurves() Q_DECL_EQ_DEFAULT;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    virtual void VisualMode(const quint32 &id) Q_DECL_OVERRIDE;

    void setObject2Id(const quint32 &value);
    void setVCrossPoint(const VCrossCurvesPoint &value);
    void setHCrossPoint(const HCrossCurvesPoint &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointOfIntersectionCurves)};
private:
    Q_DISABLE_COPY(VisToolPointOfIntersectionCurves)
    quint32              object2Id;
    VCrossCurvesPoint    vCrossPoint;
    HCrossCurvesPoint    hCrossPoint;

    QGraphicsEllipseItem *point;
    VCurvePathItem       *visCurve2;

};

#endif // VISTOOLPOINTOFINTERSECTIONCURVES_H
