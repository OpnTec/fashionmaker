#pragma GCC diagnostic ignored "-Weffc++"
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
#include "../tools/vtoolsplinepath.h"
#pragma GCC diagnostic warning "-Weffc++"
#include "../options.h"
#include "../container/calculator.h"
#include "../geometry/vsplinepoint.h"



VDomDocument::VDomDocument(VContainer *data) : QDomDocument() {
    this->data = data;
}

VDomDocument::VDomDocument(const QString& name, VContainer *data) : QDomDocument(name) {
    this->data = data;
}

VDomDocument::VDomDocument(const QDomDocumentType& doctype, VContainer *data) : QDomDocument(doctype){
    this->data = data;
}

VDomDocument::~VDomDocument(){
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
        tools.clear();
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

QMap<qint64, VDataTool *> *VDomDocument::getTools(){
    return &tools;
}

void VDomDocument::ParseIncrementsElement(const QDomNode &node){
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "increment"){
                    QString name,desc;
                    qreal base;
                    qreal ksize, kgrowth;
                    qint64 id;
                    id = domElement.attribute("id", "").toLongLong();
                    name = domElement.attribute("name", "");
                    base = domElement.attribute("base","").toDouble();
                    ksize = domElement.attribute("ksize","").toDouble();
                    kgrowth = domElement.attribute("kgrowth","").toDouble();
                    desc = domElement.attribute("description","");
                    data->UpdateId(id);
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

            id = domElement.attribute("id", "").toLongLong();
            name = domElement.attribute("name", "");
            x = domElement.attribute("x","").toDouble()*PrintDPI/25.4;
            y = domElement.attribute("y","").toDouble()*PrintDPI/25.4;
            mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

            data->UpdatePoint(id, VPointF(x, y, name, mx, my));
            if(parse != Document::FullParse){
                VToolSinglePoint *spoint = qobject_cast<VToolSinglePoint*>(tools[id]);
                spoint->VDataTool::setData(data);
                tools[id] = spoint;
            }
            if(parse == Document::FullParse){
                VToolSinglePoint *spoint = new VToolSinglePoint(this, data, id, Tool::FromFile);
                scene->addItem(spoint);
                connect(spoint, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                tools[id] = spoint;
            }
        }
        return;
    }
    if(type == "endLine"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString typeLine = domElement.attribute("typeLine", "");
            QString formula = domElement.attribute("length", "");
            qint64 basePointId = domElement.attribute("basePoint", "").toLongLong();
            qint32 angle = domElement.attribute("angle", "").toInt();
            VToolEndLine::Create(id, name, typeLine, formula, angle, basePointId, mx, my, scene, this, data,
                                 parse, Tool::FromFile);
        }
        return;
    }
    if(type == "alongLine"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString typeLine = domElement.attribute("typeLine", "");
            QString formula = domElement.attribute("length", "");
            qint64 firstPointId = domElement.attribute("firstPoint", "").toLongLong();
            qint64 secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            VToolAlongLine::Create(id, name, typeLine, formula, firstPointId, secondPointId, mx, my,
                                   scene, this, data, parse, Tool::FromFile);
        }
        return;
    }
    if(type == "shoulder"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString typeLine = domElement.attribute("typeLine", "");
            QString formula = domElement.attribute("length", "");
            qint64 p1Line = domElement.attribute("p1Line", "").toLongLong();
            qint64 p2Line = domElement.attribute("p2Line", "").toLongLong();
            qint64 pShoulder = domElement.attribute("pShoulder", "").toLongLong();
            VToolShoulderPoint::Create(id, formula, p1Line, p2Line, pShoulder, typeLine, name, mx, my,
                                       scene, this, data, parse, Tool::FromFile);
        }
        return;
    }
    if(type == "normal"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString typeLine = domElement.attribute("typeLine", "");
            QString formula = domElement.attribute("length", "");
            qint64 firstPointId = domElement.attribute("firstPoint", "").toLongLong();
            qint64 secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            qreal angle = domElement.attribute("angle", "").toInt();
            VToolNormal::Create(id, formula, firstPointId, secondPointId, typeLine, name, angle,
                                mx, my, scene, this, data, parse, Tool::FromFile);
        }
        return;
    }
    if(type == "bisector"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString typeLine = domElement.attribute("typeLine", "");
            QString formula = domElement.attribute("length", "");
            qint64 firstPointId = domElement.attribute("firstPoint", "").toLongLong();
            qint64 secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            qint64 thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
            VToolBisector::Create(id, formula, firstPointId, secondPointId, thirdPointId, typeLine,
                                  name, mx, my, scene, this, data, parse, Tool::FromFile);
        }
        return;
    }
    if(type == "lineIntersect"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            qint64 p1Line1Id = domElement.attribute("p1Line1", "").toLongLong();
            qint64 p2Line1Id = domElement.attribute("p2Line1", "").toLongLong();
            qint64 p1Line2Id = domElement.attribute("p1Line2", "").toLongLong();
            qint64 p2Line2Id = domElement.attribute("p2Line2", "").toLongLong();
            VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name, mx, my, scene,
                                       this, data, parse, Tool::FromFile);
        }
        return;
    }
}

void VDomDocument::ParseLineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                    Document::Enum parse){
    if(!domElement.isNull()){
        qint64 id = domElement.attribute("id", "").toLongLong();
        qint64 firstPoint = domElement.attribute("firstPoint", "").toLongLong();
        qint64 secondPoint = domElement.attribute("secondPoint", "").toLongLong();
        VToolLine::Create(id, firstPoint, secondPoint, scene, this, data, parse, Tool::FromFile);
    }
}

void VDomDocument::ParseSplineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                      Document::Enum parse, const QString &type){
    if(type == "simple"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 point1 = domElement.attribute("point1", "").toLongLong();
            qint64 point4 = domElement.attribute("point4", "").toLongLong();
            qreal angle1 = domElement.attribute("angle1","").toDouble();
            qreal angle2 = domElement.attribute("angle2","").toDouble();
            qreal kAsm1 = domElement.attribute("kAsm1","").toDouble();
            qreal kAsm2 = domElement.attribute("kAsm2","").toDouble();
            qreal kCurve = domElement.attribute("kCurve","").toDouble();
            VToolSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, this, data,
                                parse, Tool::FromFile);
        }
        return;
    }
    if(type == "path"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qreal kCurve = domElement.attribute("kCurve","").toDouble();
            VSplinePath path(data->DataPoints(), kCurve);

            QDomNodeList nodeList = domElement.childNodes();
            qint32 num = nodeList.size();
            for(qint32 i = 0; i < num; ++i){
                QDomElement element = nodeList.at(i).toElement();
                if(!element.isNull()){
                    if(element.tagName() == "pathPoint"){
                        qint64 pSpline = element.attribute("pSpline","").toLongLong();
                        qreal kAsm1 = element.attribute("kAsm1","").toDouble();
                        qreal angle = element.attribute("angle","").toDouble();
                        qreal kAsm2 = element.attribute("kAsm2","").toDouble();
                        VSplinePoint splPoint(pSpline, kAsm1, angle, kAsm2);
                        path.append(splPoint);
                    }
                }
            }
            VToolSplinePath::Create(id, path, scene, this, data, Document::FullParse, Tool::FromFile);
        }
        return;
    }
}

void VDomDocument::ParseArcElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                   Document::Enum parse, const QString &type){
    if(type == "simple"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 center = domElement.attribute("center", "").toLongLong();
            QString radius = domElement.attribute("radius", "");
            QString f1 = domElement.attribute("angle1", "");
            QString f2 = domElement.attribute("angle2","");
            VToolArc::Create(id, center, radius, f1, f2, scene, this, data, parse, Tool::FromFile);
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
