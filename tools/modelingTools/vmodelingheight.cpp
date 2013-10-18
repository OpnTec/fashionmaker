#include "vmodelingheight.h"
#include "../drawTools/vtoolheight.h"

VModelingHeight::VModelingHeight(VDomDocument *doc, VContainer *data, const qint64 &id,
                                 const QString &typeLine, const qint64 &basePointId, const qint64 &p1LineId,
                                 const qint64 &p2LineId, Tool::Sources typeCreation,
                                 QGraphicsItem * parent)
    :VModelingLinePoint(doc, data, id, typeLine, QString(), basePointId, 0, parent),
      dialogHeight(QSharedPointer<DialogHeight>()), p1LineId(p1LineId), p2LineId(p2LineId){
    ignoreFullUpdate = true;
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingHeight::setDialog(){
    Q_ASSERT(!dialogHeight.isNull());
    VPointF p = VAbstractTool::data.GetModelingPoint(id);
    dialogHeight->setTypeLine(typeLine);
    dialogHeight->setBasePointId(basePointId, id);
    dialogHeight->setP1LineId(p1LineId, id);
    dialogHeight->setP2LineId(p2LineId, id);
    dialogHeight->setPointName(p.name());
}

VModelingHeight *VModelingHeight::Create(QSharedPointer<DialogHeight> &dialog, VDomDocument *doc,
                                         VContainer *data){
    disconnect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &DialogHeight::UpdateList);
    QString pointName = dialog->getPointName();
    QString typeLine = dialog->getTypeLine();
    qint64 basePointId = dialog->getBasePointId();
    qint64 p1LineId = dialog->getP1LineId();
    qint64 p2LineId = dialog->getP2LineId();
    return Create(0, pointName, typeLine, basePointId, p1LineId, p2LineId, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingHeight *VModelingHeight::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                         const qint64 &basePointId, const qint64 &p1LineId,
                                         const qint64 &p2LineId, const qreal &mx, const qreal &my,
                                         VDomDocument *doc, VContainer *data,
                                         const Document::Documents &parse, Tool::Sources typeCreation){
    VModelingHeight *point = 0;
    VPointF basePoint = data->GetModelingPoint(basePointId);
    VPointF p1Line = data->GetModelingPoint(p1LineId);
    VPointF p2Line = data->GetModelingPoint(p2LineId);

    QPointF pHeight = VToolHeight::FindPoint(QLineF(p1Line.toQPointF(), p2Line.toQPointF()),
                                             basePoint.toQPointF());
    QLineF line = QLineF(basePoint.toQPointF(), pHeight);
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddModelingPoint(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
    } else {
        data->UpdateModelingPoint(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        if(parse != Document::FullParse){
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLine(basePointId, id, Draw::Modeling);
    if(parse == Document::FullParse){
        point = new VModelingHeight(doc, data, id, typeLine, basePointId, p1LineId, p2LineId, typeCreation);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(p1LineId);
        doc->IncrementReferens(p2LineId);
    }
    return point;
}

void VModelingHeight::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        basePointId = domElement.attribute("basePoint", "").toLongLong();
        p1LineId = domElement.attribute("p1Line", "").toLongLong();
        p2LineId = domElement.attribute("p2Line", "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingHeight::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogHeight->getPointName());
            domElement.setAttribute("typeLine", dialogHeight->getTypeLine());
            domElement.setAttribute("basePoint", QString().setNum(dialogHeight->getBasePointId()));
            domElement.setAttribute("p1Line", QString().setNum(dialogHeight->getP1LineId()));
            domElement.setAttribute("p2Line", QString().setNum(dialogHeight->getP2LineId()));
            emit FullUpdateTree();
        }
    }
    dialogHeight.clear();
}

void VModelingHeight::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogHeight, this, event);
}

void VModelingHeight::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "endLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "basePoint", basePointId);
    AddAttribute(domElement, "p1Line", p1LineId);
    AddAttribute(domElement, "p2Line", p2LineId);

    AddToModeling(domElement);
}
