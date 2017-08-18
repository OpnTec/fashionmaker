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

#include <QDomNode>
#include <QDomNodeList>
#include <QLatin1String>
#include <QList>
#include <QMessageLogger>
#include <QSet>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>

#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionconversionerror.h"
#include "../qmuparser/qmutokenparser.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/ifcdef.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vtools/tools/vdatatool.h"
#include "vpatternconverter.h"
#include "vdomdocument.h"
#include "vtoolrecord.h"

class QDomElement;

const QString VAbstractPattern::TagPattern          = QStringLiteral("pattern");
const QString VAbstractPattern::TagCalculation      = QStringLiteral("calculation");
const QString VAbstractPattern::TagModeling         = QStringLiteral("modeling");
const QString VAbstractPattern::TagDetails          = QStringLiteral("details");
const QString VAbstractPattern::TagDetail           = QStringLiteral("detail");
const QString VAbstractPattern::TagDescription      = QStringLiteral("description");
const QString VAbstractPattern::TagNotes            = QStringLiteral("notes");
const QString VAbstractPattern::TagImage            = QStringLiteral("image");
const QString VAbstractPattern::TagMeasurements     = QStringLiteral("measurements");
const QString VAbstractPattern::TagIncrements       = QStringLiteral("increments");
const QString VAbstractPattern::TagIncrement        = QStringLiteral("increment");
const QString VAbstractPattern::TagDraw             = QStringLiteral("draw");
const QString VAbstractPattern::TagGroups           = QStringLiteral("groups");
const QString VAbstractPattern::TagGroup            = QStringLiteral("group");
const QString VAbstractPattern::TagGroupItem        = QStringLiteral("item");
const QString VAbstractPattern::TagPoint            = QStringLiteral("point");
const QString VAbstractPattern::TagLine             = QStringLiteral("line");
const QString VAbstractPattern::TagSpline           = QStringLiteral("spline");
const QString VAbstractPattern::TagArc              = QStringLiteral("arc");
const QString VAbstractPattern::TagElArc            = QStringLiteral("elArc");
const QString VAbstractPattern::TagTools            = QStringLiteral("tools");
const QString VAbstractPattern::TagOperation        = QStringLiteral("operation");
const QString VAbstractPattern::TagGradation        = QStringLiteral("gradation");
const QString VAbstractPattern::TagHeights          = QStringLiteral("heights");
const QString VAbstractPattern::TagSizes            = QStringLiteral("sizes");
const QString VAbstractPattern::TagData             = QStringLiteral("data");
const QString VAbstractPattern::TagPatternInfo      = QStringLiteral("patternInfo");
const QString VAbstractPattern::TagPatternName      = QStringLiteral("patternName");
const QString VAbstractPattern::TagPatternNum       = QStringLiteral("patternNumber");
const QString VAbstractPattern::TagCustomerName     = QStringLiteral("customer");
const QString VAbstractPattern::TagCompanyName      = QStringLiteral("company");
const QString VAbstractPattern::TagPatternLabel     = QStringLiteral("patternLabel");
const QString VAbstractPattern::TagGrainline        = QStringLiteral("grainline");
const QString VAbstractPattern::TagPath             = QStringLiteral("path");
const QString VAbstractPattern::TagNodes            = QStringLiteral("nodes");
const QString VAbstractPattern::TagNode             = QStringLiteral("node");

const QString VAbstractPattern::AttrName              = QStringLiteral("name");
const QString VAbstractPattern::AttrVisible           = QStringLiteral("visible");
const QString VAbstractPattern::AttrObject            = QStringLiteral("object");
const QString VAbstractPattern::AttrTool              = QStringLiteral("tool");
const QString VAbstractPattern::AttrType              = QStringLiteral("type");
const QString VAbstractPattern::AttrLetter            = QStringLiteral("letter");
const QString VAbstractPattern::AttrQuantity          = QStringLiteral("quantity");
const QString VAbstractPattern::AttrOnFold            = QStringLiteral("onFold");
const QString VAbstractPattern::AttrArrows            = QStringLiteral("arrows");
const QString VAbstractPattern::AttrNodeReverse       = QStringLiteral("reverse");
const QString VAbstractPattern::AttrNodeExcluded      = QStringLiteral("excluded");
const QString VAbstractPattern::AttrNodePassmark      = QStringLiteral("passmark");
const QString VAbstractPattern::AttrNodePassmarkLine  = QStringLiteral("passmarkLine");
const QString VAbstractPattern::AttrNodePassmarkAngle = QStringLiteral("passmarkAngle");
const QString VAbstractPattern::AttrNodeShowSecondPassmark = QStringLiteral("showSecondPassmark");
const QString VAbstractPattern::AttrSABefore          = QStringLiteral("before");
const QString VAbstractPattern::AttrSAAfter           = QStringLiteral("after");
const QString VAbstractPattern::AttrStart             = QStringLiteral("start");
const QString VAbstractPattern::AttrPath              = QStringLiteral("path");
const QString VAbstractPattern::AttrEnd               = QStringLiteral("end");
const QString VAbstractPattern::AttrIncludeAs         = QStringLiteral("includeAs");
const QString VAbstractPattern::AttrWidth             = QStringLiteral("width");
const QString VAbstractPattern::AttrRotation          = QStringLiteral("rotation");

const QString VAbstractPattern::AttrAll             = QStringLiteral("all");

const QString VAbstractPattern::AttrH50             = QStringLiteral("h50");
const QString VAbstractPattern::AttrH56             = QStringLiteral("h56");
const QString VAbstractPattern::AttrH62             = QStringLiteral("h62");
const QString VAbstractPattern::AttrH68             = QStringLiteral("h68");
const QString VAbstractPattern::AttrH74             = QStringLiteral("h74");
const QString VAbstractPattern::AttrH80             = QStringLiteral("h80");
const QString VAbstractPattern::AttrH86             = QStringLiteral("h86");
const QString VAbstractPattern::AttrH92             = QStringLiteral("h92");
const QString VAbstractPattern::AttrH98             = QStringLiteral("h98");
const QString VAbstractPattern::AttrH104            = QStringLiteral("h104");
const QString VAbstractPattern::AttrH110            = QStringLiteral("h110");
const QString VAbstractPattern::AttrH116            = QStringLiteral("h116");
const QString VAbstractPattern::AttrH122            = QStringLiteral("h122");
const QString VAbstractPattern::AttrH128            = QStringLiteral("h128");
const QString VAbstractPattern::AttrH134            = QStringLiteral("h134");
const QString VAbstractPattern::AttrH140            = QStringLiteral("h140");
const QString VAbstractPattern::AttrH146            = QStringLiteral("h146");
const QString VAbstractPattern::AttrH152            = QStringLiteral("h152");
const QString VAbstractPattern::AttrH158            = QStringLiteral("h158");
const QString VAbstractPattern::AttrH164            = QStringLiteral("h164");
const QString VAbstractPattern::AttrH170            = QStringLiteral("h170");
const QString VAbstractPattern::AttrH176            = QStringLiteral("h176");
const QString VAbstractPattern::AttrH182            = QStringLiteral("h182");
const QString VAbstractPattern::AttrH188            = QStringLiteral("h188");
const QString VAbstractPattern::AttrH194            = QStringLiteral("h194");
const QString VAbstractPattern::AttrH200            = QStringLiteral("h200");

const QString VAbstractPattern::AttrS22             = QStringLiteral("s22");
const QString VAbstractPattern::AttrS24             = QStringLiteral("s24");
const QString VAbstractPattern::AttrS26             = QStringLiteral("s26");
const QString VAbstractPattern::AttrS28             = QStringLiteral("s28");
const QString VAbstractPattern::AttrS30             = QStringLiteral("s30");
const QString VAbstractPattern::AttrS32             = QStringLiteral("s32");
const QString VAbstractPattern::AttrS34             = QStringLiteral("s34");
const QString VAbstractPattern::AttrS36             = QStringLiteral("s36");
const QString VAbstractPattern::AttrS38             = QStringLiteral("s38");
const QString VAbstractPattern::AttrS40             = QStringLiteral("s40");
const QString VAbstractPattern::AttrS42             = QStringLiteral("s42");
const QString VAbstractPattern::AttrS44             = QStringLiteral("s44");
const QString VAbstractPattern::AttrS46             = QStringLiteral("s46");
const QString VAbstractPattern::AttrS48             = QStringLiteral("s48");
const QString VAbstractPattern::AttrS50             = QStringLiteral("s50");
const QString VAbstractPattern::AttrS52             = QStringLiteral("s52");
const QString VAbstractPattern::AttrS54             = QStringLiteral("s54");
const QString VAbstractPattern::AttrS56             = QStringLiteral("s56");
const QString VAbstractPattern::AttrS58             = QStringLiteral("s58");
const QString VAbstractPattern::AttrS60             = QStringLiteral("s60");
const QString VAbstractPattern::AttrS62             = QStringLiteral("s62");
const QString VAbstractPattern::AttrS64             = QStringLiteral("s64");
const QString VAbstractPattern::AttrS66             = QStringLiteral("s66");
const QString VAbstractPattern::AttrS68             = QStringLiteral("s68");
const QString VAbstractPattern::AttrS70             = QStringLiteral("s70");
const QString VAbstractPattern::AttrS72             = QStringLiteral("s72");

const QString VAbstractPattern::AttrCustom          = QStringLiteral("custom");
const QString VAbstractPattern::AttrDefHeight       = QStringLiteral("defHeight");
const QString VAbstractPattern::AttrDefSize         = QStringLiteral("defSize");
const QString VAbstractPattern::AttrExtension       = QStringLiteral("extension");

const QString VAbstractPattern::IncrementName        = QStringLiteral("name");
const QString VAbstractPattern::IncrementFormula     = QStringLiteral("formula");
const QString VAbstractPattern::IncrementDescription = QStringLiteral("description");

const QString VAbstractPattern::NodeArc        = QStringLiteral("NodeArc");
const QString VAbstractPattern::NodeElArc      = QStringLiteral("NodeElArc");
const QString VAbstractPattern::NodePoint      = QStringLiteral("NodePoint");
const QString VAbstractPattern::NodeSpline     = QStringLiteral("NodeSpline");
const QString VAbstractPattern::NodeSplinePath = QStringLiteral("NodeSplinePath");

QHash<quint32, VDataTool*> VAbstractPattern::tools = QHash<quint32, VDataTool*>();
QVector<VLabelTemplateLine> VAbstractPattern::patternLabelLines = QVector<VLabelTemplateLine>();
bool VAbstractPattern::patternLabelWasChanged = false;

namespace
{
void ReadExpressionAttribute(QVector<VFormulaField> &expressions, const QDomElement &element, const QString &attribute)
{
    VFormulaField formula;
    try
    {
        formula.expression = VDomDocument::GetParametrString(element, attribute);
    }
    catch (VExceptionEmptyParameter &e)
    {
        Q_UNUSED(e)
        return;
    }

    formula.element = element;
    formula.attribute = attribute;

    expressions.append(formula);
}
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern::VAbstractPattern(QObject *parent)
    : QObject(parent),
      VDomDocument(),
      nameActivPP(QString()),
      cursor(0),
      toolsOnRemove(QVector<VDataTool*>()),
      history(QVector<VToolRecord>()),
      patternPieces(QStringList()),
      modified(false)
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

    const QVector<VFormulaField> expressions = ListExpressions();
    for (int i=0; i < expressions.size(); ++i)
    {
        // Eval formula
        QScopedPointer<qmu::QmuTokenParser> cal(new qmu::QmuTokenParser(expressions.at(i).expression, false, false));
        const QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
        delete cal.take();

        const QList<QString> tValues = tokens.values();
        for (int j = 0; j < tValues.size(); ++j)
        {
            if (tValues.at(j) == QChar('-'))
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
void VAbstractPattern::ParseGroups(const QDomElement &domElement)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    QMap<quint32, quint32> itemTool;
    QMap<quint32, bool> itemVisibility;

    QDomNode domNode = domElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == TagGroup)
                {
                    VContainer::UpdateId(GetParametrUInt(domElement, AttrId, NULL_ID_STR));

                    const QPair<bool, QMap<quint32, quint32> > groupData = ParseItemElement(domElement);
                    const QMap<quint32, quint32> group = groupData.second;
                    auto i = group.constBegin();
                    while (i != group.constEnd())
                    {
                        if (not itemTool.contains(i.key()))
                        {
                            itemTool.insert(i.key(), i.value());
                        }

                        const bool previous = itemVisibility.value(i.key(), false);
                        itemVisibility.insert(i.key(), previous || groupData.first);
                        ++i;
                    }
                }
            }
        }
        domNode = domNode.nextSibling();
    }

    auto i = itemTool.constBegin();
    while (i != itemTool.constEnd())
    {
        if (tools.contains(i.value()))
        {
            VDataTool* tool = tools.value(i.value());
            tool->GroupVisibility(i.key(), itemVisibility.value(i.key(), true));
        }
        ++i;
    }
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
VDataTool *VAbstractPattern::getTool(quint32 id)
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
void VAbstractPattern::AddTool(quint32 id, VDataTool *tool)
{
    Q_ASSERT_X(id != 0, Q_FUNC_INFO, "id == 0");
    SCASSERT(tool != nullptr)
            tools.insert(id, tool);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::RemoveTool(quint32 id)
{
    tools.remove(id);
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath VAbstractPattern::ParsePieceNodes(const QDomElement &domElement)
{
    VPiecePath path;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull())
        {
            path.Append(ParseSANode(element));
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VAbstractPattern::ParsePieceCSARecords(const QDomElement &domElement)
{
    QVector<CustomSARecord> records;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull())
        {
            CustomSARecord record;
            record.startPoint = GetParametrUInt(element, VAbstractPattern::AttrStart, NULL_ID_STR);
            record.path = GetParametrUInt(element, VAbstractPattern::AttrPath, NULL_ID_STR);
            record.endPoint = GetParametrUInt(element, VAbstractPattern::AttrEnd, NULL_ID_STR);
            record.reverse = GetParametrBool(element, VAbstractPattern::AttrNodeReverse, falseStr);
            record.includeType = static_cast<PiecePathIncludeType>(GetParametrUInt(element,
                                                                                   VAbstractPattern::AttrIncludeAs,
                                                                                   "1"));
            records.append(record);
        }
    }
    return records;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VAbstractPattern::ParsePieceInternalPaths(const QDomElement &domElement)
{
    QVector<quint32> records;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull())
        {
            const quint32 path = GetParametrUInt(element, VAbstractPattern::AttrPath, NULL_ID_STR);
            if (path > NULL_ID)
            {
                records.append(path);
            }
        }
    }
    return records;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VAbstractPattern::ParsePiecePins(const QDomElement &domElement)
{
    QVector<quint32> records;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull())
        {
            const quint32 path = element.text().toUInt();
            if (path > NULL_ID)
            {
                records.append(path);
            }
        }
    }
    return records;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode VAbstractPattern::ParseSANode(const QDomElement &domElement)
{
    const quint32 id = VDomDocument::GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
    const bool reverse = VDomDocument::GetParametrUInt(domElement, VAbstractPattern::AttrNodeReverse, "0");
    const bool excluded = VDomDocument::GetParametrBool(domElement, VAbstractPattern::AttrNodeExcluded, falseStr);
    const QString saBefore = VDomDocument::GetParametrString(domElement, VAbstractPattern::AttrSABefore,
                                                             currentSeamAllowance);
    const QString saAfter = VDomDocument::GetParametrString(domElement, VAbstractPattern::AttrSAAfter,
                                                            currentSeamAllowance);
    const PieceNodeAngle angle = static_cast<PieceNodeAngle>(VDomDocument::GetParametrUInt(domElement, AttrAngle, "0"));

    const bool passmark = VDomDocument::GetParametrBool(domElement, VAbstractPattern::AttrNodePassmark, falseStr);
    const PassmarkLineType passmarkLine = StringToPassmarkLineType(VDomDocument::GetParametrString(domElement,
                                                                                 VAbstractPattern::AttrNodePassmarkLine,
                                                                                                   strOne));
    const PassmarkAngleType passmarkAngle = StringToPassmarkAngleType(VDomDocument::GetParametrString(domElement,
                                                                                VAbstractPattern::AttrNodePassmarkAngle,
                                                                                                   strStraightforward));

    const bool showSecond = VDomDocument::GetParametrBool(domElement, VAbstractPattern::AttrNodeShowSecondPassmark,
                                                          trueStr);

    const QString t = VDomDocument::GetParametrString(domElement, AttrType, VAbstractPattern::NodePoint);
    Tool tool;

    const QStringList types = QStringList() << VAbstractPattern::NodePoint
                                            << VAbstractPattern::NodeArc
                                            << VAbstractPattern::NodeSpline
                                            << VAbstractPattern::NodeSplinePath
                                            << VAbstractPattern::NodeElArc;

    switch (types.indexOf(t))
    {
        case 0: // VAbstractPattern::NodePoint
            tool = Tool::NodePoint;
            break;
        case 1: // VAbstractPattern::NodeArc
            tool = Tool::NodeArc;
            break;
        case 2: // VAbstractPattern::NodeSpline
            tool = Tool::NodeSpline;
            break;
        case 3: // VAbstractPattern::NodeSplinePath
            tool = Tool::NodeSplinePath;
            break;
        case 4: // NodeElArc
            tool = Tool::NodeElArc;
            break;
        default:
            VException e(QObject::tr("Wrong tag name '%1'.").arg(t));
            throw e;
    }
    VPieceNode node(id, tool, reverse);
    node.SetFormulaSABefore(saBefore);
    node.SetFormulaSAAfter(saAfter);
    node.SetAngleType(angle);
    node.SetExcluded(excluded);
    node.SetShowSecondPassmark(showSecond);
    node.SetPassmark(passmark);
    node.SetPassmarkLineType(passmarkLine);
    node.SetPassmarkAngleType(passmarkAngle);

    return node;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::AddToolOnRemove(VDataTool *tool)
{
    SCASSERT(tool != nullptr)
    toolsOnRemove.append(tool);
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
void VAbstractPattern::SetMPath(const QString &path)
{
    if (setTagText(TagMeasurements, path))
    {
        emit patternChanged(false);
        emit UpdatePatternLabel();
    }
    else
    {
        qDebug()<<"Can't save path to measurements"<<Q_FUNC_INFO;
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
                        case Tool::Piece:
                        case Tool::UnionDetails:
                        case Tool::NodeArc:
                        case Tool::NodeElArc:
                        case Tool::NodePoint:
                        case Tool::NodeSpline:
                        case Tool::NodeSplinePath:
                            continue;
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
    map.insert(GHeights::H50, true);
    map.insert(GHeights::H56, true);
    map.insert(GHeights::H62, true);
    map.insert(GHeights::H68, true);
    map.insert(GHeights::H74, true);
    map.insert(GHeights::H80, true);
    map.insert(GHeights::H86, true);
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
    map.insert(GHeights::H200, true);

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
                const QString defValue = trueStr;
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

                        map.insert(GHeights::H50, GetParametrBool(domElement, AttrH50, defValue));
                        map.insert(GHeights::H56, GetParametrBool(domElement, AttrH56, defValue));
                        map.insert(GHeights::H62, GetParametrBool(domElement, AttrH62, defValue));
                        map.insert(GHeights::H68, GetParametrBool(domElement, AttrH68, defValue));
                        map.insert(GHeights::H74, GetParametrBool(domElement, AttrH74, defValue));
                        map.insert(GHeights::H80, GetParametrBool(domElement, AttrH80, defValue));
                        map.insert(GHeights::H86, GetParametrBool(domElement, AttrH86, defValue));
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
                        map.insert(GHeights::H200, GetParametrBool(domElement, AttrH200, defValue));
                        return map;
                    case 1: // TagSizes
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
    if (tags.isEmpty())
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
                            domElement.removeAttribute(AttrH50);
                            domElement.removeAttribute(AttrH56);
                            domElement.removeAttribute(AttrH62);
                            domElement.removeAttribute(AttrH68);
                            domElement.removeAttribute(AttrH74);
                            domElement.removeAttribute(AttrH80);
                            domElement.removeAttribute(AttrH86);
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
                            domElement.removeAttribute(AttrH200);
                        }
                        else
                        {
                            SetAttribute(domElement, AttrH50, options.value(GHeights::H50));
                            SetAttribute(domElement, AttrH56, options.value(GHeights::H56));
                            SetAttribute(domElement, AttrH62, options.value(GHeights::H62));
                            SetAttribute(domElement, AttrH68, options.value(GHeights::H68));
                            SetAttribute(domElement, AttrH74, options.value(GHeights::H74));
                            SetAttribute(domElement, AttrH80, options.value(GHeights::H80));
                            SetAttribute(domElement, AttrH86, options.value(GHeights::H86));
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
                            SetAttribute(domElement, AttrH200, options.value(GHeights::H200));
                        }

                        modified = true;
                        emit patternChanged(false);
                        return;
                    case 1: // TagSizes
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
    map.insert(GSizes::S58, true);
    map.insert(GSizes::S60, true);
    map.insert(GSizes::S62, true);
    map.insert(GSizes::S64, true);
    map.insert(GSizes::S66, true);
    map.insert(GSizes::S68, true);
    map.insert(GSizes::S70, true);
    map.insert(GSizes::S72, true);

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
                const QString defValue = trueStr;
                switch (gTags.indexOf(domElement.tagName()))
                {
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
                        map.insert(GSizes::S58, GetParametrBool(domElement, AttrS58, defValue));
                        map.insert(GSizes::S60, GetParametrBool(domElement, AttrS60, defValue));
                        map.insert(GSizes::S62, GetParametrBool(domElement, AttrS62, defValue));
                        map.insert(GSizes::S64, GetParametrBool(domElement, AttrS64, defValue));
                        map.insert(GSizes::S66, GetParametrBool(domElement, AttrS66, defValue));
                        map.insert(GSizes::S68, GetParametrBool(domElement, AttrS68, defValue));
                        map.insert(GSizes::S70, GetParametrBool(domElement, AttrS70, defValue));
                        map.insert(GSizes::S72, GetParametrBool(domElement, AttrS72, defValue));
                        return map;
                    case 0: // TagHeights
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
    if (tags.isEmpty())
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
                            domElement.removeAttribute(AttrS58);
                            domElement.removeAttribute(AttrS60);
                            domElement.removeAttribute(AttrS62);
                            domElement.removeAttribute(AttrS64);
                            domElement.removeAttribute(AttrS66);
                            domElement.removeAttribute(AttrS68);
                            domElement.removeAttribute(AttrS70);
                            domElement.removeAttribute(AttrS72);
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
                            SetAttribute(domElement, AttrS58, options.value(GSizes::S58));
                            SetAttribute(domElement, AttrS60, options.value(GSizes::S60));
                            SetAttribute(domElement, AttrS62, options.value(GSizes::S62));
                            SetAttribute(domElement, AttrS64, options.value(GSizes::S64));
                            SetAttribute(domElement, AttrS66, options.value(GSizes::S66));
                            SetAttribute(domElement, AttrS68, options.value(GSizes::S68));
                            SetAttribute(domElement, AttrS70, options.value(GSizes::S70));
                            SetAttribute(domElement, AttrS72, options.value(GSizes::S72));
                        }

                        modified = true;
                        emit patternChanged(false);
                        return;
                    case 0: // TagHeights
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
QString VAbstractPattern::GetPatternName() const
{
    return UniqueTagText(TagPatternName);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPatternName(const QString &qsName)
{
    CheckTagExists(TagPatternName);
    setTagText(TagPatternName, qsName);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetCompanyName() const
{
    return UniqueTagText(TagCompanyName);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetCompanyName(const QString& qsName)
{
    CheckTagExists(TagCompanyName);
    setTagText(TagCompanyName, qsName);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetPatternNumber() const
{
    return UniqueTagText(TagPatternNum);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPatternNumber(const QString& qsNum)
{
    CheckTagExists(TagPatternNum);
    setTagText(TagPatternNum, qsNum);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetCustomerName() const
{
    return UniqueTagText(TagCustomerName);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetCustomerName(const QString& qsName)
{
    CheckTagExists(TagCustomerName);
    setTagText(TagCustomerName, qsName);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPatternLabelTemplate(const QVector<VLabelTemplateLine> &lines)
{
    QDomElement tag = CheckTagExists(TagPatternLabel);
    RemoveAllChildren(tag);
    SetLabelTemplate(tag, lines);
    patternLabelLines = lines;
    patternLabelWasChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLabelTemplateLine> VAbstractPattern::GetPatternLabelTemplate() const
{
    if (patternLabelLines.isEmpty())
    {
        const QDomNodeList list = elementsByTagName(TagPatternLabel);
        if (list.isEmpty())
        {
            return QVector<VLabelTemplateLine>();
        }

        patternLabelLines = GetLabelTemplate(list.at(0).toElement());
    }

    return patternLabelLines;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetPatternWasChanged(bool changed)
{
    patternLabelWasChanged = changed;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPattern::GetPatternWasChanged() const
{
    return patternLabelWasChanged;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetImage() const
{
    return UniqueTagText(TagImage);
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetImageExtension() const
{
    const QString defExt =  QStringLiteral("PNG");
    const QDomNodeList nodeList = this->elementsByTagName(TagImage);
    if (nodeList.isEmpty())
    {
        return defExt;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                const QString ext = domElement.attribute(AttrExtension, defExt);
                return ext;
            }
        }
    }
    return defExt;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetImage(const QString &text, const QString &extension)
{
    QDomElement imageElement = CheckTagExists(TagImage);
    setTagText(imageElement, text);
    CheckTagExists(TagImage).setAttribute(AttrExtension, extension);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::DeleteImage()
{
    QDomElement pattern = documentElement();
    pattern.removeChild(CheckTagExists(TagImage));
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
void VAbstractPattern::CheckInLayoutList()
{
    emit UpdateInLayoutList();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SelectedDetail(quint32 id)
{
    emit ShowDetail(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::ToolExists(const quint32 &id)
{
    if (tools.contains(id) == false)
    {
        throw VExceptionBadId(tr("Can't find tool in table."), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath VAbstractPattern::ParsePathNodes(const QDomElement &domElement)
{
    VPiecePath path;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VAbstractPattern::TagNode)
        {
            path.Append(ParseSANode(element));
        }
    }
    return path;
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
QDomElement VAbstractPattern::CheckTagExists(const QString &tag)
{
    const QDomNodeList list = elementsByTagName(tag);
    QDomElement element;
    if (list.isEmpty())
    {
        const QStringList tags = QStringList() << TagUnit << TagImage << TagDescription << TagNotes
                                         << TagGradation << TagPatternName << TagPatternNum << TagCompanyName
                                         << TagCustomerName << TagPatternLabel;
        switch (tags.indexOf(tag))
        {
            case 1: //TagImage
                element = createElement(TagImage);
                break;
            case 2: //TagDescription
                element = createElement(TagDescription);
                break;
            case 3: //TagNotes
                element = createElement(TagNotes);
                break;
            case 4: //TagGradation
            {
                element = createElement(TagGradation);

                QDomElement heights = createElement(TagHeights);
                heights.setAttribute(AttrAll, QLatin1String("true"));
                element.appendChild(heights);

                QDomElement sizes = createElement(TagSizes);
                sizes.setAttribute(AttrAll, QLatin1String("true"));
                element.appendChild(sizes);
                break;
            }
            case 5: // TagPatternName
                element = createElement(TagPatternName);
                break;
            case 6: // TagPatternNum
                element = createElement(TagPatternNum);
                break;
            case 7: // TagCompanyName
                element = createElement(TagCompanyName);
                break;
            case 8: // TagCustomerName
                element = createElement(TagCustomerName);
                break;
            case 9: // TagPatternLabel
                element = createElement(TagPatternLabel);
                break;
            case 0: //TagUnit (Mandatory tag)
            default:
                return QDomElement();
        }
        InsertTag(tags, element);
        return element;
    }
    return list.at(0).toElement();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::InsertTag(const QStringList &tags, const QDomElement &element)
{
    QDomElement pattern = documentElement();
    for (int i = tags.indexOf(element.tagName())-1; i >= 0; --i)
    {
        const QDomNodeList list = elementsByTagName(tags.at(i));
        if (not list.isEmpty())
        {
            pattern.insertAfter(element, list.at(0));
            break;
        }
    }
    SetVersion();
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractPattern::GetIndexActivPP() const
{
    const QDomNodeList drawList = elementsByTagName(TagDraw);

    int index = 0;
    if (not drawList.isEmpty())
    {
        for (int i = 0; i < drawList.size(); ++i)
        {
            QDomElement node = drawList.at(i).toElement();
            if (node.attribute(AttrName) == nameActivPP)
            {
                index = i;
                break;
            }
        }
    }

    return index;
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
QVector<VFormulaField> VAbstractPattern::ListExpressions() const
{
    QVector<VFormulaField> list;

    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    // Note. Tool Union Details also contains formulas, but we don't use them for union and keep only to simplifying
    // working with nodes. Same code for saving reading.
    list << ListPointExpressions();
    list << ListArcExpressions();
    list << ListElArcExpressions();
    list << ListSplineExpressions();
    list << ListIncrementExpressions();
    list << ListOperationExpressions();
    list << ListPathExpressions();
    list << ListPieceExpressions();

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListPointExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment a number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagPoint);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrLength);
        ReadExpressionAttribute(expressions, dom, AttrAngle);
        ReadExpressionAttribute(expressions, dom, AttrC1Radius);
        ReadExpressionAttribute(expressions, dom, AttrC2Radius);
        ReadExpressionAttribute(expressions, dom, AttrCRadius);
        ReadExpressionAttribute(expressions, dom, AttrRadius);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListArcExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagArc);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrAngle1);
        ReadExpressionAttribute(expressions, dom, AttrAngle2);
        ReadExpressionAttribute(expressions, dom, AttrRadius);
        ReadExpressionAttribute(expressions, dom, AttrLength);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListElArcExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagElArc);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrRadius1);
        ReadExpressionAttribute(expressions, dom, AttrRadius2);
        ReadExpressionAttribute(expressions, dom, AttrAngle1);
        ReadExpressionAttribute(expressions, dom, AttrAngle2);
        ReadExpressionAttribute(expressions, dom, AttrRotationAngle);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListSplineExpressions() const
{
    QVector<VFormulaField> expressions;
    expressions << ListPathPointExpressions();
    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListPathPointExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(AttrPathPoint);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrKAsm1);
        ReadExpressionAttribute(expressions, dom, AttrKAsm2);
        ReadExpressionAttribute(expressions, dom, AttrAngle);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListIncrementExpressions() const
{
    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagIncrement);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        ReadExpressionAttribute(expressions, dom, IncrementFormula);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListOperationExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagOperation);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrAngle);
        ReadExpressionAttribute(expressions, dom, AttrLength);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListNodesExpressions(const QDomElement &nodes) const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;

    const QDomNodeList nodeList = nodes.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VAbstractPattern::TagNode)
        {
            ReadExpressionAttribute(expressions, element, VAbstractPattern::AttrSABefore);
            ReadExpressionAttribute(expressions, element, VAbstractPattern::AttrSAAfter);
        }
    }
    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListPathExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagPath);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();
        if (dom.isNull())
        {
            continue;
        }

        expressions << ListNodesExpressions(dom.firstChildElement(TagNodes));
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListGrainlineExpressions(const QDomElement &element) const
{
    QVector<VFormulaField> expressions;
    if (not element.isNull())
    {
        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, element, AttrRotation);
        ReadExpressionAttribute(expressions, element, AttrLength);
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VFormulaField> VAbstractPattern::ListPieceExpressions() const
{
    // Check if new tool doesn't bring new attribute with a formula.
    // If no just increment number.
    // If new tool bring absolutely new type and has formula(s) create new method to cover it.
    Q_STATIC_ASSERT(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53);

    QVector<VFormulaField> expressions;
    const QDomNodeList list = elementsByTagName(TagDetail);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();
        if (dom.isNull())
        {
            continue;
        }

        // Each tag can contains several attributes.
        ReadExpressionAttribute(expressions, dom, AttrWidth);

        expressions << ListNodesExpressions(dom.firstChildElement(TagNodes));
        expressions << ListGrainlineExpressions(dom.firstChildElement(TagGrainline));
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
            if (builInVariables.at(i) == currentLength || builInVariables.at(i) == currentSeamAllowance)
            {
                return token == builInVariables.at(i);
            }
            else
            {
                return true;
            }
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
QPair<bool, QMap<quint32, quint32> > VAbstractPattern::ParseItemElement(const QDomElement &domElement)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        const bool visible = GetParametrBool(domElement, AttrVisible, trueStr);

        QMap<quint32, quint32> items;

        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (not element.isNull() && element.tagName() == TagGroupItem)
            {
                const quint32 object = GetParametrUInt(element, AttrObject, NULL_ID_STR);
                const quint32 tool = GetParametrUInt(element, AttrTool, NULL_ID_STR);
                items.insert(object, tool);
            }
        }

        QPair<bool, QMap<quint32, quint32> > group;
        group.first = visible;
        group.second = items;

        return group;
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating group"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
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
void VAbstractPattern::SetModified(bool modified)
{
    this->modified = modified;
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

//---------------------------------------------------------------------------------------------------------------------
QDomElement VAbstractPattern::CreateGroups()
{
    QDomElement draw;
    if (GetActivDrawElement(draw))
    {
        QDomElement groups = draw.firstChildElement(TagGroups);

        if (groups.isNull())
        {
            groups = createElement(TagGroups);
            draw.appendChild(groups);
        }

        return groups;
    }
    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VAbstractPattern::CreateGroup(quint32 id, const QString &name, const QMap<quint32, quint32> &groupData)
{
    if (id == NULL_ID || groupData.isEmpty())
    {
        return QDomElement();
    }

    QDomElement group = createElement(TagGroup);
    SetAttribute(group, AttrId, id);
    SetAttribute(group, AttrName, name);
    SetAttribute(group, AttrVisible, true);

    auto i = groupData.constBegin();
    while (i != groupData.constEnd())
    {
        QDomElement item = createElement(TagGroupItem);
        item.setAttribute(AttrObject, i.key());
        item.setAttribute(AttrTool, i.value());
        group.appendChild(item);
        ++i;
    }

    return group;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPattern::GetGroupName(quint32 id)
{
    QString name = tr("New group");
    QDomElement groups = CreateGroups();
    if (not groups.isNull())
    {
        QDomElement group = elementById(id, TagGroup);
        if (group.isElement())
        {
            name = GetParametrString(group, AttrName, name);
            return name;
        }
        else
        {
            if (groups.childNodes().isEmpty())
            {
                QDomNode parent = groups.parentNode();
                parent.removeChild(groups);
            }

            qDebug("Can't get group by id = %u.", id);
            return name;
        }
    }
    else
    {
        qDebug("Can't get tag Groups.");
        return name;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetGroupName(quint32 id, const QString &name)
{
    QDomElement groups = CreateGroups();
    if (not groups.isNull())
    {
        QDomElement group = elementById(id, TagGroup);
        if (group.isElement())
        {
            group.setAttribute(AttrName, name);
            modified = true;
            emit patternChanged(false);
        }
        else
        {
            if (groups.childNodes().isEmpty())
            {
                QDomNode parent = groups.parentNode();
                parent.removeChild(groups);
            }

            qDebug("Can't get group by id = %u.", id);
            return;
        }
    }
    else
    {
        qDebug("Can't get tag Groups.");
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QMap<quint32, QPair<QString, bool> > VAbstractPattern::GetGroups()
{
    QMap<quint32, QPair<QString, bool> > data;

    try
    {
        QDomElement groups = CreateGroups();
        if (not groups.isNull())
        {
            QDomNode domNode = groups.firstChild();
            while (domNode.isNull() == false)
            {
                if (domNode.isElement())
                {
                    const QDomElement group = domNode.toElement();
                    if (group.isNull() == false)
                    {
                        if (group.tagName() == TagGroup)
                        {
                            const quint32 id = GetParametrUInt(group, AttrId, "0");
                            const bool visible = GetParametrBool(group, AttrVisible, trueStr);
                            const QString name = GetParametrString(group, AttrName, tr("New group"));

                            data.insert(id, qMakePair(name, visible));
                        }
                    }
                }
                domNode = domNode.nextSibling();
            }
        }
        else
        {
            qDebug("Can't get tag Groups.");
        }
    }
    catch (const VExceptionConversionError &)
    {
        return QMap<quint32, QPair<QString, bool> >();
    }

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPattern::GetGroupVisivility(quint32 id)
{
    QDomElement group = elementById(id, TagGroup);
    if (group.isElement())
    {
        return GetParametrBool(group, AttrVisible, trueStr);
    }
    else
    {
        qDebug("Can't get group by id = %u.", id);
        return true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPattern::SetGroupVisivility(quint32 id, bool visible)
{
    QDomElement group = elementById(id, TagGroup);
    if (group.isElement())
    {
        SetAttribute(group, AttrVisible, visible);
        modified = true;
        emit patternChanged(false);

        QDomElement groups = CreateGroups();
        if (not groups.isNull())
        {
            ParseGroups(groups);
        }
    }
    else
    {
        qDebug("Can't get group by id = %u.", id);
        return;
    }
}
