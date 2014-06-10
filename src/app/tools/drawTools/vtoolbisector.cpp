/************************************************************************
 **
 **  @file   vtoolbisector.cpp
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

#include "vtoolbisector.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogbisector.h"
#include "../../geometry/vpointf.h"

const QString VToolBisector::ToolType = QStringLiteral("bisector");

//---------------------------------------------------------------------------------------------------------------------
VToolBisector::VToolBisector(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                             const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                             const quint32 &thirdPointId, const Valentina::Sources &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(0), thirdPointId(0)
{
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

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
QPointF VToolBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                 const QPointF &thirdPoint, const qreal &length)
{
    QLineF line1(secondPoint, firstPoint);
    QLineF line2(secondPoint, thirdPoint);
    qreal angle = line1.angleTo(line2);
    if (angle>180)
    {
        angle = 360 - angle;
        line1.setAngle(line1.angle()-angle/2);
    }
    else
    {
        line1.setAngle(line1.angle()+angle/2);
    }
    line1.setLength(length);
    return line1.p2();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setFirstPointId(firstPointId, id);
    dialogTool->setSecondPointId(basePointId, id);
    dialogTool->setThirdPointId(thirdPointId, id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolBisector* VToolBisector::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                           VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString formula = dialogTool->getFormula();
    const quint32 firstPointId = dialogTool->getFirstPointId();
    const quint32 secondPointId = dialogTool->getSecondPointId();
    const quint32 thirdPointId = dialogTool->getThirdPointId();
    const QString typeLine = dialogTool->getTypeLine();
    const QString pointName = dialogTool->getPointName();
    VToolBisector *point = nullptr;
    point=Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Valentina::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolBisector* VToolBisector::Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                           const quint32 &secondPointId, const quint32 &thirdPointId, const QString &typeLine,
                           const QString &pointName, const qreal &mx, const qreal &my,
                           VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                           const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);
    const VPointF *thirdPoint = data->GeometricObject<const VPointF *>(thirdPointId);

    const qreal result = CheckFormula(formula, data);

    QPointF fPoint = VToolBisector::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                              thirdPoint->toQPointF(), qApp->toPixel(result));
    quint32 id = _id;
    if (typeCreation == Valentina::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Valentina::BisectorTool, doc);
    if (parse == Document::FullParse)
    {
        VToolBisector *point = new VToolBisector(doc, data, id, typeLine, formula, firstPointId, secondPointId,
                                                 thirdPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolBisector::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        doc->IncrementReferens(thirdPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        basePointId = domElement.attribute(AttrSecondPoint, "").toUInt();
        thirdPointId = domElement.attribute(AttrThirdPoint, "").toUInt();
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, basePointId);
    doc->SetAttribute(domElement, AttrThirdPoint, thirdPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, basePointId);
        doc->SetAttribute(domElement, AttrThirdPoint, thirdPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(thirdPointId);
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
    doc->SetAttribute(domElement, AttrThirdPoint, QString().setNum(dialogTool->getThirdPointId()));
}
