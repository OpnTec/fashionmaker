/************************************************************************
 **
 **  @file   vtoolspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QDomElement>
#include <QEvent>
#include <QFlags>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QList>
#include <QPen>
#include <QPoint>
#include <QRectF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QUndoStack>
#include <QVector>
#include <Qt>
#include <new>

#include "../../../dialogs/tools/dialogspline.h"
#include "../../../dialogs/tools/dialogtool.h"
#include "../../../undocommands/movespline.h"
#include "../../../visualization/visualization.h"
#include "../../../visualization/path/vistoolspline.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../qmuparser/qmutokenparser.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vmath.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../vabstracttool.h"
#include "../vdrawtool.h"
#include "vabstractspline.h"

const QString VToolSpline::ToolType = QStringLiteral("simpleInteractive");
const QString VToolSpline::OldToolType = QStringLiteral("simple");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSpline constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolSpline::VToolSpline(VToolSplineInitData initData, QGraphicsItem *parent)
    : VAbstractSpline(initData.doc, initData.data, initData.id, parent),
      oldPosition(),
      moved(false),
      oldMoveSpline(),
      newMoveSpline()
{
    sceneType = SceneObject::Spline;

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(initData.id);

    auto InitControlPoint = [this](VControlPointSpline* cPoint)
    {
        connect(cPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        connect(this, &VToolSpline::setEnabledPoint, cPoint, &VControlPointSpline::setEnabledPoint);
        connect(cPoint, &VControlPointSpline::ShowContextMenu, this, &VToolSpline::contextMenuEvent);
        connect(cPoint, &VControlPointSpline::Released, this, &VToolSpline::CurveReleased);
        connect(cPoint, &VControlPointSpline::Selected, this, &VToolSpline::CurveSelected);
        controlPoints.append(cPoint);
    };

    const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl->GetStartAngleFormula());
    const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl->GetC1LengthFormula());

    auto *controlPoint1 = new VControlPointSpline(1, SplinePointPosition::FirstPoint,
                                                  static_cast<QPointF>(spl->GetP2()),
                                                  static_cast<QPointF>(spl->GetP1()),
                                                  freeAngle1, freeLength1, this);
    InitControlPoint(controlPoint1);

    const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl->GetEndAngleFormula());
    const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl->GetC2LengthFormula());

    auto *controlPoint2 = new VControlPointSpline(1, SplinePointPosition::LastPoint,
                                                  static_cast<QPointF>(spl->GetP3()),
                                                  static_cast<QPointF>(spl->GetP4()),
                                                  freeAngle2, freeLength2, this);
    InitControlPoint(controlPoint2);

    ShowHandles(detailsMode);

    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolSpline::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogSpline> dialogTool = m_dialog.objectCast<DialogSpline>();
    SCASSERT(not dialogTool.isNull())
    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(m_id);
    dialogTool->SetSpline(*spl);
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
VToolSpline* VToolSpline::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogSpline> dialogTool = dialog.objectCast<DialogSpline>();
    SCASSERT(not dialogTool.isNull())

    VToolSplineInitData initData;
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    auto spl = Create(initData, new VSpline(dialogTool->GetSpline()));

    if (spl != nullptr)
    {
        spl->m_dialog = dialogTool;
    }
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 * @param spline spline.
 * @return the created tool
 */
VToolSpline* VToolSpline::Create(VToolSplineInitData &initData, VSpline *spline)
{
    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(spline);
        initData.data->AddSpline(initData.data->GeometricObject<VAbstractBezier>(initData.id), initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, spline);
        initData.data->AddSpline(initData.data->GeometricObject<VAbstractBezier>(initData.id), initData.id);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::Spline, initData.doc);
        auto _spl = new VToolSpline(initData);
        initData.scene->addItem(_spl);
        InitSplineToolConnections(initData.scene, _spl);
        VAbstractPattern::AddTool(initData.id, _spl);
        initData.doc->IncrementReferens(spline->GetP1().getIdTool());
        initData.doc->IncrementReferens(spline->GetP4().getIdTool());
        return _spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSpline *VToolSpline::Create(VToolSplineInitData &initData)
{
    const qreal calcAngle1 = CheckFormula(initData.id, initData.a1, initData.data);
    const qreal calcAngle2 = CheckFormula(initData.id, initData.a2, initData.data);

    const qreal calcLength1 = qApp->toPixel(CheckFormula(initData.id, initData.l1, initData.data));
    const qreal calcLength2 = qApp->toPixel(CheckFormula(initData.id, initData.l2, initData.data));

    auto p1 = initData.data->GeometricObject<VPointF>(initData.point1);
    auto p4 = initData.data->GeometricObject<VPointF>(initData.point4);

    auto spline = new VSpline(*p1, *p4, calcAngle1, initData.a1, calcAngle2, initData.a2, calcLength1, initData.l1,
                              calcLength2, initData.l2);
    if (initData.duplicate > 0)
    {
        spline->SetDuplicate(initData.duplicate);
    }

    spline->SetColor(initData.color);
    spline->SetPenStyle(initData.penStyle);
    spline->SetApproximationScale(initData.approximationScale);

    return VToolSpline::Create(initData, spline);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VToolSpline::getSpline() const
{
    auto spline = VAbstractTool::data.GeometricObject<VSpline>(m_id);
    return *spline.data();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::setSpline(const VSpline &spl)
{
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
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
    Q_UNUSED(indexSpline)
    const QSharedPointer<VSpline> spline = VAbstractTool::data.GeometricObject<VSpline>(m_id);

    if (qApp->Settings()->IsFreeCurveMode() && not moved)
    {
        oldMoveSpline = spline;
        moved = true;
    }

    const VSpline spl = CorrectedSpline(*spline, position, pos);

    if (not qApp->Settings()->IsFreeCurveMode())
    {
        UndoCommandMove(*spline, spl);
    }
    else
    {
        newMoveSpline = QSharedPointer<VSpline>(new VSpline(spl));
        VAbstractTool::data.UpdateGObject(m_id, newMoveSpline);
        RefreshGeometry();

        if (QGraphicsScene *sc = scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), this);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::EnableToolMove(bool move)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, move);

    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemIsMovable, move);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSpline::RemoveReferens()
{
    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(m_id);
    doc->DecrementReferens(spl->GetP1().getIdTool());
    doc->DecrementReferens(spl->GetP4().getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSpline::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies, QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    auto dialogTool = qobject_cast<DialogSpline*>(m_dialog);
    SCASSERT(dialogTool != nullptr)

    const auto oldSpl = VAbstractTool::data.GeometricObject<VSpline>(m_id);
    AddDependence(oldDependencies, oldSpl->GetP1().id());
    AddDependence(oldDependencies, oldSpl->GetP4().id());

    const VSpline spl = dialogTool->GetSpline();
    AddDependence(newDependencies, spl.GetP1().id());
    AddDependence(newDependencies, spl.GetP4().id());

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    controlPoints[0]->setPos(static_cast<QPointF>(spl.GetP2()));
    controlPoints[1]->setPos(static_cast<QPointF>(spl.GetP3()));

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    SetSplineAttributes(domElement, spl);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    auto spl = qSharedPointerDynamicCast<VSpline>(obj);
    SCASSERT(spl.isNull() == false)
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
                SetItemOverrideCursor(this, cursorArrowCloseHand, 1, 1);
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
                SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);

                CurveReleased();
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

        const auto spline = VAbstractTool::data.GeometricObject<VSpline>(m_id);

        if (qApp->Settings()->IsFreeCurveMode() && not moved)
        {
            oldMoveSpline = spline;
            moved = true;
        }

        const qreal t = spline->ParamT(oldPosition);

        if (qFloor(t) == -1)
        {
            moved = false;
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

        const QPointF p2 = static_cast<QPointF>(spline->GetP2()) + offset0;
        const QPointF p3 = static_cast<QPointF>(spline->GetP3()) + offset1;

        oldPosition = event->scenePos(); // Now mouse here

        newMoveSpline = QSharedPointer<VSpline>(new VSpline(spline->GetP1(), p2, p3, spline->GetP4()));

        if (not qApp->Settings()->IsFreeCurveMode())
        {
            UndoCommandMove(*spline, *newMoveSpline);
        }
        else
        {
            VAbstractTool::data.UpdateGObject(m_id, newMoveSpline);
            RefreshGeometry();

            if (QGraphicsScene *sc = scene())
            {
                VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), this);
            }
        }

        // Each time we move something we call recalculation scene rect. In some cases this can cause moving
        // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished)
        {
           changeFinished = false;

           const QList<QGraphicsView *> viewList = scene()->views();
           if (not viewList.isEmpty())
           {
               if (VMainGraphicsView *view = qobject_cast<VMainGraphicsView *>(viewList.at(0)))
               {
                   VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                   SCASSERT(currentScene)
                   const QPointF cursorPosition = currentScene->getScenePos();
                   const qreal scale = SceneScale(scene());
                   view->EnsureVisibleWithDelay(QRectF(cursorPosition.x()-5/scale, cursorPosition.y()-5/scale, 10/scale,
                                                       10/scale),
                                                VMainGraphicsView::scrollDelay);
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
            SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
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
            setCursor(QCursor());
        }
    }

    VAbstractSpline::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolSpline *visual = qobject_cast<VisToolSpline *>(vis);
        SCASSERT(visual != nullptr)

        const QSharedPointer<VSpline> spl = VAbstractTool::data.GeometricObject<VSpline>(m_id);
        visual->setObject1Id(spl->GetP1().id());
        visual->setObject4Id(spl->GetP4().id());
        visual->SetAngle1(spl->GetStartAngle());
        visual->SetAngle2(spl->GetEndAngle());
        visual->SetKAsm1(spl->GetKasm1());
        visual->SetKAsm2(spl->GetKasm2());
        visual->SetKCurve(spl->GetKcurve());
        visual->setLineStyle(LineStyleToPenStyle(spl->GetPenStyle()));
        visual->setApproximationScale(spl->GetApproximationScale());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolSpline::IsMovable() const
{
    const auto spline = VAbstractTool::data.GeometricObject<VSpline>(m_id);

    return qmu::QmuTokenParser::IsSingle(spline->GetStartAngleFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetEndAngleFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetC1LengthFormula()) &&
           qmu::QmuTokenParser::IsSingle(spline->GetC2LengthFormula());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::RefreshCtrlPoints()
{
    // Very important to disable control points. Without it the pogram can't move the curve.
    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    const auto spl = VAbstractTool::data.GeometricObject<VSpline>(m_id);

    controlPoints[0]->blockSignals(true);
    controlPoints[1]->blockSignals(true);

    {
        const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl->GetStartAngleFormula());
        const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl->GetC1LengthFormula());

        const QPointF splinePoint =
                static_cast<QPointF>(*VAbstractTool::data.GeometricObject<VPointF>(spl->GetP1().id()));
        controlPoints[0]->RefreshCtrlPoint(1, SplinePointPosition::FirstPoint, static_cast<QPointF>(spl->GetP2()),
                                           static_cast<QPointF>(splinePoint), freeAngle1, freeLength1);
    }

    {
        const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl->GetEndAngleFormula());
        const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl->GetC2LengthFormula());

        const QPointF splinePoint =
                static_cast<QPointF>(*VAbstractTool::data.GeometricObject<VPointF>(spl->GetP4().id()));
        controlPoints[1]->RefreshCtrlPoint(1, SplinePointPosition::LastPoint, static_cast<QPointF>(spl->GetP3()),
                                           static_cast<QPointF>(splinePoint), freeAngle2, freeLength2);
    }

    controlPoints[0]->blockSignals(false);
    controlPoints[1]->blockSignals(false);

    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::CurveReleased()
{
    if (qApp->Settings()->IsFreeCurveMode() && moved)
    {
        UndoCommandMove(*oldMoveSpline, *newMoveSpline);

        oldMoveSpline.clear();
        newMoveSpline.clear();

        moved = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    Q_UNUSED(id)
    try
    {
        ContextMenu<DialogSpline>(event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::SetSplineAttributes(QDomElement &domElement, const VSpline &spl)
{
    SCASSERT(doc != nullptr)

    doc->SetAttribute(domElement, AttrType,    ToolType);
    doc->SetAttribute(domElement, AttrPoint1,  spl.GetP1().id());
    doc->SetAttribute(domElement, AttrPoint4,  spl.GetP4().id());
    doc->SetAttribute(domElement, AttrAngle1,  spl.GetStartAngleFormula());
    doc->SetAttribute(domElement, AttrAngle2,  spl.GetEndAngleFormula());
    doc->SetAttribute(domElement, AttrLength1, spl.GetC1LengthFormula());
    doc->SetAttribute(domElement, AttrLength2, spl.GetC2LengthFormula());
    doc->SetAttribute(domElement, AttrColor,   spl.GetColor());
    doc->SetAttribute(domElement, AttrPenStyle, spl.GetPenStyle());
    doc->SetAttribute(domElement, AttrAScale, spl.GetApproximationScale());

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

//---------------------------------------------------------------------------------------------------------------------
void VToolSpline::UndoCommandMove(const VSpline &oldSpl, const VSpline &newSpl)
{
    MoveSpline *moveSpl = new MoveSpline(doc, oldSpl, newSpl, m_id);
    connect(moveSpl, &MoveSpline::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveSpl);
}
