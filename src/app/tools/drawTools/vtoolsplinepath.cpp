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
#include "../../dialogs/tools/dialogsplinepath.h"
#include "../../undocommands/movesplinepath.h"
#include "../../visualization/vistoolsplinepath.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../../../utils/vmath.h"
#else
#   include <QtMath>
#endif

const QString VToolSplinePath::ToolType = QStringLiteral("path");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolSplinePath::VToolSplinePath(VPattern *doc, VContainer *data, quint32 id, const QString &color,
                                 const Source &typeCreation, QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent), oldPosition()
{
    sceneType = SceneObject::SplinePath;
    lineColor = color;

    this->setPath(ToolPath());
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setAcceptHoverEvents(true);

    const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(id);
    for (qint32 i = 1; i<=splPath->Count(); ++i)
    {
        VSpline spl = splPath->GetSpline(i);
        VControlPointSpline *controlPoint = new VControlPointSpline(i, SplinePointPosition::FirstPoint, spl.GetP2(),
                                                                     spl.GetP1().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        connect(controlPoint, &VControlPointSpline::ShowContextMenu, this, &VToolSplinePath::contextMenuEvent);
        controlPoints.append(controlPoint);

        controlPoint = new VControlPointSpline(i, SplinePointPosition::LastPoint, spl.GetP3(), spl.GetP4().toQPointF(),
                                               this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        connect(controlPoint, &VControlPointSpline::ShowContextMenu, this, &VToolSplinePath::contextMenuEvent);
        controlPoints.append(controlPoint);
    }

    ShowHandles(false);

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
VToolSplinePath::~VToolSplinePath()
{
    //Disable cursor-arrow-openhand
    VApplication::restoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    dialogTool->SetPath(*splPath);
    dialogTool->SetColor(lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolSplinePath* VToolSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VSplinePath *path = new VSplinePath(dialogTool->GetPath());
    const QString color = dialogTool->GetColor();
    for (qint32 i = 0; i < path->CountPoint(); ++i)
    {
        doc->IncrementReferens((*path)[i].P().id());
    }
    VToolSplinePath* spl = nullptr;
    spl = Create(0, path, color, scene, doc, data, Document::FullParse, Source::FromGui);
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
                                         VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                         const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(path);
        data->AddCurveLength<VSplineLength>(id);
    }
    else
    {
        data->UpdateGObject(id, path);
        data->AddCurveLength<VSplineLength>(id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::SplinePath, doc);
    if (parse == Document::FullParse)
    {
        VToolSplinePath *spl = new VToolSplinePath(doc, data, id, color, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, spl, &VToolSplinePath::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, spl, &VToolSplinePath::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, spl, &VToolSplinePath::EnableToolMove);
        connect(scene, &VMainGraphicsScene::CurveDetailsMode, spl, &VToolSplinePath::DetailsMode);
        doc->AddTool(id, spl);
        return spl;
    }
    return nullptr;
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
    VSplinePath oldSplPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
    VSplinePath newSplPath = oldSplPath;
    VSpline spl = newSplPath.GetSpline(indexSpline);
    if (position == SplinePointPosition::FirstPoint)
    {
        spl = VSpline(spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    }
    else
    {
        spl = VSpline(spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
    }

    UpdateControlPoints(spl, newSplPath, indexSpline);

    MoveSplinePath *moveSplPath = new MoveSplinePath(doc, oldSplPath, newSplPath, id, this->scene());
    connect(moveSplPath, &VUndoCommand::NeedLiteParsing, doc, &VPattern::LiteParseTree);
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
    p.SetAngle2(spl.GetAngle1());
    p.SetKAsm2(spl.GetKasm1());
    splPath.UpdatePoint(indexSpline, SplinePointPosition::FirstPoint, p);

    p = splPath.GetSplinePoint(indexSpline, SplinePointPosition::LastPoint);
    p.SetAngle2(spl.GetAngle2()-180);
    p.SetKAsm1(spl.GetKasm2());
    splPath.UpdatePoint(indexSpline, SplinePointPosition::LastPoint, p);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::RefreshSplinePath(VSplinePath &splPath)
{
    for (qint32 i = 1; i<=splPath.Count(); ++i)
    {
        VSpline spl = splPath.GetSpline(i);
        qint32 j = i*2;

        controlPoints[j-2]->blockSignals(true);
        controlPoints[j-1]->blockSignals(true);

        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());

        controlPoints[j-2]->blockSignals(false);
        controlPoints[j-1]->blockSignals(false);

        spl = VSpline (spl.GetP1(),  controlPoints[j-2]->pos(), controlPoints[j-1]->pos(), spl.GetP4(),
                splPath.GetKCurve());
        UpdateControlPoints(spl, splPath, i);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePathPoint update spline path in pattern file.
 * @param doc dom document container.
 * @param node tag in file.
 * @param path spline path.
 */
void VToolSplinePath::UpdatePathPoint(VPattern *doc, QDomNode& node, const VSplinePath &path)
{
    SCASSERT(doc != nullptr);
    QDomElement element = node.toElement();
    if (element.isElement() == false)
    {
        qDebug()<<"Couldn't convert parent to element.";
        return;
    }

    doc->removeAllChilds(element);
    for (qint32 i = 0; i < path.CountPoint(); ++i)
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
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolSplinePath *visual = new VisToolSplinePath(getData(), this);
            VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
            SCASSERT(scene != nullptr)
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);

            QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
            visual->setPath(*splPath.data());
            visual->setMode(Mode::Show);
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolSplinePath *visual = qobject_cast<VisToolSplinePath *>(vis);
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

    if (detailsMode)
    {
        ShowHandles(detailsMode);
    }
    else
    {
        ShowHandles(show);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogSplinePath>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolSplinePath::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement() == false)
    {
        qDebug()<<"Can't find element with id="<<id<<"in pattern file";
        return;
    }
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
    RefreshSplinePath(splPath);
    doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.GetKCurve()));
    UpdatePathPoint(doc, domElement, splPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddPathPoint write path point to pattern file.
 * @param domElement dom element.
 * @param splPoint spline path point.
 */
void VToolSplinePath::AddPathPoint(VPattern *doc, QDomElement &domElement, const VSplinePoint &splPoint)
{
    SCASSERT(doc != nullptr);
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    doc->SetAttribute(pathPoint, AttrPSpline, splPoint.P().id());
    doc->SetAttribute(pathPoint, AttrKAsm1, splPoint.KAsm1());
    doc->SetAttribute(pathPoint, AttrKAsm2, splPoint.KAsm2());
    doc->SetAttribute(pathPoint, AttrAngle, splPoint.Angle2());

    domElement.appendChild(pathPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSplinePath::RemoveReferens()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
    for (qint32 i = 0; i < splPath.Count(); ++i)
    {
        doc->DecrementReferens(splPath.at(i).P().id());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSplinePath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);

    VSplinePath splPath = dialogTool->GetPath();
    RefreshSplinePath(splPath);
    doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.GetKCurve()));
    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
    UpdatePathPoint(doc, domElement, splPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VSplinePath> splPath = qSharedPointerDynamicCast<VSplinePath>(obj);
    SCASSERT(splPath.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrKCurve, splPath->GetKCurve());
    doc->SetAttribute(tag, AttrColor, lineColor);

    doc->RemoveAllChild(tag);
    for (qint32 i = 0; i < splPath->CountPoint(); ++i)
    {
        AddPathPoint(doc, tag, splPath->at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            VApplication::setOverrideCursor(cursorArrowCloseHand, 1, 1);
            oldPosition = event->scenePos();
            event->accept();
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
            //Disable cursor-arrow-closehand
            VApplication::restoreOverrideCursor(cursorArrowCloseHand);
        }
    }
    VAbstractSpline::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't need check if left mouse button was pressed. According to the Qt documentation "If you do receive this
    // event, you can be certain that this item also received a mouse press event, and that this item is the current
    // mouse grabber.".

    VSplinePath oldSplPath = *VAbstractTool::data.GeometricObject<VSplinePath>(id);
    VSplinePath newSplPath = oldSplPath;
    int indexSpline = oldSplPath.Segment(oldPosition);
    if (indexSpline == -1)
    {
        return;
    }

    VSpline spline = newSplPath.GetSpline(indexSpline);
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

    const VSpline spl = VSpline(spline.GetP1(), p2, p3, spline.GetP4(), spline.GetKcurve());

    UpdateControlPoints(spl, newSplPath, indexSpline);

    MoveSplinePath *moveSplPath = new MoveSplinePath(doc, oldSplPath, newSplPath, id, this->scene());
    connect(moveSplPath, &VUndoCommand::NeedLiteParsing, doc, &VPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveSplPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        VApplication::setOverrideCursor(cursorArrowOpenHand, 1, 1);
    }

    VAbstractSpline::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        //Disable cursor-arrow-openhand
        VApplication::restoreOverrideCursor(cursorArrowOpenHand);
    }

    VAbstractSpline::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSplinePath::RefreshGeometry()
{
    if (isHovered || detailsMode)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }
    this->setPen(QPen(CorrectColor(lineColor), qApp->toPixel(qApp->widthHairLine())/factor));
    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
    for (qint32 i = 1; i<=splPath->Count(); ++i)
    {
        VSpline spl = splPath->GetSpline(i);
        QPointF splinePoint = spl.GetP1().toQPointF();
        QPointF controlPoint = spl.GetP2();
        emit RefreshLine(i, SplinePointPosition::FirstPoint, controlPoint, splinePoint);
        splinePoint = spl.GetP4().toQPointF();
        controlPoint = spl.GetP3();
        emit RefreshLine(i, SplinePointPosition::LastPoint, controlPoint, splinePoint);

        qint32 j = i*2;

        controlPoints[j-2]->blockSignals(true);
        controlPoints[j-1]->blockSignals(true);

        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());

        controlPoints[j-2]->blockSignals(false);
        controlPoints[j-1]->blockSignals(false);
    }

    if (vis != nullptr)
    {
        VisToolSplinePath *visual = qobject_cast<VisToolSplinePath *>(vis);
        QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
        visual->setPath(*splPath.data());
        visual->setMode(Mode::Show);
        visual->RefreshGeometry();
    }
}
