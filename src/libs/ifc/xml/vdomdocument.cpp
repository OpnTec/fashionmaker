/************************************************************************
 **
 **  @file   vdomdocument.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include <qcompilerdetection.h>
#include <qdom.h>
#include <QSaveFile>

#include "../exception/vexceptionbadid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionwrongid.h"
#include "../exception/vexception.h"
#include "../ifcdef.h"

#include <QAbstractMessageHandler>
#include <QByteArray>
#include <QDomNodeList>
#include <QDomText>
#include <QFile>
#include <QIODevice>
#include <QMessageLogger>
#include <QObject>
#include <QSourceLocation>
#include <QStringList>
#include <QTemporaryFile>
#include <QTextDocument>
#include <QTextStream>
#include <QUrl>
#include <QVector>
#include <QtDebug>
#include <QXmlStreamWriter>
#include <QTimer>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QRegularExpression>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
void SaveNodeCanonically(QXmlStreamWriter &stream, const QDomNode &domNode)
{
    if (stream.hasError())
    {
       return;
    }

    if (domNode.isElement())
    {
        const QDomElement domElement = domNode.toElement();
        if (not domElement.isNull())
        {
            stream.writeStartElement(domElement.tagName());

            if (domElement.hasAttributes())
            {
                QMap<QString, QString> attributes;
                const QDomNamedNodeMap attributeMap = domElement.attributes();
                for (int i = 0; i < attributeMap.count(); ++i)
                {
                    const QDomNode attribute = attributeMap.item(i);
                    attributes.insert(attribute.nodeName(), attribute.nodeValue());
                }

                QMap<QString, QString>::const_iterator i = attributes.constBegin();
                while (i != attributes.constEnd())
                {
                    stream.writeAttribute(i.key(), i.value());
                    ++i;
                }
            }

            if (domElement.hasChildNodes())
            {
                QDomNode elementChild = domElement.firstChild();
                while (not elementChild.isNull())
                {
                    SaveNodeCanonically(stream, elementChild);
                    elementChild = elementChild.nextSibling();
                }
            }

            stream.writeEndElement();
        }
    }
    else if (domNode.isComment())
    {
        stream.writeComment(domNode.nodeValue());
    }
    else if (domNode.isText())
    {
        stream.writeCharacters(domNode.nodeValue());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QList<QDomNode> GetChildElements(const QDomNode& e)
{
    QDomNodeList children = e.childNodes();
    QList<QDomNode> r;
    r.reserve(children.size());
    for (int k = 0; k < children.size(); ++k)
    {
        r << children.at(k);
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QDomNode> GetElementAttributes(const QDomNode& e)
{
    QDomNamedNodeMap attributes = e.attributes();
    QList<QDomNode> r;
    r.reserve(attributes.size());
    for (int k = 0; k < attributes.size(); ++k)
    {
        r << attributes.item(k);
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool LessThen(const QDomNode &element1, const QDomNode &element2)
{
    if (element1.nodeType() != element2.nodeType())
    {
        return element1.nodeType() < element2.nodeType();
    }

    QString tag1 = element1.nodeName();
    QString tag2 = element2.nodeName();

    //qDebug() << tag1 <<tag2;
    if (tag1 != tag2)
    {
        return tag1 < tag2;
    }

    // Compare attributes
    QList<QDomNode> attributes1 = GetElementAttributes(element1);
    QList<QDomNode> attributes2 = GetElementAttributes(element2);

    if(attributes1.size() != attributes2.size())
    {
        return attributes1.size() < attributes2.size();
    }

    auto CompareDomNodeLists = [](QList<QDomNode> list1, QList<QDomNode> list2, bool *stop)
    {
        *stop = false;
        std::sort(list1.begin(), list1.end(), LessThen);
        std::sort(list2.begin(), list2.end(), LessThen);
        //qDebug() << "comparing sorted lists";
        for(int k = 0; k < list1.size(); ++k)
        {
            if (!LessThen(list1[k], list2[k]))
            {
                if (LessThen(list2[k], list1[k]))
                {
                    *stop = true;
                    //qDebug() << "false!";
                    return false;
                }
            }
            else
            {
                *stop = true;
                //qDebug() << "true!";
                return true;
            }
        }
        return false;
    };

    bool stop = false;
    bool result = CompareDomNodeLists(attributes1, attributes2, &stop);
    if (stop)
    {
        return result;
    }

    // Compare children
    QList<QDomNode> elts1 = GetChildElements(element1);
    QList<QDomNode> elts2 = GetChildElements(element2);

    QString value1, value2;

    if(elts1.size() != elts2.size())
    {
        return elts1.size() < elts2.size();
    }

    if(elts1.isEmpty())
    {
        value1 = element1.nodeValue();
        value2 = element2.nodeValue();

        //qDebug() <<value1 << value2 << (value1 < value2);
        return value1 < value2;
    }

    result = CompareDomNodeLists(elts1, elts2, &stop);
    if (stop)
    {
        return result;
    }
    return false;
}
}

Q_LOGGING_CATEGORY(vXML, "v.xml")

const QString VDomDocument::AttrId          = QStringLiteral("id");
const QString VDomDocument::AttrText        = QStringLiteral("text");
const QString VDomDocument::AttrBold        = QStringLiteral("bold");
const QString VDomDocument::AttrItalic      = QStringLiteral("italic");
const QString VDomDocument::AttrAlignment   = QStringLiteral("alignment");
const QString VDomDocument::AttrFSIncrement = QStringLiteral("sfIncrement");

const QString VDomDocument::TagVersion = QStringLiteral("version");
const QString VDomDocument::TagUnit    = QStringLiteral("unit");
const QString VDomDocument::TagLine    = QStringLiteral("line");

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::VDomDocument(QObject *parent)
    : QObject(parent),
      QDomDocument(),
      m_elementIdCache(),
      m_watcher(new QFutureWatcher<QHash<quint32, QDomElement>>(this))
{
    connect(m_watcher, &QFutureWatcher<QHash<quint32, QDomElement>>::finished, this, &VDomDocument::CacheRefreshed);
}

//---------------------------------------------------------------------------------------------------------------------
VDomDocument::~VDomDocument()
{
    m_watcher->cancel();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VDomDocument::elementById(quint32 id, const QString &tagName, bool updateCache)
{
    if (id == 0)
    {
        return QDomElement();
    }

    if (m_elementIdCache.contains(id))
    {
        const QDomElement e = m_elementIdCache.value(id);
        if (e.parentNode().nodeType() != QDomNode::BaseNode)
        {
            if (not tagName.isEmpty())
            {
                if (e.tagName() == tagName)
                {
                    return e;
                }
            }
            else
            {
                return e;
            }
        }
    }

    if (updateCache)
    {   // Cached missed
        RefreshElementIdCache();
    }

    if (tagName.isEmpty())
    {
        // Because VDomDocument::find checks for unique id we must use temp cache
        QHash<quint32, QDomElement> tmpCache;
        if (VDomDocument::find(tmpCache, this->documentElement(), id))
        {
            return tmpCache.value(id);
        }
    }
    else
    {
        const QDomNodeList list = elementsByTagName(tagName);
        for (int i=0; i < list.size(); ++i)
        {
            const QDomElement domElement = list.at(i).toElement();
            if (not domElement.isNull() && domElement.hasAttribute(AttrId))
            {
                const quint32 elementId = GetParametrUInt(domElement, AttrId, NULL_ID_STR);

                if (elementId == id)
                {
                    return domElement;
                }
            }
        }
    }

    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Find element by id.
 * @param cache cache with element ids
 * @param node node
 * @param id id value
 * @return true if found
 */
bool VDomDocument::find(QHash<quint32, QDomElement> &cache, const QDomElement &node, quint32 id)
{
    if (node.hasAttribute(AttrId))
    {
        const quint32 elementId = GetParametrUInt(node, AttrId, NULL_ID_STR);

        if (cache.contains(elementId))
        {
            qWarning() << tr("Not unique id (%1)").arg(elementId);
        }

        cache.insert(elementId, node);
        if (elementId == id)
        {
            return true;
        }
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i)
    {
        const QDomNode n = node.childNodes().at(i);
        if (n.isElement())
        {
            if (VDomDocument::find(cache, n.toElement(), id))
            {
                return true;
            }
        }
    }
    return false;
}


//---------------------------------------------------------------------------------------------------------------------
QHash<quint32, QDomElement> VDomDocument::RefreshCache(const QDomElement &root) const
{
    QHash<quint32, QDomElement> cache;
    VDomDocument::find(cache, root, NULL_ID);
    return cache;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::SaveCanonicalXML(QIODevice *file, int indent, QString &error) const
{
    SCASSERT(file != nullptr)

    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(indent);
    stream.writeStartDocument();

    QDomNode root = documentElement();
    while (not root.isNull())
    {
        SaveNodeCanonically(stream, root);
        if (stream.hasError())
        {
            break;
        }
        root = root.nextSibling();
    }

    stream.writeEndDocument();

    if (stream.hasError())
    {
        error = tr("Fail to write Canonical XML.");
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the long long value of the given attribute. RENAME: GetParameterLongLong?
 * @param domElement tag in xml tree
 * @param name attribute name
 * @return long long value
 */
quint32 VDomDocument::GetParametrUInt(const QDomElement &domElement, const QString &name, const QString &defValue)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null"); //-V591

    bool ok = false;
    QString parametr;
    quint32 id = 0;

    try
    {
        parametr = GetParametrString(domElement, name, defValue);
        id = parametr.toUInt(&ok);
        if (ok == false)
        {
            throw VExceptionConversionError(QObject::tr("Can't convert toUInt parameter"), name);
        }
    }
    catch (const VExceptionEmptyParameter &e)
    {
        VExceptionConversionError excep(QObject::tr("Can't convert toUInt parameter"), name);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }

    return id;
}

//---------------------------------------------------------------------------------------------------------------------
int VDomDocument::GetParametrInt(const QDomElement &domElement, const QString &name, const QString &defValue)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null"); //-V591

    bool ok = false;
    QString parametr;
    int value = 0;

    try
    {
        parametr = GetParametrString(domElement, name, defValue);
        value = parametr.toInt(&ok);
        if (ok == false)
        {
            throw VExceptionConversionError(QObject::tr("Can't convert toInt parameter"), name);
        }
    }
    catch (const VExceptionEmptyParameter &e)
    {
        VExceptionConversionError excep(QObject::tr("Can't convert toInt parameter"), name);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }

    return value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::GetParametrBool(const QDomElement &domElement, const QString &name, const QString &defValue)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    QString parametr;
    bool val = true;

    const QString message = QObject::tr("Can't convert toBool parameter");
    try
    {
        parametr = GetParametrString(domElement, name, defValue);

        const QStringList bools {trueStr, falseStr, QChar('1'), QChar('0')};
        switch (bools.indexOf(parametr))
        {
            case 0: // true
            case 2: // 1
                val = true;
                break;
            case 1: // false
            case 3: // 0
                val = false;
                break;
            default:// others
                throw VExceptionConversionError(message, name);
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
NodeUsage VDomDocument::GetParametrUsage(const QDomElement &domElement, const QString &name)
{
    const bool value = GetParametrBool(domElement, name, trueStr);
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
        domElement.setAttribute(name, trueStr);
    }
    else
    {
        domElement.setAttribute(name, falseStr);
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
                                        const QString &defValue)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    const QString parameter = domElement.attribute(name, defValue);
    if (parameter.isEmpty())
    {
        if (defValue.isEmpty())
        {
            throw VExceptionEmptyParameter(QObject::tr("Got empty parameter"), name, domElement);
        }
        else
        {
            return defValue;
        }
    }
    return parameter;
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::GetParametrEmptyString(const QDomElement &domElement, const QString &name)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    return domElement.attribute(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the double value of the given attribute.
 * @param domElement tag in xml tree
 * @param name attribute name
 * @return double value
 */
qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    bool ok = false;
    qreal param = 0;

    const QString message = QObject::tr("Can't convert toDouble parameter");
    try
    {
        QString parametr = GetParametrString(domElement, name, defValue);
        param = parametr.replace(QChar(','), QChar('.')).toDouble(&ok);
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
quint32 VDomDocument::GetParametrId(const QDomElement &domElement)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    quint32 id = NULL_ID;

    const QString message = QObject::tr("Got wrong parameter id. Need only id > 0.");
    try
    {
        id = GetParametrUInt(domElement, VDomDocument::AttrId, NULL_ID_STR);
        if (id == NULL_ID)
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
Unit VDomDocument::MUnit() const
{
    Unit unit = StrToUnits(UniqueTagText(TagUnit, unitCM));

    if (unit == Unit::Px)
    {
        unit = Unit::Cm;
    }

    return unit;
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
            throw VExceptionWrongId(tr("This id (%1) is not unique.").arg(id), node);
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
void VDomDocument::RefreshElementIdCache()
{
    if (m_watcher->isFinished())
    {
        m_watcher->setFuture(QtConcurrent::run(this, &VDomDocument::RefreshCache, documentElement()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::Compare(const QDomElement &element1, const QDomElement &element2)
{
    QFuture<bool> lessThen2 = QtConcurrent::run(LessThen, element2, element1);
    return !LessThen(element1, element2) && !lessThen2.result();
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::CacheRefreshed()
{
    m_elementIdCache = m_watcher->future().result();
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::setXMLContent(const QString &fileName)
{
    QFile file(fileName);
    // cppcheck-suppress ConfigurationNotChecked
    if (file.open(QIODevice::ReadOnly) == false)
    {
        const QString errorMsg(tr("Can't open file %1:\n%2.").arg(fileName, file.errorString()));
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

    RefreshElementIdCache();
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
QDomElement VDomDocument::CreateElementWithText(const QString &tagName, const QString &text)
{
    QDomElement tag = createElement(tagName);
    tag.appendChild(createTextNode(text));
    return tag;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::SaveDocument(const QString &fileName, QString &error)
{
    if (fileName.isEmpty())
    {
        qDebug()<<"Got empty file name.";
        return false;
    }
    bool success = false;
    QSaveFile file(fileName);
    // cppcheck-suppress ConfigurationNotChecked
    if (file.open(QIODevice::WriteOnly))
    {
        // See issue #666. QDomDocument produces random attribute order.
        const int indent = 4;
        if (not SaveCanonicalXML(&file, indent, error))
        {
            return false;
        }
        // Left these strings in case we will need them for testing purposes
        // QTextStream out(&file);
        // out.setCodec("UTF-8");
        // save(out, indent);

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
    QStringList v = version.split(QChar('.'));
    return v.at(0);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VDomDocument::Minor() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(QChar('.'));
    return v.at(1);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VDomDocument::Patch() const
{
    QString version = UniqueTagText(TagVersion, "0.0.0");
    QStringList v = version.split(QChar('.'));
    return v.at(2);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDomDocument::GetFormatVersionStr() const
{
    const QDomNodeList nodeList = this->elementsByTagName(TagVersion);
    if (nodeList.isEmpty())
    {
        const QString errorMsg(tr("Couldn't get version information."));
        throw VException(errorMsg);
    }

    if (nodeList.count() > 1)
    {
        const QString errorMsg(tr("Too many tags <%1> in file.").arg(TagVersion));
        throw VException(errorMsg);
    }

    const QDomNode domNode = nodeList.at(0);
    if (domNode.isNull() == false && domNode.isElement())
    {
        const QDomElement domElement = domNode.toElement();
        if (domElement.isNull() == false)
        {
            return domElement.text();
        }
    }
    return QString(QStringLiteral("0.0.0"));
}

//---------------------------------------------------------------------------------------------------------------------
int VDomDocument::GetFormatVersion(const QString &version)
{
    ValidateVersion(version);

    const QStringList ver = version.split(QChar('.'));

    bool ok = false;
    const int major = ver.at(0).toInt(&ok);
    if (not ok)
    {
        return 0x0;
    }

    ok = false;
    const int minor = ver.at(1).toInt(&ok);
    if (not ok)
    {
        return 0x0;
    }

    ok = false;
    const int patch = ver.at(2).toInt(&ok);
    if (not ok)
    {
        return 0x0;
    }

    return (major<<16)|(minor<<8)|(patch);
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
            return setTagText(domElement, text);
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDomDocument::setTagText(const QDomElement &domElement, const QString &text)
{
    if (domElement.isNull() == false)
    {
        QDomElement parent = domElement.parentNode().toElement();
        QDomElement newTag = createElement(domElement.tagName());

        const QDomText newTagText = createTextNode(text);
        newTag.appendChild(newTagText);

        parent.replaceChild(newTag, domElement);
        return true;
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
QDomElement VDomDocument::NodeById(const quint32 &nodeId, const QString &tagName)
{
    QDomElement domElement = elementById(nodeId, tagName);
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

    QTemporaryFile destFile(destination + QLatin1String(".XXXXXX"));
    destFile.setAutoRemove(false);// Will be renamed to be destination file
    // cppcheck-suppress ConfigurationNotChecked
    if (not destFile.open())
    {
        error = destFile.errorString();
    }
    else
    {
        QFile sourceFile(source);
        // cppcheck-suppress ConfigurationNotChecked
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
        else
        {
            error = sourceFile.errorString();
        }
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn off check permission again
#endif /*Q_OS_WIN32*/

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLabelTemplateLine> VDomDocument::GetLabelTemplate(const QDomElement &element) const
{
    // We use implicit conversion. That's why check if values are still the same as excpected.
    Q_STATIC_ASSERT(Qt::AlignLeft == 1);
    Q_STATIC_ASSERT(Qt::AlignRight == 2);
    Q_STATIC_ASSERT(Qt::AlignHCenter == 4);

    QVector<VLabelTemplateLine> lines;

    if (not element.isNull())
    {
        QDomElement tagLine = element.firstChildElement();
        while (tagLine.isNull() == false)
        {
            if (tagLine.tagName() == TagLine)
            {
                VLabelTemplateLine line;
                line.line = GetParametrString(tagLine, AttrText, tr("<empty>"));
                line.bold = GetParametrBool(tagLine, AttrBold, falseStr);
                line.italic = GetParametrBool(tagLine, AttrItalic, falseStr);
                line.alignment = static_cast<int>(GetParametrUInt(tagLine, AttrAlignment, QChar('0')));
                line.fontSizeIncrement = static_cast<int>(GetParametrUInt(tagLine, AttrFSIncrement, QChar('0')));
                lines.append(line);
            }
            tagLine = tagLine.nextSiblingElement(TagLine);
        }
    }

    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::SetLabelTemplate(QDomElement &element, const QVector<VLabelTemplateLine> &lines)
{
    if (not element.isNull())
    {
        for (auto &line : lines)
        {
            QDomElement tagLine = createElement(TagLine);

            SetAttribute(tagLine, AttrText, line.line);
            SetAttribute(tagLine, AttrBold, line.bold);
            SetAttribute(tagLine, AttrItalic, line.italic);
            SetAttribute(tagLine, AttrAlignment, line.alignment);
            SetAttribute(tagLine, AttrFSIncrement, line.fontSizeIncrement);

            element.appendChild(tagLine);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDomDocument::ValidateVersion(const QString &version)
{
    const QRegularExpression rx(QStringLiteral("^([0-9]|[1-9][0-9]|[1-2][0-5][0-5]).([0-9]|[1-9][0-9]|[1-2][0-5][0-5])"
                                               ".([0-9]|[1-9][0-9]|[1-2][0-5][0-5])$"));

    if (rx.match(version).hasMatch() == false)
    {
        const QString errorMsg(tr("Version \"%1\" invalid.").arg(version));
        throw VException(errorMsg);
    }

    if (version == QLatin1String("0.0.0"))
    {
        const QString errorMsg(tr("Version \"0.0.0\" invalid."));
        throw VException(errorMsg);
    }
}
