/************************************************************************
 **
 **  @file   vpattern.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 2, 2014
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

#include "vpattern.h"
#include "../tools/vdatatool.h"
#include "../tools/vtooldetail.h"
#include "../tools/vtooluniondetails.h"
#include "../tools/drawTools/drawtools.h"
#include "../tools/nodeDetails/nodedetails.h"
#include "../libs/ifc/exception/vexceptionobjecterror.h"
#include "../libs/ifc/exception/vexceptionwrongid.h"
#include "../libs/ifc/exception/vexceptionconversionerror.h"
#include "../libs/ifc/exception/vexceptionemptyparameter.h"
#include "../libs/ifc/exception/vexceptionundo.h"
#include "../libs/ifc/xml/vpatternconverter.h"
#include "../core/undoevent.h"
#include "../core/vsettings.h"
#include "vstandardmeasurements.h"
#include "vindividualmeasurements.h"
#include "../../libs/qmuparser/qmuparsererror.h"
#include "../geometry/varc.h"

#include <QMessageBox>
#include <QUndoStack>
#include <QtCore/qmath.h>

const QString VPattern::TagPattern      = QStringLiteral("pattern");
const QString VPattern::TagCalculation  = QStringLiteral("calculation");
const QString VPattern::TagModeling     = QStringLiteral("modeling");
const QString VPattern::TagDetails      = QStringLiteral("details");
const QString VPattern::TagAuthor       = QStringLiteral("author");
const QString VPattern::TagDescription  = QStringLiteral("description");
const QString VPattern::TagNotes        = QStringLiteral("notes");
const QString VPattern::TagMeasurements = QStringLiteral("measurements");
const QString VPattern::TagIncrements   = QStringLiteral("increments");
const QString VPattern::TagIncrement    = QStringLiteral("increment");
const QString VPattern::TagDraw         = QStringLiteral("draw");
const QString VPattern::TagPoint        = QStringLiteral("point");
const QString VPattern::TagLine         = QStringLiteral("line");
const QString VPattern::TagSpline       = QStringLiteral("spline");
const QString VPattern::TagArc          = QStringLiteral("arc");
const QString VPattern::TagTools        = QStringLiteral("tools");
const QString VPattern::TagGradation    = QStringLiteral("gradation");
const QString VPattern::TagHeights      = QStringLiteral("heights");
const QString VPattern::TagSizes        = QStringLiteral("sizes");

const QString VPattern::AttrName        = QStringLiteral("name");
const QString VPattern::AttrType        = QStringLiteral("type");
const QString VPattern::AttrPath        = QStringLiteral("path");

const QString VPattern::AttrAll         = QStringLiteral("all");

const QString VPattern::AttrH92         = QStringLiteral("h92");
const QString VPattern::AttrH98         = QStringLiteral("h98");
const QString VPattern::AttrH104        = QStringLiteral("h104");
const QString VPattern::AttrH110        = QStringLiteral("h110");
const QString VPattern::AttrH116        = QStringLiteral("h116");
const QString VPattern::AttrH122        = QStringLiteral("h122");
const QString VPattern::AttrH128        = QStringLiteral("h128");
const QString VPattern::AttrH134        = QStringLiteral("h134");
const QString VPattern::AttrH140        = QStringLiteral("h140");
const QString VPattern::AttrH146        = QStringLiteral("h146");
const QString VPattern::AttrH152        = QStringLiteral("h152");
const QString VPattern::AttrH158        = QStringLiteral("h158");
const QString VPattern::AttrH164        = QStringLiteral("h164");
const QString VPattern::AttrH170        = QStringLiteral("h170");
const QString VPattern::AttrH176        = QStringLiteral("h176");
const QString VPattern::AttrH182        = QStringLiteral("h182");
const QString VPattern::AttrH188        = QStringLiteral("h188");
const QString VPattern::AttrH194        = QStringLiteral("h194");

const QString VPattern::AttrS22         = QStringLiteral("s22");
const QString VPattern::AttrS24         = QStringLiteral("s24");
const QString VPattern::AttrS26         = QStringLiteral("s26");
const QString VPattern::AttrS28         = QStringLiteral("s28");
const QString VPattern::AttrS30         = QStringLiteral("s30");
const QString VPattern::AttrS32         = QStringLiteral("s32");
const QString VPattern::AttrS34         = QStringLiteral("s34");
const QString VPattern::AttrS36         = QStringLiteral("s36");
const QString VPattern::AttrS38         = QStringLiteral("s38");
const QString VPattern::AttrS40         = QStringLiteral("s40");
const QString VPattern::AttrS42         = QStringLiteral("s42");
const QString VPattern::AttrS44         = QStringLiteral("s44");
const QString VPattern::AttrS46         = QStringLiteral("s46");
const QString VPattern::AttrS48         = QStringLiteral("s48");
const QString VPattern::AttrS50         = QStringLiteral("s50");
const QString VPattern::AttrS52         = QStringLiteral("s52");
const QString VPattern::AttrS54         = QStringLiteral("s54");
const QString VPattern::AttrS56         = QStringLiteral("s56");

const QString VPattern::IncrementName        = QStringLiteral("name");
const QString VPattern::IncrementBase        = QStringLiteral("base");
const QString VPattern::IncrementKsize       = QStringLiteral("ksize");
const QString VPattern::IncrementKgrowth     = QStringLiteral("kgrowth");
const QString VPattern::IncrementDescription = QStringLiteral("description");

//---------------------------------------------------------------------------------------------------------------------
VPattern::VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw,
                   VMainGraphicsScene *sceneDetail, QObject *parent)
    : QObject(parent), VDomDocument(), data(data), nameActivPP(QString()), tools(QHash<quint32, VDataTool*>()),
      history(QVector<VToolRecord>()), cursor(0), patternPieces(QStringList()), mode(mode), sceneDraw(sceneDraw),
      sceneDetail(sceneDetail)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CreateEmptyFile create minimal empty file.
 * @param tablePath path to measurement file (standard or individual)
 */
void VPattern::CreateEmptyFile(const QString &tablePath)
{
    if (tablePath.isEmpty())
    {
        throw VException("Path to measurement table empty.");
    }
    this->clear();
    QDomElement patternElement = this->createElement(TagPattern);

    patternElement.appendChild(createComment("Valentina pattern format."));

    QDomElement version = createElement(TagVersion);
    QDomText newNodeText = createTextNode(VPatternConverter::PatternMaxVerStr);
    version.appendChild(newNodeText);
    patternElement.appendChild(version);

    patternElement.appendChild(createElement(TagAuthor));
    patternElement.appendChild(createElement(TagDescription));
    patternElement.appendChild(createElement(TagNotes));

    QDomElement measurements = createElement(TagMeasurements);
    SetAttribute(measurements, AttrUnit, UnitsToStr(qApp->patternUnit()));
    SetAttribute(measurements, AttrType, qApp->patternType());
    SetAttribute(measurements, AttrPath, tablePath);
    patternElement.appendChild(measurements);

    patternElement.appendChild(createElement(TagIncrements));

    this->appendChild(patternElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeActivPP set new active pattern piece name.
 * @param name new name.
 * @param parse parser file mode.
 */
void VPattern::ChangeActivPP(const QString &name, const Document &parse)
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
 * @brief GetActivDrawElement return draw tag for current pattern peace.
 * @param element draw tag.
 * @return true if found.
 */
bool VPattern::GetActivDrawElement(QDomElement &element) const
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
 * @brief appendPP add new pattern piece.
 *
 * Method check if not exist pattern piece with the same name and change name active pattern piece name, send signal
 * about change pattern piece. Doen't add pattern piece to file structure. This task make SPoint tool.
 * @param name pattern peace name.
 * @return true if success.
 */
bool VPattern::appendPP(const QString &name)
{
    Q_ASSERT_X(name.isEmpty() == false, "appendPP", "name pattern piece is empty");
    if (name.isEmpty())
    {
        return false;
    }
    if (CheckExistNamePP(name) == false)
    {
        if (nameActivPP.isEmpty())
        {
            SetActivPP(name);
        }
        else
        {
            this->nameActivPP = name;
            emit ChangedActivPP(name);
        }
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeNamePP change pattern piece name.
 * @param oldName old pattern piece name.
 * @param newName new pattern piece name.
 * @return true if success.
 */
bool VPattern::ChangeNamePP(const QString& oldName, const QString &newName)
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
 * @brief Parse parse file.
 * @param parse parser file mode.
 */
void VPattern::Parse(const Document &parse)
{
    qCDebug(vXML)<<"Parsing pattern.";
    switch (parse)
    {
        case Document::FullParse:
            qCDebug(vXML)<<"Full parse.";
            break;
        case Document::LiteParse:
            qCDebug(vXML)<<"Lite parse.";
            break;
        case Document::LitePPParse:
            qCDebug(vXML)<<"Lite pattern piece parse.";
            break;
        default:
            break;
    }

    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    QStringList tags = QStringList() << TagDraw << TagIncrements << TagAuthor << TagDescription << TagNotes
                                        << TagMeasurements << TagVersion << TagGradation;
    PrepareForParse(parse);
    QDomNode domNode = documentElement().firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                switch (tags.indexOf(domElement.tagName()))
                {
                    case 0: // TagDraw
                        qCDebug(vXML)<<"Tag draw.";
                        if (parse == Document::FullParse)
                        {
                            if (nameActivPP.isEmpty())
                            {
                                SetActivPP(GetParametrString(domElement, AttrName));
                            }
                            else
                            {
                                ChangeActivPP(GetParametrString(domElement, AttrName));
                            }
                            patternPieces << GetParametrString(domElement, AttrName);
                        }
                        else
                        {
                            ChangeActivPP(GetParametrString(domElement, AttrName), Document::LiteParse);
                        }
                        ParseDrawElement(domElement, parse);
                        break;
                    case 1: // TagIncrements
                        qCDebug(vXML)<<"Tag increments.";
                        ParseIncrementsElement(domElement);
                        break;
                    case 2: // TagAuthor
                        qCDebug(vXML)<<"Tag author.";
                        break;
                    case 3: // TagDescription
                        qCDebug(vXML)<<"Tag description.";
                        break;
                    case 4: // TagNotes
                        qCDebug(vXML)<<"Tag notes.";
                        break;
                    case 5: // TagMeasurements
                        qCDebug(vXML)<<"Tag measurements.";
                        break;
                    case 6: // TagVersion
                        qCDebug(vXML)<<"Tag version.";
                        break;
                    case 7: // TagGradation
                        qCDebug(vXML)<<"Tag gradation.";
                        break;
                    default:
                        qCDebug(vXML)<<"Wrong tag name"<<domElement.tagName();
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
    emit CheckLayout();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTool return tool from tool list.
 * @param id tool id.
 * @return tool.
 */
VDataTool *VPattern::getTool(const quint32 &id)
{
    ToolExists(id);
    return tools.value(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ToolExists(const quint32 &id) const
{
    if (tools.contains(id) == false)
    {
        throw VExceptionBadId(tr("Can't find tool in table."), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCursor set cursor.
 * @param value cursor.
 */
void VPattern::setCursor(const quint32 &value)
{
    if (cursor != value)
    {
        cursor = value;
        emit ChangedCursor(cursor);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentData set current data set.
 *
 * Each time after parsing need set correct data set for current pattern piece. After parsing it is always last.
 * Current data set for pattern pice it is data set for last object in pattern pice (point, arc, spline, spline path so
 * on).
 */
void VPattern::setCurrentData()
{
    if (*mode == Draw::Calculation)
    {
        if (CountPP() > 1)//don't need upadate data if we have only one pattern piece
        {
            qCDebug(vXML)<<"Setting current data";
            qCDebug(vXML)<<"Current PP name"<<nameActivPP;
            qCDebug(vXML)<<"PP count"<<CountPP();

            quint32 id = 0;
            if (history.size() == 0)
            {
                qCDebug(vXML)<<"History is empty!";
                return;
            }
            for (qint32 i = 0; i < history.size(); ++i)
            {
                const VToolRecord tool = history.at(i);
                if (tool.getNameDraw() == nameActivPP)
                {
                    id = tool.getId();
                }
            }
            qCDebug(vXML)<<"Resoring data from tool with id"<<id;
            if (id == NULL_ID)
            {
                qCDebug(vXML)<<"Could not find record for this current pattern piece"<<nameActivPP;

                const VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                qCDebug(vXML)<<"Taking record with id"<<id<<"from PP"<<tool.getNameDraw();
                if (id == NULL_ID)
                {
                    qCDebug(vXML)<<"Bad id for last record in history.";
                    return;
                }
            }
            if (tools.size() > 0)
            {
                try
                {
                    ToolExists(id);
                }
                catch (VExceptionBadId &e)
                {
                    qCDebug(vXML)<<"List of tools doesn't containe id="<<id;
                    return;
                }

                const VDataTool *vTool = tools.value(id);
                *data = vTool->getData();
                qCDebug(vXML)<<"Data successfully updated.";
            }
            else
            {
                qCDebug(vXML)<<"List of tools is empty!";
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddTool add tool to list tools.
 * @param id tool id.
 * @param tool tool.
 */
void VPattern::AddTool(const quint32 &id, VDataTool *tool)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    SCASSERT(tool != nullptr);
    tools.insert(id, tool);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateToolData update tool in list tools.
 * @param id tool id.
 * @param data container with variables.
 */
void VPattern::UpdateToolData(const quint32 &id, VContainer *data)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    SCASSERT(data != nullptr);
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr);
    tool->VDataTool::setData(data);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IncrementReferens increment reference parent objects.
 * @param id parent object id.
 */
void VPattern::IncrementReferens(quint32 id) const
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr);
    tool->incrementReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DecrementReferens decrement reference parent objects.
 * @param id parent object id.
 */
void VPattern::DecrementReferens(quint32 id) const
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr);
    tool->decrementReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SPointActiveDraw return id base point current pattern peace.
 * @return id base point.
 */
quint32 VPattern::SPointActiveDraw()
{
    QDomElement calcElement;
    if (GetActivNodeElement(TagCalculation, calcElement))
    {
        const QDomNode domNode = calcElement.firstChild();
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == VToolPoint::TagName &&
                        domElement.attribute(AttrType, "") == VToolSinglePoint::ToolType)
                {
                    return GetParametrId(domElement);
                }
            }
        }
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckNameDraw check if exist pattern peace with this name.
 * @param name pattern peace name.
 * @return true if exist.
 */
bool VPattern::CheckExistNamePP(const QString &name) const
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
 * @brief SetActivPP set current pattern piece.
 * @param name pattern peace name.
 */
void VPattern::SetActivPP(const QString &name)
{
    Q_ASSERT_X(name.isEmpty() == false, "SetActivPP", "name pattern piece is empty");
    this->nameActivPP = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetActivNodeElement find element in current pattern piece by name.
 * @param name name tag.
 * @param element element.
 * @return true if found.
 */
bool VPattern::GetActivNodeElement(const QString &name, QDomElement &element) const
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
QString VPattern::MPath() const
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
void VPattern::SetPath(const QString &path)
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
Unit VPattern::MUnit() const
{
    QDomNodeList list = elementsByTagName(VPattern::TagMeasurements);
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
MeasurementsType VPattern::MType() const
{
    QDomNodeList list = elementsByTagName(VPattern::TagMeasurements);
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
bool VPattern::SaveDocument(const QString &fileName, QString &error) const
{
    try
    {
        TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error no unique id."), qApp->getMainWindow());
        return false;
    }

    return VDomDocument::SaveDocument(fileName, error);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LiteParseTree lite parse file.
 */
void VPattern::LiteParseTree(const Document &parse)
{
    // Save name current pattern piece
    QString namePP = nameActivPP;

    try
    {
        emit SetEnabledGUI(true);
        switch (parse)
        {
            case Document::LitePPParse:
                ParseCurrentPP();
                break;
            case Document::LiteParse:
                Parse(parse);
                break;
            case Document::FullParse:
                qCWarning(vXML)<<"Lite parsing doesn't support full parsing";
                break;
            default:
                break;
        }
    }
    catch (const VExceptionUndo &e)
    {
        Q_UNUSED(e);
        /* If user want undo last operation before undo we need finish broken redo operation. For those we post event
         * myself. Later in method customEvent call undo.*/
        QApplication::postEvent(this, new UndoEvent());
        return;
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), qApp->getMainWindow());
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value."), qApp->getMainWindow());
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter."), qApp->getMainWindow());
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id."), qApp->getMainWindow());
        emit SetEnabledGUI(false);
        return;
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), qApp->getMainWindow());
        emit SetEnabledGUI(false);
        return;
    }
    catch (const std::bad_alloc &)
    {
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        QMessageBox::critical(qApp->getMainWindow(), tr("Critical error!"), tr("Error parsing file (std::bad_alloc)."),
                              QMessageBox::Ok, QMessageBox::Ok);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        emit SetEnabledGUI(false);
        return;
    }

    // Restore name current pattern piece
    nameActivPP = namePP;
    qCDebug(vXML)<<"Current pattern piece"<<nameActivPP;
    setCurrentData();
    emit FullUpdateFromFile();
    // Recalculate scene rect
    VAbstractTool::NewSceneRect(sceneDraw, qApp->getSceneView());
    VAbstractTool::NewSceneRect(sceneDetail, qApp->getSceneView());
    qCDebug(vXML)<<"Scene size updated.";
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveLiteChange we have unsaved change.
 */
void VPattern::haveLiteChange()
{
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowHistoryTool hightlight tool.
 * @param id tool id.
 * @param color hightlight color.
 * @param enable enable or diasable hightlight.
 */
void VPattern::ShowHistoryTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    emit ShowTool(id, color, enable);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::NeedFullParsing()
{
    emit UndoCommand();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ClearScene()
{
    emit ClearMainWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::customEvent(QEvent *event)
{
    if (event->type() == UNDO_EVENT)
    {
        qApp->getUndoStack()->undo();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDrawElement parse draw tag.
 * @param node node.
 * @param parse parser file mode.
 */
void VPattern::ParseDrawElement(const QDomNode &node, const Document &parse)
{
    QStringList tags = QStringList() << TagCalculation << TagModeling << TagDetails;
    QDomNode domNode = node.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                switch (tags.indexOf(domElement.tagName()))
                {
                    case 0: // TagCalculation
                        qCDebug(vXML)<<"Tag calculation.";
                        data->ClearCalculationGObjects();
                        ParseDrawMode(domElement, parse, Draw::Calculation);
                        break;
                    case 1: // TagModeling
                        qCDebug(vXML)<<"Tag modeling.";
                        ParseDrawMode(domElement, parse, Draw::Modeling);
                        break;
                    case 2: // TagDetails
                        qCDebug(vXML)<<"Tag details.";
                        ParseDetails(domElement, parse);
                        break;
                    default:
                        qCDebug(vXML)<<"Wrong tag name";
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDrawMode parse draw tag with draw mode.
 * @param node node.
 * @param parse parser file mode.
 * @param mode draw mode.
 */
void VPattern::ParseDrawMode(const QDomNode &node, const Document &parse, const Draw &mode)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    VMainGraphicsScene *scene = nullptr;
    if (mode == Draw::Calculation)
    {
        scene = sceneDraw;
    }
    else
    {
        scene = sceneDetail;
    }
    QStringList tags = QStringList() << TagPoint << TagLine << TagSpline << TagArc << TagTools;
    const QDomNodeList nodeList = node.childNodes();
    const qint32 num = nodeList.size();
    for (qint32 i = 0; i < num; ++i)
    {
        QDomElement domElement = nodeList.at(i).toElement();
        if (domElement.isNull() == false)
        {
            switch (tags.indexOf(domElement.tagName()))
            {
                case 0: // TagPoint
                    qCDebug(vXML)<<"Tag point.";
                    ParsePointElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 1: // TagLine
                    qCDebug(vXML)<<"Tag line.";
                    ParseLineElement(scene, domElement, parse);
                    break;
                case 2: // TagSpline
                    qCDebug(vXML)<<"Tag spline.";
                    ParseSplineElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 3: // TagArc
                    qCDebug(vXML)<<"Tag arc.";
                    ParseArcElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 4: // TagTools
                    qCDebug(vXML)<<"Tag tools.";
                    ParseToolsElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                default:
                    qCDebug(vXML)<<"Wrong tag name";
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDetailElement parse detail tag.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetailElement(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    try
    {
        VDetail detail;
        const quint32 id = GetParametrId(domElement);
        detail.setName(GetParametrString(domElement, VAbstractTool::AttrName, ""));
        detail.setMx(qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "0.0")));
        detail.setMy(qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "0.0")));
        detail.setSeamAllowance(GetParametrUInt(domElement, VToolDetail::AttrSupplement, "1"));
        detail.setWidth(GetParametrDouble(domElement, VToolDetail::AttrWidth, "10.0"));
        detail.setClosed(GetParametrUInt(domElement, VToolDetail::AttrClosed, "1"));

        QStringList types = QStringList() << VToolDetail::NodePoint << VToolDetail::NodeArc << VToolDetail::NodeSpline
                                          << VToolDetail::NodeSplinePath;
        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (element.isNull() == false)
            {
                if (element.tagName() == VToolDetail::TagNode)
                {
                    const quint32 id = GetParametrUInt(element, VToolDetail::AttrIdObject, NULL_ID_STR);
                    const qreal mx = qApp->toPixel(GetParametrDouble(element, VAbstractTool::AttrMx, "0.0"));
                    const qreal my = qApp->toPixel(GetParametrDouble(element, VAbstractTool::AttrMy, "0.0"));
                    const bool reverse = GetParametrUInt(element, VToolDetail::AttrReverse, "0");
                    const NodeDetail nodeType = NodeDetail::Contour;

                    const QString t = GetParametrString(element, AttrType, "NodePoint");
                    Tool tool;

                    switch (types.indexOf(t))
                    {
                        case 0: // VToolDetail::NodePoint
                            tool = Tool::NodePoint;
                            break;
                        case 1: // VToolDetail::NodeArc
                            tool = Tool::NodeArc;
                            break;
                        case 2: // VToolDetail::NodeSpline
                            tool = Tool::NodeSpline;
                            break;
                        case 3: // VToolDetail::NodeSplinePath
                            tool = Tool::NodeSplinePath;
                            break;
                        default:
                            qDebug()<<"Wrong node type."<<Q_FUNC_INFO;
                            continue;
                            break;
                    }
                    detail.append(VNodeDetail(id, tool, nodeType, mx, my, reverse));
                }
            }
        }
        VToolDetail::Create(id, detail, sceneDetail, this, data, parse, Source::FromFile);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating detail"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDetails parse details tag.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetails(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    QDomNode domNode = domElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == VToolDetail::TagName)
                {
                    ParseDetailElement(domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx, qreal &my,
                                      QString &typeLine, QString &lineColor)
{
    PointsCommonAttributes(domElement, id, name, mx, my);
    typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
    lineColor = GetParametrString(domElement, VAbstractTool::AttrLineColor, VAbstractTool::ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx, qreal &my)
{
    PointsCommonAttributes(domElement, id, mx, my);
    name = GetParametrString(domElement, VAbstractTool::AttrName, "A");
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, qreal &mx, qreal &my)
{
    ToolsCommonAttributes(domElement, id);
    mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
    my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParsePointElement parse point tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of point.
 */
void VPattern::ParsePointElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                 const Document &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of point is empty");

    quint32 id = 0;
    QString name;
    qreal mx = 0;
    qreal my = 0;
    QString typeLine;
    QString lineColor;

    QStringList points = QStringList() << VToolSinglePoint::ToolType << VToolEndLine::ToolType
                                       << VToolAlongLine::ToolType << VToolShoulderPoint::ToolType
                                       << VToolNormal::ToolType << VToolBisector::ToolType
                                       << VToolLineIntersect::ToolType << VToolPointOfContact::ToolType
                                       << VNodePoint::ToolType << VToolHeight::ToolType << VToolTriangle::ToolType
                                       << VToolPointOfIntersection::ToolType << VToolCutSpline::ToolType
                                       << VToolCutSplinePath::ToolType << VToolCutArc::ToolType
                                       << VToolLineIntersectAxis::ToolType << VToolCurveIntersectAxis::ToolType;
    switch (points.indexOf(type))
    {
        case 0: //VToolSinglePoint::ToolType
        {
            VToolSinglePoint *spoint = 0;
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const qreal x = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrX, "10.0"));
                const qreal y = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrY, "10.0"));

                data->UpdateGObject(id, new VPointF(x, y, name, mx, my));
                VDrawTool::AddRecord(id, Tool::SinglePoint, this);
                if (parse != Document::FullParse)
                {
                    UpdateToolData(id, data);
                }
                if (parse == Document::FullParse)
                {
                    spoint = new VToolSinglePoint(this, data, id, Source::FromFile, nameActivPP, MPath());
                    scene->addItem(spoint);
                    connect(spoint, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    connect(scene, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
                    connect(scene, &VMainGraphicsScene::DisableItem, spoint, &VToolSinglePoint::Disable);
                    tools[id] = spoint;
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating single point"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                scene->removeItem(spoint);
                delete spoint;
                throw excep;
            }
            break;
        }
        case 1: //VToolEndLine::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;

                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, NULL_ID_STR);

                const QString angle = GetParametrString(domElement, VAbstractTool::AttrAngle, "0.0");
                QString angleFix = angle;

                VToolEndLine::Create(id, name, typeLine, lineColor, f, angleFix, basePointId, mx, my, scene, this, data,
                                     parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula || angleFix != angle)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    SetAttribute(domElement, VAbstractTool::AttrAngle, angleFix);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of end line"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of end line"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 2: //VToolAlongLine::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);

                VToolAlongLine::Create(id, name, typeLine, lineColor, f, firstPointId, secondPointId, mx, my, scene,
                                       this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point along line"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point along line"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 3: //VToolShoulderPoint::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 p1Line = GetParametrUInt(domElement, VAbstractTool::AttrP1Line, NULL_ID_STR);
                const quint32 p2Line = GetParametrUInt(domElement, VAbstractTool::AttrP2Line, NULL_ID_STR);
                const quint32 pShoulder = GetParametrUInt(domElement, VAbstractTool::AttrPShoulder, NULL_ID_STR);

                VToolShoulderPoint::Create(id, f, p1Line, p2Line, pShoulder, typeLine, lineColor, name, mx, my, scene,
                                           this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of shoulder"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of shoulder"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 4: //VToolNormal::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);
                const qreal angle = GetParametrDouble(domElement, VAbstractTool::AttrAngle, "0.0");

                VToolNormal::Create(id, f, firstPointId, secondPointId, typeLine, lineColor, name, angle, mx, my, scene,
                                    this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of normal"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of normal"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 5: //VToolBisector::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);
                const quint32 thirdPointId = GetParametrUInt(domElement, VAbstractTool::AttrThirdPoint, NULL_ID_STR);

                VToolBisector::Create(id, f, firstPointId, secondPointId, thirdPointId,
                                    typeLine, lineColor, name, mx, my, scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of bisector"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of bisector"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 6: //VToolLineIntersect::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 p1Line1Id = GetParametrUInt(domElement, VAbstractTool::AttrP1Line1, NULL_ID_STR);
                const quint32 p2Line1Id = GetParametrUInt(domElement, VAbstractTool::AttrP2Line1, NULL_ID_STR);
                const quint32 p1Line2Id = GetParametrUInt(domElement, VAbstractTool::AttrP1Line2, NULL_ID_STR);
                const quint32 p2Line2Id = GetParametrUInt(domElement, VAbstractTool::AttrP2Line2, NULL_ID_STR);

                VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name,
                                            mx, my, scene, this, data, parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of lineintersection"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 7: //VToolPointOfContact::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "0");
                QString f = radius;//need for saving fixed formula;
                const quint32 center = GetParametrUInt(domElement, VAbstractTool::AttrCenter, NULL_ID_STR);
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);

                VToolPointOfContact::Create(id, f, center, firstPointId, secondPointId, name, mx, my, scene, this,
                                            data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != radius)
                {
                    SetAttribute(domElement, VAbstractTool::AttrRadius, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of contact"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of contact"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 8: //VNodePoint::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, mx, my);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, NULL_ID_STR);
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idObject );
                data->UpdateGObject(id, new VPointF(point->toQPointF(), point->name(), mx, my, idObject,
                                                    Draw::Modeling));
                VNodePoint::Create(this, data, id, idObject, parse, Source::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling point"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 9: //VToolHeight::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, NULL_ID_STR);
                const quint32 p1LineId = GetParametrUInt(domElement, VAbstractTool::AttrP1Line, NULL_ID_STR);
                const quint32 p2LineId = GetParametrUInt(domElement, VAbstractTool::AttrP2Line, NULL_ID_STR);

                VToolHeight::Create(id, name, typeLine, lineColor, basePointId, p1LineId, p2LineId,
                                    mx, my, scene, this, data, parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating height"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 10: //VToolTriangle::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 axisP1Id = GetParametrUInt(domElement, VAbstractTool::AttrAxisP1, NULL_ID_STR);
                const quint32 axisP2Id = GetParametrUInt(domElement, VAbstractTool::AttrAxisP2, NULL_ID_STR);
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);

                VToolTriangle::Create(id, name, axisP1Id, axisP2Id, firstPointId, secondPointId, mx, my, scene, this,
                                      data, parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating triangle"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 11: //VToolPointOfIntersection::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);

                VToolPointOfIntersection::Create(id, name, firstPointId, secondPointId, mx, my, scene, this, data,
                                                 parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 12: //VToolCutSpline::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 splineId = GetParametrUInt(domElement, VToolCutSpline::AttrSpline, NULL_ID_STR);

                VToolCutSpline::Create(id, name, f, splineId, mx, my, scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline point"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline point"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 13: //VToolCutSplinePath::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 splinePathId = GetParametrUInt(domElement, VToolCutSplinePath::AttrSplinePath,
                                                             NULL_ID_STR);

                VToolCutSplinePath::Create(id, name, f, splinePathId, mx, my, scene, this, data, parse,
                                           Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline path point"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline path point"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 14: //VToolCutArc::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 arcId = GetParametrUInt(domElement, VToolCutArc::AttrArc, NULL_ID_STR);

                VToolCutArc::Create(id, name, f, arcId, mx, my, scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut arc point"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut arc point"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 15: //VToolLineIntersectAxis::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, NULL_ID_STR);
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrP1Line, NULL_ID_STR);
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrP2Line, NULL_ID_STR);

                const QString angle = GetParametrString(domElement, VAbstractTool::AttrAngle, "0.0");
                QString angleFix = angle;

                VToolLineIntersectAxis::Create(id, name, typeLine, lineColor, angleFix, basePointId, firstPointId,
                                               secondPointId, mx, my, scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (angleFix != angle)
                {
                    SetAttribute(domElement, VAbstractTool::AttrAngle, angleFix);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection line and axis"),
                                            domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection line and axis"),
                                            domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 16: //VToolCurveIntersectAxis::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, NULL_ID_STR);
                const quint32 curveId = GetParametrUInt(domElement, VAbstractTool::AttrCurve, NULL_ID_STR);
                const QString angle = GetParametrString(domElement, VAbstractTool::AttrAngle, "0.0");
                QString angleFix = angle;

                VToolCurveIntersectAxis::Create(id, name, typeLine, lineColor, angleFix, basePointId, curveId, mx, my,
                                                scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (angleFix != angle)
                {
                    SetAttribute(domElement, VAbstractTool::AttrAngle, angleFix);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection curve and axis"),
                                            domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection curve and axis"),
                                            domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        default:
            qDebug() << "Illegal point type in VDomDocument::ParsePointElement().";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseLineElement parse line tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseLineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                const Document &parse)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    quint32 id = 0;
    try
    {
        ToolsCommonAttributes(domElement, id);
        const quint32 firstPoint = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPoint = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, NULL_ID_STR);
        const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                   VAbstractTool::TypeLineLine);
        const QString lineColor = GetParametrString(domElement, VAbstractTool::AttrLineColor,
                                                    VAbstractTool::ColorBlack);

        VToolLine::Create(id, firstPoint, secondPoint, typeLine, lineColor, scene, this, data, parse, Source::FromFile);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating line"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SplinesCommonAttributes(const QDomElement &domElement, quint32 &id, quint32 &idObject, quint32 &idTool)
{
    ToolsCommonAttributes(domElement, id);
    idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, NULL_ID_STR);
    idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseCurrentPP()
{
    QDomElement domElement;
    if (GetActivDrawElement(domElement))
    {
        ParseDrawElement(domElement, Document::LiteParse);
    }
    emit CheckLayout();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::CheckTagExists(const QString &tag)
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
                gradation.appendChild(createElement(TagHeights));
                gradation.appendChild(createElement(TagSizes));

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
QString VPattern::GetLabelBase(unsigned int index) const
{
    QStringList list = VApplication::LabelLanguages();
    QStringList alphabet;
    switch (list.indexOf(qApp->getSettings()->GetLabelLanguage()))
    {
        case 0: // de
        {
            QString al = QStringLiteral("A,Ä,B,C,D,E,F,G,H,I,J,K,L,M,N,O,Ö,P,Q,R,S,ß,T,U,Ü,V,W,X,Y,Z");
            alphabet = al.split(",");
            break;
        }
        case 1: // en
        {
            QString al = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
            alphabet = al.split(",");
            break;
        }
        case 2: // fr
        {
            QString al = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
            alphabet = al.split(",");
            break;
        }
        case 3: // ru
        {
            QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,И,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Э,Ю,Я");
            alphabet = al.split(",");
            break;
        }
        case 4: // uk
        {
            QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,І,Ї,Й,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Є,Ю,Я");
            alphabet = al.split(",");
            break;
        }
        case 5: // hr
        {
            QString al = QStringLiteral("A,B,C,Č,Ć,D,Dž,Ð,E,F,G,H,I,J,K,L,Lj,M,N,Nj,O,P,R,S,Š,T,U,V,Z,Ž");
            alphabet = al.split(",");
            break;
        }
        case 6: // sr
        {
            QString al = QStringLiteral("А,Б,В,Г,Д,Ђ,Е,Ж,З,И,Ј,К,Л,Љ,М,Н,Њ,О,П,Р,С,Т,Ћ,У,Ф,Х,Ц,Ч,Џ,Ш");
            alphabet = al.split(",");
            break;
        }
        case 7: // bs
        {
            QString al = QStringLiteral("A,B,C,Č,Ć,D,Dž,Ð,E,F,G,H,I,J,K,L,Lj,M,N,Nj,O,P,R,S,Š,T,U,V,Z,Ž");
            alphabet = al.split(",");
            break;
        }
        default: // en
        {
            QString al = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
            alphabet = al.split(",");
            break;
        }
    }

    QString base;
    int count = qFloor(index/static_cast<unsigned int>(alphabet.size()));
    int number = static_cast<int>(index) - alphabet.size() * count;
    int i = 0;
    do
    {
        base.append(alphabet.at(number));
        ++i;
    } while (i < count);
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseSplineElement parse spline tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of spline.
 */
void VPattern::ParseSplineElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                  const Document &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    quint32 id = 0;
    quint32 idObject = 0;
    quint32 idTool = 0;

    QStringList splines = QStringList() << VToolSpline::ToolType << VToolSplinePath::ToolType << VNodeSpline::ToolType
                                        << VNodeSplinePath::ToolType;
    switch (splines.indexOf(type))
    {
        case 0: //VToolSpline::ToolType
            qCDebug(vXML)<<"VToolSpline.";
            try
            {
                ToolsCommonAttributes(domElement, id);
                const quint32 point1 = GetParametrUInt(domElement, VAbstractTool::AttrPoint1, NULL_ID_STR);
                const quint32 point4 = GetParametrUInt(domElement, VAbstractTool::AttrPoint4, NULL_ID_STR);
                const qreal angle1 = GetParametrDouble(domElement, VAbstractTool::AttrAngle1, "270.0");
                const qreal angle2 = GetParametrDouble(domElement, VAbstractTool::AttrAngle2, "90.0");
                const qreal kAsm1 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm1, "1.0");
                const qreal kAsm2 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm2, "1.0");
                const qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, color, scene, this, data,
                                    parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating simple curve"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 1: //VToolSplinePath::ToolType
            qCDebug(vXML)<<"VToolSplinePath.";
            try
            {
                ToolsCommonAttributes(domElement, id);
                const qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);
                VSplinePath *path = new VSplinePath(kCurve);

                const QDomNodeList nodeList = domElement.childNodes();
                const qint32 num = nodeList.size();
                for (qint32 i = 0; i < num; ++i)
                {
                    const QDomElement element = nodeList.at(i).toElement();
                    if (element.isNull() == false)
                    {
                        if (element.tagName() == VAbstractTool::AttrPathPoint)
                        {
                            const qreal kAsm1 = GetParametrDouble(element, VAbstractTool::AttrKAsm1, "1.0");
                            const qreal angle = GetParametrDouble(element, VAbstractTool::AttrAngle, "0");
                            const qreal kAsm2 = GetParametrDouble(element, VAbstractTool::AttrKAsm2, "1.0");
                            const quint32 pSpline = GetParametrUInt(element, VAbstractTool::AttrPSpline, NULL_ID_STR);
                            const VPointF p = *data->GeometricObject<VPointF>(pSpline);

                            QLineF line(0, 0, 100, 0);
                            line.setAngle(angle+180);

                            VSplinePoint splPoint(p, kAsm1, line.angle(), kAsm2, angle);
                            path->append(splPoint);
                            if (parse == Document::FullParse)
                            {
                                IncrementReferens(pSpline);
                            }
                        }
                    }
                }

                VToolSplinePath::Create(id, path, color, scene, this, data, parse, Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating curve path"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 2: //VNodeSpline::ToolType
            qCDebug(vXML)<<"VNodeSpline.";
            try
            {
                SplinesCommonAttributes(domElement, id, idObject, idTool);
                VSpline *spl = new VSpline(*data->GeometricObject<VSpline>(idObject));
                spl->setIdObject(idObject);
                spl->setMode(Draw::Modeling);
                data->UpdateGObject(id, spl);
                VNodeSpline::Create(this, data, id, idObject, parse, Source::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling simple curve"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 3: //VNodeSplinePath::ToolType
            qCDebug(vXML)<<"VNodeSplinePath.";
            try
            {
                SplinesCommonAttributes(domElement, id, idObject, idTool);
                VSplinePath *path = new VSplinePath(*data->GeometricObject<VSplinePath>(idObject));
                path->setIdObject(idObject);
                path->setMode(Draw::Modeling);
                data->UpdateGObject(id, path);
                VNodeSplinePath::Create(this, data, id, idObject, parse, Source::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling curve path"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        default:
            qCDebug(vXML) << "Illegal spline type in VDomDocument::ParseSplineElement().";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseArcElement parse arc tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of spline.
 */
void VPattern::ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse,
                               const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    quint32 id = 0;
    QStringList arcs = QStringList() << VToolArc::ToolType << VNodeArc::ToolType;

    switch (arcs.indexOf(type))
    {
        case 0: //VToolArc::ToolType
            try
            {
                ToolsCommonAttributes(domElement, id);
                const quint32 center = GetParametrUInt(domElement, VAbstractTool::AttrCenter, NULL_ID_STR);
                const QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "10");
                QString r = radius;//need for saving fixed formula;
                const QString f1 = GetParametrString(domElement, VAbstractTool::AttrAngle1, "180");
                QString f1Fix = f1;//need for saving fixed formula;
                const QString f2 = GetParametrString(domElement, VAbstractTool::AttrAngle2, "270");
                QString f2Fix = f2;//need for saving fixed formula;
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolArc::Create(id, center, r, f1Fix, f2Fix, color, scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (r != radius || f1Fix != f1 || f2Fix != f2)
                {
                    SetAttribute(domElement, VAbstractTool::AttrRadius, r);
                    SetAttribute(domElement, VAbstractTool::AttrAngle1, f1Fix);
                    SetAttribute(domElement, VAbstractTool::AttrAngle2, f2Fix);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating simple arc"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch (qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating simple arc"), domElement);
                excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
                throw excep;
            }
            break;
        case 1: //VNodeArc::ToolType
            try
            {
                ToolsCommonAttributes(domElement, id);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, NULL_ID_STR);
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
                VArc *arc = new VArc(*data->GeometricObject<VArc>(idObject));
                arc->setIdObject(idObject);
                arc->setMode(Draw::Modeling);
                data->UpdateGObject(id, arc);
                VNodeArc::Create(this, data, id, idObject, parse, Source::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling arc"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        default:
            qDebug() << "Illegal arc type in VDomDocument::ParseArcElement().";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseToolsElement parse tools tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of spline.
 */
void VPattern::ParseToolsElement(VMainGraphicsScene *scene, const QDomElement &domElement,
                                 const Document &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    quint32 id = 0;
    QStringList tools{VToolUnionDetails::ToolType};

    switch (tools.indexOf(type))
    {
        case 0: //VToolUnionDetails::ToolType
            try
            {
                ToolsCommonAttributes(domElement, id);
                const quint32 indexD1 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD1, "-1");
                const quint32 indexD2 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD2, "-1");

                const QVector<VDetail> vector = VToolUnionDetails::GetDetailFromFile(this, domElement);

                VToolUnionDetails::Create(id, vector[0], vector[1], 0, 0, indexD1, indexD2, scene, this, data, parse,
                                        Source::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating union details"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        default:
            qDebug() << "Illegal tools type in VDomDocument::ParseToolsElement().";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseIncrementsElement parse increments tag.
 * @param node tag in xml tree.
 */
void VPattern::ParseIncrementsElement(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == TagIncrement)
                {
                    const quint32 id = GetParametrId(domElement);
                    const QString name = GetParametrString(domElement, IncrementName, "");
                    const qreal base = GetParametrDouble(domElement, IncrementBase, "0");
                    const qreal ksize = GetParametrDouble(domElement, IncrementKsize, "0");
                    const qreal kgrowth = GetParametrDouble(domElement, IncrementKgrowth, "0");
                    const QString desc = GetParametrString(domElement, IncrementDescription, "Description");
                    data->UpdateId(id);
                    data->AddVariable(name, new VIncrement(name, id, base, ksize, kgrowth, desc));
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QMap<GHeights, bool> VPattern::GetGradationHeights() const
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
void VPattern::SetGradationHeights(const QMap<GHeights, bool> &options)
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
QMap<GSizes, bool> VPattern::GetGradationSizes() const
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
void VPattern::SetGradationSizes(const QMap<GSizes, bool> &options)
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
QString VPattern::GetAuthor() const
{
    return UniqueTagText(TagAuthor, qApp->getSettings()->GetUser());
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetAuthor(const QString &text)
{
    CheckTagExists(TagAuthor);
    setTagText(TagAuthor, text);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GetDescription() const
{
    return UniqueTagText(TagDescription);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetDescription(const QString &text)
{
    CheckTagExists(TagDescription);
    setTagText(TagDescription, text);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GetNotes() const
{
    return UniqueTagText(TagNotes);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetNotes(const QString &text)
{
    CheckTagExists(TagNotes);
    setTagText(TagNotes, text);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GetVersion() const
{
    return UniqueTagText(TagVersion, VPatternConverter::PatternMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetVersion()
{
    setTagText(TagVersion, VPatternConverter::PatternMaxVerStr);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GenerateLabel(const LabelType &type) const
{
    QDomNodeList drawList = elementsByTagName(TagDraw);

    if (type == LabelType::NewPatternPiece)
    {
        QString name;
        int i = 0;
        for (;;)
        {
            name = GetLabelBase(static_cast<unsigned int>(drawList.size() + i));
            if (data->IsUnique(name))
            {
                return name;
            }
            if (i == INT_MAX)
            {
                break;
            }
            ++i;
        }
        qCDebug(vXML)<<"Point label:"<<name;
        return name;
    }
    else if (type == LabelType::NewLabel)
    {
        if (drawList.isEmpty())
        {
            const QString label = GetLabelBase(0);
            qCDebug(vXML)<<"Point label:"<<label;
            return label;
        }

        int index = 0;
        for (int i = 0; i < drawList.size(); ++i)
        {
            QDomElement node = drawList.at(i).toElement();
            if (node.attribute(AttrName) == nameActivPP)
            {
                index = i;
                break;
            }
        }

        QString labelBase = GetLabelBase(static_cast<unsigned int>(index));

        qint32 num = 1;
        QString name;
        do
        {
            name = QString("%1%2").arg(labelBase).arg(num);
            num++;
            if (num == INT_MAX)
            {
                break;
            }
        } while (data->IsUnique(name) == false);
        qCDebug(vXML)<<"Point label:"<<name;
        return name;
    }
    qCDebug(vXML)<<"Got unknow type"<<static_cast<char>(type);
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PrepareForParse(const Document &parse)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    if (parse == Document::FullParse)
    {
        TestUniqueId();
        sceneDraw->clear();
        sceneDetail->clear();
        data->Clear();
        UpdateMeasurements();
        nameActivPP.clear();
        patternPieces.clear();
        tools.clear();
        cursor = 0;
        history.clear();
    }
    else if (parse == Document::LiteParse)
    {
        data->ClearUniqueNames();
        data->ClearVariables(VarType::ArcLength);
        data->ClearVariables(VarType::LineAngle);
        data->ClearVariables(VarType::LineLength);
        data->ClearVariables(VarType::SplineLength);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::UpdateMeasurements()
{
    try
    {
        const QString path = MPath();
        if (MType() == MeasurementsType::Standard)
        {
            VStandardMeasurements m(data);
            ValidateXML("://schema/standard_measurements.xsd", path);
            m.setXMLContent(path);
            m.Measurements();
        }
        else
        {
            VIndividualMeasurements m(data);
            ValidateXML("://schema/individual_measurements.xsd", path);
            m.setXMLContent(path);
            m.Measurements();
        }
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("File error."), qApp->getMainWindow());
        emit ClearMainWindow();
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ToolsCommonAttributes(const QDomElement &domElement, quint32 &id)
{
    id = GetParametrId(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPattern::GetPPElement(const QString &name)
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
int VPattern::CountPP() const
{
    const QDomElement rootElement = this->documentElement();
    if (rootElement.isNull())
    {
        return 0;
    }

    return rootElement.elementsByTagName( TagDraw ).count();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VPattern::ActiveDrawBoundingRect() const
{
    QRectF rec;

    for (qint32 i = 0; i< history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        if (tool.getNameDraw() == nameActivPP)
        {
            switch ( tool.getTypeTool() )
            {
                case Tool::Arrow:
                    Q_UNREACHABLE();
                    break;
                case Tool::SinglePoint:
                    rec = ToolBoundingRect<VToolSinglePoint>(rec, tool.getId());
                    break;
                case Tool::EndLine:
                    rec = ToolBoundingRect<VToolEndLine>(rec, tool.getId());
                    break;
                case Tool::Line:
                    rec = ToolBoundingRect<VToolLine>(rec, tool.getId());
                    break;
                case Tool::AlongLine:
                    rec = ToolBoundingRect<VToolAlongLine>(rec, tool.getId());
                    break;
                case Tool::ShoulderPoint:
                    rec = ToolBoundingRect<VToolShoulderPoint>(rec, tool.getId());
                    break;
                case Tool::Normal:
                    rec = ToolBoundingRect<VToolNormal>(rec, tool.getId());
                    break;
                case Tool::Bisector:
                    rec = ToolBoundingRect<VToolBisector>(rec, tool.getId());
                    break;
                case Tool::LineIntersect:
                    rec = ToolBoundingRect<VToolLineIntersect>(rec, tool.getId());
                    break;
                case Tool::Spline:
                    rec = ToolBoundingRect<VToolSpline>(rec, tool.getId());
                    break;
                case Tool::Arc:
                    rec = ToolBoundingRect<VToolArc>(rec, tool.getId());
                    break;
                case Tool::SplinePath:
                    rec = ToolBoundingRect<VToolSplinePath>(rec, tool.getId());
                    break;
                case Tool::PointOfContact:
                    rec = ToolBoundingRect<VToolPointOfContact>(rec, tool.getId());
                    break;
                case Tool::Height:
                    rec = ToolBoundingRect<VToolHeight>(rec, tool.getId());
                    break;
                case Tool::Triangle:
                    rec = ToolBoundingRect<VToolTriangle>(rec, tool.getId());
                    break;
                case Tool::PointOfIntersection:
                    rec = ToolBoundingRect<VToolPointOfIntersection>(rec, tool.getId());
                    break;
                case Tool::CutArc:
                    rec = ToolBoundingRect<VToolCutArc>(rec, tool.getId());
                    break;
                case Tool::CutSpline:
                    rec = ToolBoundingRect<VToolCutSpline>(rec, tool.getId());
                    break;
                case Tool::CutSplinePath:
                    rec = ToolBoundingRect<VToolCutSplinePath>(rec, tool.getId());
                    break;
                //Because "history" not only show history of pattern, but help restore current data for each pattern's
                //piece, we need add record about details and nodes, but don't show them.
                case Tool::Detail:
                    break;
                case Tool::UnionDetails:
                    break;
                case Tool::NodeArc:
                    break;
                case Tool::NodePoint:
                    break;
                case Tool::NodeSpline:
                    break;
                case Tool::NodeSplinePath:
                    break;
                default:
                    qDebug()<<"Got wrong tool type. Ignore.";
                    break;
            }
        }
    }
    return rec;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QRectF VPattern::ToolBoundingRect(const QRectF &rec, const quint32 &id) const
{
    QRectF recTool = rec;
    if (tools.contains(id))
    {
        T *vTool = qobject_cast<T *>(tools.value(id));
        SCASSERT(vTool != nullptr);

        QRectF childrenRect = vTool->childrenBoundingRect();
        //map to scene coordinate.
        childrenRect.translate(vTool->scenePos());

        recTool = recTool.united(vTool->boundingRect());
        recTool = recTool.united(childrenRect);
    }
    else
    {
        qDebug()<<"Can't find tool with id="<<id;
    }
    return recTool;
}


//---------------------------------------------------------------------------------------------------------------------
QVector<VToolRecord> VPattern::getLocalHistory() const
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
quint32 VPattern::SiblingNodeId(const quint32 &nodeId) const
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
