#include "vtoolspline.h"
#include "../geometry/vspline.h"
#include <QMenu>
#include <QDebug>

VToolSpline::VToolSpline(VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                         QGraphicsItem *parent):VAbstractTool(doc, data, id), QGraphicsPathItem(parent){

    VSpline spl = data->GetSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    controlPoint1 = new VControlPointSpline(spl.GetP2(), spl.GetPointP1(), this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPoint1ChangePosition);

    controlPoint2 = new VControlPointSpline(spl.GetP3(), spl.GetPointP4(), this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPoint2ChangePosition);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSpline::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolSpline::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        VSpline spl = VSpline (VAbstractTool::data->DataPoints(), dialogSpline->getP1(),
                               dialogSpline->getP4(), dialogSpline->getAngle1(), dialogSpline->getAngle2(),
                               dialogSpline->getKAsm1(), dialogSpline->getKAsm2(), dialogSpline->getKCurve());

        disconnect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPoint1ChangePosition);
        disconnect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPoint2ChangePosition);
        controlPoint1->setPos(spl.GetP2());
        controlPoint2->setPos(spl.GetP3());
        connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPoint1ChangePosition);
        connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPoint2ChangePosition);

        spl = VSpline (VAbstractTool::data->DataPoints(), dialogSpline->getP1(),  controlPoint1->pos(),
                       controlPoint2->pos(), dialogSpline->getP4(), dialogSpline->getKCurve());
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

void VToolSpline::ControlPoint1ChangePosition(const QPointF pos){
    VSpline spl = VAbstractTool::data->GetSpline(id);
    spl.ModifiSpl (spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
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

void VToolSpline::ControlPoint2ChangePosition(const QPointF pos){
    VSpline spl = VAbstractTool::data->GetSpline(id);
    spl.ModifiSpl (spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
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
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogSpline = QSharedPointer<DialogSpline>(new DialogSpline(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogSpline.data(), &DialogSpline::ChoosedObject);
            connect(dialogSpline.data(), &DialogSpline::DialogClosed, this,
                    &VToolSpline::FullUpdateFromGui);

            VSpline spl = VAbstractTool::data->GetSpline(id);

            dialogSpline->setP1(spl.GetP1());
            dialogSpline->setP4(spl.GetP4());
            dialogSpline->setAngle1(spl.GetAngle1());
            dialogSpline->setAngle2(spl.GetAngle2());
            dialogSpline->setKAsm1(spl.GetKasm1());
            dialogSpline->setKAsm2(spl.GetKasm2());
            dialogSpline->setKCurve(spl.GetKcurve());

            dialogSpline->show();
        }
    }
}

void VToolSpline::AddToFile(){
    VSpline spl = VAbstractTool::data->GetSpline(id);
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
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VToolSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}

void VToolSpline::RefreshGeometry(){
    VSpline spl = VAbstractTool::data->GetSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    QPointF splinePoint = VAbstractTool::data->GetPoint(spl.GetP1());
    QPointF controlPoint = spl.GetP2();
    controlPoint1->RefreshLine(controlPoint, splinePoint);
    splinePoint = VAbstractTool::data->GetPoint(spl.GetP4());
    controlPoint = spl.GetP3();
    controlPoint2->RefreshLine(controlPoint, splinePoint);
}


void VToolSpline::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        controlPoint1->setPen(QPen(Qt::black, widthHairLine));
        controlPoint1->setFlag(QGraphicsItem::ItemIsSelectable, true);
        controlPoint1->setFlag(QGraphicsItem::ItemIsMovable, true);
        controlPoint1->setAcceptHoverEvents(true);
        controlPoint2->setPen(QPen(Qt::black, widthHairLine));
        controlPoint2->setFlag(QGraphicsItem::ItemIsSelectable, true);
        controlPoint2->setFlag(QGraphicsItem::ItemIsMovable, true);
        controlPoint2->setAcceptHoverEvents(true);
        VAbstractTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setAcceptHoverEvents (false);
        controlPoint1->setPen(QPen(Qt::gray, widthHairLine));
        controlPoint1->setFlag(QGraphicsItem::ItemIsSelectable, false);
        controlPoint1->setFlag(QGraphicsItem::ItemIsMovable, false);
        controlPoint1->setAcceptHoverEvents(false);
        controlPoint2->setPen(QPen(Qt::gray, widthHairLine));
        controlPoint2->setFlag(QGraphicsItem::ItemIsSelectable, false);
        controlPoint2->setFlag(QGraphicsItem::ItemIsMovable, false);
        controlPoint2->setAcceptHoverEvents(false);
        VAbstractTool::ChangedActivDraw(newName);
    }
}
