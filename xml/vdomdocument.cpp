#include "vdomdocument.h"
#include <QDebug>
#include "tools/drawTools/drawtools.h"
#include "tools/nodeDetails/nodedetails.h"
#include "tools/modelingTools/modelingtools.h"
#include "tools/vtooldetail.h"
#include "options.h"
#include "container/calculator.h"
#include "geometry/vsplinepoint.h"



VDomDocument::VDomDocument(VContainer *data, QComboBox *comboBoxDraws, Draw::Mode *mode) : QDomDocument(),
    map(QMap<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QMap<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){
}

VDomDocument::VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws,
                           Draw::Mode *mode) :
    QDomDocument(name), map(QMap<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QMap<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){
}

VDomDocument::VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                           Draw::Mode *mode) :
    QDomDocument(doctype), map(QMap<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QMap<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){
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
        QDomElement pathsElement = this->createElement("details");
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

void VDomDocument::ChangeActivDraw(const QString& name, Document::Enum parse){
    if(CheckNameDraw(name) == true){
        this->nameActivDraw = name;
        if(parse == Document::FullParse){
            emit ChangedActivDraw(name);
        }
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

bool VDomDocument::GetActivDetailsElement(QDomElement &element){
    bool ok = GetActivNodeElement("details", element);
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

void VDomDocument::Parse(Document::Enum parse, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail){
    if(parse == Document::FullParse){
        data->Clear();
        nameActivDraw.clear();
        sceneDraw->clear();
        sceneDetail->clear();
        comboBoxDraws->clear();
        tools.clear();
        cursor = 0;
    }
    data->ClearLengthLines();
    data->ClearLengthArcs();
    data->ClearLengthSplines();
    data->ClearLineAngles();
    history.clear();
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
                        comboBoxDraws->addItem(domElement.attribute("name"));
                    } else {
                        ChangeActivDraw(domElement.attribute("name"), Document::LiteParse);
                    }
                    ParseDrawElement(sceneDraw, sceneDetail, domElement, parse);
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

QVector<VToolRecord> *VDomDocument::getHistory(){
    return &history;
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

void VDomDocument::ParseDrawElement(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, Document::Enum parse){
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "calculation"){
                    data->ClearObject();
                    ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Draw::Calculation);
                }
                if(domElement.tagName() == "modeling"){
                    ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Draw::Modeling);
                }
                if(domElement.tagName() == "details"){
                    ParseDetails(sceneDetail, domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParseDrawMode(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
                                           const QDomNode& node, Document::Enum parse, Draw::Mode mode){
    VMainGraphicsScene *scene = 0;
    if(mode == Draw::Calculation){
        scene = sceneDraw;
    } else {
        scene = sceneDetail;
    }
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for(qint32 i = 0; i < num; ++i){
        QDomElement domElement = nodeList.at(i).toElement();
        if(!domElement.isNull()){
            if(domElement.tagName() == "point"){
                ParsePointElement(scene, domElement, parse, domElement.attribute("type", ""), mode);
            }
            if(domElement.tagName() == "line"){
                ParseLineElement(scene, domElement, parse, mode);
            }
            if(domElement.tagName() == "spline"){
                ParseSplineElement(scene, domElement, parse, domElement.attribute("type", ""), mode);
            }
            if(domElement.tagName() == "arc"){
                ParseArcElement(scene, domElement, parse, domElement.attribute("type", ""), mode);
            }
        }
    }
}

void VDomDocument::ParseDetailElement(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                                      Document::Enum parse){
    if(!domElement.isNull()){
        VDetail detail;
        VDetail oldDetail;
        qint64 id = domElement.attribute("id", "").toLongLong();
        detail.setName(domElement.attribute("name", ""));
        detail.setMx(toPixel(domElement.attribute("mx","").toDouble()));
        detail.setMy(toPixel(domElement.attribute("my","").toDouble()));

        QDomNodeList nodeList = domElement.childNodes();
        qint32 num = nodeList.size();
        for(qint32 i = 0; i < num; ++i){
            QDomElement element = nodeList.at(i).toElement();
            if(!element.isNull()){
                if(element.tagName() == "node"){
                    qint64 id = element.attribute("id","").toLongLong();
                    Tools::Enum tool;
                    Draw::Mode mode;
                    NodeDetail::Type nodeType = NodeDetail::Contour;
                    QString t = element.attribute("type","");
                    if(t == "NodePoint"){
                        tool = Tools::NodePoint;
                        VPointF point = data->GetModelingPoint(id);
                        mode = point.getMode();
                        oldDetail.append(VNodeDetail(point.getIdObject(), tool, mode, NodeDetail::Contour));
                    } else if(t == "NodeArc"){
                        tool = Tools::NodeArc;
                        VArc arc = data->GetModelingArc(id);
                        mode = arc.getMode();
                        oldDetail.append(VNodeDetail(arc.getIdObject(), tool, mode, NodeDetail::Contour));
                    } else if(t == "NodeSpline"){
                        tool = Tools::NodeSpline;
                        VSpline spl = data->GetModelingSpline(id);
                        mode = spl.getMode();
                        oldDetail.append(VNodeDetail(spl.getIdObject(), tool, mode, NodeDetail::Contour));
                    } else if(t == "NodeSplinePath"){
                        tool = Tools::NodeSplinePath;
                        VSplinePath splPath = data->GetModelingSplinePath(id);
                        mode = splPath.getMode();
                        oldDetail.append(VNodeDetail(splPath.getIdObject(), tool, mode, NodeDetail::Contour));
                    } else if(t == "AlongLineTool"){
                        tool = Tools::AlongLineTool;
                    } else if(t == "ArcTool"){
                        tool = Tools::ArcTool;
                    } else if(t == "BisectorTool"){
                        tool = Tools::BisectorTool;
                    } else if(t == "EndLineTool"){
                        tool = Tools::EndLineTool;
                    } else if(t == "LineIntersectTool"){
                        tool = Tools::LineIntersectTool;
                    } else if(t == "LineTool"){
                        tool = Tools::LineTool;
                    } else if(t == "NormalTool"){
                        tool = Tools::NormalTool;
                    } else if(t == "PointOfContact"){
                        tool = Tools::PointOfContact;
                    } else if(t == "ShoulderPointTool"){
                        tool = Tools::ShoulderPointTool;
                    } else if(t == "SplinePathTool"){
                        tool = Tools::SplinePathTool;
                    } else if(t == "SplineTool"){
                        tool = Tools::SplineTool;
                    }
                    detail.append(VNodeDetail(id, tool, mode, nodeType));
                }
            }
        }
        VToolDetail::Create(id, detail, oldDetail, sceneDetail, this, data, parse, Tool::FromFile);
    }
}

void VDomDocument::ParseDetails(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                                Document::Enum parse){
    QDomNode domNode = domElement.firstChild();
    while(!domNode.isNull()){
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "detail"){
                    ParseDetailElement(sceneDetail, domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     Document::Enum parse, const QString& type, Draw::Mode mode){
    if(type == "single"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            Q_ASSERT(id > 0);
            QString name = domElement.attribute("name", "");
            qreal x = domElement.attribute("x","").toDouble()*PrintDPI/25.4;
            qreal y = domElement.attribute("y","").toDouble()*PrintDPI/25.4;
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;

            data->UpdatePoint(id, VPointF(x, y, name, mx, my));
            VDrawTool::AddRecord(id, Tools::SinglePointTool, this);
            if(parse != Document::FullParse){
                VDataTool *tool = tools.value(id);
                Q_CHECK_PTR(tool);
                tool->VDataTool::setData(data);
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
            if(mode == Draw::Calculation){
                VToolEndLine::Create(id, name, typeLine, formula, angle, basePointId, mx, my, scene, this,
                                     data, parse, Tool::FromFile);
            } else {
                VModelingEndLine::Create(id, name, typeLine, formula, angle, basePointId, mx, my, this,
                                         data, parse, Tool::FromFile);
            }
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
            if(mode == Draw::Calculation){
                VToolAlongLine::Create(id, name, typeLine, formula, firstPointId, secondPointId, mx, my,
                                       scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingAlongLine::Create(id, name, typeLine, formula, firstPointId, secondPointId, mx, my,
                                           this, data, parse, Tool::FromFile);
            }
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
            if(mode == Draw::Calculation){
                VToolShoulderPoint::Create(id, formula, p1Line, p2Line, pShoulder, typeLine, name, mx, my,
                                           scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingShoulderPoint::Create(id, formula, p1Line, p2Line, pShoulder, typeLine, name, mx,
                                               my, this, data, parse, Tool::FromFile);
            }
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
            qreal angle = domElement.attribute("angle", "").toDouble();
            if(mode == Draw::Calculation){
                VToolNormal::Create(id, formula, firstPointId, secondPointId, typeLine, name, angle,
                                    mx, my, scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingNormal::Create(id, formula, firstPointId, secondPointId, typeLine, name, angle,
                                        mx, my, this, data, parse, Tool::FromFile);
            }
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
            if(mode == Draw::Calculation){
                VToolBisector::Create(id, formula, firstPointId, secondPointId, thirdPointId, typeLine,
                                      name, mx, my, scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingBisector::Create(id, formula, firstPointId, secondPointId, thirdPointId, typeLine,
                                          name, mx, my, this, data, parse, Tool::FromFile);
            }
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
            if(mode == Draw::Calculation){
                VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name, mx, my,
                                           scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name, mx, my,
                                               this, data, parse, Tool::FromFile);
            }
        }
        return;
    }
    if(type == "pointOfContact"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            QString name = domElement.attribute("name", "");
            qreal mx = domElement.attribute("mx","").toDouble()*PrintDPI/25.4;
            qreal my = domElement.attribute("my","").toDouble()*PrintDPI/25.4;
            QString radius = domElement.attribute("radius", "");
            qint64 center = domElement.attribute("center", "").toLongLong();
            qint64 firstPointId = domElement.attribute("firstPoint", "").toLongLong();
            qint64 secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            if(mode == Draw::Calculation){
                VToolPointOfContact::Create(id, radius, center, firstPointId, secondPointId, name, mx, my,
                                            scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingPointOfContact::Create(id, radius, center, firstPointId, secondPointId, name, mx,
                                                my, this, data, parse, Tool::FromFile);
            }
        }
        return;
    }
    if(type == "modeling"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 idObject = domElement.attribute("idObject", "").toLongLong();
            QString tObject = domElement.attribute("typeObject", "");
            VPointF point;
            Draw::Mode typeObject;
            if(tObject == "Calculation"){
                typeObject = Draw::Calculation;
                point = data->GetPoint(idObject );
            } else {
                typeObject = Draw::Modeling;
                point = data->GetModelingPoint(idObject);
            }
            qreal mx = toPixel(domElement.attribute("mx","").toDouble());
            qreal my = toPixel(domElement.attribute("my","").toDouble());
            data->UpdateModelingPoint(id, VPointF(point.x(), point.y(), point.name(), mx, my, typeObject,
                                                  idObject ));
            data->IncrementReferens(idObject, Scene::Point, typeObject);
        }
        return;
    }
}

void VDomDocument::ParseLineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                    Document::Enum parse, Draw::Mode mode){
    if(!domElement.isNull()){
        qint64 id = domElement.attribute("id", "").toLongLong();
        qint64 firstPoint = domElement.attribute("firstPoint", "").toLongLong();
        qint64 secondPoint = domElement.attribute("secondPoint", "").toLongLong();
        if(mode == Draw::Calculation){
            VToolLine::Create(id, firstPoint, secondPoint, scene, this, data, parse, Tool::FromFile);
        } else {
            VModelingLine::Create(id, firstPoint, secondPoint, this, data, parse, Tool::FromFile);
        }
    }
}

void VDomDocument::ParseSplineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                      Document::Enum parse, const QString &type, Draw::Mode mode){
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
            if(mode == Draw::Calculation){
                VToolSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, this,
                                    data, parse, Tool::FromFile);
            } else {
                VModelingSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, this,
                                        data, parse, Tool::FromFile);
            }
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
            if(mode == Draw::Calculation){
                VToolSplinePath::Create(id, path, scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingSplinePath::Create(id, path, this, data, parse, Tool::FromFile);
            }
        }
        return;
    }
    if(type == "modelingSpline"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 idObject = domElement.attribute("idObject", "").toLongLong();
            QString tObject = domElement.attribute("typeObject", "");
            VSpline spl;
            Draw::Mode typeObject;
            if(tObject == "Calculation"){
                typeObject = Draw::Calculation;
                spl = data->GetSpline(idObject);
            } else {
                typeObject = Draw::Modeling;
                spl = data->GetModelingSpline(idObject);
            }
            spl.setMode(typeObject);
            spl.setIdObject(idObject);
            data->UpdateModelingSpline(id, spl);
            data->IncrementReferens(spl.GetP1(), Scene::Point, Draw::Modeling);
            data->IncrementReferens(spl.GetP4(), Scene::Point, Draw::Modeling);
        }
        return;
    }
    if(type == "modelingPath"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 idObject = domElement.attribute("idObject", "").toLongLong();
            QString tObject = domElement.attribute("typeObject", "");
            VSplinePath path;
            Draw::Mode typeObject;
            if(tObject == "Calculation"){
                typeObject = Draw::Calculation;
                path = data->GetSplinePath(idObject);
            } else {
                typeObject = Draw::Modeling;
                path = data->GetModelingSplinePath(idObject);
            }
            path.setMode(typeObject);
            path.setIdObject(idObject);
            data->UpdateModelingSplinePath(id, path);
            const QVector<VSplinePoint> *points = path.GetPoint();
            for(qint32 i = 0; i<points->size(); ++i){
                data->IncrementReferens(points->at(i).P(), Scene::Point, Draw::Modeling);
            }
        }
        return;
    }
}

void VDomDocument::ParseArcElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                   Document::Enum parse, const QString &type, Draw::Mode mode){
    if(type == "simple"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 center = domElement.attribute("center", "").toLongLong();
            QString radius = domElement.attribute("radius", "");
            QString f1 = domElement.attribute("angle1", "");
            QString f2 = domElement.attribute("angle2","");
            if(mode == Draw::Calculation){
                VToolArc::Create(id, center, radius, f1, f2, scene, this, data, parse, Tool::FromFile);
            } else {
                VModelingArc::Create(id, center, radius, f1, f2, this, data, parse, Tool::FromFile);
            }
        }
        return;
    }
    if(type == "modeling"){
        if(!domElement.isNull()){
            qint64 id = domElement.attribute("id", "").toLongLong();
            qint64 idObject = domElement.attribute("idObject", "").toLongLong();
            QString tObject = domElement.attribute("typeObject", "");
            VArc arc;
            Draw::Mode typeObject;
            if(tObject == "Calculation"){
                typeObject = Draw::Calculation;
                arc = data->GetArc(idObject);
            } else {
                typeObject = Draw::Modeling;
                arc = data->GetModelingArc(idObject);
            }
            arc.setMode(typeObject);
            arc.setIdObject(idObject);
            data->UpdateModelingArc(id, arc);
        }
        return;
    }
}

void VDomDocument::FullUpdateTree(){
    VMainGraphicsScene *scene = new VMainGraphicsScene();
    data->ClearObject();
    Parse(Document::LiteParse, scene, scene);
    delete scene;
    setCurrentData();
    emit FullUpdateFromFile();
    emit haveChange();
}

void VDomDocument::haveLiteChange(){
    emit haveChange();
}

void VDomDocument::ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable){
    emit ShowTool(id, color, enable);
}

qint64 VDomDocument::getCursor() const{
    return cursor;
}

void VDomDocument::setCursor(const qint64 &value){
    cursor = value;
    emit ChangedCursor(cursor);
}

void VDomDocument::setCurrentData(){
    if(*mode == Draw::Calculation){
        QString nameDraw = comboBoxDraws->itemText(comboBoxDraws->currentIndex());
        if(nameActivDraw != nameDraw){
            nameActivDraw = nameDraw;
            qint64 id = 0;
            if(history.size() == 0){
                return;
            }
            for(qint32 i = 0; i < history.size(); ++i){
                VToolRecord tool = history.at(i);
                if(tool.getNameDraw() == nameDraw){
                    id = tool.getId();
                }
            }
            if(id == 0){
                VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                if(id == 0){
                    return;
                }
            }
            if(tools.size() > 0){
                VDataTool *vTool = tools.value(id);
                data->setData(vTool->getData());
            }
        }
    }
}

void VDomDocument::GarbageCollector(){
    const QMap<qint64, VPointF> *points = data->DataPoints();
    QMapIterator<qint64, VPointF> p(*points);
    while (p.hasNext()) {
        p.next();
        VPointF point = p.value();
        if(point.referens() <= 0){
            QDomElement domElement = elementById(QString().setNum(p.key()));
            if(domElement.isElement()){
                QDomElement element;
                bool ok = GetActivModelingElement(element);
                if(ok){
                    element.removeChild(domElement);
                }
            }
        }
    }

    const QMap<qint64, VArc> *arc = data->DataArcs();
    QMapIterator<qint64, VArc> a(*arc);
    while (a.hasNext()) {
        a.next();
        VArc arc = a.value();
        if(arc.referens() <= 0){
            QDomElement domElement = elementById(QString().setNum(a.key()));
            if(domElement.isElement()){
                QDomElement element;
                bool ok = GetActivModelingElement(element);
                if(ok){
                    element.removeChild(domElement);
                }
            }
        }
    }

    const QMap<qint64, VSpline> *spl = data->DataSplines();
    QMapIterator<qint64, VSpline> s(*spl);
    while (s.hasNext()) {
        s.next();
        VSpline spl = s.value();
        if(spl.referens() <= 0){
            QDomElement domElement = elementById(QString().setNum(s.key()));
            if(domElement.isElement()){
                QDomElement element;
                bool ok = GetActivModelingElement(element);
                if(ok){
                    element.removeChild(domElement);
                }
            }
        }
    }

    const QMap<qint64, VSplinePath> *splPath = data->DataSplinePaths();
    QMapIterator<qint64, VSplinePath> q(*splPath);
    while (q.hasNext()) {
        q.next();
        VSplinePath splPath = q.value();
        if(splPath.referens() <= 0){
            QDomElement domElement = elementById(QString().setNum(q.key()));
            if(domElement.isElement()){
                QDomElement element;
                bool ok = GetActivModelingElement(element);
                if(ok){
                    element.removeChild(domElement);
                }
            }
        }
    }
}

void VDomDocument::AddTool(const qint64 &id, VDataTool *tool){
    tools.insert(id, tool);
}

void VDomDocument::UpdateToolData(const qint64 &id, VContainer *data){
    VDataTool *tool = tools.value(id);
    if(tool != 0){
        tool->VDataTool::setData(data);
    } else {
        qWarning()<<"Can't find tool with id ="<< id<<".";
    }
}

