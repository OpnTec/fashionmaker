/************************************************************************
 **
 **  @file   vtoollineintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vtoollineintersectaxis.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialoglineintersectaxis.h"
#include "../../dialogs/tools/dialogeditwrongformula.h"
#include "../../geometry/vpointf.h"
#include "./../visualization/vistoollineintersectaxis.h"

const QString VToolLineIntersectAxis::ToolType = QStringLiteral("lineIntersectAxis");

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis::VToolLineIntersectAxis(VPattern *doc, VContainer *data, const quint32 &id,
                                               const QString &typeLine, const QString &formulaAngle,
                                               const quint32 &basePointId, const quint32 &firstPointId,
                                               const quint32 &secondPointId, const Source &typeCreation,
                                               QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, QString(), basePointId, 0, parent), formulaAngle(formulaAngle),
      firstPointId(firstPointId), secondPointId(secondPointId)
{
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis::~VToolLineIntersectAxis()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::setDialog()
{
    SCASSERT(dialog != nullptr);
    dialog->setModal(true);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetBasePointId(basePointId);
    dialogTool->SetFirstPointId(firstPointId);
    dialogTool->SetSecondPointId(secondPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis *VToolLineIntersectAxis::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                                       VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->GetTypeLine();
    QString formulaAngle = dialogTool->GetAngle();
    const quint32 basePointId = dialogTool->GetBasePointId();
    const quint32 firstPointId = dialogTool->GetFirstPointId();
    const quint32 secondPointId = dialogTool->GetSecondPointId();

    VToolLineIntersectAxis *point = nullptr;
    point=Create(0, pointName, typeLine, formulaAngle, basePointId, firstPointId, secondPointId, 5, 10, scene, doc,
                 data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis *VToolLineIntersectAxis::Create(const quint32 _id, const QString &pointName,
                                                       const QString &typeLine, QString &formulaAngle,
                                                       const quint32 &basePointId, const quint32 &firstPointId,
                                                       const quint32 &secondPointId, const qreal &mx, const qreal &my,
                                                       VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                                       const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    QLineF axis = QLineF(basePoint->toQPointF(), QPointF(basePoint->x()+100, basePoint->y()));
    axis.setAngle(CheckFormula(_id, formulaAngle, data));

    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);
    QLineF line(firstPoint->toQPointF(), secondPoint->toQPointF());

    QPointF fPoint = FindPoint(axis, line);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::LineIntersectAxis, doc);
    if (parse == Document::FullParse)
    {
        VToolLineIntersectAxis *point = new VToolLineIntersectAxis(doc, data, id, typeLine, formulaAngle,
                                                                   basePointId, firstPointId, secondPointId,
                                                                   typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolLineIntersectAxis::FindPoint(const QLineF &axis, const QLineF &line)
{
    QPointF fPoint;
    QLineF::IntersectType intersect = axis.intersect(line, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        return fPoint;
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolLineIntersectAxis::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersectAxis::GetFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetFirstPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersectAxis::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolLineIntersectAxis * visual = new VisToolLineIntersectAxis(getData());
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
            scene->addItem(visual);

            visual->setPoint1Id(firstPointId);
            visual->setPoint2Id(secondPointId);
            visual->setAxisPointId(basePointId);
            visual->SetAngle(qApp->FormulaToUser(formulaAngle));
            visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolLineIntersectAxis *visual = qobject_cast<VisToolLineIntersectAxis *>(vis);
            if (visual != nullptr)
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        vis = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toUInt();
        firstPointId = domElement.attribute(AttrP1Line, "").toUInt();
        secondPointId = domElement.attribute(AttrP2Line, "").toUInt();
        formulaAngle = domElement.attribute(AttrAngle, "");
    }
    RefreshGeometry();

    if (vis != nullptr)
    {
        VisToolLineIntersectAxis *visual = qobject_cast<VisToolLineIntersectAxis *>(vis);
        visual->setPoint1Id(firstPointId);
        visual->setPoint2Id(secondPointId);
        visual->setAxisPointId(basePointId);
        visual->SetAngle(qApp->FormulaToUser(formulaAngle));
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLineIntersectAxis>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLineIntersectAxis>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(tag, AttrTypeLine, typeLine);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
    doc->SetAttribute(tag, AttrP1Line, firstPointId);
    doc->SetAttribute(tag, AttrP2Line, secondPointId);
}
