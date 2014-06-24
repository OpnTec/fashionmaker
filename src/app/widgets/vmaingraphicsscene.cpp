/************************************************************************
 **
 **  @file   vmaingraphicsscene.cpp
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

#include "vmaingraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QGraphicsItem>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsScene default constructor.
 */
VMainGraphicsScene::VMainGraphicsScene()
    :QGraphicsScene(), horScrollBar(0), verScrollBar(0), scaleFactor(1), _transform(QTransform())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsScene constructor.
 * @param sceneRect scene rect.
 * @param parent parent object.
 */
VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent)
    :QGraphicsScene ( sceneRect, parent ), horScrollBar(0), verScrollBar(0), scaleFactor(1), _transform(QTransform())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMoveEvent handle mouse move events.
 * @param event mouse move event.
 */
void VMainGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    emit mouseMove(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mousePressEvent mouse press events.
 * @param event mouse press event
 */
void VMainGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePress(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief transform return view transformation.
 * @return view transformation.
 */
QTransform VMainGraphicsScene::transform() const
{
    return _transform;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTransform set view transformation.
 * @param transform view transformation.
 */
void VMainGraphicsScene::setTransform(const QTransform &transform)
{
    _transform = transform;
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::SetDisable(bool disable)
{
    emit DisableItem(disable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedItem emit ChoosedObject signal.
 * @param id object id.
 * @param type object scene type.
 */
void VMainGraphicsScene::ChoosedItem(quint32 id, const SceneObject &type)
{
    emit ChoosedObject(id, type);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor. scene scale factor.
 */
void VMainGraphicsScene::SetFactor(qreal factor)
{
    scaleFactor=scaleFactor*factor;
    emit NewFactor(scaleFactor);
}
