/************************************************************************
 **
 **  @file   vnodesplinepath.cpp
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

#include "vnodesplinepath.h"

#include "../../core/vapplication.h"
#include "../../geometry/vsplinepath.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>

const QString VNodeSplinePath::TagName = QStringLiteral("spline");
const QString VNodeSplinePath::ToolType = QStringLiteral("modelingPath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodeSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idSpline object id in containerSpline.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param qoParent QObject parent.
 * @param parent parent object.
 */
VNodeSplinePath::VNodeSplinePath(VPattern *doc, VContainer *data, quint32 id, quint32 idSpline,
                                 const Source &typeCreation, const quint32 &idTool, QObject *qoParent,
                                 QGraphicsItem * parent)
    :VAbstractNode(doc, data, id, idSpline, idTool, qoParent), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, qApp->toPixel(qApp->widthHairLine())));

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
 * @brief Create help create tool.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idSpline object id in containerSpline.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param parent QObject parent.
 */
void VNodeSplinePath::Create(VPattern *doc, VContainer *data, quint32 id, quint32 idSpline, const Document &parse,
                             const Source &typeCreation, const quint32 &idTool, QObject *parent)
{
    VAbstractTool::AddRecord(id, Tool::NodeSplinePath, doc);
    if (parse == Document::FullParse)
    {
        VNodeSplinePath *splPath = new VNodeSplinePath(doc, data, id, idSpline, typeCreation, idTool, parent);
        doc->AddTool(id, splPath);
        const QSharedPointer<VSplinePath> path = data->GeometricObject<VSplinePath>(id);
        const QVector<VSplinePoint> *points = path->GetPoint();
        for (qint32 i = 0; i<points->size(); ++i)
        {
            if (idTool != 0)
            {
                doc->IncrementReferens(idTool);
                //Some nodes we don't show on scene. Tool that create this nodes must free memory.
                VDataTool *tool = doc->getTool(idTool);
                SCASSERT(tool != nullptr);
                splPath->setParent(tool);
            }
            else
            {
                doc->IncrementReferens(points->at(i).P().id());
            }
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteNode delete node from detail.
 */
void VNodeSplinePath::DeleteNode()
{
    VAbstractNode::DeleteNode();
    this->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeSplinePath::RestoreNode()
{
    if (this->isVisible() == false)
    {
        VAbstractNode::RestoreNode();
        this->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodeSplinePath::getTagName() const
{
    return VNodeSplinePath::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VNodeSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodeSplinePath::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != 0)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VNodeSplinePath::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIdObject, QString().setNum(idNode));
        if (idTool != 0)
        {
            doc->SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VNodeSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VNodeSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VNodeSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VNodeSplinePath::RefreshGeometry()
{
//    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(id);
//    QPainterPath path;
//    path.addPath(splPath->GetPath());
//    path.setFillRule( Qt::WindingFill );
//    this->setPath(path);
}
