/************************************************************************
 **
 **  @file   vistoolpointofintersectionarcs.cpp
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

#include "vistoolpointofintersectionarcs.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectionarcs.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "visualization/line/../../tools/drawTools/toolpoint/toolsinglepoint/../../../../dialogs/tools/../../visualization/visualization.h"
#include "visualization/line/visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersectionArcs::VisToolPointOfIntersectionArcs(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), arc1Id(NULL_ID), arc2Id(NULL_ID), crossPoint(CrossCirclesPoint::FirstPoint),
      point(nullptr),
      arc1Path(nullptr), arc2Path(nullptr)
{
    this->setPen(QPen(Qt::NoPen)); // don't use parent this time

    arc1Path = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    arc1Path->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    arc2Path = InitItem<QGraphicsPathItem>(Qt::darkRed, this);
    arc2Path->setFlag(QGraphicsItem::ItemStacksBehindParent, false);

    point = InitPoint(mainColor, this);
    point->setZValue(2);
    point->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersectionArcs::~VisToolPointOfIntersectionArcs()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionArcs::RefreshGeometry()
{
    if (arc1Id > NULL_ID)
    {
        const QSharedPointer<VArc> arc1 = Visualization::data->GeometricObject<VArc>(arc1Id);
        DrawPath(arc1Path, arc1->GetPath(PathDirection::Show), Qt::darkGreen, Qt::SolidLine, Qt::RoundCap);

        if (arc2Id > NULL_ID)
        {
            const QSharedPointer<VArc> arc2 = Visualization::data->GeometricObject<VArc>(arc2Id);
            DrawPath(arc2Path, arc2->GetPath(PathDirection::Show), Qt::darkRed, Qt::SolidLine, Qt::RoundCap);

            const QPointF fPoint = VToolPointOfIntersectionArcs::FindPoint(arc1.data(), arc2.data(), crossPoint);
            DrawPoint(point, fPoint, mainColor);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionArcs::VisualMode(const quint32 &id)
{
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);

    this->arc1Id = id;
    Visualization::scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionArcs::setArc1Id(const quint32 &value)
{
    arc1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionArcs::setArc2Id(const quint32 &value)
{
    arc2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionArcs::setCrossPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;
}
