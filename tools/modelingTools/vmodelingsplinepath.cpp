/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vmodelingsplinepath.h"

const QString VModelingSplinePath::TagName = QStringLiteral("spline");
const QString VModelingSplinePath::ToolType = QStringLiteral("path");

VModelingSplinePath::VModelingSplinePath(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                 QGraphicsItem *parent)
    :VModelingTool(doc, data, id), QGraphicsPathItem(parent), dialogSplinePath(QSharedPointer<DialogSplinePath>()),
    controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;
    VSplinePath splPath = data->GetModelingSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    for (qint32 i = 1; i<=splPath.Count(); ++i)
    {
        VSpline spl = splPath.GetSpline(i);
        VControlPointSpline *controlPoint = new VControlPointSpline(i, SplinePoint::FirstPoint, spl.GetP2(),
                                                                     spl.GetPointP1().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSplinePath::ControlPointChangePosition);
        connect(this, &VModelingSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VModelingSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);

        controlPoint = new VControlPointSpline(i, SplinePoint::LastPoint, spl.GetP3(),
                                               spl.GetPointP4().toQPointF(), this);
        connect(controlPoint, &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSplinePath::ControlPointChangePosition);
        connect(this, &VModelingSplinePath::RefreshLine, controlPoint, &VControlPointSpline::RefreshLine);
        connect(this, &VModelingSplinePath::setEnabledPoint, controlPoint, &VControlPointSpline::setEnabledPoint);
        controlPoints.append(controlPoint);
    }
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingSplinePath::setDialog()
{
    Q_ASSERT(dialogSplinePath.isNull() == false);
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
    dialogSplinePath->SetPath(splPath);
}

VModelingSplinePath *VModelingSplinePath::Create(QSharedPointer<DialogSplinePath> &dialog, VDomDocument *doc,
                                                 VContainer *data)
{
    VSplinePath path = dialog->GetPath();
    for (qint32 i = 0; i < path.CountPoint(); ++i)
    {
        doc->IncrementReferens(path[i].P());
    }
    return Create(0, path, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingSplinePath * VModelingSplinePath::Create(const qint64 _id, const VSplinePath &path, VDomDocument *doc,
                                                  VContainer *data, const Document::Documents &parse,
                                                  const Tool::Sources &typeCreation)
{
    VModelingSplinePath *spl = 0;
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddModelingSplinePath(path);
    }
    else
    {
        data->UpdateModelingSplinePath(id, path);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLengthSpline(data->GetNameSplinePath(path), toMM(path.GetLength()));
    if (parse == Document::FullParse)
    {
        spl = new VModelingSplinePath(doc, data, id, typeCreation);
        doc->AddTool(id, spl);
    }
    return spl;
}

void VModelingSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VModelingSplinePath::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        VSplinePath splPath = dialogSplinePath->GetPath();
        for (qint32 i = 1; i<=splPath.Count(); ++i)
        {
            VSpline spl = splPath.GetSpline(i);
            qint32 j = i*2;
            disconnect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                    &VModelingSplinePath::ControlPointChangePosition);
            disconnect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                    &VModelingSplinePath::ControlPointChangePosition);
            controlPoints[j-2]->setPos(spl.GetP2());
            controlPoints[j-1]->setPos(spl.GetP3());
            connect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                    &VModelingSplinePath::ControlPointChangePosition);
            connect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                    &VModelingSplinePath::ControlPointChangePosition);

            spl = VSpline (VAbstractTool::data.DataModelingPoints(), spl.GetP1(),  controlPoints[j-2]->pos(),
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

void VModelingSplinePath::ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF pos)
{
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
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

void VModelingSplinePath::CorectControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline)
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

void VModelingSplinePath::UpdatePathPoint(QDomNode& node, VSplinePath &path)
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

void VModelingSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogSplinePath, this, event);
}

void VModelingSplinePath::AddToFile()
{
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrKCurve, splPath.getKCurve());

    for (qint32 i = 0; i < splPath.CountPoint(); ++i)
    {
        AddPathPoint(domElement, splPath[i]);
    }

    AddToModeling(domElement);
}

void VModelingSplinePath::AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint)
{
    QDomElement pathPoint = doc->createElement(AttrPathPoint);

    AddAttribute(pathPoint, AttrPSpline, splPoint.P());
    AddAttribute(pathPoint, AttrKAsm1, splPoint.KAsm1());
    AddAttribute(pathPoint, AttrKAsm2, splPoint.KAsm2());
    AddAttribute(pathPoint, AttrAngle, splPoint.Angle2());

    domElement.appendChild(pathPoint);
}

void VModelingSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingSplinePath::RemoveReferens()
{
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
    for (qint32 i = 0; i < splPath.Count(); ++i)
    {
        doc->DecrementReferens(splPath[i].P());
    }
}

void VModelingSplinePath::RefreshGeometry()
{
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
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
                &VModelingSplinePath::ControlPointChangePosition);
        disconnect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSplinePath::ControlPointChangePosition);
        controlPoints[j-2]->setPos(spl.GetP2());
        controlPoints[j-1]->setPos(spl.GetP3());
        connect(controlPoints[j-2], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSplinePath::ControlPointChangePosition);
        connect(controlPoints[j-1], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSplinePath::ControlPointChangePosition);
    }
}
