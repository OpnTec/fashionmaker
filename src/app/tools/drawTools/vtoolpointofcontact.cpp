/************************************************************************
 **
 **  @file   vtoolpointofcontact.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vtoolpointofcontact.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogpointofcontact.h"

const QString VToolPointOfContact::ToolType = QStringLiteral("pointOfContact");

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfContact::VToolPointOfContact(VPattern *doc, VContainer *data, const quint32 &id,
                                         const QString &radius, const quint32 &center,
                                         const quint32 &firstPointId, const quint32 &secondPointId,
                                         const Valentina::Sources &typeCreation, QGraphicsItem *parent)
    : VToolPoint(doc, data, id, parent), arcRadius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId)
{
    if (typeCreation == Valentina::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setRadius(arcRadius);
    dialogTool->setCenter(center, id);
    dialogTool->setFirstPoint(firstPointId, id);
    dialogTool->setSecondPoint(secondPointId, id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointOfContact::FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                       const QPointF &secondPoint)
{
    QPointF pArc;
    qreal s = 0.0, s_x, s_y, step = 0.01, distans;
    while ( s < 1)
    {
        s_x = secondPoint.x()-(qAbs(secondPoint.x()-firstPoint.x()))*s;
        s_y = secondPoint.y()-(qAbs(secondPoint.y()-firstPoint.y()))*s;
        distans = QLineF(center.x(), center.y(), s_x, s_y).length();
        if (fabs(distans*10 - radius*10) < 0.1)
        {
            pArc.rx() = s_x;
            pArc.ry() = s_y;
            break;
        }
        if (distans<radius)
        {
            pArc.rx() = s_x;
            pArc.ry() = s_y;
        }
        s = s + step;
    }
    return pArc;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString radius = dialogTool->getRadius();
    quint32 center = dialogTool->getCenter();
    quint32 firstPointId = dialogTool->getFirstPoint();
    quint32 secondPointId = dialogTool->getSecondPoint();
    QString pointName = dialogTool->getPointName();
    Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Valentina::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::Create(const quint32 _id, const QString &radius, const quint32 &center,
                                 const quint32 &firstPointId, const quint32 &secondPointId,
                                 const QString &pointName, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                 const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VPointF *centerP = data->GeometricObject<const VPointF *>(center);
    const VPointF *firstP = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondP = data->GeometricObject<const VPointF *>(secondPointId);

    Calculator cal(data);
    const qreal result = cal.EvalFormula(radius);

    QPointF fPoint = VToolPointOfContact::FindPoint(qApp->toPixel(result), centerP->toQPointF(),
                                                     firstP->toQPointF(), secondP->toQPointF());
    quint32 id =  _id;
    if (typeCreation == Valentina::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Valentina::PointOfContact, doc);
    if (parse == Document::FullParse)
    {
        VToolPointOfContact *point = new VToolPointOfContact(doc, data, id, radius, center,
                                                             firstPointId, secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfContact::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(center);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        arcRadius = domElement.attribute(AttrRadius, "");
        center = domElement.attribute(AttrCenter, "").toUInt();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfContact>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfContact>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrRadius, arcRadius);
    doc->SetAttribute(domElement, AttrCenter, center);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrRadius, arcRadius);
        doc->SetAttribute(domElement, AttrCenter, center);
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::RemoveReferens()
{
    doc->DecrementReferens(center);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrRadius, dialogTool->getRadius());
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->getCenter()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPoint()));
}
