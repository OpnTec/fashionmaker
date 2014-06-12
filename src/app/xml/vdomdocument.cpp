/************************************************************************
 **
 **  @file   vdomdocument.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include <QFileInfo>
#include <QTemporaryFile>

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
    // cppcheck-suppress unusedFunction
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation);
private:
    QtMsgType       m_messageType;
    QString         m_description;
    QSourceLocation m_sourceLocation;
};

//---------------------------------------------------------------------------------------------------------------------
inline QString MessageHandler::statusMessage() const
{
    return m_description;
}

//---------------------------------------------------------------------------------------------------------------------
inline qint64  MessageHandler::line() const
{
    return m_sourceLocation.line();
}

//---------------------------------------------------------------------------------------------------------------------
inline qint64  MessageHandler::column() const
{
    return m_sourceLocation.column();
}

//---------------------------------------------------------------------------------------------------------------------
void MessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier,
                                   const QSourceLocation &sourceLocation)
{
    Q_UNUSED(type);
    Q_UNUSED(identifier);

    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
}

const QString VDomDocument::AttrId     = QStringLiteral("id");
const QString VDomDocument::AttrUnit   = QStringLiteral("unit");
const QString VDomDocument::UnitMM     = QStringLiteral("mm");
const QString VDomDocument::UnitCM     = QStringLiteral("cm");
const QString VDomDocument::UnitINCH   = QStringLiteral("inch");
const QString VDomDocument::TagVersion = QStringLiteral("version");

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::VDomDocument(VContainer *data)
    : QDomDocument(), data(data), map(QHash<QString, QDomElement>())
{}

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::~VDomDocument()
{}

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::find(const QDomElement &node, const QString& id)
{
    if (node.hasAttribute(AttrId))
    {
        const QString value = node.attribute(AttrId);
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

//---------------------------------------------------------------------------------------------------------------------
quint32 VDomDocument::GetParametrUInt(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    bool ok = false;
    QString parametr;
    quint32 id = 0;

    QString message = tr("Can't convert toUInt parameter");
    try
    {
        parametr = GetParametrString(domElement, name, defValue);
        id = parametr.toUInt(&ok);
        if (ok == false)
        {
            throw VExceptionConversionError(message, name);
        }
    }
    catch (const VExceptionEmptyParameter &e)
    {
        VExceptionConversionError excep(message, name);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }

    return id;
}

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    bool ok = false;
    qreal param = 0;

    QString message = tr("Can't convert toDouble parameter");
    try
    {
        QString parametr = GetParametrString(domElement, name, defValue);
        param = parametr.replace(",", ".").toDouble(&ok);
        if (ok == false)
        {
            throw VExceptionConversionError(message, name);
        }
    }
    catch (const VExceptionEmptyParameter &e)
    {
        VExceptionConversionError excep(message, name);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    return param;
}

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::ValidateXML(const QString &schema, const QString &fileName)
{
    QFile pattern(fileName);
    if (pattern.open(QIODevice::ReadOnly) == false)
    {
        const QString errorMsg(tr("Can't open file %1:\n%2.").arg(fileName).arg(pattern.errorString()));
        throw VException(errorMsg);
    }

    QFile fileSchema(schema);
    if (fileSchema.open(QIODevice::ReadOnly) == false)
    {
        pattern.close();
        const QString errorMsg(tr("Can't open schema file %1:\n%2.").arg(schema).arg(fileSchema.errorString()));
        throw VException(errorMsg);
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
        pattern.close();
        fileSchema.close();
        VException e(messageHandler.statusMessage());
        e.AddMoreInformation(tr("Validation error in line %1 column %2").arg(messageHandler.line())
                             .arg(messageHandler.column()));
        throw e;
    }
    pattern.close();
    fileSchema.close();
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::setContent(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly) == false)
    {
        const QString errorMsg(tr("Can't open file %1:\n%2.").arg(fileName).arg(file.errorString()));
        throw VException(errorMsg);
    }

    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    if (QDomDocument::setContent(&file, &errorMsg, &errorLine, &errorColumn) == false)
    {
        file.close();
        VException e(errorMsg);
        e.AddMoreInformation(tr("Parcing error in line %1 column %2").arg(errorLine).arg(errorColumn));
        throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Unit VDomDocument::StrToUnits(const QString &unit)
{
    QStringList units{UnitMM, UnitCM, UnitINCH};
    Unit result = Unit::Cm;
    switch (units.indexOf(unit))
    {
        case 0:// mm
            result = Unit::Mm;
            break;
        case 1:// cm
            result = Unit::Cm;
            break;
        case 2:// inch
            result = Unit::Inch;
            break;
        default:
            result = Unit::Cm;
            break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UnitsToStr translate unit to string.
 *
 * This method used when need write unit in xml file and for showing unit in dialogs.
 * @param unit curent unit
 * @param translate true if need show translated name. Default value false.
 * @return string reprezantation for unit.
 */
QString VDomDocument::UnitsToStr(const Unit &unit, const bool translate)
{
    QString result;
    switch (unit)
    {
        case Unit::Mm:
            if (translate)
            {
                result = QObject::tr("mm");
            }
            else
            {
                result = "mm";
            }
            break;
        case Unit::Cm:
            if (translate)
            {
                result = QObject::tr("cm");
            }
            else
            {
                result = "cm";
            }
            break;
        case Unit::Inch:
            if (translate)
            {
                result = QObject::tr("in", "inch abbreviation");
            }
            else
            {
                result = "inch";//I decided use full name in xml file.
            }
            break;
        default:
            if (translate)
            {
                result = QObject::tr("cm");
            }
            else
            {
                result = "cm";
            }
            break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::SaveDocument(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        qDebug()<<"Got empty file name.";
        return false;
    }
    //Writing in temporary file
    QFileInfo tempInfo(fileName);
    QString temp = tempInfo.absolutePath() + "/" + tempInfo.baseName() + ".tmp";
    QFile tempFile(temp);
    if (tempFile.open(QIODevice::WriteOnly| QIODevice::Truncate))
    {
        const int indent = 4;
        QTextStream out(&tempFile);
        out.setCodec("UTF-8");
        save(out, indent);
        tempFile.close();
    }
    //Replace temp file our
    bool result = false;
    QFile patternFile(fileName);
    // We need here temporary file because we need restore document after error of copying temp file.
    QTemporaryFile tempOfPattern;
    if (tempOfPattern.open())
    {
        patternFile.copy(tempOfPattern.fileName());
    }
    if ( patternFile.exists() == false || patternFile.remove() )
    {
        if ( tempFile.copy(patternFile.fileName()) == false )
        {
            qDebug()<<"Could not copy temp file to document file"<<Q_FUNC_INFO;
            tempOfPattern.copy(fileName);
            result = false;
        }
        else
        {
            tempFile.remove();
            result = true;
        }
    }
    else
    {
        qDebug()<<"Could not remove document file"<<Q_FUNC_INFO;
        result = false;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::Major() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(0);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::Minor() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(1);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::Patch() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(2);
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::setTagText(const QString &tag, const QString &text)
{
    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        qDebug()<<"Can't save tag "<<tag<<Q_FUNC_INFO;
        return;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                QDomElement parent = domElement.parentNode().toElement();
                QDomElement newTag = createElement(tag);
                const QDomText newTagText = createTextNode(text);
                newTag.appendChild(newTagText);

                parent.replaceChild(newTag, domElement);
                return;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveAllChild remove all child from file.
 * @param domElement tag in xml tree.
 */
void VDomDocument::RemoveAllChild(QDomElement &domElement)
{
    if ( domElement.hasChildNodes() )
    {
        while ( domElement.childNodes().length() >= 1 )
        {
            domElement.removeChild( domElement.firstChild() );
        }
    }
}
