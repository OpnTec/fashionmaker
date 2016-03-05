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

#include "vtoolspline.h"
#include "../vgeometry/vspline.h"
#include "../../../dialogs/tools/dialogspline.h"
#include "../../../undocommands/movespline.h"
#include "../../../visualization/vistoolspline.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "../qmuparser/qmutokenparser.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

const QString VToolSpline::ToolType = QStringLiteral("simpleInteractive");
const QString VToolSpline::OldToolType = QStringLiteral("simple");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSpline constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolSpline::VToolSpline(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &color,
                         const Source &typeCreation,
                         QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent), oldPosition()
{
    sceneType = SceneObject::Spline;
    lineColor = color;

    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setAcceptHoverEvents(true);
    this->setPath(ToolPath());

    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(id);

    const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl->GetStartAngleFormula());
    const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl->GetC1LengthFormula());

    auto *controlPoint1 = new VControlPointSpline(1, SplinePointPosition::FirstPoint, spl->GetP2(),
                                                  spl->GetP1().toQPointF(), *data->GetPatternUnit(), freeAngle1,
                                                  freeLength1, this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint1, &VControlPointSpline::setEnabledPoint);
    connect(controlPoint1, &VControlPointSpline::ShowContextMenu, this, &VToolSpline::contextMenuEvent);
    controlPoints.append(controlPoint1);

    const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl->GetEndAngleFormula());
    const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl->GetC2LengthFormula());

    auto *controlPoint2 = new VControlPointSpline(1, SplinePointPosition::LastPoint, spl->GetP3(),
                                                  spl->GetP4().toQPointF(), *data->GetPatternUnit(), freeAngle2,
                                                  freeLength2, this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint2, &VControlPointSpline::setEnabledPoint);
    connect(controlPoint2, &VControlPointSpline::ShowContextMenu, this, &VToolSpline::contextMenuEvent);
    controlPoints.append(controlPoint2);

    ShowHandles(false);

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolSpline::~VToolSpline()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
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
    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    dialogTool->SetSpline(*spl);
    dialogTool->SetColor(lineColor);
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
VToolSpline* VToolSpline::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);

    auto spl = Create(0, new VSpline(dialogTool->GetSpline()), dialogTool->GetColor(), scene, doc, data,
                      Document::FullParse, Source::FromGui);

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
 * @param spline spline.
 * @param color spline color.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolSpline* VToolSpline::Create(const quint32 _id, VSpline *spline, const QString &color, VMainGraphicsScene *scene,
                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                 const Source &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(spline);
        data->AddCurve<VSpline>(id);
    }
    else
    {
        data->UpdateGObject(id, spline);
        data->AddCurve<VSpline>(id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Spline, doc);
    if (parse == Document::FullParse)
    {
        auto _spl = new VToolSpline(doc, data, id, color, typeCreation);
        scene->addItem(_spl);
        connect(_spl, &VToolSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, _spl, &VToolSpline::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, _spl, &VToolSpline::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, _spl, &VToolSpline::EnableToolMove);
        connect(scene, &VMainGraphicsScene::CurveDetailsMode, _spl, &VToolSpline::DetailsMode);
        doc->AddTool(id, _spl);
        doc->IncrementReferens(spline->GetP1().id());
        doc->IncrementReferens(spline->GetP4().id());
        return _spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSpline *VToolSpline::Create(const quint32 _id, quint32 point1, quint32 point4, QString &a1, QString &a2,
                                 QString &l1, QString &l2, quint32 duplicate, const QString &color,
                                 VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                 const Document &parse, const Source &typeCreation)
{
    const qreal calcAngle1 = CheckFormula(_id, a1, data);
    const qreal calcAngle2 = CheckFormula(_id, a2, data);

    const qreal calcLength1 = qApp->toPixel(CheckFormula(_id, l1, data));
    const qreal calcLength2 = qApp->toPixel(CheckFormula(_id, l2, data));

    auto p1 = data->GeometricObject<VPointF>(point1);
    auto p4 = data->GeometricObject<VPointF>(point4);

    auto spline = new VSpline(*p1, *p4, calcAngle1, a1, calcAngle2, a2, calcLength1, l1, calcLength2, l2);
    if (duplicate > 0)
    {
        spline->SetDuplicate(duplicate);
    }

    return VToolSpline::Create(_id, spline, color, scene, doc, data, parse, typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VToolSpline::getSpline() const
{
    auto spline = VAbstractTool::data.GeometricObject<VSpline>(id);
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
    ShowToolVisualization<VisToolSpline>(show);
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
    const VSpline spl = CorrectedSpline(*spline, position, pos);

    MoveSpline *moveSpl = new MoveSpline(doc, spline.data(), spl, id, this->scene());
    connect(moveSpl, &MoveSpline::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveSpl);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::EnableToolMove(bool move)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogSpline>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSpline::RemoveReferens()
{
    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(id);
    doc->DecrementReferens(spl->GetP1().getIdTool());
    doc->DecrementReferens(spl->GetP4().getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSpline::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);

    const VSpline spl = dialogTool->GetSpline();

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    controlPoints[0]->setPos(spl.GetP2());
    controlPoints[1]->setPos(spl.GetP3());

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    SetSplineAttributes(domElement, spl);
    doc->SetAttribute(domElement, AttrColor,   dialogTool->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    auto spl = qSharedPointerDynamicCast<VSpline>(obj);
    SCASSERT(spl.isNull() == false);
    SetSplineAttributes(tag, *spl);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            if (IsMovable())
            {
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
                oldPosition = event->scenePos();
                event->accept();
            }
        }
    }
    VAbstractSpline::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            if (IsMovable())
            {
                //Disable cursor-arrow-closehand
                RestoreOverrideCursor(cursorArrowCloseHand);
            }
        }
    }
    VAbstractSpline::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (IsMovable())
    {
        // Don't need check if left mouse button was pressed. According to the Qt documentation "If you do receive this
        // event, you can be certain that this item also received a mouse press event, and that this item is the current
        // mouse grabber.".

        // Magic Bezier Drag Equations follow!
        // "weight" describes how the influence of the drag should be distributed
        // among the handles; 0 = front handle only, 1 = back handle only.

        const auto spline = VAbstractTool::data.GeometricObject<VSpline>(id);
        const qreal t = spline->ParamT(oldPosition);

        if (qFloor(t) == -1)
        {
            return;
        }

        double weight;
        if (t <= 1.0 / 6.0)
        {
            weight = 0;
        }
        else if (t <= 0.5)
        {
            weight = (pow((6 * t - 1) / 2.0, 3)) / 2;
        }
        else if (t <= 5.0 / 6.0)
        {
            weight = (1 - pow((6 * (1-t) - 1) / 2.0, 3)) / 2 + 0.5;
        }
        else
        {
            weight = 1;
        }

        const QPointF delta = event->scenePos() - oldPosition;
        const QPointF offset0 = ((1-weight)/(3*t*(1-t)*(1-t))) * delta;
        const QPointF offset1 = (weight/(3*t*t*(1-t))) * delta;

        const QPointF p2 = spline->GetP2() + offset0;
        const QPointF p3 = spline->GetP3() + offset1;

        oldPosition = event->scenePos(); // Now mouse here

        VSpline spl = VSpline(spline->GetP1(), p2, p3, spline->GetP4());

        MoveSpline *moveSpl = new MoveSpline(doc, spline.data(), spl, id, this->scene());
        connect(moveSpl, &MoveSpline::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(moveSpl);

        // Each time we move something we call recalculation scene rect. In some cases this can cause moving
        // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished)
        {
           changeFinished = false;

           const QList<QGraphicsView *> viewList = scene()->views();
           if (not viewList.isEmpty())
           {
               if (QGraphicsView *view = viewList.at(0))
               {
                   VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                   SCASSERT(currentScene);
                   const QPointF cursorPosition = currentScene->getScenePos();
                   view->ensureVisible(QRectF(cursorPosition.x()-5, cursorPosition.y()-5, 10, 10));
               }
           }
           changeFinished = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (IsMovable())
        {
            SetOverrideCursor(cursorArrowOpenHand, 1, 1);
        }
    }

    VAbstractSpline::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (IsMovable())
        {
            //Disable cursor-arrow-openhand
            RestoreOverrideCursor(cursorArrowOpenHand);
        }
    }

    VAbstractSpline::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolSpline *visual = qobject_cast<VisToolSpline *>(vis);
        SCASSERT(visual != nullptr);

        const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(id);
        visual->setObject1Id(spl->GetP1().id());
        visual->setObject4Id(spl->GetP4().id());
        visual->SetAngle1(spl->GetStartAngle());
        visual->SetAngle2(spl->GetEndAngle());
        visual->SetKAsm1(spl->GetKasm1());
        visual->SetKAsm2(spl->GetKasm2());
        visual->SetKCurve(spl->GetKcurve());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolSpline::IsMovable() const
{
    const auto spline = VAbstractTool::data.GeometricObject<VSpline>(id);

    return qmu::QmuTokenParser::IsSingle(spline->GetStartAngleFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetEndAngleFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetC1LengthFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetC2LengthFormula());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSpline::RefreshGeometry()
{
    // Very important to disable control points. Without it the pogram can't move the curve.
    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    this->setPen(QPen(CorrectColor(lineColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    if (isHovered || detailsMode)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(id);

    {
        const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl->GetStartAngleFormula());
        const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl->GetC1LengthFormula());

        const QPointF splinePoint = VAbstractTool::data.GeometricObject<VPointF>(spl->GetP1().id())->toQPointF();
        controlPoints[0]->RefreshCtrlPoint(1, SplinePointPosition::FirstPoint, spl->GetP2(), splinePoint, freeAngle1,
                                           freeLength1);
    }

    {
        const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl->GetEndAngleFormula());
        const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl->GetC2LengthFormula());

        const QPointF splinePoint = VAbstractTool::data.GeometricObject<VPointF>(spl->GetP4().id())->toQPointF();
        controlPoints[1]->RefreshCtrlPoint(1, SplinePointPosition::LastPoint, spl->GetP3(), splinePoint, freeAngle2,
                                           freeLength2);
    }

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    SetVisualization();

    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SetSplineAttributes(QDomElement &domElement, const VSpline &spl)
{
    SCASSERT(doc != nullptr);

    doc->SetAttribute(domElement, AttrType,    ToolType);
    doc->SetAttribute(domElement, AttrPoint1,  spl.GetP1().id());
    doc->SetAttribute(domElement, AttrPoint4,  spl.GetP4().id());
    doc->SetAttribute(domElement, AttrAngle1,  spl.GetStartAngleFormula());
    doc->SetAttribute(domElement, AttrAngle2,  spl.GetEndAngleFormula());
    doc->SetAttribute(domElement, AttrLength1, spl.GetC1LengthFormula());
    doc->SetAttribute(domElement, AttrLength2, spl.GetC2LengthFormula());

    if (spl.GetDuplicate() > 0)
    {
        doc->SetAttribute(domElement, AttrDuplicate, spl.GetDuplicate());
    }
    else
    {
        if (domElement.hasAttribute(AttrDuplicate))
        {
            domElement.removeAttribute(AttrDuplicate);
        }
    }

    if (domElement.hasAttribute(AttrKCurve))
    {
        domElement.removeAttribute(AttrKCurve);
    }

    if (domElement.hasAttribute(AttrKAsm1))
    {
        domElement.removeAttribute(AttrKAsm1);
    }

    if (domElement.hasAttribute(AttrKAsm2))
    {
        domElement.removeAttribute(AttrKAsm2);
    }
}
