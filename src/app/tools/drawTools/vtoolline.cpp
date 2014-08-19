/************************************************************************
 **
 **  @file   vtoolline.cpp
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

#include "vtoolline.h"
#include "../../dialogs/tools/dialogline.h"
#include <QKeyEvent>
#include "../../geometry/vpointf.h"
#include "../../dialogs/tools/dialogline.h"

const QString VToolLine::TagName = QStringLiteral("line");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLine constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param firstPoint id first line point.
 * @param secondPoint id second line point.
 * @param typeLine line type.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolLine::VToolLine(VPattern *doc, VContainer *data, quint32 id, quint32 firstPoint, quint32 secondPoint,
                     const QString &typeLine, const Source &typeCreation, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint), secondPoint(secondPoint)
{
    this->typeLine = typeLine;
    ignoreFullUpdate = true;
    //Line
    const VPointF *first = data->GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = data->GeometricObject<const VPointF *>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));

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
void VToolLine::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->setFirstPoint(firstPoint);
    dialogTool->setSecondPoint(secondPoint);
    dialogTool->setTypeLine(typeLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolLine *VToolLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 firstPoint = dialogTool->getFirstPoint();
    const quint32 secondPoint = dialogTool->getSecondPoint();
    const QString typeLine = dialogTool->getTypeLine();

    VToolLine *line = nullptr;
    line = Create(0, firstPoint, secondPoint, typeLine, scene, doc, data, Document::FullParse, Source::FromGui);
    if (line != nullptr)
    {
        line->dialog=dialogTool;
    }
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param firstPoint id first line point.
 * @param secondPoint id second line point.
 * @param typeLine line type.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolLine * VToolLine::Create(const quint32 &_id, const quint32 &firstPoint, const quint32 &secondPoint,
                       const QString &typeLine, VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                       const Document &parse, const Source &typeCreation)
{
    SCASSERT(scene != nullptr);
    SCASSERT(doc != nullptr);
    SCASSERT(data != nullptr);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->getNextId();
        data->AddLine(firstPoint, secondPoint);
    }
    else
    {
        data->UpdateId(id);
        data->AddLine(firstPoint, secondPoint);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::LineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, typeLine, typeCreation);
        scene->addItem(line);
        connect(line, &VToolLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VToolLine::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, line, &VToolLine::Disable);
        doc->AddTool(id, line);
        doc->IncrementReferens(firstPoint);
        doc->IncrementReferens(secondPoint);
        return line;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsLineItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolLine::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool highlight tool.
 * @param id object id in container
 * @param color highlight color.
 * @param enable enable or disable highlight.
 */
void VToolLine::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::Disable(bool disable)
{
    DisableItem(this, disable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolLine::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
    this->setAcceptHoverEvents (selectable);
    VDrawTool::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolLine::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);
    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPoint);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPoint);
    doc->SetAttribute(domElement, AttrTypeLine, typeLine);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolLine::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrFirstPoint, firstPoint);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPoint);
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
void VToolLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())/factor, LineStyle(typeLine)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLine::RemoveReferens()
{
    doc->DecrementReferens(firstPoint);
    doc->DecrementReferens(secondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolLine::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
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

    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key realse events.
 * @param event key realse event.
 */
void VToolLine::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPoint()));
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VToolLine::RefreshGeometry()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPoint = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
        secondPoint = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
        typeLine = doc->GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
    }
    const VPointF *first = VAbstractTool::data.GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = VAbstractTool::data.GeometricObject<const VPointF *>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
}
