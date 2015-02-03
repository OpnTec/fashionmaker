/************************************************************************
 **
 **  @file   vtoolshoulderpoint.cpp
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

#include "vtoolshoulderpoint.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogshoulderpoint.h"
#include "../../geometry/vpointf.h"
#include "../../visualization/vistoolshoulderpoint.h"

const QString VToolShoulderPoint::ToolType = QStringLiteral("shoulder");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolShoulderPoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formula string with formula length.
 * @param p1Line id first line point.
 * @param p2Line id second line point.
 * @param pShoulder id shoulder point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolShoulderPoint::VToolShoulderPoint(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                                       const QString &formula, const quint32 &p1Line, const quint32 &p2Line,
                                       const quint32 &pShoulder, const Source &typeCreation,
                                       QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line), pShoulder(pShoulder)
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
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolShoulderPoint::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetP1Line(basePointId);
    dialogTool->SetP2Line(p2Line);
    dialogTool->SetP3(pShoulder);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find point.
 * @param p1Line first line point.
 * @param p2Line second line point.
 * @param pShoulder shoulder point.
 * @param length length form shoulder point to our.
 * @return point.
 */
//TODO find better way calculate point.
QPointF VToolShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                      const qreal &length)
{
    QLineF line = QLineF(p1Line, p2Line);
    qreal toolLength = length;
    qreal dist = line.length();
    if (dist>toolLength)
    {
        qDebug()<<"Correction of length in shoulder point tool. Parameter length too small.";
        toolLength = dist;
    }
    if (qFuzzyCompare(dist, toolLength))
    {
        return line.p2();
    }
    qreal step = 0.01;
    while (1)
    {
        line.setLength(line.length()+step);
        QLineF line2 = QLineF(pShoulder, line.p2());
        if (line2.length()>=toolLength)
        {
            return line.p2();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    SCASSERT(dialogTool);
    QString formula = dialogTool->GetFormula();
    const quint32 p1Line = dialogTool->GetP1Line();
    const quint32 p2Line = dialogTool->GetP2Line();
    const quint32 pShoulder = dialogTool->GetP3();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString pointName = dialogTool->getPointName();
    VToolShoulderPoint * point = nullptr;
    point=Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param formula string with formula length.
 * @param p1Line id first line point.
 * @param p2Line id second line point.
 * @param pShoulder id shoulder point.
 * @param typeLine line type.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(const quint32 _id, QString &formula, const quint32 &p1Line,
                                const quint32 &p2Line, const quint32 &pShoulder, const QString &typeLine,
                                const QString &pointName, const qreal &mx, const qreal &my,
                                VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(p1Line);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(p2Line);
    const QSharedPointer<VPointF> shoulderPoint = data->GeometricObject<VPointF>(pShoulder);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = VToolShoulderPoint::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                                   shoulderPoint->toQPointF(), qApp->toPixel(result));
    quint32 id =  _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(p1Line, id);
        data->AddLine(p2Line, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(p1Line, id);
        data->AddLine(p2Line, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::ShoulderPoint, doc);
    if (parse == Document::FullParse)
    {
        VToolShoulderPoint *point = new VToolShoulderPoint(doc, data, id, typeLine, formula,
                                                           p1Line, p2Line, pShoulder,
                                                           typeCreation);
        scene->addItem(point);
        connect(point, &VToolShoulderPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolShoulderPoint::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(p1Line);
        doc->IncrementReferens(p2Line);
        doc->IncrementReferens(pShoulder);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolShoulderPoint::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formulaLength = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrP1Line, "").toUInt();
        p2Line = domElement.attribute(AttrP2Line, "").toUInt();
        pShoulder = domElement.attribute(AttrPShoulder, "").toUInt();
    }
    RefreshGeometry();

    if (vis != nullptr)
    {
        VisToolShoulderPoint *visual = qobject_cast<VisToolShoulderPoint *>(vis);
        visual->setPoint1Id(pShoulder);
        visual->setLineP1Id(basePointId);
        visual->setLineP2Id(p2Line);
        visual->setLength(qApp->FormulaToUser(formulaLength));
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolShoulderPoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolShoulderPoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogShoulderPoint>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogShoulderPoint>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolShoulderPoint::RemoveReferens()
{
    doc->DecrementReferens(p2Line);
    doc->DecrementReferens(pShoulder);
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolShoulderPoint::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetP1Line()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetP2Line()));
    doc->SetAttribute(domElement, AttrPShoulder, QString().setNum(dialogTool->GetP3()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(tag, AttrTypeLine, typeLine);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrP1Line, basePointId);
    doc->SetAttribute(tag, AttrP2Line, p2Line);
    doc->SetAttribute(tag, AttrPShoulder, pShoulder);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolShoulderPoint::getPShoulder() const
{
    return pShoulder;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::setPShoulder(const quint32 &value)
{
    if (value != NULL_ID)
    {
        pShoulder = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolShoulderPoint * visual = new VisToolShoulderPoint(getData());
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
            scene->addItem(visual);

            visual->setPoint1Id(pShoulder);
            visual->setLineP1Id(basePointId);
            visual->setLineP2Id(p2Line);
            visual->setLength(qApp->FormulaToUser(formulaLength));
            visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolShoulderPoint *visual = qobject_cast<VisToolShoulderPoint *>(vis);
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
quint32 VToolShoulderPoint::GetP2Line() const
{
    return p2Line;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SetP2Line(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p2Line = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}
