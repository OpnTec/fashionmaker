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
#include "../../visualization/vistoolline.h"

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
                     const QString &typeLine, const QString &lineColor, const Source &typeCreation,
                     QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint), secondPoint(secondPoint)
{
    this->typeLine = typeLine;
    this->lineColor = lineColor;
    ignoreFullUpdate = true;
    //Line
    const QSharedPointer<VPointF> first = data->GeometricObject<VPointF>(firstPoint);
    const QSharedPointer<VPointF> second = data->GeometricObject<VPointF>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor, LineStyleToPenStyle(typeLine)));

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
    dialogTool->SetFirstPoint(firstPoint);
    dialogTool->SetSecondPoint(secondPoint);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetLineColor(lineColor);
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
    const quint32 firstPoint = dialogTool->GetFirstPoint();
    const quint32 secondPoint = dialogTool->GetSecondPoint();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();

    VToolLine *line = nullptr;
    line = Create(0, firstPoint, secondPoint, typeLine, lineColor,  scene, doc, data, Document::FullParse,
                  Source::FromGui);
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
                              const QString &typeLine, const QString &lineColor, VMainGraphicsScene *scene,
                              VPattern *doc, VContainer *data, const Document &parse, const Source &typeCreation)
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
    VDrawTool::AddRecord(id, Tool::Line, doc);
    if (parse == Document::FullParse)
    {
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, typeLine, lineColor, typeCreation);
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
QString VToolLine::getTagName() const
{
    return VToolLine::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolLine::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();

    if (vis != nullptr)
    {
        VisToolLine *visual = qobject_cast<VisToolLine *>(vis);
        visual->setPoint1Id(firstPoint);
        visual->setPoint2Id(secondPoint);
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool highlight tool.
 * @param id object id in container
 * @param enable enable or disable highlight.
 */
void VToolLine::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
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
    enabled = !disable;
    this->setEnabled(enabled);
    this->setPen(QPen(QColor(baseColor), qApp->toPixel(qApp->widthHairLine())/factor, LineStyleToPenStyle(typeLine)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolLine::ChangedActivDraw(const QString &newName)
{
    Disable(!(nameActivDraw == newName));
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
    QSharedPointer<VGObject> obj = QSharedPointer<VGObject> ();
    SaveOptions(domElement, obj);
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
        QSharedPointer<VGObject> obj = QSharedPointer<VGObject> ();
        SaveOptions(domElement, obj);
    }
    else
    {
        qDebug()<<"Can't find tool with id ="<< id << Q_FUNC_INFO;
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
    this->setPen(QPen(CorrectColor(lineColor), qApp->toPixel(qApp->widthMainLine())/factor,
                      LineStyleToPenStyle(typeLine)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (vis == nullptr)
    {
        this->setPen(QPen(CorrectColor(lineColor), qApp->toPixel(qApp->widthHairLine())/factor,
                          LineStyleToPenStyle(typeLine)));
    }
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
            DeleteTool();
            return; //Leave this method immediately after call!!!
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
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPoint()));
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    Q_UNUSED(obj)

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrFirstPoint, firstPoint);
    doc->SetAttribute(tag, AttrSecondPoint, secondPoint);
    doc->SetAttribute(tag, AttrTypeLine, typeLine);
    doc->SetAttribute(tag, AttrLineColor, lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::ReadToolAttributes(const QDomElement &domElement)
{
    firstPoint = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPoint = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLine::GetSecondPoint() const
{
    return secondPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetSecondPoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPoint = value;

        QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolLine * visual = new VisToolLine(getData());
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
            scene->addItem(visual);

            visual->setPoint1Id(firstPoint);
            visual->setPoint2Id(secondPoint);
            visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolLine *visual = qobject_cast<VisToolLine *>(vis);
            if (visual != nullptr)
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        vis = nullptr;
        hoverLeaveEvent(nullptr);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetTypeLine(const QString &value)
{
    typeLine = value;

    QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetLineColor(const QString &value)
{
    lineColor = value;

    QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLine::GetFirstPoint() const
{
    return firstPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetFirstPoint(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstPoint = value;

        QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VToolLine::RefreshGeometry()
{
    const QSharedPointer<VPointF> first = VAbstractTool::data.GeometricObject<VPointF>(firstPoint);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setPen(QPen(CorrectColor(lineColor), pen().widthF(), LineStyleToPenStyle(typeLine)));
}
