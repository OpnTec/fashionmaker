/************************************************************************
 **
 **  @file vtoolsplinepath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

const QString VToolSplinePath::TagName = QStringLiteral("spline");
const QString VToolSplinePath::ToolType = QStringLiteral("path");

VToolSplinePath::VToolSplinePath(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                 QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), dialogSplinePath(QSharedPointer<DialogSplinePath>()),
    controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;
    VSplinePath splPath = data->GetSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    for (qint32 i = 1; i<=splPath.Count(); ++i)
    {
        VSpline spl = splPath.GetSpline(i);
        VControlPointSpline *controlPoint = new VControlPointSpline(i, SplinePoint::FirstPoint, spl.GetP2(),
                                                                     spl.GetPointP1().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSplinePath::ControlPointChangePosition);
        connect(this, &VToolSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VToolSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);

        controlPoint = new VControlPointSpline(i, SplinePoint::LastPoint, spl.GetP3(),
                                               spl.GetPointP4().toQPointF(), this);
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
}

void VToolSplinePath::setDialog()
{
    Q_ASSERT(dialogSplinePath.isNull() == false);
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    dialogSplinePath->SetPath(splPath);
}

void VToolSplinePath::Create(QSharedPointer<DialogSplinePath> &dialog, VMainGraphicsScene *scene,
                             VDomDocument *doc, VContainer *data)
{
    VSplinePath path = dialog->GetPath();
    for (qint32 i = 0; i < path.CountPoint(); ++i)
    {
        doc->IncrementReferens(path[i].P());
    }
    Create(0, path, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolSplinePath::Create(const qint64 _id, const VSplinePath &path, VMainGraphicsScene *scene,
                             VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                             const Tool::Sources &typeCreation)
{
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddSplinePath(path);
        data->AddLengthSpline(path.name(), toMM(path.GetLength()));
    }
    else
    {
        data->UpdateSplinePath(id, path);
        data->AddLengthSpline(path.name(), toMM(path.GetLength()));
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
        connect(spl, &VToolSplinePath::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        connect(scene, &VMainGraphicsScene::NewFactor, spl, &VToolSplinePath::SetFactor);
        doc->AddTool(id, spl);
    }
}

void VToolSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolSplinePath::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        VSplinePath splPath = dialogSplinePath->GetPath();
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

            spl = VSpline (VAbstractTool::data.DataPoints(), spl.GetP1(),  controlPoints[j-2]->pos(),
                           controlPoints[j-1]->pos(), spl.GetP4(), splPath.getKCurve());
            CorectControlPoints(spl, splPath, i);
            CorectControlPoints(spl, splPath, i);

            QDomElement domElement = doc->elementById(QString().setNum(id));
            if (domElement.isElement())
            {
                domElement.setAttribute(AttrKCurve, QString().setNum(splPath.getKCurve()));
                UpdatePathPoint(domElement, splPath);
                emit FullUpdateTree();
            }

        }
    }
    dialogSplinePath.clear();
}

void VToolSplinePath::ControlPointChangePosition(const qint32 &indexSpline, const SplinePoint::Position &position,
                                                 const QPointF &pos)
{
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    VSpline spl = splPath.GetSpline(indexSpline);
    if (position == SplinePoint::FirstPoint)
    {
        spl.ModifiSpl (spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    }
    else
    {
        spl.ModifiSpl (spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
    }

    CorectControlPoints(spl, splPath, indexSpline);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrKCurve, QString().setNum(splPath.getKCurve()));
        UpdatePathPoint(domElement, splPath);
        emit FullUpdateTree();
    }
}

void VToolSplinePath::CorectControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline)
{
    VSplinePoint p = splPath.GetSplinePoint(indexSpline, SplinePoint::FirstPoint);
    p.SetAngle(spl.GetAngle1());
    p.SetKAsm2(spl.GetKasm1());
    splPath.UpdatePoint(indexSpline, SplinePoint::FirstPoint, p);

    p = splPath.GetSplinePoint(indexSpline, SplinePoint::LastPoint);
    p.SetAngle(spl.GetAngle2()-180);
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
            domElement.setAttribute(AttrPSpline, QString().setNum(p.P()));
            domElement.setAttribute(AttrKAsm1, QString().setNum(p.KAsm1()));
            domElement.setAttribute(AttrKAsm2, QString().setNum(p.KAsm2()));
            domElement.setAttribute(AttrAngle, QString().setNum(p.Angle2()));
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
    this->setPen(QPen(currentColor, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    emit setEnabledPoint(selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolSplinePath::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
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
    ContextMenu(dialogSplinePath, this, event);
}

void VToolSplinePath::AddToFile()
{
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrKCurve, splPath.getKCurve());

    for (qint32 i = 0; i < splPath.CountPoint(); ++i)
    {
        AddPathPoint(domElement, splPath[i]);
    }

    AddToCalculation(domElement);
}

void VToolSplinePath::AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint)
{
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    AddAttribute(pathPoint, AttrPSpline, splPoint.P());
    AddAttribute(pathPoint, AttrKAsm1, splPoint.KAsm1());
    AddAttribute(pathPoint, AttrKAsm2, splPoint.KAsm2());
    AddAttribute(pathPoint, AttrAngle, splPoint.Angle2());

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
    this->setPen(QPen(currentColor, widthMainLine/factor));
}

void VToolSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine/factor));
}

void VToolSplinePath::RemoveReferens()
{
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    for (qint32 i = 0; i < splPath.Count(); ++i)
    {
        doc->DecrementReferens(splPath[i].P());
    }
}

void VToolSplinePath::RefreshGeometry()
{
    this->setPen(QPen(currentColor, widthHairLine/factor));
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    for (qint32 i = 1; i<=splPath.Count(); ++i)
    {
        VSpline spl = splPath.GetSpline(i);
        QPointF splinePoint = spl.GetPointP1().toQPointF();
        QPointF controlPoint = spl.GetP2();
        emit RefreshLine(i, SplinePoint::FirstPoint, controlPoint, splinePoint);
        splinePoint = spl.GetPointP4().toQPointF();
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
