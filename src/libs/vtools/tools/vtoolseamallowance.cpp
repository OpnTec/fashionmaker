/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../dialogs/tools/piece/dialogduplicatedetail.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "nodeDetails/nodedetails.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vplacelabelitem.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../undocommands/addpiece.h"
#include "../undocommands/deletepiece.h"
#include "../undocommands/movepiece.h"
#include "../undocommands/savepieceoptions.h"
#include "../undocommands/togglepiecestate.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vwidgets/vnobrushscalepathitem.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../qmuparser/qmutokenparser.h"

#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>

// Current version of seam allowance tag need for backward compatibility
const quint8 VToolSeamAllowance::pieceVersion = 2;

const QString VToolSeamAllowance::TagCSA         = QStringLiteral("csa");
const QString VToolSeamAllowance::TagRecord      = QStringLiteral("record");
const QString VToolSeamAllowance::TagIPaths      = QStringLiteral("iPaths");
const QString VToolSeamAllowance::TagPins        = QStringLiteral("pins");
const QString VToolSeamAllowance::TagPlaceLabels = QStringLiteral("placeLabels");

const QString VToolSeamAllowance::AttrSeamAllowance        = QStringLiteral("seamAllowance");
const QString VToolSeamAllowance::AttrHideMainPath         = QStringLiteral("hideMainPath");
const QString VToolSeamAllowance::AttrSeamAllowanceBuiltIn = QStringLiteral("seamAllowanceBuiltIn");
const QString VToolSeamAllowance::AttrUnited               = QStringLiteral("united");
const QString VToolSeamAllowance::AttrFont                 = QStringLiteral("fontSize");
const QString VToolSeamAllowance::AttrTopLeftPin           = QStringLiteral("topLeftPin");
const QString VToolSeamAllowance::AttrBottomRightPin       = QStringLiteral("bottomRightPin");
const QString VToolSeamAllowance::AttrCenterPin            = QStringLiteral("centerPin");
const QString VToolSeamAllowance::AttrTopPin               = QStringLiteral("topPin");
const QString VToolSeamAllowance::AttrBottomPin            = QStringLiteral("bottomPin");

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull());
    const QPointer<DialogSeamAllowance> dialogTool = qobject_cast<DialogSeamAllowance*>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolSeamAllowanceInitData initData;
    initData.detail = dialogTool->GetPiece();
    initData.width = initData.detail.GetFormulaSAWidth();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    initData.detail.GetPath().SetNodes(PrepareNodes(initData.detail.GetPath(), scene, doc, data));

    VToolSeamAllowance *piece = Create(initData);

    if (piece != nullptr)
    {
        piece->m_dialog = dialog;
        piece->RefreshGeometry(true); // Refresh internal paths
    }
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(VToolSeamAllowanceInitData &initData)
{
    if (initData.typeCreation == Source::FromGui || initData.typeCreation == Source::FromTool)
    {
        auto currentSA = new VIncrement(initData.data, currentSeamAllowance);
        currentSA->SetFormula(initData.detail.GetSAWidth(), initData.width, true);
        currentSA->SetDescription(tr("Current seam allowance"));

        initData.data->AddVariable(currentSeamAllowance, currentSA);
        initData.id = initData.data->AddPiece(initData.detail);
    }
    else
    {
        const qreal calcWidth = CheckFormula(initData.id, initData.width, initData.data);
        initData.detail.SetFormulaSAWidth(initData.width, calcWidth);

        auto currentSA = new VIncrement(initData.data, currentSeamAllowance);
        currentSA->SetFormula(calcWidth, initData.width, true);
        currentSA->SetDescription(tr("Current seam allowance"));

        initData.data->AddVariable(currentSeamAllowance, currentSA);

        initData.data->UpdatePiece(initData.id, initData.detail);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    VToolSeamAllowance *piece = nullptr;
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::Piece, initData.doc);
        piece = new VToolSeamAllowance(initData);
        initData.scene->addItem(piece);
        VMainGraphicsView::NewSceneRect(initData.scene, qApp->getSceneView(), piece);
        VAbstractPattern::AddTool(initData.id, piece);
    }
    //Very important to delete it. Only this tool need this special variable.
    initData.data->RemoveVariable(currentSeamAllowance);
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Duplicate(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                                  VAbstractPattern *doc)
{
    SCASSERT(not dialog.isNull());
    const QPointer<DialogDuplicateDetail> dialogTool = qobject_cast<DialogDuplicateDetail*>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolSeamAllowanceInitData initData;
    initData.scene = scene;
    initData.doc = doc;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;
    initData.drawName = doc->PieceDrawName(dialogTool->Duplicate());

    VContainer toolData = VAbstractPattern::getTool(dialogTool->Duplicate())->getData();
    initData.data = &toolData;

    VPiece detail = initData.data->GetPiece(dialogTool->Duplicate());
    detail.SetMx(dialogTool->MoveDuplicateX());
    detail.SetMy(dialogTool->MoveDuplicateY());
    initData.detail = detail;
    initData.width = initData.detail.GetFormulaSAWidth();

    VToolSeamAllowance *piece = Duplicate(initData);
    if (piece != nullptr)
    {
        piece->RefreshGeometry(true); // Refresh internal paths
    }
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Duplicate(VToolSeamAllowanceInitData &initData)
{
    VPiece dupDetail = initData.detail;

    QMap<quint32, quint32> replacements;
    dupDetail.GetPath().SetNodes(DuplicateNodes(initData.detail.GetPath(), initData, replacements));
    dupDetail.SetCustomSARecords(DuplicateCustomSARecords(initData.detail.GetCustomSARecords(), initData,
                                                          replacements));
    dupDetail.SetInternalPaths(DuplicateInternalPaths(initData.detail.GetInternalPaths(), initData));
    dupDetail.SetPins(DuplicatePins(initData.detail.GetPins(), initData));
    dupDetail.SetPlaceLabels(DuplicatePlaceLabels(initData.detail.GetPlaceLabels(), initData));

    initData.detail = dupDetail;
    return VToolSeamAllowance::Create(initData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RemoveWithConfirm(bool ask)
{
    try
    {
        DeleteToolWithConfirm(ask);
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

        qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, pieceId));
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
    doc->SetAttribute(domElement, AttrForceFlipping, piece.IsForceFlipping());
    doc->SetAttribute(domElement, AttrSeamAllowance, piece.IsSeamAllowance());
    doc->SetAttribute(domElement, AttrHideMainPath, piece.IsHideMainPath());
    doc->SetAttributeOrRemoveIf(domElement, AttrSeamAllowanceBuiltIn, piece.IsSeamAllowanceBuiltIn(), false);
    doc->SetAttribute(domElement, AttrWidth, piece.GetFormulaSAWidth());
    doc->SetAttribute(domElement, AttrUnited, piece.IsUnited());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, CustomSARecord record)
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
        for (auto record : records)
        {
            AddCSARecord(doc, csaRecordsElement, record);
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
        for (auto path : paths)
        {
            QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);
            doc->SetAttribute(recordNode, VAbstractPattern::AttrPath, path);
            iPathsElement.appendChild(recordNode);
        }
        domElement.appendChild(iPathsElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPins(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &pins)
{
    AddPointRecords(doc, domElement, pins, VToolSeamAllowance::TagPins);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPlaceLabels(VAbstractPattern *doc, QDomElement &domElement,
                                        const QVector<quint32> &placeLabels)
{
    AddPointRecords(doc, domElement, placeLabels, VToolSeamAllowance::TagPlaceLabels);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    QDomElement domData = doc->createElement(VAbstractPattern::TagData);
    const VPieceLabelData& data = piece.GetPatternPieceData();
    doc->SetAttribute(domData, VAbstractPattern::AttrLetter, data.GetLetter());
    doc->SetAttribute(domData, VAbstractPattern::AttrAnnotation, data.GetAnnotation());
    doc->SetAttribute(domData, VAbstractPattern::AttrOrientation, data.GetOrientation());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotationWay, data.GetRotationWay());
    doc->SetAttribute(domData, VAbstractPattern::AttrTilt, data.GetTilt());
    doc->SetAttribute(domData, VAbstractPattern::AttrFoldPosition, data.GetFoldPosition());
    doc->SetAttribute(domData, VAbstractPattern::AttrQuantity, data.GetQuantity());
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, data.IsVisible());
    doc->SetAttribute(domData, VAbstractPattern::AttrOnFold, data.IsOnFold());
    doc->SetAttribute(domData, AttrMx, data.GetPos().x());
    doc->SetAttribute(domData, AttrMy, data.GetPos().y());
    doc->SetAttribute(domData, AttrWidth, data.GetLabelWidth());
    doc->SetAttribute(domData, AttrHeight, data.GetLabelHeight());
    doc->SetAttribute(domData, AttrFont, data.GetFontSize());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotation, data.GetRotation());
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrCenterPin, data.CenterPin(), not (data.CenterPin() > NULL_ID));
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrTopLeftPin, data.TopLeftPin(), not (data.TopLeftPin() > NULL_ID));
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrBottomRightPin, data.BottomRightPin(),
                                      not (data.BottomRightPin() > NULL_ID));
    doc->SetLabelTemplate(domData, data.GetLabelTemplate());

    domElement.appendChild(domData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    QDomElement domData = doc->createElement(VAbstractPattern::TagPatternInfo);
    const VPatternLabelData& geom = piece.GetPatternInfo();
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, geom.IsVisible());
    doc->SetAttribute(domData, AttrMx, geom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, geom.GetPos().y());
    doc->SetAttribute(domData, AttrWidth, geom.GetLabelWidth());
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
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, glGeom.IsVisible());
    doc->SetAttribute(domData, AttrMx, glGeom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, glGeom.GetPos().y());
    doc->SetAttribute(domData, AttrLength, glGeom.GetLength());
    doc->SetAttribute(domData, VAbstractPattern::AttrRotation, glGeom.GetRotation());
    doc->SetAttribute(domData, VAbstractPattern::AttrArrows, int(glGeom.GetArrowType()));
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrCenterPin, glGeom.CenterPin(), not (glGeom.CenterPin() > NULL_ID));
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrTopPin, glGeom.TopPin(), not (glGeom.TopPin() > NULL_ID));
    doc->SetAttributeOrRemoveIf<bool>(domData, AttrBottomPin, glGeom.BottomPin(), not (glGeom.BottomPin() > NULL_ID));

    domElement.appendChild(domData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::Move(qreal x, qreal y)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    VPiece detail = VAbstractTool::data.GetPiece(m_id);
    detail.SetMx(x);
    detail.SetMy(y);
    VAbstractTool::data.UpdatePiece(m_id, detail);

    setPos(x, y);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::Update(const VPiece &piece)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    VAbstractTool::data.UpdatePiece(m_id, piece);
    RefreshGeometry();
    VMainGraphicsView::NewSceneRect(m_sceneDetails, qApp->getSceneView(), this);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DisconnectOutsideSignals()
{
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
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ConnectOutsideSignals()
{
    connect(m_dataLabel, &VTextGraphicsItem::SignalMoved, this, &VToolSeamAllowance::SaveMoveDetail);
    connect(m_dataLabel, &VTextGraphicsItem::SignalResized, this, &VToolSeamAllowance::SaveResizeDetail);
    connect(m_dataLabel, &VTextGraphicsItem::SignalRotated, this, &VToolSeamAllowance::SaveRotationDetail);

    connect(m_patternInfo, &VTextGraphicsItem::SignalMoved, this, &VToolSeamAllowance::SaveMovePattern);
    connect(m_patternInfo, &VTextGraphicsItem::SignalResized, this, &VToolSeamAllowance::SaveResizePattern);
    connect(m_patternInfo, &VTextGraphicsItem::SignalRotated, this, &VToolSeamAllowance::SaveRotationPattern);

    connect(m_grainLine, &VGrainlineItem::SignalMoved, this, &VToolSeamAllowance::SaveMoveGrainline);
    connect(m_grainLine, &VGrainlineItem::SignalResized, this, &VToolSeamAllowance::SaveResizeGrainline);
    connect(m_grainLine, &VGrainlineItem::SignalRotated, this, &VToolSeamAllowance::SaveRotateGrainline);

    connect(doc, &VAbstractPattern::UpdatePatternLabel, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::UpdatePatternLabel, this, &VToolSeamAllowance::UpdateDetailLabel);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdateDetailLabel);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolSeamAllowance::UpdateGrainline);

    connect(m_sceneDetails, &VMainGraphicsScene::EnableToolMove, this, &VToolSeamAllowance::EnableToolMove);
    connect(m_sceneDetails, &VMainGraphicsScene::ItemClicked, this, &VToolSeamAllowance::ResetChildren);
    connect(m_sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolSeamAllowance::UpdateDetailLabel);
    connect(m_sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolSeamAllowance::UpdatePatternInfo);
    connect(m_sceneDetails, &VMainGraphicsScene::LanguageChanged, this, &VToolSeamAllowance::retranslateUi);
    connect(m_sceneDetails, &VMainGraphicsScene::EnableDetailItemHover, this, &VToolSeamAllowance::AllowHover);
    connect(m_sceneDetails, &VMainGraphicsScene::EnableDetailItemSelection, this, &VToolSeamAllowance::AllowSelecting);
    connect(m_sceneDetails, &VMainGraphicsScene::HighlightDetail, this, &VToolSeamAllowance::Highlight);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ReinitInternals(const VPiece &detail, VMainGraphicsScene *scene)
{
    InitNodes(detail, scene);
    InitCSAPaths(detail);
    InitInternalPaths(detail);
    InitSpecialPoints(detail.GetPins());
    InitSpecialPoints(detail.GetPlaceLabels());
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
    const bool updateChildren = false;// Chilren have their own signals. Avoid double refresh.
    RefreshGeometry(updateChildren);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::EnableToolMove(bool move)
{
    setFlag(QGraphicsItem::ItemIsMovable, move);

    m_dataLabel->setFlag(QGraphicsItem::ItemIsMovable, move);
    m_patternInfo->setFlag(QGraphicsItem::ItemIsMovable, move);
    m_grainLine->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AllowHover(bool enabled)
{
    // Manually handle hover events. Need for setting cursor for not selectable paths.
    m_acceptHoverEvents = enabled;
    m_dataLabel->setAcceptHoverEvents(enabled);
    m_patternInfo->setAcceptHoverEvents(enabled);
    m_grainLine->setAcceptHoverEvents(enabled);
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
    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
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
    setSelected(m_id == id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateLabel updates the text label, making it just big enough for the text to fit it
 */
void VToolSeamAllowance::UpdateDetailLabel()
{
    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
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
    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
    const VPatternLabelData& geom = detail.GetPatternInfo();

    if (geom.IsVisible() == true)
    {
        QPointF pos;
        qreal labelAngle = 0;

        if (PrepareLabelData(geom, m_patternInfo, pos, labelAngle))
        {
            m_patternInfo->UpdateData(doc);
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
    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
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
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(ptPos);

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    moveCommand->setText(tr("move pattern piece label"));
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveResizeDetail saves the resize detail label operation to the undo stack
 */
void VToolSeamAllowance::SaveResizeDetail(qreal dLabelW, int iFontSize)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    dLabelW = FromPixel(dLabelW, *VDataTool::data.GetPatternUnit());
    newDet.GetPatternPieceData().SetLabelWidth(QString().setNum(dLabelW));
    const qreal height = FromPixel(m_dataLabel->boundingRect().height(), *VDataTool::data.GetPatternUnit());
    newDet.GetPatternPieceData().SetLabelHeight(QString().setNum(height));
    newDet.GetPatternPieceData().SetFontSize(iFontSize);

    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    resizeCommand->setText(tr("resize pattern piece label"));
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveRotationDetail saves the rotation detail label operation to the undo stack
 */
void VToolSeamAllowance::SaveRotationDetail(qreal dRot)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(m_dataLabel->pos());
    newDet.GetPatternPieceData().SetFontSize(m_dataLabel->GetFontSize());

    // Tranform angle to anticlockwise
    QLineF line(0, 0, 100, 0);
    line.setAngle(-dRot);
    newDet.GetPatternPieceData().SetRotation(QString().setNum(line.angle()));

    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    rotateCommand->setText(tr("rotate pattern piece label"));
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveMovePattern saves the pattern label position
 */
void VToolSeamAllowance::SaveMovePattern(const QPointF &ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;
    newDet.GetPatternInfo().SetPos(ptPos);

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    moveCommand->setText(tr("move pattern info label"));
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief: SaveResizePattern saves the pattern label width and font size
 */
void VToolSeamAllowance::SaveResizePattern(qreal dLabelW, int iFontSize)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    dLabelW = FromPixel(dLabelW, *VDataTool::data.GetPatternUnit());
    newDet.GetPatternInfo().SetLabelWidth(QString().setNum(dLabelW));
    qreal height = FromPixel(m_patternInfo->boundingRect().height(), *VDataTool::data.GetPatternUnit());
    newDet.GetPatternInfo().SetLabelHeight(QString().setNum(height));
    newDet.GetPatternInfo().SetFontSize(iFontSize);

    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    resizeCommand->setText(tr("resize pattern info label"));
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveRotationPattern(qreal dRot)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    newDet.GetPatternInfo().SetPos(m_patternInfo->pos());
    newDet.GetPatternInfo().SetFontSize(m_patternInfo->GetFontSize());

    // Tranform angle to anticlockwise
    QLineF line(0, 0, 100, 0);
    line.setAngle(-dRot);
    newDet.GetPatternInfo().SetRotation(QString().setNum(line.angle()));

    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    rotateCommand->setText(tr("rotate pattern info label"));
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveMoveGrainline(const QPointF& ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;
    newDet.GetGrainlineGeometry().SetPos(ptPos);
    qDebug() << "******* new grainline pos" << ptPos;

    SavePieceOptions* moveCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    moveCommand->setText(tr("move grainline"));
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveResizeGrainline(qreal dLength)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    dLength = FromPixel(dLength, *VDataTool::data.GetPatternUnit());
    newDet.GetGrainlineGeometry().SetPos(m_grainLine->pos());
    newDet.GetGrainlineGeometry().SetLength(QString().setNum(dLength));

    SavePieceOptions* resizeCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    resizeCommand->setText(tr("resize grainline"));
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveRotateGrainline(qreal dRot, const QPointF& ptPos)
{
    VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    newDet.GetGrainlineGeometry().SetRotation(QString().setNum(qRadiansToDegrees(dRot)));
    newDet.GetGrainlineGeometry().SetPos(ptPos);

    SavePieceOptions* rotateCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    rotateCommand->setText(tr("rotate grainline"));
    qApp->getUndoStack()->push(rotateCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail::paint draws a bounding box around detail, if one of its text or grainline items is not idle.
 */
void VToolSeamAllowance::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen toolPen = pen();
    toolPen.setWidthF(ScaleWidth(qApp->Settings()->WidthHairLine(), SceneScale(scene())));

    setPen(toolPen);
    m_seamAllowance->setPen(toolPen);
    m_passmarks->setPen(toolPen);
    m_placeLabels->setPen(toolPen);

    if ((m_dataLabel->IsIdle() == false
            || m_patternInfo->IsIdle() == false
            || m_grainLine->IsIdle() == false) && not isSelected())
    {
        setSelected(true);
    }
    PaintWithFixItemHighlightSelected<QGraphicsPathItem>(this, painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VToolSeamAllowance::boundingRect() const
{
    if (m_mainPathRect.isNull())
    {
        return QGraphicsPathItem::boundingRect();
    }
    else
    {
        return m_mainPathRect;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VToolSeamAllowance::shape() const
{
    if (m_mainPath == QPainterPath())
    {
        return QGraphicsPathItem::shape();
    }
    else
    {
        return ItemShapeFromPath(m_mainPath, pen());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::FullUpdateFromGuiApply()
{
    SaveDialogChange(tr("apply save detail options"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddToFile()
{
    const QDomElement duplicate = doc->elementById(m_id);
    if (not duplicate.isNull())
    {
        throw VExceptionWrongId(tr("This id (%1) is not unique.").arg(m_id), duplicate);
    }

    const VPiece piece = VAbstractTool::data.GetPiece(m_id);

    QDomElement domElement = doc->createElement(getTagName());

    AddAttributes(doc, domElement, m_id, piece);
    AddPatternPieceData(doc, domElement, piece);
    AddPatternInfo(doc, domElement, piece);
    AddGrainline(doc, domElement, piece);

    // nodes
    AddNodes(doc, domElement, piece);
    //custom seam allowance
    AddCSARecords(doc, domElement, piece.GetCustomSARecords());
    AddInternalPaths(doc, domElement, piece.GetInternalPaths());
    AddPins(doc, domElement, piece.GetPins());
    AddPlaceLabels(doc, domElement, piece.GetPlaceLabels());

    qApp->getUndoStack()->push(new AddPiece(domElement, doc, VAbstractTool::data, m_sceneDetails, m_drawName));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(m_id, getTagName());
    if (domElement.isElement())
    {
        // Refresh only parts that we possibly need to update
        {
            // TODO. Delete if minimal supported version is 0.4.0
            Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < FORMAT_VERSION(0, 4, 0),
                              "Time to refactor the code.");

            const uint version = doc->GetParametrUInt(domElement, AttrVersion, QChar('1'));
            if (version == 1)
            {
                const VPiece piece = VAbstractTool::data.GetPiece(m_id);

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

            qApp->getUndoStack()->push(new MovePiece(doc, newPos.x(), newPos.y(), m_id, scene()));

            const QList<QGraphicsView *> viewList = scene()->views();
            if (not viewList.isEmpty())
            {
                if (VMainGraphicsView *view = qobject_cast<VMainGraphicsView *>(viewList.at(0)))
                {
                    const qreal scale = SceneScale(scene());
                    const int xmargin = qCeil(50/scale);
                    const int ymargin = qCeil(50/scale);

                    const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                    const QRectF itemRect = mapToScene(boundingRect()|childrenBoundingRect()).boundingRect();

                    // If item's rect is bigger than view's rect ensureVisible works very unstable.
                    if (itemRect.height() + 2*ymargin < viewRect.height() &&
                        itemRect.width() + 2*xmargin < viewRect.width())
                    {
                        view->EnsureVisibleWithDelay(itemRect, VMainGraphicsView::scrollDelay, xmargin, ymargin);
                    }
                    else
                    {
                        // Ensure visible only small rect around a cursor
                        VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                        SCASSERT(currentScene);
                        const QPointF cursorPosition = currentScene->getScenePos();
                        view->EnsureVisibleWithDelay(QRectF(cursorPosition.x()-5/scale, cursorPosition.y()-5/scale,
                                                            10/scale, 10/scale),
                                                     VMainGraphicsView::scrollDelay);
                    }
                }
            }

            changeFinished = true;
        }
    }

    if (change == QGraphicsItem::ItemSelectedHasChanged)
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
            SetItemOverrideCursor(this, cursorArrowCloseHand, 1, 1);
        }
    }

    if (selectionType == SelectionType::ByMouseRelease)
    {
        event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
    }
    else
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            doc->SelectedDetail(m_id);
            emit ChoosedTool(m_id, SceneObject::Detail);
            event->accept();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (flags() & QGraphicsItem::ItemIsMovable))
    {
        SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
    }

    if (selectionType == SelectionType::ByMouseRelease)
    {
        if (IsSelectedByReleaseEvent(this, event))
        {
            doc->SelectedDetail(m_id);
            emit ChoosedTool(m_id, SceneObject::Detail);
        }
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_acceptHoverEvents)
    {
        if (flags() & QGraphicsItem::ItemIsMovable)
        {
            SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
        }
        else
        {
            setCursor(qApp->getSceneView()->viewport()->cursor());
        }
        QGraphicsPathItem::hoverEnterEvent(event);
    }
    else
    {
        setCursor(qApp->getSceneView()->viewport()->cursor());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_acceptHoverEvents)
    {
        QGraphicsPathItem::hoverLeaveEvent(event);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (m_suppressContextMenu)
    {
        return;
    }

    QMenu menu;
    QAction *actionOption = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-other")), tr("Options"));

    const VPiece detail = VAbstractTool::data.GetPiece(m_id);

    QAction *inLayoutOption = menu.addAction(tr("In layout"));
    inLayoutOption->setCheckable(true);
    inLayoutOption->setChecked(detail.IsInLayout());

    QAction *forbidFlippingOption = menu.addAction(tr("Forbid flipping"));
    forbidFlippingOption->setCheckable(true);
    forbidFlippingOption->setChecked(detail.IsForbidFlipping());

    QAction *forceFlippingOption = menu.addAction(tr("Force flipping"));
    forceFlippingOption->setCheckable(true);
    forceFlippingOption->setChecked(detail.IsForceFlipping());

    QAction *actionRemove = menu.addAction(QIcon::fromTheme(QStringLiteral("edit-delete")), tr("Delete"));
    _referens > 1 ? actionRemove->setEnabled(false) : actionRemove->setEnabled(true);

    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        ShowOptions();
    }
    else if (selectedAction == inLayoutOption)
    {
        ToggleInLayout(selectedAction->isChecked());
    }
    else if (selectedAction == forbidFlippingOption)
    {
        ToggleForbidFlipping(selectedAction->isChecked());
    }
    else if (selectedAction == forceFlippingOption)
    {
        ToggleForceFlipping(selectedAction->isChecked());
    }
    else if (selectedAction == actionRemove)
    {
        try
        {
            DeleteFromMenu();
        }
        catch(const VExceptionToolWasDeleted &e)
        {
            Q_UNUSED(e);
            return;//Leave this method immediately!!!
        }
        //Leave this method immediately after call!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            if (ConfirmDeletion() == QMessageBox::Yes)
            {
                const QList<VToolSeamAllowance *> toolList = SelectedTools();

                try
                {
                    if (not toolList.isEmpty())
                    {
                        qApp->getUndoStack()->beginMacro(tr("multi deletion"));

                        for(auto tool : toolList)
                        {
                            tool->RemoveWithConfirm(false);
                        }
                    }
                    DeleteToolWithConfirm(false);
                }
                catch(const VExceptionToolWasDeleted &e)
                {
                    Q_UNUSED(e);
                    if (not toolList.isEmpty())
                    {
                        qApp->getUndoStack()->endMacro();
                    }
                    return;//Leave this method immediately!!!
                }
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
    const QPointer<DialogSeamAllowance> dialogTool = qobject_cast<DialogSeamAllowance *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetPiece(VAbstractTool::data.GetPiece(m_id));
    dialogTool->EnableApply(true);
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance::VToolSeamAllowance(const VToolSeamAllowanceInitData &initData, QGraphicsItem *parent)
    : VInteractiveTool(initData.doc, initData.data, initData.id),
      QGraphicsPathItem(parent),
      m_mainPath(),
      m_mainPathRect(),
      m_sceneDetails(initData.scene),
      m_drawName(initData.drawName),
      m_seamAllowance(new VNoBrushScalePathItem(this)),
      m_dataLabel(new VTextGraphicsItem(this)),
      m_patternInfo(new VTextGraphicsItem(this)),
      m_grainLine(new VGrainlineItem(this)),
      m_passmarks(new QGraphicsPathItem(this)),
      m_placeLabels(new QGraphicsPathItem(this)),
      m_acceptHoverEvents(true)
{
    VPiece detail = initData.data->GetPiece(initData.id);
    ReinitInternals(detail, m_sceneDetails);
    AllowSelecting(true);
    EnableToolMove(true);
    AllowHover(true);

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    ToolCreation(initData.typeCreation);
    setAcceptHoverEvents(m_acceptHoverEvents);

    connect(this, &VToolSeamAllowance::ChoosedTool, m_sceneDetails, &VMainGraphicsScene::ChoosedItem);
    connect(m_sceneDetails, &VMainGraphicsScene::EnableToolMove, this, &VToolSeamAllowance::EnableToolMove);
    connect(m_sceneDetails, &VMainGraphicsScene::ItemSelection, this, &VToolSeamAllowance::ToolSelectionType);

    ConnectOutsideSignals();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::UpdateExcludeState()
{
    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
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
void VToolSeamAllowance::UpdateInternalPaths()
{
    const QVector<quint32> paths = VAbstractTool::data.GetPiece(m_id).GetInternalPaths();
    for (auto path : paths)
    {
        try
        {
            if (auto *tool = qobject_cast<VToolPiecePath*>(VAbstractPattern::getTool(path)))
            {
                tool->RefreshGeometry();
            }
        }
        catch(const VExceptionBadId &)
        {
            // ignore
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RefreshGeometry(bool updateChildren)
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    const VPiece detail = VAbstractTool::data.GetPiece(m_id);
    QFuture<QPainterPath > futurePath = QtConcurrent::run(detail,
                                                          QOverload<const VContainer *>::of(&VPiece::MainPathPath),
                                                          this->getData());
    QFuture<QPainterPath > futurePassmarks = QtConcurrent::run(detail, &VPiece::PassmarksPath, this->getData());

    QFuture<QVector<QPointF> > futureSeamAllowance;

    if (detail.IsSeamAllowance())
    {
        futureSeamAllowance = QtConcurrent::run(detail, &VPiece::SeamAllowancePoints, this->getData());
    }

    this->setPos(detail.GetMx(), detail.GetMy());

    QPainterPath path;

    if (not detail.IsHideMainPath() || not detail.IsSeamAllowance() || detail.IsSeamAllowanceBuiltIn())
    {
        m_mainPath = QPainterPath();
        m_mainPathRect = QRectF();
        m_seamAllowance->setBrush(QBrush(Qt::Dense7Pattern));
        path = futurePath.result();
    }
    else
    {
        m_seamAllowance->setBrush(QBrush(Qt::NoBrush)); // Disable if the main path was hidden
        // need for returning a bounding rect when main path is not visible
        m_mainPath = futurePath.result();
        m_mainPathRect = m_mainPath.controlPointRect();
        path = QPainterPath();
    }

    this->setPath(path);

    m_placeLabels->setPath(detail.PlaceLabelPath(this->getData()));

    if (detail.IsSeamAllowance() && not detail.IsSeamAllowanceBuiltIn())
    {
        path.addPath(detail.SeamAllowancePath(futureSeamAllowance.result()));
        path.setFillRule(Qt::OddEvenFill);
        m_seamAllowance->setPath(path);
    }
    else
    {
        m_seamAllowance->setPath(QPainterPath());
    }

    UpdateDetailLabel();
    UpdatePatternInfo();
    UpdateGrainline();
    UpdateExcludeState();
    if (updateChildren)
    {
        UpdateInternalPaths();
    }

    m_passmarks->setPath(futurePassmarks.result());

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SaveDialogChange(const QString &undoText)
{
    SCASSERT(not m_dialog.isNull());
    DialogSeamAllowance *dialogTool = qobject_cast<DialogSeamAllowance*>(m_dialog.data());
    SCASSERT(dialogTool != nullptr);
    const VPiece newDet = dialogTool->GetPiece();
    const VPiece oldDet = VAbstractTool::data.GetPiece(m_id);

    QVector<QPointer<VUndoCommand>> &undocommands = dialogTool->UndoStack();
    const bool groupChange = not undocommands.isEmpty();

    SavePieceOptions *saveCommand = new SavePieceOptions(oldDet, newDet, doc, m_id);
    if (VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow()))
    { // Better not to crash here, just silently do not update list.
        connect(saveCommand, &SavePieceOptions::UpdateGroups, window, &VAbstractMainWindow::UpdateDetailsList);
    }

    if (groupChange)
    {
        qApp->getUndoStack()->beginMacro(undoText.isEmpty() ? saveCommand->text(): undoText);

        for (auto command : undocommands)
        {
            qApp->getUndoStack()->push(command);
            command.clear(); // To prevent double free memory
        }
        undocommands.clear();
    }

    qApp->getUndoStack()->push(saveCommand);

    if (groupChange)
    {
        qApp->getUndoStack()->endMacro();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ShowOptions()
{
    QPointer<DialogSeamAllowance> dialog = new DialogSeamAllowance(getData(), doc, m_id, qApp->getMainWindow());
    dialog->EnableApply(true);
    m_dialog = dialog;
    m_dialog->setModal(true);
    connect(m_dialog.data(), &DialogTool::DialogClosed, this, &VToolSeamAllowance::FullUpdateFromGuiOk);
    connect(m_dialog.data(), &DialogTool::DialogApplied, this, &VToolSeamAllowance::FullUpdateFromGuiApply);
    SetDialog();
    m_dialog->show();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleInLayout(bool checked)
{
    TogglePieceInLayout *togglePrint = new TogglePieceInLayout(m_id, checked, &(VAbstractTool::data), doc);
    connect(togglePrint, &TogglePieceInLayout::Toggled, doc, &VAbstractPattern::CheckInLayoutList);
    qApp->getUndoStack()->push(togglePrint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleForbidFlipping(bool checked)
{
    qApp->getUndoStack()->push(new TogglePieceForceForbidFlipping(m_id, checked,
                                                                  ForceForbidFlippingType::ForbidFlipping,
                                                                  &(VAbstractTool::data), doc));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleForceFlipping(bool checked)
{
    qApp->getUndoStack()->push(new TogglePieceForceForbidFlipping(m_id, checked, ForceForbidFlippingType::ForceFlipping,
                                                                  &(VAbstractTool::data), doc));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DeleteFromMenu()
{
    DeleteToolWithConfirm();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleExcludeState(quint32 id)
{
    const VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    for (int i = 0; i< oldDet.GetPath().CountNodes(); ++i)
    {
        VPieceNode node = oldDet.GetPath().at(i);
        if (node.GetId() == id && node.GetTypeTool() == Tool::NodePoint)
        {
            node.SetExcluded(not node.IsExcluded());
            newDet.GetPath()[i] = node;

            qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, m_id));
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleNodePointAngleType(quint32 id, PieceNodeAngle type)
{
    const VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    for (int i = 0; i< oldDet.GetPath().CountNodes(); ++i)
    {
        VPieceNode node = oldDet.GetPath().at(i);
        if (node.GetId() == id && node.GetTypeTool() == Tool::NodePoint)
        {
            node.SetAngleType(type);
            newDet.GetPath()[i] = node;

            qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, m_id));
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToggleNodePointPassmark(quint32 id, bool toggle)
{
    const VPiece oldDet = VAbstractTool::data.GetPiece(m_id);
    VPiece newDet = oldDet;

    for (int i = 0; i< oldDet.GetPath().CountNodes(); ++i)
    {
        VPieceNode node = oldDet.GetPath().at(i);
        if (node.GetId() == id && node.GetTypeTool() == Tool::NodePoint)
        {
            node.SetPassmark(toggle);
            newDet.GetPath()[i] = node;

            qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, m_id));
            return;
        }
    }
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
        rotationAngle = cal1.EvalFormula(VAbstractTool::data.DataVariables(), labelData.GetRotation());
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
        labelWidth = cal1.EvalFormula(VAbstractTool::data.DataVariables(), labelData.GetLabelWidth());

        const bool heightIsSingle = qmu::QmuTokenParser::IsSingle(labelData.GetLabelHeight());

        Calculator cal2;
        labelHeight = cal2.EvalFormula(VAbstractTool::data.DataVariables(), labelData.GetLabelHeight());

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
        rotationAngle = cal1.EvalFormula(VAbstractTool::data.DataVariables(), geom.GetRotation());

        if (not qmu::QmuTokenParser::IsSingle(geom.GetLength()))
        {
            restrictions &= ~ VPieceItem::IsResizable;
        }

        Calculator cal2;
        length = cal2.EvalFormula(VAbstractTool::data.DataVariables(), geom.GetLength());
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

            if (tool->parent() != parent)
            {
                connect(tool, &VNodePoint::ShowOptions, parent, &VToolSeamAllowance::ShowOptions, Qt::UniqueConnection);
                connect(tool, &VNodePoint::ToggleInLayout, parent, &VToolSeamAllowance::ToggleInLayout,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::ToggleForbidFlipping, parent, &VToolSeamAllowance::ToggleForbidFlipping,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::ToggleForceFlipping, parent, &VToolSeamAllowance::ToggleForceFlipping,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::Delete, parent, &VToolSeamAllowance::DeleteFromMenu, Qt::UniqueConnection);
                connect(tool, &VNodePoint::ToggleExcludeState, parent, &VToolSeamAllowance::ToggleExcludeState,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::ToggleAngleType, parent, &VToolSeamAllowance::ToggleNodePointAngleType,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::TogglePassmark, parent, &VToolSeamAllowance::ToggleNodePointPassmark,
                        Qt::UniqueConnection);
                connect(tool, &VNodePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem, Qt::UniqueConnection);
                tool->setParentItem(parent);
                tool->SetParentType(ParentType::Item);
                tool->SetExluded(node.IsExcluded());
                doc->IncrementReferens(node.GetId());
            }
            tool->setVisible(not node.IsExcluded());//Hide excluded point
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
void VToolSeamAllowance::InitCSAPaths(const VPiece &detail) const
{
    const QVector<CustomSARecord> records = detail.GetCustomSARecords();
    for (auto record : records)
    {
        doc->IncrementReferens(record.path);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitInternalPaths(const VPiece &detail)
{
    const QVector<quint32> paths = detail.GetInternalPaths();
    for (auto path : paths)
    {
        auto *tool = qobject_cast<VToolPiecePath*>(VAbstractPattern::getTool(path));
        SCASSERT(tool != nullptr);

        if (tool->parent() != this)
        {
            tool->setParentItem(this);
            tool->SetParentType(ParentType::Item);
        }
        tool->show();
        doc->IncrementReferens(path);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitSpecialPoints(const QVector<quint32> &points) const
{
    for (auto point : points)
    {
        doc->IncrementReferens(point);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DeleteToolWithConfirm(bool ask)
{
    QScopedPointer<DeletePiece> delDet(new DeletePiece(doc, m_id, VAbstractTool::data, m_sceneDetails));
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::No)
        {
            return;
        }
    }

    qApp->getUndoStack()->push(delDet.take());

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e(tr("Tool was used after deleting."));
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::ToolCreation(const Source &typeCreation)
{
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        AddToFile();
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

//---------------------------------------------------------------------------------------------------------------------
QList<VToolSeamAllowance *> VToolSeamAllowance::SelectedTools() const
{
    QList<VToolSeamAllowance *> tools;
    if(m_sceneDetails)
    {
        const QList<QGraphicsItem *> list = m_sceneDetails->selectedItems();

        if (not list.isEmpty())
        {
            for(auto item : list)
            {
                VToolSeamAllowance *tool = qgraphicsitem_cast<VToolSeamAllowance *>(item);
                if (tool != nullptr && tool->getId() != m_id)
                {
                    tools.append(tool);
                }
            }
        }
    }

    return tools;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPointRecords(VAbstractPattern *doc, QDomElement &domElement,
                                         const QVector<quint32> &records, const QString &tag)
{
    if (records.size() > 0)
    {
        QDomElement pinsElement = doc->createElement(tag);
        for (auto record : records)
        {
            QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);
            recordNode.appendChild(doc->createTextNode(QString().setNum(record)));
            pinsElement.appendChild(recordNode);
        }
        domElement.appendChild(pinsElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolSeamAllowance::DuplicateNode(const VPieceNode &node, const VToolSeamAllowanceInitData &initData)
{
    SCASSERT(initData.scene != nullptr)
    SCASSERT(initData.doc != nullptr)
    SCASSERT(initData.data != nullptr)

    const QSharedPointer<VGObject> gobj = initData.data->GetGObject(node.GetId());
    VAbstractNode *tool = qobject_cast<VAbstractNode *>(VAbstractPattern::getTool(node.GetId()));
    SCASSERT(tool != nullptr)

    VAbstractNodeInitData initNodeData;
    initNodeData.idObject = gobj->getIdObject();
    initNodeData.doc = initData.doc;
    initNodeData.data = initData.data;
    initNodeData.parse = Document::FullParse;
    initNodeData.typeCreation = Source::FromGui;
    initNodeData.scene = initData.scene;
    initNodeData.drawName = initData.drawName;
    initNodeData.idTool = tool->GetIdTool();

    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
        {
            const QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(gobj);
            initNodeData.id = VAbstractTool::CreateNodePoint(initData.data, gobj->getIdObject(), point);
            VNodePoint::Create(initNodeData);
            break;
        }
        case (Tool::NodeArc):
            initNodeData.id = VAbstractTool::CreateNode<VArc>(initData.data, gobj->getIdObject());
            VNodeArc::Create(initNodeData);
            break;
        case (Tool::NodeElArc):
            initNodeData.id = VAbstractTool::CreateNode<VEllipticalArc>(initData.data, gobj->getIdObject());
            VNodeEllipticalArc::Create(initNodeData);
            break;
        case (Tool::NodeSpline):
            initNodeData.id = VAbstractTool::CreateNodeSpline(initData.data, gobj->getIdObject());
            VNodeSpline::Create(initNodeData);
            break;
        case (Tool::NodeSplinePath):
            initNodeData.id = VAbstractTool::CreateNodeSplinePath(initData.data, gobj->getIdObject());
            VNodeSplinePath::Create(initNodeData);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    return initNodeData.id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolSeamAllowance::DuplicatePiecePath(quint32 id, const VToolSeamAllowanceInitData &initData)
{
    const VPiecePath path = initData.data->GetPiecePath(id);
    VPiecePath newPath = path;
    QMap<quint32, quint32> recordReplacements; // Not used
    newPath.SetNodes(DuplicateNodes(path, initData, recordReplacements));

    const quint32 idPath = initData.data->AddPiecePath(newPath);

    VAbstractNode *tool = qobject_cast<VAbstractNode *>(VAbstractPattern::getTool(id));
    SCASSERT(tool != nullptr)

    VToolPiecePathInitData initNodeData;
    initNodeData.id = idPath;
    initNodeData.idObject = NULL_ID; // piece id
    initNodeData.scene = initData.scene;
    initNodeData.doc = initData.doc;
    initNodeData.data = initData.data;
    initNodeData.parse = Document::FullParse;
    initNodeData.typeCreation = Source::FromTool;
    initNodeData.drawName = initData.drawName;
    initNodeData.idTool = tool->GetIdTool();
    initNodeData.path = newPath;

    VToolPiecePath::Create(initNodeData);
    return idPath;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VToolSeamAllowance::DuplicateCustomSARecords(const QVector<CustomSARecord> &records,
                                                                     const VToolSeamAllowanceInitData &initData,
                                                                     const QMap<quint32, quint32> &replacements)
{
    QVector<CustomSARecord> newRecords;
    for(auto record : records)
    {
        record.path = DuplicatePiecePath(record.path, initData);
        record.startPoint = replacements.value(record.startPoint, NULL_ID);
        record.endPoint = replacements.value(record.endPoint, NULL_ID);
        newRecords.append(record);
    }
    return newRecords;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolSeamAllowance::DuplicateInternalPaths(const QVector<quint32> &iPaths,
                                                            const VToolSeamAllowanceInitData &initData)
{
    QVector<quint32> newPaths;
    for(auto iPath : iPaths)
    {
        newPaths.append(DuplicatePiecePath(iPath, initData));
    }
    return newPaths;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolSeamAllowance::DuplicatePins(const QVector<quint32> &pins,
                                                   const VToolSeamAllowanceInitData &initData)
{
    QVector<quint32> newPins;
    for(auto p : pins)
    {
        QSharedPointer<VPointF> pin = initData.data->GeometricObject<VPointF>(p);

        VAbstractNode *tool = qobject_cast<VAbstractNode *>(VAbstractPattern::getTool(p));
        SCASSERT(tool != nullptr)

        VToolPinInitData initNodeData;
        initNodeData.id = initData.data->AddGObject(new VPointF(*pin));
        initNodeData.pointId = pin->getIdObject();
        initNodeData.idObject = NULL_ID; // piece id
        initNodeData.doc = initData.doc;
        initNodeData.data = initData.data;
        initNodeData.parse = Document::FullParse;
        initNodeData.typeCreation = Source::FromTool;
        initNodeData.drawName = initData.drawName;
        initNodeData.idTool = tool->GetIdTool();

        VToolPin::Create(initNodeData);
        newPins.append(initNodeData.id);
    }
    return newPins;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolSeamAllowance::DuplicatePlaceLabels(const QVector<quint32> &placeLabels,
                                                          const VToolSeamAllowanceInitData &initData)
{
    QVector<quint32> newPlaceLabels;
    for(auto placeLabel : placeLabels)
    {
        QSharedPointer<VPlaceLabelItem> label = initData.data->GeometricObject<VPlaceLabelItem>(placeLabel);
        VAbstractNode *tool = qobject_cast<VAbstractNode *>(VAbstractPattern::getTool(placeLabel));
        SCASSERT(tool != nullptr)

        VToolPlaceLabelInitData initNodeData;
        initNodeData.idObject = NULL_ID; // piece id
        initNodeData.doc = initData.doc;
        initNodeData.data = initData.data;
        initNodeData.parse = Document::FullParse;
        initNodeData.typeCreation = Source::FromTool;
        initNodeData.drawName = initData.drawName;
        initNodeData.width = label->GetWidthFormula();
        initNodeData.height = label->GetHeightFormula();
        initNodeData.angle = label->GetAngleFormula();
        initNodeData.visibilityTrigger = label->GetVisibilityTrigger();
        initNodeData.type = label->GetLabelType();
        initNodeData.centerPoint = label->GetCenterPoint();
        initNodeData.id = initNodeData.data->AddGObject(new VPlaceLabelItem(*label));
        initNodeData.idTool = tool->GetIdTool();

        VToolPlaceLabel::Create(initNodeData);
        newPlaceLabels.append(initNodeData.id);
    }
    return newPlaceLabels;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VToolSeamAllowance::DuplicateNodes(const VPiecePath &path,
                                                       const VToolSeamAllowanceInitData &initData,
                                                       QMap<quint32, quint32> &replacements)
{
    QVector<VPieceNode> nodes;
    for (int i = 0; i< path.CountNodes(); ++i)
    {
        VPieceNode nodeD = path.at(i);
        const quint32 oldId = nodeD.GetId();
        const quint32 id = DuplicateNode(nodeD, initData);
        if (id > NULL_ID)
        {
            nodeD.SetId(id);
            nodes.append(nodeD);
            replacements.insert(oldId, id);
        }
    }
    return nodes;
}
