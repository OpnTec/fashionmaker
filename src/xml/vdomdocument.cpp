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
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionbadid.h"

#include <QAbstractMessageHandler>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QFile>

//This class need for validation pattern file using XSD shema
class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler() : QAbstractMessageHandler(), m_messageType(QtMsgType()), m_description(QString()),
        m_sourceLocation(QSourceLocation()){}
    QString statusMessage() const;
    qint64  line() const;
    qint64  column() const;
protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        Q_UNUSED(type);
        Q_UNUSED(identifier);

        m_messageType = type;
        m_description = description;
        m_sourceLocation = sourceLocation;
    }
private:
    QtMsgType       m_messageType;
    QString         m_description;
    QSourceLocation m_sourceLocation;
};

inline QString MessageHandler::statusMessage() const
{
    return m_description;
}

inline qint64  MessageHandler::line() const
{
    return m_sourceLocation.line();
}

inline qint64  MessageHandler::column() const
{
    return m_sourceLocation.column();
}

VDomDocument::VDomDocument(VContainer *data)
    : QDomDocument(), data(data), map(QHash<QString, QDomElement>())
{

}

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

    if (this->find(this->documentElement(), id))
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
        const QString value = node.attribute("id");
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
            if (this->find(n.toElement(), id))
            {
                return true;
            }
        }
    }
    return false;
}

quint32 VDomDocument::GetParametrUInt(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    const QString parametr = GetParametrString(domElement, name, defValue);
    const quint32 id = parametr.toUInt(&ok);
    if (ok == false)
    {
        throw VExceptionConversionError(tr("Can't convert toUInt parameter"), name);
    }
    return id;
}

QString VDomDocument::GetParametrString(const QDomElement &domElement, const QString &name,
                                        const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    const QString parameter = domElement.attribute(name, defValue);
    if (parameter.isEmpty())
    {
        if (defValue.isEmpty())
        {
            throw VExceptionEmptyParameter(tr("Got empty parameter"), name, domElement);
        }
        else
        {
            return defValue;
        }
    }
    return parameter;
}

qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    QString parametr = GetParametrString(domElement, name, defValue);
    const qreal param = parametr.replace(",", ".").toDouble(&ok);
    if (ok == false)
    {
        throw VExceptionConversionError(tr("Can't convert toDouble parameter"), name);
    }
    return param;
}

QString VDomDocument::UniqueTagText(const QString &tagName, const QString &defVal) const
{
    const QDomNodeList nodeList = this->elementsByTagName(tagName);
    if (nodeList.isEmpty())
    {
        return defVal;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                return domElement.text();
            }
        }
    }
    return defVal;
}

bool VDomDocument::ValidatePattern(const QString &schema, const QString &fileName, QString &errorMsg, qint64 &errorLine,
                                   qint64 &errorColumn)
{
    errorLine = -1;
    errorColumn = -1;
    QFile pattern(fileName);
    if (pattern.open(QIODevice::ReadOnly) == false)
    {
        errorMsg = QString(tr("Can't open file %1:\n%2.").arg(fileName).arg(pattern.errorString()));
        return false;
    }
    QFile fileSchema(schema);
    if (fileSchema.open(QIODevice::ReadOnly) == false)
    {
        errorMsg = QString(tr("Can't open schema file %1:\n%2.").arg(schema).arg(fileSchema.errorString()));
        return false;
    }

    MessageHandler messageHandler;
    QXmlSchema sch;
    sch.setMessageHandler(&messageHandler);
    sch.load(&fileSchema, QUrl::fromLocalFile(fileSchema.fileName()));

    bool errorOccurred = false;
    if (sch.isValid() == false)
    {
        errorOccurred = true;
    }
    else
    {
        QXmlSchemaValidator validator(sch);
        if (validator.validate(&pattern, QUrl::fromLocalFile(pattern.fileName())) == false)
        {
            errorOccurred = true;
        }
    }

    if (errorOccurred)
    {
        errorMsg = messageHandler.statusMessage();
        errorLine = messageHandler.line();
        errorColumn = messageHandler.column();
        return false;
    }
    else
    {
        return true;
    }
}

Valentina::Units VDomDocument::Units(const QString &unit)
{
    QStringList units;
    units << "mm" << "cm" << "in";
    Valentina::Units result = Valentina::Cm;
    switch (units.indexOf(unit))
    {
        case 0:// mm
            result = Valentina::Mm;
            break;
        case 1:// cm
            result = Valentina::Cm;
            break;
        case 2:// in
            result = Valentina::In;
            break;
        default:
            result = Valentina::Cm;
            break;
    }
    return result;
}
