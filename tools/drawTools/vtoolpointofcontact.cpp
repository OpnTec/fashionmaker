/************************************************************************
 **
 **  @file vtoolpointofcontact.cpp
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

#include "vtoolpointofcontact.h"
#include "../../container/calculator.h"

const QString VToolPointOfContact::ToolType = QStringLiteral("pointOfContact");

VToolPointOfContact::VToolPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                         const QString &radius, const qint64 &center,
                                         const qint64 &firstPointId, const qint64 &secondPointId,
                                         const Tool::Sources &typeCreation, QGraphicsItem *parent)
    : VToolPoint(doc, data, id, parent), radius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogPointOfContact(QSharedPointer<DialogPointOfContact>())
{
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolPointOfContact::setDialog()
{
    Q_ASSERT(dialogPointOfContact.isNull() == false);
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogPointOfContact->setRadius(radius);
    dialogPointOfContact->setCenter(center, id);
    dialogPointOfContact->setFirstPoint(firstPointId, id);
    dialogPointOfContact->setSecondPoint(secondPointId, id);
    dialogPointOfContact->setPointName(p.name());
}

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

void VToolPointOfContact::Create(QSharedPointer<DialogPointOfContact> &dialog, VMainGraphicsScene *scene,
                                 VDomDocument *doc, VContainer *data)
{
    QString radius = dialog->getRadius();
    qint64 center = dialog->getCenter();
    qint64 firstPointId = dialog->getFirstPoint();
    qint64 secondPointId = dialog->getSecondPoint();
    QString pointName = dialog->getPointName();
    Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolPointOfContact::Create(const qint64 _id, const QString &radius, const qint64 &center,
                                 const qint64 &firstPointId, const qint64 &secondPointId,
                                 const QString &pointName, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                 const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VPointF centerP = data->GetPoint(center);
    VPointF firstP = data->GetPoint(firstPointId);
    VPointF secondP = data->GetPoint(secondPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF fPoint = VToolPointOfContact::FindPoint(toPixel(result), centerP.toQPointF(),
                                                         firstP.toQPointF(), secondP.toQPointF());
        qint64 id =  _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
            data->AddLine(secondPointId, id);
            data->AddLine(center, id);
        }
        else
        {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
            data->AddLine(secondPointId, id);
            data->AddLine(center, id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::PointOfContact, doc);
        if (parse == Document::FullParse)
        {
            VToolPointOfContact *point = new VToolPointOfContact(doc, data, id, radius, center,
                                                                 firstPointId, secondPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolPointOfContact::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfContact::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(center);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
}

void VToolPointOfContact::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        radius = domElement.attribute(AttrRadius, "");
        center = domElement.attribute(AttrCenter, "").toLongLong();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolPointOfContact::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogPointOfContact->getPointName());
            domElement.setAttribute(AttrRadius, dialogPointOfContact->getRadius());
            domElement.setAttribute(AttrCenter, QString().setNum(dialogPointOfContact->getCenter()));
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogPointOfContact->getFirstPoint()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogPointOfContact->getSecondPoint()));
            emit FullUpdateTree();
        }
    }
    dialogPointOfContact.clear();
}

void VToolPointOfContact::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogPointOfContact, this, event);
}

void VToolPointOfContact::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrRadius, radius);
    AddAttribute(domElement, AttrCenter, center);
    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolPointOfContact::RemoveReferens()
{
    doc->DecrementReferens(center);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}
