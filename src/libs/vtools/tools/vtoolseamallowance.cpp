/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vtoolseamallowance.h"
#include "../dialogs/tools/piece/dialogseamallowance.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "nodeDetails/vnodearc.h"
#include "nodeDetails/vnodeellipticalarc.h"
#include "nodeDetails/vnodepoint.h"
#include "nodeDetails/vnodespline.h"
#include "nodeDetails/vnodesplinepath.h"
#include "nodeDetails/vtoolpiecepath.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../undocommands/addpiece.h"
#include "../undocommands/deletepiece.h"
#include "../undocommands/movepiece.h"
#include "../undocommands/savepieceoptions.h"
#include "../undocommands/togglepieceinlayout.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vwidgets/vnobrushscalepathitem.h"
#include "../qmuparser/qmutokenparser.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>

// Current version of seam allowance tag need for backward compatibility
const quint8 VToolSeamAllowance::pieceVersion = 2;

const QString VToolSeamAllowance::TagCSA     = QStringLiteral("csa");
const QString VToolSeamAllowance::TagRecord  = QStringLiteral("record");
const QString VToolSeamAllowance::TagIPaths  = QStringLiteral("iPaths");
const QString VToolSeamAllowance::TagPins    = QStringLiteral("pins");

const QString VToolSeamAllowance::AttrVersion              = QStringLiteral("version");
const QString VToolSeamAllowance::AttrForbidFlipping       = QStringLiteral("forbidFlipping");
const QString VToolSeamAllowance::AttrSeamAllowance        = QStringLiteral("seamAllowance");
const QString VToolSeamAllowance::AttrHideMainPath         = QStringLiteral("hideMainPath");
const QString VToolSeamAllowance::AttrSeamAllowanceBuiltIn = QStringLiteral("seamAllowanceBuiltIn");
const QString VToolSeamAllowance::AttrHeight               = QStringLiteral("height");
const QString VToolSeamAllowance::AttrUnited               = QStringLiteral("united");
const QString VToolSeamAllowance::AttrFont                 = QStringLiteral("fontSize");
const QString VToolSeamAllowance::AttrTopLeftPin           = QStringLiteral("topLeftPin");
const QString VToolSeamAllowance::AttrBottomRightPin       = QStringLiteral("bottomRightPin");
const QString VToolSeamAllowance::AttrCenterPin            = QStringLiteral("centerPin");
const QString VToolSeamAllowance::AttrTopPin               = QStringLiteral("topPin");
const QString VToolSeamAllowance::AttrBottomPin            = QStringLiteral("bottomPin");

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull());
    QSharedPointer<DialogSeamAllowance> dialogTool = dialog.objectCast<DialogSeamAllowance>();
    SCASSERT(not dialogTool.isNull())
    VPiece detail = dialogTool->GetPiece();
    QString width = detail.GetFormulaSAWidth();
    qApp->getUndoStack()->beginMacro("add detail");
    detail.GetPath().SetNodes(PrepareNodes(detail.GetPath(), scene, doc, data));

    VToolSeamAllowance *piece = Create(0, detail, width, scene, doc, data, Document::FullParse, Source::FromGui);

    if (piece != nullptr)
    {
        piece->m_dialog = dialogTool;
    }
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(quint32 id, VPiece newPiece, QString &width, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data, const Document &parse,
                                               const Source &typeCreation, const QString &drawName)
{
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        data->AddVariable(currentSeamAllowance, new VIncrement(data, currentSeamAllowance, 0, newPiece.GetSAWidth(),
                                                               width, true, tr("Current seam allowance")));
        id = data->AddPiece(newPiece);
    }
    else
    {
        const qreal calcWidth = CheckFormula(id, width, data);
        newPiece.SetFormulaSAWidth(width, calcWidth);

        data->AddVariable(currentSeamAllowance, new VIncrement(data, currentSeamAllowance, 0, calcWidth,
                                                               width, true, tr("Current seam allowance")));

        data->UpdatePiece(id, newPiece);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VToolSeamAllowance *piece = nullptr;
    if (parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(id, Tool::Piece, doc);
        piece = new VToolSeamAllowance(doc, data, id, typeCreation, scene, drawName);
        scene->addItem(piece);
        connect(piece, &VToolSeamAllowance::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::EnableDetailItemHover, piece, &VToolSeamAllowance::AllowHover);
        connect(scene, &VMainGraphicsScene::EnableDetailItemSelection, piece, &VToolSeamAllowance::AllowSelecting);
        connect(scene, &VMainGraphicsScene::HighlightDetail, piece, &VToolSeamAllowance::Highlight);
        VAbstractPattern::AddTool(id, piece);
    }
    //Very important to delete it. Only this tool need this special variable.
    data->RemoveVariable(currentSeamAllowance);
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::Remove(bool ask)
{
    try
    {
        DeleteTool(ask);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InsertNode(VPieceNode node, quint32 pieceId, VMainGraphicsScene *scene,
                                    VContainer *data, VAbstractPattern *doc)
{
    SCASSERT(scene != nullptr)
    SCASSERT(data != nullptr)
    SCASSERT(doc != nullptr)

    if (pieceId > NULL_ID)
    {
        VPiece oldDet;
        try
        {
            oldDet = data->GetPiece(pieceId);
        }
        catch (const VExceptionBadId &)
        {
            return;
        }

        VPiece newDet = oldDet;

        const quint32 id = PrepareNode(node, scene, doc, data);
        if (id == NULL_ID)
        {
            return;
        }

        node.SetId(id);
        newDet.GetPath().Append(node);

        // Seam allowance tool already initializated and can't init the node
        VToolSeamAllowance *saTool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(pieceId));
        SCASSERT(saTool != nullptr);

        InitNode(node, scene, data, doc, saTool);

        SavePieceOptions *saveCommand = new SavePieceOptions(oldDet, newDet, doc, pieceId);
        qApp->getUndoStack()->push(saveCommand);// First push then make a connect
        data->UpdatePiece(pieceId, newDet);// Update piece because first save will not call lite update
        connect(saveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiece &piece)
{
    SCASSERT(doc != nullptr);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrName, piece.GetName());
    doc->SetAttribute(domElement, AttrVersion, QString().setNum(pieceVersion));
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(piece.GetMx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(piece.GetMy()));
    doc->SetAttribute(domElement, AttrInLayout, piece.IsInLayout());
    doc->SetAttribute(domElement, AttrForbidFlipping, piece.IsForbidFlipping());
    doc->SetAttribute(domElement, AttrSeamAllowance, piece.IsSeamAllowance());
    doc->SetAttribute(domElement, AttrHideMainPath, piece.IsHideMainPath());

    const bool saBuiltIn = piece.IsSeamAllowanceBuiltIn();
    if (saBuiltIn)
    {
        doc->SetAttribute(domElement, AttrSeamAllowanceBuiltIn, saBuiltIn);
    }
    else
    { // For backward compatebility.
        domElement.removeAttribute(AttrSeamAllowanceBuiltIn);
    }

    doc->SetAttribute(domElement, VAbstractPattern::AttrWidth, piece.GetFormulaSAWidth());
    doc->SetAttribute(domElement, AttrUnited, piece.IsUnited());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, const CustomSARecord &record)
{
    QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);

    doc->SetAttribute(recordNode, VAbstractPattern::AttrStart, record.startPoint);
    doc->SetAttribute(recordNode, VAbstractPattern::AttrPath, record.path);
    doc->SetAttribute(recordNode, VAbstractPattern::AttrEnd, record.endPoint);
    doc->SetAttribute(recordNode, VAbstractPattern::AttrNodeReverse, record.reverse);
    doc->SetAttribute(recordNode, VAbstractPattern::AttrIncludeAs, static_cast<unsigned int>(record.includeType));

    domElement.appendChild(recordNode);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddCSARecords(VAbstractPattern *doc, QDomElement &domElement,
                                       const QVector<CustomSARecord> &records)
{
    if (records.size() > 0)
    {
        QDomElement csaRecordsElement = doc->createElement(VToolSeamAllowance::TagCSA);
        for (int i = 0; i < records.size(); ++i)
        {
            AddCSARecord(doc, csaRecordsElement, records.at(i));
        }
        domElement.appendChild(csaRecordsElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddInternalPaths(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &paths)
{
    if (paths.size() > 0)
    {
        QDomElement iPathsElement = doc->createElement(VToolSeamAllowance::TagIPaths);
        for (int i = 0; i < paths.size(); ++i)
        {
            QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);
            doc->SetAttribute(recordNode, VAbstractPattern::AttrPath, paths.at(i));
            iPathsElement.appendChild(recordNode);
        }
        domElement.appendChild(iPathsElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPins(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &pins)
{
    if (pins.size() > 0)
    {
        QDomElement pinsElement = doc->createElement(VToolSeamAllowance::TagPins);
        for (int i = 0; i < pins.size(); ++i)
        {
            QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);
            recordNode.appendChild(doc->createTextNode(QString().setNum(pins.at(i))));
            pinsElement.appendChild(recordNode);
        }
        domElement.appendChild(pinsElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    QDomElement domData = doc->createElement(VAbstractPattern::TagData);
    const VPieceLabelData& data = piece.GetPatternPieceData();
    doc->SetAttribute(domData, VAbstractPattern::AttrLetter, data.GetLetter());
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, data.IsVisible() == true? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, data.GetPos().x());
    doc->SetAttribute(domData, AttrMy, data.GetPos().y());
    doc->SetAttribute(domData, VAbstractPattern::AttrWidth, data.GetLabelWidth());
    doc->SetAttribute(domData, AttrHeight, data.GetLabelHeight());
    doc->SetAttribute(domData, AttrFont, data.GetFontSize());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotation, data.GetRotation());

    if (data.CenterPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrCenterPin, data.CenterPin());
    }
    else
    {
        domData.removeAttribute(AttrCenterPin);
    }

    if (data.TopLeftPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrTopLeftPin, data.TopLeftPin());
    }
    else
    {
        domData.removeAttribute(AttrTopLeftPin);
    }

    if (data.BottomRightPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrBottomRightPin, data.BottomRightPin());
    }
    else
    {
        domData.removeAttribute(AttrBottomRightPin);
    }

    for (int i = 0; i < data.GetMCPCount(); ++i)
    {
        const MaterialCutPlacement mcp = data.GetMCP(i);
        QDomElement domMCP = doc->createElement(VAbstractPattern::TagMCP);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrMaterial, int(mcp.m_eMaterial));
        if (mcp.m_eMaterial == MaterialType::mtUserDefined)
        {
            doc->SetAttribute(domMCP, VAbstractPattern::AttrUserDefined, mcp.m_qsMaterialUserDef);
        }
        doc->SetAttribute(domMCP, VAbstractPattern::AttrCutNumber, mcp.m_iCutNumber);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrPlacement, int(mcp.m_ePlacement));
        domData.appendChild(domMCP);
    }
    domElement.appendChild(domData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    QDomElement domData = doc->createElement(VAbstractPattern::TagPatternInfo);
    const VPatternLabelData& geom = piece.GetPatternInfo();
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, geom.IsVisible() == true ? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, geom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, geom.GetPos().y());
    doc->SetAttribute(domData, VAbstractPattern::AttrWidth, geom.GetLabelWidth());
    doc->SetAttribute(domData, AttrHeight, geom.GetLabelHeight());
    doc->SetAttribute(domData, AttrFont, geom.GetFontSize());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotation, geom.GetRotation());

    if (geom.CenterPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrCenterPin, geom.CenterPin());
    }
    else
    {
        domData.removeAttribute(AttrCenterPin);
    }

    if (geom.TopLeftPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrTopLeftPin, geom.TopLeftPin());
    }
    else
    {
        domData.removeAttribute(AttrTopLeftPin);
    }

    if (geom.BottomRightPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrBottomRightPin, geom.BottomRightPin());
    }
    else
    {
        domData.removeAttribute(AttrBottomRightPin);
    }

    domElement.appendChild(domData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddGrainline(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    // grainline
    QDomElement domData = doc->createElement(VAbstractPattern::TagGrainline);
    const VGrainlineData& glGeom = piece.GetGrainlineGeometry();
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, glGeom.IsVisible() == true ? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, glGeom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, glGeom.GetPos().y());
    doc->SetAttribute(domData, AttrLength, glGeom.GetLength());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotation, glGeom.GetRotation());
    doc->SetAttribute(domData, VAbstractPattern::AttrArrows, int(glGeom.GetArrowType()));

    if (glGeom.CenterPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrCenterPin, glGeom.CenterPin());
    }
    else
    {
        domData.removeAttribute(AttrCenterPin);
    }

    if (glGeom.TopPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrTopPin, glGeom.TopPin());
    }
    else
    {
        domData.removeAttribute(AttrTopPin);
    }

    if (glGeom.BottomPin() > NULL_ID)
    {
        doc->SetAttribute(domData, AttrBottomPin, glGeom.BottomPin());
    }
    else
    {
        domData.removeAttribute(AttrBottomPin);
    }

    domElement.appendChild(domData);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolSeamAllowance::getTagName() const
{
    return VAbstractPattern::TagDetail;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object);
    Q_UNUSED(visible);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::FullUpdateFromFile()
{
    UpdateExcludeState();
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::EnableToolMove(bool move)
{
    setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AllowHover(bool enabled)
{
    setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AllowSelecting(bool enabled)
{
    setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ResetChildren(QGraphicsItem *pItem)
{
    const bool selected = isSelected();
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    VTextGraphicsItem* pVGI = qgraphicsitem_cast<VTextGraphicsItem*>(pItem);
    if (pVGI != m_dataLabel)
    {
        if (detail.GetPatternPieceData().IsVisible())
        {
            m_dataLabel->Reset();
        }
    }
    if (pVGI != m_patternInfo)
    {
        if (detail.GetPatternInfo().IsVisible())
        {
            m_patternInfo->Reset();
        }
    }
    VGrainlineItem* pGLI = qgraphicsitem_cast<VGrainlineItem*>(pItem);
    if (pGLI != m_grainLine)
    {
        if (detail.GetGrainlineGeometry().IsVisible())
        {
            m_grainLine->Reset();
        }
    }

    setSelected(selected);
    update();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::UpdateAll()
{
    m_sceneDetails->update();
    update();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::retranslateUi()
{
    UpdateDetailLabel();
    UpdatePatternInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::Highlight(quint32 id)
{
    setSelected(this->id == id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateLabel updates the text label, making it just big enough for the text to fit it
 */
void VToolSeamAllowance::UpdateDetailLabel()
{
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    const VPieceLabelData& labelData = detail.GetPatternPieceData();

    if (labelData.IsVisible() == true)
    {
        QPointF pos;
        qreal labelAngle = 0;

        if (PrepareLabelData(labelData, m_dataLabel, pos, labelAngle))
        {
            m_dataLabel->UpdateData(detail.GetName(), labelData);
            UpdateLabelItem(m_dataLabel, pos, labelAngle);
        }
    }
    else
    {
        m_dataLabel->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePatternInfo updates the pattern info label
 */
void VToolSeamAllowance::UpdatePatternInfo()
{
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    const VPatternLabelData& geom = detail.GetPatternInfo();

    if (geom.IsVisible() == true)
    {
        QPointF pos;
        qreal labelAngle = 0;

        if (PrepareLabelData(geom, m_patternInfo, pos, labelAngle))
        {
            m_patternInfo->UpdateData(doc, VContainer::size(), VContainer::height());
            UpdateLabelItem(m_patternInfo, pos, labelAngle);
        }
    }
    else
    {
        m_patternInfo->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail::UpdateGrainline updates the grain line item
 */
void VToolSeamAllowance::UpdateGrainline()
{
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    const VGrainlineData& geom = detail.GetGrainlineGeometry();

    if (geom.IsVisible() == true)
    {
        QPointF pos;
        qreal dRotation = 0;
        qreal dLength = 0;

        const VGrainlineItem::MoveTypes type = FindGrainlineGeometry(geom, dLength, dRotation, pos);
        if (type & VGrainlineItem::Error)
        {
            m_grainLine->hide();
            return;
        }

        m_grainLine->SetMoveType(type);
        m_grainLine->UpdateGeometry(pos, dRotation, ToPixel(dLength, *VDataTool::data.GetPatternUnit()),
                                    geom.GetArrowType());
        m_grainLine->show();
    }
    else
    {
        m_grainLine->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveMoveDetail saves the move detail operation to the undo stack
 */
void VToolSeamAllowance::SaveMoveDetail(const QPointF& ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(ptPos);

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    moveCommand->setText(tr("move pattern piece label"));
    connect(moveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveResizeDetail saves the resize detail label operation to the undo stack
 */
void VToolSeamAllowance::SaveResizeDetail(qreal dLabelW, int iFontSize)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;

    dLabelW = FromPixel(dLabelW, *VDataTool::data.GetPatternUnit());
    newDet.GetPatternPieceData().SetLabelWidth(QString().setNum(dLabelW));
    const qreal height = FromPixel(m_dataLabel->boundingRect().height(), *VDataTool::data.GetPatternUnit());
    newDet.GetPatternPieceData().SetLabelHeight(QString().setNum(height));
    newDet.GetPatternPieceData().SetFontSize(iFontSize);

    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    resizeCommand->setText(tr("resize pattern piece label"));
    connect(resizeCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveRotationDetail saves the rotation detail label operation to the undo stack
 */
void VToolSeamAllowance::SaveRotationDetail(qreal dRot)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(m_dataLabel->pos());
    newDet.GetPatternPieceData().SetFontSize(m_dataLabel->GetFontSize());

    // Tranform angle to anticlockwise
    QLineF line(0, 0, 100, 0);
    line.setAngle(-dRot);
    newDet.GetPatternPieceData().SetRotation(QString().setNum(line.angle()));

    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    rotateCommand->setText(tr("rotate pattern piece label"));
    connect(rotateCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveMovePattern saves the pattern label position
 */
void VToolSeamAllowance::SaveMovePattern(const QPointF &ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;
    newDet.GetPatternInfo().SetPos(ptPos);

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    moveCommand->setText(tr("move pattern info label"));
    connect(moveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief: SaveResizePattern saves the pattern label width and font size
 */
void VToolSeamAllowance::SaveResizePattern(qreal dLabelW, int iFontSize)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;

    dLabelW = FromPixel(dLabelW, *VDataTool::data.GetPatternUnit());
    newDet.GetPatternInfo().SetLabelWidth(QString().setNum(dLabelW));
    qreal height = FromPixel(m_patternInfo->boundingRect().height(), *VDataTool::data.GetPatternUnit());
    newDet.GetPatternInfo().SetLabelHeight(QString().setNum(height));
    newDet.GetPatternInfo().SetFontSize(iFontSize);

    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    resizeCommand->setText(tr("resize pattern info label"));
    connect(resizeCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveRotationPattern(qreal dRot)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;

    newDet.GetPatternInfo().SetPos(m_patternInfo->pos());
    newDet.GetPatternInfo().SetFontSize(m_patternInfo->GetFontSize());

    // Tranform angle to anticlockwise
    QLineF line(0, 0, 100, 0);
    line.setAngle(-dRot);
    newDet.GetPatternInfo().SetRotation(QString().setNum(line.angle()));

    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    rotateCommand->setText(tr("rotate pattern info label"));
    connect(rotateCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveMoveGrainline(const QPointF& ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;
    newDet.GetGrainlineGeometry().SetPos(ptPos);
    qDebug() << "******* new grainline pos" << ptPos;

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    moveCommand->setText(tr("move grainline"));
    connect(moveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveResizeGrainline(qreal dLength)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;

    dLength = FromPixel(dLength, *VDataTool::data.GetPatternUnit());
    newDet.GetGrainlineGeometry().SetPos(m_grainLine->pos());
    newDet.GetGrainlineGeometry().SetLength(QString().setNum(dLength));
    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    resizeCommand->setText(tr("resize grainline"));
    connect(resizeCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveRotateGrainline(qreal dRot, const QPointF& ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(id);
    VPiece newDet = oldDet;

    newDet.GetGrainlineGeometry().SetRotation(QString().setNum(qRadiansToDegrees(dRot)));
    newDet.GetGrainlineGeometry().SetPos(ptPos);
    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    rotateCommand->setText(tr("rotate grainline"));
    connect(rotateCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail::paint draws a bounding box around detail, if one of its text or grainline items is not idle.
 */
void VToolSeamAllowance::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen toolPen = pen();
    toolPen.setWidthF(ScaleWidth(widthHairLine, SceneScale(scene())));

    setPen(toolPen);
    m_seamAllowance->setPen(toolPen);

    if ((m_dataLabel->IsIdle() == false
            || m_patternInfo->IsIdle() == false
            || m_grainLine->IsIdle() == false) && not isSelected())
    {
        setSelected(true);
    }
    QGraphicsPathItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddToFile()
{
    const VPiece piece = VAbstractTool::data.GetPiece(id);

    QDomElement domElement = doc->createElement(getTagName());

    AddAttributes(doc, domElement, id, piece);
    AddPatternPieceData(doc, domElement, piece);
    AddPatternInfo(doc, domElement, piece);
    AddGrainline(doc, domElement, piece);

    // nodes
    AddNodes(doc, domElement, piece);
    //custom seam allowance
    AddCSARecords(doc, domElement, piece.GetCustomSARecords());
    AddInternalPaths(doc, domElement, piece.GetInternalPaths());
    AddPins(doc, domElement, piece.GetPins());

    AddPiece *addDet = new AddPiece(domElement, doc, piece, m_drawName);
    connect(addDet, &AddPiece::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addDet);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        // Refresh only parts that we possibly need to update
        {
            // TODO. Delete if minimal supported version is 0.4.0
            Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                              "Time to refactor the code.");

            const uint version = doc->GetParametrUInt(domElement, VToolSeamAllowance::AttrVersion, "1");
            if (version == 1)
            {
                const VPiece piece = VAbstractTool::data.GetPiece(id);

                doc->SetAttribute(domElement, AttrVersion, QString().setNum(pieceVersion));

                doc->RemoveAllChildren(domElement);//Very important to clear before rewrite
                AddPatternPieceData(doc, domElement, piece);
                AddPatternInfo(doc, domElement, piece);
                AddGrainline(doc, domElement, piece);
                AddNodes(doc, domElement, piece);
                AddCSARecords(doc, domElement, piece.GetCustomSARecords());
                AddInternalPaths(doc, domElement, piece.GetInternalPaths());
                AddPins(doc, domElement, piece.GetPins());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VToolSeamAllowance::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // Each time we move something we call recalculation scene rect. In some cases this can cause moving
        // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished)
        {
            changeFinished = false;

            // value - this is new position.
            const QPointF newPos = value.toPointF();

            MovePiece *moveDet = new MovePiece(doc, newPos.x(), newPos.y(), id, scene());
            connect(moveDet, &MovePiece::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
            qApp->getUndoStack()->push(moveDet);

            const QList<QGraphicsView *> viewList = scene()->views();
            if (not viewList.isEmpty())
            {
                if (QGraphicsView *view = viewList.at(0))
                {
                    const int xmargin = 50;
                    const int ymargin = 50;

                    const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                    const QRectF itemRect = mapToScene(boundingRect()|childrenBoundingRect()).boundingRect();

                    // If item's rect is bigger than view's rect ensureVisible works very unstable.
                    if (itemRect.height() + 2*ymargin < viewRect.height() &&
                        itemRect.width() + 2*xmargin < viewRect.width())
                    {
                        view->ensureVisible(itemRect, xmargin, ymargin);
                    }
                    else
                    {
                        // Ensure visible only small rect around a cursor
                        VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                        SCASSERT(currentScene);
                        const QPointF cursorPosition = currentScene->getScenePos();
                        view->ensureVisible(QRectF(cursorPosition.x()-5, cursorPosition.y()-5, 10, 10));
                    }
                }
            }
            // Don't forget to update geometry, because first change never call full parse
            VPiece detail = VAbstractTool::data.GetPiece(id);
            detail.SetMx(newPos.x());
            detail.SetMy(newPos.y());
            VAbstractTool::data.UpdatePiece(id, detail);

            RefreshGeometry();
            changeFinished = true;
        }
    }

    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsPathItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    QGraphicsPathItem::mousePressEvent(event);

    // Somehow clicking on notselectable object do not clean previous selections.
    if (not (flags() & ItemIsSelectable) && scene())
    {
        scene()->clearSelection();
    }

    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }

    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        doc->SelectedDetail(id);
        emit ChoosedTool(id, SceneObject::Detail);
    }

    event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //Disable cursor-arrow-closehand
        RestoreOverrideCursor(cursorArrowCloseHand);
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
    QGraphicsPathItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //Disable cursor-arrow-openhand
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
    QGraphicsPathItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (m_suppressContextMenu)
    {
        return;
    }

    QMenu menu;
    QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

    QAction *inLayoutOption = menu.addAction(tr("In layout"));
    inLayoutOption->setCheckable(true);
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    inLayoutOption->setChecked(detail.IsInLayout());

    QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    _referens > 1 ? actionRemove->setEnabled(false) : actionRemove->setEnabled(true);

    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        QSharedPointer<DialogSeamAllowance> dialog =
                QSharedPointer<DialogSeamAllowance>(new DialogSeamAllowance(getData(), id, qApp->getMainWindow()));
        dialog->EnableApply(true);
        m_dialog = dialog;
        m_dialog->setModal(true);
        connect(m_dialog.data(), &DialogTool::DialogClosed, this, &VToolSeamAllowance::FullUpdateFromGuiOk);
        connect(m_dialog.data(), &DialogTool::DialogApplied, this, &VToolSeamAllowance::FullUpdateFromGuiApply);
        SetDialog();
        m_dialog->show();
    }
    else if (selectedAction == inLayoutOption)
    {
        TogglePieceInLayout *togglePrint = new TogglePieceInLayout(id, selectedAction->isChecked(),
                                                                   &(VAbstractTool::data), doc);
        connect(togglePrint, &TogglePieceInLayout::UpdateList, doc, &VAbstractPattern::CheckInLayoutList);
        qApp->getUndoStack()->push(togglePrint);
    }
    else if (selectedAction == actionRemove)
    {
        try
        {
            DeleteTool();
        }
        catch(const VExceptionToolWasDeleted &e)
        {
            Q_UNUSED(e);
            return;//Leave this method immediately!!!
        }
        return; //Leave this method immediately after call!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            try
            {
                DeleteTool();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e);
                return;//Leave this method immediately!!!
            }
            break;
        default:
            break;
    }

    QGraphicsPathItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SetDialog()
{
    SCASSERT(not m_dialog.isNull());
    QSharedPointer<DialogSeamAllowance> dialogTool = m_dialog.objectCast<DialogSeamAllowance>();
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetPiece(VAbstractTool::data.GetPiece(id));
    dialogTool->EnableApply(true);
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance::VToolSeamAllowance(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const Source &typeCreation, VMainGraphicsScene *scene,
                                       const QString &drawName, QGraphicsItem *parent)
    : VInteractiveTool(doc, data, id),
      QGraphicsPathItem(parent),
      m_sceneDetails(scene),
      m_drawName(drawName),
      m_seamAllowance(new VNoBrushScalePathItem(this)),
      m_dataLabel(new VTextGraphicsItem(this)),
      m_patternInfo(new VTextGraphicsItem(this)),
      m_grainLine(new VGrainlineItem(this))
{
    VPiece detail = data->GetPiece(id);
    InitNodes(detail, scene);
    InitCSAPaths(detail);
    InitInternalPaths(detail);
    InitPins(detail);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();

    m_seamAllowance->setBrush(QBrush(Qt::Dense7Pattern));

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    connect(scene, &VMainGraphicsScene::EnableToolMove, this, &VToolSeamAllowance::EnableToolMove);
    connect(scene, &VMainGraphicsScene::ItemClicked, this, &VToolSeamAllowance::ResetChildren);
    ToolCreation(typeCreation);
    setAcceptHoverEvents(true);

    connect(m_dataLabel, &VTextGraphicsItem::SignalMoved, this, &VToolSeamAllowance::SaveMoveDetail);
    connect(m_dataLabel, &VTextGraphicsItem::SignalResized, this, &VToolSeamAllowance::SaveResizeDetail);
    connect(m_dataLabel, &VTextGraphicsItem::SignalRotated, this, &VToolSeamAllowance::SaveRotationDetail);

    connect(m_patternInfo, &VTextGraphicsItem::SignalMoved, this, &VToolSeamAllowance::SaveMovePattern);
    connect(m_patternInfo, &VTextGraphicsItem::SignalResized, this, &VToolSeamAllowance::SaveResizePattern);
    connect(m_patternInfo, &VTextGraphicsItem::SignalRotated, this, &VToolSeamAllowance::SaveRotationPattern);

    connect(m_grainLine, &VGrainlineItem::SignalMoved, this, &VToolSeamAllowance::SaveMoveGrainline);
    connect(m_grainLine, &VGrainlineItem::SignalResized, this, &VToolSeamAllowance::SaveResizeGrainline);
    connect(m_grainLine, &VGrainlineItem::SignalRotated, this, &VToolSeamAllowance::SaveRotateGrainline);

    connect(doc, &VAbstractPattern::patternChanged, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdateDetailLabel);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdateGrainline);

    connect(m_sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolSeamAllowance::UpdateDetailLabel);
    connect(m_sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(m_sceneDetails, &VMainGraphicsScene::LanguageChanged, this, &VToolSeamAllowance::retranslateUi);

    UpdateDetailLabel();
    UpdatePatternInfo();
    UpdateGrainline();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::UpdateExcludeState()
{
    const VPiece detail = VAbstractTool::data.GetPiece(id);
    for (int i = 0; i< detail.GetPath().CountNodes(); ++i)
    {
        const VPieceNode &node = detail.GetPath().at(i);
        if (node.GetTypeTool() == Tool::NodePoint)
        {
            VNodePoint *tool = qobject_cast<VNodePoint*>(VAbstractPattern::getTool(node.GetId()));
            SCASSERT(tool != nullptr);

            tool->SetExluded(node.IsExcluded());
            tool->setVisible(not node.IsExcluded());//Hide excluded point
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RefreshGeometry()
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    const VPiece detail = VAbstractTool::data.GetPiece(id);
    QPainterPath path;

    if (not detail.IsHideMainPath() || not detail.IsSeamAllowance() || detail.IsSeamAllowanceBuiltIn())
    {
        m_seamAllowance->setBrush(QBrush(Qt::Dense7Pattern));
        path = detail.MainPathPath(this->getData());
    }
    else
    {
        m_seamAllowance->setBrush(QBrush(Qt::NoBrush)); // Disable if the main path was hidden
    }

    {
        QPainterPath mainPath = path;
        mainPath.addPath(detail.PassmarksPath(this->getData()));
        this->setPath(mainPath);
    }

    this->setPos(detail.GetMx(), detail.GetMy());

    if (detail.IsSeamAllowance() && not detail.IsSeamAllowanceBuiltIn())
    {
        path.addPath(detail.SeamAllowancePath(this->getData()));
        path.setFillRule(Qt::OddEvenFill);
        m_seamAllowance->setPath(path);
    }
    else
    {
        m_seamAllowance->setPath(QPainterPath());
    }

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveDialogChange()
{
    SCASSERT(not m_dialog.isNull());
    DialogSeamAllowance *dialogTool = qobject_cast<DialogSeamAllowance*>(m_dialog.data());
    SCASSERT(dialogTool != nullptr);
    const VPiece newDet = dialogTool->GetPiece();
    const VPiece oldDet = VAbstractTool::data.GetPiece(id);

    SavePieceOptions *saveCommand = new SavePieceOptions(oldDet, newDet, doc, id);
    connect(saveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(saveCommand);
    UpdateDetailLabel();
}

//---------------------------------------------------------------------------------------------------------------------
VPieceItem::MoveTypes VToolSeamAllowance::FindLabelGeometry(const VPatternLabelData& labelData, qreal &rotationAngle,
                                                            qreal &labelWidth, qreal &labelHeight, QPointF &pos)
{

    VPieceItem::MoveTypes restrictions = VPieceItem::AllModifications;
    try
    {
        if (not qmu::QmuTokenParser::IsSingle(labelData.GetRotation()))
        {
            restrictions &= ~ VPieceItem::IsRotatable;
        }

        Calculator cal1;
        rotationAngle = cal1.EvalFormula(VAbstractTool::data.PlainVariables(), labelData.GetRotation());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return VPieceItem::Error;
    }

    const quint32 topLeftPin = labelData.TopLeftPin();
    const quint32 bottomRightPin = labelData.BottomRightPin();

    if (topLeftPin != NULL_ID && bottomRightPin != NULL_ID)
    {
        try
        {
            const auto topLeftPinPoint = VAbstractTool::data.GeometricObject<VPointF>(topLeftPin);
            const auto bottomRightPinPoint = VAbstractTool::data.GeometricObject<VPointF>(bottomRightPin);

            const QRectF labelRect = QRectF(static_cast<QPointF>(*topLeftPinPoint),
                                            static_cast<QPointF>(*bottomRightPinPoint));
            labelWidth = FromPixel(qAbs(labelRect.width()), *VDataTool::data.GetPatternUnit());
            labelHeight = FromPixel(qAbs(labelRect.height()), *VDataTool::data.GetPatternUnit());

            pos = labelRect.topLeft();

            restrictions &= ~ VPieceItem::IsMovable;
            restrictions &= ~ VPieceItem::IsResizable;

            return restrictions;
        }
        catch(const VExceptionBadId &)
        {
            // do nothing.
        }
    }

    try
    {
        const bool widthIsSingle = qmu::QmuTokenParser::IsSingle(labelData.GetLabelWidth());

        Calculator cal1;
        labelWidth = cal1.EvalFormula(VAbstractTool::data.PlainVariables(), labelData.GetLabelWidth());

        const bool heightIsSingle = qmu::QmuTokenParser::IsSingle(labelData.GetLabelHeight());

        Calculator cal2;
        labelHeight = cal2.EvalFormula(VAbstractTool::data.PlainVariables(), labelData.GetLabelHeight());

        if (not widthIsSingle || not heightIsSingle)
        {
            restrictions &= ~ VPieceItem::IsResizable;
        }
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return VPieceItem::Error;
    }

    const quint32 centerPin = labelData.CenterPin();
    if (centerPin != NULL_ID)
    {
        try
        {
            const auto centerPinPoint = VAbstractTool::data.GeometricObject<VPointF>(centerPin);

            const qreal lWidth = ToPixel(labelWidth, *VDataTool::data.GetPatternUnit());
            const qreal lHeight = ToPixel(labelHeight, *VDataTool::data.GetPatternUnit());

            pos = static_cast<QPointF>(*centerPinPoint) - QRectF(0, 0, lWidth, lHeight).center();
            restrictions &= ~ VPieceItem::IsMovable;
        }
        catch(const VExceptionBadId &)
        {
            pos = labelData.GetPos();
        }
    }
    else
    {
        pos = labelData.GetPos();
    }

    return restrictions;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceItem::MoveTypes VToolSeamAllowance::FindGrainlineGeometry(const VGrainlineData& geom, qreal &length,
                                                               qreal &rotationAngle, QPointF &pos)
{
    const quint32 topPin = geom.TopPin();
    const quint32 bottomPin = geom.BottomPin();

    if (topPin != NULL_ID && bottomPin != NULL_ID)
    {
        try
        {
            const auto topPinPoint = VAbstractTool::data.GeometricObject<VPointF>(topPin);
            const auto bottomPinPoint = VAbstractTool::data.GeometricObject<VPointF>(bottomPin);

            QLineF grainline(static_cast<QPointF>(*bottomPinPoint), static_cast<QPointF>(*topPinPoint));
            length = FromPixel(grainline.length(), *VDataTool::data.GetPatternUnit());
            rotationAngle = grainline.angle();

            if (not VFuzzyComparePossibleNulls(rotationAngle, 0))
            {
                grainline.setAngle(0);
            }

            pos = grainline.p1();

            return VPieceItem::NotMovable;
        }
        catch(const VExceptionBadId &)
        {
            // do nothing.
        }
    }

    VPieceItem::MoveTypes restrictions = VPieceItem::AllModifications;
    try
    {
        if (not qmu::QmuTokenParser::IsSingle(geom.GetRotation()))
        {
            restrictions &= ~ VPieceItem::IsRotatable;
        }

        Calculator cal1;
        rotationAngle = cal1.EvalFormula(VAbstractTool::data.PlainVariables(), geom.GetRotation());

        if (not qmu::QmuTokenParser::IsSingle(geom.GetLength()))
        {
            restrictions &= ~ VPieceItem::IsResizable;
        }

        Calculator cal2;
        length = cal2.EvalFormula(VAbstractTool::data.PlainVariables(), geom.GetLength());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return VPieceItem::Error;
    }

    const quint32 centerPin = geom.CenterPin();
    if (centerPin != NULL_ID)
    {
        try
        {
            const auto centerPinPoint = VAbstractTool::data.GeometricObject<VPointF>(centerPin);

            const qreal cLength = ToPixel(length, *VDataTool::data.GetPatternUnit());
            QLineF grainline(centerPinPoint->x(), centerPinPoint->y(),
                             centerPinPoint->x() + cLength / 2.0, centerPinPoint->y());

            grainline.setAngle(rotationAngle);
            grainline = QLineF(grainline.p2(), grainline.p1());
            grainline.setLength(cLength);

            pos = grainline.p2();
            restrictions &= ~ VPieceItem::IsMovable;
        }
        catch(const VExceptionBadId &)
        {
            pos = geom.GetPos();
        }
    }
    else
    {
        pos = geom.GetPos();
    }

    return restrictions;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitNodes(const VPiece &detail, VMainGraphicsScene *scene)
{
    for (int i = 0; i< detail.GetPath().CountNodes(); ++i)
    {
        InitNode(detail.GetPath().at(i), scene, &(VAbstractTool::data), doc, this);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitNode(const VPieceNode &node, VMainGraphicsScene *scene, VContainer *data,
                                  VAbstractPattern *doc, VToolSeamAllowance *parent)
{
    SCASSERT(scene != nullptr)
    SCASSERT(data != nullptr)
    SCASSERT(doc != nullptr)
    SCASSERT(parent != nullptr)

    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
        {
            VNodePoint *tool = qobject_cast<VNodePoint*>(VAbstractPattern::getTool(node.GetId()));
            SCASSERT(tool != nullptr);

            connect(tool, &VNodePoint::ShowContextMenu, parent, &VToolSeamAllowance::contextMenuEvent);
            connect(tool, &VNodePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(parent);
            tool->SetParentType(ParentType::Item);
            tool->SetExluded(node.IsExcluded());
            tool->setVisible(not node.IsExcluded());//Hide excluded point
            doc->IncrementReferens(node.GetId());
            break;
        }
        case (Tool::NodeArc):
        case (Tool::NodeElArc):
        case (Tool::NodeSpline):
        case (Tool::NodeSplinePath):
            doc->IncrementReferens(data->GetGObject(node.GetId())->getIdTool());
            break;
        default:
            qDebug()<<"Get wrong tool type. Ignore.";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitCSAPaths(const VPiece &detail)
{
    for (int i = 0; i < detail.GetCustomSARecords().size(); ++i)
    {
        doc->IncrementReferens(detail.GetCustomSARecords().at(i).path);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitInternalPaths(const VPiece &detail)
{
    for (int i = 0; i < detail.GetInternalPaths().size(); ++i)
    {
        auto *tool = qobject_cast<VToolPiecePath*>(VAbstractPattern::getTool(detail.GetInternalPaths().at(i)));
        SCASSERT(tool != nullptr);
        tool->setParentItem(this);
        tool->SetParentType(ParentType::Item);
        tool->show();
        doc->IncrementReferens(detail.GetInternalPaths().at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitPins(const VPiece &detail)
{
    for (int i = 0; i < detail.GetPins().size(); ++i)
    {
        doc->IncrementReferens(detail.GetPins().at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DeleteTool(bool ask)
{
    QScopedPointer<DeletePiece> delDet(new DeletePiece(doc, id, VAbstractTool::data.GetPiece(id)));
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::No)
        {
            return;
        }
        /* If UnionDetails tool delete detail no need emit FullParsing.*/
        connect(delDet.data(), &DeletePiece::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    }

    // If UnionDetails tool delete the detail this object will be deleted only after full parse.
    // Deleting inside UnionDetails cause crash.
    // Because this object should be inactive from no one we disconnect all signals that may cause a crash
    // KEEP THIS LIST ACTUALL!!!
    disconnect(doc, nullptr, this, nullptr);
    if (QGraphicsScene *toolScene = scene())
    {
        disconnect(toolScene, nullptr, this, nullptr);
    }
    disconnect(m_dataLabel, nullptr, this, nullptr);
    disconnect(m_patternInfo, nullptr, this, nullptr);
    disconnect(m_grainLine, nullptr, this, nullptr);
    disconnect(m_sceneDetails, nullptr, this, nullptr);

    hide();// User shouldn't see this object

    qApp->getUndoStack()->push(delDet.take());

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e("Tool was used after deleting.");
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToolCreation(const Source &typeCreation)
{
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        AddToFile();
        if (typeCreation != Source::FromTool)
        {
            qApp->getUndoStack()->endMacro();
        }
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolSeamAllowance::PrepareLabelData(const VPatternLabelData &labelData, VTextGraphicsItem *labelItem,
                                          QPointF &pos, qreal &labelAngle)
{
    SCASSERT(labelItem != nullptr)

    qreal labelWidth = 0;
    qreal labelHeight = 0;
    const VTextGraphicsItem::MoveTypes type = FindLabelGeometry(labelData, labelAngle, labelWidth, labelHeight, pos);
    if (type & VGrainlineItem::Error)
    {
        labelItem->hide();
        return false;
    }
    labelItem->SetMoveType(type);

    QFont fnt = qApp->Settings()->GetLabelFont();
    {
        const int iFS = labelData.GetFontSize();
        iFS < MIN_FONT_SIZE ? fnt.setPixelSize(MIN_FONT_SIZE) : fnt.setPixelSize(iFS);
    }
    labelItem->SetFont(fnt);
    labelItem->SetSize(ToPixel(labelWidth, *VDataTool::data.GetPatternUnit()),
                       ToPixel(labelHeight, *VDataTool::data.GetPatternUnit()));

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::UpdateLabelItem(VTextGraphicsItem *labelItem, QPointF pos, qreal labelAngle)
{
    SCASSERT(labelItem != nullptr)

    QRectF rectBB;
    rectBB.setTopLeft(pos);
    rectBB.setWidth(labelItem->boundingRect().width());
    rectBB.setHeight(labelItem->boundingRect().height());
    qreal dX;
    qreal dY;
    if (labelItem->IsContained(rectBB, labelAngle, dX, dY) == false)
    {
        pos.setX(pos.x() + dX);
        pos.setY(pos.y() + dY);
    }

    labelItem->setPos(pos);
    labelItem->setRotation(-labelAngle);// expects clockwise direction
    labelItem->Update();
    labelItem->GetTextLines() > 0 ? labelItem->show() : labelItem->hide();
}
