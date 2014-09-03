/************************************************************************
 **
 **  @file   vtoolcutspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "vtoolcutspline.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutspline.h"
#include "../../geometry/vpointf.h"

#include <geometry/vspline.h>

const QString VToolCutSpline::ToolType   = QStringLiteral("cutSpline");
const QString VToolCutSpline::AttrSpline = QStringLiteral("spline");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutSpline constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first spline.
 * @param splineId id spline in data container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutSpline::VToolCutSpline(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                               const quint32 &splineId, const quint32 &spl1id, const quint32 &spl2id,
                               const Source &typeCreation, QGraphicsItem *parent)
    :VToolCut(doc, data, id, formula, splineId, spl1id, spl2id, parent)
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
void VToolCutSpline::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setFormula(formula);
    dialogTool->setSplineId(curveCutId);
    dialogTool->setPointName(point->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolCutSpline* VToolCutSpline::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                            VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->getFormula();
    const quint32 splineId = dialogTool->getSplineId();
    VToolCutSpline* point = nullptr;
    point = Create(0, pointName, formula, splineId, 5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
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
 * @param formula string with formula length first spline.
 * @param splineId id spline in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolCutSpline* VToolCutSpline::Create(const quint32 _id, const QString &pointName, QString &formula,
                                       const quint32 &splineId, const qreal &mx, const qreal &my,
                                       VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                       const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VSpline> spl = data->GeometricObject<VSpline>(splineId);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF spl1p2, spl1p3, spl2p2, spl2p3;
    QPointF point = spl->CutSpline(qApp->toPixel(result), spl1p2, spl1p3, spl2p2, spl2p3);

    quint32 id = _id;
    quint32 spl1id = 0;
    quint32 spl2id = 0;
    if (typeCreation == Source::FromGui)
    {
        VPointF *p = new VPointF(point, pointName, mx, my);
        id = data->AddGObject(p);

        VSpline *spline1 = new VSpline(spl->GetP1(), spl1p2, spl1p3, *p, spl->GetKcurve());
        spl1id = data->AddGObject(spline1);
        data->AddCurveLength<VSplineLength>(spl1id, id);

        VSpline *spline2 = new VSpline(*p, spl2p2, spl2p3, spl->GetP4(), spl->GetKcurve());
        spl2id = data->AddGObject(spline2);
        data->AddCurveLength<VSplineLength>(spl2id, id);
    }
    else
    {
        VPointF *p = new VPointF(point, pointName, mx, my);
        data->UpdateGObject(id, p);

        spl1id = id + 1;
        spl2id = id + 2;

        VSpline *spline1 = new VSpline(spl->GetP1(), spl1p2, spl1p3, *p, spl->GetKcurve());
        data->UpdateGObject(spl1id, spline1);
        data->AddCurveLength<VSplineLength>(spl1id, id);

        VSpline *spline2 = new VSpline(*p, spl2p2, spl2p3, spl->GetP4(), spl->GetKcurve());
        data->UpdateGObject(spl2id, spline2);
        data->AddCurveLength<VSplineLength>(spl2id, id);

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::CutSpline, doc);
    if (parse == Document::FullParse)
    {
        VToolCutSpline *point = new VToolCutSpline(doc, data, id, formula, splineId, spl1id, spl2id, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolCutSpline::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->AddTool(spl1id, point);
        doc->AddTool(spl2id, point);
        doc->IncrementReferens(splineId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::ShowVisualization(bool show)
{
//    if (show)
//    {
//        if (vis == nullptr)
//        {
//            VisTool * visual = new VisTool(getData());
//            VMainGraphicsScene *scene = qApp->getCurrentScene();
//            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
//            scene->addItem(visual);

//            // add options
//            visual->RefreshGeometry();
//            vis = visual;
//        }
//        else
//        {
//            VisTool * visual = qobject_cast<VisTool *>(vis);
//            if (visual != nullptr)
//            {
//                visual->show();
//            }
//        }
//    }
//    else
//    {
//        delete vis;
//        vis = nullptr;
//    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolCutSpline::FullUpdateFromFile()
{
    FullUpdateCurveFromFile(AttrSpline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CurveChoosed send signal about selection from spline.
 * @param id object id in container.
 */
void VToolCutSpline::CurveChoosed(quint32 id)
{
    emit ChoosedTool(id, SceneObject::Spline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolCutSpline::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSpline>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSpline>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutSpline::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrSpline, QString().setNum(dialogTool->getSplineId()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshCurve refresh curve on scene.
 * @param curve curve.
 * @param curveId curve id.
 * @param tr point type.
 */
void VToolCutSpline::RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint curvePosition,
                                  PathDirection direction)
{
    const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(curveId);
    QPainterPath path;
    path.addPath(spl->GetPath(direction));
    path.setFillRule( Qt::WindingFill );
    if (curvePosition == SimpleCurvePoint::FirstPoint)
    {
        path.translate(-spl->GetP1().toQPointF().x(), -spl->GetP1().toQPointF().y());
    }
    else
    {
        path.translate(-spl->GetP4().toQPointF().x(), -spl->GetP4().toQPointF().y());
    }
    curve->setPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(tag, AttrLength, formula);
    doc->SetAttribute(tag, AttrSpline, curveCutId);
}
