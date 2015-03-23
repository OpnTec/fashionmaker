/************************************************************************
 **
 **  @file   vtoolcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vtoolcutarc.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutarc.h"
#include "../../geometry/vpointf.h"
#include "../../geometry/varc.h"
#include "../../visualization/vistoolcutarc.h"

const QString VToolCutArc::ToolType = QStringLiteral("cutArc");
const QString VToolCutArc::AttrArc  = QStringLiteral("arc");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutArc constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first arc.
 * @param arcId id arc in data container.
 * @param arc1id id first cutting arc.
 * @param arc2id id second cutting arc.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutArc::VToolCutArc(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                         const quint32 &arcId, const quint32 &arc1id, const quint32 &arc2id, const QString &color,
                         const Source &typeCreation, QGraphicsItem * parent)
    :VToolCut(doc, data, id, formula, arcId, arc1id, arc2id, color, parent)
{
    RefreshCurve(firstCurve, curve1id, SimpleCurvePoint::ForthPoint);
    RefreshCurve(secondCurve, curve2id, SimpleCurvePoint::FirstPoint);

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
void VToolCutArc::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFormula(formula);
    dialogTool->setArcId(curveCutId);
    dialogTool->SetPointName(point->name());
    dialogTool->SetColor(lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolCutArc* VToolCutArc::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->GetFormula();
    const quint32 arcId = dialogTool->getArcId();
    const QString color = dialogTool->GetColor();
    VToolCutArc* point = nullptr;
    point=Create(0, pointName, formula, arcId, 5, 10, color, scene, doc, data, Document::FullParse, Source::FromGui);
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
 * @param pointName point name.
 * @param formula string with formula length first arc.
 * @param arcId id arc in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolCutArc* VToolCutArc::Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &arcId,
                                 const qreal &mx, const qreal &my, const QString &color, VMainGraphicsScene *scene,
                                 VPattern *doc, VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(arcId);

    const qreal result = CheckFormula(_id, formula, data);

    VArc arc1;
    VArc arc2;
    QPointF point = arc->CutArc(qApp->toPixel(result), arc1, arc2);

    quint32 id = _id;
    quint32 arc1id = 0;
    quint32 arc2id = 0;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(point, pointName, mx, my));
        arc1id = data->AddGObject(new VArc(arc1));
        arc2id = data->AddGObject(new VArc(arc2));

        data->AddCurveLength<VArcLength>(arc1id, id);
        data->AddCurveLength<VArcLength>(arc2id, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(point, pointName, mx, my));
        arc1id = id + 1;
        arc2id = id + 2;

        data->UpdateGObject(arc1id, new VArc(arc1));
        data->UpdateGObject(arc2id, new VArc(arc2));

        data->AddCurveLength<VArcLength>(arc1id, id);
        data->AddCurveLength<VArcLength>(arc2id, id);

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::CutArc, doc);
    if (parse == Document::FullParse)
    {
        VToolCutArc *point = new VToolCutArc(doc, data, id, formula, arcId, arc1id, arc2id, color, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolCutArc::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolCutArc::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolCutArc::EnableToolMove);
        doc->AddTool(id, point);
        doc->AddTool(arc1id, point);
        doc->AddTool(arc2id, point);
        doc->IncrementReferens(arcId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolCutArc * visual = new VisToolCutArc(getData());
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
            scene->addItem(visual);

            visual->setPoint1Id(curveCutId);
            visual->setLength(qApp->FormulaToUser(formula));
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolCutArc *visual = qobject_cast<VisToolCutArc *>(vis);
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
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolCutArc::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();

    if (vis != nullptr)
    {
        VisToolCutArc *visual = qobject_cast<VisToolCutArc *>(vis);
        visual->setPoint1Id(curveCutId);
        visual->setLength(qApp->FormulaToUser(formula));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CurveChoosed send signal about selection from cutted arc.
 * @param id object id in container.
 */
void VToolCutArc::CurveChoosed(quint32 id)
{
    emit ChoosedTool(id, SceneObject::Arc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolCutArc::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutArc>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutArc>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutArc::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrArc, QString().setNum(dialogTool->getArcId()));
    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshCurve refresh curve on scene.
 * @param curve curve.
 * @param curveId curve id.
 */
void VToolCutArc::RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint curvePosition,
                               PathDirection direction)
{
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(curveId);
    QPainterPath path;
    path.addPath(arc->GetPath(direction));
    path.setFillRule( Qt::WindingFill );
    if (curvePosition == SimpleCurvePoint::FirstPoint)
    {
        path.translate(-arc->GetP1().x(), -arc->GetP1().y());
    }
    else
    {
        path.translate(-arc->GetP2().x(), -arc->GetP2().y());
    }
    curve->SetCurrentColor(QColor(lineColor));
    curve->setPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));
    doc->SetAttribute(tag, AttrColor, lineColor);

    doc->SetAttribute(tag, AttrLength, formula);
    doc->SetAttribute(tag, AttrArc, curveCutId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::ReadToolAttributes(const QDomElement &domElement)
{
    formula = doc->GetParametrString(domElement, AttrLength, "");
    curveCutId = doc->GetParametrUInt(domElement, AttrArc, NULL_ID_STR);
    lineColor = doc->GetParametrString(domElement, AttrColor, ColorBlack);
}
