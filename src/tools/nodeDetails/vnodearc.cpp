/************************************************************************
 **
 **  @file   vnodearc.cpp
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

#include "vnodearc.h"

#include <QtWidgets>

const QString VNodeArc::TagName = QStringLiteral("arc");
const QString VNodeArc::ToolType = QStringLiteral("modeling");

VNodeArc::VNodeArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, const Tool::Sources &typeCreation,
                   const qint64 &idTool, QObject *qoParent, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idArc, idTool, qoParent), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, toPixel(widthHairLine)));

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VNodeArc::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc,  const Document::Documents &parse,
                      const Tool::Sources &typeCreation, const qint64 &idTool, QObject *parent)
{
    VAbstractTool::AddRecord(id, Tool::NodeArc, doc);
    if (parse == Document::FullParse)
    {
        VNodeArc *arc = new VNodeArc(doc, data, id, idArc, typeCreation, idTool, parent);
        Q_CHECK_PTR(arc);
        doc->AddTool(id, arc);
        if (idTool != 0)
        {
            doc->IncrementReferens(idTool);
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            Q_CHECK_PTR(tool);
            arc->setParent(tool);
        }
        else
        {
            doc->IncrementReferens(idArc);
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

void VNodeArc::DeleteNode()
{
    VAbstractNode::DeleteNode();
    this->setVisible(false);
}

void VNodeArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VNodeArc::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != 0)
    {
        SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

void VNodeArc::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrIdObject, idNode);
        if (idTool != 0)
        {
            SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
}

void VNodeArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthMainLine)));
}

void VNodeArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthHairLine)));
}

void VNodeArc::RefreshGeometry()
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(id);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
