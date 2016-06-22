/************************************************************************
 **
 **  @file   vtooldetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vtooldetail.h"
#include "nodeDetails/nodedetails.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../dialogs/tools/dialogtool.h"
#include "../dialogs/tools/dialogdetail.h"
#include "../undocommands/savedetailoptions.h"
#include "../undocommands/movedetail.h"
#include "../undocommands/adddet.h"
#include "../undocommands/deletedetail.h"
#include "../vpatterndb/vpatternpiecedata.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsView>
#include <QMessageBox>

const QString VToolDetail::TagNode          = QStringLiteral("node");

const QString VToolDetail::AttrSupplement   = QStringLiteral("supplement");
const QString VToolDetail::AttrClosed       = QStringLiteral("closed");
const QString VToolDetail::AttrWidth        = QStringLiteral("width");
const QString VToolDetail::AttrNodeType     = QStringLiteral("nodeType");
const QString VToolDetail::AttrReverse      = QStringLiteral("reverse");

const QString VToolDetail::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolDetail::NodeTypeModeling = QStringLiteral("Modeling");

const QString VToolDetail::NodeArc          = QStringLiteral("NodeArc");
const QString VToolDetail::NodePoint        = QStringLiteral("NodePoint");
const QString VToolDetail::NodeSpline       = QStringLiteral("NodeSpline");
const QString VToolDetail::NodeSplinePath   = QStringLiteral("NodeSplinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail constructor.
 * @param doc dom document container
 * @param data container with variables
 * @param id object id in container
 * @param typeCreation way we create this tool.
 * @param scene pointer to scene.
 * @param parent parent object
 */
VToolDetail::VToolDetail(VAbstractPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                         VMainGraphicsScene *scene, const QString &drawName, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), VNoBrushScalePathItem(parent), dialog(nullptr), sceneDetails(scene),
      drawName(drawName), seamAllowance(new VNoBrushScalePathItem(this)),
      dataLabel(new VTextGraphicsItem(this))
{
    VDetail detail = data->GetDetail(id);
    for (int i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                VNodePoint *tool = InitTool<VNodePoint>(scene, detail.at(i));
                connect(tool, &VNodePoint::ShowContextMenu, this, &VToolDetail::contextMenuEvent);
                break;
            }
            case (Tool::NodeArc):
                doc->IncrementReferens(detail.at(i).getId());
                break;
            case (Tool::NodeSpline):
                doc->IncrementReferens(detail.at(i).getId());
                break;
            case (Tool::NodeSplinePath):
                doc->IncrementReferens(detail.at(i).getId());
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
    seamAllowance->setBrush(QBrush(Qt::FDiagPattern));

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    connect(scene, &VMainGraphicsScene::EnableToolMove, this, &VToolDetail::EnableToolMove);
    connect(scene, &VMainGraphicsScene::MouseLeftPressed, this, &VToolDetail::ResetChild);
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        AddToFile();
        if (typeCreation != Source::FromTool)
        {
            qApp->getUndoStack()->endMacro();
        }
    }
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
VToolDetail::~VToolDetail()
{
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolDetail::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail detail = VAbstractTool::data.GetDetail(id);
    dialogTool->setDetail(detail);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
void VToolDetail::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail detail = dialogTool->getDetail();
    VDetail det;
    qApp->getUndoStack()->beginMacro("add detail");
    for (int i = 0; i< detail.CountNode(); ++i)
    {
        quint32 id = 0;
        const VNodeDetail &nodeD = detail.at(i);
        switch (nodeD.getTypeTool())
        {
            case (Tool::NodePoint):
            {
                id = CreateNode<VPointF>(data, nodeD.getId());
                VNodePoint::Create(doc, data, scene, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeArc):
            {
                id = CreateNode<VArc>(data, nodeD.getId());
                VNodeArc::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSpline):
            {
                const auto obj = data->GetGObject(nodeD.getId());
                if (obj->getType() == GOType::Spline)
                {
                    id = CreateNode<VSpline>(data, nodeD.getId());
                }
                else
                {
                    id = CreateNode<VCubicBezier>(data, nodeD.getId());
                }
                VNodeSpline::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSplinePath):
            {
                const auto obj = data->GetGObject(nodeD.getId());
                if (obj->getType() == GOType::SplinePath)
                {
                    id = CreateNode<VSplinePath>(data, nodeD.getId());
                }
                else
                {
                    id = CreateNode<VCubicBezierPath>(data, nodeD.getId());
                }
                VNodeSplinePath::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            default:
                qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        VNodeDetail node(id, nodeD.getTypeTool(), NodeDetail::Contour, nodeD.getMx(), nodeD.getMy(),
                         nodeD.getReverse());
        det.append(node);
    }
    det.setName(detail.getName());
    det.setWidth(detail.getWidth());
    det.setClosed(detail.getClosed());
    det.setSeamAllowance(detail.getSeamAllowance());
    Create(0, det, scene, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param newDetail detail what we want show.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
void VToolDetail::Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene *scene, VAbstractPattern *doc,
                         VContainer *data, const Document &parse, const Source &typeCreation, const QString &drawName)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        id = data->AddDetail(newDetail);
    }
    else
    {
        data->UpdateDetail(id, newDetail);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::Detail, doc);
    if (parse == Document::FullParse)
    {
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene, drawName);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::EnableDetailItemHover, detail, &VToolDetail::AllowHover);
        connect(scene, &VMainGraphicsScene::EnableDetailItemSelection, detail, &VToolDetail::AllowSelecting);
        doc->AddTool(id, detail);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Remove full delete detail.
 */
void VToolDetail::Remove(bool ask)
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
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolDetail::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiOk refresh tool data after change in options.
 * @param result keep result working dialog.
 */
void VToolDetail::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(dialog != nullptr);
        DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
        SCASSERT(dialogTool != nullptr);

        VDetail newDet = dialogTool->getDetail();
        VDetail oldDet = VAbstractTool::data.GetDetail(id);

        qDebug() << "VTOOL" << newDet.GetPatternPieceData().GetLetter()
                    << newDet.GetPatternPieceData().GetName()
                       << newDet.GetPatternPieceData().GetMCPCount()
                          << dialogTool->getDetail().GetPatternPieceData().GetName();


        SaveDetailOptions *saveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
        connect(saveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveCommand);
    }
    delete dialog;
    dialog = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolDetail::AddToFile()
{
    VDetail detail = VAbstractTool::data.GetDetail(id);
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrName, detail.getName());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(detail.getMx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(detail.getMy()));
    doc->SetAttribute(domElement, AttrSupplement, static_cast<quint8>(detail.getSeamAllowance()));
    doc->SetAttribute(domElement, AttrClosed, static_cast<quint8>(detail.getClosed()));
    doc->SetAttribute(domElement, AttrWidth, detail.getWidth());

    QDomElement domData = doc->createElement(VAbstractPattern::TagData);
    doc->SetAttribute(domData, VAbstractPattern::AttrLetter, detail.GetPatternPieceData().GetLetter());
    doc->SetAttribute(domData, VAbstractPattern::AttrName, detail.GetPatternPieceData().GetName());
    for (int i = 0; i < detail.GetPatternPieceData().GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = detail.GetPatternPieceData().GetMCP(i);
        QDomElement domMCP = doc->createElement(VAbstractPattern::TagMCP);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrMaterial, int(mcp.m_eMaterial));
        doc->SetAttribute(domMCP, VAbstractPattern::AttrUserDefined, mcp.m_qsMaterialUserDef);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrCutNumber, mcp.m_iCutNumber);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrPlacement, int(mcp.m_ePlacement));
        domData.appendChild(domMCP);
    }
    domElement.appendChild(domData);

    for (int i = 0; i < detail.CountNode(); ++i)
    {
       AddNode(doc, domElement, detail.at(i));
    }

    AddDet *addDet = new AddDet(domElement, doc, detail, drawName);
    connect(addDet, &AddDet::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addDet);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolDetail::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        VDetail det = VAbstractTool::data.GetDetail(id);
        doc->SetAttribute(domElement, AttrName, det.getName());
        doc->SetAttribute(domElement, AttrSupplement, QString().setNum(static_cast<quint8>(det.getSeamAllowance())));
        doc->SetAttribute(domElement, AttrClosed, QString().setNum(static_cast<quint8>(det.getClosed())));
        doc->SetAttribute(domElement, AttrWidth, QString().setNum(det.getWidth()));
        doc->RemoveAllChildren(domElement);

        QDomElement domData = doc->createElement(VAbstractPattern::TagData);
        doc->SetAttribute(domData, VAbstractPattern::AttrLetter, det.GetPatternPieceData().GetLetter());
        doc->SetAttribute(domData, VAbstractPattern::AttrName, det.GetPatternPieceData().GetName());
        for (int i = 0; i < det.GetPatternPieceData().GetMCPCount(); ++i)
        {
            MaterialCutPlacement mcp = det.GetPatternPieceData().GetMCP(i);
            QDomElement domMCP = doc->createElement(VAbstractPattern::TagMCP);
            doc->SetAttribute(domMCP, VAbstractPattern::AttrMaterial, int(mcp.m_eMaterial));
            doc->SetAttribute(domMCP, VAbstractPattern::AttrUserDefined, mcp.m_qsMaterialUserDef);
            doc->SetAttribute(domMCP, VAbstractPattern::AttrCutNumber, mcp.m_iCutNumber);
            doc->SetAttribute(domMCP, VAbstractPattern::AttrPlacement, int(mcp.m_ePlacement));
            domData.appendChild(domMCP);
        }
        domElement.appendChild(domData);

        for (int i = 0; i < det.CountNode(); ++i)
        {
           AddNode(doc, domElement, det.at(i));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle detail change.
 * @param change change
 * @param value value
 * @return new value.
 */
QVariant VToolDetail::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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

               MoveDetail *moveDet = new MoveDetail(doc, newPos.x(), newPos.y(), id);
               connect(moveDet, &MoveDetail::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
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
/**
 * @brief keyReleaseEvent handle key release events.
 * @param event key release event.
 */
void VToolDetail::keyReleaseEvent(QKeyEvent *event)
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
void VToolDetail::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        emit ChoosedTool(id, SceneObject::Detail);
    }

    event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VToolDetail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //Disable cursor-arrow-closehand
        RestoreOverrideCursor(cursorArrowCloseHand);
    }
    VNoBrushScalePathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    //Disable cursor-arrow-openhand
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolDetail::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));
    QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    if (_referens > 1)
    {
        actionRemove->setEnabled(false);
    }
    else
    {
        actionRemove->setEnabled(true);
    }
    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        dialog = new DialogDetail(getData(), id, qApp->getMainWindow());
        dialog->setModal(true);
        connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                dialog, &DialogTool::ChosenObject);
        connect(dialog, &DialogTool::DialogClosed, this, &VToolDetail::FullUpdateFromGuiOk);
        setDialog();
        dialog->show();
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
/**
 * @brief AddNode add node to the file.
 * @param domElement tag in xml tree.
 * @param node node of detail.
 */
void VToolDetail::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VNodeDetail &node)
{
    QDomElement nod = doc->createElement(TagNode);

    doc->SetAttribute(nod, AttrIdObject, node.getId());
    doc->SetAttribute(nod, AttrMx, qApp->fromPixel(node.getMx()));
    doc->SetAttribute(nod, AttrMy, qApp->fromPixel(node.getMy()));

    if (node.getTypeTool() != Tool::NodePoint)
    {
        doc->SetAttribute(nod, AttrReverse, static_cast<quint8>(node.getReverse()));
    }

    if (node.getTypeNode() == NodeDetail::Contour)
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
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
QString VToolDetail::getTagName() const
{
    return VAbstractPattern::TagDetail;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object);
    Q_UNUSED(visible);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VToolDetail::RefreshGeometry()
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    const VDetail detail = VAbstractTool::data.GetDetail(id);
    QPainterPath mainPath = detail.ContourPath(this->getData());
    this->setPath(mainPath);
    this->setPos(detail.getMx(), detail.getMy());

    if (detail.getSeamAllowance())
    {
        mainPath.addPath(detail.SeamAllowancePath(this->getData()));
        mainPath.setFillRule(Qt::OddEvenFill);
        seamAllowance->setPath(mainPath);
    }
    else
    {
        seamAllowance->setPath(QPainterPath());
    }

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    const VPatternPieceData& data = detail.GetPatternPieceData();
    if (data.GetLetter().isEmpty() == false || data.GetName().isEmpty() == false || data.GetMCPCount() > 0)
    {
        QString qsText = "Cut %1 on %2%3";
        QStringList qslPlace;
        qslPlace << "" << " on Fold";
        QFont fnt = qApp->font();
        fnt.setPixelSize(24);
        QFontMetrics fm(fnt);
        dataLabel->setFont(fnt);
        int iMinW = 200;
        QString qsHTML = "<b><font size=\"+4\">" + data.GetLetter() + "</font></b><br/>";
        qsHTML += "<font size=\"+2\">" + data.GetName() + "</font><br/>";
        for (int i = 0; i < data.GetMCPCount(); ++i)
        {
            MaterialCutPlacement mcp = data.GetMCP(i);
            QString qsLine = qsText.arg(mcp.m_iCutNumber).arg(mcp.m_qsMaterialUserDef).arg(qslPlace[int(mcp.m_ePlacement)]);
            if (fm.width(qsLine) > iMinW)
                iMinW = fm.width(qsLine);
            qsHTML += qsLine + "<br/>";
        }
        // also add some offset
        dataLabel->SetMinimalWidth(iMinW + 10);
        dataLabel->SetHTML(qsHTML);
        dataLabel->show();
    }
    else
    {
        dataLabel->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::DeleteTool(bool ask)
{
    DeleteDetail *delDet = new DeleteDetail(doc, id, VAbstractTool::data.GetDetail(id));
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::No)
        {
            return;
        }
        /* If UnionDetails tool delete detail no need emit FullParsing.*/
        connect(delDet, &DeleteDetail::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    }
    qApp->getUndoStack()->push(delDet);

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e("Tool was used after deleting.");
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Tool>
//cppcheck-suppress unusedFunction
Tool* VToolDetail::InitTool(VMainGraphicsScene *scene, const VNodeDetail &node)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.getId()));
    SCASSERT(tool != nullptr);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
    tool->SetParentType(ParentType::Item);
    doc->IncrementReferens(node.getId());
    return tool;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::EnableToolMove(bool move)
{
    setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::AllowHover(bool enabled)
{
    setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::AllowSelecting(bool enabled)
{
    setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::ResetChild()
{
    dataLabel->Reset();
}
