/************************************************************************
 **
 **  @file   vtoolsinglepoint.cpp
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

#include "vtoolbasepoint.h"
#include "../../../../dialogs/tools/dialogsinglepoint.h"
#include "../vwidgets/vgraphicssimpletextitem.h"
#include "../../../../undocommands/movespoint.h"
#include "../../../../undocommands/addpatternpiece.h"
#include "../../../../undocommands/deletepatternpiece.h"
#include "../vgeometry/vpointf.h"

#include <QMessageBox>

const QString VToolBasePoint::ToolType = QStringLiteral("single");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolBasePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolBasePoint::VToolBasePoint (VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                                const QString &namePP, QGraphicsItem * parent )
    :VToolSinglePoint(doc, data, id, parent), namePP(namePP)
{
    baseColor = Qt::red;
    this->setPen(QPen(baseColor, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    SetColorLabel(Qt::black);
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolBasePoint::~VToolBasePoint()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolBasePoint::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetData(p->name(), p->toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::ShowVisualization(bool show)
{
    Q_UNUSED(show); //don't have any visualization for base point yet
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolBasePoint::AddToFile()
{
    Q_ASSERT_X(not namePP.isEmpty(), Q_FUNC_INFO, "name pattern piece is empty");

    QDomElement sPoint = doc->createElement(TagName);

    // Create SPoint tag
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOptions(sPoint, obj);

    //Create pattern piece structure
    QDomElement patternPiece = doc->createElement(VAbstractPattern::TagDraw);
    doc->SetAttribute(patternPiece, AttrName, namePP);

    QDomElement calcElement = doc->createElement(VAbstractPattern::TagCalculation);
    calcElement.appendChild(sPoint);

    patternPiece.appendChild(calcElement);
    patternPiece.appendChild(doc->createElement(VAbstractPattern::TagModeling));
    patternPiece.appendChild(doc->createElement(VAbstractPattern::TagDetails));

    AddPatternPiece *addPP = new AddPatternPiece(patternPiece, doc, namePP);
    connect(addPP, &AddPatternPiece::ClearScene, doc, &VAbstractPattern::ClearScene);
    connect(addPP, &AddPatternPiece::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addPP);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle tool change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolBasePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - this is new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (rect.contains(newPos) == false)
        {
            // Save element into rect of scene.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    if (change == ItemPositionHasChanged && scene())
    {
        // Each time we move something we call recalculation scene rect. In some cases this can cause moving
        // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished)
        {
            changeFinished = false;
            // value - this is new position.
            QPointF newPos = value.toPointF();

            MoveSPoint *moveSP = new MoveSPoint(doc, newPos.x(), newPos.y(), id, this->scene());
            connect(moveSP, &MoveSPoint::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
            qApp->getUndoStack()->push(moveSP);
            changeFinished = true;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief decrementReferens decrement referens parents objects.
 */
void VToolBasePoint::decrementReferens()
{
    if (_referens > 1)
    {
        --_referens;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::DeleteTool(bool ask)
{
    qCDebug(vTool, "Deleting base point.");
    qApp->getSceneView()->itemClicked(nullptr);
    if (ask)
    {
        qCDebug(vTool, "Asking.");
        if (ConfirmDeletion() == QMessageBox::No)
        {
            qCDebug(vTool, "User said no.");
            return;
        }
    }

    qCDebug(vTool, "Begin deleting.");
    DeletePatternPiece *deletePP = new DeletePatternPiece(doc, nameActivDraw);
    connect(deletePP, &DeletePatternPiece::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(deletePP);

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e("Tool was used after deleting.");
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolBasePoint::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QPointF p = dialogTool->GetPoint();
    QString name = dialogTool->getPointName();
    doc->SetAttribute(domElement, AttrName, name);
    doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(p.x())));
    doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(p.y())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    VToolSinglePoint::hoverEnterEvent(event);

    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    VToolSinglePoint::hoverLeaveEvent(event);

    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        //Disable cursor-arrow-openhand
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }
    VToolSinglePoint::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            //Disable cursor-arrow-closehand
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
    }
    VToolSinglePoint::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetColorLabel change color for label and label line.
 * @param color new color.
 */
void VToolBasePoint::SetColorLabel(const Qt::GlobalColor &color)
{
    namePoint->setBrush(color);
    lineName->setPen(QPen(color, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrX, qApp->fromPixel(point->x()));
    doc->SetAttribute(tag, AttrY, qApp->fromPixel(point->y()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::ReadToolAttributes(const QDomElement &domElement)
{
    Q_UNUSED(domElement);
    // This tool doesn't need read attributes from file.
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolBasePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
    qCDebug(vTool, "Context menu base point");
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
    qCDebug(vTool, "Restored overriden cursor");
#endif

    try
    {
        if (doc->CountPP() > 1)
        {
            qCDebug(vTool, "PP count > 1");
            ContextMenu<DialogSinglePoint>(this, event, RemoveOption::Enable, Referens::Ignore);
        }
        else
        {
            qCDebug(vTool, "PP count = 1");
            ContextMenu<DialogSinglePoint>(this, event, RemoveOption::Disable);
        }
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        qCDebug(vTool, "Tool was deleted. Immediately leave method.");
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
    qCDebug(vTool, "Context menu closed. Tool was not deleted.");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void  VToolBasePoint::FullUpdateFromFile()
{
    VPointF point = *VAbstractTool::data.GeometricObject<VPointF>(id);
    RefreshPointGeometry(point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolBasePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*(VAbstractTool::data.GeometricObject<VPointF>(id)));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBasePoint::EnableToolMove(bool move)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, move);
    VToolSinglePoint::EnableToolMove(move);
}
