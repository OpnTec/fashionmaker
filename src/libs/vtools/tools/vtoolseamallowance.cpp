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
#include "../dialogs/tools/dialogseamallowance.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "nodeDetails/vnodearc.h"
#include "nodeDetails/vnodepoint.h"
#include "nodeDetails/vnodespline.h"
#include "nodeDetails/vnodesplinepath.h"
#include "../vgeometry/varc.h"
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

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>

// Current version of seam allowance tag nned for backward compatibility
const quint8 VToolSeamAllowance::pieceVersion = 2;

const QString VToolSeamAllowance::TagCSA     = QStringLiteral("csa");
const QString VToolSeamAllowance::TagRecord  = QStringLiteral("record");

const QString VToolSeamAllowance::AttrVersion        = QStringLiteral("version");
const QString VToolSeamAllowance::AttrForbidFlipping = QStringLiteral("forbidFlipping");
const QString VToolSeamAllowance::AttrSeamAllowance  = QStringLiteral("seamAllowance");
const QString VToolSeamAllowance::AttrWidth          = QStringLiteral("width");
const QString VToolSeamAllowance::AttrUnited         = QStringLiteral("united");
const QString VToolSeamAllowance::AttrStart          = QStringLiteral("start");
const QString VToolSeamAllowance::AttrPath           = QStringLiteral("path");
const QString VToolSeamAllowance::AttrEnd            = QStringLiteral("end");
const QString VToolSeamAllowance::AttrIncludeAs      = QStringLiteral("includeAs");

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance::~VToolSeamAllowance()
{
    delete m_dialog;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogSeamAllowance *dialogTool = qobject_cast<DialogSeamAllowance*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VPiece detail = dialogTool->GetPiece();
    QVector<VPieceNode> nodes;
    qApp->getUndoStack()->beginMacro("add detail");
    for (int i = 0; i< detail.GetPath().CountNodes(); ++i)
    {
        quint32 id = 0;
        VPieceNode nodeD = detail.GetPath().at(i);
        switch (nodeD.GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                id = CreateNode<VPointF>(data, nodeD.GetId());
                VNodePoint::Create(doc, data, scene, id, nodeD.GetId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeArc):
            {
                id = CreateNode<VArc>(data, nodeD.GetId());
                VNodeArc::Create(doc, data, id, nodeD.GetId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSpline):
            {
                const auto obj = data->GetGObject(nodeD.GetId());
                if (obj->getType() == GOType::Spline)
                {
                    id = CreateNode<VSpline>(data, nodeD.GetId());
                }
                else
                {
                    id = CreateNode<VCubicBezier>(data, nodeD.GetId());
                }
                VNodeSpline::Create(doc, data, id, nodeD.GetId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSplinePath):
            {
                const auto obj = data->GetGObject(nodeD.GetId());
                if (obj->getType() == GOType::SplinePath)
                {
                    id = CreateNode<VSplinePath>(data, nodeD.GetId());
                }
                else
                {
                    id = CreateNode<VCubicBezierPath>(data, nodeD.GetId());
                }
                VNodeSplinePath::Create(doc, data, id, nodeD.GetId(), Document::FullParse, Source::FromGui);
            }
            break;
            default:
                qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        nodeD.SetId(id);
        nodes.append(nodeD);
    }

    detail.GetPath().SetNodes(nodes);
    VToolSeamAllowance *piece = Create(0, detail, scene, doc, data, Document::FullParse, Source::FromGui);

    if (piece != nullptr)
    {
        piece->m_dialog = dialogTool;
    }
    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance *VToolSeamAllowance::Create(quint32 id, const VPiece &newPiece, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data, const Document &parse,
                                               const Source &typeCreation, const QString &drawName)
{
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        id = data->AddPiece(newPiece);
    }
    else
    {
        data->UpdatePiece(id, newPiece);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::Piece, doc);
    if (parse == Document::FullParse)
    {
        VToolSeamAllowance *piece = new VToolSeamAllowance(doc, data, id, typeCreation, scene, drawName);
        scene->addItem(piece);
        connect(piece, &VToolSeamAllowance::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::EnableDetailItemHover, piece, &VToolSeamAllowance::AllowHover);
        connect(scene, &VMainGraphicsScene::EnableDetailItemSelection, piece, &VToolSeamAllowance::AllowSelecting);
        connect(scene, &VMainGraphicsScene::HighlightDetail, piece, &VToolSeamAllowance::Highlight);
        doc->AddTool(id, piece);

        return piece;
    }
    return nullptr;
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
void VToolSeamAllowance::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VPieceNode &node)
{
    domElement.appendChild(AddSANode(doc, VAbstractPattern::TagNode, node));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    if (piece.GetPath().CountNodes() > 0)
    {
        QDomElement nodesElement = doc->createElement(VAbstractPattern::TagNodes);
        for (int i = 0; i < piece.GetPath().CountNodes(); ++i)
        {
            AddNode(doc, nodesElement, piece.GetPath().at(i));
        }
        domElement.appendChild(nodesElement);
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
    doc->SetAttribute(domElement, AttrWidth, piece.GetSAWidth());
    doc->SetAttribute(domElement, AttrUnited, piece.IsUnited());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, const CustomSARecord &record)
{
    QDomElement recordNode = doc->createElement(VToolSeamAllowance::TagRecord);

    doc->SetAttribute(recordNode, AttrStart, record.startPoint);
    doc->SetAttribute(recordNode, AttrPath, record.path);
    doc->SetAttribute(recordNode, AttrEnd, record.endPoint);
    doc->SetAttribute(recordNode, VAbstractPattern::AttrNodeReverse, record.reverse);
    doc->SetAttribute(recordNode, AttrIncludeAs, static_cast<unsigned int>(record.includeType));

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
void VToolSeamAllowance::AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddGrainline(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{

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
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SaveDialogChange();
    }
    delete m_dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::FullUpdateFromGuiApply()
{
    SaveDialogChange();
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
void VToolSeamAllowance::Highlight(quint32 id)
{
    setSelected(this->id == id);
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

    return VNoBrushScalePathItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    VNoBrushScalePathItem::mousePressEvent(event);

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
    VNoBrushScalePathItem::mouseReleaseEvent(event);
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
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    //Disable cursor-arrow-openhand
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
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
        auto *dialog = new DialogSeamAllowance(getData(), id, qApp->getMainWindow());
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

    VNoBrushScalePathItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SetDialog()
{
    SCASSERT(not m_dialog.isNull());
    DialogSeamAllowance *dialogTool = qobject_cast<DialogSeamAllowance*>(m_dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->SetPiece(VAbstractTool::data.GetPiece(id));
    dialogTool->EnableApply(true);
}

//---------------------------------------------------------------------------------------------------------------------
VToolSeamAllowance::VToolSeamAllowance(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const Source &typeCreation, VMainGraphicsScene *scene,
                                       const QString &drawName, QGraphicsItem *parent)
    : VAbstractTool(doc, data, id),
      VNoBrushScalePathItem(parent),
      m_dialog(),
      m_sceneDetails(scene),
      m_drawName(drawName),
      m_seamAllowance(new VNoBrushScalePathItem(this))
{
    VPiece detail = data->GetPiece(id);
    InitNodes(detail, scene);
    InitCSAPaths(detail);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();

    this->setBrush(QBrush(Qt::Dense7Pattern));
    m_seamAllowance->setBrush(QBrush(Qt::FDiagPattern));

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    connect(scene, &VMainGraphicsScene::EnableToolMove, this, &VToolSeamAllowance::EnableToolMove);
    //connect(scene, &VMainGraphicsScene::ItemClicked, this, &VToolSeamAllowance::ResetChildren);
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
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::RefreshGeometry()
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    const VPiece detail = VAbstractTool::data.GetPiece(id);
    QPainterPath mainPath = detail.MainPathPath(this->getData());
    this->setPath(mainPath);
    this->setPos(detail.GetMx(), detail.GetMy());

    if (detail.IsSeamAllowance())
    {
        mainPath.addPath(detail.SeamAllowancePath(this->getData()));
        mainPath.setFillRule(Qt::OddEvenFill);
        m_seamAllowance->setPath(mainPath);
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
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitNodes(const VPiece &detail, VMainGraphicsScene *scene)
{
    for (int i = 0; i< detail.GetPath().CountNodes(); ++i)
    {
        switch (detail.GetPath().at(i).GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                VNodePoint *tool = InitTool<VNodePoint>(scene, detail.GetPath().at(i).GetId());
                connect(tool, &VNodePoint::ShowContextMenu, this, &VToolSeamAllowance::contextMenuEvent);
                break;
            }
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
                doc->IncrementReferens(detail.GetPath().at(i).GetId());
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::InitCSAPaths(const VPiece &detail)
{
    QVector<CustomSARecord> records = detail.GetCustomSARecords();
    for (int i = 0; i < records.size(); ++i)
    {
        doc->IncrementReferens(records.at(i).path);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DeleteTool(bool ask)
{
    DeletePiece *delDet = new DeletePiece(doc, id, VAbstractTool::data.GetPiece(id));
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::No)
        {
            return;
        }
        /* If UnionDetails tool delete detail no need emit FullParsing.*/
        connect(delDet, &DeletePiece::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    }
    qApp->getUndoStack()->push(delDet);

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e("Tool was used after deleting.");
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
/**
 * @brief CreateNode create new node for detail.
 * @param data container.
 * @param id id parent object.
 * @return id for new object.
 */
quint32 VToolSeamAllowance::CreateNode(VContainer *data, const quint32 &id)
{
    //We can't use exist object. Need create new.
    T *node = new T(*data->GeometricObject<T>(id).data());
    node->setMode(Draw::Modeling);
    return data->AddGObject(node);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Tool>
/**
 * @brief InitTool initial node item on scene
 * @param scene pointer to scene.
 * @param toolId if of tool object.
 */
Tool *VToolSeamAllowance::InitTool(VMainGraphicsScene *scene, quint32 toolId)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    Tool *tool = qobject_cast<Tool*>(tools->value(toolId));
    SCASSERT(tool != nullptr);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
    tool->SetParentType(ParentType::Item);
    doc->IncrementReferens(toolId);
    return tool;
}
