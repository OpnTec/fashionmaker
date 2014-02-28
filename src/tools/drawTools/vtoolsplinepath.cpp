/************************************************************************
 **
 **  @file   vtoolsplinepath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

const QString VToolSplinePath::TagName = QStringLiteral("spline");
const QString VToolSplinePath::ToolType = QStringLiteral("path");

VToolSplinePath::VToolSplinePath(VPattern *doc, VContainer *data, quint32 id, const Tool::Sources &typeCreation,
                                 QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;
    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(id);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, toPixel(widthHairLine)/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

    for (qint32 i = 1; i<=splPath->Count(); ++i)
    {
        VSpline spl = splPath->GetSpline(i);
        VControlPointSpline *controlPoint = new VControlPointSpline(i, SplinePoint::FirstPoint, spl.GetP2(),
                                                                     spl.GetP1().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);

        controlPoint = new VControlPointSpline(i, SplinePoint::LastPoint, spl.GetP3(), spl.GetP4().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);
    }
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolSplinePath::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    dialogTool->SetPath(*splPath);
}

void VToolSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    Q_CHECK_PTR(dialogTool);
    VSplinePath *path = new VSplinePath(dialogTool->GetPath());
    Q_CHECK_PTR(path);
    for (qint32 i = 0; i < path->CountPoint(); ++i)
    {
        doc->IncrementReferens((*path)[i].P().id());
    }
    Create(0, path, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolSplinePath::Create(const quint32 _id, VSplinePath *path, VMainGraphicsScene *scene,
                             VPattern *doc, VContainer *data, const Document::Documents &parse,
                             const Tool::Sources &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(path);
        data->AddLengthSpline(path->name(), toMM(path->GetLength()));
    }
    else
    {
        data->UpdateGObject(id, path);
        data->AddLengthSpline(path->name(), toMM(path->GetLength()));
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
        doc->AddTool(id, spl);
    }
}

void VToolSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolSplinePath::ControlPointChangePosition(const qint32 &indexSpline, const SplinePoint::Position &position,
                                                 const QPointF &pos)
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    VSpline spl = splPath.GetSpline(indexSpline);
    if (position == SplinePoint::FirstPoint)
    {
        spl = VSpline(spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    }
    else
    {
        spl = VSpline(spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
    }

    CorectControlPoints(spl, splPath, indexSpline);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.getKCurve()));
        UpdatePathPoint(domElement, splPath);
        emit FullUpdateTree();
        emit toolhaveChange();
    }
}

void VToolSplinePath::CorectControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline)
{
    VSplinePoint p = splPath.GetSplinePoint(indexSpline, SplinePoint::FirstPoint);
    p.SetAngle2(spl.GetAngle1());
    p.SetKAsm2(spl.GetKasm1());
    splPath.UpdatePoint(indexSpline, SplinePoint::FirstPoint, p);

    p = splPath.GetSplinePoint(indexSpline, SplinePoint::LastPoint);
    p.SetAngle2(spl.GetAngle2()-180);
    p.SetKAsm1(spl.GetKasm2());
    splPath.UpdatePoint(indexSpline, SplinePoint::LastPoint, p);
}

void VToolSplinePath::UpdatePathPoint(QDomNode& node, VSplinePath &path)
{
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for (qint32 i = 0; i < num; ++i)
    {
        QDomElement domElement = nodeList.at(i).toElement();
        if (domElement.isNull() == false)
        {
            VSplinePoint p = path[i];
            doc->SetAttribute(domElement, AttrPSpline, p.P().id());
            doc->SetAttribute(domElement, AttrKAsm1, p.KAsm1());
            doc->SetAttribute(domElement, AttrKAsm2, p.KAsm2());
            doc->SetAttribute(domElement, AttrAngle, p.Angle2());
        }
    }
}

void VToolSplinePath::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    emit setEnabledPoint(selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolSplinePath::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolSplinePath::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogSplinePath>(this, event);
}

void VToolSplinePath::AddToFile()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrKCurve, splPath.getKCurve());

    for (qint32 i = 0; i < splPath.CountPoint(); ++i)
    {
        AddPathPoint(domElement, splPath[i]);
    }

    AddToCalculation(domElement);
}

void VToolSplinePath::RefreshDataInFile()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
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
        CorectControlPoints(spl, splPath, i);
        CorectControlPoints(spl, splPath, i);

        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.getKCurve()));
            UpdatePathPoint(domElement, splPath);
        }

    }
}

void VToolSplinePath::AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint)
{
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    doc->SetAttribute(pathPoint, AttrPSpline, splPoint.P().id());
    doc->SetAttribute(pathPoint, AttrKAsm1, splPoint.KAsm1());
    doc->SetAttribute(pathPoint, AttrKAsm2, splPoint.KAsm2());
    doc->SetAttribute(pathPoint, AttrAngle, splPoint.Angle2());

    domElement.appendChild(pathPoint);
}

void VToolSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthMainLine)/factor));
}

void VToolSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
}

void VToolSplinePath::RemoveReferens()
{
    VSplinePath splPath = *VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    for (qint32 i = 0; i < splPath.Count(); ++i)
    {
        doc->DecrementReferens(splPath[i].P().id());
    }
}

QVariant VToolSplinePath::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void VToolSplinePath::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

void VToolSplinePath::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogSplinePath *dialogTool = qobject_cast<DialogSplinePath*>(dialog);
    Q_CHECK_PTR(dialogTool);

    VSplinePath splPath = dialogTool->GetPath();
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
        CorectControlPoints(spl, splPath, i);
        CorectControlPoints(spl, splPath, i);
    }

    doc->SetAttribute(domElement, AttrKCurve, QString().setNum(splPath.getKCurve()));
    UpdatePathPoint(domElement, splPath);
}

void VToolSplinePath::RefreshGeometry()
{
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
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
        emit RefreshLine(i, SplinePoint::FirstPoint, controlPoint, splinePoint);
        splinePoint = spl.GetP4().toQPointF();
        controlPoint = spl.GetP3();
        emit RefreshLine(i, SplinePoint::LastPoint, controlPoint, splinePoint);

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
