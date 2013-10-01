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
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vtoolsplinepath.h"
#include <QMenu>

VToolSplinePath::VToolSplinePath(VDomDocument *doc, VContainer *data, qint64 id,
                                 Tool::Sources typeCreation,
                                 QGraphicsItem *parent):VDrawTool(doc, data, id),
    QGraphicsPathItem(parent), dialogSplinePath(QSharedPointer<DialogSplinePath>()),
    controlPoints(QVector<VControlPointSpline *>()){
    VSplinePath splPath = data->GetSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    for(qint32 i = 1; i<=splPath.Count(); ++i){
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
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSplinePath::setDialog(){
    Q_ASSERT(!dialogSplinePath.isNull());
    if(!dialogSplinePath.isNull()){
        VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
        dialogSplinePath->SetPath(splPath);
    }
}

void VToolSplinePath::Create(QSharedPointer<DialogSplinePath> &dialog, VMainGraphicsScene *scene,
                             VDomDocument *doc, VContainer *data){
    VSplinePath path = dialog->GetPath();
    for(qint32 i = 0; i < path.CountPoint(); ++i){
        doc->IncrementReferens(path[i].P());
    }
    Create(0, path, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolSplinePath::Create(const qint64 _id, const VSplinePath &path, VMainGraphicsScene *scene,
                             VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                             Tool::Sources typeCreation){
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddSplinePath(path);
    } else {
        data->UpdateSplinePath(id, path);
        if(parse != Document::FullParse){
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLengthSpline(data->GetNameSplinePath(path), path.GetLength());
    VDrawTool::AddRecord(id, Tool::SplinePathTool, doc);
    if(parse == Document::FullParse){
        VToolSplinePath *spl = new VToolSplinePath(doc, data, id, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(spl, &VToolSplinePath::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        doc->AddTool(id, spl);
    }
}

void VToolSplinePath::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolSplinePath::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        VSplinePath splPath = dialogSplinePath->GetPath();
        for(qint32 i = 1; i<=splPath.Count(); ++i){
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
            if(domElement.isElement()){
                domElement.setAttribute("kCurve", QString().setNum(splPath.getKCurve()));
                UpdatePathPoint(domElement, splPath);
                emit FullUpdateTree();
            }

        }
    }
    dialogSplinePath.clear();
}

void VToolSplinePath::ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF pos){
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    VSpline spl = splPath.GetSpline(indexSpline);
    if(position == SplinePoint::FirstPoint){
        spl.ModifiSpl (spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    } else {
        spl.ModifiSpl (spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
    }

    CorectControlPoints(spl, splPath, indexSpline);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("kCurve", QString().setNum(splPath.getKCurve()));
        UpdatePathPoint(domElement, splPath);
        emit FullUpdateTree();
    }
}

void VToolSplinePath::CorectControlPoints(const VSpline &spl, VSplinePath &splPath,
                                          const qint32 &indexSpline){
    VSplinePoint p = splPath.GetSplinePoint(indexSpline, SplinePoint::FirstPoint);
    p.SetAngle(spl.GetAngle1());
    p.SetKAsm2(spl.GetKasm1());
    splPath.UpdatePoint(indexSpline, SplinePoint::FirstPoint, p);

    p = splPath.GetSplinePoint(indexSpline, SplinePoint::LastPoint);
    p.SetAngle(spl.GetAngle2()-180);
    p.SetKAsm1(spl.GetKasm2());
    splPath.UpdatePoint(indexSpline, SplinePoint::LastPoint, p);
}

void VToolSplinePath::UpdatePathPoint(QDomNode& node, VSplinePath &path){
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for(qint32 i = 0; i < num; ++i){
        QDomElement domElement = nodeList.at(i).toElement();
        if(!domElement.isNull()){
            VSplinePoint p = path[i];
            domElement.setAttribute("pSpline", QString().setNum(p.P()));
            domElement.setAttribute("kAsm1", QString().setNum(p.KAsm1()));
            domElement.setAttribute("kAsm2", QString().setNum(p.KAsm2()));
            domElement.setAttribute("angle", QString().setNum(p.Angle2()));
        }
    }
}

void VToolSplinePath::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        emit setEnabledPoint(true);
        VDrawTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setAcceptHoverEvents (false);
        emit setEnabledPoint(false);
        VDrawTool::ChangedActivDraw(newName);
    }
}

void VToolSplinePath::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
    if(id == this->id){
        if(enable == false){
            this->setPen(QPen(baseColor, widthHairLine));
            currentColor = baseColor;
        } else {
            this->setPen(QPen(color, widthHairLine));
            currentColor = color;
        }
    }
}

void VToolSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogSplinePath, this, event);
}

void VToolSplinePath::AddToFile(){
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "path");
    AddAttribute(domElement, "kCurve", splPath.getKCurve());

    for(qint32 i = 0; i < splPath.CountPoint(); ++i){
        AddPathPoint(domElement, splPath[i]);
    }

    AddToCalculation(domElement);
}

void VToolSplinePath::AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint){
    QDomElement pathPoint = doc->createElement("pathPoint");

    AddAttribute(pathPoint, "pSpline", splPoint.P());
    AddAttribute(pathPoint, "kAsm1", splPoint.KAsm1());
    AddAttribute(pathPoint, "kAsm2", splPoint.KAsm2());
    AddAttribute(pathPoint, "angle", splPoint.Angle2());

    domElement.appendChild(pathPoint);
}

void VToolSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VToolSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VToolSplinePath::RemoveReferens(){
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    for(qint32 i = 0; i < splPath.Count(); ++i){
        doc->DecrementReferens(splPath[i].P());
    }
}

void VToolSplinePath::RefreshGeometry(){
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    for(qint32 i = 1; i<=splPath.Count(); ++i){
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
