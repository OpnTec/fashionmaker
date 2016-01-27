/************************************************************************
 **
 **  @file   vabstractpattern.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vabstractpattern.h"
#include "exception/vexceptionbadid.h"
#include "exception/vexceptionemptyparameter.h"
#include "vpatternconverter.h"
#include "../qmuparser/qmutokenparser.h"

const QString VAbstractPattern::TagPattern      = QStringLiteral("pattern");
const QString VAbstractPattern::TagCalculation  = QStringLiteral("calculation");
const QString VAbstractPattern::TagModeling     = QStringLiteral("modeling");
const QString VAbstractPattern::TagDetails      = QStringLiteral("details");
const QString VAbstractPattern::TagAuthor       = QStringLiteral("author");
const QString VAbstractPattern::TagDescription  = QStringLiteral("description");
const QString VAbstractPattern::TagNotes        = QStringLiteral("notes");
const QString VAbstractPattern::TagMeasurements = QStringLiteral("measurements");
const QString VAbstractPattern::TagIncrements   = QStringLiteral("increments");
const QString VAbstractPattern::TagIncrement    = QStringLiteral("increment");
const QString VAbstractPattern::TagDraw         = QStringLiteral("draw");
const QString VAbstractPattern::TagPoint        = QStringLiteral("point");
const QString VAbstractPattern::TagLine         = QStringLiteral("line");
const QString VAbstractPattern::TagSpline       = QStringLiteral("spline");
const QString VAbstractPattern::TagArc          = QStringLiteral("arc");
const QString VAbstractPattern::TagTools        = QStringLiteral("tools");
const QString VAbstractPattern::TagGradation    = QStringLiteral("gradation");
const QString VAbstractPattern::TagHeights      = QStringLiteral("heights");
const QString VAbstractPattern::TagSizes        = QStringLiteral("sizes");
const QString VAbstractPattern::TagUnit         = QStringLiteral("unit");

const QString VAbstractPattern::AttrName        = QStringLiteral("name");
const QString VAbstractPattern::AttrType        = QStringLiteral("type");

const QString VAbstractPattern::AttrAll         = QStringLiteral("all");

const QString VAbstractPattern::AttrH92         = QStringLiteral("h92");
const QString VAbstractPattern::AttrH98         = QStringLiteral("h98");
const QString VAbstractPattern::AttrH104        = QStringLiteral("h104");
const QString VAbstractPattern::AttrH110        = QStringLiteral("h110");
const QString VAbstractPattern::AttrH116        = QStringLiteral("h116");
const QString VAbstractPattern::AttrH122        = QStringLiteral("h122");
const QString VAbstractPattern::AttrH128        = QStringLiteral("h128");
const QString VAbstractPattern::AttrH134        = QStringLiteral("h134");
const QString VAbstractPattern::AttrH140        = QStringLiteral("h140");
const QString VAbstractPattern::AttrH146        = QStringLiteral("h146");
const QString VAbstractPattern::AttrH152        = QStringLiteral("h152");
const QString VAbstractPattern::AttrH158        = QStringLiteral("h158");
const QString VAbstractPattern::AttrH164        = QStringLiteral("h164");
const QString VAbstractPattern::AttrH170        = QStringLiteral("h170");
const QString VAbstractPattern::AttrH176        = QStringLiteral("h176");
const QString VAbstractPattern::AttrH182        = QStringLiteral("h182");
const QString VAbstractPattern::AttrH188        = QStringLiteral("h188");
const QString VAbstractPattern::AttrH194        = QStringLiteral("h194");

const QString VAbstractPattern::AttrS22         = QStringLiteral("s22");
const QString VAbstractPattern::AttrS24         = QStringLiteral("s24");
const QString VAbstractPattern::AttrS26         = QStringLiteral("s26");
const QString VAbstractPattern::AttrS28         = QStringLiteral("s28");
const QString VAbstractPattern::AttrS30         = QStringLiteral("s30");
const QString VAbstractPattern::AttrS32         = QStringLiteral("s32");
const QString VAbstractPattern::AttrS34         = QStringLiteral("s34");
const QString VAbstractPattern::AttrS36         = QStringLiteral("s36");
const QString VAbstractPattern::AttrS38         = QStringLiteral("s38");
const QString VAbstractPattern::AttrS40         = QStringLiteral("s40");
const QString VAbstractPattern::AttrS42         = QStringLiteral("s42");
const QString VAbstractPattern::AttrS44         = QStringLiteral("s44");
const QString VAbstractPattern::AttrS46         = QStringLiteral("s46");
const QString VAbstractPattern::AttrS48         = QStringLiteral("s48");
const QString VAbstractPattern::AttrS50         = QStringLiteral("s50");
const QString VAbstractPattern::AttrS52         = QStringLiteral("s52");
const QString VAbstractPattern::AttrS54         = QStringLiteral("s54");
const QString VAbstractPattern::AttrS56         = QStringLiteral("s56");

const QString VAbstractPattern::AttrCustom      = QStringLiteral("custom");
const QString VAbstractPattern::AttrDefHeight   = QStringLiteral("defHeight");
const QString VAbstractPattern::AttrDefSize     = QStringLiteral("defSize");

const QString VAbstractPattern::IncrementName        = QStringLiteral("name");
const QString VAbstractPattern::IncrementFormula     = QStringLiteral("formula");
const QString VAbstractPattern::IncrementDescription = QStringLiteral("description");

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern::VAbstractPattern(QObject *parent)
    : QObject(parent), VDomDocument(), nameActivPP(QString()), cursor(0), tools(QHash<quint32, VDataTool*>()),
      history(QVector<VToolRecord>()), patternPieces(QStringList()), modified(false)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern::~VAbstractPattern()
{}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListMeasurements() const
{
    QSet<QString> measurements;
    QSet<QString> others;

    const QStringList increments = ListIncrements();
    for (int i=0; i < increments.size(); ++i)
    {
        others.insert(increments.at(i));
    }

    const QStringList expressions = ListExpressions();
    for (int i=0; i < expressions.size(); ++i)
    {
        qmu::QmuTokenParser *cal = new qmu::QmuTokenParser(expressions.at(i), false, false);// Eval formula
        const QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
        delete cal;

        const QList<QString> tValues = tokens.values();
        for (int j = 0; j < tValues.size(); ++j)
        {
            if (tValues.at(j) == QLatin1Literal("-"))
            {
                continue;
            }

            if (measurements.contains(tValues.at(j)))
            {
                continue;
            }

            if (others.contains(tValues.at(j)))
            {
                continue;
            }

            if (IsVariable(tValues.at(j)) || IsPostfixOperator(tValues.at(j)) || IsFunction(tValues.at(j)))
            {
                others.insert(tValues.at(j));
                continue;
            }
            else
            {
                measurements.insert(tValues.at(j));
            }
        }
    }

    return QStringList(measurements.toList());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeActivPP set new active pattern piece name.
 * @param name new name.
 * @param parse parser file mode.
 */
void VAbstractPattern::ChangeActivPP(const QString &name, const Document &parse)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name pattern piece is empty");
    if (CheckExistNamePP(name) && this->nameActivPP != name)
    {
        this->nameActivPP = name;
        if (parse == Document::FullParse)
        {
            emit ChangedActivPP(name);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameActivPP return current pattern piece name.
 * @return pattern piece name.
 */
QString VAbstractPattern::GetNameActivPP() const
{
    return nameActivPP;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetActivDrawElement return draw tag for current pattern peace.
 * @param element draw tag.
 * @return true if found.
 */
bool VAbstractPattern::GetActivDrawElement(QDomElement &element) const
{
    if (nameActivPP.isEmpty() == false)
    {
        const QDomNodeList elements = this->documentElement().elementsByTagName( TagDraw );
        if (elements.size() == 0)
        {
            return false;
        }
        for ( qint32 i = 0; i < elements.count(); i++ )
        {
            element = elements.at( i ).toElement();
            if (element.isNull() == false)
            {
                const QString fieldName = element.attribute( AttrName );
                if ( fieldName == nameActivPP )
                {
                    return true;
                }
            }
        }
        element = QDomElement();
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckNameDraw check if exist pattern peace with this name.
 * @param name pattern peace name.
 * @return true if exist.
 */
bool VAbstractPattern::CheckExistNamePP(const QString &name) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name draw is empty");
    const QDomNodeList elements = this->documentElement().elementsByTagName( TagDraw );
    if (elements.size() == 0)
    {
        return false;
    }
    for ( qint32 i = 0; i < elements.count(); i++ )
    {
        const QDomElement elem = elements.at( i ).toElement();
        if (elem.isNull() == false)
        {
            if ( GetParametrString(elem, AttrName) == name )
            {
                return true;
            }
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetActivNodeElement find element in current pattern piece by name.
 * @param name name tag.
 * @param element element.
 * @return true if found.
 */
bool VAbstractPattern::GetActivNodeElement(const QString &name, QDomElement &element) const
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name draw is empty");
    QDomElement drawElement;
    if (GetActivDrawElement(drawElement))
    {
        const QDomNodeList listElement = drawElement.elementsByTagName(name);
        if (listElement.size() != 1)
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
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractPattern::CountPP() const
{
    const QDomElement rootElement = this->documentElement();
    if (rootElement.isNull())
    {
        return 0;
    }

    return rootElement.elementsByTagName( TagDraw ).count();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VAbstractPattern::GetPPElement(const QString &name)
{
    if (name.isEmpty() == false)
    {
        const QDomNodeList elements = this->documentElement().elementsByTagName( TagDraw );
        if (elements.size() == 0)
        {
            return QDomElement();
        }
        for ( qint32 i = 0; i < elements.count(); i++ )
        {
            QDomElement element = elements.at( i ).toElement();
            if (element.isNull() == false)
            {
                if ( element.attribute( AttrName ) == name )
                {
                    return element;
                }
            }
        }
    }
    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeNamePP change pattern piece name.
 * @param oldName old pattern piece name.
 * @param newName new pattern piece name.
 * @return true if success.
 */
bool VAbstractPattern::ChangeNamePP(const QString &oldName, const QString &newName)
{
    Q_ASSERT_X(not newName.isEmpty(), Q_FUNC_INFO, "new name pattern piece is empty");
    Q_ASSERT_X(not oldName.isEmpty(), Q_FUNC_INFO, "old name pattern piece is empty");

    if (CheckExistNamePP(oldName) == false)
    {
        qDebug()<<"Do not exist pattern piece with name"<<oldName;
        return false;
    }

    if (CheckExistNamePP(newName))
    {
        qDebug()<<"Already exist pattern piece with name"<<newName;
        return false;
    }

    QDomElement ppElement = GetPPElement(oldName);
    if (ppElement.isElement())
    {
        if (nameActivPP == oldName)
        {
            nameActivPP = newName;
        }
        ppElement.setAttribute(AttrName, newName);
        emit patternChanged(false);//For situation when we change name directly, without undocommands.
        emit ChangedNameDraw(oldName, newName);
        return true;
    }
    else
    {
        qDebug()<<"Can't find pattern piece node with name"<<oldName<<Q_FUNC_INFO;
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief appendPP add new pattern piece.
 *
 * Method check if not exist pattern piece with the same name and change name active pattern piece name, send signal
 * about change pattern piece. Doen't add pattern piece to file structure. This task make SPoint tool.
 * @param name pattern peace name.
 * @return true if success.
 */
bool VAbstractPattern::appendPP(const QString &name)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name pattern piece is empty");
    if (name.isEmpty())
    {
        return false;
    }
    if (CheckExistNamePP(name) == false)
    {
        SetActivPP(name);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractPattern::getCursor() const
{
    return cursor;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::setCursor(const quint32 &value)
{
    if (cursor != value)
    {
        cursor = value;
        emit ChangedCursor(cursor);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTool return tool from tool list.
 * @param id tool id.
 * @return tool.
 */
VDataTool *VAbstractPattern::getTool(const quint32 &id)
{
    ToolExists(id);
    return tools.value(id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddTool add tool to list tools.
 * @param id tool id.
 * @param tool tool.
 */
void VAbstractPattern::AddTool(const quint32 &id, VDataTool *tool)
{
    Q_ASSERT_X(id != 0, Q_FUNC_INFO, "id == 0");
    SCASSERT(tool != nullptr);
    tools.insert(id, tool);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getHistory return list with list of history records.
 * @return list of history records.
 */
QVector<VToolRecord> *VAbstractPattern::getHistory()
{
    return &history;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VToolRecord> VAbstractPattern::getLocalHistory() const
{
    QVector<VToolRecord> historyPP;
    for (qint32 i = 0; i< history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        if (tool.getNameDraw() != GetNameActivPP())
        {
            continue;
        }
        historyPP.append(tool);
    }
    return historyPP;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::MPath() const
{
    return UniqueTagText(TagMeasurements);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPath(const QString &path)
{
    if (setTagText(TagMeasurements, path))
    {
        emit patternChanged(false);
    }
    else
    {
        qDebug()<<"Can't save path to measurements"<<Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Unit VAbstractPattern::MUnit() const
{
    const QStringList units = QStringList() << "mm" << "cm" << "inch";
    const QString unit = UniqueTagText(TagUnit);
    switch (units.indexOf(unit))
    {
        case 0:// mm
            return Unit::Mm;
            break;
        case 1:// cm
            return Unit::Cm;
            break;
        case 2:// in
            return Unit::Inch;
            break;
        default:
            return Unit::Cm;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractPattern::SiblingNodeId(const quint32 &nodeId) const
{
    quint32 siblingId = NULL_ID;

    const QVector<VToolRecord> history = getLocalHistory();
    for (qint32 i = 0; i < history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        if (nodeId == tool.getId())
        {
            if (i == 0)
            {
                siblingId = NULL_ID;
            }
            else
            {
                for (qint32 j = i; j > 0; --j)
                {
                    const VToolRecord tool = history.at(j-1);
                    switch ( tool.getTypeTool() )
                    {
                        case Tool::Detail:
                        case Tool::UnionDetails:
                        case Tool::NodeArc:
                        case Tool::NodePoint:
                        case Tool::NodeSpline:
                        case Tool::NodeSplinePath:
                            continue;
                            break;
                        default:
                            siblingId = tool.getId();
                            j = 0;// break loop
                            break;
                    }
                }
            }
        }
    }
    return siblingId;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::getPatternPieces() const
{
    return patternPieces;
}

//---------------------------------------------------------------------------------------------------------------------
QMap<GHeights, bool> VAbstractPattern::GetGradationHeights() const
{
    QMap<GHeights, bool> map;
    map.insert(GHeights::ALL, true);
    map.insert(GHeights::H92, true);
    map.insert(GHeights::H98, true);
    map.insert(GHeights::H104, true);
    map.insert(GHeights::H110, true);
    map.insert(GHeights::H116, true);
    map.insert(GHeights::H122, true);
    map.insert(GHeights::H128, true);
    map.insert(GHeights::H134, true);
    map.insert(GHeights::H140, true);
    map.insert(GHeights::H146, true);
    map.insert(GHeights::H152, true);
    map.insert(GHeights::H158, true);
    map.insert(GHeights::H164, true);
    map.insert(GHeights::H170, true);
    map.insert(GHeights::H176, true);
    map.insert(GHeights::H182, true);
    map.insert(GHeights::H188, true);
    map.insert(GHeights::H194, true);

    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.size() == 0)
    {
        return map;
    }

    QStringList gTags = QStringList() << TagHeights << TagSizes;
    QDomNode domNode = tags.at(0).firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                const QString defValue = QStringLiteral("true");
                switch (gTags.indexOf(domElement.tagName()))
                {
                    case 0: // TagHeights
                        if (GetParametrBool(domElement, AttrAll, defValue))
                        {
                            return map;
                        }
                        else
                        {
                            map.insert(GHeights::ALL, false);
                        }

                        map.insert(GHeights::H92, GetParametrBool(domElement, AttrH92, defValue));
                        map.insert(GHeights::H98, GetParametrBool(domElement, AttrH98, defValue));
                        map.insert(GHeights::H104, GetParametrBool(domElement, AttrH104, defValue));
                        map.insert(GHeights::H110, GetParametrBool(domElement, AttrH110, defValue));
                        map.insert(GHeights::H116, GetParametrBool(domElement, AttrH116, defValue));
                        map.insert(GHeights::H122, GetParametrBool(domElement, AttrH122, defValue));
                        map.insert(GHeights::H128, GetParametrBool(domElement, AttrH128, defValue));
                        map.insert(GHeights::H134, GetParametrBool(domElement, AttrH134, defValue));
                        map.insert(GHeights::H140, GetParametrBool(domElement, AttrH140, defValue));
                        map.insert(GHeights::H146, GetParametrBool(domElement, AttrH146, defValue));
                        map.insert(GHeights::H152, GetParametrBool(domElement, AttrH152, defValue));
                        map.insert(GHeights::H158, GetParametrBool(domElement, AttrH158, defValue));
                        map.insert(GHeights::H164, GetParametrBool(domElement, AttrH164, defValue));
                        map.insert(GHeights::H170, GetParametrBool(domElement, AttrH170, defValue));
                        map.insert(GHeights::H176, GetParametrBool(domElement, AttrH176, defValue));
                        map.insert(GHeights::H182, GetParametrBool(domElement, AttrH182, defValue));
                        map.insert(GHeights::H188, GetParametrBool(domElement, AttrH188, defValue));
                        map.insert(GHeights::H194, GetParametrBool(domElement, AttrH194, defValue));
                        return map;
                        break;
                    case 1: // TagSizes
                        break;
                    default:
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetGradationHeights(const QMap<GHeights, bool> &options)
{
    CheckTagExists(TagGradation);
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.size() == 0)
    {
        qDebug()<<"Can't save tag "<<TagGradation<<Q_FUNC_INFO;
        return;
    }

    QStringList gTags = QStringList() << TagHeights << TagSizes;
    QDomNode domNode = tags.at(0).firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                switch (gTags.indexOf(domElement.tagName()))
                {
                    case 0: // TagHeights
                        SetAttribute(domElement, AttrAll, options.value(GHeights::ALL));
                        if (options.value(GHeights::ALL))
                        {
                            domElement.removeAttribute(AttrH92);
                            domElement.removeAttribute(AttrH98);
                            domElement.removeAttribute(AttrH104);
                            domElement.removeAttribute(AttrH110);
                            domElement.removeAttribute(AttrH116);
                            domElement.removeAttribute(AttrH122);
                            domElement.removeAttribute(AttrH128);
                            domElement.removeAttribute(AttrH134);
                            domElement.removeAttribute(AttrH140);
                            domElement.removeAttribute(AttrH146);
                            domElement.removeAttribute(AttrH152);
                            domElement.removeAttribute(AttrH158);
                            domElement.removeAttribute(AttrH164);
                            domElement.removeAttribute(AttrH170);
                            domElement.removeAttribute(AttrH176);
                            domElement.removeAttribute(AttrH182);
                            domElement.removeAttribute(AttrH188);
                            domElement.removeAttribute(AttrH194);
                        }
                        else
                        {
                            SetAttribute(domElement, AttrH92, options.value(GHeights::H92));
                            SetAttribute(domElement, AttrH98, options.value(GHeights::H98));
                            SetAttribute(domElement, AttrH104, options.value(GHeights::H104));
                            SetAttribute(domElement, AttrH110, options.value(GHeights::H110));
                            SetAttribute(domElement, AttrH116, options.value(GHeights::H116));
                            SetAttribute(domElement, AttrH122, options.value(GHeights::H122));
                            SetAttribute(domElement, AttrH128, options.value(GHeights::H128));
                            SetAttribute(domElement, AttrH134, options.value(GHeights::H134));
                            SetAttribute(domElement, AttrH140, options.value(GHeights::H140));
                            SetAttribute(domElement, AttrH146, options.value(GHeights::H146));
                            SetAttribute(domElement, AttrH152, options.value(GHeights::H152));
                            SetAttribute(domElement, AttrH158, options.value(GHeights::H158));
                            SetAttribute(domElement, AttrH164, options.value(GHeights::H164));
                            SetAttribute(domElement, AttrH170, options.value(GHeights::H170));
                            SetAttribute(domElement, AttrH176, options.value(GHeights::H176));
                            SetAttribute(domElement, AttrH182, options.value(GHeights::H182));
                            SetAttribute(domElement, AttrH188, options.value(GHeights::H188));
                            SetAttribute(domElement, AttrH194, options.value(GHeights::H194));
                        }

                        modified = true;
                        emit patternChanged(false);
                        return;
                        break;
                    case 1: // TagSizes
                        break;
                    default:
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QMap<GSizes, bool> VAbstractPattern::GetGradationSizes() const
{
    QMap<GSizes, bool> map;
    map.insert(GSizes::ALL, true);
    map.insert(GSizes::S22, true);
    map.insert(GSizes::S24, true);
    map.insert(GSizes::S26, true);
    map.insert(GSizes::S28, true);
    map.insert(GSizes::S30, true);
    map.insert(GSizes::S32, true);
    map.insert(GSizes::S34, true);
    map.insert(GSizes::S36, true);
    map.insert(GSizes::S38, true);
    map.insert(GSizes::S40, true);
    map.insert(GSizes::S42, true);
    map.insert(GSizes::S44, true);
    map.insert(GSizes::S46, true);
    map.insert(GSizes::S48, true);
    map.insert(GSizes::S50, true);
    map.insert(GSizes::S52, true);
    map.insert(GSizes::S54, true);
    map.insert(GSizes::S56, true);

    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.size() == 0)
    {
        return map;
    }

    QStringList gTags = QStringList() << TagHeights << TagSizes;
    QDomNode domNode = tags.at(0).firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                const QString defValue = QStringLiteral("true");
                switch (gTags.indexOf(domElement.tagName()))
                {
                    case 0: // TagHeights
                        break;
                    case 1: // TagSizes
                        if (GetParametrBool(domElement, AttrAll, defValue))
                        {
                            return map;
                        }
                        else
                        {
                            map.insert(GSizes::ALL, false);
                        }

                        map.insert(GSizes::S22, GetParametrBool(domElement, AttrS22, defValue));
                        map.insert(GSizes::S24, GetParametrBool(domElement, AttrS24, defValue));
                        map.insert(GSizes::S26, GetParametrBool(domElement, AttrS26, defValue));
                        map.insert(GSizes::S28, GetParametrBool(domElement, AttrS28, defValue));
                        map.insert(GSizes::S30, GetParametrBool(domElement, AttrS30, defValue));
                        map.insert(GSizes::S32, GetParametrBool(domElement, AttrS32, defValue));
                        map.insert(GSizes::S34, GetParametrBool(domElement, AttrS34, defValue));
                        map.insert(GSizes::S36, GetParametrBool(domElement, AttrS36, defValue));
                        map.insert(GSizes::S38, GetParametrBool(domElement, AttrS38, defValue));
                        map.insert(GSizes::S40, GetParametrBool(domElement, AttrS40, defValue));
                        map.insert(GSizes::S42, GetParametrBool(domElement, AttrS42, defValue));
                        map.insert(GSizes::S44, GetParametrBool(domElement, AttrS44, defValue));
                        map.insert(GSizes::S46, GetParametrBool(domElement, AttrS46, defValue));
                        map.insert(GSizes::S48, GetParametrBool(domElement, AttrS48, defValue));
                        map.insert(GSizes::S50, GetParametrBool(domElement, AttrS50, defValue));
                        map.insert(GSizes::S52, GetParametrBool(domElement, AttrS52, defValue));
                        map.insert(GSizes::S54, GetParametrBool(domElement, AttrS54, defValue));
                        map.insert(GSizes::S56, GetParametrBool(domElement, AttrS56, defValue));
                        return map;
                        break;
                    default:
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetGradationSizes(const QMap<GSizes, bool> &options)
{
    CheckTagExists(TagGradation);
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.size() == 0)
    {
        qDebug()<<"Can't save tag "<<TagGradation<<Q_FUNC_INFO;
        return;
    }

    QStringList gTags = QStringList() << TagHeights << TagSizes;
    QDomNode domNode = tags.at(0).firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                switch (gTags.indexOf(domElement.tagName()))
                {
                    case 0: // TagHeights
                        break;
                    case 1: // TagSizes
                        SetAttribute(domElement, AttrAll, options.value(GSizes::ALL));
                        if (options.value(GSizes::ALL))
                        {
                            domElement.removeAttribute(AttrS22);
                            domElement.removeAttribute(AttrS24);
                            domElement.removeAttribute(AttrS26);
                            domElement.removeAttribute(AttrS28);
                            domElement.removeAttribute(AttrS30);
                            domElement.removeAttribute(AttrS32);
                            domElement.removeAttribute(AttrS34);
                            domElement.removeAttribute(AttrS36);
                            domElement.removeAttribute(AttrS38);
                            domElement.removeAttribute(AttrS40);
                            domElement.removeAttribute(AttrS42);
                            domElement.removeAttribute(AttrS44);
                            domElement.removeAttribute(AttrS46);
                            domElement.removeAttribute(AttrS48);
                            domElement.removeAttribute(AttrS50);
                            domElement.removeAttribute(AttrS52);
                            domElement.removeAttribute(AttrS54);
                            domElement.removeAttribute(AttrS56);
                        }
                        else
                        {
                            SetAttribute(domElement, AttrS22, options.value(GSizes::S22));
                            SetAttribute(domElement, AttrS24, options.value(GSizes::S24));
                            SetAttribute(domElement, AttrS26, options.value(GSizes::S26));
                            SetAttribute(domElement, AttrS28, options.value(GSizes::S28));
                            SetAttribute(domElement, AttrS30, options.value(GSizes::S30));
                            SetAttribute(domElement, AttrS32, options.value(GSizes::S32));
                            SetAttribute(domElement, AttrS34, options.value(GSizes::S34));
                            SetAttribute(domElement, AttrS36, options.value(GSizes::S36));
                            SetAttribute(domElement, AttrS38, options.value(GSizes::S38));
                            SetAttribute(domElement, AttrS40, options.value(GSizes::S40));
                            SetAttribute(domElement, AttrS42, options.value(GSizes::S42));
                            SetAttribute(domElement, AttrS44, options.value(GSizes::S44));
                            SetAttribute(domElement, AttrS46, options.value(GSizes::S46));
                            SetAttribute(domElement, AttrS48, options.value(GSizes::S48));
                            SetAttribute(domElement, AttrS50, options.value(GSizes::S50));
                            SetAttribute(domElement, AttrS52, options.value(GSizes::S52));
                            SetAttribute(domElement, AttrS54, options.value(GSizes::S54));
                            SetAttribute(domElement, AttrS56, options.value(GSizes::S56));
                        }

                        modified = true;
                        emit patternChanged(false);
                        return;
                        break;
                    default:
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetDescription() const
{
    return UniqueTagText(TagDescription);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetDescription(const QString &text)
{
    CheckTagExists(TagDescription);
    setTagText(TagDescription, text);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetNotes() const
{
    return UniqueTagText(TagNotes);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetNotes(const QString &text)
{
    CheckTagExists(TagNotes);
    setTagText(TagNotes, text);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetVersion() const
{
    return UniqueTagText(TagVersion, VPatternConverter::PatternMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetVersion()
{
    setTagText(TagVersion, VPatternConverter::PatternMaxVerStr);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveLiteChange we have unsaved change.
 */
void VAbstractPattern::haveLiteChange()
{
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowHistoryTool hightlight tool.
 * @param id tool id.
 * @param enable enable or diasable hightlight.
 */
void VAbstractPattern::ShowHistoryTool(quint32 id, bool enable)
{
    emit ShowTool(id, enable);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::NeedFullParsing()
{
    emit UndoCommand();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::ClearScene()
{
    emit ClearMainWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::ToolExists(const quint32 &id) const
{
    if (tools.contains(id) == false)
    {
        throw VExceptionBadId(tr("Can't find tool in table."), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetActivPP set current pattern piece.
 * @param name pattern peace name.
 */
void VAbstractPattern::SetActivPP(const QString &name)
{
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "name pattern piece is empty");
    this->nameActivPP = name;
    emit ChangedActivPP(name);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::CheckTagExists(const QString &tag)
{
    QDomNodeList list = elementsByTagName(tag);
    if (list.size() == 0)
    {
        QStringList tags = QStringList() << TagVersion << TagAuthor << TagDescription << TagNotes << TagGradation;
        QDomElement pattern = documentElement();
        switch (tags.indexOf(tag))
        {
            case 0: //TagVersion
                break;// Mandatory tag
            case 1: //TagAuthor
                pattern.insertAfter(createElement(TagAuthor), elementsByTagName(TagVersion).at(0));
                SetVersion();
                break;
            case 2: //TagDescription
            {
                for (int i = tags.indexOf(tag)-1; i >= 0; --i)
                {
                    QDomNodeList list = elementsByTagName(tags.at(i));
                    if (list.isEmpty())
                    {
                        continue;
                    }
                    pattern.insertAfter(createElement(TagDescription), list.at(0));
                }
                SetVersion();
                break;
            }
            case 3: //TagNotes
            {
                for (int i = tags.indexOf(tag)-1; i >= 0; --i)
                {
                    QDomNodeList list = elementsByTagName(tags.at(i));
                    if (list.isEmpty())
                    {
                        continue;
                    }
                    pattern.insertAfter(createElement(TagNotes), list.at(0));
                }
                SetVersion();
                break;
            }
            case 4: //TagGradation
            {
                QDomElement gradation = createElement(TagGradation);

                QDomElement heights = createElement(TagHeights);
                heights.setAttribute(AttrAll, QLatin1Literal("true"));
                gradation.appendChild(heights);

                QDomElement sizes = createElement(TagSizes);
                sizes.setAttribute(AttrAll, QLatin1Literal("true"));
                gradation.appendChild(sizes);

                for (int i = tags.indexOf(tag)-1; i >= 0; --i)
                {
                    QDomNodeList list = elementsByTagName(tags.at(i));
                    if (list.isEmpty())
                    {
                        continue;
                    }
                    pattern.insertAfter(gradation, list.at(0));
                    break;
                }
                SetVersion();
                break;
            }
            default:
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListIncrements() const
{
    QStringList increments;
    const QDomNodeList list = elementsByTagName(TagIncrement);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        try
        {
            increments.append(GetParametrString(dom, IncrementName));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return increments;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListExpressions() const
{
    QStringList list;

    list << ListPointExpressions();
    list << ListArcExpressions();
    list << ListSplineExpressions();
    list << ListIncrementExpressions();

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListPointExpressions() const
{
    QStringList expressions;
    const QDomNodeList list = elementsByTagName(TagPoint);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        try
        {
            expressions.append(GetParametrString(dom, AttrLength));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrAngle));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
        try
        {
            expressions.append(GetParametrString(dom, AttrC1Radius));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrC2Radius));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrCRadius));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrRadius));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListArcExpressions() const
{
    QStringList expressions;
    const QDomNodeList list = elementsByTagName(TagArc);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        try
        {
            expressions.append(GetParametrString(dom, AttrAngle1));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrAngle2));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrRadius));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrLength));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListSplineExpressions() const
{
    QStringList expressions;
    expressions << ListPathPointExpressions();
    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListPathPointExpressions() const
{
    QStringList expressions;
    const QDomNodeList list = elementsByTagName(AttrPathPoint);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        try
        {
            expressions.append(GetParametrString(dom, AttrKAsm1));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrKAsm2));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, AttrAngle));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VAbstractPattern::ListIncrementExpressions() const
{
    QStringList expressions;
    const QDomNodeList list = elementsByTagName(TagIncrement);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        try
        {
            expressions.append(GetParametrString(dom, IncrementFormula));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPattern::IsVariable(const QString &token) const
{
    for (int i = 0; i < builInVariables.size(); ++i)
    {
        if (token.indexOf( builInVariables.at(i) ) == 0)
        {
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPattern::IsPostfixOperator(const QString &token) const
{
    for (int i = 0; i < builInPostfixOperators.size(); ++i)
    {
        if (token.indexOf( builInPostfixOperators.at(i) ) == 0)
        {
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPattern::IsFunction(const QString &token) const
{
    for (int i = 0; i < builInFunctions.size(); ++i)
    {
        if (token.indexOf( builInFunctions.at(i) ) == 0)
        {
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsModified state of the document for cases that do not cover QUndoStack.
 * @return true if the document was modified without using QUndoStack.
 */
bool VAbstractPattern::IsModified() const
{
    return modified;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VAbstractPattern::GetDraw(const QString &name) const
{
    const QDomNodeList draws = documentElement().elementsByTagName(TagDraw);
    for (int i=0; i < draws.size(); ++i)
    {
        QDomElement draw = draws.at(i).toElement();
        if (draw.isNull())
        {
            continue;
        }

        if (draw.attribute(AttrName) == name)
        {
            return draw;
        }
    }
    return QDomElement();
}
