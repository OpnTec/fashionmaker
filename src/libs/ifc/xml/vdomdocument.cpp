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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionbadid.h"
#include "exception/vexceptionwrongid.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/backport/qsavefile.h"
#else
#   include <QSaveFile>
#endif

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
                               const QUrl &identifier, const QSourceLocation &sourceLocation) Q_DECL_OVERRIDE;
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
// cppcheck-suppress unusedFunction
void MessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier,
                                   const QSourceLocation &sourceLocation)
{
    Q_UNUSED(type);
    Q_UNUSED(identifier);

    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
}

Q_LOGGING_CATEGORY(vXML, "v.xml")

const QString VDomDocument::AttrId     = QStringLiteral("id");
const QString VDomDocument::UnitMM     = QStringLiteral("mm");
const QString VDomDocument::UnitCM     = QStringLiteral("cm");
const QString VDomDocument::UnitINCH   = QStringLiteral("inch");
const QString VDomDocument::UnitPX     = QStringLiteral("px");
const QString VDomDocument::TagVersion = QStringLiteral("version");

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::VDomDocument()
    : QDomDocument(), map(QHash<QString, QDomElement>())
{}

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::~VDomDocument()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Finds an element by id.
 * @param id value id attribute.
 * @return dom element.
 */
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
QDomElement VDomDocument::elementById(quint32 id)
{
    return elementById(QString().setNum(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Find element by id.
 * @param node node
 * @param id id value
 * @return true if found
 */
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
/**
 * @brief Returns the long long value of the given attribute. RENAME: GetParameterLongLong?
 * @param domElement tag in xml tree
 * @param name attribute name
 * @return long long value
 */
quint32 VDomDocument::GetParametrUInt(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null"); //-V591

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
bool VDomDocument::GetParametrBool(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    QString parametr;
    bool val = true;

    QString message = tr("Can't convert toBool parameter");
    try
    {
        parametr = GetParametrString(domElement, name, defValue);

        QStringList bools = QStringList() << QLatin1String("true") << QLatin1String("false");
        switch (bools.indexOf(parametr))
        {
            case 0: // true
                val = true;
                break;
            case 1: // false
                val = false;
                break;
            default:// others
                throw VExceptionConversionError(message, name);
                break;
        }
    }
    catch (const VExceptionEmptyParameter &e)
    {
        VExceptionConversionError excep(message, name);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }

    return val;
}

//---------------------------------------------------------------------------------------------------------------------
NodeUsage VDomDocument::GetParametrUsage(const QDomElement &domElement, const QString &name) const
{
    const bool value = GetParametrBool(domElement, name, QStringLiteral("true"));
    if (value)
    {
        return NodeUsage::InUse;
    }
    else
    {
        return NodeUsage::NotInUse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::SetParametrUsage(QDomElement &domElement, const QString &name, const NodeUsage &value)
{
    if (value == NodeUsage::InUse)
    {
        domElement.setAttribute(name, QStringLiteral("true"));
    }
    else
    {
        domElement.setAttribute(name, QStringLiteral("false"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the string value of the given attribute. RENAME: see above
 *
 * if attribute empty return default value. If default value empty too throw exception.
 * @return attribute value
 * @throw VExceptionEmptyParameter when attribute is empty
 */
QString VDomDocument::GetParametrString(const QDomElement &domElement, const QString &name,
                                        const QString &defValue) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
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
/**
 * @brief Returns the double value of the given attribute.
 * @param domElement tag in xml tree
 * @param name attribute name
 * @return double value
 */
qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

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
/**
 * @brief GetParametrId return value id attribute.
 * @param domElement tag in xml tree.
 * @return id value.
 */
quint32 VDomDocument::GetParametrId(const QDomElement &domElement) const
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    quint32 id = 0;

    QString message = tr("Got wrong parameter id. Need only id > 0.");
    try
    {
        id = GetParametrUInt(domElement, VDomDocument::AttrId, NULL_ID_STR);
        if (id <= 0)
        {
            throw VExceptionWrongId(message, domElement);
        }
    }
    catch (const VExceptionConversionError &e)
    {
        VExceptionWrongId excep(message, domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    return id;
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
                const QString text = domElement.text();
                if (text.isEmpty())
                {
                    return defVal;
                }
                else
                {
                    return text;
                }
            }
        }
    }
    return defVal;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TestUniqueId test exist unique id in pattern file. Each id must be unique.
 */
void VDomDocument::TestUniqueId() const
{
    QVector<quint32> vector;
    CollectId(documentElement(), vector);
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::CollectId(const QDomElement &node, QVector<quint32> &vector) const
{
    if (node.hasAttribute(VDomDocument::AttrId))
    {
        const quint32 id = GetParametrId(node);
        if (vector.contains(id))
        {
            throw VExceptionWrongId(tr("This id is not unique."), node);
        }
        vector.append(id);
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i)
    {
        const QDomNode n = node.childNodes().at(i);
        if (n.isElement())
        {
            CollectId(n.toElement(), vector);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValidateXML validate xml file by xsd schema.
 * @param schema path to schema file.
 * @param fileName name of xml file.
 */
void VDomDocument::ValidateXML(const QString &schema, const QString &fileName)
{
    qCDebug(vXML, "Validation xml file %s.", qUtf8Printable(fileName));
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
    if (sch.load(&fileSchema, QUrl::fromLocalFile(fileSchema.fileName()))==false)
    {
        pattern.close();
        fileSchema.close();
        const QString errorMsg(tr("Could not load schema file '%1'.").arg(fileSchema.fileName()));
        throw VException(errorMsg);
    }
    qCDebug(vXML, "Schema loaded.");

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
        e.AddMoreInformation(tr("Validation error file %3 in line %1 column %2").arg(messageHandler.line())
                             .arg(messageHandler.column()).arg(fileName));
        throw e;
    }
    pattern.close();
    fileSchema.close();
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::setXMLContent(const QString &fileName)
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
        e.AddMoreInformation(tr("Parsing error file %3 in line %1 column %2").arg(errorLine).arg(errorColumn)
                             .arg(fileName));
        throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Unit VDomDocument::StrToUnits(const QString &unit)
{
    QStringList units = QStringList() << UnitMM << UnitCM << UnitINCH << UnitPX;
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
        case 3:// px
            result = Unit::Px;
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
                result = UnitMM;
            }
            break;
        case Unit::Cm:
            if (translate)
            {
                result = QObject::tr("cm");
            }
            else
            {
                result = UnitCM;
            }
            break;
        case Unit::Inch:
            if (translate)
            {
                result = QObject::tr("inch");
            }
            else
            {
                result = UnitINCH;
            }
            break;
        case Unit::Px:
            if (translate)
            {
                result = QObject::tr("px");
            }
            else
            {
                result = UnitPX;
            }
            break;
        default:
            if (translate)
            {
                result = QObject::tr("cm");
            }
            else
            {
                result = UnitCM;
            }
            break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::UnitsHelpString()
{
    QString r;
    for (auto i = static_cast<int>(Unit::Mm), last = static_cast<int>(Unit::LAST_UNIT_DO_NOT_USE); i < last;++i)
    {
        r += UnitsToStr(static_cast<Unit>(i));
        if (i < last - 1)
        {
            r += ", ";
        }
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::SaveDocument(const QString &fileName, QString &error) const
{
    if (fileName.isEmpty())
    {
        qDebug()<<"Got empty file name.";
        return false;
    }
    bool success = false;
    QSaveFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        const int indent = 4;
        QTextStream out(&file);
        out.setCodec("UTF-8");
        save(out, indent);
        success = file.commit();
    }

    if (not success)
    {
        error = file.errorString();
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VDomDocument::Major() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(0);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VDomDocument::Minor() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(1);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VDomDocument::Patch() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(".");
    return v.at(2);
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::setTagText(const QString &tag, const QString &text)
{
    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        qDebug()<<"Can't save tag "<<tag<<Q_FUNC_INFO;
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
                if (not text.isEmpty())
                {
                    const QDomText newTagText = createTextNode(text);
                    newTag.appendChild(newTagText);
                }

                parent.replaceChild(newTag, domElement);
                return true;
            }
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveAllChildren remove all children from file.
 * @param domElement tag in xml tree.
 */
void VDomDocument::RemoveAllChildren(QDomElement &domElement)
{
    if ( domElement.hasChildNodes() )
    {
        while ( domElement.childNodes().length() >= 1 )
        {
            domElement.removeChild( domElement.firstChild() );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomNode VDomDocument::ParentNodeById(const quint32 &nodeId)
{
    QDomElement domElement = NodeById(nodeId);
    return domElement.parentNode();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VDomDocument::CloneNodeById(const quint32 &nodeId)
{
    QDomElement domElement = NodeById(nodeId);
    return domElement.cloneNode().toElement();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VDomDocument::NodeById(const quint32 &nodeId)
{
    QDomElement domElement = elementById(QString().setNum(nodeId));
    if (domElement.isNull() || domElement.isElement() == false)
    {
        throw VExceptionBadId(tr("Couldn't get node"), nodeId);
    }
    return domElement;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::SafeCopy(const QString &source, const QString &destination, QString &error)
{
    bool result = false;

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/

    QTemporaryFile destFile(destination + QLatin1Literal(".XXXXXX"));
    destFile.setAutoRemove(false);
    if (not destFile.open())
    {
        error = destFile.errorString();
        result = false;
    }
    else
    {
        QFile sourceFile(source);
        if (sourceFile.open(QIODevice::ReadOnly))
        {
            result = true;
            char block[4096];
            qint64 bytes;
            while ((bytes = sourceFile.read(block, sizeof(block))) > 0)
            {
                if (bytes != destFile.write(block, bytes))
                {
                    error = destFile.errorString();
                    result = false;
                    break;
                }
            }

            if (bytes == -1)
            {
                error = sourceFile.errorString();
                result = false;
            }

            if (result)
            {
                QFile::remove(destination);
                if (not destFile.rename(destination))
                {
                    error = destFile.errorString();
                    result = false;
                }
                else
                {
                    result = true;
                }
            }
        }
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn off check permission again
#endif /*Q_OS_WIN32*/

    return result;
}
