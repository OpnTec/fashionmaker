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

#include "vtoolsinglepoint.h"
#include "../../dialogs/tools/dialogsinglepoint.h"
#include "../../visualization/vgraphicssimpletextitem.h"
#include "../../undocommands/movespoint.h"
#include "../../undocommands/addpatternpiece.h"
#include "../../undocommands/deletepatternpiece.h"
#include "../../geometry/vpointf.h"

#include <QMessageBox>

const QString VToolSinglePoint::ToolType = QStringLiteral("single");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSinglePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolSinglePoint::VToolSinglePoint (VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                                    const QString &namePP, const QString &mPath, QGraphicsItem * parent )
    :VToolPoint(doc, data, id, parent), namePP(namePP), mPath(mPath)
{
    baseColor = Qt::red;
    currentColor = baseColor;
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    ignoreFullUpdate = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    setColorLabel(Qt::black);
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolSinglePoint::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setData(p->name(), p->toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::ShowVisualization(bool show)
{
    Q_UNUSED(show); //don't have any visualization for base point yet
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolSinglePoint::AddToFile()
{
    Q_ASSERT_X(namePP.isEmpty() == false, "AddToFile", "name pattern piece is empty");

    QDomElement sPoint = doc->createElement(TagName);

    // Create SPoint tag
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOptions(sPoint, obj);

    //Create pattern piece structure
    QDomElement patternPiece = doc->createElement(VPattern::TagDraw);
    doc->SetAttribute(patternPiece, AttrName, namePP);

    QDomElement calcElement = doc->createElement(VPattern::TagCalculation);
    calcElement.appendChild(sPoint);

    patternPiece.appendChild(calcElement);
    patternPiece.appendChild(doc->createElement(VPattern::TagModeling));
    patternPiece.appendChild(doc->createElement(VPattern::TagDetails));

    AddPatternPiece *addPP = new AddPatternPiece(patternPiece, doc, namePP, mPath);
    connect(addPP, &AddPatternPiece::ClearScene, doc, &VPattern::ClearScene);
    connect(addPP, &AddPatternPiece::NeedFullParsing, doc, &VPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addPP);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle tool change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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
        // value - this is new position.
        QPointF newPos = value.toPointF();

        MoveSPoint *moveSP = new MoveSPoint(doc, newPos.x(), newPos.y(), id, this->scene());
        connect(moveSP, &MoveSPoint::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(moveSP);
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief decrementReferens decrement referens parents objects.
 */
void VToolSinglePoint::decrementReferens()
{
    if (_referens > 1)
    {
        --_referens;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::DeleteTool(bool ask)
{
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::Cancel)
        {
            return;
        }
    }

    DeletePatternPiece *deletePP = new DeletePatternPiece(doc, nameActivDraw);
    connect(deletePP, &DeletePatternPiece::NeedFullParsing, doc, &VPattern::NeedFullParsing);
    qApp->getUndoStack()->push(deletePP);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolSinglePoint::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QPointF p = dialogTool->getPoint();
    QString name = dialogTool->getName();
    doc->SetAttribute(domElement, AttrName, name);
    doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(p.x())));
    doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(p.y())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    VToolPoint::hoverEnterEvent(event);

    VApplication::setOverrideCursor(QStringLiteral("://cursor/cursor-arrow-openhand.png"), 1, 1);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    VToolPoint::hoverLeaveEvent(event);

    //Disable cursor-arrow-openhand
    VApplication::restoreOverrideCursor(QStringLiteral("://cursor/cursor-arrow-openhand.png"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        VApplication::setOverrideCursor(QStringLiteral("://cursor/cursor-arrow-closehand.png"), 1, 1);
    }
    VToolPoint::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        //Disable cursor-arrow-closehand
        VApplication::restoreOverrideCursor(QStringLiteral("://cursor/cursor-arrow-closehand.png"));
    }
    VToolPoint::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setColorLabel change color for label and label line.
 * @param color new color.
 */
void VToolSinglePoint::setColorLabel(const Qt::GlobalColor &color)
{
    namePoint->setBrush(color);
    lineName->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrX, qApp->fromPixel(point->x()));
    doc->SetAttribute(tag, AttrY, qApp->fromPixel(point->y()));
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    quint32 ref = _referens; // store referens
    _referens = 1; // make available delete pattern piece
    if (doc->CountPP() > 1)
    {
        ContextMenu<DialogSinglePoint>(this, event);
    }
    else
    {
        ContextMenu<DialogSinglePoint>(this, event, false);
    }
    _referens = ref; // restore referens. If not restore garbage collector delete point!!!
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void  VToolSinglePoint::FullUpdateFromFile()
{
    VPointF point = *VAbstractTool::data.GeometricObject<VPointF>(id);
    RefreshPointGeometry(point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolSinglePoint::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::black);
    }
    else
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::gray);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolSinglePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*(VAbstractTool::data.GeometricObject<VPointF>(id)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolSinglePoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    if (doc->CountPP() > 1)
    {
        ContextMenu<DialogSinglePoint>(this, event);
    }
    else
    {
        ContextMenu<DialogSinglePoint>(this, event, false);
    }
}
