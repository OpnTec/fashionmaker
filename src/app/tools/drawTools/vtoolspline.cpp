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
    const VSpline *spl = data->GeometricObject<const VSpline *>(id);
    QPainterPath path;
    path.addPath(spl->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

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
void VToolSpline::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSpline *dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
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
 */
void VToolSpline::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
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
    Create(0, p1, p4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, doc, data, Document::FullParse,
           Source::FromGui);
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
 */
void VToolSpline::Create(const quint32 _id, const quint32 &p1, const quint32 &p4, const qreal &kAsm1,
                         const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                         VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                         const Document &parse, const Source &typeCreation)
{
    VPointF point1 = *data->GeometricObject<const VPointF *>(p1);
    VPointF point4 = *data->GeometricObject<const VPointF *>(p4);
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
    VDrawTool::AddRecord(id, Tool::SplineTool, doc);
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
    }
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
    const VSpline *spline = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    VSpline spl;
    if (position == SplinePointPosition::FirstPoint)
    {
        spl = VSpline(spline->GetP1(), pos, spline->GetP3(), spline->GetP4(), spline->GetKcurve());
    }
    else
    {
        spl = VSpline(spline->GetP1(), spline->GetP2(), pos, spline->GetP4(), spline->GetKcurve());
    }

    MoveSpline *moveSpl = new MoveSpline(doc, spline, spl, id, this->scene());
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
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolSpline::AddToFile()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrPoint1, spl->GetP1().id());
    doc->SetAttribute(domElement, AttrPoint4, spl->GetP4().id());
    doc->SetAttribute(domElement, AttrAngle1, spl->GetAngle1());
    doc->SetAttribute(domElement, AttrAngle2, spl->GetAngle2());
    doc->SetAttribute(domElement, AttrKAsm1, spl->GetKasm1());
    doc->SetAttribute(domElement, AttrKAsm2, spl->GetKasm2());
    doc->SetAttribute(domElement, AttrKCurve, spl->GetKcurve());

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolSpline::RefreshDataInFile()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrPoint1, spl->GetP1().id());
        doc->SetAttribute(domElement, AttrPoint4, spl->GetP4().id());
        doc->SetAttribute(domElement, AttrAngle1, spl->GetAngle1());
        doc->SetAttribute(domElement, AttrAngle2, spl->GetAngle2());
        doc->SetAttribute(domElement, AttrKAsm1, spl->GetKasm1());
        doc->SetAttribute(domElement, AttrKAsm2, spl->GetKasm2());
        doc->SetAttribute(domElement, AttrKCurve, spl->GetKcurve());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent  handle mouse release events.
 * @param event context menu event.
 */
void VToolSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSpline::RemoveReferens()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
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

    VPointF point1 = *VAbstractTool::data.GeometricObject<const VPointF *>(dialogTool->getP1());
    VPointF point4 = *VAbstractTool::data.GeometricObject<const VPointF *>(dialogTool->getP4());
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
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSpline::RefreshGeometry()
{
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QPainterPath path;
    path.addPath(spl->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    QPointF splinePoint = VAbstractTool::data.GeometricObject<const VPointF *>(spl->GetP1().id())->toQPointF();
    QPointF controlPoint = spl->GetP2();
    emit RefreshLine(1, SplinePointPosition::FirstPoint, controlPoint, splinePoint);
    splinePoint = VAbstractTool::data.GeometricObject<const VPointF *>(spl->GetP4().id())->toQPointF();
    controlPoint = spl->GetP3();
    emit RefreshLine(1, SplinePointPosition::LastPoint, controlPoint, splinePoint);

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    controlPoints[0]->setPos(spl->GetP2());
    controlPoints[1]->setPos(spl->GetP3());

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);
}
