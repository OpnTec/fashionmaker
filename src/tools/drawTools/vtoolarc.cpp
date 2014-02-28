/************************************************************************
 **
 **  @file   vtoolarc.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vtoolarc.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogarc.h"

const QString VToolArc::TagName = QStringLiteral("arc");
const QString VToolArc::ToolType = QStringLiteral("simple");

VToolArc::VToolArc(VPattern *doc, VContainer *data, quint32 id, const Tool::Sources &typeCreation,
                   QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent)
{
    const VArc *arc = data->GeometricObject<const VArc *>(id);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, toPixel(widthHairLine)/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolArc::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    dialogTool->SetCenter(arc->GetCenter().id());
    dialogTool->SetF1(arc->GetFormulaF1());
    dialogTool->SetF2(arc->GetFormulaF2());
    dialogTool->SetRadius(arc->GetFormulaRadius());
}

void VToolArc::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                      VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    quint32 center = dialogTool->GetCenter();
    QString radius = dialogTool->GetRadius();
    QString f1 = dialogTool->GetF1();
    QString f2 = dialogTool->GetF2();
    Create(0, center, radius, f1, f2, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolArc::Create(const quint32 _id, const quint32 &center, const QString &radius, const QString &f1,
                      const QString &f2, VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                      const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    qreal calcRadius = 0, calcF1 = 0, calcF2 = 0;

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcRadius = toPixel(result);
    }

    errorMsg.clear();
    result = cal.eval(f1, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcF1 = result;
    }

    errorMsg.clear();
    result = cal.eval(f2, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcF2 = result;
    }

    VPointF c = *data->GeometricObject<const VPointF *>(center);
    VArc *arc = new VArc(c, calcRadius, radius, calcF1, f1, calcF2, f2 );
    Q_CHECK_PTR(arc);
    quint32 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(arc);
    }
    else
    {
        data->UpdateGObject(id, arc);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLengthArc(id);
    VDrawTool::AddRecord(id, Tool::ArcTool, doc);
    if (parse == Document::FullParse)
    {
        VToolArc *toolArc = new VToolArc(doc, data, id, typeCreation);
        scene->addItem(toolArc);
        connect(toolArc, &VToolArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        doc->AddTool(id, toolArc);
        doc->IncrementReferens(center);
    }
}

void VToolArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolArc::ChangedActivDraw(const QString &newName)
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
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolArc::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolArc::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogArc>(this, event);
}

void VToolArc::AddToFile()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrCenter, arc->GetCenter().id());
    doc->SetAttribute(domElement, AttrRadius, arc->GetFormulaRadius());
    doc->SetAttribute(domElement, AttrAngle1, arc->GetFormulaF1());
    doc->SetAttribute(domElement, AttrAngle2, arc->GetFormulaF2());

    AddToCalculation(domElement);
}

void VToolArc::RefreshDataInFile()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrCenter, arc->GetCenter().id());
        doc->SetAttribute(domElement, AttrRadius, arc->GetFormulaRadius());
        doc->SetAttribute(domElement, AttrAngle1, arc->GetFormulaF1());
        doc->SetAttribute(domElement, AttrAngle2, arc->GetFormulaF2());
    }
}

void VToolArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//cppcheck-suppress unusedFunction
void VToolArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthMainLine)/factor));
}

//cppcheck-suppress unusedFunction
void VToolArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
}

void VToolArc::RemoveReferens()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    doc->DecrementReferens(arc->GetCenter().id());
}

QVariant VToolArc::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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

void VToolArc::keyReleaseEvent(QKeyEvent *event)
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

void VToolArc::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetCenter()));
    doc->SetAttribute(domElement, AttrRadius, dialogTool->GetRadius());
    doc->SetAttribute(domElement, AttrAngle1, dialogTool->GetF1());
    doc->SetAttribute(domElement, AttrAngle2, dialogTool->GetF2());
}

void VToolArc::RefreshGeometry()
{
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
