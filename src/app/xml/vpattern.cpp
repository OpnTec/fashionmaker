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
#include "../../libs/vtools/tools/vdatatool.h"
#include "../../libs/vtools/tools/vtooldetail.h"
#include "../../libs/vtools/tools/vtooluniondetails.h"
#include "../../libs/vtools/tools/drawTools/drawtools.h"
#include "../../libs/vtools/tools/nodeDetails/nodedetails.h"
#include "../../libs/ifc/exception/vexceptionobjecterror.h"
#include "../../libs/ifc/exception/vexceptionwrongid.h"
#include "../../libs/ifc/exception/vexceptionconversionerror.h"
#include "../../libs/ifc/exception/vexceptionemptyparameter.h"
#include "../../libs/ifc/exception/vexceptionundo.h"
#include "../../libs/ifc/xml/vpatternconverter.h"
#include "../../libs/vmisc/undoevent.h"
#include "../../libs/vmisc/vsettings.h"
#include "vstandardmeasurements.h"
#include "vindividualmeasurements.h"
#include "../../libs/qmuparser/qmuparsererror.h"
#include "../../libs/vgeometry/varc.h"
#include "../xml/vpattern.h"
#include "../core/vapplication.h"

#include <QMessageBox>
#include <QUndoStack>
#include <QtCore/qmath.h>

//---------------------------------------------------------------------------------------------------------------------
VPattern::VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw,
                   VMainGraphicsScene *sceneDetail, QObject *parent)
    : VAbstractPattern(parent), data(data), mode(mode), sceneDraw(sceneDraw), sceneDetail(sceneDetail)
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

    patternElement.appendChild(createComment("Pattern created with Valentina (http://www.valentina-project.org/)."));

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
                    default:
                        qCDebug(vXML, "Wrong tag name %s", domElement.tagName().toUtf8().constData());
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
            qCDebug(vXML, "Current PP name %s", nameActivPP.toUtf8().constData());
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
                        nameActivPP.toUtf8().constData());

                const VToolRecord tool = history.at(history.size()-1);
                id = tool.getId();
                qCDebug(vXML, "Taking record with id %u from PP %s", id, tool.getNameDraw().toUtf8().constData());
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
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    SCASSERT(data != nullptr);
    ToolExists(id);
    VDataTool *tool = tools.value(id);
    SCASSERT(tool != nullptr);
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
                if (domElement.tagName() == VToolSinglePoint::TagName &&
                        domElement.attribute(AttrType, "") == VToolBasePoint::ToolType)
                {
                    return GetParametrId(domElement);
                }
            }
        }
    }
    return 0;
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
                qCWarning(vXML, "Lite parsing doesn't support full parsing");
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
    qCDebug(vXML, "Current pattern piece %s", nameActivPP.toUtf8().constData());
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
                    default:
                        qCDebug(vXML, "Wrong tag name");
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
                default:
                    qCDebug(vXML, "Wrong tag name");
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

    QStringList points = QStringList() << VToolBasePoint::ToolType << VToolEndLine::ToolType
                                       << VToolAlongLine::ToolType << VToolShoulderPoint::ToolType
                                       << VToolNormal::ToolType << VToolBisector::ToolType
                                       << VToolLineIntersect::ToolType << VToolPointOfContact::ToolType
                                       << VNodePoint::ToolType << VToolHeight::ToolType << VToolTriangle::ToolType
                                       << VToolPointOfIntersection::ToolType << VToolCutSpline::ToolType
                                       << VToolCutSplinePath::ToolType << VToolCutArc::ToolType
                                       << VToolLineIntersectAxis::ToolType << VToolCurveIntersectAxis::ToolType
                                       << VToolPointOfIntersectionArcs::ToolType
                                       << VToolPointOfIntersectionCircles::ToolType
                                       << VToolPointFromCircleAndTangent::ToolType
                                       << VToolPointFromArcAndTangent::ToolType;
    switch (points.indexOf(type))
    {
        case 0: //VToolBasePoint::ToolType
        {
            VToolBasePoint *spoint = 0;
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const qreal x = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrX, "10.0"));
                const qreal y = qApp->toPixel(GetParametrDouble(domElement, VAbstractTool::AttrY, "10.0"));

                data->UpdateGObject(id, new VPointF(x, y, name, mx, my));
                VDrawTool::AddRecord(id, Tool::BasePoint, this);
                if (parse != Document::FullParse)
                {
                    UpdateToolData(id, data);
                }
                if (parse == Document::FullParse)
                {
                    spoint = new VToolBasePoint(this, data, id, Source::FromFile, nameActivPP, MPath());
                    scene->addItem(spoint);
                    connect(spoint, &VToolBasePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
                    connect(scene, &VMainGraphicsScene::NewFactor, spoint, &VToolBasePoint::SetFactor);
                    connect(scene, &VMainGraphicsScene::DisableItem, spoint, &VToolBasePoint::Disable);
                    connect(scene, &VMainGraphicsScene::EnableToolMove, spoint, &VToolBasePoint::EnableToolMove);
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
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolCutSpline::Create(id, name, f, splineId, mx, my, color, scene, this, data, parse,
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
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolCutSplinePath::Create(id, name, f, splinePathId, mx, my, color, scene, this, data, parse,
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
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolCutArc::Create(id, name, f, arcId, mx, my, color, scene, this, data, parse, Source::FromFile);
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
        case 17: //VToolPointOfIntersectionArcs::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 firstArcId = GetParametrUInt(domElement, VAbstractTool::AttrFirstArc, NULL_ID_STR);
                const quint32 secondArcId = GetParametrUInt(domElement, VAbstractTool::AttrSecondArc, NULL_ID_STR);
                const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                          VAbstractTool::AttrCrossPoint,
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
            break;
        case 18: //VToolPointOfIntersectionCircles::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 c1CenterId = GetParametrUInt(domElement, VAbstractTool::AttrC1Center, NULL_ID_STR);
                const quint32 c2CenterId = GetParametrUInt(domElement, VAbstractTool::AttrC2Center, NULL_ID_STR);
                const QString c1Radius = GetParametrString(domElement, VAbstractTool::AttrC1Radius);
                QString c1R = c1Radius;
                const QString c2Radius = GetParametrString(domElement, VAbstractTool::AttrC2Radius);
                QString c2R = c2Radius;
                const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                          VAbstractTool::AttrCrossPoint,
                                                                                          "1"));

                VToolPointOfIntersectionCircles::Create(id, name, c1CenterId, c2CenterId, c1R, c2R, crossPoint, mx, my,
                                                        scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (c1R != c1Radius || c2R != c2Radius)
                {
                    SetAttribute(domElement, VAbstractTool::AttrC1Center, c1R);
                    SetAttribute(domElement, VAbstractTool::AttrC2Center, c2R);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point of intersection circles"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 19: //VToolPointFromCircleAndTangent::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 cCenterId = GetParametrUInt(domElement, VAbstractTool::AttrCCenter, NULL_ID_STR);
                const quint32 tangentId = GetParametrUInt(domElement, VAbstractTool::AttrTangent, NULL_ID_STR);
                const QString cRadius = GetParametrString(domElement, VAbstractTool::AttrCRadius);
                QString cR = cRadius;
                const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                          VAbstractTool::AttrCrossPoint,
                                                                                          "1"));

                VToolPointFromCircleAndTangent::Create(id, name, cCenterId, cR, tangentId, crossPoint, mx, my,
                                                       scene, this, data, parse, Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (cR != cRadius)
                {
                    SetAttribute(domElement, VAbstractTool::AttrCCenter, cR);
                    haveLiteChange();
                }
            }
            catch (const VExceptionBadId &e)
            {
                VExceptionObjectError excep(tr("Error creating or updating point from circle and tangent"), domElement);
                excep.AddMoreInformation(e.ErrorMessage());
                throw excep;
            }
            break;
        case 20: //VToolPointFromArcAndTangent::ToolType
            try
            {
                PointsCommonAttributes(domElement, id, name, mx, my);
                const quint32 arcId = GetParametrUInt(domElement, VAbstractTool::AttrArc, NULL_ID_STR);
                const quint32 tangentId = GetParametrUInt(domElement, VAbstractTool::AttrTangent, NULL_ID_STR);
                const CrossCirclesPoint crossPoint = static_cast<CrossCirclesPoint>(GetParametrUInt(domElement,
                                                                                          VAbstractTool::AttrCrossPoint,
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
    try
    {
        quint32 id = 0;
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
QString VPattern::GetLabelBase(unsigned int index) const
{
    QStringList list = VApplication::LabelLanguages();
    QStringList alphabet;
    switch (list.indexOf(qApp->Settings()->GetLabelLanguage()))
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
            qCDebug(vXML, "VToolSpline.");
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
            qCDebug(vXML, "VToolSplinePath.");
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
            qCDebug(vXML, "VNodeSpline.");
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
            qCDebug(vXML, "VNodeSplinePath.");
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
            qCDebug(vXML, "Illegal spline type in VDomDocument::ParseSplineElement().");
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
    QStringList arcs = QStringList() << VToolArc::ToolType << VNodeArc::ToolType << VToolArcWithLength::ToolType;

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
        case 2: //VToolArcWithLength::ToolType
            try
            {
                ToolsCommonAttributes(domElement, id);
                const quint32 center = GetParametrUInt(domElement, VAbstractTool::AttrCenter, NULL_ID_STR);
                const QString radius = GetParametrString(domElement, VAbstractTool::AttrRadius, "10");
                QString r = radius;//need for saving fixed formula;
                const QString f1 = GetParametrString(domElement, VAbstractTool::AttrAngle1, "180");
                QString f1Fix = f1;//need for saving fixed formula;
                const QString length = GetParametrString(domElement, VAbstractTool::AttrLength, "10");
                QString lengthFix = length;//need for saving fixed length;
                const QString color = GetParametrString(domElement, VAbstractTool::AttrColor,
                                                        VAbstractTool::ColorBlack);

                VToolArcWithLength::Create(id, center, r, f1Fix, lengthFix, color, scene, this, data, parse,
                                           Source::FromFile);
                //Rewrite attribute formula. Need for situation when we have wrong formula.
                if (r != radius || f1Fix != f1 || lengthFix != length)
                {
                    SetAttribute(domElement, VAbstractTool::AttrRadius, r);
                    SetAttribute(domElement, VAbstractTool::AttrAngle1, f1Fix);
                    SetAttribute(domElement, VAbstractTool::AttrLength, lengthFix);
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

    QStringList tools{VToolUnionDetails::ToolType};

    switch (tools.indexOf(type))
    {
        case 0: //VToolUnionDetails::ToolType
            try
            {
                quint32 id = 0;
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
QString VPattern::GetAuthor() const
{
    return UniqueTagText(TagAuthor, qApp->Settings()->GetUser());
}

//---------------------------------------------------------------------------------------------------------------------
void VPattern::SetAuthor(const QString &text)
{
    CheckTagExists(TagAuthor);
    setTagText(TagAuthor, text);
    emit patternChanged(false);
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
        qCDebug(vXML, "Point label: %s", name.toUtf8().constData());
        return name;
    }
    else if (type == LabelType::NewLabel)
    {
        if (drawList.isEmpty())
        {
            const QString label = GetLabelBase(0);
            qCDebug(vXML, "Point label: %s", label.toUtf8().constData());
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
        } while (data->IsUnique(name) == false || name == reservedName);
        qCDebug(vXML, "Point label: %s", name.toUtf8().constData());
        return name;
    }
    qCDebug(vXML, "Got unknow type %d", static_cast<int>(type));
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
        data->ClearVariables(VarType::ArcRadius);
        data->ClearVariables(VarType::ArcAngle);
        data->ClearVariables(VarType::SplineAngle);
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
