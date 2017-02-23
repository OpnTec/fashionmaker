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
#include "../vmisc/undoevent.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/vmath.h"
#include "../qmuparser/qmuparsererror.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../core/vapplication.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vpiecelabeldata.h"
#include "../vpatterndb/vpatternlabeldata.h"
#include "../vpatterndb/vgrainlinegeometry.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/vnodedetail.h"

#include <QMessageBox>
#include <QUndoStack>
#include <QtNumeric>
#include <QDebug>
#include <QFileInfo>

const QString VPattern::AttrReadOnly = QStringLiteral("readOnly");

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

    patternElement.appendChild(
                createComment(QStringLiteral("Pattern created with Valentina (http://www.valentina-project.org/).")));

    QDomElement version = createElement(TagVersion);
    QDomText newNodeText = createTextNode(VPatternConverter::PatternMaxVerStr);
    version.appendChild(newNodeText);
    patternElement.appendChild(version);

    QDomElement unit = createElement(TagUnit);
    newNodeText = createTextNode(UnitsToStr(qApp->patternUnit()));
    unit.appendChild(newNodeText);
    patternElement.appendChild(unit);

    patternElement.appendChild(createElement(TagAuthor));
    patternElement.appendChild(createElement(TagDescription));
    patternElement.appendChild(createElement(TagNotes));

    patternElement.appendChild(createElement(TagMeasurements));
    patternElement.appendChild(createElement(TagIncrements));

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
    QStringList tags = QStringList() << TagDraw << TagIncrements << TagAuthor << TagDescription << TagNotes
                                     << TagMeasurements << TagVersion << TagGradation << TagImage << TagUnit
                                     << TagPatternName << TagPatternNum << TagCompanyName << TagCustomerName
                                     << TagSize << TagShowDate << TagShowMeasurements;
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
                        ParseIncrementsElement(domElement);
                        break;
                    case 2: // TagAuthor
                        qCDebug(vXML, "Tag author.");
                        break;
                    case 3: // TagDescription
                        qCDebug(vXML, "Tag description.");
                        break;
                    case 4: // TagNotes
                        qCDebug(vXML, "Tag notes.");
                        break;
                    case 5: // TagMeasurements
                        qCDebug(vXML, "Tag measurements.");
                        break;
                    case 6: // TagVersion
                        qCDebug(vXML, "Tag version.");
                        break;
                    case 7: // TagGradation
                        qCDebug(vXML, "Tag gradation.");
                        break;
                    case 8: // TagImage
                        qCDebug(vXML, "Tag image.");
                        break;
                    case 9: // TagUnit
                        qCDebug(vXML, "Tag unit.");
                        break;
                    case 10: // TagPatternName
                        qCDebug(vXML, "Pattern name.");
                        break;
                    case 11: // TagPatternNumber
                        qCDebug(vXML, "Pattern number.");
                        break;
                    case 12: // TagCompanyName
                        qCDebug(vXML, "Company name.");
                        break;
                    case 13: // TagCustomerName
                        qCDebug(vXML, "Customer name.");
                        break;
                    case 14: // TagSize
                        qCDebug(vXML, "Size");
                        break;
                    case 15:
                        qCDebug(vXML, "Show creation date");
                        break;
                    case 16:
                        qCDebug(vXML, "Show measurements");
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
        if (CountPP() > 1)//don't need upadate data if we have only one pattern piece
        {
            qCDebug(vXML, "Setting current data");
            qCDebug(vXML, "Current PP name %s", qUtf8Printable(nameActivPP));
            qCDebug(vXML, "PP count %d", CountPP());

            quint32 id = 0;
            if (history.size() == 0)
            {
                qCDebug(vXML, "History is empty!");
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
            qCDebug(vXML, "Resoring data from tool with id %u", id);
            if (id == NULL_ID)
            {
                qCDebug(vXML, "Could not find record for this current pattern piece %s",
                        qUtf8Printable(nameActivPP));

                const VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                qCDebug(vXML, "Taking record with id %u from PP %s", id, qUtf8Printable(tool.getNameDraw()));
                if (id == NULL_ID)
                {
                    qCDebug(vXML, "Bad id for last record in history.");
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
                if (domElement.tagName() == TagPoint && domElement.attribute(AttrType, "") == VToolBasePoint::ToolType)
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
bool VPattern::SaveDocument(const QString &fileName, QString &error) const
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

    const bool saved = VAbstractPattern::SaveDocument(fileName, error);
    if (saved && QFileInfo(fileName).suffix() != QLatin1String("autosave"))
    {
        modified = false;
    }

    return saved;
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
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail VPattern::ParseDetailNode(const QDomElement &domElement) const
{
    const quint32 id = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
    const qreal mx = GetParametrDouble(domElement, AttrMx, "0.0");
    const qreal my = GetParametrDouble(domElement, AttrMy, "0.0");
    const bool reverse = GetParametrUInt(domElement, VAbstractPattern::AttrNodeReverse, "0");
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
                    ParsePointElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 1: // TagLine
                    qCDebug(vXML, "Tag line.");
                    ParseLineElement(scene, domElement, parse);
                    break;
                case 2: // TagSpline
                    qCDebug(vXML, "Tag spline.");
                    ParseSplineElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 3: // TagArc
                    qCDebug(vXML, "Tag arc.");
                    ParseArcElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 4: // TagTools
                    qCDebug(vXML, "Tag tools.");
                    ParseToolsElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 5: // TagOperation
                    qCDebug(vXML, "Tag operation.");
                    ParseOperationElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
                    break;
                case 6: // TagElArc
                    qCDebug(vXML, "Tag elliptical arc.");
                    ParseEllipticalArcElement(scene, domElement, parse, domElement.attribute(AttrType, ""));
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
        VPiece detail;
        const quint32 id = GetParametrId(domElement);
        detail.SetName(GetParametrString(domElement, AttrName, tr("Detail")));
        detail.SetMx(qApp->toPixel(GetParametrDouble(domElement, AttrMx, "0.0")));
        detail.SetMy(qApp->toPixel(GetParametrDouble(domElement, AttrMy, "0.0")));
        detail.SetSeamAllowance(GetParametrBool(domElement, VToolSeamAllowance::AttrSeamAllowance, falseStr));
        detail.SetForbidFlipping(GetParametrBool(domElement, VToolSeamAllowance::AttrForbidFlipping,
                                           QString().setNum(qApp->ValentinaSettings()->GetForbidWorkpieceFlipping())));
        detail.SetInLayout(GetParametrBool(domElement, AttrInLayout, trueStr));
        detail.SetUnited(GetParametrBool(domElement, VToolSeamAllowance::AttrUnited, falseStr));

        const QString width = GetParametrString(domElement, AttrWidth, "0.0");
        QString w = width;//need for saving fixed formula;
        const uint version = GetParametrUInt(domElement, VToolSeamAllowance::AttrVersion, "1");

        const QStringList tags = QStringList() << TagNodes
                                               << TagData
                                               << TagPatternInfo
                                               << TagGrainline
                                               << VToolSeamAllowance::TagCSA
                                               << VToolSeamAllowance::TagIPaths
                                               << VToolSeamAllowance::TagPins;

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
                            const bool closed = GetParametrUInt(domElement, AttrClosed, "1");
                            const qreal width = GetParametrDouble(domElement, AttrWidth, "0.0");
                            ParseDetailNodes(element, detail, width, closed);
                        }
                        else
                        {
                            detail.SetPath(ParsePieceNodes(element));
                        }
                        break;
                    case 1:// TagData
                        ParsePieceDataTag(element, detail);
                        break;
                    case 2:// TagPatternInfo
                        ParsePiecePatternInfo(element, detail);
                        break;
                    case 3:// TagGrainline
                        ParsePieceGrainline(element, detail);
                        break;
                    case 4:// VToolSeamAllowance::TagCSA
                        detail.SetCustomSARecords(ParsePieceCSARecords(element));
                        break;
                    case 5:// VToolSeamAllowance::TagIPaths
                        detail.SetInternalPaths(ParsePieceInternalPaths(element));
                        break;
                    case 6:// VToolSeamAllowance::TagPins
                        detail.SetPins(ParsePiecePins(element));
                        break;
                    default:
                        break;
                }
            }
        }
        VToolSeamAllowance::Create(id, detail, w, sceneDetail, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (w != width)
        {
            SetAttribute(domElement, AttrWidth, w);
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
void VPattern::ParseDetailNodes(const QDomElement &domElement, VPiece &detail, qreal width, bool closed) const
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

    detail.GetPath().SetNodes(VNodeDetail::Convert(data, oldNodes, width, closed));
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePieceDataTag(const QDomElement &domElement, VPiece &detail) const
{
    detail.GetPatternPieceData().SetVisible(GetParametrBool(domElement, AttrVisible, trueStr));
    try
    {
        QString qsLetter = GetParametrString(domElement, AttrLetter, "");
        detail.GetPatternPieceData().SetLetter(qsLetter);
    }
    catch(const VExceptionEmptyParameter &e)
    {
        Q_UNUSED(e)
        detail.GetPatternPieceData().SetLetter("");
    }
    QPointF ptPos;
    ptPos.setX(GetParametrDouble(domElement, AttrMx, "0"));
    ptPos.setY(GetParametrDouble(domElement, AttrMy, "0"));
    detail.GetPatternPieceData().SetPos(ptPos);
    qreal dLW = GetParametrDouble(domElement, AttrWidth, "0");
    detail.GetPatternPieceData().SetLabelWidth(dLW);
    qreal dLH = GetParametrDouble(domElement, VToolSeamAllowance::AttrHeight, "0");
    detail.GetPatternPieceData().SetLabelHeight(dLH);
    int iFS = static_cast<int>(GetParametrUInt(domElement, VToolSeamAllowance::AttrFont, "0"));
    detail.GetPatternPieceData().SetFontSize(iFS);
    qreal dRot = GetParametrDouble(domElement, AttrRotation, "0");
    detail.GetPatternPieceData().SetRotation(dRot);

    QDomNodeList nodeListMCP = domElement.childNodes();
    for (int iMCP = 0; iMCP < nodeListMCP.count(); ++iMCP)
    {
        MaterialCutPlacement mcp;
        QDomElement domMCP = nodeListMCP.at(iMCP).toElement();
        mcp.m_eMaterial = MaterialType(GetParametrUInt(domMCP, AttrMaterial, 0));
        if (mcp.m_eMaterial == MaterialType::mtUserDefined)
        {
            mcp.m_qsMaterialUserDef = GetParametrString(domMCP, AttrUserDefined, "");
        }
        mcp.m_iCutNumber = static_cast<int>(GetParametrUInt(domMCP, AttrCutNumber, 0));
        mcp.m_ePlacement = PlacementType(GetParametrUInt(domMCP, AttrPlacement, 0));
        detail.GetPatternPieceData().Append(mcp);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePiecePatternInfo(const QDomElement &domElement, VPiece &detail) const
{
    detail.GetPatternInfo().SetVisible(GetParametrBool(domElement, AttrVisible, trueStr));
    QPointF ptPos;
    ptPos.setX(GetParametrDouble(domElement, AttrMx, "0"));
    ptPos.setY(GetParametrDouble(domElement, AttrMy, "0"));
    detail.GetPatternInfo().SetPos(ptPos);
    qreal dLW = GetParametrDouble(domElement, AttrWidth, "0");
    detail.GetPatternInfo().SetLabelWidth(dLW);
    qreal dLH = GetParametrDouble(domElement, VToolSeamAllowance::AttrHeight, "0");
    detail.GetPatternInfo().SetLabelHeight(dLH);
    int iFS = static_cast<int>(GetParametrUInt(domElement, VToolSeamAllowance::AttrFont, "0"));
    detail.GetPatternInfo().SetFontSize(iFS);
    qreal dRot = GetParametrDouble(domElement, AttrRotation, "0");
    detail.GetPatternInfo().SetRotation(dRot);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::ParsePieceGrainline(const QDomElement &domElement, VPiece &detail) const
{
    detail.GetGrainlineGeometry().SetVisible(GetParametrBool(domElement, AttrVisible, falseStr));
    QPointF ptPos;
    ptPos.setX(GetParametrDouble(domElement, AttrMx, "0"));
    ptPos.setY(GetParametrDouble(domElement, AttrMy, "0"));
    detail.GetGrainlineGeometry().SetPos(ptPos);
    QString qsLength = GetParametrString(domElement, AttrLength, "0");
    detail.GetGrainlineGeometry().SetLength(qsLength);
    QString qsRot = GetParametrString(domElement, AttrRotation, "90");
    detail.GetGrainlineGeometry().SetRotation(qsRot);
    VGrainlineGeometry::ArrowType eAT =
            VGrainlineGeometry::ArrowType(GetParametrUInt(domElement, AttrArrows, "0"));
    detail.GetGrainlineGeometry().SetArrowType(eAT);
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
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx, qreal &my,
                                      QString &typeLine, QString &lineColor)
{
    PointsCommonAttributes(domElement, id, name, mx, my);
    typeLine = GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = GetParametrString(domElement, AttrLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx, qreal &my)
{
    PointsCommonAttributes(domElement, id, mx, my);
    name = GetParametrString(domElement, AttrName, "A");
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
                                       << VToolPin::ToolType;                       /*23*/
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
        quint32 id = 0;
        ToolsCommonAttributes(domElement, id);
        const quint32 firstPoint = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPoint = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        const QString typeLine = GetParametrString(domElement, AttrTypeLine,
                                                   TypeLineLine);
        const QString lineColor = GetParametrString(domElement, AttrLineColor,
                                                    ColorBlack);

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
            alphabet = al.split(",");
            break;
        }
        case 2: // fr
        {
            const QString al = QStringLiteral("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z");
            alphabet = al.split(",");
            break;
        }
        case 3: // ru
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,И,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Э,Ю,Я");
            alphabet = al.split(",");
            break;
        }
        case 4: // uk
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Е,Ж,З,І,Ї,Й,К,Л,М,Н,О,П,Р,С,Т,У,Ф,Х,Ц,Ч,Ш,Щ,Є,Ю,Я");
            alphabet = al.split(",");
            break;
        }
        case 5: // hr
        {
            const QString al = QStringLiteral("A,B,C,Č,Ć,D,Dž,Ð,E,F,G,H,I,J,K,L,Lj,M,N,Nj,O,P,R,S,Š,T,U,V,Z,Ž");
            alphabet = al.split(",");
            break;
        }
        case 6: // sr
        {
            const QString al = QStringLiteral("А,Б,В,Г,Д,Ђ,Е,Ж,З,И,Ј,К,Л,Љ,М,Н,Њ,О,П,Р,С,Т,Ћ,У,Ф,Х,Ц,Ч,Џ,Ш");
            alphabet = al.split(",");
            break;
        }
        case 7: // bs
        {
            const QString al = QStringLiteral("A,B,C,Č,Ć,D,Dž,Ð,E,F,G,H,I,J,K,L,Lj,M,N,Nj,O,P,R,S,Š,T,U,V,Z,Ž");
            alphabet = al.split(",");
            break;
        }
        case 1: // en
        default: // en
        {
            alphabet = def.split(",");
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const qreal x = qApp->toPixel(GetParametrDouble(domElement, AttrX, "10.0"));
        const qreal y = qApp->toPixel(GetParametrDouble(domElement, AttrY, "10.0"));

        VPointF *point = new VPointF(x, y, name, mx, my);
        spoint = VToolBasePoint::Create(id, nameActivPP, point, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

        const QString formula = GetParametrString(domElement, AttrLength, "100.0");
        QString f = formula;//need for saving fixed formula;

        const quint32 basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);

        const QString angle = GetParametrString(domElement, AttrAngle, "0.0");
        QString angleFix = angle;

        VToolEndLine::Create(id, name, typeLine, lineColor, f, angleFix, basePointId, mx, my, scene, this, data,
                             parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula || angleFix != angle)
        {
            SetAttribute(domElement, AttrLength, f);
            SetAttribute(domElement, AttrAngle, angleFix);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
        const QString formula = GetParametrString(domElement, AttrLength, "100.0");
        QString f = formula;//need for saving fixed formula;
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolAlongLine::Create(id, name, typeLine, lineColor, f, firstPointId, secondPointId, mx, my, scene,
                               this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
        const QString formula = GetParametrString(domElement, AttrLength, "100.0");
        QString f = formula;//need for saving fixed formula;
        const quint32 p1Line = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        const quint32 p2Line = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
        const quint32 pShoulder = GetParametrUInt(domElement, AttrPShoulder, NULL_ID_STR);

        VToolShoulderPoint::Create(id, f, p1Line, p2Line, pShoulder, typeLine, lineColor, name, mx, my, scene,
                                   this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
        const QString formula = GetParametrString(domElement, AttrLength, "100.0");
        QString f = formula;//need for saving fixed formula;
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        const qreal angle = GetParametrDouble(domElement, AttrAngle, "0.0");

        VToolNormal::Create(id, f, firstPointId, secondPointId, typeLine, lineColor, name, angle, mx, my, scene,
                            this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
        const QString formula = GetParametrString(domElement, AttrLength, "100.0");
        QString f = formula;//need for saving fixed formula;
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
        const quint32 thirdPointId = GetParametrUInt(domElement, AttrThirdPoint, NULL_ID_STR);

        VToolBisector::Create(id, f, firstPointId, secondPointId, thirdPointId,
                            typeLine, lineColor, name, mx, my, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 p1Line1Id = GetParametrUInt(domElement, AttrP1Line1, NULL_ID_STR);
        const quint32 p2Line1Id = GetParametrUInt(domElement, AttrP2Line1, NULL_ID_STR);
        const quint32 p1Line2Id = GetParametrUInt(domElement, AttrP1Line2, NULL_ID_STR);
        const quint32 p2Line2Id = GetParametrUInt(domElement, AttrP2Line2, NULL_ID_STR);

        VToolLineIntersect::Create(id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, name,
                                    mx, my, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const QString radius = GetParametrString(domElement, AttrRadius, "0");
        QString f = radius;//need for saving fixed formula;
        const quint32 center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolPointOfContact::Create(id, f, center, firstPointId, secondPointId, name, mx, my, scene, this,
                                    data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != radius)
        {
            SetAttribute(domElement, AttrRadius, f);
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
        quint32 id = 0;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, mx, my);
        const quint32 idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        QSharedPointer<VPointF> point;
        try
        {
            point = data->GeometricObject<VPointF>(idObject);
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        data->UpdateGObject(id, new VPointF(*point, point->name(), mx, my, idObject, Draw::Modeling));
        VNodePoint::Create(this, data, sceneDetail, id, idObject, parse, Source::FromFile, "", idTool);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VToolPin::Create(id, idObject, NULL_ID, this, data, parse, Source::FromFile, "", idTool);
    }
    catch (const VExceptionBadId &e)
    {
        VExceptionObjectError excep(tr("Error creating or updating pin point"), domElement);
        excep.AddMoreInformation(e.ErrorMessage());
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);
        const quint32 basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        const quint32 p1LineId = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        const quint32 p2LineId = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);

        VToolHeight::Create(id, name, typeLine, lineColor, basePointId, p1LineId, p2LineId,
                            mx, my, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 axisP1Id = GetParametrUInt(domElement, AttrAxisP1, NULL_ID_STR);
        const quint32 axisP2Id = GetParametrUInt(domElement, AttrAxisP2, NULL_ID_STR);
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolTriangle::Create(id, name, axisP1Id, axisP2Id, firstPointId, secondPointId, mx, my, scene, this,
                              data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 firstPointId = GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);

        VToolPointOfIntersection::Create(id, name, firstPointId, secondPointId, mx, my, scene, this, data,
                                         parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const QString formula = GetParametrString(domElement, AttrLength, "0");
        QString f = formula;//need for saving fixed formula;
        const quint32 splineId = GetParametrUInt(domElement, VToolCutSpline::AttrSpline, NULL_ID_STR);

        VToolCutSpline::Create(id, name, f, splineId, mx, my, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const QString formula = GetParametrString(domElement, AttrLength, "0");
        QString f = formula;//need for saving fixed formula;
        const quint32 splinePathId = GetParametrUInt(domElement, VToolCutSplinePath::AttrSplinePath,
                                                     NULL_ID_STR);

        VToolCutSplinePath::Create(id, name, f, splinePathId, mx, my, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const QString formula = GetParametrString(domElement, AttrLength, "0");
        QString f = formula;//need for saving fixed formula;
        const quint32 arcId = GetParametrUInt(domElement, AttrArc, NULL_ID_STR);

        VToolCutArc::Create(id, name, f, arcId, mx, my, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (f != formula)
        {
            SetAttribute(domElement, AttrLength, f);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

        const quint32 basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        const quint32 firstPointId = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        const quint32 secondPointId = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);

        const QString angle = GetParametrString(domElement, AttrAngle, "0.0");
        QString angleFix = angle;

        VToolLineIntersectAxis::Create(id, name, typeLine, lineColor, angleFix, basePointId, firstPointId,
                                       secondPointId, mx, my, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (angleFix != angle)
        {
            SetAttribute(domElement, AttrAngle, angleFix);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;
        QString typeLine;
        QString lineColor;

        PointsCommonAttributes(domElement, id, name, mx, my, typeLine, lineColor);

        const quint32 basePointId = GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
        const quint32 curveId = GetParametrUInt(domElement, AttrCurve, NULL_ID_STR);
        const QString angle = GetParametrString(domElement, AttrAngle, "0.0");
        QString angleFix = angle;

        VToolCurveIntersectAxis::Create(id, name, typeLine, lineColor, angleFix, basePointId, curveId, mx, my,
                                        scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (angleFix != angle)
        {
            SetAttribute(domElement, AttrAngle, angleFix);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 firstArcId = GetParametrUInt(domElement, AttrFirstArc, NULL_ID_STR);
        const quint32 secondArcId = GetParametrUInt(domElement, AttrSecondArc, NULL_ID_STR);
        const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                  AttrCrossPoint,
                                                                                  "1"));

        VToolPointOfIntersectionArcs::Create(id, name, firstArcId, secondArcId, crossPoint, mx, my, scene, this,
                                             data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 c1CenterId = GetParametrUInt(domElement, AttrC1Center, NULL_ID_STR);
        const quint32 c2CenterId = GetParametrUInt(domElement, AttrC2Center, NULL_ID_STR);
        const QString c1Radius = GetParametrString(domElement, AttrC1Radius);
        QString c1R = c1Radius;
        const QString c2Radius = GetParametrString(domElement, AttrC2Radius);
        QString c2R = c2Radius;
        const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                  AttrCrossPoint, "1"));

        VToolPointOfIntersectionCircles::Create(id, name, c1CenterId, c2CenterId, c1R, c2R, crossPoint, mx, my,
                                                scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (c1R != c1Radius || c2R != c2Radius)
        {
            SetAttribute(domElement, AttrC1Center, c1R);
            SetAttribute(domElement, AttrC2Center, c2R);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const auto curve1Id = GetParametrUInt(domElement, AttrCurve1, NULL_ID_STR);
        const auto curve2Id = GetParametrUInt(domElement, AttrCurve2, NULL_ID_STR);
        const auto vCrossPoint = static_cast<VCrossCurvesPoint>(GetParametrUInt(domElement, AttrVCrossPoint, "1"));
        const auto hCrossPoint = static_cast<HCrossCurvesPoint>(GetParametrUInt(domElement, AttrHCrossPoint, "1"));

        VToolPointOfIntersectionCurves::Create(id, name, curve1Id, curve2Id, vCrossPoint, hCrossPoint, mx, my,
                                               scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 cCenterId = GetParametrUInt(domElement, AttrCCenter, NULL_ID_STR);
        const quint32 tangentId = GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
        const QString cRadius = GetParametrString(domElement, AttrCRadius);
        QString cR = cRadius;
        const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                  AttrCrossPoint,
                                                                                  "1"));

        VToolPointFromCircleAndTangent::Create(id, name, cCenterId, cR, tangentId, crossPoint, mx, my,
                                               scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (cR != cRadius)
        {
            SetAttribute(domElement, AttrCCenter, cR);
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
        quint32 id = 0;
        QString name;
        qreal mx = 0;
        qreal my = 0;

        PointsCommonAttributes(domElement, id, name, mx, my);
        const quint32 arcId = GetParametrUInt(domElement, AttrArc, NULL_ID_STR);
        const quint32 tangentId = GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
        const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                  AttrCrossPoint,
                                                                                  "1"));

        VToolPointFromArcAndTangent::Create(id, name, arcId, tangentId, crossPoint, mx, my,
                                            scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);

        const quint32 p1Id = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 p2Id = GetParametrUInt(domElement, AttrPoint2, NULL_ID_STR);

        const quint32 baseLineP1Id = GetParametrUInt(domElement, AttrBaseLineP1, NULL_ID_STR);
        const quint32 baseLineP2Id = GetParametrUInt(domElement, AttrBaseLineP2, NULL_ID_STR);
        const quint32 dartP1Id = GetParametrUInt(domElement, AttrDartP1, NULL_ID_STR);
        const quint32 dartP2Id = GetParametrUInt(domElement, AttrDartP2, NULL_ID_STR);
        const quint32 dartP3Id = GetParametrUInt(domElement, AttrDartP3, NULL_ID_STR);

        const QString name1 = GetParametrString(domElement, AttrName1, "A");
        const qreal mx1 = qApp->toPixel(GetParametrDouble(domElement, AttrMx1, "10.0"));
        const qreal my1 = qApp->toPixel(GetParametrDouble(domElement, AttrMy1, "15.0"));

        const QString name2 = GetParametrString(domElement, AttrName2, "A");
        const qreal mx2 = qApp->toPixel(GetParametrDouble(domElement, AttrMx2, "10.0"));
        const qreal my2 = qApp->toPixel(GetParametrDouble(domElement, AttrMy2, "15.0"));

        VToolTrueDarts::Create(id, p1Id, p2Id,
                               baseLineP1Id, baseLineP2Id, dartP1Id, dartP2Id, dartP3Id,
                               name1, mx1, my1, name2, mx2, my2,
                               scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);
        const qreal angle1 = GetParametrDouble(domElement, AttrAngle1, "270.0");
        const qreal angle2 = GetParametrDouble(domElement, AttrAngle2, "90.0");
        const qreal kAsm1 = GetParametrDouble(domElement, AttrKAsm1, "1.0");
        const qreal kAsm2 = GetParametrDouble(domElement, AttrKAsm2, "1.0");
        const qreal kCurve = GetParametrDouble(domElement, AttrKCurve, "1.0");
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

        const auto p1 = data->GeometricObject<VPointF>(point1);
        const auto p4 = data->GeometricObject<VPointF>(point4);

        auto spline = new VSpline(*p1, *p4, angle1, angle2, kAsm1, kAsm2, kCurve);
        if (duplicate > 0)
        {
            spline->SetDuplicate(duplicate);
        }

        VToolSpline::Create(id, spline, color, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);

        const QString angle1 = GetParametrString(domElement, AttrAngle1, "0");
        QString a1 = angle1;//need for saving fixed formula;

        const QString angle2 = GetParametrString(domElement, AttrAngle2, "0");
        QString a2 = angle2;//need for saving fixed formula;

        const QString length1 = GetParametrString(domElement, AttrLength1, "0");
        QString l1 = length1;//need for saving fixed formula;

        const QString length2 = GetParametrString(domElement, AttrLength2, "0");
        QString l2 = length2;//need for saving fixed formula;

        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

        VToolSpline *spl = VToolSpline::Create(id, point1, point4, a1, a2, l1, l2, duplicate, color, scene, this,
                                                data, parse, Source::FromFile);

        if (spl != nullptr)
        {
            VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
            SCASSERT(window != nullptr)
            connect(spl, &VToolSpline::ToolTip, window, &VAbstractMainWindow::ShowToolTip);
        }

        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (a1 != angle1 || a2 != angle2 || l1 != length1 || l2 != length2)
        {
            SetAttribute(domElement, AttrAngle1, a1);
            SetAttribute(domElement, AttrAngle2, a2);
            SetAttribute(domElement, AttrLength1, l1);
            SetAttribute(domElement, AttrLength2, l2);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 point1 = GetParametrUInt(domElement, AttrPoint1, NULL_ID_STR);
        const quint32 point2 = GetParametrUInt(domElement, AttrPoint2, NULL_ID_STR);
        const quint32 point3 = GetParametrUInt(domElement, AttrPoint3, NULL_ID_STR);
        const quint32 point4 = GetParametrUInt(domElement, AttrPoint4, NULL_ID_STR);

        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

        auto p1 = data->GeometricObject<VPointF>(point1);
        auto p2 = data->GeometricObject<VPointF>(point2);
        auto p3 = data->GeometricObject<VPointF>(point3);
        auto p4 = data->GeometricObject<VPointF>(point4);

        VCubicBezier *spline = new VCubicBezier(*p1, *p2, *p3, *p4);
        if (duplicate > 0)
        {
            spline->SetDuplicate(duplicate);
        }

        VToolCubicBezier::Create(id, spline, color, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const qreal kCurve = GetParametrDouble(domElement, AttrKCurve, "1.0");
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

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
                    const qreal angle = GetParametrDouble(element, AttrAngle, "0");
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

        auto path = new VSplinePath(points, kCurve);
        if (duplicate > 0)
        {
            path->SetDuplicate(duplicate);
        }

        VToolSplinePath::Create(id, path, color, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

        auto path = new VSplinePath();
        if (duplicate > 0)
        {
            path->SetDuplicate(duplicate);
        }

        QVector<quint32> points;
        QVector<QString> angle1, a1;
        QVector<QString> angle2, a2;
        QVector<QString> length1, l1;
        QVector<QString> length2, l2;

        const QDomNodeList nodeList = domElement.childNodes();
        const qint32 num = nodeList.size();
        for (qint32 i = 0; i < num; ++i)
        {
            const QDomElement element = nodeList.at(i).toElement();
            if (not element.isNull() && element.tagName() == AttrPathPoint)
            {
                angle1.append(GetParametrString(element, AttrAngle1, "0"));
                angle2.append(GetParametrString(element, AttrAngle2, "0"));
                length1.append(GetParametrString(element, AttrLength1, "0"));
                length2.append(GetParametrString(element, AttrLength2, "0"));
                const quint32 pSpline = GetParametrUInt(element, AttrPSpline, NULL_ID_STR);
                points.append(pSpline);

                if (parse == Document::FullParse)
                {
                    IncrementReferens(data->GeometricObject<VPointF>(pSpline)->getIdTool());
                }
            }
        }

        //need for saving fixed formula;
        a1 = angle1;
        a2 = angle2;
        l1 = length1;
        l2 = length2;

        VToolSplinePath *spl = VToolSplinePath::Create(id, points, a1, a2, l1, l2, color, scene, this, data, parse,
                                                        Source::FromFile);

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
                if (a1.at(count) != angle1.at(count) || a2.at(count) != angle2.at(count) ||
                    l1.at(count) != length1.at(count) || l2.at(count) != length2.at(count))
                {
                    SetAttribute(element, AttrAngle1, a1.at(count));
                    SetAttribute(element, AttrAngle2, a2.at(count));
                    SetAttribute(element, AttrLength1, l1.at(count));
                    SetAttribute(element, AttrLength2, l2.at(count));
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);
        const quint32 duplicate = GetParametrUInt(domElement, AttrDuplicate, "0");

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

        auto path = new VCubicBezierPath(points);
        if (duplicate > 0)
        {
            path->SetDuplicate(duplicate);
        }

        VToolCubicBezierPath::Create(id, path, color, scene, this, data, parse, Source::FromFile);
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
        quint32 id = 0;
        quint32 idObject = 0;
        quint32 idTool = 0;

        SplinesCommonAttributes(domElement, id, idObject, idTool);
        try
        {
            const auto obj = data->GetGObject(idObject);
            if (obj->getType() == GOType::Spline)
            {
                VSpline *spl = new VSpline(*data->GeometricObject<VSpline>(idObject));
                spl->setIdObject(idObject);
                spl->setMode(Draw::Modeling);
                data->UpdateGObject(id, spl);
            }
            else
            {
                VCubicBezier *spl = new VCubicBezier(*data->GeometricObject<VCubicBezier>(idObject));
                spl->setIdObject(idObject);
                spl->setMode(Draw::Modeling);
                data->UpdateGObject(id, spl);
            }
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }

        VNodeSpline::Create(this, data, id, idObject, parse, Source::FromFile, "", idTool);
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
        quint32 id = 0;
        quint32 idObject = 0;
        quint32 idTool = 0;

        SplinesCommonAttributes(domElement, id, idObject, idTool);
        try
        {
            const auto obj = data->GetGObject(idObject);
            if (obj->getType() == GOType::SplinePath)
            {
                VSplinePath *path = new VSplinePath(*data->GeometricObject<VSplinePath>(idObject));
                path->setIdObject(idObject);
                path->setMode(Draw::Modeling);
                data->UpdateGObject(id, path);
            }
            else
            {
                VCubicBezierPath *spl = new VCubicBezierPath(*data->GeometricObject<VCubicBezierPath>(idObject));
                spl->setIdObject(idObject);
                spl->setMode(Draw::Modeling);
                data->UpdateGObject(id, spl);
            }
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        VNodeSplinePath::Create(this, data, id, idObject, parse, Source::FromFile, "", idTool);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const QString radius = GetParametrString(domElement, AttrRadius, "10");
        QString r = radius;//need for saving fixed formula;
        const QString f1 = GetParametrString(domElement, AttrAngle1, "180");
        QString f1Fix = f1;//need for saving fixed formula;
        const QString f2 = GetParametrString(domElement, AttrAngle2, "270");
        QString f2Fix = f2;//need for saving fixed formula;
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);

        VToolArc::Create(id, center, r, f1Fix, f2Fix, color, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r != radius || f1Fix != f1 || f2Fix != f2)
        {
            SetAttribute(domElement, AttrRadius, r);
            SetAttribute(domElement, AttrAngle1, f1Fix);
            SetAttribute(domElement, AttrAngle2, f2Fix);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const QString radius1 = GetParametrString(domElement, AttrRadius1, "10");
        const QString radius2 = GetParametrString(domElement, AttrRadius2, "10");
        QString r1 = radius1;//need for saving fixed formula;
        QString r2 = radius2;//need for saving fixed formula;
        const QString f1 = GetParametrString(domElement, AttrAngle1, "180");
        QString f1Fix = f1;//need for saving fixed formula;
        const QString f2 = GetParametrString(domElement, AttrAngle2, "270");
        QString f2Fix = f2;//need for saving fixed formula;
        const QString frotation = GetParametrString(domElement, AttrRotationAngle, "0");
        QString frotationFix = frotation;//need for saving fixed formula;
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);

        VToolEllipticalArc::Create(id, center, r1, r2, f1Fix, f2Fix, frotationFix, color, scene, this, data, parse,
                                   Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r1 != radius1 || r2 != radius2 || f1Fix != f1 || f2Fix != f2 || frotationFix != frotation)
        {
            SetAttribute(domElement, AttrRadius1, r1);
            SetAttribute(domElement, AttrRadius2, r2);
            SetAttribute(domElement, AttrAngle1, f1Fix);
            SetAttribute(domElement, AttrAngle2, f2Fix);
            SetAttribute(domElement, AttrRotationAngle, frotationFix);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VEllipticalArc *arc = nullptr;
        try
        {
            arc = new VEllipticalArc(*data->GeometricObject<VEllipticalArc>(idObject));
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        arc->setIdObject(idObject);
        arc->setMode(Draw::Modeling);
        data->UpdateGObject(id, arc);
        VNodeEllipticalArc::Create(this, data, id, idObject, parse, Source::FromFile, "", idTool);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 idObject = GetParametrUInt(domElement, AttrIdObject, NULL_ID_STR);
        const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        VArc *arc = nullptr;
        try
        {
            arc = new VArc(*data->GeometricObject<VArc>(idObject));
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return;// Just ignore
        }
        arc->setIdObject(idObject);
        arc->setMode(Draw::Modeling);
        data->UpdateGObject(id, arc);
        VNodeArc::Create(this, data, id, idObject, parse, Source::FromFile, "", idTool);
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
        quint32 id = 0;

        ToolsCommonAttributes(domElement, id);
        const quint32 center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const QString radius = GetParametrString(domElement, AttrRadius, "10");
        QString r = radius;//need for saving fixed formula;
        const QString f1 = GetParametrString(domElement, AttrAngle1, "180");
        QString f1Fix = f1;//need for saving fixed formula;
        const QString length = GetParametrString(domElement, AttrLength, "10");
        QString lengthFix = length;//need for saving fixed length;
        const QString color = GetParametrString(domElement, AttrColor, ColorBlack);

        VToolArcWithLength::Create(id, center, r, f1Fix, lengthFix, color, scene, this, data, parse,
                                   Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (r != radius || f1Fix != f1 || lengthFix != length)
        {
            SetAttribute(domElement, AttrRadius, r);
            SetAttribute(domElement, AttrAngle1, f1Fix);
            SetAttribute(domElement, AttrLength, lengthFix);
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
        quint32 id = NULL_ID;

        ToolsCommonAttributes(domElement, id);
        const quint32 center = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const QString angle = GetParametrString(domElement, AttrAngle, "10");
        QString a = angle;//need for saving fixed formula;
        const QString suffix = GetParametrString(domElement, AttrSuffix, "");

        QVector<quint32> source;
        QVector<DestinationItem> destination;
        VAbstractOperation::ExtractData(domElement, source, destination);

        VToolRotation::Create(id, center, a, suffix, source, destination, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (a != angle)
        {
            SetAttribute(domElement, AttrAngle, a);
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
        quint32 id = NULL_ID;

        ToolsCommonAttributes(domElement, id);
        const quint32 p1 = GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
        const quint32 p2 = GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
        const QString suffix = GetParametrString(domElement, AttrSuffix, "");

        QVector<quint32> source;
        QVector<DestinationItem> destination;
        VAbstractOperation::ExtractData(domElement, source, destination);

        VToolFlippingByLine::Create(id, p1, p2, suffix, source, destination, scene, this, data, parse,
                                    Source::FromFile);
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
        quint32 id = NULL_ID;

        ToolsCommonAttributes(domElement, id);
        const quint32 origin = GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
        const auto axisType = static_cast<AxisType>(GetParametrUInt(domElement, AttrAxisType, "1"));
        const QString suffix = GetParametrString(domElement, AttrSuffix, "");

        QVector<quint32> source;
        QVector<DestinationItem> destination;
        VAbstractOperation::ExtractData(domElement, source, destination);

        VToolFlippingByAxis::Create(id, origin, axisType, suffix, source, destination, scene, this, data, parse,
                                    Source::FromFile);
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
        quint32 id = NULL_ID;

        ToolsCommonAttributes(domElement, id);
        const QString angle = GetParametrString(domElement, AttrAngle, "0");
        QString a = angle;//need for saving fixed formula;
        const QString length = GetParametrString(domElement, AttrLength, "0");
        QString len = length;//need for saving fixed formula;
        const QString suffix = GetParametrString(domElement, AttrSuffix, "");

        QVector<quint32> source;
        QVector<DestinationItem> destination;
        VAbstractOperation::ExtractData(domElement, source, destination);

        VToolMove::Create(id, a, len, suffix, source, destination, scene, this, data, parse, Source::FromFile);
        //Rewrite attribute formula. Need for situation when we have wrong formula.
        if (a != angle || len != length)
        {
            SetAttribute(domElement, AttrAngle, a);
            SetAttribute(domElement, AttrLength, len);
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
            // Replace line return character with spaces for calc if exist
            QString f = formula;
            f.replace("\n", " ");
            QScopedPointer<Calculator> cal(new Calculator());
            const qreal result = cal->EvalFormula(data->PlainVariables(), f);

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
    SetAttribute(element, IncrementName, name);
    SetAttribute(element, IncrementFormula, QString("0"));
    SetAttribute(element, IncrementDescription, QString(""));
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
            const QString parameter = domElement.attribute(IncrementName);
            if (parameter == name)
            {
                return domElement;
            }
        }
    }

    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::GarbageCollector()
{
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
                if (modNode.hasAttribute(VAbstractNode::AttrInUse))
                {
                    const NodeUsage inUse = GetParametrUsage(modNode, VAbstractNode::AttrInUse);
                    if (inUse == NodeUsage::InUse)
                    { // It is dangerous to leave object with attribute 'inUse'
                      // Each parse should confirm this status.
                        SetParametrUsage(modNode, VAbstractNode::AttrInUse, NodeUsage::NotInUse);
                    }
                    else
                    { // Parent was deleted. We do not need this object anymore
                        modElement.removeChild(modNode);
                    }
                }
                else
                { // Each parse should confirm his status.
                    SetParametrUsage(modNode, VAbstractNode::AttrInUse, NodeUsage::NotInUse);
                }

                modNode = nextSibling;
            }
        }
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
                quint32 id = 0;
                ToolsCommonAttributes(domElement, id);

                VToolUnionDetailsInitData initData;
                initData.indexD1 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD1, "-1");
                initData.indexD2 = GetParametrUInt(domElement, VToolUnionDetails::AttrIndexD2, "-1");
                initData.scene = scene;
                initData.doc = this;
                initData.data = data;
                initData.parse = parse;
                initData.typeCreation = Source::FromFile;

                VToolUnionDetails::Create(id, initData);
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
        quint32 id = 0;
        ToolsCommonAttributes(domElement, id);
        const QString name = GetParametrString(domElement, AttrName, tr("Unnamed path"));
        const QString defType = QString().setNum(static_cast<int>(PiecePathType::CustomSeamAllowance));
        const PiecePathType type = static_cast<PiecePathType>(GetParametrUInt(domElement, AttrType, defType));
        const quint32 idTool = GetParametrUInt(domElement, VAbstractNode::AttrIdTool, NULL_ID_STR);
        const QString penType = GetParametrString(domElement, AttrTypeLine, TypeLineLine);

        VPiecePath path;
        const QDomElement element = domElement.firstChildElement(VAbstractPattern::TagNodes);
        if (not element.isNull())
        {
            path = ParsePathNodes(element);
        }

        path.SetType(type);
        path.SetName(name);
        path.SetPenType(VAbstractTool::LineStyleToPenStyle(penType));

        VToolPiecePath::Create(id, path, 0, scene, this, data, parse, Source::FromFile, "", idTool);
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
 */
void VPattern::ParseIncrementsElement(const QDomNode &node)
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
                    const QString name = GetParametrString(domElement, IncrementName, "");

                    QString desc;
                    try
                    {
                        desc = GetParametrString(domElement, IncrementDescription);
                    }
                    catch (VExceptionEmptyParameter &e)
                    {
                        Q_UNUSED(e)
                    }

                    const QString formula = GetParametrString(domElement, IncrementFormula, "0");
                    bool ok = false;
                    const qreal value = EvalFormula(data, formula, &ok);

                    data->AddVariable(name, new VIncrement(data, name, static_cast<quint32>(index), value, formula, ok,
                                                           desc));
                    ++index;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VPattern::GetAuthor() const
{
    return UniqueTagText(TagAuthor, qApp->ValentinaSettings()->GetUser());
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetAuthor(const QString &text)
{
    CheckTagExists(TagAuthor);
    setTagText(TagAuthor, text);
    modified = true;
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyIncrement(const QString &name)
{
    const QDomElement element = MakeEmptyIncrement(name);

    const QDomNodeList list = elementsByTagName(TagIncrements);
    list.at(0).appendChild(element);
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::AddEmptyIncrementAfter(const QString &after, const QString &name)
{
    const QDomElement element = MakeEmptyIncrement(name);
    const QDomElement sibling = FindIncrement(after);

    const QDomNodeList list = elementsByTagName(TagIncrements);

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
void VPattern::RemoveIncrement(const QString &name)
{
    const QDomNodeList list = elementsByTagName(TagIncrements);
    list.at(0).removeChild(FindIncrement(name));
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveUpIncrement(const QString &name)
{
    const QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        const QDomElement prSibling = node.previousSiblingElement(TagIncrement);
        if (not prSibling.isNull())
        {
            const QDomNodeList list = elementsByTagName(TagIncrements);
            list.at(0).insertBefore(node, prSibling);
        }
    }
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::MoveDownIncrement(const QString &name)
{
    const QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        const QDomElement nextSibling = node.nextSiblingElement(TagIncrement);
        if (not nextSibling.isNull())
        {
            const QDomNodeList list = elementsByTagName(TagIncrements);
            list.at(0).insertAfter(node, nextSibling);
        }
    }
    emit patternChanged(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementName(const QString &name, const QString &text)
{
    QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        SetAttribute(node, IncrementName, text);
        emit patternChanged(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementFormula(const QString &name, const QString &text)
{
    QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        SetAttribute(node, IncrementFormula, text);
        emit patternChanged(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetIncrementDescription(const QString &name, const QString &text)
{
    QDomElement node = FindIncrement(name);
    if (not node.isNull())
    {
        SetAttribute(node, IncrementDescription, text);
        emit patternChanged(false);
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
    const QStringList uniqueNames = data->AllUniqueNames();
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
            return static_cast<int>(GetParametrUInt(domElement, AttrDefHeight, QStringLiteral("0")));
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
            return static_cast<int>(GetParametrUInt(domElement, AttrDefSize, QStringLiteral("0")));
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
        data->ClearUniqueNames();
        data->ClearVariables(VarType::Increment);
        data->ClearVariables(VarType::LineAngle);
        data->ClearVariables(VarType::LineLength);
        data->ClearVariables(VarType::CurveLength);
        data->ClearVariables(VarType::CurveCLength);
        data->ClearVariables(VarType::ArcRadius);
        data->ClearVariables(VarType::CurveAngle);
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
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 52, "Not all tools were used.");

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
                    rec = ToolBoundingRect<VToolBasePoint>(rec, tool.getId());
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
                case Tool::CubicBezier:
                    rec = ToolBoundingRect<VToolCubicBezier>(rec, tool.getId());
                    break;
                case Tool::Arc:
                    rec = ToolBoundingRect<VToolArc>(rec, tool.getId());
                    break;
                case Tool::SplinePath:
                    rec = ToolBoundingRect<VToolSplinePath>(rec, tool.getId());
                    break;
                case Tool::CubicBezierPath:
                    rec = ToolBoundingRect<VToolCubicBezierPath>(rec, tool.getId());
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
                case Tool::ArcWithLength:
                    rec = ToolBoundingRect<VToolArcWithLength>(rec, tool.getId());
                    break;
                case Tool::LineIntersectAxis:
                    rec = ToolBoundingRect<VToolLineIntersectAxis>(rec, tool.getId());
                    break;
                case Tool::PointOfIntersectionArcs:
                    rec = ToolBoundingRect<VToolPointOfIntersectionArcs>(rec, tool.getId());
                    break;
                case Tool::PointOfIntersectionCircles:
                    rec = ToolBoundingRect<VToolPointOfIntersectionCircles>(rec, tool.getId());
                    break;
                case Tool::PointOfIntersectionCurves:
                    rec = ToolBoundingRect<VToolPointOfIntersectionCurves>(rec, tool.getId());
                    break;
                case Tool::CurveIntersectAxis:
                    rec = ToolBoundingRect<VToolCurveIntersectAxis>(rec, tool.getId());
                    break;
                case Tool::PointFromCircleAndTangent:
                    rec = ToolBoundingRect<VToolPointFromCircleAndTangent>(rec, tool.getId());
                    break;
                case Tool::PointFromArcAndTangent:
                    rec = ToolBoundingRect<VToolPointFromArcAndTangent>(rec, tool.getId());
                    break;
                case Tool::TrueDarts:
                    rec = ToolBoundingRect<VToolTrueDarts>(rec, tool.getId());
                    break;
                case Tool::Rotation:
                    rec = ToolBoundingRect<VToolRotation>(rec, tool.getId());
                    break;
                case Tool::FlippingByLine:
                    rec = ToolBoundingRect<VToolFlippingByLine>(rec, tool.getId());
                    break;
                case Tool::FlippingByAxis:
                    rec = ToolBoundingRect<VToolFlippingByAxis>(rec, tool.getId());
                    break;
                case Tool::Move:
                    rec = ToolBoundingRect<VToolMove>(rec, tool.getId());
                    break;
                case Tool::EllipticalArc:
                    rec = ToolBoundingRect<VToolEllipticalArc>(rec, tool.getId());
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
