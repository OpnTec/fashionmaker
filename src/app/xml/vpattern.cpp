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
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionwrongid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "vstandardmeasurements.h"
#include "vindividualmeasurements.h"
#include "../../libs/qmuparser/qmuparsererror.h"
#include "../geometry/varc.h"
#include <QMessageBox>

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

const QString VPattern::AttrName        = QStringLiteral("name");
const QString VPattern::AttrType        = QStringLiteral("type");
const QString VPattern::AttrPath        = QStringLiteral("path");

const QString VPattern::IncrementName        = QStringLiteral("name");
const QString VPattern::IncrementBase        = QStringLiteral("base");
const QString VPattern::IncrementKsize       = QStringLiteral("ksize");
const QString VPattern::IncrementKgrowth     = QStringLiteral("kgrowth");
const QString VPattern::IncrementDescription = QStringLiteral("description");

//---------------------------------------------------------------------------------------------------------------------
VPattern::VPattern(VContainer *data, Valentina::Draws *mode, VMainGraphicsScene *sceneDraw,
                   VMainGraphicsScene *sceneDetail, QObject *parent)
    : QObject(parent), VDomDocument(data), nameActivDraw(QString()), tools(QHash<quint32, VDataTool*>()),
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
    QDomText newNodeText = createTextNode(VAL_STR_VERSION);
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
void VPattern::ChangeActivPP(const QString &name, const Document::Documents &parse)
{
    Q_ASSERT_X(name.isEmpty() == false, "ChangeActivPP", "name pattern piece is empty");
    if (this->nameActivDraw != name)
    {
        if (CheckNamePP(name))
        {
            this->nameActivDraw = name;
            if (parse == Document::FullParse)
            {
                emit ChangedActivPP(name);
            }
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
    if (nameActivDraw.isEmpty() == false)
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
                if ( fieldName == nameActivDraw )
                {
                    return true;
                }
            }
        }
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
    if (CheckNamePP(name) == false)
    {
        if (nameActivDraw.isEmpty())
        {
            SetActivPP(name);
        }
        else
        {
            ChangeActivPP(name);
        }
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetNameDraw change current pattern peace.
 * @param name pattern peace name.
 * @return true if success.
 */
bool VPattern::SetNameDraw(const QString &name)
{
    Q_ASSERT_X(name.isEmpty() == false, "SetNameDraw", "name draw is empty");
    const QString oldName = nameActivDraw;
    QDomElement element;
    if (GetActivDrawElement(element))
    {
        nameActivDraw = name;
        element.setAttribute(AttrName, nameActivDraw);
        emit patternChanged();
        emit ChangedNameDraw(oldName, nameActivDraw);
        return true;
    }
    else
    {
        qDebug()<<"Can't find activ draw"<<Q_FUNC_INFO;
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Parse parse file.
 * @param parse parser file mode.
 * @param sceneDraw pointer to draw scene.
 * @param sceneDetail pointer to details scene.
 */
void VPattern::Parse(const Document::Documents &parse)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    QStringList tags{TagDraw, TagIncrements, TagAuthor, TagDescription, TagNotes, TagMeasurements, TagVersion};
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
                        if (parse == Document::FullParse)
                        {
                            if (nameActivDraw.isEmpty())
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
                        ParseDrawElement(sceneDraw, sceneDetail, domElement, parse);
                        break;
                    case 1: // TagIncrements
                        ParseIncrementsElement(domElement);
                        break;
                    case 2: // TagAuthor
                        break;
                    case 3: // TagDescription
                        break;
                    case 4: // TagNotes
                        break;
                    case 5: // TagMeasurements
                        break;
                    case 6: // TagVersion
                        break;
                    default:
                        qDebug()<<"Wrong tag name"<<domElement.tagName()<<Q_FUNC_INFO;
                        break;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTool return tool from tool list.
 * @param id tool id.
 * @return tool.
 */
VDataTool *VPattern::getTool(const quint32 &id)
{
    if (tools.contains(id))
    {
        return tools.value(id);
    }
    else
    {
        const QString error = QString(tr("Can't find tool id = %1 in table.")).arg(id);
        throw VException(error);
    }
    return 0;
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
    if (*mode == Valentina::Calculation)
    {
        if (patternPieces.size() > 1)//don't need upadate data if we have only one pattern piece
        {
            quint32 id = 0;
            if (history.size() == 0)
            {
                return;
            }
            for (qint32 i = 0; i < history.size(); ++i)
            {
                const VToolRecord tool = history.at(i);
                if (tool.getNameDraw() == nameActivDraw)
                {
                    id = tool.getId();
                }
            }
            if (id == 0)
            {
                const VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                if (id == 0)
                {
                    return;
                }
            }
            if (tools.size() > 0)
            {
                const VDataTool *vTool = tools.value(id);
                data->setData(vTool->getData());
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
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr);
    tool->decrementReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TestUniqueId test exist unique id in pattern file. Each id must be unique.
 */
void VPattern::TestUniqueId() const
{
    QVector<quint32> vector;
    CollectId(documentElement(), vector);
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
bool VPattern::CheckNamePP(const QString &name) const
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
    this->nameActivDraw = name;
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
        emit patternChanged();
    }
    else
    {
        qDebug()<<"Can't save path to measurements"<<Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Valentina::Units VPattern::MUnit() const
{
    QDomNodeList list = elementsByTagName(VPattern::TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        QStringList units{"mm", "cm", "inch"};
        QString unit = GetParametrString(element, AttrUnit);
        switch (units.indexOf(unit))
        {
            case 0:// mm
                return Valentina::Mm;
                break;
            case 1:// cm
                return Valentina::Cm;
                break;
            case 2:// in
                return Valentina::Inch;
                break;
            default:
                return Valentina::Cm;
                break;
        }
    }
    else
    {
        return Valentina::Cm;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Pattern::Measurements VPattern::MType() const
{
    QDomNodeList list = elementsByTagName(VPattern::TagMeasurements);
    QDomElement element = list.at(0).toElement();
    if (element.isElement())
    {
        QString type = GetParametrString(element, AttrType);
        QStringList types{"standard", "individual"};
        switch (types.indexOf(type))
        {
            case 0:// standard
                return Pattern::Standard;
                break;
            case 1:// individual
                return Pattern::Individual;
                break;
            default:
                return Pattern::Individual;
                break;
        }
    }
    else
    {
        return Pattern::Individual;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPattern::SaveDocument(const QString &fileName)
{
    try
    {
        TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error no unique id."));
        return false;
    }
    return VDomDocument::SaveDocument(fileName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LiteParseTree lite parse file.
 */
void VPattern::LiteParseTree()
{
    try
    {
        Parse(Document::LiteParse);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."));
        emit ClearMainWindow();
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value."));
        emit ClearMainWindow();
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter."));
        emit ClearMainWindow();
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id."));
        emit ClearMainWindow();
        return;
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."));
        emit ClearMainWindow();
        return;
    }
    catch (const std::bad_alloc &)
    {
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error parsing file."));
        msgBox.setInformativeText("std::bad_alloc");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        emit ClearMainWindow();
        return;
    }

    setCurrentData();
    emit FullUpdateFromFile();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveLiteChange we have unsaved change.
 */
void VPattern::haveLiteChange()
{
    emit patternChanged();
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
/**
 * @brief ParseDrawElement parse draw tag.
 * @param sceneDraw draw scene.
 * @param sceneDetail details scene.
 * @param node node.
 * @param parse parser file mode.
 */
void VPattern::ParseDrawElement(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail, const QDomNode &node,
                                const Document::Documents &parse)
{
    QStringList tags{TagCalculation, TagModeling, TagDetails};
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
                        data->ClearCalculationGObjects();
                        ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Valentina::Calculation);
                        break;
                    case 1: // TagModeling
                        ParseDrawMode(sceneDraw, sceneDetail, domElement, parse, Valentina::Modeling);
                        break;
                    case 2: // TagDetails
                        ParseDetails(sceneDetail, domElement, parse);
                        break;
                    default:
                        qDebug()<<"Wrong tag name"<<Q_FUNC_INFO;
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
 * @param sceneDraw draw scene.
 * @param sceneDetail details scene.
 * @param node node.
 * @param parse parser file mode.
 * @param mode draw mode.
 */
void VPattern::ParseDrawMode(VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail, const QDomNode &node,
                             const Document::Documents &parse, const Valentina::Draws &mode)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    VMainGraphicsScene *scene = nullptr;
    if (mode == Valentina::Calculation)
    {
        scene = sceneDraw;
    }
    else
    {
        scene = sceneDetail;
    }
    QStringList tags{TagPoint, TagLine, TagSpline, TagArc, TagTools};
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
                    ParsePointElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 1: // TagLine
                    ParseLineElement(scene, domElement, parse);
                    break;
                case 2: // TagSpline
                    ParseSplineElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 3: // TagArc
                    ParseArcElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 4: // TagTools
                    ParseToolsElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                default:
                    qDebug()<<"Wrong tag name"<<Q_FUNC_INFO;
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDetailElement parse detail tag.
 * @param sceneDetail detail scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetailElement(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                                  const Document::Documents &parse)
{
    SCASSERT(sceneDetail != nullptr);
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

        QStringList types{VToolDetail::NodePoint, VToolDetail::NodeArc, VToolDetail::NodeSpline,
                    VToolDetail::NodeSplinePath};
        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (element.isNull() == false)
            {
                if (element.tagName() == VToolDetail::TagNode)
                {
                    const quint32 id = GetParametrUInt(element, VToolDetail::AttrIdObject, "0");
                    const qreal mx = qApp->toPixel(GetParametrDouble(element, VAbstractTool::AttrMx, "0.0"));
                    const qreal my = qApp->toPixel(GetParametrDouble(element, VAbstractTool::AttrMy, "0.0"));
                    const NodeDetail::NodeDetails nodeType = NodeDetail::Contour;

                    const QString t = GetParametrString(element, AttrType, "NodePoint");
                    Valentina::Tools tool;

                    switch (types.indexOf(t))
                    {
                        case 0: // VToolDetail::NodePoint
                            tool = Valentina::NodePoint;
                            break;
                        case 1: // VToolDetail::NodeArc
                            tool = Valentina::NodeArc;
                            break;
                        case 2: // VToolDetail::NodeSpline
                            tool = Valentina::NodeSpline;
                            break;
                        case 3: // VToolDetail::NodeSplinePath
                            tool = Valentina::NodeSplinePath;
                            break;
                        default:
                            qDebug()<<"Wrong node type."<<Q_FUNC_INFO;
                            break;
                    }
                    detail.append(VNodeDetail(id, tool, nodeType, mx, my));
                }
            }
        }
        VToolDetail::Create(id, detail, sceneDetail, this, data, parse, Valentina::FromFile);
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
 * @param sceneDetail detail scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetails(VMainGraphicsScene *sceneDetail, const QDomElement &domElement,
                            const Document::Documents &parse)
{
    SCASSERT(sceneDetail != nullptr);
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
                    ParseDetailElement(sceneDetail, domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }
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
                                 const Document::Documents &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of point is empty");


    QStringList points{VToolSinglePoint::ToolType, VToolEndLine::ToolType, VToolAlongLine::ToolType,
                VToolShoulderPoint::ToolType, VToolNormal::ToolType, VToolBisector::ToolType,
                VToolLineIntersect::ToolType, VToolPointOfContact::ToolType, VNodePoint::ToolType,
                VToolHeight::ToolType, VToolTriangle::ToolType, VToolPointOfIntersection::ToolType,
                VToolCutSpline::ToolType, VToolCutSplinePath::ToolType, VToolCutArc::ToolType};
    switch (points.indexOf(type))
    {
        case 0: //VToolSinglePoint::ToolType
        {
            VToolSinglePoint *spoint = 0;
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "A");
                const qreal x = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrX, "10.0"));
                const qreal y = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrY, "10.0"));
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));

                data->UpdateGObject(id, new VPointF(x, y, name, mx, my));
                VDrawTool::AddRecord(id, Valentina::SinglePointTool, this);
                if (parse != Document::FullParse)
                {
                    UpdateToolData(id, data);
                }
                if (parse == Document::FullParse)
                {
                    spoint = new VToolSinglePoint(this, data, id, Valentina::FromFile);
                    scene->addItem(spoint);
                    connect(spoint, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    connect(scene, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;

                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, "0");
                const qreal angle = GetParametrDouble(domElement, VAbstractTool::AttrAngle, "0.0");

                VToolEndLine::Create(id, name, typeLine, f, angle, basePointId,
                                        mx, my, scene, this, data, parse, Valentina::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (f != formula)
                {
                    SetAttribute(domElement, VAbstractTool::AttrLength, f);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of end line"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of end line"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 2: //VToolAlongLine::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");

                VToolAlongLine::Create(id, name, typeLine, f, firstPointId, secondPointId, mx, my, scene, this,
                                       data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point along line"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 3: //VToolShoulderPoint::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 p1Line = GetParametrUInt(domElement, VAbstractTool::AttrP1Line, "0");
                const quint32 p2Line = GetParametrUInt(domElement, VAbstractTool::AttrP2Line, "0");
                const quint32 pShoulder = GetParametrUInt(domElement, VAbstractTool::AttrPShoulder, "0");

                VToolShoulderPoint::Create(id, f, p1Line, p2Line, pShoulder, typeLine, name, mx, my, scene, this,
                                       data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of shoulder"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 4: //VToolNormal::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const qreal angle = GetParametrDouble(domElement, VAbstractTool::AttrAngle, "0.0");

                VToolNormal::Create(id, f, firstPointId, secondPointId, typeLine, name, angle, mx, my, scene,
                                    this, data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of normal"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 5: //VToolBisector::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "100.0");
                QString f = formula;//need for saving fixed formula;
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const quint32 thirdPointId = GetParametrUInt(domElement, VAbstractTool::AttrThirdPoint, "0");

                VToolBisector::Create(id, f, firstPointId, secondPointId, thirdPointId,
                                    typeLine, name, mx, my, scene, this, data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of bisector"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 6: //VToolLineIntersect::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const quint32 p1Line1Id = GetParametrUInt(domElement, VAbstractTool::AttrP1Line1, "0");
                const quint32 p2Line1Id = GetParametrUInt(domElement, VAbstractTool::AttrP2Line1, "0");
                const quint32 p1Line2Id = GetParametrUInt(domElement, VAbstractTool::AttrP1Line2, "0");
                const quint32 p2Line2Id = GetParametrUInt(domElement, VAbstractTool::AttrP2Line2, "0");

                VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name,
                                            mx, my, scene, this, data, parse, Valentina::FromFile);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "0");
                QString f = radius;//need for saving fixed formula;
                const quint32 center = GetParametrUInt(domElement, VAbstractTool::AttrCenter, "0");
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");

                VToolPointOfContact::Create(id, f, center, firstPointId, secondPointId, name, mx, my, scene, this,
                                            data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of contact"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 8: //VNodePoint::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, "0");
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, "0");
                const VPointF *point = data->GeometricObject<const VPointF *>(idObject );
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                data->UpdateGObject(id, new VPointF(point->x(), point->y(), point->name(), mx, my, idObject,
                                                    Valentina::Modeling));
                VNodePoint::Create(this, data, id, idObject, parse, Valentina::FromFile, idTool);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                           VAbstractTool::TypeLineLine);
                const quint32 basePointId = GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, "0");
                const quint32 p1LineId = GetParametrUInt(domElement, VAbstractTool::AttrP1Line, "0");
                const quint32 p2LineId = GetParametrUInt(domElement, VAbstractTool::AttrP2Line, "0");

                VToolHeight::Create(id, name, typeLine, basePointId, p1LineId, p2LineId,
                                    mx, my, scene, this, data, parse, Valentina::FromFile);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const quint32 axisP1Id = GetParametrUInt(domElement, VAbstractTool::AttrAxisP1, "0");
                const quint32 axisP2Id = GetParametrUInt(domElement, VAbstractTool::AttrAxisP2, "0");
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");

                VToolTriangle::Create(id, name, axisP1Id, axisP2Id, firstPointId, secondPointId, mx, my, scene, this,
                                      data, parse, Valentina::FromFile);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const quint32 firstPointId = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");

                VToolPointOfIntersection::Create(id, name, firstPointId, secondPointId, mx, my, scene, this, data,
                                                 parse, Valentina::FromFile);
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
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 splineId = GetParametrUInt(domElement, VToolCutSpline::AttrSpline, "0");

                VToolCutSpline::Create(id, name, f, splineId, mx, my, scene, this, data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline point"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 13: //VToolCutSplinePath::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 splinePathId = GetParametrUInt(domElement, VToolCutSplinePath::AttrSplinePath, "0");

                VToolCutSplinePath::Create(id, name, f, splinePathId, mx, my, scene, this, data, parse,
                                           Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut spline path point"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 14: //VToolCutArc::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const QString name = GetParametrString(domElement, VAbstractTool::AttrName, "");
                const qreal mx = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMx, "10.0"));
                const qreal my = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrMy, "15.0"));
                const QString formula = GetParametrString(domElement, VAbstractTool::AttrLength, "0");
                QString f = formula;//need for saving fixed formula;
                const quint32 arcId = GetParametrUInt(domElement, VToolCutArc::AttrArc, "0");

                VToolCutArc::Create(id, name, f, arcId, mx, my, scene, this, data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating cut arc point"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
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
                                const Document::Documents &parse)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    try
    {
        const quint32 id = GetParametrId(domElement);
        const quint32 firstPoint = GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
        const quint32 secondPoint = GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
        const QString typeLine = GetParametrString(domElement, VAbstractTool::AttrTypeLine,
                                                   VAbstractTool::TypeLineLine);

        VToolLine::Create(id, firstPoint, secondPoint, typeLine, scene, this, data, parse, Valentina::FromFile);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating line"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
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
                                  const Document::Documents &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    QStringList splines{VToolSpline::ToolType, VToolSplinePath::ToolType, VNodeSpline::ToolType,
                VNodeSplinePath::ToolType};
    switch (splines.indexOf(type))
    {
        case 0: //VToolSpline::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 point1 = GetParametrUInt(domElement, VAbstractTool::AttrPoint1, "0");
                const quint32 point4 = GetParametrUInt(domElement, VAbstractTool::AttrPoint4, "0");
                const qreal angle1 = GetParametrDouble(domElement, VAbstractTool::AttrAngle1, "270.0");
                const qreal angle2 = GetParametrDouble(domElement, VAbstractTool::AttrAngle2, "90.0");
                const qreal kAsm1 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm1, "1.0");
                const qreal kAsm2 = GetParametrDouble(domElement, VAbstractTool::AttrKAsm2, "1.0");
                const qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");

                VToolSpline::Create(id, point1, point4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, this, data, parse,
                                    Valentina::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating simple curve"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 1: //VToolSplinePath::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const qreal kCurve = GetParametrDouble(domElement, VAbstractTool::AttrKCurve, "1.0");
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
                            const quint32 pSpline = GetParametrUInt(element, VAbstractTool::AttrPSpline, "0");
                            const VPointF p = *data->GeometricObject<const VPointF *>(pSpline);

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

                VToolSplinePath::Create(id, path, scene, this, data, parse, Valentina::FromFile);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating curve path"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 2: //VNodeSpline::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, "0");
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, "0");
                VSpline *spl = new VSpline(*data->GeometricObject<const VSpline *>(idObject));
                spl->setIdObject(idObject);
                spl->setMode(Valentina::Modeling);
                data->UpdateGObject(id, spl);
                VNodeSpline::Create(this, data, id, idObject, parse, Valentina::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling simple curve"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 3: //VNodeSplinePath::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, "0");
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, "0");
                VSplinePath *path = new VSplinePath(*data->GeometricObject<const VSplinePath *>(idObject));
                path->setIdObject(idObject);
                path->setMode(Valentina::Modeling);
                data->UpdateGObject(id, path);
                VNodeSplinePath::Create(this, data, id, idObject, parse, Valentina::FromFile, idTool);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating modeling curve path"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        default:
            qDebug() << "Illegal spline type in VDomDocument::ParseSplineElement().";
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
void VPattern::ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document::Documents &parse,
                               const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    QStringList arcs{VToolArc::ToolType, VNodeArc::ToolType};

    switch (arcs.indexOf(type))
    {
        case 0: //VToolArc::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 center = GetParametrUInt(domElement, VAbstractTool::AttrCenter, "0");
                const QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "10");
                QString r = radius;//need for saving fixed formula;
                const QString f1 = GetParametrString(domElement, VAbstractTool::AttrAngle1, "180");
                QString f1Fix = f1;//need for saving fixed formula;
                const QString f2 = GetParametrString(domElement, VAbstractTool::AttrAngle2, "270");
                QString f2Fix = f2;//need for saving fixed formula;

                VToolArc::Create(id, center, r, f1Fix, f2Fix, scene, this, data, parse, Valentina::FromFile);
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
            catch(qmu::QmuParserError &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating simple arc"), domElement);
                QString message("Message:     " + e.GetMsg() + "\n"+
                                "Expression:  " + e.GetExpr());
                excep.AddMoreInformation(message);
                throw excep;
            }
            break;
        case 1: //VNodeArc::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 idObject = GetParametrUInt(domElement, VAbstractNode::AttrIdObject, "0");
                const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, "0");
                VArc *arc = new VArc(*data->GeometricObject<const VArc *>(idObject));
                arc->setIdObject(idObject);
                arc->setMode(Valentina::Modeling);
                data->UpdateGObject(id, arc);
                VNodeArc::Create(this, data, id, idObject, parse, Valentina::FromFile, idTool);
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
                                 const Document::Documents &parse, const QString &type)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    QStringList tools{VToolUnionDetails::ToolType};

    switch (tools.indexOf(type))
    {
        case 0: //VToolUnionDetails::ToolType
            try
            {
                const quint32 id = GetParametrId(domElement);
                const quint32 indexD1 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD1, "-1");
                const quint32 indexD2 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD2, "-1");

                const QVector<VDetail> vector = VToolUnionDetails::GetDetailFromFile(this, domElement);

                VToolUnionDetails::Create(id, vector[0], vector[1], 0, 0, indexD1, indexD2, scene, this, data, parse,
                                        Valentina::FromFile);
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
                    data->AddIncrement(name, VIncrement(id, base, ksize, kgrowth, desc));
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetParametrId return value id attribute.
 * @param domElement tag in xml tree.
 * @return id value.
 */
quint32 VPattern::GetParametrId(const QDomElement &domElement) const
{
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    quint32 id = 0;

    QString message = tr("Got wrong parameter id. Need only id > 0.");
    try
    {
        id = GetParametrUInt(domElement, VDomDocument::AttrId, "0");
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
/**
 * @brief CollectId recursive function, try find id attribute in file. Throw exclusion if find not unique.
 * @param node tag in xml tree.
 * @param vector list with ids.
 */
void VPattern::CollectId(const QDomElement &node, QVector<quint32> &vector) const
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
void VPattern::PrepareForParse(const Document::Documents &parse)
{
    SCASSERT(sceneDraw != nullptr);
    SCASSERT(sceneDetail != nullptr);
    if (parse == Document::FullParse)
    {
        TestUniqueId();
        data->Clear();
        UpdateMeasurements();
        nameActivDraw.clear();
        sceneDraw->clear();
        sceneDetail->clear();
        patternPieces.clear();
        tools.clear();
        cursor = 0;
    }
    data->ClearLengthLines();
    data->ClearLengthArcs();
    data->ClearLengthSplines();
    data->ClearLineAngles();
    data->ClearDetails();
    history.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::UpdateMeasurements()
{
    const QString path = MPath();
    if (MType() == Pattern::Standard)
    {
        VStandardMeasurements m(data);
        m.setContent(path);
        m.Measurements();
    }
    else
    {
        VIndividualMeasurements m(data);
        m.setContent(path);
        m.Measurements();
    }
}


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
