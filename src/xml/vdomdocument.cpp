/************************************************************************
 **
 **  @file   vdomdocument.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#include "vdomdocument.h"
#include "../exception/vexceptionwrongparameterid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionuniqueid.h"
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionbadid.h"
#include "../tools/vtooldetail.h"
#include "../tools/vtooluniondetails.h"
#include "../tools/drawTools/drawtools.h"
#include "../tools/nodeDetails/nodedetails.h"

#include <QMessageBox>

VDomDocument::VDomDocument(VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode, QObject *parent)
    : QObject(parent), QDomDocument(), map(QHash<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QHash<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){}

VDomDocument::VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode,
                           QObject *parent)
    :QObject(parent), QDomDocument(name), map(QHash<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QHash<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){}

VDomDocument::VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                           Draw::Draws *mode, QObject *parent)
    :QObject(parent), QDomDocument(doctype), map(QHash<QString, QDomElement>()), nameActivDraw(QString()), data(data),
    tools(QHash<qint64, VDataTool*>()), history(QVector<VToolRecord>()), cursor(0),
    comboBoxDraws(comboBoxDraws), mode(mode){}

QDomElement VDomDocument::elementById(const QString& id)
{
    if (map.contains(id))
    {
       QDomElement e = map[id];
       if (e.parentNode().nodeType() != QDomNode::BaseNode)
       {
           return e;
       }
       map.remove(id);
    }

    bool res = this->find(this->documentElement(), id);
    if (res)
    {
       return map[id];
    }

    return QDomElement();
}

void VDomDocument::removeAllChilds(QDomElement &element)
{
    QDomNode domNode = element.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                element.removeChild(domElement);
            }
        }
        domNode = element.firstChild();
    }
}

bool VDomDocument::find(const QDomElement &node, const QString& id)
{
    if (node.hasAttribute("id"))
    {
        QString value = node.attribute("id");
        this->map[value] = node;
        if (value == id)
        {
            return true;
        }
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i)
    {
        QDomNode n = node.childNodes().at(i);
        if (n.isElement())
        {
            bool res = this->find(n.toElement(), id);
            if (res)
            {
                return true;
            }
        }
    }

    return false;
}

void VDomDocument::CreateEmptyFile()
{
    QDomElement domElement = this->createElement("pattern");

    this->appendChild(domElement);
    QDomComment info = this->createComment("Valentina pattern format.");
    domElement.appendChild(info);
    QDomNode xmlNode = this->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    this->insertBefore(xmlNode, this->firstChild());
    QDomElement incrElement = this->createElement("increments");
    domElement.appendChild(incrElement);
}

bool VDomDocument::CheckNameDraw(const QString& name) const
{
    Q_ASSERT_X(name.isEmpty() == false, "CheckNameDraw", "name draw is empty");
    QDomNodeList elements = this->documentElement().elementsByTagName( "draw" );
    if (elements.size() == 0)
    {
        return false;
    }
    for ( qint32 i = 0; i < elements.count(); i++ )
    {
        QDomElement elem = elements.at( i ).toElement();
        if (elem.isNull() == false)
        {
            QString fieldName = elem.attribute( "name" );
            if ( fieldName == name )
            {
                return true;
            }
        }
    }
    return false;
}

bool VDomDocument::appendDraw(const QString& name)
{
    Q_ASSERT_X(name.isEmpty() == false, "appendDraw", "name draw is empty");
    if (name.isEmpty())
    {
        return false;
    }
    if (CheckNameDraw(name)== false)
    {
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

        if (nameActivDraw.isEmpty())
        {
            SetActivDraw(name);
        }
        else
        {
            ChangeActivDraw(name);
        }
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

void VDomDocument::ChangeActivDraw(const QString& name, const Document::Documents &parse)
{
    Q_ASSERT_X(name.isEmpty() == false, "ChangeActivDraw", "name draw is empty");
    if (CheckNameDraw(name) == true)
    {
        this->nameActivDraw = name;
        if (parse == Document::FullParse)
        {
            emit ChangedActivDraw(name);
        }
    }
}

bool VDomDocument::SetNameDraw(const QString& name)
{
    Q_ASSERT_X(name.isEmpty() == false, "SetNameDraw", "name draw is empty");
    QString oldName = nameActivDraw;
    QDomElement element;
    if (GetActivDrawElement(element))
    {
        nameActivDraw = name;
        element.setAttribute("name", nameActivDraw);
        emit haveChange();
        emit ChangedNameDraw(oldName, nameActivDraw);
        return true;
    }
    else
    {
        qWarning()<<"Can't find activ draw"<<Q_FUNC_INFO;
        return false;
    }
}

void VDomDocument::SetActivDraw(const QString& name)
{
    Q_ASSERT_X(name.isEmpty() == false, "SetActivDraw", "name draw is empty");
    this->nameActivDraw = name;
}

bool VDomDocument::GetActivDrawElement(QDomElement &element)
{
    if (nameActivDraw.isEmpty() == false)
    {
        QDomNodeList elements = this->documentElement().elementsByTagName( "draw" );
        if (elements.size() == 0)
        {
            return false;
        }
        for ( qint32 i = 0; i < elements.count(); i++ )
        {
            element = elements.at( i ).toElement();
            if (element.isNull() == false)
            {
                QString fieldName = element.attribute( "name" );
                if ( fieldName == nameActivDraw )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool VDomDocument::GetActivCalculationElement(QDomElement &element)
{
    bool ok = GetActivNodeElement("calculation", element);
    if (ok)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VDomDocument::GetActivModelingElement(QDomElement &element)
{
    bool ok = GetActivNodeElement("modeling", element);
    if (ok)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VDomDocument::GetActivDetailsElement(QDomElement &element)
{
    bool ok = GetActivNodeElement("details", element);
    if (ok)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VDomDocument::GetActivNodeElement(const QString& name, QDomElement &element)
{
    Q_ASSERT_X(name.isEmpty() == false, "GetActivNodeElement", "name draw is empty");
    QDomElement drawElement;
    bool drawOk = this->GetActivDrawElement(drawElement);
    if (drawOk == true)
    {
        QDomNodeList listElement = drawElement.elementsByTagName(name);
        if (listElement.size() == 0 || listElement.size() > 1)
        {
            return false;
        }
        element = listElement.at( 0 ).toElement();
        if (element.isNull() == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void VDomDocument::Parse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                         VMainGraphicsScene *sceneDetail)
{
    Q_CHECK_PTR(sceneDraw);
    Q_CHECK_PTR(sceneDetail);
    if (parse == Document::FullParse)
    {
        TestUniqueId();
        data->Clear();
        data->CreateManTableIGroup();
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
    data->ClearDetails();
    history.clear();
    QDomElement rootElement = this->documentElement();
    QDomNode domNode = rootElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName()=="draw")
                {
                    if (parse == Document::FullParse)
                    {
                        if (nameActivDraw.isEmpty())
                        {
                            SetActivDraw(domElement.attribute("name"));
                        }
                        else
                        {
                            ChangeActivDraw(domElement.attribute("name"));
                        }
                        comboBoxDraws->addItem(domElement.attribute("name"));
                    }
                    else
                    {
                        ChangeActivDraw(domElement.attribute("name"), Document::LiteParse);
                    }
                    ParseDrawElement(sceneDraw, sceneDetail, domElement, parse);
                }
                if (domElement.tagName()=="increments")
                {
                    ParseIncrementsElement(domElement);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

VDataTool *VDomDocument::getTool(const qint64 &id)
{
    if (tools.contains(id))
    {
        return tools.value(id);
    }
    else
    {
        QString error = QString(tr("Can't find tool id = %1 in table.")).arg(id);
        throw VException(error);
    }
    return 0;
}

void VDomDocument::ParseIncrementsElement(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == "increment")
                {
                    qint64 id = GetParametrId(domElement);
                    QString name = GetParametrString(domElement, "name", "");
                    qreal base = GetParametrDouble(domElement, "base", "0");
                    qreal ksize = GetParametrDouble(domElement, "ksize", "0");
                    qreal kgrowth = GetParametrDouble(domElement, "kgrowth", "0");
                    QString desc = GetParametrString(domElement, "description", "Description");
                    data->UpdateId(id);
                    data->AddIncrementTableRow(name, VIncrementTableRow(id, base, ksize, kgrowth, desc));
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

qint64 VDomDocument::GetParametrId(const QDomElement &domElement) const
{
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    qint64 id = GetParametrLongLong(domElement, "id", "0");
    if (id <= 0)
    {
        throw VExceptionWrongParameterId(tr("Got wrong parameter id. Need only id > 0."), domElement);
    }
    return id;
}

qint64 VDomDocument::GetParametrLongLong(const QDomElement &domElement, const QString &name,
                                         const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    QString parametr = GetParametrString(domElement, name, defValue);
    qint64 id = parametr.toLongLong(&ok);
    if (ok == false)
    {
        throw VExceptionConversionError(tr("Can't convert toLongLong parameter"), name);
    }
    return id;
}

QString VDomDocument::GetParametrString(const QDomElement &domElement, const QString &name,
                                        const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    QString parameter = domElement.attribute(name, defValue);
    if (parameter.isEmpty())
    {
        throw VExceptionEmptyParameter(tr("Got empty parameter"), name, domElement);
    }
    return parameter;
}

qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    QString parametr = GetParametrString(domElement, name, defValue);
    qreal param = parametr.replace(",", ".").toDouble(&ok);
    if (ok == false)
    {
        qDebug()<<"defValue"<<defValue;
        qDebug()<<"parametr"<<parametr;
        throw VExceptionConversionError(tr("Can't convert toDouble parameter"), name);
    }
    return param;
}

qint64 VDomDocument::SPointActiveDraw()
{
    QDomElement calcElement;
    bool ok = GetActivCalculationElement(calcElement);
    if (ok)
    {
        QDomNode domNode = calcElement.firstChild();
        if (domNode.isNull() == false)
        {
            if (domNode.isElement())
            {
                QDomElement domElement = domNode.toElement();
                if (domElement.isNull() == false)
                {
                    if (domElement.tagName() == VToolPoint::TagName &&
                            domElement.attribute("type", "") == VToolSinglePoint::ToolType)
                    {
                        return GetParametrId(domElement);
                    }
                }
            }
        }
    }
    return 0;
}

void VDomDocument::TestUniqueId() const
{
    QVector<qint64> vector;
    CollectId(this->documentElement(), vector);
}

void VDomDocument::CollectId(const QDomElement &node, QVector<qint64> &vector) const
{
    if (node.hasAttribute("id"))
    {
        qint64 id = GetParametrId(node);
        if (vector.contains(id))
        {
            throw VExceptionUniqueId(tr("This id is not unique."), node);
        }
        vector.append(id);
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i)
    {
        QDomNode n = node.childNodes().at(i);
        if (n.isElement())
        {
            CollectId(n.toElement(), vector);
        }
    }
}

void VDomDocument::ParseDrawElement(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, const Document::Documents &parse)
{
    QDomNode domNode = node.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == "calculation")
                {
                    data->ClearCalculationGObjects();
                    ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Draw::Calculation);
                }
                if (domElement.tagName() == "modeling")
                {
                    ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Draw::Modeling);
                }
                if (domElement.tagName() == "details")
                {
                    ParseDetails(sceneDetail, domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParseDrawMode(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
                                 const QDomNode& node, const Document::Documents &parse, const Draw::Draws &mode)
{
    Q_CHECK_PTR(sceneDraw);
    Q_CHECK_PTR(sceneDetail);
    VMainGraphicsScene *scene = 0;
    if (mode == Draw::Calculation)
    {
        scene = sceneDraw;
    }
    else
    {
        scene = sceneDetail;
    }
    QDomNodeList nodeList = node.childNodes();
    qint32 num = nodeList.size();
    for (qint32 i = 0; i < num; ++i)
    {
        QDomElement domElement = nodeList.at(i).toElement();
        if (domElement.isNull() == false)
        {
            if (domElement.tagName() == "point")
            {
                ParsePointElement(scene, domElement, parse, domElement.attribute("type", ""));
                continue;
            }
            if (domElement.tagName() == "line")
            {
                ParseLineElement(scene, domElement, parse);
                continue;
            }
            if (domElement.tagName() == "spline")
            {
                ParseSplineElement(scene, domElement, parse, domElement.attribute("type", ""));
                continue;
            }
            if (domElement.tagName() == "arc")
            {
                ParseArcElement(scene, domElement, parse, domElement.attribute("type", ""));
                continue;
            }
            if (domElement.tagName() == "tools")
            {
                ParseToolsElement(scene, domElement, parse, domElement.attribute("type", ""));
                continue;
            }
        }
    }
}

void VDomDocument::ParseDetailElement(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                                      const Document::Documents &parse)
{
    Q_CHECK_PTR(sceneDetail);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    try
    {
        VDetail detail;
        VDetail oldDetail;
        qint64 id = GetParametrId(domElement);
        detail.setName(GetParametrString(domElement, VAbstractTool::AttrName, ""));
        detail.setMx(toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "0.0")));
        detail.setMy(toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "0.0")));
        detail.setSeamAllowance(GetParametrLongLong(domElement, VToolDetail::AttrSupplement, "1"));
        detail.setWidth(GetParametrDouble(domElement, VToolDetail::AttrWidth, "10.0"));
        detail.setClosed(GetParametrLongLong(domElement, VToolDetail::AttrClosed, "1"));

        QDomNodeList nodeList = domElement.childNodes();
        qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            QDomElement element = nodeList.at(i).toElement();
            if (element.isNull() == false)
            {
                if (element.tagName() == VToolDetail::TagNode)
                {
                    qint64 id = GetParametrLongLong(element, VToolDetail::AttrIdObject, "0");
                    qreal mx = toPixel(GetParametrDouble(element, VAbstractTool::AttrMx, "0.0"));
                    qreal my = toPixel(GetParametrDouble(element, VAbstractTool::AttrMy, "0.0"));
                    Tool::Tools tool;
                    NodeDetail::NodeDetails nodeType = NodeDetail::Contour;
                    QString t = GetParametrString(element, "type", "NodePoint");
                    if (t == "NodePoint")
                    {
                        tool = Tool::NodePoint;
//                        const VPointF *point = data->GeometricObject<const VPointF *>(id);
//                        oldDetail.append(VNodeDetail(point.getIdObject(), tool, NodeDetail::Contour));
                    }
                    else if (t == "NodeArc")
                    {
                        tool = Tool::NodeArc;
//                        VArc arc = data->GetArc(id);
//                        oldDetail.append(VNodeDetail(arc.getIdObject(), tool, NodeDetail::Contour));
                    }
                    else if (t == "NodeSpline")
                    {
                        tool = Tool::NodeSpline;
//                        VSpline spl = data->GetSpline(id);
//                        oldDetail.append(VNodeDetail(spl.getIdObject(), tool, NodeDetail::Contour));
                    }
                    else if (t == "NodeSplinePath")
                    {
                        tool = Tool::NodeSplinePath;
//                        VSplinePath splPath = data->GetSplinePath(id);
//                        oldDetail.append(VNodeDetail(splPath.getIdObject(), tool, NodeDetail::Contour));
                    }
                    detail.append(VNodeDetail(id, tool, nodeType, mx, my));
                }
            }
        }
        VToolDetail::Create(id, detail, sceneDetail, this, data, parse, Tool::FromFile);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating detail"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

void VDomDocument::ParseDetails(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                                const Document::Documents &parse)
{
    Q_CHECK_PTR(sceneDetail);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    QDomNode domNode = domElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == VToolDetail::TagName)
                {
                    ParseDetailElement(sceneDetail, domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

void VDomDocument::ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString& type)
{
    Q_CHECK_PTR(scene);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of point is empty");
    if (type == VToolSinglePoint::ToolType)
    {
        VToolSinglePoint *spoint = 0;
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "A");
            qreal x = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrX, "10.0"));
            qreal y = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrY, "10.0"));
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));

            data->UpdateGObject(id, new VPointF(x, y, name, mx, my));
            VDrawTool::AddRecord(id, Tool::SinglePointTool, this);
            if (parse != Document::FullParse)
            {
                UpdateToolData(id, data);
            }
            if (parse == Document::FullParse)
            {
                spoint = new VToolSinglePoint(this, data, id, Tool::FromFile);
                Q_CHECK_PTR(spoint);
                scene->addItem(spoint);
                connect(spoint, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                connect(scene, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
                tools[id] = spoint;
            }
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating single point"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            scene->removeItem(spoint);
            delete spoint;
            throw excep;
        }
    }
    if (type == VToolEndLine::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
            qint64 basePointId = GetParametrLongLong(domElement, VAbstractTool::AttrBasePoint, "0");
            qreal angle = GetParametrDouble(domElement, VAbstractTool::AttrAngle, "0.0");

            VToolEndLine::Create(id, name, typeLine, formula, angle, basePointId, mx, my, scene, this, data, parse,
                                 Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of end line"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolAlongLine::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");

            VToolAlongLine::Create(id, name, typeLine, formula, firstPointId, secondPointId, mx, my, scene, this, data,
                                   parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point along line"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolShoulderPoint::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
            qint64 p1Line = GetParametrLongLong(domElement, VAbstractTool::AttrP1Line, "0");
            qint64 p2Line = GetParametrLongLong(domElement, VAbstractTool::AttrP2Line, "0");
            qint64 pShoulder = GetParametrLongLong(domElement, VAbstractTool::AttrPShoulder, "0");

            VToolShoulderPoint::Create(id, formula, p1Line, p2Line, pShoulder, typeLine, name, mx, my, scene, this,
                                       data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of shoulder"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolNormal::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            qreal angle = GetParametrDouble(domElement, VAbstractTool::AttrAngle, "0.0");

            VToolNormal::Create(id, formula, firstPointId, secondPointId, typeLine, name, angle, mx, my, scene, this,
                                data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of normal"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolBisector::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            qint64 thirdPointId = GetParametrLongLong(domElement, VAbstractTool::AttrThirdPoint, "0");

            VToolBisector::Create(id, formula, firstPointId, secondPointId, thirdPointId, typeLine, name, mx, my, scene,
                                  this, data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of bisector"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolLineIntersect::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            qint64 p1Line1Id = GetParametrLongLong(domElement, VAbstractTool::AttrP1Line1, "0");
            qint64 p2Line1Id = GetParametrLongLong(domElement, VAbstractTool::AttrP2Line1, "0");
            qint64 p1Line2Id = GetParametrLongLong(domElement, VAbstractTool::AttrP1Line2, "0");
            qint64 p2Line2Id = GetParametrLongLong(domElement, VAbstractTool::AttrP2Line2, "0");

            VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name, mx, my, scene, this, data,
                                       parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of lineintersection"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolPointOfContact::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "0");
            qint64 center = GetParametrLongLong(domElement, VAbstractTool::AttrCenter, "0");
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");

            VToolPointOfContact::Create(id, radius, center, firstPointId, secondPointId, name, mx, my, scene, this,
                                        data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of contact"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VNodePoint::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 idObject = GetParametrLongLong(domElement, VAbstractNode::AttrIdObject, "0");
            qint64 idTool = GetParametrLongLong(domElement, VAbstractNode::AttrIdTool, "0");
            const VPointF *point = data->GeometricObject<const VPointF *>(idObject );
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            data->UpdateGObject(id, new VPointF(point->x(), point->y(), point->name(), mx, my, idObject,
                                                Draw::Modeling));
            VNodePoint::Create(this, data, id, idObject, parse, Tool::FromFile, idTool);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating modeling point"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolHeight::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
            qint64 basePointId = GetParametrLongLong(domElement, VAbstractTool::AttrBasePoint, "0");
            qint64 p1LineId = GetParametrLongLong(domElement, VAbstractTool::AttrP1Line, "0");
            qint64 p2LineId = GetParametrLongLong(domElement, VAbstractTool::AttrP2Line, "0");

            VToolHeight::Create(id, name, typeLine, basePointId, p1LineId, p2LineId, mx, my, scene, this, data, parse,
                                Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating height"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolTriangle::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            qint64 axisP1Id = GetParametrLongLong(domElement, VAbstractTool::AttrAxisP1, "0");
            qint64 axisP2Id = GetParametrLongLong(domElement, VAbstractTool::AttrAxisP2, "0");
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");

            VToolTriangle::Create(id, name, axisP1Id, axisP2Id, firstPointId, secondPointId, mx, my, scene, this, data,
                                  parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating triangle"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolPointOfIntersection::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            qint64 firstPointId = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
            qint64 secondPointId = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");

            VToolPointOfIntersection::Create(id, name, firstPointId, secondPointId, mx, my, scene, this, data, parse,
                                             Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating point of intersection"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolCutSpline::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
            qint64 splineId = GetParametrLongLong(domElement, VToolCutSpline::AttrSpline, "0");

            VToolCutSpline::Create(id, name, formula, splineId, mx, my, scene, this, data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating cut spline point"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolCutSplinePath::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
            qint64 splinePathId = GetParametrLongLong(domElement, VToolCutSplinePath::AttrSplinePath, "0");

            VToolCutSplinePath::Create(id, name, formula, splinePathId, mx, my, scene, this, data, parse,
                                       Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating cut spline path point"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolCutArc::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
            qreal mx = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
            qreal my = toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
            QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
            qint64 arcId = GetParametrLongLong(domElement, VToolCutArc::AttrArc, "0");

            VToolCutArc::Create(id, name, formula, arcId, mx, my, scene, this, data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating cut arc point"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
}

void VDomDocument::ParseLineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                    const Document::Documents &parse)
{
    Q_CHECK_PTR(scene);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    try
    {
        qint64 id = GetParametrId(domElement);
        qint64 firstPoint = GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
        qint64 secondPoint = GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");

        VToolLine::Create(id, firstPoint, secondPoint, scene, this, data, parse, Tool::FromFile);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating line"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

void VDomDocument::ParseSplineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                      const Document::Documents &parse, const QString &type)
{
    Q_CHECK_PTR(scene);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");
    if (type == VToolSpline::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 point1 = GetParametrLongLong(domElement, VAbstractTool::AttrPoint1, "0");
            qint64 point4 = GetParametrLongLong(domElement, VAbstractTool::AttrPoint4, "0");
            qreal angle1 = GetParametrDouble(domElement, VAbstractTool::AttrAngle1, "270.0");
            qreal angle2 = GetParametrDouble(domElement, VAbstractTool::AttrAngle2, "90.0");
            qreal kAsm1 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm1, "1.0");
            qreal kAsm2 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm2, "1.0");
            qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");

            VToolSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, this, data, parse,
                                Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating simple curve"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VToolSplinePath::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");
            VSplinePath *path = new VSplinePath(kCurve);
            Q_CHECK_PTR(path);

            QDomNodeList nodeList = domElement.childNodes();
            qint32 num = nodeList.size();
            for (qint32 i = 0; i < num; ++i)
            {
                QDomElement element = nodeList.at(i).toElement();
                if (element.isNull() == false)
                {
                    if (element.tagName() == VAbstractTool::AttrPathPoint)
                    {
                        qreal kAsm1 = GetParametrDouble(element, VAbstractTool::AttrKAsm1, "1.0");
                        qreal angle = GetParametrDouble(element, VAbstractTool::AttrAngle, "0");
                        qreal kAsm2 = GetParametrDouble(element, VAbstractTool::AttrKAsm2, "1.0");
                        qint64 pSpline = GetParametrLongLong(element, VAbstractTool::AttrPSpline, "0");
                        VPointF p = *data->GeometricObject<const VPointF *>(pSpline);

                        VSplinePoint splPoint(p, kAsm1, angle, kAsm2);
                        path->append(splPoint);
                        if (parse == Document::FullParse)
                        {
                            IncrementReferens(pSpline);
                        }
                    }
                }
            }

            VToolSplinePath::Create(id, path, scene, this, data, parse, Tool::FromFile);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating curve path"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VNodeSpline::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 idObject = GetParametrLongLong(domElement, VAbstractNode::AttrIdObject, "0");
            qint64 idTool = GetParametrLongLong(domElement, VAbstractNode::AttrIdTool, "0");
            VSpline *spl = new VSpline(*data->GeometricObject<const VSpline *>(idObject));
            Q_CHECK_PTR(spl);
            spl->setIdObject(idObject);
            spl->setMode(Draw::Modeling);
            data->UpdateGObject(id, spl);
            VNodeSpline::Create(this, data, id, idObject, parse, Tool::FromFile, idTool);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating modeling simple curve"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VNodeSplinePath::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 idObject = GetParametrLongLong(domElement, VAbstractNode::AttrIdObject, "0");
            qint64 idTool = GetParametrLongLong(domElement, VAbstractNode::AttrIdTool, "0");
            VSplinePath *path = new VSplinePath(*data->GeometricObject<const VSplinePath *>(idObject));
            Q_CHECK_PTR(path);
            path->setIdObject(idObject);
            path->setMode(Draw::Modeling);
            data->UpdateGObject(id, path);
            VNodeSplinePath::Create(this, data, id, idObject, parse, Tool::FromFile, idTool);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating modeling curve path"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
}

void VDomDocument::ParseArcElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                   const Document::Documents &parse, const QString &type)
{
    Q_CHECK_PTR(scene);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");
    if (type == VToolArc::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 center = GetParametrLongLong(domElement, VAbstractTool::AttrCenter, "0");
            QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "10");
            QString f1 = GetParametrString(domElement, VAbstractTool::AttrAngle1, "180");
            QString f2 = GetParametrString(domElement, VAbstractTool::AttrAngle2, "270");

            VToolArc::Create(id, center, radius, f1, f2, scene, this, data, parse, Tool::FromFile);

            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating simple arc"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
    if (type == VNodeArc::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 idObject = GetParametrLongLong(domElement, VAbstractNode::AttrIdObject, "0");
            qint64 idTool = GetParametrLongLong(domElement, VAbstractNode::AttrIdTool, "0");
            VArc *arc = new VArc(*data->GeometricObject<const VArc *>(idObject));
            Q_CHECK_PTR(arc);
            arc->setIdObject(idObject);
            arc->setMode(Draw::Modeling);
            data->UpdateGObject(id, arc);
            VNodeArc::Create(this, data, id, idObject, parse, Tool::FromFile, idTool);
            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating modeling arc"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
}

void VDomDocument::ParseToolsElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                     const Document::Documents &parse, const QString &type)
{
    Q_CHECK_PTR(scene);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");
    if (type == VToolUnionDetails::ToolType)
    {
        try
        {
            qint64 id = GetParametrId(domElement);
            qint64 indexD1 = GetParametrLongLong(domElement, VToolUnionDetails::AttrIndexD1, "-1");
            qint64 indexD2 = GetParametrLongLong(domElement, VToolUnionDetails::AttrIndexD2, "-1");

            QVector<VDetail> vector = VToolUnionDetails::GetDetailFromFile(this, domElement);

            VToolUnionDetails::Create(id, vector[0], vector[1], 0, 0, indexD1, indexD2, scene, this, data, parse,
                                      Tool::FromFile);

            return;
        }
        catch (const VExceptionBadId &e)
        {
            VExceptionObjectError excep(tr("Error creating or updating union details"), domElement);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }
    }
}

void VDomDocument::FullUpdateTree()
{
    VMainGraphicsScene *scene = new VMainGraphicsScene();
    Q_CHECK_PTR(scene);
    try
    {
        Parse(Document::LiteParse, scene, scene);
    }
    catch (const std::bad_alloc &)
    {
        delete scene;
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error parsing file."));
        msgBox.setInformativeText("std::bad_alloc");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    catch (...)
    {
        delete scene;
        throw;
    }

    delete scene;
    setCurrentData();
    emit FullUpdateFromFile();
    emit haveChange();
}

void VDomDocument::haveLiteChange()
{
    emit haveChange();
}

void VDomDocument::ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable)
{
    emit ShowTool(id, color, enable);
}

void VDomDocument::setCursor(const qint64 &value)
{
    cursor = value;
    emit ChangedCursor(cursor);
}

void VDomDocument::setCurrentData()
{
    if (*mode == Draw::Calculation)
    {
        QString nameDraw = comboBoxDraws->itemText(comboBoxDraws->currentIndex());
        if (nameActivDraw != nameDraw)
        {
            nameActivDraw = nameDraw;
            qint64 id = 0;
            if (history.size() == 0)
            {
                return;
            }
            for (qint32 i = 0; i < history.size(); ++i)
            {
                VToolRecord tool = history.at(i);
                if (tool.getNameDraw() == nameDraw)
                {
                    id = tool.getId();
                }
            }
            if (id == 0)
            {
                VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                if (id == 0)
                {
                    return;
                }
            }
            if (tools.size() > 0)
            {
                VDataTool *vTool = tools.value(id);
                data->setData(vTool->getData());
            }
        }
    }
}

void VDomDocument::AddTool(const qint64 &id, VDataTool *tool)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    Q_CHECK_PTR(tool);
    tools.insert(id, tool);
}

void VDomDocument::UpdateToolData(const qint64 &id, VContainer *data)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    Q_CHECK_PTR(data);
    VDataTool *tool = tools.value(id);
    Q_CHECK_PTR(tool);
    tool->VDataTool::setData(data);
}

void VDomDocument::IncrementReferens(qint64 id) const
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    VDataTool *tool = tools.value(id);
    Q_CHECK_PTR(tool);
    tool->incrementReferens();
}

void VDomDocument::DecrementReferens(qint64 id) const
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    VDataTool *tool = tools.value(id);
    if (tool != 0)
    {
        tool->decrementReferens();
    }
}
