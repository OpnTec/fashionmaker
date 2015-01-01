/************************************************************************
 **
 **  @file   vtoolspline.cpp
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

#include "vtoolspline.h"
#include "../../geometry/vspline.h"
#include "../../dialogs/tools/dialogspline.h"
#include "../../undocommands/movespline.h"
#include "../../visualization/vistoolspline.h"

const QString VToolSpline::ToolType = QStringLiteral("simple");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSpline constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolSpline::VToolSpline(VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                         QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::Spline;

    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    this->setPath(ToolPath());

    const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    VControlPointSpline *controlPoint1 = new VControlPointSpline(1, SplinePointPosition::FirstPoint, spl->GetP2(),
                                                                 spl->GetP1().toQPointF(), this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint1, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint1, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint1);

    VControlPointSpline *controlPoint2 = new VControlPointSpline(1, SplinePointPosition::LastPoint, spl->GetP3(),
                                                                 spl->GetP4().toQPointF(), this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint2, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint2, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint2);

    ShowFoot(false);

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
VToolSpline::~VToolSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolSpline::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSpline *dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    dialogTool->setP1(spl->GetP1().id());
    dialogTool->setP4(spl->GetP4().id());
    dialogTool->setAngle1(spl->GetAngle1());
    dialogTool->setAngle2(spl->GetAngle2());
    dialogTool->setKAsm1(spl->GetKasm1());
    dialogTool->setKAsm2(spl->GetKasm2());
    dialogTool->setKCurve(spl->GetKcurve());
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
VToolSpline* VToolSpline::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogSpline *dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 p1 = dialogTool->getP1();
    const quint32 p4 = dialogTool->getP4();
    const qreal kAsm1 = dialogTool->getKAsm1();
    const qreal kAsm2 = dialogTool->getKAsm2();
    const qreal angle1 = dialogTool->getAngle1();
    const qreal angle2 = dialogTool->getAngle2();
    const qreal kCurve = dialogTool->getKCurve();
    VToolSpline *spl = nullptr;
    spl = Create(0, p1, p4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, doc, data, Document::FullParse,
                 Source::FromGui);
    if (spl != nullptr)
    {
        spl->dialog=dialogTool;
    }
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param p1 id first spline point.
 * @param p4 id last spline point.
 * @param kAsm1 coefficient of length first control line.
 * @param kAsm2 coefficient of length second control line.
 * @param angle1 angle from first point to first control point.
 * @param angle2 angle from second point to second control point.
 * @param kCurve coefficient of curvature spline.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolSpline* VToolSpline::Create(const quint32 _id, const quint32 &p1, const quint32 &p4, const qreal &kAsm1,
                                 const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                                 VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                 const Document &parse, const Source &typeCreation)
{
    VPointF point1 = *data->GeometricObject<VPointF>(p1);
    VPointF point4 = *data->GeometricObject<VPointF>(p4);
    VSpline *spline = new VSpline(point1, point4, angle1, angle2, kAsm1, kAsm2, kCurve);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(spline);
        data->AddCurveLength<VSplineLength>(id);
    }
    else
    {
        data->UpdateGObject(id, spline);
        data->AddCurveLength<VSplineLength>(id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Spline, doc);
    if (parse == Document::FullParse)
    {
        VToolSpline *spl = new VToolSpline(doc, data, id, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, spl, &VToolSpline::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, spl, &VToolSpline::Disable);
        doc->AddTool(id, spl);
        doc->IncrementReferens(p1);
        doc->IncrementReferens(p4);
        return spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VToolSpline::getSpline() const
{
    QSharedPointer<VSpline> spline = VAbstractTool::data.GeometricObject<VSpline>(id);
    return *spline.data();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::setSpline(const VSpline &spl)
{
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    QSharedPointer<VSpline> spline = qSharedPointerDynamicCast<VSpline>(obj);
    *spline.data() = spl;
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolSpline *visual = new VisToolSpline(getData(), this);
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);

            const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
            visual->setPoint1Id(spl->GetP1().id());
            visual->setPoint4Id(spl->GetP4().id());
            visual->setAngle1(spl->GetAngle1());
            visual->setAngle2(spl->GetAngle2());
            visual->setKAsm1(spl->GetKasm1());
            visual->setKAsm2(spl->GetKasm2());
            visual->setKCurve(spl->GetKcurve());
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolSpline *visual = qobject_cast<VisToolSpline *>(vis);
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
    ShowFoot(show);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ControlPointChangePosition handle change position control point.
 * @param indexSpline position spline in spline list.
 * @param position position point in spline.
 * @param pos new position.
 */
void VToolSpline::ControlPointChangePosition(const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos)
{
    Q_UNUSED(indexSpline);
    const QSharedPointer<VSpline> spline = VAbstractTool::data.GeometricObject<VSpline>(id);
    VSpline spl;
    if (position == SplinePointPosition::FirstPoint)
    {
        spl = VSpline(spline->GetP1(), pos, spline->GetP3(), spline->GetP4(), spline->GetKcurve());
    }
    else
    {
        spl = VSpline(spline->GetP1(), spline->GetP2(), pos, spline->GetP4(), spline->GetKcurve());
    }

    MoveSpline *moveSpl = new MoveSpline(doc, spline.data(), spl, id, this->scene());
    connect(moveSpl, &MoveSpline::NeedLiteParsing, doc, &VPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveSpl);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogSpline>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSpline::RemoveReferens()
{
    const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    doc->DecrementReferens(spl->GetP1().id());
    doc->DecrementReferens(spl->GetP4().id());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSpline::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogSpline *dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);

    VPointF point1 = *VAbstractTool::data.GeometricObject<VPointF>(dialogTool->getP1());
    VPointF point4 = *VAbstractTool::data.GeometricObject<VPointF>(dialogTool->getP4());
    VSpline spl = VSpline (point1, point4, dialogTool->getAngle1(), dialogTool->getAngle2(),
                           dialogTool->getKAsm1(), dialogTool->getKAsm2(), dialogTool->getKCurve());

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    controlPoints[0]->setPos(spl.GetP2());
    controlPoints[1]->setPos(spl.GetP3());

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    spl = VSpline (point1, controlPoints[0]->pos(), controlPoints[1]->pos(), point4, dialogTool->getKCurve());

    doc->SetAttribute(domElement, AttrPoint1, spl.GetP1().id());
    doc->SetAttribute(domElement, AttrPoint4, spl.GetP4().id());
    doc->SetAttribute(domElement, AttrAngle1, spl.GetAngle1());
    doc->SetAttribute(domElement, AttrAngle2, spl.GetAngle2());
    doc->SetAttribute(domElement, AttrKAsm1, spl.GetKasm1());
    doc->SetAttribute(domElement, AttrKAsm2, spl.GetKasm2());
    doc->SetAttribute(domElement, AttrKCurve, spl.GetKcurve());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VSpline> spl = qSharedPointerDynamicCast<VSpline>(obj);
    SCASSERT(spl.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrPoint1, spl->GetP1().id());
    doc->SetAttribute(tag, AttrPoint4, spl->GetP4().id());
    doc->SetAttribute(tag, AttrAngle1, spl->GetAngle1());
    doc->SetAttribute(tag, AttrAngle2, spl->GetAngle2());
    doc->SetAttribute(tag, AttrKAsm1, spl->GetKasm1());
    doc->SetAttribute(tag, AttrKAsm2, spl->GetKasm2());
    doc->SetAttribute(tag, AttrKCurve, spl->GetKcurve());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSpline::RefreshGeometry()
{
    //this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    if (isHovered)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }

    const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    QPointF splinePoint = VAbstractTool::data.GeometricObject<VPointF>(spl->GetP1().id())->toQPointF();
    QPointF controlPoint = spl->GetP2();
    emit RefreshLine(1, SplinePointPosition::FirstPoint, controlPoint, splinePoint);
    splinePoint = VAbstractTool::data.GeometricObject<VPointF>(spl->GetP4().id())->toQPointF();
    controlPoint = spl->GetP3();
    emit RefreshLine(1, SplinePointPosition::LastPoint, controlPoint, splinePoint);

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    controlPoints[0]->setPos(spl->GetP2());
    controlPoints[1]->setPos(spl->GetP3());

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    if (vis != nullptr)
    {
        VisToolSpline *visual = qobject_cast<VisToolSpline *>(vis);
        visual->setPoint1Id(spl->GetP1().id());
        visual->setPoint4Id(spl->GetP4().id());
        visual->setAngle1(spl->GetAngle1());
        visual->setAngle2(spl->GetAngle2());
        visual->setKAsm1(spl->GetKasm1());
        visual->setKAsm2(spl->GetKasm2());
        visual->setKCurve(spl->GetKcurve());
        visual->RefreshGeometry();
    }
}
