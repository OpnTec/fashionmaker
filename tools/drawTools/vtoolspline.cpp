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

#include "vtoolspline.h"
#include <QMenu>
#include <QDebug>
#include "geometry/vspline.h"


VToolSpline::VToolSpline(VDomDocument *doc, VContainer *data, qint64 id,
                         Tool::Enum typeCreation,
                         QGraphicsItem *parent):VDrawTool(doc, data, id), QGraphicsPathItem(parent),
    dialogSpline(QSharedPointer<DialogSpline>()), controlPoints(QVector<VControlPointSpline *>()){

    VSpline spl = data->GetSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    VControlPointSpline *controlPoint1 = new VControlPointSpline(1, SplinePoint::FirstPoint, spl.GetP2(),
                                                                 spl.GetPointP1().toQPointF(), this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint1, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint1, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint1);

    VControlPointSpline *controlPoint2 = new VControlPointSpline(1, SplinePoint::LastPoint, spl.GetP3(),
                                                                 spl.GetPointP4().toQPointF(), this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint2, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint2, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint2);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSpline::setDialog(){
    Q_ASSERT(!dialogSpline.isNull());
    if(!dialogSpline.isNull()){
        VSpline spl = VAbstractTool::data.GetSpline(id);
        dialogSpline->setP1(spl.GetP1());
        dialogSpline->setP4(spl.GetP4());
        dialogSpline->setAngle1(spl.GetAngle1());
        dialogSpline->setAngle2(spl.GetAngle2());
        dialogSpline->setKAsm1(spl.GetKasm1());
        dialogSpline->setKAsm2(spl.GetKasm2());
        dialogSpline->setKCurve(spl.GetKcurve());
    }
}

void VToolSpline::Create(QSharedPointer<DialogSpline> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data){
    qint64 p1 = dialog->getP1();
    qint64 p4 = dialog->getP4();
    qreal kAsm1 = dialog->getKAsm1();
    qreal kAsm2 = dialog->getKAsm2();
    qreal angle1 = dialog->getAngle1();
    qreal angle2 = dialog->getAngle2();
    qreal kCurve = dialog->getKCurve();
    Create(0, p1, p4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolSpline::Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                         const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                         VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                         Tool::Enum typeCreation){
    VSpline spline = VSpline(data->DataPoints(), p1, p4, angle1, angle2, kAsm1, kAsm2, kCurve);
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddSpline(spline);
    } else {
        data->UpdateSpline(id, spline);
        if(parse != Document::FullParse){
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VDataTool *tool = tools->value(id);
            if(tool != 0){
                tool->VDataTool::setData(data);
                data->IncrementReferens(id, Scene::Spline);
            }
        }
    }
    data->AddLengthSpline(data->GetNameSpline(p1, p4), spline.GetLength());
    VDrawTool::AddRecord(id, Tool::SplineTool, doc);
    if(parse == Document::FullParse){
        VToolSpline *spl = new VToolSpline(doc, data, id, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(spl, &VToolSpline::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id,spl);
    }
}

void VToolSpline::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolSpline::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        VSpline spl = VSpline (VAbstractTool::data.DataPoints(), dialogSpline->getP1(),
                               dialogSpline->getP4(), dialogSpline->getAngle1(), dialogSpline->getAngle2(),
                               dialogSpline->getKAsm1(), dialogSpline->getKAsm2(), dialogSpline->getKCurve());

        disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        controlPoints[0]->setPos(spl.GetP2());
        controlPoints[1]->setPos(spl.GetP3());
        connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);

        spl = VSpline (VAbstractTool::data.DataPoints(), dialogSpline->getP1(),  controlPoints[0]->pos(),
                       controlPoints[1]->pos(), dialogSpline->getP4(), dialogSpline->getKCurve());
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("point1", QString().setNum(spl.GetP1()));
            domElement.setAttribute("point4", QString().setNum(spl.GetP4()));
            domElement.setAttribute("angle1", QString().setNum(spl.GetAngle1()));
            domElement.setAttribute("angle2", QString().setNum(spl.GetAngle2()));
            domElement.setAttribute("kAsm1", QString().setNum(spl.GetKasm1()));
            domElement.setAttribute("kAsm2", QString().setNum(spl.GetKasm2()));
            domElement.setAttribute("kCurve", QString().setNum(spl.GetKcurve()));
            emit FullUpdateTree();
        }
    }
    dialogSpline.clear();
}

void VToolSpline::ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                              const QPointF pos){
    Q_UNUSED(indexSpline);
    VSpline spl = VAbstractTool::data.GetSpline(id);
    if(position == SplinePoint::FirstPoint){
        spl.ModifiSpl (spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    } else {
        spl.ModifiSpl (spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
    }
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("angle1", QString().setNum(spl.GetAngle1()));
        domElement.setAttribute("angle2", QString().setNum(spl.GetAngle2()));
        domElement.setAttribute("kAsm1", QString().setNum(spl.GetKasm1()));
        domElement.setAttribute("kAsm2", QString().setNum(spl.GetKasm2()));
        domElement.setAttribute("kCurve", QString().setNum(spl.GetKcurve()));
        emit FullUpdateTree();
    }
}

void VToolSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    VSpline spl = VDrawTool::data.GetSpline(id);
    if(spl.referens() > 1){
        ContextMenu(dialogSpline, this, event, false);
    } else {
        ContextMenu(dialogSpline, this, event);
    }
}

void VToolSpline::AddToFile(){
    VSpline spl = VAbstractTool::data.GetSpline(id);
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "simple");
    AddAttribute(domElement, "point1", spl.GetP1());
    AddAttribute(domElement, "point4", spl.GetP4());
    AddAttribute(domElement, "angle1", spl.GetAngle1());
    AddAttribute(domElement, "angle2", spl.GetAngle2());
    AddAttribute(domElement, "kAsm1", spl.GetKasm1());
    AddAttribute(domElement, "kAsm2", spl.GetKasm2());
    AddAttribute(domElement, "kCurve", spl.GetKcurve());

    AddToCalculation(domElement);
}

void VToolSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VToolSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VToolSpline::RefreshGeometry(){
    VSpline spl = VAbstractTool::data.GetSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    QPointF splinePoint = VAbstractTool::data.GetPoint(spl.GetP1()).toQPointF();
    QPointF controlPoint = spl.GetP2();
    emit RefreshLine(1, SplinePoint::FirstPoint, controlPoint, splinePoint);
    splinePoint = VAbstractTool::data.GetPoint(spl.GetP4()).toQPointF();
    controlPoint = spl.GetP3();
    emit RefreshLine(1, SplinePoint::LastPoint, controlPoint, splinePoint);

    disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    controlPoints[0]->setPos(spl.GetP2());
    controlPoints[1]->setPos(spl.GetP3());
    connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
}


void VToolSpline::ChangedActivDraw(const QString newName){
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

void VToolSpline::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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
