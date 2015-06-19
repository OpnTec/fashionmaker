/************************************************************************
 **
 **  @file   vtoolpointofintersectioncircles.cpp
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

#include "vtoolpointofintersectioncircles.h"
#include "../../../../dialogs/tools/dialogpointofintersectioncircles.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/varc.h"
#include "../../../../visualization/vistoolpointofintersectioncircles.h"
#include "../vpatterndb/vformula.h"

const QString VToolPointOfIntersectionCircles::ToolType = QStringLiteral("pointOfIntersectionCircles");

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCircles::VToolPointOfIntersectionCircles(VAbstractPattern *doc, VContainer *data,
                                                                 const quint32 &id,
                                                                 quint32 firstCircleCenterId,
                                                                 quint32 secondCircleCenterId,
                                                                 const QString &firstCircleRadius,
                                                                 const QString &secondCircleRadius,
                                                                 CrossCirclesPoint crossPoint,
                                                                 const Source &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), firstCircleCenterId(firstCircleCenterId),
      secondCircleCenterId(secondCircleCenterId), firstCircleRadius(firstCircleRadius),
      secondCircleRadius(secondCircleRadius), crossPoint(crossPoint)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersectionCircles *dialogTool = qobject_cast<DialogPointOfIntersectionCircles*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFirstCircleCenterId(firstCircleCenterId);
    dialogTool->SetSecondCircleCenterId(secondCircleCenterId);
    dialogTool->SetFirstCircleRadius(firstCircleRadius);
    dialogTool->SetSecondCircleRadius(secondCircleRadius);
    dialogTool->SetCrossCirclesPoint(crossPoint);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCircles *VToolPointOfIntersectionCircles::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                                                                         VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersectionCircles *dialogTool = qobject_cast<DialogPointOfIntersectionCircles*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 firstCircleCenterId = dialogTool->GetFirstCircleCenterId();
    const quint32 secondCircleCenterId = dialogTool->GetSecondCircleCenterId();
    QString firstCircleRadius = dialogTool->GetFirstCircleRadius();
    QString secondCircleRadius = dialogTool->GetSecondCircleRadius();
    const CrossCirclesPoint pType = dialogTool->GetCrossCirclesPoint();
    const QString pointName = dialogTool->getPointName();
    VToolPointOfIntersectionCircles *point = nullptr;
    point = Create(0, pointName, firstCircleCenterId, secondCircleCenterId, firstCircleRadius, secondCircleRadius,
                   pType, 5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCircles *VToolPointOfIntersectionCircles::Create(const quint32 _id, const QString &pointName,
                                                                         quint32 firstCircleCenterId,
                                                                         quint32 secondCircleCenterId,
                                                                         QString &firstCircleRadius,
                                                                         QString &secondCircleRadius,
                                                                         CrossCirclesPoint crossPoint, const qreal &mx,
                                                                         const qreal &my, VMainGraphicsScene *scene,
                                                                         VAbstractPattern *doc, VContainer *data,
                                                                         const Document &parse,
                                                                         const Source &typeCreation)
{
    const qreal calcC1Radius = qApp->toPixel(CheckFormula(_id, firstCircleRadius, data));
    const qreal calcC2Radius = qApp->toPixel(CheckFormula(_id, secondCircleRadius, data));

    const VPointF c1Point = *data->GeometricObject<VPointF>(firstCircleCenterId);
    const VPointF c2Point = *data->GeometricObject<VPointF>(secondCircleCenterId);

    const QPointF point = FindPoint(c1Point.toQPointF(), c2Point.toQPointF(), calcC1Radius, calcC2Radius, crossPoint);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(point, pointName, mx, my));
    }
    else
    {
        data->UpdateGObject(id, new VPointF(point, pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::PointOfIntersectionCircles, doc);
    if (parse == Document::FullParse)
    {
        VToolPointOfIntersectionCircles *point = new VToolPointOfIntersectionCircles(doc, data, id, firstCircleCenterId,
                                                                                     secondCircleCenterId,
                                                                                     firstCircleRadius,
                                                                                     secondCircleRadius, crossPoint,
                                                                                     typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfIntersectionCircles::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfIntersectionCircles::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPointOfIntersectionCircles::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolPointOfIntersectionCircles::EnableToolMove);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstCircleCenterId);
        doc->IncrementReferens(secondCircleCenterId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointOfIntersectionCircles::FindPoint(const QPointF &c1Point, const QPointF &c2Point, qreal c1Radius,
                                                   qreal c2Radius, const CrossCirclesPoint crossPoint)
{
    QPointF p1, p2;
    const int res = VGObject::IntersectionCircles(c1Point, c1Radius, c2Point, c2Radius, p1, p2);

    switch(res)
    {
        case 2:
            if (crossPoint == CrossCirclesPoint::FirstPoint)
            {
                return p1;
            }
            else
            {
                return p2;
            }
            break;
        case 1:
            return p1;
            break;
        case 3:
        case 0:
        default:
            return QPointF(0, 0);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfIntersectionCircles::GetFirstCircleCenterId() const
{
    return firstCircleCenterId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetFirstCircleCenterId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstCircleCenterId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfIntersectionCircles::GetSecondCircleCenterId() const
{
    return secondCircleCenterId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetSecondCircleCenterId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondCircleCenterId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolPointOfIntersectionCircles::GetFirstCircleRadius() const
{
    VFormula radius(firstCircleRadius, getData());
    radius.setCheckZero(true);
    radius.setToolId(id);
    radius.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetFirstCircleRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        if (value.getDoubleValue() > 0)// Formula don't check this, but radius can't be 0 or negative
        {
            firstCircleRadius = value.GetFormula(FormulaType::FromUser);
            QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
            SaveOption(obj);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolPointOfIntersectionCircles::GetSecondCircleRadius() const
{
    VFormula radius(secondCircleRadius, getData());
    radius.setCheckZero(true);
    radius.setToolId(id);
    radius.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetSecondCircleRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        if (value.getDoubleValue() > 0)// Formula don't check this, but radius can't be 0 or negative
        {
            secondCircleRadius = value.GetFormula(FormulaType::FromUser);
            QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
            SaveOption(obj);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint VToolPointOfIntersectionCircles::GetCrossCirclesPoint() const
{
    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetCrossCirclesPoint(CrossCirclesPoint &value)
{
    crossPoint = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfIntersectionCircles>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::RemoveReferens()
{
    doc->DecrementReferens(firstCircleCenterId);
    doc->DecrementReferens(secondCircleCenterId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfIntersectionCircles>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersectionCircles *dialogTool = qobject_cast<DialogPointOfIntersectionCircles*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrC1Center, QString().setNum(dialogTool->GetFirstCircleCenterId()));
    doc->SetAttribute(domElement, AttrC2Center, QString().setNum(dialogTool->GetSecondCircleCenterId()));
    doc->SetAttribute(domElement, AttrC1Radius, dialogTool->GetFirstCircleRadius());
    doc->SetAttribute(domElement, AttrC2Radius, dialogTool->GetSecondCircleRadius());
    doc->SetAttribute(domElement, AttrCrossPoint,
                      QString().setNum(static_cast<int>(dialogTool->GetCrossCirclesPoint())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolPoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrC1Center, firstCircleCenterId);
    doc->SetAttribute(tag, AttrC2Center, secondCircleCenterId);
    doc->SetAttribute(tag, AttrC1Radius, firstCircleRadius);
    doc->SetAttribute(tag, AttrC2Radius, secondCircleRadius);
    doc->SetAttribute(tag, AttrCrossPoint, static_cast<int>(crossPoint));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::ReadToolAttributes(const QDomElement &domElement)
{
    firstCircleCenterId = doc->GetParametrUInt(domElement, AttrC1Center, NULL_ID_STR);
    secondCircleCenterId = doc->GetParametrUInt(domElement, AttrC2Center, NULL_ID_STR);
    firstCircleRadius = doc->GetParametrString(domElement, AttrC1Radius);
    secondCircleRadius = doc->GetParametrString(domElement, AttrC2Radius);
    crossPoint = static_cast<CrossCirclesPoint>(doc->GetParametrUInt(domElement, AttrCrossPoint, "1"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCircles::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolPointOfIntersectionCircles *visual = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
        SCASSERT(visual != nullptr);

        visual->setPoint1Id(firstCircleCenterId);
        visual->setPoint2Id(secondCircleCenterId);
        visual->setC1Radius(firstCircleRadius);
        visual->setC2Radius(secondCircleRadius);
        visual->setCrossPoint(crossPoint);
        visual->RefreshGeometry();
    }
}
