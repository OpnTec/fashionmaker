/************************************************************************
 **
 **  @file vmodelingpointofintersection.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#include "vmodelingpointofintersection.h"

const QString VModelingPointOfIntersection::ToolType = QStringLiteral("pointOfIntersection");

VModelingPointOfIntersection::VModelingPointOfIntersection(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                           const qint64 &firstPointId, const qint64 &secondPointId,
                                                           const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), firstPointId(firstPointId), secondPointId(secondPointId),
    dialogPointOfIntersection(QSharedPointer<DialogPointOfIntersection>())
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingPointOfIntersection::setDialog()
{
    Q_ASSERT(dialogPointOfIntersection.isNull() == false);
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogPointOfIntersection->setFirstPointId(firstPointId, id);
    dialogPointOfIntersection->setSecondPointId(secondPointId, id);
    dialogPointOfIntersection->setPointName(p.name());
}

VModelingPointOfIntersection *VModelingPointOfIntersection::Create(QSharedPointer<DialogPointOfIntersection> &dialog,
                                                                   VDomDocument *doc, VContainer *data)
{
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    return Create(0, pointName, firstPointId, secondPointId, 5, 10, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingPointOfIntersection *VModelingPointOfIntersection::Create(const qint64 _id, const QString &pointName,
                                                                   const qint64 &firstPointId,
                                                                   const qint64 &secondPointId, const qreal &mx,
                                                                   const qreal &my, VDomDocument *doc,
                                                                   VContainer *data, const Document::Documents &parse,
                                                                   const Tool::Sources &typeCreation)
{
    VModelingPointOfIntersection *tool = 0;
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);

    QPointF point(firstPoint.x(), secondPoint.y());
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddPoint(VPointF(point.x(), point.y(), pointName, mx, my));
    }
    else
    {
        data->UpdatePoint(id, VPointF(point.x(), point.y(), pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    if (parse == Document::FullParse)
    {
        tool = new VModelingPointOfIntersection(doc, data, id, firstPointId, secondPointId, typeCreation);
        doc->AddTool(id, tool);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
    return tool;
}

void VModelingPointOfIntersection::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPoint(id));
}

void VModelingPointOfIntersection::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogPointOfIntersection->getPointName());
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogPointOfIntersection->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogPointOfIntersection->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogPointOfIntersection.clear();
}

void VModelingPointOfIntersection::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VModelingPointOfIntersection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogPointOfIntersection, this, event);
}

void VModelingPointOfIntersection::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToModeling(domElement);
}
