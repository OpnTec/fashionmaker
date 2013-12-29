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

const QString VToolArc::TagName = QStringLiteral("arc");
const QString VToolArc::ToolType = QStringLiteral("simple");

VToolArc::VToolArc(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                   QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), dialogArc(QSharedPointer<DialogArc>())
{
    const VArc *arc = data->GeometricObject<const VArc *>(id);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
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
    Q_ASSERT(dialogArc.isNull() == false);
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    dialogArc->SetCenter(arc->GetCenter().id());
    dialogArc->SetF1(arc->GetFormulaF1());
    dialogArc->SetF2(arc->GetFormulaF2());
    dialogArc->SetRadius(arc->GetFormulaRadius());
}

void VToolArc::Create(QSharedPointer<DialogArc> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                      VContainer *data)
{
    qint64 center = dialog->GetCenter();
    QString radius = dialog->GetRadius();
    QString f1 = dialog->GetF1();
    QString f2 = dialog->GetF2();
    Create(0, center, radius, f1, f2, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolArc::Create(const qint64 _id, const qint64 &center, const QString &radius, const QString &f1,
                      const QString &f2, VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
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
    Q_ASSERT(arc != 0);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(arc);
        data->AddLengthArc(arc->name(), toMM(arc->GetLength()));
    }
    else
    {
        data->UpdateGObject(id, arc);
        data->AddLengthArc(arc->name(), toMM(arc->GetLength()));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::ArcTool, doc);
    if (parse == Document::FullParse)
    {
        VToolArc *toolArc = new VToolArc(doc, data, id, typeCreation);
        scene->addItem(toolArc);
        connect(toolArc, &VToolArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(toolArc, &VToolArc::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        doc->AddTool(id, toolArc);
        doc->IncrementReferens(center);
    }
}

void VToolArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolArc::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrCenter, QString().setNum(dialogArc->GetCenter()));
            domElement.setAttribute(AttrRadius, dialogArc->GetRadius());
            domElement.setAttribute(AttrAngle1, dialogArc->GetF1());
            domElement.setAttribute(AttrAngle2, dialogArc->GetF2());
            emit FullUpdateTree();
        }
    }
    dialogArc.clear();
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
    this->setPen(QPen(currentColor, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolArc::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
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
    ContextMenu(dialogArc, this, event);
}

void VToolArc::AddToFile()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrCenter, arc->GetCenter().id());
    AddAttribute(domElement, AttrRadius, arc->GetFormulaRadius());
    AddAttribute(domElement, AttrAngle1, arc->GetFormulaF1());
    AddAttribute(domElement, AttrAngle2, arc->GetFormulaF2());

    AddToCalculation(domElement);
}

void VToolArc::RefreshDataInFile()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrCenter, arc->GetCenter().id());
        domElement.setAttribute(AttrRadius, arc->GetFormulaRadius());
        domElement.setAttribute(AttrAngle1, arc->GetFormulaF1());
        domElement.setAttribute(AttrAngle2, arc->GetFormulaF2());
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

void VToolArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine/factor));
}

void VToolArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine/factor));
}

void VToolArc::RemoveReferens()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    doc->DecrementReferens(arc->GetCenter().id());
}

void VToolArc::RefreshGeometry()
{
    this->setPen(QPen(currentColor, widthHairLine/factor));
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
