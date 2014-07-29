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

#include "vtoolsplinepath.h"
#include "../../dialogs/tools/dialogsplinepath.h"
#include "../../undocommands/movesplinepath.h"

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
VToolSplinePath::VToolSplinePath(VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                                 QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent)
{
    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(id);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

    for (qint32 i = 1; i<=splPath->Count(); ++i)
    {
        VSpline spl = splPath->GetSpline(i);
        VControlPointSpline *controlPoint = new VControlPointSpline(i, SplinePointPosition::FirstPoint, spl.GetP2(),
                                                                     spl.GetP1().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);

        controlPoint = new VControlPointSpline(i, SplinePointPosition::LastPoint, spl.GetP3(), spl.GetP4().toQPointF(),
                                               this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);
    }
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
void VToolSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    dialogTool->SetPath(*splPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
void VToolSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VSplinePath *path = new VSplinePath(dialogTool->GetPath());
    for (qint32 i = 0; i < path->CountPoint(); ++i)
    {
        doc->IncrementReferens((*path)[i].P().id());
    }
    Create(0, path, scene, doc, data, Document::FullParse, Source::FromGui);
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
void VToolSplinePath::Create(const quint32 _id, VSplinePath *path, VMainGraphicsScene *scene, VPattern *doc,
                             VContainer *data, const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(path);
        data->AddVariable(path->name(), new VLengthSpline(id, path));
    }
    else
    {
        data->UpdateGObject(id, path);
        data->AddVariable(path->name(), new VLengthSpline(id, path));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::SplinePathTool, doc);
    if (parse == Document::FullParse)
    {
        VToolSplinePath *spl = new VToolSplinePath(doc, data, id, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, spl, &VToolSplinePath::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, spl, &VToolSplinePath::Disable);
        doc->AddTool(id, spl);
    }
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
    VSplinePath oldSplPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
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
        disconnect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        disconnect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());
        connect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);

        spl = VSpline (spl.GetP1(),  controlPoints[j-2]->pos(), controlPoints[j-1]->pos(), spl.GetP4(),
                splPath.getKCurve());
        UpdateControlPoints(spl, splPath, i);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePathPoint update spline path in pattern file.
 * @brief doc dom document container.
 * @param node tag in file.
 * @param path spline path.
 */
void VToolSplinePath::UpdatePathPoint(VPattern *doc, QDomNode& node, const VSplinePath &path)
{
    SCASSERT(doc != nullptr)
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for (qint32 i = 0; i < num; ++i)
    {
        QDomElement domElement = nodeList.at(i).toElement();
        if (domElement.isNull() == false)
        {
            VSplinePoint p = path.at(i);
            doc->SetAttribute(domElement, AttrPSpline, p.P().id());
            doc->SetAttribute(domElement, AttrKAsm1, p.KAsm1());
            doc->SetAttribute(domElement, AttrKAsm2, p.KAsm2());
            doc->SetAttribute(domElement, AttrAngle, p.Angle2());
        }
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
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolSplinePath::AddToFile()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrKCurve, splPath.getKCurve());

    for (qint32 i = 0; i < splPath.CountPoint(); ++i)
    {
        AddPathPoint(domElement, splPath.at(i));
    }

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolSplinePath::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement() == false)
    {
        qDebug()<<"Can't find element with id="<<id<<"in pattern file";
        return;
    }
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    RefreshSplinePath(splPath);
    doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.getKCurve()));
    UpdatePathPoint(doc, domElement, splPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddPathPoint write path point to pattern file.
 * @param domElement dom element.
 * @param splPoint spline path point.
 */
void VToolSplinePath::AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint)
{
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    doc->SetAttribute(pathPoint, AttrPSpline, splPoint.P().id());
    doc->SetAttribute(pathPoint, AttrKAsm1, splPoint.KAsm1());
    doc->SetAttribute(pathPoint, AttrKAsm2, splPoint.KAsm2());
    doc->SetAttribute(pathPoint, AttrAngle, splPoint.Angle2());

    domElement.appendChild(pathPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent  handle mouse release events.
 * @param event mouse release event.
 */
void VToolSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolSplinePath::RemoveReferens()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
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
    doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.getKCurve()));
    UpdatePathPoint(doc, domElement, splPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolSplinePath::RefreshGeometry()
{
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
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
        disconnect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        disconnect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());
        connect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
    }
}
