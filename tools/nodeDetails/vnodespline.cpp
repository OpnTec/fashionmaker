/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vnodespline.h"

const QString VNodeSpline::TagName = QStringLiteral("spline");
const QString VNodeSpline::ToolType = QStringLiteral("modelingSpline");

VNodeSpline::VNodeSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                 Draw::Draws typeobject, Tool::Sources typeCreation, QGraphicsItem * parent) :
    VAbstractNode(doc, data, id, idSpline, typeobject), QGraphicsPathItem(parent){
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VNodeSpline *VNodeSpline::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                 Draw::Draws typeobject, const Document::Documents &parse,
                                 Tool::Sources typeCreation){
    VNodeSpline *spl = 0;
    if(parse == Document::FullParse){
        spl = new VNodeSpline(doc, data, id, idSpline, typeobject, typeCreation);
        doc->AddTool(id, spl);
        doc->IncrementReferens(idSpline);
    } else {
        doc->UpdateToolData(id, data);
    }
    return spl;
}

void VNodeSpline::FullUpdateFromFile(){
    RefreshGeometry();
}

void VNodeSpline::AddToFile(){
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrIdObject, idNode);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, AttrTypeObject, TypeObjectCalculation);
    } else {
        AddAttribute(domElement, AttrTypeObject, TypeObjectModeling);
    }

    AddToModeling(domElement);
}

void VNodeSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeSpline::RefreshGeometry(){
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
