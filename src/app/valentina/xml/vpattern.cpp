/************************************************************************
 **
 **  @file   vpattern.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 2, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vpattern.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../vtools/tools/vdatatool.h"
#include "../vtools/tools/vtoolseamallowance.h"
#include "../vtools/tools/vtooluniondetails.h"
#include "../vtools/tools/drawTools/drawtools.h"
#include "../vtools/tools/nodeDetails/nodedetails.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionundo.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vmisc/customevents.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/vmath.h"
#include "../vmisc/projectversion.h"
#include "../qmuparser/qmuparsererror.h"
#include "../qmuparser/qmutokenparser.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../core/vapplication.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/vnodedetail.h"

#include <QMessageBox>
#include <QUndoStack>
#include <QtNumeric>
#include <QDebug>
#include <QFileInfo>
#include <QtConcurrentMap>
#include <QFuture>
#include <QtConcurrentRun>

#ifdef Q_CC_MSVC
    #include <functional>
#endif /* Q_CC_MSVC */

const QString VPattern::AttrReadOnly = QStringLiteral("readOnly");

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QString FileComment()
{
    return QString("Pattern created with Valentina v%1 (https://valentinaproject.bitbucket.io/).").arg(APP_VERSION_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void GatherCount(int &count, const int nodes)
{
    count += nodes;
}
}

//---------------------------------------------------------------------------------------------------------------------
VPattern::VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw,
                   VMainGraphicsScene *sceneDetail, QObject *parent)
    : VAbstractPattern(parent), data(data), mode(mode), sceneDraw(sceneDraw), sceneDetail(sceneDetail)
{
    SCASSERT(sceneDraw != nullptr)
    SCASSERT(sceneDetail != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CreateEmptyFile create minimal empty file.
 */
void VPattern::CreateEmptyFile()
{
    this->clear();
    QDomElement patternElement = this->createElement(TagPattern);

    patternElement.appendChild(createComment(FileComment()));

    QDomElement version = createElement(TagVersion);
    QDomText newNodeText = createTextNode(VPatternConverter::PatternMaxVerStr);
    version.appendChild(newNodeText);
    patternElement.appendChild(version);

    QDomElement unit = createElement(TagUnit);
    newNodeText = createTextNode(UnitsToStr(qApp->patternUnit()));
    unit.appendChild(newNodeText);
    patternElement.appendChild(unit);

    patternElement.appendChild(createElement(TagDescription));
    patternElement.appendChild(createElement(TagNotes));

    patternElement.appendChild(createElement(TagMeasurements));
    patternElement.appendChild(createElement(TagIncrements));
    patternElement.appendChild(createElement(TagPreviewCalculations));

    this->appendChild(patternElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::setXMLContent(const QString &fileName)
{
    VDomDocument::setXMLContent(fileName);
    GarbageCollector();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Parse parse file.
 * @param parse parser file mode.
 */
void VPattern::Parse(const Document &parse)
{
    qCDebug(vXML, "Parsing pattern.");
    switch (parse)
    {
        case Document::FullParse:
            qCDebug(vXML, "Full parse.");
            break;
        case Document::LiteParse:
            qCDebug(vXML, "Lite parse.");
            break;
        case Document::LitePPParse:
            qCDebug(vXML, "Lite pattern piece parse.");
            break;
        default:
            break;
    }

    SCASSERT(sceneDraw != nullptr)
    SCASSERT(sceneDetail != nullptr)
    QStringList tags = QStringList() << TagDraw << TagIncrements << TagDescription << TagNotes
                                     << TagMeasurements << TagVersion << TagGradation << TagImage << TagUnit
                                     << TagPatternName << TagPatternNum << TagCompanyName << TagCustomerName
                                     << TagPatternLabel << TagPatternMaterials << TagPreviewCalculations
                                     << TagFinalMeasurements;
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
                        qCDebug(vXML, "Tag draw.");
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
                        qCDebug(vXML, "Tag increments.");
                        ParseIncrementsElement(domElement, parse);
                        break;
                    case 2: // TagDescription
                        qCDebug(vXML, "Tag description.");
                        break;
                    case 3: // TagNotes
                        qCDebug(vXML, "Tag notes.");
                        break;
                    case 4: // TagMeasurements
                        qCDebug(vXML, "Tag measurements.");
                        break;
                    case 5: // TagVersion
                        qCDebug(vXML, "Tag version.");
                        break;
                    case 6: // TagGradation
                        qCDebug(vXML, "Tag gradation.");
                        break;
                    case 7: // TagImage
                        qCDebug(vXML, "Tag image.");
                        break;
                    case 8: // TagUnit
                        qCDebug(vXML, "Tag unit.");
                        break;
                    case 9: // TagPatternName
                        qCDebug(vXML, "Pattern name.");
                        break;
                    case 10: // TagPatternNumber
                        qCDebug(vXML, "Pattern number.");
                        break;
                    case 11: // TagCompanyName
                        qCDebug(vXML, "Company name.");
                        break;
                    case 12: // TagCustomerName
                        qCDebug(vXML, "Customer name.");
                        break;
                    case 13: // TagPatternLabel
                        qCDebug(vXML, "Pattern label.");
                        break;
                    case 14: // TagPatternMaterials
                        qCDebug(vXML, "Pattern materials.");
                        break;
                    case 15: // TagPreviewCalculations
                        qCDebug(vXML, "Tag prewiew calculations.");
                        ParseIncrementsElement(domElement, parse);
                        break;
                    case 16: // TagFinalMeasurements
                        qCDebug(vXML, "Tag final measurements.");
                        break;
                    default:
                        qCDebug(vXML, "Wrong tag name %s", qUtf8Printable(domElement.tagName()));
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
        const int countPP = CountPP();
        if (countPP > 1)//don't need upadate data if we have only one pattern piece
        {
            qCDebug(vXML, "Setting current data");
            qCDebug(vXML, "Current PP name %s", qUtf8Printable(nameActivPP));
            qCDebug(vXML, "PP count %d", countPP);

            const QVector<VToolRecord> localHistory = getLocalHistory();
            if (localHistory.size() == 0)
            {
                qCDebug(vXML, "History is empty!");
                return;
            }

            const quint32 id = localHistory.last().getId();
            qCDebug(vXML, "Resoring data from tool with id %u", id);

            if (tools.size() > 0)
            {
                try
                {
                    ToolExists(id);
                }
                catch (VExceptionBadId &e)
                {
                    Q_UNUSED(e)
                    qCDebug(vXML, "List of tools doesn't containe id= %u", id);
                    return;
                }

                const VDataTool *vTool = tools.value(id);
                *data = vTool->getData();
                //Delete special variables if exist
                data->RemoveVariable(currentLength);
                data->RemoveVariable(currentSeamAllowance);
                qCDebug(vXML, "Data successfully updated.");
            }
            else
            {
                qCDebug(vXML, "List of tools is empty!");
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateToolData update tool in list tools.
 * @param id tool id.
 * @param data container with variables.
 */
void VPattern::UpdateToolData(const quint32 &id, VContainer *data)
{
    Q_ASSERT_X(id != 0, Q_FUNC_INFO, "id == 0"); //-V712 //-V654
    SCASSERT(data != nullptr)
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr)
    tool->VDataTool::setData(data);
}

//---------------------------------------------------------------------------------------------------------------------
VContainer VPattern::GetCompleteData() const
{
    const int countPP = CountPP();
    if (countPP <= 0 || history.isEmpty() || tools.isEmpty())
    {
        return (data != nullptr ? *data : VContainer(nullptr, nullptr));
    }

    const quint32 id = (countPP == 1 ? history.last().getId() : LastToolId());

    if (id == NULL_ID)
    {
        return (data != nullptr ? *data : VContainer(nullptr, nullptr));
    }

    try
    {
        ToolExists(id);
    }
    catch (VExceptionBadId &e)
    {
        Q_UNUSED(e)
        return (data != nullptr ? *data : VContainer(nullptr, nullptr));
    }

    const VDataTool *vTool = tools.value(id);
    VContainer lastData = vTool->getData();
    //Delete special variables if exist
    lastData.RemoveVariable(currentLength);
    lastData.RemoveVariable(currentSeamAllowance);
    return lastData;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SPointActiveDraw return id base point current pattern peace.
 * @return id base point.
 */
// cppcheck-suppress unusedFunction
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
                if (domElement.tagName() == TagPoint && domElement.attribute(AttrType, QString()) == VToolBasePoint::ToolType)
                {
                    return GetParametrId(domElement);
                }
            }
        }
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPattern::GetActivePPPieces() const
{
    QVector<quint32> pieces;
    QDomElement drawElement;
    if (GetActivDrawElement(drawElement))
    {
        const QDomElement details = drawElement.firstChildElement(TagDetails);
        if (not details.isNull())
        {
            QDomElement detail = details.firstChildElement(TagDetail);
            while(not detail.isNull())
            {
                bool united = GetParametrBool(detail, VToolSeamAllowance::AttrUnited, falseStr);
                if (not united)
                {
                    pieces.append(GetParametrId(detail));
                }
                detail = detail.nextSiblingElement(TagDetail);
            }
        }
    }
    return pieces;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPattern::SaveDocument(const QString &fileName, QString &error)
{
    try
    {
        TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error not unique id.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return false;
    }

    // Update comment with Valentina version
    QDomNode commentNode = documentElement().firstChild();
    if (commentNode.isComment())
    {
        QDomComment comment = commentNode.toComment();
        comment.setData(FileComment());
    }

    const bool saved = VAbstractPattern::SaveDocument(fileName, error);
    if (saved && QFileInfo(fileName).suffix() != QLatin1String("autosave"))
    {
        modified = false;
    }

    return saved;
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::LiteParseIncrements()
{
    try
    {
        emit SetEnabledGUI(true);

        VContainer::ClearUniqueIncrementNames();
        data->ClearVariables(VarType::Increment);

        QDomNodeList tags = elementsByTagName(TagIncrements);
        if (not tags.isEmpty())
        {
            const QDomNode domElement = tags.at(0);
            if (not domElement.isNull())
            {
                ParseIncrementsElement(domElement, Document::LiteParse);
            }
        }

        tags = elementsByTagName(TagPreviewCalculations);
        if (not tags.isEmpty())
        {
            const QDomNode domElement = tags.at(0);
            if (not domElement.isNull())
            {
                ParseIncrementsElement(domElement, Document::LiteParse);
            }
        }
    }
    catch (const VExceptionUndo &e)
    {
        Q_UNUSED(e)
        /* If user want undo last operation before undo we need finish broken redo operation. For those we post event
         * myself. Later in method customEvent call undo.*/
        QApplication::postEvent(this, new UndoEvent());
        return;
    }
    catch (const VExceptionObjectError &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")), //-V807
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        return;
    }
    catch (VException &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        return;
    }
    catch (const std::bad_alloc &)
    {
        qCCritical(vXML, "%s", qUtf8Printable(tr("Error parsing file (std::bad_alloc).")));
        emit SetEnabledGUI(false);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VPattern::ElementsToParse() const
{
    QVector<QString> tags{TagCalculation, TagDetails, TagModeling, TagIncrements};

    std::function<int (const QString &tagName)> TagsCount = [this](const QString &tagName)
    {
        return elementsByTagName(tagName).length();
    };

    return QtConcurrent::blockingMappedReduced(tags, TagsCount, GatherCount);
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
                qCWarning(vXML, "Lite parsing doesn't support full parsing");
                break;
            default:
                break;
        }
    }
    catch (const VExceptionUndo &e)
    {
        Q_UNUSED(e)
        /* If user want undo last operation before undo we need finish broken redo operation. For those we post event
         * myself. Later in method customEvent call undo.*/
        QApplication::postEvent(this, new UndoEvent());
        return;
    }
    catch (const VExceptionObjectError &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")), //-V807
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (VException &e)
    {
        qCCritical(vXML, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const std::bad_alloc &)
    {
        qCCritical(vXML, "%s", qUtf8Printable(tr("Error parsing file (std::bad_alloc).")));
        emit SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    // Restore name current pattern piece
    nameActivPP = namePP;
    qCDebug(vXML, "Current pattern piece %s", qUtf8Printable(nameActivPP));
    setCurrentData();
    emit FullUpdateFromFile();
    // Recalculate scene rect
    VMainGraphicsView::NewSceneRect(sceneDraw, qApp->getSceneView());
    VMainGraphicsView::NewSceneRect(sceneDetail, qApp->getSceneView());
    qCDebug(vXML, "Scene size updated.");
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VPattern::customEvent(QEvent *event)
{
    if (event->type() == UNDO_EVENT)
    {
        qApp->getUndoStack()->undo();
    }
    else if (event->type() == LITE_PARSE_EVENT)
    {
        LiteParseTree(Document::LiteParse);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail VPattern::ParseDetailNode(const QDomElement &domElement) const
{
    const quint32 id = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
    const qreal mx = GetParametrDouble(domElement, AttrMx, "0.0");
    const qreal my = GetParametrDouble(domElement, AttrMy, "0.0");
    const bool reverse = GetParametrUInt(domElement, VAbstractPattern::AttrNodeReverse, QChar('0'));
    const NodeDetail nodeType = NodeDetail::Contour;

    const QString t = GetParametrString(domElement, AttrType, "NodePoint");
    Tool tool;

    QStringList types = QStringList() << VAbstractPattern::NodePoint
                                      << VAbstractPattern::NodeArc
                                      << VAbstractPattern::NodeSpline
                                      << VAbstractPattern::NodeSplinePath
                                      << VAbstractPattern::NodeElArc;
    switch (types.indexOf(t))
    {
        case 0: // NodePoint
            tool = Tool::NodePoint;
            break;
        case 1: // NodeArc
            tool = Tool::NodeArc;
            break;
        case 2: // NodeSpline
            tool = Tool::NodeSpline;
            break;
        case 3: // NodeSplinePath
            tool = Tool::NodeSplinePath;
            break;
        case 4: // NodeElArc
            tool = Tool::NodeElArc;
            break;
        default:
            VException e(tr("Wrong tag name '%1'.").arg(t));
            throw e;
    }
    return VNodeDetail(id, tool, nodeType, mx, my, reverse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDrawElement parse draw tag.
 * @param node node.
 * @param parse parser file mode.
 */
void VPattern::ParseDrawElement(const QDomNode &node, const Document &parse)
{
    QStringList tags = QStringList() << TagCalculation << TagModeling << TagDetails << TagGroups;
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
                        qCDebug(vXML, "Tag calculation.");
                        data->ClearCalculationGObjects();
                        ParseDrawMode(domElement, parse, Draw::Calculation);
                        break;
                    case 1: // TagModeling
                        qCDebug(vXML, "Tag modeling.");
                        ParseDrawMode(domElement, parse, Draw::Modeling);
                        break;
                    case 2: // TagDetails
                        qCDebug(vXML, "Tag details.");
                        ParseDetails(domElement, parse);
                        break;
                    case 3: // TagGroups
                        qCDebug(vXML, "Tag groups.");
                        ParseGroups(domElement);
                        break;
                    default:
                        VException e(tr("Wrong tag name '%1'.").arg(domElement.tagName()));
                        throw e;
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
    SCASSERT(sceneDraw != nullptr)
    SCASSERT(sceneDetail != nullptr)
    VMainGraphicsScene *scene = nullptr;
    if (mode == Draw::Calculation)
    {
        scene = sceneDraw;
    }
    else
    {
        scene = sceneDetail;
    }
    const QStringList tags = QStringList() << TagPoint
                                           << TagLine
                                           << TagSpline
                                           << TagArc
                                           << TagTools
                                           << TagOperation
                                           << TagElArc
                                           << TagPath;
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
                    qCDebug(vXML, "Tag point.");
                    ParsePointElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 1: // TagLine
                    qCDebug(vXML, "Tag line.");
                    ParseLineElement(scene, domElement, parse);
                    break;
                case 2: // TagSpline
                    qCDebug(vXML, "Tag spline.");
                    ParseSplineElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 3: // TagArc
                    qCDebug(vXML, "Tag arc.");
                    ParseArcElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 4: // TagTools
                    qCDebug(vXML, "Tag tools.");
                    ParseToolsElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 5: // TagOperation
                    qCDebug(vXML, "Tag operation.");
                    ParseOperationElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 6: // TagElArc
                    qCDebug(vXML, "Tag elliptical arc.");
                    ParseEllipticalArcElement(scene, domElement, parse, domElement.attribute(AttrType, QString()));
                    break;
                case 7: // TagPath
                    qCDebug(vXML, "Tag path.");
                    ParsePathElement(scene, domElement, parse);
                    break;
                default:
                    VException e(tr("Wrong tag name '%1'.").arg(domElement.tagName()));
                    throw e;
            }
        }
    }

    if (parse == Document::FullParse)
    {
        emit MadeProgress();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDetailElement parse detail tag.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetailElement(QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    try
    {
        VToolSeamAllowanceInitData initData;
        initData.id = GetParametrId(domElement);
        initData.detail.SetName(GetParametrString(domElement, AttrName, tr("Detail")));
        initData.detail.SetMx(qApp->toPixel(GetParametrDouble(domElement, AttrMx, "0.0")));
        initData.detail.SetMy(qApp->toPixel(GetParametrDouble(domElement, AttrMy, "0.0")));
        initData.detail.SetSeamAllowance(GetParametrBool(domElement, VToolSeamAllowance::AttrSeamAllowance, falseStr));
        initData.detail.SetHideMainPath(GetParametrBool(domElement, VToolSeamAllowance::AttrHideMainPath,
                                               QString().setNum(qApp->ValentinaSettings()->IsHideMainPath())));
        initData.detail.SetSeamAllowanceBuiltIn(GetParametrBool(domElement,
                                                                VToolSeamAllowance::AttrSeamAllowanceBuiltIn,
                                                                falseStr));
        initData.detail.SetForbidFlipping(GetParametrBool(domElement, AttrForbidFlipping,
                                          QString().setNum(qApp->ValentinaSettings()->GetForbidWorkpieceFlipping())));
        initData.detail.SetForceFlipping(GetParametrBool(domElement, AttrForceFlipping,
                                         QString().setNum(qApp->ValentinaSettings()->GetForceWorkpieceFlipping())));
        initData.detail.SetInLayout(GetParametrBool(domElement, AttrInLayout, trueStr));
        initData.detail.SetUnited(GetParametrBool(domElement, VToolSeamAllowance::AttrUnited, falseStr));

        initData.width = GetParametrString(domElement, AttrWidth, "0.0");
        const QString w = initData.width;//need for saving fixed formula;

        ParseDetailInternals(domElement, initData.detail);

        initData.scene = sceneDetail;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        VToolSeamAllowance::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (w != initData.width)
        {
            SetAttribute(domElement, AttrWidth, initData.width);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating detail"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseDetailInternals(const QDomElement &domElement, VPiece &detail) const
{
    const uint version = GetParametrUInt(domElement, AttrVersion, QChar('1'));

    const QStringList tags = QStringList() << TagNodes
                                           << TagData
                                           << TagPatternInfo
                                           << TagGrainline
                                           << VToolSeamAllowance::TagCSA
                                           << VToolSeamAllowance::TagIPaths
                                           << VToolSeamAllowance::TagPins
                                           << VToolSeamAllowance::TagPlaceLabels;

    QFuture<QVector<VPieceNode>> futurePathV1;
    QFuture<VPiecePath> futurePathV2;
    QFuture<VPieceLabelData> futurePPData;
    QFuture<VPatternLabelData> futurePatternInfo;
    QFuture<VGrainlineData> futureGGeometry;
    QFuture<QVector<CustomSARecord>> futureRecords;
    QFuture<QVector<quint32>> futureIPaths;
    QFuture<QVector<quint32>> futurePins;
    QFuture<QVector<quint32>> futurePlaceLabels;

    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull())
        {
            switch (tags.indexOf(element.tagName()))
            {
                case 0:// TagNodes
                    if (version == 1)
                    {
                        // TODO. Delete if minimal supported version is 0.4.0
                        Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                                          "Time to refactor the code.");
                        const bool closed = GetParametrUInt(domElement, AttrClosed, QChar('1'));
                        const qreal width = GetParametrDouble(domElement, AttrWidth, "0.0");
                        futurePathV1 = QtConcurrent::run(this, &VPattern::ParseDetailNodes, element, width, closed);
                    }
                    else
                    {
                        futurePathV2 = QtConcurrent::run(&VPattern::ParsePieceNodes, element);
                    }
                    break;
                case 1:// TagData
                    futurePPData = QtConcurrent::run(this, &VPattern::ParsePieceDataTag, element,
                                                     detail.GetPatternPieceData());
                    break;
                case 2:// TagPatternInfo
                    futurePatternInfo = QtConcurrent::run(this, &VPattern::ParsePiecePatternInfo, element,
                                                          detail.GetPatternInfo());
                    break;
                case 3:// TagGrainline
                    futureGGeometry = QtConcurrent::run(this, &VPattern::ParsePieceGrainline, element,
                                                        detail.GetGrainlineGeometry());
                    break;
                case 4:// VToolSeamAllowance::TagCSA
                    futureRecords = QtConcurrent::run(&VPattern::ParsePieceCSARecords, element);
                    break;
                case 5:// VToolSeamAllowance::TagIPaths
                    futureIPaths = QtConcurrent::run(&VPattern::ParsePieceInternalPaths, element);
                    break;
                case 6:// VToolSeamAllowance::TagPins
                    futurePins = QtConcurrent::run(&VPattern::ParsePiecePointRecords, element);
                    break;
                case 7:// VToolSeamAllowance::TagPlaceLabels
                    futurePlaceLabels = QtConcurrent::run(&VPattern::ParsePiecePointRecords, element);
                    break;
                default:
                    break;
            }
        }
    }

    if (version == 1 && not futurePathV1.isCanceled())
    {
        detail.GetPath().SetNodes(futurePathV1.result());
    }
    else if (not futurePathV2.isCanceled())
    {
        detail.SetPath(futurePathV2.result());
    }

    if (not futurePPData.isCanceled())
    {
        detail.SetPatternPieceData(futurePPData.result());
    }

    if (not futurePatternInfo.isCanceled())
    {
        detail.SetPatternInfo(futurePatternInfo.result());
    }

    if (not futureGGeometry.isCanceled())
    {
        detail.SetGrainlineGeometry(futureGGeometry.result());
    }

    if (not futureRecords.isCanceled())
    {
        detail.SetCustomSARecords(futureRecords.result());
    }

    if (not futureIPaths.isCanceled())
    {
        detail.SetInternalPaths(futureIPaths.result());
    }

    if (not futurePins.isCanceled())
    {
        detail.SetPins(futurePins.result());
    }

    if (not futurePlaceLabels.isCanceled())
    {
        detail.SetPlaceLabels(futurePlaceLabels.result());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VPattern::ParseDetailNodes(const QDomElement &domElement, qreal width, bool closed) const
{
    QVector<VNodeDetail> oldNodes;
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement element = nodeList.at(i).toElement();
        if (not element.isNull()
                && element.tagName() == VAbstractPattern::TagNode) // Old detail version need this check!
        {
            oldNodes.append(ParseDetailNode(element));
        }
    }

    return VNodeDetail::Convert(data, oldNodes, width, closed);
}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData VPattern::ParsePieceDataTag(const QDomElement &domElement, VPieceLabelData ppData) const
{
    ppData.SetVisible(GetParametrBool(domElement, AttrVisible, trueStr));
    ppData.SetLetter(GetParametrEmptyString(domElement, AttrLetter));
    ppData.SetAnnotation(GetParametrEmptyString(domElement, AttrAnnotation));
    ppData.SetOrientation(GetParametrEmptyString(domElement, AttrOrientation));
    ppData.SetRotationWay(GetParametrEmptyString(domElement, AttrRotationWay));
    ppData.SetTilt(GetParametrEmptyString(domElement, AttrTilt));
    ppData.SetFoldPosition(GetParametrEmptyString(domElement, AttrFoldPosition));
    ppData.SetQuantity(static_cast<int>(GetParametrUInt(domElement, AttrQuantity, QChar('1'))));
    ppData.SetOnFold(GetParametrBool(domElement, AttrOnFold, falseStr));
    ppData.SetPos(QPointF(GetParametrDouble(domElement, AttrMx, QChar('0')),
                          GetParametrDouble(domElement, AttrMy, QChar('0'))));
    ppData.SetLabelWidth(GetParametrString(domElement, AttrWidth, QChar('1')));
    ppData.SetLabelHeight(GetParametrString(domElement, AttrHeight, QChar('1')));
    ppData.SetFontSize(static_cast<int>(GetParametrUInt(domElement, VToolSeamAllowance::AttrFont, QChar('0'))));
    ppData.SetRotation(GetParametrString(domElement, AttrRotation, QChar('0')));
    ppData.SetCenterPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrCenterPin, NULL_ID_STR));
    ppData.SetTopLeftPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrTopLeftPin, NULL_ID_STR));
    ppData.SetBottomRightPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrBottomRightPin, NULL_ID_STR));
    ppData.SetLabelTemplate(GetLabelTemplate(domElement));
    return ppData;
}

//---------------------------------------------------------------------------------------------------------------------
VPatternLabelData VPattern::ParsePiecePatternInfo(const QDomElement &domElement, VPatternLabelData patternInfo) const
{
    patternInfo.SetVisible(GetParametrBool(domElement, AttrVisible, trueStr));
    patternInfo.SetPos(QPointF(GetParametrDouble(domElement, AttrMx, QChar('0')),
                               GetParametrDouble(domElement, AttrMy, QChar('0'))));
    patternInfo.SetLabelWidth(GetParametrString(domElement, AttrWidth, QChar('1')));
    patternInfo.SetLabelHeight(GetParametrString(domElement, AttrHeight, QChar('1')));
    patternInfo.SetFontSize(static_cast<int>(GetParametrUInt(domElement, VToolSeamAllowance::AttrFont, QChar('0'))));
    patternInfo.SetRotation(GetParametrString(domElement, AttrRotation, QChar('0')));
    patternInfo.SetCenterPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrCenterPin, NULL_ID_STR));
    patternInfo.SetTopLeftPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrTopLeftPin, NULL_ID_STR));
    patternInfo.SetBottomRightPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrBottomRightPin, NULL_ID_STR));
    return patternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData VPattern::ParsePieceGrainline(const QDomElement &domElement, VGrainlineData gGeometry) const
{
    gGeometry.SetVisible(GetParametrBool(domElement, AttrVisible, falseStr));
    gGeometry.SetPos(QPointF(GetParametrDouble(domElement, AttrMx, QChar('0')),
                             GetParametrDouble(domElement, AttrMy, QChar('0'))));
    gGeometry.SetLength(GetParametrString(domElement, AttrLength, QChar('1')));
    gGeometry.SetRotation(GetParametrString(domElement, AttrRotation, "90"));
    gGeometry.SetArrowType(static_cast<ArrowType>(GetParametrUInt(domElement, AttrArrows, QChar('0'))));
    gGeometry.SetCenterPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrCenterPin, NULL_ID_STR));
    gGeometry.SetTopPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrTopPin, NULL_ID_STR));
    gGeometry.SetBottomPin(GetParametrUInt(domElement, VToolSeamAllowance::AttrBottomPin, NULL_ID_STR));
    return gGeometry;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseDetails parse details tag.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseDetails(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    QDomNode domNode = domElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == TagDetail)
                {
                    ParseDetailElement(domElement, parse);
                }
            }
        }
        domNode = domNode.nextSibling();
    }

    if (parse == Document::FullParse)
    {
        emit MadeProgress();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsWithLineCommonAttributes(const QDomElement &domElement, VToolLinePointInitData &initData)
{
    PointsCommonAttributes(domElement, initData);
    initData.typeLine = GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    initData.lineColor = GetParametrString(domElement, AttrLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, VToolSinglePointInitData &initData)
{
    PointsCommonAttributes(domElement, initData.id, initData.mx, initData.my);
    initData.name = GetParametrString(domElement, AttrName, "A");
    initData.showLabel = GetParametrBool(domElement, AttrShowLabel, trueStr);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, qreal &mx, qreal &my)
{
    ToolsCommonAttributes(domElement, id);
    mx = qApp->toPixel(GetParametrDouble(domElement, AttrMx, "10.0"));
    my = qApp->toPixel(GetParametrDouble(domElement, AttrMy, "15.0"));
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
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(not type.isEmpty(), Q_FUNC_INFO, "type of point is empty");

    QStringList points = QStringList() << VToolBasePoint::ToolType                  /*0*/
                                       << VToolEndLine::ToolType                    /*1*/
                                       << VToolAlongLine::ToolType                  /*2*/
                                       << VToolShoulderPoint::ToolType              /*3*/
                                       << VToolNormal::ToolType                     /*4*/
                                       << VToolBisector::ToolType                   /*5*/
                                       << VToolLineIntersect::ToolType              /*6*/
                                       << VToolPointOfContact::ToolType             /*7*/
                                       << VNodePoint::ToolType                      /*8*/
                                       << VToolHeight::ToolType                     /*9*/
                                       << VToolTriangle::ToolType                   /*10*/
                                       << VToolPointOfIntersection::ToolType        /*11*/
                                       << VToolCutSpline::ToolType                  /*12*/
                                       << VToolCutSplinePath::ToolType              /*13*/
                                       << VToolCutArc::ToolType                     /*14*/
                                       << VToolLineIntersectAxis::ToolType          /*15*/
                                       << VToolCurveIntersectAxis::ToolType         /*16*/
                                       << VToolPointOfIntersectionArcs::ToolType    /*17*/
                                       << VToolPointOfIntersectionCircles::ToolType /*18*/
                                       << VToolPointFromCircleAndTangent::ToolType  /*19*/
                                       << VToolPointFromArcAndTangent::ToolType     /*20*/
                                       << VToolTrueDarts::ToolType                  /*21*/
                                       << VToolPointOfIntersectionCurves::ToolType  /*22*/
                                       << VToolPin::ToolType                        /*23*/
                                       << VToolPlaceLabel::ToolType;                /*24*/
    switch (points.indexOf(type))
    {
        case 0: //VToolBasePoint::ToolType
            ParseToolBasePoint(scene, domElement, parse);
            break;
        case 1: //VToolEndLine::ToolType
            ParseToolEndLine(scene, domElement, parse);
            break;
        case 2: //VToolAlongLine::ToolType
            ParseToolAlongLine(scene, domElement, parse);
            break;
        case 3: //VToolShoulderPoint::ToolType
            ParseToolShoulderPoint(scene, domElement, parse);
            break;
        case 4: //VToolNormal::ToolType
            ParseToolNormal(scene, domElement, parse);
            break;
        case 5: //VToolBisector::ToolType
            ParseToolBisector(scene, domElement, parse);
            break;
        case 6: //VToolLineIntersect::ToolType
            ParseToolLineIntersect(scene, domElement, parse);
            break;
        case 7: //VToolPointOfContact::ToolType
            ParseToolPointOfContact(scene, domElement, parse);
            break;
        case 8: //VNodePoint::ToolType
            ParseNodePoint(domElement, parse);
            break;
        case 9: //VToolHeight::ToolType
            ParseToolHeight(scene, domElement, parse);
            break;
        case 10: //VToolTriangle::ToolType
            ParseToolTriangle(scene, domElement, parse);
            break;
        case 11: //VToolPointOfIntersection::ToolType
            ParseToolPointOfIntersection(scene, domElement, parse);
            break;
        case 12: //VToolCutSpline::ToolType
            ParseToolCutSpline(scene, domElement, parse);
            break;
        case 13: //VToolCutSplinePath::ToolType
            ParseToolCutSplinePath(scene, domElement, parse);
            break;
        case 14: //VToolCutArc::ToolType
            ParseToolCutArc(scene, domElement, parse);
            break;
        case 15: //VToolLineIntersectAxis::ToolType
            ParseToolLineIntersectAxis(scene, domElement, parse);
            break;
        case 16: //VToolCurveIntersectAxis::ToolType
            ParseToolCurveIntersectAxis(scene, domElement, parse);
            break;
        case 17: //VToolPointOfIntersectionArcs::ToolType
            ParseToolPointOfIntersectionArcs(scene, domElement, parse);
            break;
        case 18: //VToolPointOfIntersectionCircles::ToolType
            ParseToolPointOfIntersectionCircles(scene, domElement, parse);
            break;
        case 19: //VToolPointFromCircleAndTangent::ToolType
            ParseToolPointFromCircleAndTangent(scene, domElement, parse);
            break;
        case 20: //VToolPointFromArcAndTangent::ToolType
            ParseToolPointFromArcAndTangent(scene, domElement, parse);
            break;
        case 21: //VToolTrueDarts::ToolType
            ParseToolTrueDarts(scene, domElement, parse);
            break;
        case 22: //VToolPointOfIntersectionCurves::ToolType
            ParseToolPointOfIntersectionCurves(scene, domElement, parse);
            break;
        case 23: //VToolPin::ToolType
            ParsePinPoint(domElement, parse);
            break;
        case 24: //VToolPlaceLabel::ToolType
            ParsePlaceLabel(domElement, parse);
            break;
        default:
            VException e(tr("Unknown point type '%1'.").arg(type));
            throw e;
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
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    try
    {
        VToolLineInitData initData;
        ToolsCommonAttributes(domElement, initData.id);
        initData.firstPoint = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPoint = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        initData.typeLine = GetParametrString(domElement, AttrTypeLine, TypeLineLine);
        initData.lineColor = GetParametrString(domElement, AttrLineColor, ColorBlack);
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        VToolLine::Create(initData);
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
    idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
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
QString VPattern::GetLabelBase(quint32 index) const
{
    const QStringList list = VApplication::LabelLanguages();
    const QString def = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
    QStringList alphabet;
    switch (list.indexOf(qApp->ValentinaSettings()->GetLabelLanguage()))
    {
        case 0: // de
        {
            const QString al = QStringLiteral("A,Ä,B,C,D,E,F,G,H,I,J,K,L,M,N,O,Ö,P,Q,R,S,ß,T,U,Ü,V,W,X,Y,Z");
            alphabet = al.split(QChar(','));
            break;
        }
        case 2: // fr
        {
            const QString al = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
            alphabet = al.split(QChar(','));
            break;
        }
        case 3: // ru
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,И,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Э,Ю,Я");
            alphabet = al.split(QChar(','));
            break;
        }
        case 4: // uk
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,І,Ї,Й,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Є,Ю,Я");
            alphabet = al.split(QChar(','));
            break;
        }
        case 5: // hr
        case 7: // bs
        {
            const QString al = QStringLiteral("A,B,C,Č,Ć,D,Dž,Ð,E,F,G,H,I,J,K,L,Lj,M,N,Nj,O,P,R,S,Š,T,U,V,Z,Ž");
            alphabet = al.split(QChar(','));
            break;
        }
        case 6: // sr
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Ђ,Е,Ж,З,И,Ј,К,Л,Љ,М,Н,Њ,О,П,Р,С,Т,Ћ,У,Ф,Х,Ц,Ч,Џ,Ш");
            alphabet = al.split(QChar(','));
            break;
        }
        case 1: // en
        default: // en
        {
            alphabet = def.split(QChar(','));
            break;
        }
    }

    QString base;
    const int count = qFloor(index/static_cast<quint32>(alphabet.size()));
    const int number = static_cast<int>(index) - alphabet.size() * count;
    int i = 0;
    do
    {
        base.append(alphabet.at(number));
        ++i;
    } while (i < count);
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolBasePoint(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    VToolBasePoint *spoint = nullptr;
    try
    {
        VToolBasePointInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.x = qApp->toPixel(GetParametrDouble(domElement, AttrX, "10.0"));
        initData.y = qApp->toPixel(GetParametrDouble(domElement, AttrY, "10.0"));

        spoint = VToolBasePoint::Create(initData);
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

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolEndLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolEndLineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);

        initData.formulaLength = GetParametrString(domElement, AttrLength, "100.0");
        const QString f = initData.formulaLength;//need for saving fixed formula;

        initData.basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);

        initData.formulaAngle = GetParametrString(domElement, AttrAngle, "0.0");
        const QString angleFix = initData.formulaAngle;

        VToolEndLine::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formulaLength || angleFix != initData.formulaAngle)
        {
            SetAttribute(domElement, AttrLength, initData.formulaLength);
            SetAttribute(domElement, AttrAngle, initData.formulaAngle);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolAlongLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolAlongLineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, "100.0");
        const QString f = initData.formula;//need for saving fixed formula;
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolAlongLine::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolShoulderPoint(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolShoulderPointInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, "100.0");
        const QString f = initData.formula;//need for saving fixed formula;
        initData.p1Line = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        initData.p2Line = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
        initData.pShoulder = GetParametrUInt(domElement, AttrPShoulder, NULL_ID_STR);

        VToolShoulderPoint::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolNormal(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolNormalInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, "100.0");
        const QString f = initData.formula;//need for saving fixed formula;
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        initData.angle = GetParametrDouble(domElement, AttrAngle, "0.0");

        VToolNormal::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolBisector(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolBisectorInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, "100.0");
        const QString f = initData.formula;//need for saving fixed formula;
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        initData.thirdPointId = GetParametrUInt(domElement, AttrThirdPoint, NULL_ID_STR);

        VToolBisector::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolLineIntersect(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolLineIntersectInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.p1Line1Id = GetParametrUInt(domElement, AttrP1Line1, NULL_ID_STR);
        initData.p2Line1Id = GetParametrUInt(domElement, AttrP2Line1, NULL_ID_STR);
        initData.p1Line2Id = GetParametrUInt(domElement, AttrP1Line2, NULL_ID_STR);
        initData.p2Line2Id = GetParametrUInt(domElement, AttrP2Line2, NULL_ID_STR);

        VToolLineIntersect::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point of line intersection"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointOfContact(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointOfContactInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.radius = GetParametrString(domElement, AttrRadius, QChar('0'));
        const QString f = initData.radius;//need for saving fixed formula;
        initData.center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolPointOfContact::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.radius)
        {
            SetAttribute(domElement, AttrRadius, initData.radius);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseNodePoint(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VAbstractNodeInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;
        initData.scene = sceneDetail;

        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, initData.id, mx, my);
        initData.idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        QSharedPointer<VPointF> point;
        try
        {
            point = initData.data->GeometricObject<VPointF>(initData.idObject);
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }

        QSharedPointer<VPointF> p(new VPointF(*point));
        p->setIdObject(initData.idObject);
        p->setMode(Draw::Modeling);
        p->SetShowLabel(GetParametrBool(domElement, AttrShowLabel, trueStr));

        initData.data->UpdateGObject(initData.id, p);
        VNodePoint::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating modeling point"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePinPoint(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPinInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.pointId = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VToolPin::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating pin point"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePlaceLabel(QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPlaceLabelInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.centerPoint = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);

        initData.width = GetParametrString(domElement, AttrWidth, "1.0");
        const QString w = initData.width;//need for saving fixed formula;

        initData.height = GetParametrString(domElement, AttrHeight, "1.0");
        const QString h = initData.height;//need for saving fixed formula;

        initData.angle = GetParametrString(domElement, AttrAngle, "0.0");
        const QString angle = initData.angle;//need for saving fixed formula;

        initData.type = static_cast<PlaceLabelType>(GetParametrUInt(domElement, AttrPlaceLabelType, QChar('0')));

        VToolPlaceLabel::Create(initData);

        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (w != initData.width || h != initData.height || angle != initData.angle)
        {
            SetAttribute(domElement, AttrWidth, initData.width);
            SetAttribute(domElement, AttrHeight, initData.height);
            SetAttribute(domElement, AttrAngle, initData.angle);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating place lavel"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating place lavel"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolHeight(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolHeightInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);
        initData.basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        initData.p1LineId = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        initData.p2LineId = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);

        VToolHeight::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating height"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolTriangle(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolTriangleInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.axisP1Id = GetParametrUInt(domElement, AttrAxisP1, NULL_ID_STR);
        initData.axisP2Id = GetParametrUInt(domElement, AttrAxisP2, NULL_ID_STR);
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolTriangle::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating triangle"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointOfIntersection(VMainGraphicsScene *scene, const QDomElement &domElement,
                                            const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointOfIntersectionInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolPointOfIntersection::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point of intersection"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCutSpline(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCutSplineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, QChar('0'));
        const QString f = initData.formula;//need for saving fixed formula;
        initData.splineId = GetParametrUInt(domElement, VToolCutSpline::AttrSpline, NULL_ID_STR);

        VToolCutSpline::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCutSplinePath(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCutSplinePathInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, QChar('0'));
        const QString f = initData.formula;//need for saving fixed formula;
        initData.splinePathId = GetParametrUInt(domElement, VToolCutSplinePath::AttrSplinePath, NULL_ID_STR);

        VToolCutSplinePath::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCutArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCutArcInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.formula = GetParametrString(domElement, AttrLength, QChar('0'));
        const QString f = initData.formula;//need for saving fixed formula;
        initData.arcId = GetParametrUInt(domElement, AttrArc, NULL_ID_STR);

        VToolCutArc::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != initData.formula)
        {
            SetAttribute(domElement, AttrLength, initData.formula);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolLineIntersectAxis(VMainGraphicsScene *scene, QDomElement &domElement,
                                          const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolLineIntersectAxisInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);

        initData.basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        initData.firstPointId = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        initData.secondPointId = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);

        initData.formulaAngle = GetParametrString(domElement, AttrAngle, "0.0");
        const QString angleFix = initData.formulaAngle;

        VToolLineIntersectAxis::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (angleFix != initData.formulaAngle)
        {
            SetAttribute(domElement, AttrAngle, initData.formulaAngle);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCurveIntersectAxis(VMainGraphicsScene *scene, QDomElement &domElement,
                                           const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCurveIntersectAxisInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsWithLineCommonAttributes(domElement, initData);

        initData.basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        initData.curveId = GetParametrUInt(domElement, AttrCurve, NULL_ID_STR);
        initData.formulaAngle = GetParametrString(domElement, AttrAngle, "0.0");
        const QString angleFix = initData.formulaAngle;

        VToolCurveIntersectAxis::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (angleFix != initData.formulaAngle)
        {
            SetAttribute(domElement, AttrAngle, initData.formulaAngle);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointOfIntersectionArcs(VMainGraphicsScene *scene, const QDomElement &domElement,
                                                const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointOfIntersectionArcsInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.firstArcId = GetParametrUInt(domElement, AttrFirstArc, NULL_ID_STR);
        initData.secondArcId = GetParametrUInt(domElement, AttrSecondArc, NULL_ID_STR);
        initData.pType = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));

        VToolPointOfIntersectionArcs::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point of intersection arcs"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointOfIntersectionCircles(VMainGraphicsScene *scene, QDomElement &domElement,
                                                   const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointOfIntersectionCirclesInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.firstCircleCenterId = GetParametrUInt(domElement, AttrC1Center, NULL_ID_STR);
        initData.secondCircleCenterId = GetParametrUInt(domElement, AttrC2Center, NULL_ID_STR);
        initData.firstCircleRadius = GetParametrString(domElement, AttrC1Radius);
        const QString c1R = initData.firstCircleRadius;
        initData.secondCircleRadius = GetParametrString(domElement, AttrC2Radius);
        const QString c2R = initData.secondCircleRadius;
        initData.crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));

        VToolPointOfIntersectionCircles::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (c1R != initData.firstCircleRadius || c2R != initData.secondCircleRadius)
        {
            SetAttribute(domElement, AttrC1Center, initData.firstCircleRadius);
            SetAttribute(domElement, AttrC2Center, initData.secondCircleRadius);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point of intersection circles"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointOfIntersectionCurves(VMainGraphicsScene *scene, QDomElement &domElement,
                                                  const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointOfIntersectionCurvesInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.firstCurveId = GetParametrUInt(domElement, AttrCurve1, NULL_ID_STR);
        initData.secondCurveId = GetParametrUInt(domElement, AttrCurve2, NULL_ID_STR);
        initData.vCrossPoint = static_cast<VCrossCurvesPoint>(GetParametrUInt(domElement, AttrVCrossPoint, QChar('1')));
        initData.hCrossPoint = static_cast<HCrossCurvesPoint>(GetParametrUInt(domElement, AttrHCrossPoint, QChar('1')));

        VToolPointOfIntersectionCurves::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point of intersection curves"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointFromCircleAndTangent(VMainGraphicsScene *scene, QDomElement &domElement,
                                                  const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointFromCircleAndTangentInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.circleCenterId = GetParametrUInt(domElement, AttrCCenter, NULL_ID_STR);
        initData.tangentPointId = GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
        initData.circleRadius = GetParametrString(domElement, AttrCRadius);
        const QString cR = initData.circleRadius;
        initData.crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));

        VToolPointFromCircleAndTangent::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (cR != initData.circleRadius)
        {
            SetAttribute(domElement, AttrCCenter, initData.circleRadius);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point from circle and tangent"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolPointFromArcAndTangent(VMainGraphicsScene *scene, const QDomElement &domElement,
                                               const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolPointFromArcAndTangentInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        PointsCommonAttributes(domElement, initData);
        initData.arcId = GetParametrUInt(domElement, AttrArc, NULL_ID_STR);
        initData.tangentPointId = GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
        initData.crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));

        VToolPointFromArcAndTangent::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating point from arc and tangent"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolTrueDarts(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolTrueDartsInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);

        initData.p1id = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        initData.p2id = GetParametrUInt(domElement, AttrPoint2, NULL_ID_STR);

        initData.baseLineP1Id = GetParametrUInt(domElement, AttrBaseLineP1, NULL_ID_STR);
        initData.baseLineP2Id = GetParametrUInt(domElement, AttrBaseLineP2, NULL_ID_STR);
        initData.dartP1Id = GetParametrUInt(domElement, AttrDartP1, NULL_ID_STR);
        initData.dartP2Id = GetParametrUInt(domElement, AttrDartP2, NULL_ID_STR);
        initData.dartP3Id = GetParametrUInt(domElement, AttrDartP3, NULL_ID_STR);

        initData.name1 = GetParametrString(domElement, AttrName1, "A");
        initData.mx1 = qApp->toPixel(GetParametrDouble(domElement, AttrMx1, "10.0"));
        initData.my1 = qApp->toPixel(GetParametrDouble(domElement, AttrMy1, "15.0"));
        initData.showLabel1 = GetParametrBool(domElement, AttrShowLabel1, trueStr);

        initData.name2 = GetParametrString(domElement, AttrName2, "A");
        initData.mx2 = qApp->toPixel(GetParametrDouble(domElement, AttrMx2, "10.0"));
        initData.my2 = qApp->toPixel(GetParametrDouble(domElement, AttrMy2, "15.0"));
        initData.showLabel2 = GetParametrBool(domElement, AttrShowLabel2, trueStr);

        VToolTrueDarts::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating true darts"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// TODO. Delete if minimal supported version is 0.2.7
void VPattern::ParseOldToolSpline(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolSplineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        const quint32 point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);
        const qreal angle1 = GetParametrDouble(domElement, AttrAngle1, "270.0");
        const qreal angle2 = GetParametrDouble(domElement, AttrAngle2, "90.0");
        const qreal kAsm1 = GetParametrDouble(domElement, AttrKAsm1, "1.0");
        const qreal kAsm2 = GetParametrDouble(domElement, AttrKAsm2, "1.0");
        const qreal kCurve = GetParametrDouble(domElement, AttrKCurve, "1.0");
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));

        const auto p1 = data->GeometricObject<VPointF>(point1);
        const auto p4 = data->GeometricObject<VPointF>(point4);

        auto* spline = new VSpline(*p1, *p4, angle1, angle2, kAsm1, kAsm2, kCurve);
        if (duplicate > 0)
        {
            spline->SetDuplicate(duplicate);
        }
        spline->SetColor(color);

        VToolSpline::Create(initData, spline);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating simple curve"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolSpline(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolSplineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        initData.point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);

        initData.a1 = GetParametrString(domElement, AttrAngle1, QChar('0'));
        const QString angle1 = initData.a1;//need for saving fixed formula;

        initData.a2 = GetParametrString(domElement, AttrAngle2, QChar('0'));
        const QString angle2 = initData.a2;//need for saving fixed formula;

        initData.l1 = GetParametrString(domElement, AttrLength1, QChar('0'));
        const QString length1 = initData.l1;//need for saving fixed formula;

        initData.l2 = GetParametrString(domElement, AttrLength2, QChar('0'));
        const QString length2 = initData.l2;//need for saving fixed formula;

        initData.color = GetParametrString(domElement, AttrColor, ColorBlack);
        initData.penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        initData.duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));
        initData.approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        VToolSpline *spl = VToolSpline::Create(initData);

        if (spl != nullptr)
        {
            VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
            SCASSERT(window != nullptr)
            connect(spl, &VToolSpline::ToolTip, window, &VAbstractMainWindow::ShowToolTip);
        }

        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (angle1 != initData.a1 || angle2 != initData.a2 || length1 != initData.l1 || length2 != initData.l2)
        {
            SetAttribute(domElement, AttrAngle1, initData.a1);
            SetAttribute(domElement, AttrAngle2, initData.a2);
            SetAttribute(domElement, AttrLength1, initData.l1);
            SetAttribute(domElement, AttrLength2, initData.l2);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating simple curve"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating simple interactive spline"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCubicBezier(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCubicBezierInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);

        const quint32 point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 point2 = GetParametrUInt(domElement, AttrPoint2, NULL_ID_STR);
        const quint32 point3 = GetParametrUInt(domElement, AttrPoint3, NULL_ID_STR);
        const quint32 point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);

        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const QString penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));
        const qreal approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        auto p1 = data->GeometricObject<VPointF>(point1);
        auto p2 = data->GeometricObject<VPointF>(point2);
        auto p3 = data->GeometricObject<VPointF>(point3);
        auto p4 = data->GeometricObject<VPointF>(point4);

        initData.spline = new VCubicBezier(*p1, *p2, *p3, *p4);
        if (duplicate > 0)
        {
            initData.spline->SetDuplicate(duplicate);
        }
        initData.spline->SetColor(color);
        initData.spline->SetPenStyle(penStyle);
        initData.spline->SetPenStyle(penStyle);
        initData.spline->SetApproximationScale(approximationScale);

        VToolCubicBezier::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating cubic bezier curve"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseOldToolSplinePath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolSplinePathInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        const qreal kCurve = GetParametrDouble(domElement, AttrKCurve, "1.0");
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));
        const qreal approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        QVector<VFSplinePoint> points;

        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (element.isNull() == false)
            {
                if (element.tagName() == AttrPathPoint)
                {
                    const qreal kAsm1 = GetParametrDouble(element, AttrKAsm1, "1.0");
                    const qreal angle = GetParametrDouble(element, AttrAngle, QChar('0'));
                    const qreal kAsm2 = GetParametrDouble(element, AttrKAsm2, "1.0");
                    const quint32 pSpline = GetParametrUInt(element, AttrPSpline, NULL_ID_STR);
                    const VPointF p = *data->GeometricObject<VPointF>(pSpline);

                    QLineF line(0, 0, 100, 0);
                    line.setAngle(angle+180);

                    VFSplinePoint splPoint(p, kAsm1, line.angle(), kAsm2, angle);
                    points.append(splPoint);
                    if (parse == Document::FullParse)
                    {
                        IncrementReferens(p.getIdTool());
                    }
                }
            }
        }

        auto* path = new VSplinePath(points, kCurve);
        if (duplicate > 0)
        {
            path->SetDuplicate(duplicate);
        }
        path->SetColor(color);
        path->SetApproximationScale(approximationScale);

        VToolSplinePath::Create(initData, path);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating curve path"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolSplinePath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolSplinePathInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.color = GetParametrString(domElement, AttrColor, ColorBlack);
        initData.penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        initData.duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));
        initData.approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (not element.isNull() && element.tagName() == AttrPathPoint)
            {
                initData.a1.append(GetParametrString(element, AttrAngle1, QChar('0')));
                initData.a2.append(GetParametrString(element, AttrAngle2, QChar('0')));
                initData.l1.append(GetParametrString(element, AttrLength1, QChar('0')));
                initData.l2.append(GetParametrString(element, AttrLength2, QChar('0')));
                const quint32 pSpline = GetParametrUInt(element, AttrPSpline, NULL_ID_STR);
                initData.points.append(pSpline);

                if (parse == Document::FullParse)
                {
                    IncrementReferens(data->GeometricObject<VPointF>(pSpline)->getIdTool());
                }
            }
        }

        //need for saving fixed formula;
        const QVector<QString> angle1 = initData.a1;
        const QVector<QString> angle2 = initData.a2;
        const QVector<QString> length1 = initData.l1;
        const QVector<QString> length2 = initData.l2;

        VToolSplinePath *spl = VToolSplinePath::Create(initData);

        if (spl != nullptr)
        {
            VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
            SCASSERT(window != nullptr)
            connect(spl, &VToolSplinePath::ToolTip, window, &VAbstractMainWindow::ShowToolTip);
        }

        //Rewrite attribute formula. Need for situation when we have wrong formula.
        int count = 0;
        for (qint32 i = 0; i < num; ++i)
        {
            QDomElement element = nodeList.at(i).toElement();
            if (not element.isNull() && element.tagName() == AttrPathPoint)
            {
                if (angle1.at(count) != initData.a1.at(count) || angle2.at(count) != initData.a2.at(count) ||
                    length1.at(count) != initData.l1.at(count) || length2.at(count) != initData.l2.at(count))
                {
                    SetAttribute(element, AttrAngle1, initData.a1.at(count));
                    SetAttribute(element, AttrAngle2, initData.a2.at(count));
                    SetAttribute(element, AttrLength1, initData.l1.at(count));
                    SetAttribute(element, AttrLength2, initData.l2.at(count));
                    modified = true;
                    haveLiteChange();
                }
                ++count;
            }
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating curve path"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating interactive spline path"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolCubicBezierPath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolCubicBezierPathInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const QString penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, QChar('0'));
        const qreal approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        QVector<VPointF> points;

        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (element.isNull() == false)
            {
                if (element.tagName() == AttrPathPoint)
                {
                    const quint32 pSpline = GetParametrUInt(element, AttrPSpline, NULL_ID_STR);
                    const VPointF p = *data->GeometricObject<VPointF>(pSpline);
                    points.append(p);
                    if (parse == Document::FullParse)
                    {
                        IncrementReferens(p.getIdTool());
                    }
                }
            }
        }

        initData.path = new VCubicBezierPath(points);
        if (duplicate > 0)
        {
            initData.path->SetDuplicate(duplicate);
        }
        initData.path->SetColor(color);
        initData.path->SetPenStyle(penStyle);
        initData.path->SetApproximationScale(approximationScale);

        VToolCubicBezierPath::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating cubic bezier path curve"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseNodeSpline(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VAbstractNodeInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        SplinesCommonAttributes(domElement, initData.id, initData.idObject, initData.idTool);
        try
        {
            const auto obj = initData.data->GetGObject(initData.idObject);
            if (obj->getType() == GOType::Spline)
            {
                VSpline *spl = new VSpline(*data->GeometricObject<VSpline>(initData.idObject));
                spl->setIdObject(initData.idObject);
                spl->setMode(Draw::Modeling);
                initData.data->UpdateGObject(initData.id, spl);
            }
            else
            {
                VCubicBezier *spl = new VCubicBezier(*initData.data->GeometricObject<VCubicBezier>(initData.idObject));
                spl->setIdObject(initData.idObject);
                spl->setMode(Draw::Modeling);
                initData.data->UpdateGObject(initData.id, spl);
            }
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }

        VNodeSpline::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating modeling simple curve"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseNodeSplinePath(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VAbstractNodeInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        SplinesCommonAttributes(domElement, initData.id, initData.idObject, initData.idTool);
        try
        {
            const auto obj = initData.data->GetGObject(initData.idObject);
            if (obj->getType() == GOType::SplinePath)
            {
                VSplinePath *path = new VSplinePath(*initData.data->GeometricObject<VSplinePath>(initData.idObject));
                path->setIdObject(initData.idObject);
                path->setMode(Draw::Modeling);
                initData.data->UpdateGObject(initData.id, path);
            }
            else
            {
                VCubicBezierPath *spl =
                        new VCubicBezierPath(*initData.data->GeometricObject<VCubicBezierPath>(initData.idObject));
                spl->setIdObject(initData.idObject);
                spl->setMode(Draw::Modeling);
                initData.data->UpdateGObject(initData.id, spl);
            }
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        VNodeSplinePath::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating modeling curve path"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolArcInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.radius = GetParametrString(domElement, AttrRadius, "10");
        const QString r = initData.radius;//need for saving fixed formula;
        initData.f1 = GetParametrString(domElement, AttrAngle1, "180");
        const QString f1Fix = initData.f1;//need for saving fixed formula;
        initData.f2 = GetParametrString(domElement, AttrAngle2, "270");
        const QString f2Fix = initData.f2;//need for saving fixed formula;
        initData.color = GetParametrString(domElement, AttrColor, ColorBlack);
        initData.penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        initData.approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        VToolArc::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r != initData.radius || f1Fix != initData.f1 || f2Fix != initData.f2)
        {
            SetAttribute(domElement, AttrRadius, initData.radius);
            SetAttribute(domElement, AttrAngle1, initData.f1);
            SetAttribute(domElement, AttrAngle2, initData.f2);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolEllipticalArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolEllipticalArcInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.radius1 = GetParametrString(domElement, AttrRadius1, "10");
        initData.radius2 = GetParametrString(domElement, AttrRadius2, "10");
        const QString r1 = initData.radius1;//need for saving fixed formula;
        const QString r2 = initData.radius2;//need for saving fixed formula;
        initData.f1 = GetParametrString(domElement, AttrAngle1, "180");
        const QString f1Fix = initData.f1;//need for saving fixed formula;
        initData.f2 = GetParametrString(domElement, AttrAngle2, "270");
        const QString f2Fix = initData.f2;//need for saving fixed formula;
        initData.rotationAngle = GetParametrString(domElement, AttrRotationAngle, QChar('0'));
        const QString frotationFix = initData.rotationAngle;//need for saving fixed formula;
        initData.color = GetParametrString(domElement, AttrColor, ColorBlack);
        initData.penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);
        initData.approximationScale = GetParametrDouble(domElement, AttrAScale, QChar('0'));

        VToolEllipticalArc::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r1 != initData.radius1 || r2 != initData.radius2 || f1Fix != initData.f1 || f2Fix != initData.f2
                || frotationFix != initData.rotationAngle)
        {
            SetAttribute(domElement, AttrRadius1, initData.radius1);
            SetAttribute(domElement, AttrRadius2, initData.radius2);
            SetAttribute(domElement, AttrAngle1, initData.f1);
            SetAttribute(domElement, AttrAngle2, initData.f2);
            SetAttribute(domElement, AttrRotationAngle, initData.rotationAngle);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating simple elliptical arc"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating simple elliptical arc"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseNodeEllipticalArc(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VAbstractNodeInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VEllipticalArc *arc = nullptr;
        try
        {
            arc = new VEllipticalArc(*initData.data->GeometricObject<VEllipticalArc>(initData.idObject));
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        arc->setIdObject(initData.idObject);
        arc->setMode(Draw::Modeling);
        initData.data->UpdateGObject(initData.id, arc);
        VNodeEllipticalArc::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating modeling elliptical arc"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseNodeArc(const QDomElement &domElement, const Document &parse)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");

    try
    {
        VAbstractNodeInitData initData;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VArc *arc = nullptr;
        try
        {
            arc = new VArc(*data->GeometricObject<VArc>(initData.idObject));
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        arc->setIdObject(initData.idObject);
        arc->setMode(Draw::Modeling);
        initData.data->UpdateGObject(initData.id, arc);
        VNodeArc::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating modeling arc"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolArcWithLength(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolArcWithLengthInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.radius = GetParametrString(domElement, AttrRadius, "10");
        const QString r = initData.radius;//need for saving fixed formula;
        initData.f1 = GetParametrString(domElement, AttrAngle1, "180");
        const QString f1Fix = initData.f1;//need for saving fixed formula;
        initData.length = GetParametrString(domElement, AttrLength, "10");
        const QString lengthFix = initData.length;//need for saving fixed length;
        initData.color = GetParametrString(domElement, AttrColor, ColorBlack);
        initData.penStyle = GetParametrString(domElement, AttrPenStyle, TypeLineLine);

        VToolArcWithLength::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r != initData.radius || f1Fix != initData.f1 || lengthFix != initData.length)
        {
            SetAttribute(domElement, AttrRadius, initData.radius);
            SetAttribute(domElement, AttrAngle1, initData.f1);
            SetAttribute(domElement, AttrLength, initData.length);
            modified = true;
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolRotation(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolRotationInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.origin = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.angle = GetParametrString(domElement, AttrAngle, "10");
        const QString a = initData.angle;//need for saving fixed formula;
        initData.suffix = GetParametrString(domElement, AttrSuffix, QString());

        VAbstractOperation::ExtractData(domElement, initData);

        VToolRotation::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (a != initData.angle)
        {
            SetAttribute(domElement, AttrAngle, initData.angle);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of rotation"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of rotation"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolFlippingByLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolFlippingByLineInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.firstLinePointId = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        initData.secondLinePointId = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
        initData.suffix = GetParametrString(domElement, AttrSuffix, QString());

        VAbstractOperation::ExtractData(domElement, initData);

        VToolFlippingByLine::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of flipping by line"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolFlippingByAxis(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolFlippingByAxisInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.originPointId = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        initData.axisType = static_cast<AxisType>(GetParametrUInt(domElement, AttrAxisType, QChar('1')));
        initData.suffix = GetParametrString(domElement, AttrSuffix, QString());

        VAbstractOperation::ExtractData(domElement, initData);

        VToolFlippingByAxis::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of flipping by axis"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseToolMove(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");

    try
    {
        VToolMoveInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.formulaAngle = GetParametrString(domElement, AttrAngle, QChar('0'));
        const QString a = initData.formulaAngle;//need for saving fixed formula;
        initData.formulaRotationAngle = GetParametrString(domElement, AttrRotationAngle, QChar('0'));
        const QString r = initData.formulaRotationAngle;//need for saving fixed formula;
        initData.formulaLength = GetParametrString(domElement, AttrLength, QChar('0'));
        const QString len = initData.formulaLength;//need for saving fixed formula;
        initData.suffix = GetParametrString(domElement, AttrSuffix, QString());
        initData.rotationOrigin = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);

        VAbstractOperation::ExtractData(domElement, initData);

        VToolMove::Create(initData);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (a != initData.formulaAngle || r != initData.formulaRotationAngle || len != initData.formulaLength)
        {
            SetAttribute(domElement, AttrAngle, initData.formulaAngle);
            SetAttribute(domElement, AttrRotationAngle, initData.formulaRotationAngle);
            SetAttribute(domElement, AttrLength, initData.formulaLength);
            modified = true;
            haveLiteChange();
        }
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of moving"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
    catch (qmu::QmuParserError &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating operation of moving"), domElement);
        excep.AddMoreInformation(QString("Message:     " + e.GetMsg() + "\n"+ "Expression:  " + e.GetExpr()));
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPattern::EvalFormula(VContainer *data, const QString &formula, bool *ok) const
{
    if (formula.isEmpty())
    {
        *ok = true;
        return 0;
    }
    else
    {
        try
        {
            QScopedPointer<Calculator> cal(new Calculator());
            const qreal result = cal->EvalFormula(data->DataVariables(), formula);

            (qIsInf(result) || qIsNaN(result)) ? *ok = false : *ok = true;
            return result;
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            *ok = false;
            return 0;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPattern::MakeEmptyIncrement(const QString &name)
{
    QDomElement element = createElement(TagIncrement);
    SetAttribute(element, AttrName, name);
    SetAttribute(element, AttrFormula, QChar('0'));
    SetAttribute(element, AttrDescription, QString());
    return element;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPattern::FindIncrement(const QString &name) const
{
    QDomNodeList list = elementsByTagName(TagIncrement);

    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement domElement = list.at(i).toElement();
        if (domElement.isNull() == false)
        {
            const QString parameter = domElement.attribute(AttrName);
            if (parameter == name)
            {
                return domElement;
            }
        }
    }

    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::GarbageCollector(bool commit)
{
    bool cleared = false;

    QDomNodeList modelingList = elementsByTagName(TagModeling);
    for (int i=0; i < modelingList.size(); ++i)
    {
        QDomElement modElement = modelingList.at(i).toElement();
        if (not modElement.isNull())
        {
            QDomElement modNode = modElement.firstChild().toElement();
            while (not modNode.isNull())
            {
                // First get next sibling because later will not have chance to get it
                QDomElement nextSibling = modNode.nextSibling().toElement();
                if (modNode.hasAttribute(VAbstractTool::AttrInUse))
                {
                    const NodeUsage inUse = GetParametrUsage(modNode, VAbstractTool::AttrInUse);
                    if (inUse == NodeUsage::InUse)
                    { // It is dangerous to leave object with attribute 'inUse'
                      // Each parse should confirm this status.
                        SetParametrUsage(modNode, VAbstractTool::AttrInUse, NodeUsage::NotInUse);
                    }
                    else
                    { // Parent was deleted. We do not need this object anymore
                        if (commit)
                        {
                            modElement.removeChild(modNode);
                            cleared = true;
                        }
                    }
                }
                else
                { // Each parse should confirm his status.
                    SetParametrUsage(modNode, VAbstractTool::AttrInUse, NodeUsage::NotInUse);
                }

                modNode = nextSibling;
            }
        }
    }

    if (cleared)
    {
        RefreshElementIdCache();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::NewEmptyIncrement(const QString &type, const QString &name)
{
    const QDomElement element = MakeEmptyIncrement(name);

    const QDomNodeList list = elementsByTagName(type);
    list.at(0).appendChild(element);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::NewEmptyIncrementAfter(const QString &type, const QString &after, const QString &name)
{
    const QDomElement element = MakeEmptyIncrement(name);
    const QDomElement sibling = FindIncrement(after);

    const QDomNodeList list = elementsByTagName(type);

    if (sibling.isNull())
    {
        list.at(0).appendChild(element);
    }
    else
    {
        list.at(0).insertAfter(element, sibling);
    }
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::RemoveIncrement(const QString &type, const QString &name)
{
    const QDomNodeList list = elementsByTagName(type);
    list.at(0).removeChild(FindIncrement(name));
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveUpIncrement(const QString &type, const QString &name)
{
    const QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        const QDomElement prSibling = node.previousSiblingElement(TagIncrement);
        if (not prSibling.isNull())
        {
            elementsByTagName(type).at(0).insertBefore(node, prSibling);
        }
    }
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveDownIncrement(const QString &type, const QString &name)
{
    const QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        const QDomElement nextSibling = node.nextSiblingElement(TagIncrement);
        if (not nextSibling.isNull())
        {
            elementsByTagName(type).at(0).insertAfter(node, nextSibling);
        }
    }
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementAttribute(const QString &name, const QString &attr, const QString &text)
{
    QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        SetAttribute(node, attr, text);
        emit patternChanged(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::LastDrawName() const
{
    const QDomNodeList elements = this->documentElement().elementsByTagName(TagDraw);
    if (elements.size() == 0)
    {
        return QString();
    }

    const QDomElement &elem = elements.at(elements.size()-1).toElement();
    if (not elem.isNull())
    {
        return GetParametrString(elem, AttrName);
    }

    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPattern::LastToolId() const
{
    const QString name = LastDrawName();
    if (name.isEmpty())
    {
        return NULL_ID;
    }

    const QVector<VToolRecord> localHistory = getLocalHistory(name);

    return (not localHistory.isEmpty() ? localHistory.last().getId() : NULL_ID);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseSplineElement parse spline tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of spline.
 */
void VPattern::ParseSplineElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                  const Document &parse, const QString &type)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    QStringList splines = QStringList() << VToolSpline::OldToolType        /*0*/
                                        << VToolSpline::ToolType           /*1*/
                                        << VToolSplinePath::OldToolType    /*2*/
                                        << VToolSplinePath::ToolType       /*3*/
                                        << VNodeSpline::ToolType           /*4*/
                                        << VNodeSplinePath::ToolType       /*5*/
                                        << VToolCubicBezier::ToolType      /*6*/
                                        << VToolCubicBezierPath::ToolType; /*7*/
    switch (splines.indexOf(type))
    {
        case 0: //VToolSpline::OldToolType
            qCDebug(vXML, "VOldToolSpline.");
            ParseOldToolSpline(scene, domElement, parse);// TODO. Delete if minimal supported version is 0.2.7
            break;
        case 1: //VToolSpline::ToolType
            qCDebug(vXML, "VToolSpline.");
            ParseToolSpline(scene, domElement, parse);
            break;
        case 2: //VToolSplinePath::OldToolType
            qCDebug(vXML, "VOldToolSplinePath.");
            ParseOldToolSplinePath(scene, domElement, parse);// TODO. Delete if minimal supported version is 0.2.7
            break;
        case 3: //VToolSplinePath::ToolType
            qCDebug(vXML, "VToolSplinePath.");
            ParseToolSplinePath(scene, domElement, parse);
            break;
        case 4: //VNodeSpline::ToolType
            qCDebug(vXML, "VNodeSpline.");
            ParseNodeSpline(domElement, parse);
            break;
        case 5: //VNodeSplinePath::ToolType
            qCDebug(vXML, "VNodeSplinePath.");
            ParseNodeSplinePath(domElement, parse);
            break;
        case 6: //VToolCubicBezier::ToolType
            qCDebug(vXML, "VToolCubicBezier.");
            ParseToolCubicBezier(scene, domElement, parse);
            break;
        case 7: //VToolCubicBezierPath::ToolType
            qCDebug(vXML, "VToolCubicBezierPath.");
            ParseToolCubicBezierPath(scene, domElement, parse);
            break;
        default:
            VException e(tr("Unknown spline type '%1'.").arg(type));
            throw e;
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
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(not type.isEmpty(), Q_FUNC_INFO, "type of arc is empty");

    QStringList arcs = QStringList() << VToolArc::ToolType            /*0*/
                                     << VNodeArc::ToolType            /*1*/
                                     << VToolArcWithLength::ToolType; /*2*/

    switch (arcs.indexOf(type))
    {
        case 0: //VToolArc::ToolType
            ParseToolArc(scene, domElement, parse);
            break;
        case 1: //VNodeArc::ToolType
            ParseNodeArc(domElement, parse);
            break;
        case 2: //VToolArcWithLength::ToolType
            ParseToolArcWithLength(scene, domElement, parse);
            break;
        default:
            VException e(tr("Unknown arc type '%1'.").arg(type));
            throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseEllipticalArcElement parse elliptical arc tag.
 * @param scene scene.
 * @param domElement tag in xml tree.
 * @param parse parser file mode.
 * @param type type of spline.
 */
void VPattern::ParseEllipticalArcElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse,
                               const QString &type)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(not type.isEmpty(), Q_FUNC_INFO, "type of elliptical arc is empty");

    const QStringList arcs = QStringList() << VToolEllipticalArc::ToolType  /*0*/
                                           << VNodeEllipticalArc::ToolType; /*1*/

    switch (arcs.indexOf(type))
    {
        case 0: //VToolArc::ToolType
            ParseToolEllipticalArc(scene, domElement, parse);
            break;
        case 1: //VNodeEllipticalArc::ToolType
            ParseNodeEllipticalArc(domElement, parse);
            break;
        default:
            VException e(tr("Unknown elliptical arc type '%1'.").arg(type));
            throw e;
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
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(type.isEmpty() == false, Q_FUNC_INFO, "type of spline is empty");

    const QStringList tools = QStringList() << VToolUnionDetails::ToolType;
    switch (tools.indexOf(type))
    {
        case 0: //VToolUnionDetails::ToolType
            try
            {
                VToolUnionDetailsInitData initData;
                initData.indexD1 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD1, "-1");
                initData.indexD2 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD2, "-1");
                initData.version = GetParametrUInt(domElement, AttrVersion, QChar('1'));
                initData.scene = scene;
                initData.doc = this;
                initData.data = data;
                initData.parse = parse;
                initData.typeCreation = Source::FromFile;

                ToolsCommonAttributes(domElement, initData.id);

                VToolUnionDetails::Create(initData);
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating union details"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        default:
            VException e(tr("Unknown tools type '%1'.").arg(type));
            throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParseOperationElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse,
                                     const QString &type)
{
    SCASSERT(scene != nullptr)
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(not type.isEmpty(), Q_FUNC_INFO, "type of operation is empty");

    const QStringList opers = QStringList() << VToolRotation::ToolType        /*0*/
                                            << VToolFlippingByLine::ToolType  /*1*/
                                            << VToolFlippingByAxis::ToolType  /*2*/
                                            << VToolMove::ToolType;           /*3*/

    switch (opers.indexOf(type))
    {
        case 0: //VToolRotation::ToolType
            ParseToolRotation(scene, domElement, parse);
            break;
        case 1: //VToolFlippingByLine::ToolType
            ParseToolFlippingByLine(scene, domElement, parse);
            break;
        case 2: //VToolFlippingByAxis::ToolType
            ParseToolFlippingByAxis(scene, domElement, parse);
            break;
        case 3: //VToolMove::ToolType
            ParseToolMove(scene, domElement, parse);
            break;
        default:
            VException e(tr("Unknown operation type '%1'.").arg(type));
            throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePathElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse)
{
    SCASSERT(scene != nullptr);
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    try
    {
        VToolPiecePathInitData initData;
        initData.scene = scene;
        initData.doc = this;
        initData.data = data;
        initData.parse = parse;
        initData.typeCreation = Source::FromFile;

        ToolsCommonAttributes(domElement, initData.id);
        initData.idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);

        const QDomElement element = domElement.firstChildElement(VAbstractPattern::TagNodes);
        if (not element.isNull())
        {
            initData.path = ParsePathNodes(element);
        }

        const QString defType = QString().setNum(static_cast<int>(PiecePathType::CustomSeamAllowance));
        initData.path.SetType(static_cast<PiecePathType>(GetParametrUInt(domElement, AttrType, defType)));
        initData.path.SetName(GetParametrString(domElement, AttrName, tr("Unnamed path")));
        initData.path.SetPenType(LineStyleToPenStyle(GetParametrString(domElement, AttrTypeLine, TypeLineLine)));
        initData.path.SetCutPath(GetParametrBool(domElement, AttrCut, falseStr));

        if (initData.path.GetType() == PiecePathType::InternalPath)
        {
            initData.path.SetVisibilityTrigger(GetParametrString(domElement, AttrVisible, QChar('1')));
        }

        VToolPiecePath::Create(initData);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating a piece path"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParseIncrementsElement parse increments tag.
 * @param node tag in xml tree.
 * @param parse parser file mode.
 */
void VPattern::ParseIncrementsElement(const QDomNode &node, const Document &parse)
{
    int index = 0;
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
                    const QString name = GetParametrString(domElement, AttrName, QString());
                    const QString desc = GetParametrEmptyString(domElement, AttrDescription);
                    const QString formula = GetParametrString(domElement, AttrFormula, QChar('0'));
                    bool ok = false;
                    const qreal value = EvalFormula(data, formula, &ok);

                    VIncrement *increment = new VIncrement(data, name, static_cast<quint32>(index), value, formula, ok,
                                                           desc);
                    increment->SetPreviewCalculation(node.toElement().tagName() == TagPreviewCalculations);
                    data->AddVariable(name, increment);
                    ++index;
                }
            }
        }
        domNode = domNode.nextSibling();
    }

    if (parse == Document::FullParse)
    {
        emit MadeProgress();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyIncrement(const QString &name)
{
    NewEmptyIncrement(TagIncrements, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyPreviewCalculation(const QString &name)
{
    NewEmptyIncrement(TagPreviewCalculations, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyIncrementAfter(const QString &after, const QString &name)
{
    NewEmptyIncrementAfter(TagIncrements, after, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyPreviewCalculationAfter(const QString &after, const QString &name)
{
    NewEmptyIncrementAfter(TagPreviewCalculations, after, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::RemoveIncrement(const QString &name)
{
    RemoveIncrement(TagIncrements, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::RemovePreviewCalculation(const QString &name)
{
    RemoveIncrement(TagPreviewCalculations, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveUpIncrement(const QString &name)
{
    MoveUpIncrement(TagIncrements, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveUpPreviewCalculation(const QString &name)
{
    MoveUpIncrement(TagPreviewCalculations, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveDownIncrement(const QString &name)
{
    MoveDownIncrement(TagIncrements, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveDownPreviewCalculation(const QString &name)
{
    MoveDownIncrement(TagPreviewCalculations, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementName(const QString &name, const QString &text)
{
    SetIncrementAttribute(name, AttrName, text);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementFormula(const QString &name, const QString &text)
{
    SetIncrementAttribute(name, AttrFormula, text);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementDescription(const QString &name, const QString &text)
{
    SetIncrementAttribute(name, AttrDescription, text);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ReplaceNameInFormula(QVector<VFormulaField> &expressions, const QString &name, const QString &newName)
{
    const int bias = name.length() - newName.length();

    for(int i = 0; i < expressions.size(); ++i)
    {
        if (expressions.at(i).expression.indexOf(name) != -1)
        {
            QMap<int, QString> tokens;

            // Eval formula
            try
            {
                QScopedPointer<qmu::QmuTokenParser> cal(new qmu::QmuTokenParser(expressions.at(i).expression, false,
                                                                                false));
                tokens = cal->GetTokens();// Tokens (variables, measurements)

            }
            catch (const qmu::QmuParserError &)
            {
                continue;// Because we not sure if used. A formula is broken.
            }

            QList<QString> tValues = tokens.values();
            if (not tValues.contains(name))
            {
                continue;
            }

            QList<int> tKeys = tokens.keys();// Take all tokens positions
            QString newFormula = expressions.at(i).expression;

            for (int i = 0; i < tKeys.size(); ++i)
            {
                if (tValues.at(i) != name)
                {
                    continue;
                }

                newFormula.replace(tKeys.at(i), name.length(), newName);

                if (bias != 0)
                {// Translated token has different length than original. Position next tokens need to be corrected.
                    VTranslateVars::BiasTokens(tKeys.at(i), bias, tokens);
                    tKeys = tokens.keys();
                    tValues = tokens.values();
                }
            }

            expressions[i].expression = newFormula;
            expressions[i].element.setAttribute(expressions.at(i).attribute, newFormula);
            emit patternChanged(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GenerateLabel create label for pattern piece of point.
 * @param type type of the label.
 * @param reservedName reversed point name. Use when need reserve name, but point is not in data base yet.
 * @return unique name for current pattern piece.
 */
QString VPattern::GenerateLabel(const LabelType &type, const QString &reservedName) const
{
    if (type == LabelType::NewPatternPiece)
    {
        const QDomNodeList drawList = elementsByTagName(TagDraw);
        QString name;
        int i = 0;
        for (;;)
        {
            name = GetLabelBase(static_cast<quint32>(drawList.size() + i));
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
        qCDebug(vXML, "Point label: %s", qUtf8Printable(name));
        return name;
    }
    else if (type == LabelType::NewLabel)
    {
        const QString labelBase = GetLabelBase(static_cast<quint32>(GetIndexActivPP()));

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
        } while (data->IsUnique(name) == false || name == reservedName);
        qCDebug(vXML, "Point label: %s", qUtf8Printable(name));
        return name;
    }
    qCDebug(vXML, "Got unknow type %d", static_cast<int>(type));
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GenerateSuffix() const
{
    const QString suffixBase = GetLabelBase(static_cast<quint32>(GetIndexActivPP())).toLower();
    const QStringList uniqueNames = VContainer::AllUniqueNames();
    qint32 num = 1;
    QString suffix;
    for (;;)
    {
        suffix = QString("%1%2").arg(suffixBase).arg(num);

        for (int i=0; i < uniqueNames.size(); ++i)
        {
            if (not data->IsUnique(uniqueNames.at(i) + suffix))
            {
                break;
            }

            if (i == uniqueNames.size()-1)
            {
                qCDebug(vXML, "Suffix is: %s", qUtf8Printable(suffix));
                return suffix;
            }
        }

        if (num == INT_MAX)
        {
            break;
        }
        ++num;
    }
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
bool VPattern::IsDefCustom() const
{
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.size() == 0)
    {
        return false;
    }

    const QDomNode domNode = tags.at(0);
    const QDomElement domElement = domNode.toElement();
    if (domElement.isNull() == false)
    {
        return GetParametrBool(domElement, AttrCustom, falseStr);
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetDefCustom(bool value)
{
    CheckTagExists(TagGradation);
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.isEmpty())
    {
        qDebug()<<"Can't save attribute "<<AttrCustom<<Q_FUNC_INFO;
        return;
    }

    QDomNode domNode = tags.at(0);
    QDomElement domElement = domNode.toElement();
    if (domElement.isNull() == false)
    {
        if (value == false)
        {
            domElement.removeAttribute(AttrDefHeight);
            SetDefCustomHeight(0);
            SetDefCustomSize(0);
        }
        else
        {
            SetAttribute(domElement, AttrCustom, value);
        }
        modified = true;
    }
    else
    {
        qDebug()<<"Can't save attribute "<<AttrCustom<<Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VPattern::GetDefCustomHeight() const
{
    if (IsDefCustom())
    {
        QDomNodeList tags = elementsByTagName(TagGradation);
        if (tags.size() == 0)
        {
            return 0;
        }

        const QDomNode domNode = tags.at(0);
        const QDomElement domElement = domNode.toElement();
        if (domElement.isNull() == false)
        {
            return static_cast<int>(GetParametrUInt(domElement, AttrDefHeight, QChar('0')));
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetDefCustomHeight(int value)
{
    CheckTagExists(TagGradation);
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.isEmpty())
    {
        qDebug()<<"Can't save attribute "<<AttrDefHeight<<Q_FUNC_INFO;
        return;
    }

    QDomNode domNode = tags.at(0);
    QDomElement domElement = domNode.toElement();
    if (domElement.isNull() == false)
    {
        if (value == 0)
        {
            domElement.removeAttribute(AttrDefHeight);
        }
        else
        {
            SetAttribute(domElement, AttrDefHeight, value);
        }
        modified = true;
    }
    else
    {
        qDebug()<<"Can't save attribute "<<AttrDefHeight<<Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VPattern::GetDefCustomSize() const
{
    if (IsDefCustom())
    {
        QDomNodeList tags = elementsByTagName(TagGradation);
        if (tags.size() == 0)
        {
            return 0;
        }

        const QDomNode domNode = tags.at(0);
        const QDomElement domElement = domNode.toElement();
        if (domElement.isNull() == false)
        {
            return static_cast<int>(GetParametrUInt(domElement, AttrDefSize, QChar('0')));
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetDefCustomSize(int value)
{
    CheckTagExists(TagGradation);
    QDomNodeList tags = elementsByTagName(TagGradation);
    if (tags.isEmpty())
    {
        qDebug()<<"Can't save attribute "<<AttrDefSize<<Q_FUNC_INFO;
        return;
    }

    QDomNode domNode = tags.at(0);
    QDomElement domElement = domNode.toElement();
    if (domElement.isNull() == false)
    {
        if (value == 0)
        {
            domElement.removeAttribute(AttrDefSize);
        }
        else
        {
            SetAttribute(domElement, AttrDefSize, value);
        }
        modified = true;
    }
    else
    {
        qDebug()<<"Can't save attribute "<<AttrDefSize<<Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPattern::IsReadOnly() const
{
    const QDomElement pattern = documentElement();

    if (pattern.isNull())
    {
        return false;
    }

    return GetParametrBool(pattern, AttrReadOnly, falseStr);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetReadOnly(bool rOnly)
{
    QDomElement pattern = documentElement();

    if (not pattern.isNull())
    {
        if (rOnly)
        {
            SetAttribute(pattern, AttrReadOnly, rOnly);
        }
        else
        {// For better backward compatibility
            pattern.removeAttribute(AttrReadOnly);
        }
        modified = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PrepareForParse(const Document &parse)
{
    SCASSERT(sceneDraw != nullptr)
    SCASSERT(sceneDetail != nullptr)
    if (parse == Document::FullParse)
    {
        TestUniqueId();
        RefreshElementIdCache();
        sceneDraw->clear();
        sceneDraw->InitOrigins();
        sceneDetail->clear();
        sceneDetail->InitOrigins();
        data->ClearForFullParse();
        nameActivPP.clear();
        patternPieces.clear();

        qDeleteAll(toolsOnRemove);//Remove all invisible on a scene objects.
        toolsOnRemove.clear();

        tools.clear();
        cursor = 0;
        history.clear();
    }
    else if (parse == Document::LiteParse)
    {
        VContainer::ClearUniqueNames();
        Q_STATIC_ASSERT_X(static_cast<int>(VarType::Unknown) == 8, "Check that you used all types");
        data->ClearVariables(QVector<VarType>({VarType::Increment,
                                               VarType::LineAngle,
                                               VarType::LineLength,
                                               VarType::CurveLength,
                                               VarType::CurveCLength,
                                               VarType::ArcRadius,
                                               VarType::CurveAngle}));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ToolsCommonAttributes(const QDomElement &domElement, quint32 &id)
{
    id = GetParametrId(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

QRectF VPattern::ActiveDrawBoundingRect() const
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Not all tools were used.");

    QRectF rec;

    for (qint32 i = 0; i< history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        if (tool.getNameDraw() == nameActivPP)
        {
            switch ( tool.getTypeTool() )
            {
                case Tool::Arrow:
                case Tool::SinglePoint:
                case Tool::DoublePoint:
                case Tool::LinePoint:
                case Tool::AbstractSpline:
                case Tool::Cut:
                case Tool::Midpoint:// Same as Tool::AlongLine, but tool will never has such type
                case Tool::ArcIntersectAxis:// Same as Tool::CurveIntersectAxis, but tool will never has such type
                case Tool::LAST_ONE_DO_NOT_USE:
                    Q_UNREACHABLE();
                    break;
                case Tool::BasePoint:
                case Tool::LineIntersect:
                case Tool::PointOfContact:
                case Tool::Triangle:
                case Tool::PointOfIntersection:
                case Tool::CutArc:
                case Tool::CutSpline:
                case Tool::CutSplinePath:
                case Tool::PointOfIntersectionArcs:
                case Tool::PointOfIntersectionCircles:
                case Tool::PointOfIntersectionCurves:
                case Tool::PointFromCircleAndTangent:
                case Tool::PointFromArcAndTangent:
                    rec = ToolBoundingRect<VToolSinglePoint>(rec, tool.getId());
                    break;
                case Tool::EndLine:
                case Tool::AlongLine:
                case Tool::ShoulderPoint:
                case Tool::Normal:
                case Tool::Bisector:
                case Tool::Height:
                case Tool::LineIntersectAxis:
                case Tool::CurveIntersectAxis:
                    rec = ToolBoundingRect<VToolLinePoint>(rec, tool.getId());
                    break;
                case Tool::Line:
                    rec = ToolBoundingRect<VToolLine>(rec, tool.getId());
                    break;
                case Tool::Spline:
                case Tool::CubicBezier:
                case Tool::Arc:
                case Tool::SplinePath:
                case Tool::CubicBezierPath:
                case Tool::ArcWithLength:
                case Tool::EllipticalArc:
                    rec = ToolBoundingRect<VAbstractSpline>(rec, tool.getId());
                    break;
                case Tool::TrueDarts:
                    rec = ToolBoundingRect<VToolDoublePoint>(rec, tool.getId());
                    break;
                case Tool::Rotation:
                case Tool::FlippingByLine:
                case Tool::FlippingByAxis:
                case Tool::Move:
                    rec = ToolBoundingRect<VAbstractOperation>(rec, tool.getId());
                    break;
                //These tools are not accesseble in Draw mode, but still 'history' contains them.
                case Tool::Piece:
                case Tool::UnionDetails:
                case Tool::NodeArc:
                case Tool::NodeElArc:
                case Tool::NodePoint:
                case Tool::NodeSpline:
                case Tool::NodeSplinePath:
                case Tool::Group:
                case Tool::PiecePath:
                case Tool::Pin:
                case Tool::InsertNode:
                case Tool::PlaceLabel:
                case Tool::DuplicateDetail:
                    break;
            }
        }
    }
    return rec;
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QRectF VPattern::ToolBoundingRect(const QRectF &rec, const quint32 &id) const
{
    QRectF recTool = rec;
    if (tools.contains(id))
    {
        const T *vTool = qobject_cast<T *>(tools.value(id));
        SCASSERT(vTool != nullptr)

        QRectF childrenRect = vTool->childrenBoundingRect();
        //map to scene coordinate.
        childrenRect.translate(vTool->scenePos());

        recTool = recTool.united(vTool->sceneBoundingRect());
        recTool = recTool.united(childrenRect);
    }
    else
    {
        qDebug()<<"Can't find tool with id="<<id;
    }
    return recTool;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IncrementReferens increment reference parent objects.
 * @param id parent object id.
 */
void VPattern::IncrementReferens(quint32 id) const
{
    Q_ASSERT_X(id != 0, Q_FUNC_INFO, "id == 0");
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr)
    tool->incrementReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DecrementReferens decrement reference parent objects.
 * @param id parent object id.
 */
void VPattern::DecrementReferens(quint32 id) const
{
    Q_ASSERT_X(id != 0, Q_FUNC_INFO, "id == 0");
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr)
    tool->decrementReferens();
}
