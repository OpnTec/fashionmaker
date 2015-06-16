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
#include "../vmisc/def.h"
#include "exception/vexceptionbadid.h"

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

const QString VAbstractPattern::AttrName        = QStringLiteral("name");
const QString VAbstractPattern::AttrType        = QStringLiteral("type");
const QString VAbstractPattern::AttrPath        = QStringLiteral("path");

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

const QString VAbstractPattern::IncrementName        = QStringLiteral("name");
const QString VAbstractPattern::IncrementBase        = QStringLiteral("base");
const QString VAbstractPattern::IncrementKsize       = QStringLiteral("ksize");
const QString VAbstractPattern::IncrementKgrowth     = QStringLiteral("kgrowth");
const QString VAbstractPattern::IncrementDescription = QStringLiteral("description");

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern::VAbstractPattern(QObject *parent)
    : QObject(parent), VDomDocument(), nameActivPP(QString()), cursor(0), tools(QHash<quint32, VDataTool*>()),
      history(QVector<VToolRecord>())
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern::~VAbstractPattern()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeActivPP set new active pattern piece name.
 * @param name new name.
 * @param parse parser file mode.
 */
void VAbstractPattern::ChangeActivPP(const QString &name, const Document &parse)
{
    Q_ASSERT_X(name.isEmpty() == false, "ChangeActivPP", "name pattern piece is empty");
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
    Q_ASSERT_X(name.isEmpty() == false, "CheckNameDraw", "name draw is empty");
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
    Q_ASSERT_X(name.isEmpty() == false, "GetActivNodeElement", "name draw is empty");
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
    Q_ASSERT_X(newName.isEmpty() == false, "SetNamePP", "new name pattern piece is empty");
    Q_ASSERT_X(oldName.isEmpty() == false, "SetNamePP", "old name pattern piece is empty");

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
    Q_ASSERT_X(name.isEmpty() == false, "appendPP", "name pattern piece is empty");
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
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
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
    QDomNodeList list = elementsByTagName(TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        return GetParametrString(element, AttrPath);
    }
    else
    {
        return QString();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPath(const QString &path)
{
    if (path.isEmpty())
    {
        qDebug()<<"Path to measurements is empty"<<Q_FUNC_INFO;
        return;
    }
    QDomNodeList list = elementsByTagName(TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        SetAttribute(element, AttrPath, path);
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
    QDomNodeList list = elementsByTagName(VAbstractPattern::TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        QStringList units = QStringList() <<"mm" << "cm" << "inch";
        QString unit = GetParametrString(element, AttrUnit);
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
    else
    {
        return Unit::Cm;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType VAbstractPattern::MType() const
{
    QDomNodeList list = elementsByTagName(VAbstractPattern::TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        QString type = GetParametrString(element, AttrType);
        QStringList types = QStringList() << "standard" << "individual";
        switch (types.indexOf(type))
        {
            case 0:// standard
                return MeasurementsType::Standard;
                break;
            case 1:// individual
                return MeasurementsType::Individual;
                break;
            default:
                return MeasurementsType::Individual;
                break;
        }
    }
    else
    {
        return MeasurementsType::Individual;
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
    Q_ASSERT_X(name.isEmpty() == false, "SetActivPP", "name pattern piece is empty");
    this->nameActivPP = name;
    emit ChangedActivPP(name);
}
