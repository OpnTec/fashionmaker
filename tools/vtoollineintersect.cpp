#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoollineintersect.h"
#include <QMenu>
#pragma GCC diagnostic pop

VToolLineIntersect::VToolLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id, Draw::Mode mode,
                                       const qint64 &p1Line1, const qint64 &p2Line1, const qint64 &p1Line2,
                                       const qint64 &p2Line2, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolPoint(doc, data, id, mode, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2), dialogLineIntersect(QSharedPointer<DialogLineIntersect>()){
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolLineIntersect::setDialog(){
    Q_ASSERT(!dialogLineIntersect.isNull());
    if(!dialogLineIntersect.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogLineIntersect->setP1Line1(p1Line1);
        dialogLineIntersect->setP2Line1(p2Line1);
        dialogLineIntersect->setP1Line2(p1Line2);
        dialogLineIntersect->setP2Line2(p2Line2);
        dialogLineIntersect->setPointName(p.name());
    }
}

void VToolLineIntersect::Create(QSharedPointer<DialogLineIntersect> &dialog, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data, Draw::Mode mode){
    qint64 p1Line1Id = dialog->getP1Line1();
    qint64 p2Line1Id = dialog->getP2Line1();
    qint64 p1Line2Id = dialog->getP1Line2();
    qint64 p2Line2Id = dialog->getP2Line2();
    QString pointName = dialog->getPointName();
    Create(0, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui, mode);
}

void VToolLineIntersect::Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id,
                                const qint64 &p1Line2Id, const qint64 &p2Line2Id, const QString &pointName,
                                const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                                VContainer *data, Document::Enum parse, Tool::Enum typeCreation, Draw::Mode mode){
    VPointF p1Line1 = data->GetPoint(p1Line1Id);
    VPointF p2Line1 = data->GetPoint(p2Line1Id);
    VPointF p1Line2 = data->GetPoint(p1Line2Id);
    VPointF p2Line2 = data->GetPoint(p2Line2Id);

    QLineF line1(p1Line1.toQPointF(), p2Line1.toQPointF());
    QLineF line2(p1Line2.toQPointF(), p2Line2.toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if(intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection){
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    tools->insert(id, tool);
                    data->IncrementReferens(id, Scene::Point);
                }
            }
        }
        data->AddLine(p1Line1Id, id);
        data->AddLine(id, p2Line1Id);
        data->AddLine(p1Line2Id, id);
        data->AddLine(id, p2Line2Id);
        if(mode == Draw::Modeling){
            data->IncrementReferens(p1Line1Id, Scene::Point);
            data->IncrementReferens(p2Line1Id, Scene::Point);
            data->IncrementReferens(p1Line2Id, Scene::Point);
            data->IncrementReferens(p2Line2Id, Scene::Point);
        }
        VAbstractTool::AddRecord(id, Tools::LineIntersectTool, doc);
        if(parse == Document::FullParse){
            VToolLineIntersect *point = new VToolLineIntersect(doc, data, id, mode, p1Line1Id,
                                                               p2Line1Id, p1Line2Id,
                                                               p2Line2Id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolLineIntersect::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
}

void VToolLineIntersect::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        p1Line1 = domElement.attribute("p1Line1", "").toLongLong();
        p2Line1 = domElement.attribute("p2Line1", "").toLongLong();
        p1Line2 = domElement.attribute("p1Line2", "").toLongLong();
        p2Line2 = domElement.attribute("p2Line2", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolLineIntersect::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogLineIntersect->getPointName());
            domElement.setAttribute("p1Line1", QString().setNum(dialogLineIntersect->getP1Line1()));
            domElement.setAttribute("p2Line1", QString().setNum(dialogLineIntersect->getP2Line1()));
            domElement.setAttribute("p1Line2", QString().setNum(dialogLineIntersect->getP1Line2()));
            domElement.setAttribute("p2Line2", QString().setNum(dialogLineIntersect->getP2Line2()));
            emit FullUpdateTree();
        }
    }
    dialogLineIntersect.clear();
}

void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogLineIntersect, this, event);
}

void VToolLineIntersect::AddToFile(){
    VPointF point;
    if(mode == Draw::Calculation){
        point = VAbstractTool::data.GetPoint(id);
    } else {
        point = VAbstractTool::data.GetModelingPoint(id);
    }
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "lineIntersect");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "p1Line1", p1Line1);
    AddAttribute(domElement, "p2Line1", p2Line1);
    AddAttribute(domElement, "p1Line2", p1Line2);
    AddAttribute(domElement, "p2Line2", p2Line2);

    AddToDraw(domElement);
}
