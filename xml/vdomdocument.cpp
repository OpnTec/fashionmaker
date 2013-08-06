#include "vdomdocument.h"
#include <QDebug>

#include "../tools/vtoolsinglepoint.h"
#include "../tools/vtoolendline.h"
#include "../tools/vtoolline.h"
#include "../tools/vtoolalongline.h"
#include "../tools/vtoolshoulderpoint.h"
#include "../tools/vtoolnormal.h"
#include "../tools/vtoolbisector.h"
#include "../tools/vtoollineintersect.h"
#include "../tools/vtoolspline.h"
#include "../tools/vtoolarc.h"
#include "../options.h"
#include "../container/calculator.h"


VDomDocument::VDomDocument(VContainer *data) : QDomDocument() {
    this->data = data;
}

VDomDocument::VDomDocument(const QString& name, VContainer *data) : QDomDocument(name) {
    this->data = data;
}

VDomDocument::VDomDocument(const QDomDocumentType& doctype, VContainer *data) : QDomDocument(doctype){
    this->data = data;
}

QDomElement VDomDocument::elementById(const QString& id){
    if (map.contains(id)) {
       QDomElement e = map[id];
       if (e.parentNode().nodeType() != QDomNode::BaseNode) {
           return e;
       }

       map.remove(id);
    }

    bool res = this->find(this->documentElement(), id);
    if (res) {
       return map[id];
    }

    return QDomElement();
}

bool VDomDocument::find(QDomElement node, const QString& id){
    if (node.hasAttribute("id")) {
        QString value = node.attribute("id");
        this->map[value] = node;
        if (value == id) {
            return true;
        }
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i) {
        QDomNode n = node.childNodes().at(i);
        if (n.isElement()) {
            bool res = this->find(n.toElement(), id);
            if (res) {
                return true;
            }
        }
    }

    return false;
}

void VDomDocument::CreateEmptyFile(){
    QDomElement domElement = this->createElement("lekalo");
    this->appendChild(domElement);
    QDomNode xmlNode = this->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    this->insertBefore(xmlNode, this->firstChild());
    QDomElement incrElement = this->createElement("increments");
    domElement.appendChild(incrElement);
}

bool VDomDocument::CheckNameDraw(const QString& name) const{
    QDomNodeList elements = this->documentElement().elementsByTagName( "draw" );
    if(elements.size() == 0){
        return false;
    }
    for ( qint32 i = 0; i < elements.count(); i++ ){
        QDomElement elem = elements.at( i ).toElement();
        if(!elem.isNull()){
            QString fieldName = elem.attribute( "name" );
            if ( fieldName == name ){
                return true;
            }
        }
    }
    return false;
}

bool VDomDocument::appendDraw(const QString& name){
    if(name.isEmpty()){
        return false;
    }
    if(CheckNameDraw(name)== false){
        QDomElement rootElement = this->documentElement();

        QDomElement drawElement = this->createElement("draw");
        QDomAttr drawAttr = this->createAttribute("name");
        drawAttr.setValue(name);
        drawElement.setAttributeNode(drawAttr);

        QDomElement calculationElement = this->createElement("calculation");
        QDomElement modelingElement = this->createElement("modeling");
        QDomElement pathsElement = this->createElement("paths");
        drawElement.appendChild(calculationElement);
        drawElement.appendChild(modelingElement);
        drawElement.appendChild(pathsElement);

        rootElement.appendChild(drawElement);

        if(nameActivDraw.isEmpty()){
            SetActivDraw(name);
        } else {
            ChangeActivDraw(name);
        }
        return true;
    } else {
        return false;
    }
    return false;
}

void VDomDocument::ChangeActivDraw(const QString& name){
    if(CheckNameDraw(name) == true){
        this->nameActivDraw = name;
        VMainGraphicsScene  *scene = new VMainGraphicsScene();
        QDomElement domElement;
        bool ok = GetActivDrawElement(domElement);
        if(ok){
            ParseDrawElement(scene, domElement, Document::LiteParse);
        }
        delete scene;
        emit ChangedActivDraw(name);
    }
}

void VDomDocument::SetNameDraw(const QString& name){
    QString oldName = nameActivDraw;
    nameActivDraw = name;
    emit ChangedNameDraw(oldName, nameActivDraw);
}

void VDomDocument::SetActivDraw(const QString& name){
    this->nameActivDraw = name;
}

QString VDomDocument::GetNameActivDraw() const{
    return nameActivDraw;
}

bool VDomDocument::GetActivDrawElement(QDomElement &element){
    if(!nameActivDraw.isEmpty()){
        QDomNodeList elements = this->documentElement().elementsByTagName( "draw" );
        if(elements.size() == 0){
            return false;
        }
        for ( qint32 i = 0; i < elements.count(); i++ ){
            element = elements.at( i ).toElement();
            if(!element.isNull()){
                QString fieldName = element.attribute( "name" );
                if ( fieldName == nameActivDraw ){
                    return true;
                }
            }
        }
    }
    return false;
}

bool VDomDocument::GetActivCalculationElement(QDomElement &element){
    bool ok = GetActivNodeElement("calculation", element);
    if(ok){
        return true;
    } else {
        return false;
    }
}

bool VDomDocument::GetActivModelingElement(QDomElement &element){
    bool ok = GetActivNodeElement("modeling", element);
    if(ok){
        return true;
    } else {
        return false;
    }
}

bool VDomDocument::GetActivPathsElement(QDomElement &element){
    bool ok = GetActivNodeElement("paths", element);
    if(ok){
        return true;
    } else {
        return false;
    }
}

bool VDomDocument::GetActivNodeElement(const QString& name, QDomElement &element){
    QDomElement drawElement;
    bool drawOk = this->GetActivDrawElement(drawElement);
    if(drawOk == true){
        QDomNodeList listElement = drawElement.elementsByTagName(name);
        if(listElement.size() == 0 || listElement.size() > 1){
            return false;
        }
        element = listElement.at( 0 ).toElement();
        if(!element.isNull()){
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void VDomDocument::Parse(Document::Enum parse, VMainGraphicsScene *scene, QComboBox *comboBoxDraws){
    if(parse == Document::FullParse){
        data->Clear();
        nameActivDraw.clear();
        scene->clear();
        comboBoxDraws->clear();
    } else {
        data->ClearLengthLines();
        data->ClearLengthArcs();
        data->ClearLengthSplines();
        data->ClearLineArcs();
    }
    QDomElement rootElement = this->documentElement();
    QDomNode domNode = rootElement.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName()=="draw"){
                    if(parse == Document::FullParse){
                        if(nameActivDraw.isEmpty()){
                            SetActivDraw(domElement.attribute("name"));
                        } else {
                            ChangeActivDraw(domElement.attribute("name"));
                        }
                        AddNewDraw(domElement, comboBoxDraws);
                    }
                    ParseDrawElement(scene, domElement, parse);
                }
                if(domElement.tagName()=="increments"){
                    ParseIncrementsElement(domElement);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParseIncrementsElement(const QDomNode &node){
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "increment"){
                    QString name,desc;
                    qint32 base;
                    qreal ksize, kgrowth;
                    qint64 id;
                    id = domElement.attribute("id", "").toLongLong();
                    name = domElement.attribute("name", "");
                    base = domElement.attribute("base","").toInt();
                    ksize = domElement.attribute("ksize","").toDouble();
                    kgrowth = domElement.attribute("kgrowth","").toDouble();
                    desc = domElement.attribute("description","");
                    data->AddIncrementTableRow(name,
                                               VIncrementTableRow(id, base, ksize, kgrowth, desc));
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::AddNewDraw(const QDomElement& node, QComboBox *comboBoxDraws)const{
    QString name = node.attribute("name");
    QDomNode domNode = node.firstChild();
    if(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "calculation"){
                    QDomNode domCal = domElement.firstChild();
                    if(!domCal.isNull()){
                        if(domCal.isElement()){
                            QDomElement domElementPoint = domCal.toElement();
                            if(!domElementPoint.isNull()){
                                if(domElementPoint.tagName() == "point"){
                                    if(domElementPoint.attribute("type","") == "single"){
                                        comboBoxDraws->addItem(name, false);
                                        return;
                                    } else {
                                        comboBoxDraws->addItem(name, true);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    comboBoxDraws->addItem(name, true);
                }
            }
        }
    }
}

void VDomDocument::ParseDrawElement(VMainGraphicsScene *scene, const QDomNode& node,
                                    Document::Enum parse){
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "calculation"){
                    ParseCalculationElement(scene, domElement, parse);
                }
                if(domElement.tagName() == "modeling"){

                }
                if(domElement.tagName() == "paths"){

                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParseCalculationElement(VMainGraphicsScene *scene, const QDomNode& node,
                                           Document::Enum parse){
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for(qint32 i = 0; i < num; ++i){
        QDomElement domElement = nodeList.at(i).toElement();
        if(!domElement.isNull()){
            if(domElement.tagName() == "point"){
                ParsePointElement(scene, domElement, parse, domElement.attribute("type", ""));
            }
            if(domElement.tagName() == "line"){
                ParseLineElement(scene, domElement, parse);
            }
            if(domElement.tagName() == "spline"){
                ParseSplineElement(scene, domElement, parse, domElement.attribute("type", ""));
            }
            if(domElement.tagName() == "arc"){
                ParseArcElement(scene, domElement, parse, domElement.attribute("type", ""));
            }
        }
    }
}

void VDomDocument::ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     Document::Enum parse, const QString& type){
    if(type == "single"){
        if(!domElement.isNull()){
            QString name;
            qreal mx=5, my=10, x, y;
            qint64 id;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                x = domElement.attribute("x","").toDouble()*PrintDPI/25.4;
                y = domElement.attribute("y","").toDouble()*PrintDPI/25.4;
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                data->UpdatePoint(id, VPointF(x, y, name, mx, my));
                if(parse == Document::FullParse){
                    VToolSinglePoint *spoint = new VToolSinglePoint(this, data, id, Tool::FromFile);
                    scene->addItem(spoint);
                    connect(spoint, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                }
            }
        }
        return;
    }
    if(type == "endLine"){
        if(!domElement.isNull()){
            QString name, typeLine, formula;
            qreal mx=5, my=10;
            qint64 id, basePointId;
            qint32 angle;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                typeLine = domElement.attribute("typeLine", "");
                formula = domElement.attribute("length", "");
                basePointId = domElement.attribute("basePoint", "").toLongLong();
                angle = domElement.attribute("angle", "").toInt();

                VPointF basePoint = data->GetPoint(basePointId);
                QLineF line = QLineF(basePoint.toQPointF(), QPointF(basePoint.x()+100, basePoint.y()));
                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(formula, &errorMsg);
                if(errorMsg.isEmpty()){
                    line.setLength(result*PrintDPI/25.4);
                    line.setAngle(angle);
                    data->UpdatePoint(id, VPointF(line.p2().x(), line.p2().y(), name, mx, my));
                    data->AddLine(basePointId, id);
                    if(parse == Document::FullParse){
                        VToolEndLine *point = new VToolEndLine(this, data, id, typeLine, formula, angle,
                                                               basePointId, Tool::FromFile);
                        scene->addItem(point);
                        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
    if(type == "alongLine"){
        if(!domElement.isNull()){
            QString name, typeLine, formula;
            qreal mx=5, my=10;
            qint64 id, firstPointId, secondPointId;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                typeLine = domElement.attribute("typeLine", "");
                formula = domElement.attribute("length", "");
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();

                VPointF firstPoint = data->GetPoint(firstPointId);
                VPointF secondPoint = data->GetPoint(secondPointId);
                QLineF line = QLineF(firstPoint.toQPointF(), secondPoint.toQPointF());
                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(formula, &errorMsg);
                if(errorMsg.isEmpty()){
                    line.setLength(result*PrintDPI/25.4);
                    data->UpdatePoint(id, VPointF(line.p2().x(), line.p2().y(), name, mx, my));
                    data->AddLine(firstPointId, id);
                    data->AddLine(id, secondPointId);
                    if(parse == Document::FullParse){
                        VToolAlongLine *point = new VToolAlongLine(this, data, id, formula, firstPointId,
                                                                   secondPointId, typeLine, Tool::FromGui);
                        scene->addItem(point);
                        connect(point, &VToolAlongLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
    if(type == "shoulder"){
        if(!domElement.isNull()){
            QString name, typeLine, formula;
            qreal mx=5, my=10;
            qint64 id, p1Line, p2Line, pShoulder;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                typeLine = domElement.attribute("typeLine", "");
                formula = domElement.attribute("length", "");
                p1Line = domElement.attribute("p1Line", "").toLongLong();
                p2Line = domElement.attribute("p2Line", "").toLongLong();
                pShoulder = domElement.attribute("pShoulder", "").toLongLong();

                VPointF firstPoint = data->GetPoint(p1Line);
                VPointF secondPoint = data->GetPoint(p2Line);
                VPointF shoulderPoint = data->GetPoint(pShoulder);

                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(formula, &errorMsg);
                if(errorMsg.isEmpty()){
                    QPointF fPoint = VToolShoulderPoint::FindPoint(firstPoint, secondPoint, shoulderPoint,
                                                                  result*PrintDPI/25.4);
                    data->UpdatePoint(id,VPointF(fPoint.x(), fPoint.y(), name, mx, my));
                    data->AddLine(p1Line, id);
                    data->AddLine(p2Line, id);
                    if(parse == Document::FullParse){
                        VToolShoulderPoint *point = new VToolShoulderPoint(this, data, id, typeLine, formula,
                                                                           p1Line, p2Line, pShoulder,
                                                                           Tool::FromGui);
                        scene->addItem(point);
                        connect(point, &VToolShoulderPoint::ChoosedTool, scene,
                                &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
    if(type == "normal"){
        if(!domElement.isNull()){
            QString name, typeLine, formula;
            qreal mx=5, my=10, angle;
            qint64 id, firstPointId, secondPointId;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                typeLine = domElement.attribute("typeLine", "");
                formula = domElement.attribute("length", "");
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
                angle = domElement.attribute("angle", "").toInt();

                VPointF firstPoint = data->GetPoint(firstPointId);
                VPointF secondPoint = data->GetPoint(secondPointId);
                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(formula, &errorMsg);
                if(errorMsg.isEmpty()){
                    QPointF fPoint = VToolNormal::FindPoint(firstPoint, secondPoint, result*PrintDPI/25.4,
                                                            angle);
                    data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), name, mx, my));
                    data->AddLine(firstPointId, id);
                    if(parse == Document::FullParse){
                        VToolNormal *point = new VToolNormal(this, data, id, typeLine, formula, angle,
                                                             firstPointId, secondPointId, Tool::FromFile);
                        scene->addItem(point);
                        connect(point, &VToolNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
    if(type == "bisector"){
        if(!domElement.isNull()){
            QString name, typeLine, formula;
            qreal mx=5, my=10;
            qint64 id, firstPointId, secondPointId, thirdPointId;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                typeLine = domElement.attribute("typeLine", "");
                formula = domElement.attribute("length", "");
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
                thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();

                VPointF firstPoint = data->GetPoint(firstPointId);
                VPointF secondPoint = data->GetPoint(secondPointId);
                VPointF thirdPoint = data->GetPoint(thirdPointId);

                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(formula, &errorMsg);
                if(errorMsg.isEmpty()){
                    QPointF fPoint = VToolBisector::FindPoint(firstPoint, secondPoint, thirdPoint,
                                                              result*PrintDPI/25.4);
                    data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), name, mx, my));
                    data->AddLine(firstPointId, id);
                    if(parse == Document::FullParse){
                        VToolBisector *point = new VToolBisector(this, data, id, typeLine, formula,
                                                                 firstPointId, secondPointId, thirdPointId,
                                                                 Tool::FromFile);
                        scene->addItem(point);
                        connect(point, &VToolBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
    if(type == "lineIntersect"){
        if(!domElement.isNull()){
            QString name;
            qreal mx=5, my=10;
            qint64 id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                name = domElement.attribute("name", "");
                mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
                my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

                p1Line1Id = domElement.attribute("p1Line1", "").toLongLong();
                p2Line1Id = domElement.attribute("p2Line1", "").toLongLong();
                p1Line2Id = domElement.attribute("p1Line2", "").toLongLong();
                p2Line2Id = domElement.attribute("p2Line2", "").toLongLong();

                VPointF p1Line1 = data->GetPoint(p1Line1Id);
                VPointF p2Line1 = data->GetPoint(p2Line1Id);
                VPointF p1Line2 = data->GetPoint(p1Line2Id);
                VPointF p2Line2 = data->GetPoint(p2Line2Id);

                QLineF line1(p1Line1, p2Line1);
                QLineF line2(p1Line2, p2Line2);
                QPointF fPoint;
                QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
                if(intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection){
                    data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), name, mx, my));
                    data->AddLine(p1Line1Id, id);
                    data->AddLine(id, p2Line1Id);
                    data->AddLine(p1Line2Id, id);
                    data->AddLine(id, p2Line2Id);
                    if(parse == Document::FullParse){
                        VToolLineIntersect *point = new VToolLineIntersect(this, data, id, p1Line1Id,
                                                                           p2Line1Id, p1Line2Id,
                                                                           p2Line2Id, Tool::FromFile);
                        scene->addItem(point);
                        connect(point, &VToolLineIntersect::ChoosedTool, scene,
                                &VMainGraphicsScene::ChoosedItem);
                    }
                }
            }
        }
        return;
    }
}

void VDomDocument::ParseLineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                    Document::Enum parse){
    if(!domElement.isNull()){
        qint64 firstPoint;
        qint64 secondPoint;
        if(!domElement.isNull()){
            firstPoint = domElement.attribute("firstPoint", "").toLongLong();
            secondPoint = domElement.attribute("secondPoint", "").toLongLong();
            data->AddLine(firstPoint, secondPoint);
            if(parse == Document::FullParse){
                qint64 id = data->getNextId();
                VToolLine *line = new VToolLine(this, data, id, firstPoint, secondPoint, Tool::FromFile);
                scene->addItem(line);
                connect(line, &VToolLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            }
        }
    }
}

void VDomDocument::ParseSplineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                      Document::Enum parse, const QString &type){
    if(type == "simple"){
        if(!domElement.isNull()){
            qreal angle1, angle2, kAsm1, kAsm2, kCurve;
            qint64 id, point1, point4;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                point1 = domElement.attribute("point1", "").toLongLong();
                point4 = domElement.attribute("point4", "").toLongLong();
                angle1 = domElement.attribute("angle1","").toDouble();
                angle2 = domElement.attribute("angle2","").toDouble();
                kAsm1 = domElement.attribute("kAsm1","").toDouble();
                kAsm2 = domElement.attribute("kAsm2","").toDouble();
                kCurve = domElement.attribute("kCurve","").toDouble();

                VSpline spline = VSpline(data->DataPoints(), point1, point4, angle1, angle2, kAsm1, kAsm2, kCurve);
                data->UpdateSpline(id, spline);
                data->AddLengthSpline(data->GetNameSpline(point1, point4), spline.GetLength());
                if(parse == Document::FullParse){
                    VToolSpline *spl = new VToolSpline(this, data, id, Tool::FromFile);
                    scene->addItem(spl);
                    connect(spl, &VToolSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                }
            }
        }
        return;
    }
}

void VDomDocument::ParseArcElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                   Document::Enum parse, const QString &type){
    if(type == "simple"){
        if(!domElement.isNull()){
            QString radius, f1, f2;
            qint64 id, center;
            if(!domElement.isNull()){
                id = domElement.attribute("id", "").toLongLong();
                center = domElement.attribute("center", "").toLongLong();
                radius = domElement.attribute("radius", "");
                f1 = domElement.attribute("angle1", "");
                f2 = domElement.attribute("angle2","");

                qreal calcRadius = 0, calcF1 = 0, calcF2 = 0;

                Calculator cal(data);
                QString errorMsg;
                qreal result = cal.eval(radius, &errorMsg);
                if(errorMsg.isEmpty()){
                    calcRadius = result*PrintDPI/25.4;
                }

                errorMsg.clear();
                result = cal.eval(f1, &errorMsg);
                if(errorMsg.isEmpty()){
                    calcF1 = result;
                }

                errorMsg.clear();
                result = cal.eval(f2, &errorMsg);
                if(errorMsg.isEmpty()){
                    calcF2 = result;
                }

                VArc arc = VArc(data->DataPoints(), center, calcRadius, radius, calcF1, f1, calcF2, f2 );
                data->UpdateArc(id, arc);
                data->AddLengthArc(data->GetNameArc(center,id), arc.GetLength());
                if(parse == Document::FullParse){
                    VToolArc *toolArc = new VToolArc(this, data, id, Tool::FromFile);
                    scene->addItem(toolArc);
                    connect(toolArc, &VToolArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                }
            }
        }
        return;
    }
}

void VDomDocument::FullUpdateTree(){
    VMainGraphicsScene *scene = new VMainGraphicsScene();
    QComboBox *comboBoxDraws = new QComboBox();
    Parse(Document::LiteParse, scene, comboBoxDraws );
    delete scene;
    delete comboBoxDraws;
    emit FullUpdateFromFile();
    emit haveChange();
}

void VDomDocument::haveLiteChange(){
    emit haveChange();
}
