/************************************************************************
 **
 **  @file   vistoolpointofintersectionarcs.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 5, 2015
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

#ifndef VISTOOLPOINTOFINTERSECTIONARCS_H
#define VISTOOLPOINTOFINTERSECTIONARCS_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "visline.h"

class VisToolPointOfIntersectionArcs : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolPointOfIntersectionArcs(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPointOfIntersectionArcs() = default;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    virtual void VisualMode(const quint32 &id) Q_DECL_OVERRIDE;

    void         setArc1Id(const quint32 &value);
    void         setArc2Id(const quint32 &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointOfIntersectionArcs)};

    virtual void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget = nullptr) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VisToolPointOfIntersectionArcs)
    quint32              arc1Id;
    quint32              arc2Id;
    CrossCirclesPoint    crossPoint;
    QGraphicsEllipseItem *point;
    VCurvePathItem       *arc1Path;
    VCurvePathItem       *arc2Path;
};

#endif // VISTOOLPOINTOFINTERSECTIONARCS_H
