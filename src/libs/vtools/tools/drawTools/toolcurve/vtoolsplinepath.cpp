/************************************************************************
 **
 **  @file   vtoolsplinepath.cpp
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

#include "vtoolsplinepath.h"

#include <QDomElement>
#include <QEvent>
#include <QFlags>
#include <QForeachContainer>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
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
#include <Qt>
#include <new>

#include "../../../dialogs/tools/dialogtool.h"
#include "../../../dialogs/tools/dialogsplinepath.h"
#include "../../../undocommands/movesplinepath.h"
#include "../../../undocommands/vundocommand.h"
#include "../../../visualization/visualization.h"
#include "../../../visualization/path/vistoolsplinepath.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../qmuparser/qmutokenparser.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepoint.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vmath.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/../vgeometry/vsplinepath.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../vabstracttool.h"
#include "../vdrawtool.h"
#include "vabstractspline.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;

const QString VToolSplinePath::ToolType = QStringLiteral("pathInteractive");
const QString VToolSplinePath::OldToolType = QStringLiteral("path");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolSplinePath::VToolSplinePath(VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                                 QGraphicsItem *parent)
    : VAbstractSpline(doc, data, id, parent),
      oldPosition(),
      splIndex(-1)
{
    sceneType = SceneObject::SplinePath;

    this->setPath(ToolPath());
    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(id);
    for (qint32 i = 1; i<=splPath->CountSubSpl(); ++i)
    {
        const VSpline spl = splPath->GetSpline(i);

        const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl.GetStartAngleFormula());
        const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl.GetC1LengthFormula());

        auto *controlPoint = new VControlPointSpline(i, SplinePointPosition::FirstPoint, spl.GetP2(),
                                                     spl.GetP1(), *data->GetPatternUnit(), freeAngle1,
                                                     freeLength1, this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        connect(controlPoint, &VControlPointSpline::ShowContextMenu, this, &VToolSplinePath::contextMenuEvent);
        controlPoints.append(controlPoint);

        const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl.GetEndAngleFormula());
        const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl.GetC2LengthFormula());

        controlPoint = new VControlPointSpline(i, SplinePointPosition::LastPoint, spl.GetP3(), spl.GetP4(),
                                               *data->GetPatternUnit(), freeAngle2, freeLength2, this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        connect(controlPoint, &VControlPointSpline::ShowContextMenu, this, &VToolSplinePath::contextMenuEvent);
        controlPoints.append(controlPoint);
    }

    ShowHandles(false);

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolSplinePath::~VToolSplinePath()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    dialogTool->SetPath(*splPath);
    dialogTool->SetColor(splPath->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolSplinePath* VToolSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                         VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)
    VSplinePath *path = new VSplinePath(dialogTool->GetPath());
    for (qint32 i = 0; i < path->CountPoints(); ++i)
    {
        doc->IncrementReferens((*path)[i].P().getIdTool());
    }
    VToolSplinePath* spl = Create(0, path, dialogTool->GetColor(), scene, doc, data, Document::FullParse,
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
 * @param path spline path.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolSplinePath* VToolSplinePath::Create(const quint32 _id, VSplinePath *path, const QString &color,
                                         VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                         const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    path->SetColor(color);
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(path);
        data->AddCurveWithSegments(data->GeometricObject<VAbstractCubicBezierPath>(id), id);
    }
    else
    {
        data->UpdateGObject(id, path);
        data->AddCurveWithSegments(data->GeometricObject<VAbstractCubicBezierPath>(id), id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::SplinePath, doc);
    if (parse == Document::FullParse)
    {
        VToolSplinePath *spl = new VToolSplinePath(doc, data, id, typeCreation);
        scene->addItem(spl);
        InitSplinePathToolConnections(scene, spl);
        doc->AddTool(id, spl);
        return spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSplinePath *VToolSplinePath::Create(const quint32 _id, const QVector<quint32> &points, QVector<QString> &a1,
                                         QVector<QString> &a2, QVector<QString> &l1, QVector<QString> &l2,
                                         const QString &color, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                         VContainer *data, const Document &parse, const Source &typeCreation)
{
    auto path = new VSplinePath();

    for (int i = 0; i < points.size(); ++i)
    {
        const qreal calcAngle1 = CheckFormula(_id, a1[i], data);
        const qreal calcAngle2 = CheckFormula(_id, a2[i], data);

        const qreal calcLength1 = qApp->toPixel(CheckFormula(_id, l1[i], data));
        const qreal calcLength2 = qApp->toPixel(CheckFormula(_id, l2[i], data));

        const auto p = *data->GeometricObject<VPointF>(points.at(i));

        path->append(VSplinePoint(p, calcAngle1, a1.at(i), calcAngle2, a2.at(i), calcLength1, l1.at(i), calcLength2,
                                  l2.at(i)));
    }

    return VToolSplinePath::Create(_id, path, color, scene, doc, data, parse, typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ControlPointChangePosition handle change position control point.
 * @param indexSpline position spline in spline list.
 * @param position position point in spline.
 * @param pos new position.
 */
void VToolSplinePath::ControlPointChangePosition(const qint32 &indexSpline, const SplinePointPosition &position,
                                                 const QPointF &pos)
{
    const VSplinePath oldSplPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
    VSplinePath newSplPath = oldSplPath;
    const VSpline spl = CorrectedSpline(newSplPath.GetSpline(indexSpline), position, pos);
    UpdateControlPoints(spl, newSplPath, indexSpline);

    MoveSplinePath *moveSplPath = new MoveSplinePath(doc, oldSplPath, newSplPath, id);
    connect(moveSplPath, &VUndoCommand::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveSplPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::EnableToolMove(bool move)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateControlPoints update position points control points in file.
 * @param spl spline what was changed.
 * @param splPath spline path.
 * @param indexSpline index spline in spline path.
 */
void VToolSplinePath::UpdateControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline) const
{
    VSplinePoint p = splPath.GetSplinePoint(indexSpline, SplinePointPosition::FirstPoint);
    p.SetAngle2(spl.GetStartAngle(), spl.GetStartAngleFormula());
    p.SetLength2(spl.GetC1Length(), spl.GetC1LengthFormula());
    splPath.UpdatePoint(indexSpline, SplinePointPosition::FirstPoint, p);

    p = splPath.GetSplinePoint(indexSpline, SplinePointPosition::LastPoint);
    p.SetAngle1(spl.GetEndAngle(), spl.GetEndAngleFormula());
    p.SetLength1(spl.GetC2Length(), spl.GetC2LengthFormula());
    splPath.UpdatePoint(indexSpline, SplinePointPosition::LastPoint, p);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::SetSplinePathAttributes(QDomElement &domElement, const VSplinePath &path)
{
    doc->SetAttribute(domElement, AttrType, ToolType);

    if (path.GetDuplicate() > 0)
    {
        doc->SetAttribute(domElement, AttrDuplicate, path.GetDuplicate());
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

    UpdatePathPoints(doc, domElement, path);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePathPoints update spline path in pattern file.
 * @param doc dom document container.
 * @param element tag in file.
 * @param path spline path.
 */
void VToolSplinePath::UpdatePathPoints(VAbstractPattern *doc, QDomElement &element, const VSplinePath &path)
{
    VDomDocument::RemoveAllChildren(element);
    for (qint32 i = 0; i < path.CountPoints(); ++i)
    {
        AddPathPoint(doc, element, path.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath VToolSplinePath::getSplinePath() const
{
    QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    return *splPath.data();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::setSplinePath(const VSplinePath &splPath)
{
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    QSharedPointer<VSplinePath> splinePath = qSharedPointerDynamicCast<VSplinePath>(obj);
    *splinePath.data() = splPath;
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolSplinePath>(show);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogSplinePath>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddPathPoint write path point to pattern file.
 * @param domElement dom element.
 * @param splPoint spline path point.
 */
void VToolSplinePath::AddPathPoint(VAbstractPattern *doc, QDomElement &domElement, const VSplinePoint &splPoint)
{
    SCASSERT(doc != nullptr)
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    doc->SetAttribute(pathPoint, AttrPSpline, splPoint.P().id());
    doc->SetAttribute(pathPoint, AttrLength1, splPoint.Length1Formula());
    doc->SetAttribute(pathPoint, AttrLength2, splPoint.Length2Formula());
    doc->SetAttribute(pathPoint, AttrAngle1, splPoint.Angle1Formula());
    doc->SetAttribute(pathPoint, AttrAngle2, splPoint.Angle2Formula());

    if (domElement.hasAttribute(AttrKAsm1))
    {
        domElement.removeAttribute(AttrKAsm1);
    }

    if (domElement.hasAttribute(AttrKAsm2))
    {
        domElement.removeAttribute(AttrKAsm2);
    }

    if (domElement.hasAttribute(AttrAngle))
    {
        domElement.removeAttribute(AttrAngle);
    }

    domElement.appendChild(pathPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSplinePath::RemoveReferens()
{
    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    for (qint32 i = 0; i < splPath->CountSubSpl(); ++i)
    {
        doc->DecrementReferens(splPath->at(i).P().getIdTool());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSplinePath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)

    const VSplinePath splPath = dialogTool->GetPath();
    for (qint32 i = 1; i <= splPath.CountSubSpl(); ++i)
    {
        VSpline spl = splPath.GetSpline(i);
        qint32 j = i*2;

        controlPoints[j-2]->blockSignals(true);
        controlPoints[j-1]->blockSignals(true);

        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());

        controlPoints[j-2]->blockSignals(false);
        controlPoints[j-1]->blockSignals(false);
    }

    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
    SetSplinePathAttributes(domElement, splPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    QSharedPointer<VSplinePath> splPath = qSharedPointerDynamicCast<VSplinePath>(obj);
    SCASSERT(splPath.isNull() == false)

    SetSplinePathAttributes(tag, *splPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            oldPosition = event->scenePos();
            const auto splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
            splIndex = splPath->Segment(oldPosition);
            if (IsMovable(splIndex))
            {
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
                event->accept();
            }
        }
    }
    VAbstractSpline::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            oldPosition = event->scenePos();
            //Disable cursor-arrow-closehand
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
    }
    VAbstractSpline::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't need to check if left mouse button was pressed. According to the Qt documentation "If you do receive this
    // event, you can be certain that this item also received a mouse press event, and that this item is the current
    // mouse grabber.".

    if (IsMovable(splIndex))
    {
        VSplinePath oldSplPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
        VSplinePath newSplPath = oldSplPath;

        VSpline spline = newSplPath.GetSpline(splIndex);
        const qreal t = spline.ParamT(oldPosition);

        if (qFloor(t) == -1)
        {
            return;
        }

        // Magic Bezier Drag Equations follow!
        // "weight" describes how the influence of the drag should be distributed
        // among the handles; 0 = front handle only, 1 = back handle only.

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

        const QPointF p2 = spline.GetP2() + offset0;
        const QPointF p3 = spline.GetP3() + offset1;

        oldPosition = event->scenePos(); // Now mouse here

        const VSpline spl = VSpline(spline.GetP1(), p2, p3, spline.GetP4());

        UpdateControlPoints(spl, newSplPath, splIndex);

        MoveSplinePath *moveSplPath = new MoveSplinePath(doc, oldSplPath, newSplPath, id);
        connect(moveSplPath, &VUndoCommand::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(moveSplPath);

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
                   SCASSERT(currentScene)
                   const QPointF cursorPosition = currentScene->getScenePos();
                   view->ensureVisible(QRectF(cursorPosition.x()-5, cursorPosition.y()-5, 10, 10));
               }
           }
           changeFinished = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        oldPosition = event->scenePos();
        const auto splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
        splIndex = splPath->Segment(oldPosition);
        if (IsMovable(splIndex))
        {
            SetOverrideCursor(cursorArrowOpenHand, 1, 1);
        }
    }

    VAbstractSpline::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        oldPosition = event->scenePos();
        //Disable cursor-arrow-openhand
        RestoreOverrideCursor(cursorArrowOpenHand);
    }

    VAbstractSpline::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolSplinePath *visual = qobject_cast<VisToolSplinePath *>(vis);
        SCASSERT(visual != nullptr)

        QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
        visual->setPath(*splPath.data());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolSplinePath::IsMovable(int index) const
{
    const auto splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);

    //index == -1 - can delete, but decided to left
    if (index == -1 || index < 1 || index > splPath->CountSubSpl())
    {
        return false;
    }

    const VSplinePoint p1 = splPath->GetSplinePoint(index, SplinePointPosition::FirstPoint);
    const VSplinePoint p2 = splPath->GetSplinePoint(index, SplinePointPosition::LastPoint);

    return p1.IsMovable() && p2.IsMovable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSplinePath::RefreshGeometry()
{
    // Very important to disable control points. Without it the pogram can't move the curve.
    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    if (isHovered || detailsMode)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }

    const auto splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    this->setPen(QPen(CorrectColor(splPath->GetColor()),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));

    for (qint32 i = 1; i<=splPath->CountSubSpl(); ++i)
    {
        const qint32 j = i*2;

        controlPoints[j-2]->blockSignals(true);
        controlPoints[j-1]->blockSignals(true);

        const auto spl = splPath->GetSpline(i);

        {
            const bool freeAngle1 = qmu::QmuTokenParser::IsSingle(spl.GetStartAngleFormula());
            const bool freeLength1 = qmu::QmuTokenParser::IsSingle(spl.GetC1LengthFormula());

            const auto splinePoint = spl.GetP1();
            controlPoints[j-2]->RefreshCtrlPoint(i, SplinePointPosition::FirstPoint, spl.GetP2(), splinePoint,
                                                 freeAngle1, freeLength1);
        }

        {
            const bool freeAngle2 = qmu::QmuTokenParser::IsSingle(spl.GetEndAngleFormula());
            const bool freeLength2 = qmu::QmuTokenParser::IsSingle(spl.GetC2LengthFormula());

            const auto splinePoint = spl.GetP4();
            controlPoints[j-1]->RefreshCtrlPoint(i, SplinePointPosition::LastPoint, spl.GetP3(), splinePoint,
                                                 freeAngle2, freeLength2);
        }

        controlPoints[j-2]->blockSignals(false);
        controlPoints[j-1]->blockSignals(false);
    }

    SetVisualization();

    foreach (auto *point, controlPoints)
    {
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}
