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

#include <QGraphicsSceneMouseEvent>
#include <QMenu>

// Current version of seam allowance tag nned for backward compatibility
const quint8 VToolSeamAllowance::pieceVersion = 2;

const QString VToolSeamAllowance::TagNodes    = QStringLiteral("nodes");
const QString VToolSeamAllowance::TagNode     = QStringLiteral("node");

const QString VToolSeamAllowance::AttrVersion     = QStringLiteral("version");
const QString VToolSeamAllowance::AttrNodeReverse = QStringLiteral("reverse");

const QString VToolSeamAllowance::NodeArc        = QStringLiteral("NodeArc");
const QString VToolSeamAllowance::NodePoint      = QStringLiteral("NodePoint");
const QString VToolSeamAllowance::NodeSpline     = QStringLiteral("NodeSpline");
const QString VToolSeamAllowance::NodeSplinePath = QStringLiteral("NodeSplinePath");

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
    for (int i = 0; i< detail.CountNodes(); ++i)
    {
        quint32 id = 0;
        VPieceNode nodeD = detail.at(i);
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

    detail.SetNodes(nodes);
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
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VPieceNode &node)
{
    QDomElement nod = doc->createElement(TagNode);

    doc->SetAttribute(nod, AttrIdObject, node.GetId());

    if (node.GetTypeTool() != Tool::NodePoint)
    {
        doc->SetAttribute(nod, AttrNodeReverse, static_cast<quint8>(node.GetReverse()));
    }

    switch (node.GetTypeTool())
    {
        case (Tool::NodeArc):
            doc->SetAttribute(nod, AttrType, NodeArc);
            break;
        case (Tool::NodePoint):
            doc->SetAttribute(nod, AttrType, NodePoint);
            break;
        case (Tool::NodeSpline):
            doc->SetAttribute(nod, AttrType, NodeSpline);
            break;
        case (Tool::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, NodeSplinePath);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    if (piece.CountNodes() > 0)
    {
        QDomElement nodesElement = doc->createElement(TagNodes);
        for (int i = 0; i < piece.CountNodes(); ++i)
        {
            AddNode(doc, nodesElement, piece.at(i));
        }
        domElement.appendChild(nodesElement);
    }
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
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::FullUpdateFromGuiOk(int result)
{}

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

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrVersion, QString().setNum(pieceVersion));
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(piece.GetMx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(piece.GetMy()));

    // nodes
    AddNodes(doc, domElement, piece);

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
            // TODO. Delete if minimal supported version is 0.5.0
            Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < CONVERTER_VERSION_CHECK(0, 5, 0),
                              "Time to refactor the code.");
            doc->SetAttribute(domElement, AttrVersion, QString().setNum(pieceVersion));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VToolSeamAllowance::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
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

//    QAction *inLayoutOption = menu.addAction(tr("In layout"));
//    inLayoutOption->setCheckable(true);
//    const VDetail detail = VAbstractTool::data.GetDetail(id);
//    inLayoutOption->setChecked(detail.IsInLayout());

//    QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
//    _referens > 1 ? actionRemove->setEnabled(false) : actionRemove->setEnabled(true);

    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        m_dialog = new DialogSeamAllowance(getData(), id, qApp->getMainWindow());
        m_dialog->setModal(true);
        connect(m_dialog, &DialogTool::DialogClosed, this, &VToolSeamAllowance::FullUpdateFromGuiOk);
        SetDialog();
        m_dialog->show();
    }
//    else if (selectedAction == inLayoutOption)
//    {
//        ToggleDetailInLayout *togglePrint = new ToggleDetailInLayout(id, selectedAction->isChecked(),
//                                                                     &(VAbstractTool::data), doc);
//        connect(togglePrint, &ToggleDetailInLayout::UpdateList, doc, &VAbstractPattern::CheckInLayoutList);
//        qApp->getUndoStack()->push(togglePrint);
//    }
//    else if (selectedAction == actionRemove)
//    {
//        try
//        {
//            DeleteTool();
//        }
//        catch(const VExceptionToolWasDeleted &e)
//        {
//            Q_UNUSED(e);
//            return;//Leave this method immediately!!!
//        }
//        return; //Leave this method immediately after call!!!
    //    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::keyReleaseEvent(QKeyEvent *event)
{
    VNoBrushScalePathItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::SetDialog()
{
    SCASSERT(m_dialog != nullptr);
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
      m_dialog(nullptr),
      m_sceneDetails(scene),
      m_drawName(drawName)
{
    VPiece detail = data->GetPiece(id);
    for (int i = 0; i< detail.CountNodes(); ++i)
    {
        switch (detail.at(i).GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                VNodePoint *tool = InitTool<VNodePoint>(scene, detail.at(i));
                connect(tool, &VNodePoint::ShowContextMenu, this, &VToolSeamAllowance::contextMenuEvent);
                break;
            }
            case (Tool::NodeArc):
                doc->IncrementReferens(detail.at(i).GetId());
                break;
            case (Tool::NodeSpline):
                doc->IncrementReferens(detail.at(i).GetId());
                break;
            case (Tool::NodeSplinePath):
                doc->IncrementReferens(detail.at(i).GetId());
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();

    this->setBrush(QBrush(Qt::Dense7Pattern));

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

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSeamAllowance::DeleteTool(bool ask)
{

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
 * @param node node of detail.
 */
Tool *VToolSeamAllowance::InitTool(VMainGraphicsScene *scene, const VPieceNode &node)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.GetId()));
    SCASSERT(tool != nullptr);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
    tool->SetParentType(ParentType::Item);
    doc->IncrementReferens(node.GetId());
    return tool;
}
