/************************************************************************
 **
 **  @file   vmaingraphicsscene.h
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

#ifndef VMAINGRAPHICSSCENE_H
#define VMAINGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "../options.h"

/**
 * @brief The VMainGraphicsScene class
 */
class VMainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
                  /**
                   * @brief VMainGraphicsScene
                   */
                  VMainGraphicsScene();
                  /**
                   * @brief VMainGraphicsScene
                   * @param sceneRect
                   * @param parent
                   */
                  VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    /**
     * @brief getHorScrollBar
     * @return
     */
    inline qint32 getHorScrollBar() const {return horScrollBar;}
    /**
     * @brief setHorScrollBar
     * @param value
     */
    inline void   setHorScrollBar(const qint32 &value) {horScrollBar = value;}
    /**
     * @brief getVerScrollBar
     * @return
     */
    inline qint32 getVerScrollBar() const {return verScrollBar;}
    /**
     * @brief setVerScrollBar
     * @param value
     */
    inline void   setVerScrollBar(const qint32 &value) {verScrollBar = value;}
public slots:
    /**
     * @brief ChoosedItem
     * @param id
     * @param type
     */
    void          ChoosedItem(qint64 id, const Scene::Scenes &type);
    /**
     * @brief SetFactor
     * @param factor
     */
    void          SetFactor(qreal factor);
protected:
    /**
     * @brief mouseMoveEvent
     * @param event
     */
    void          mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    /**
     * @brief mousePressEvent
     * @param event
     */
    void          mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    /**
     * @brief mouseMove
     * @param scenePos
     */
    void          mouseMove(QPointF scenePos);
    /**
     * @brief mousePress
     * @param scenePos
     */
    void          mousePress(QPointF scenePos);
    /**
     * @brief ChoosedObject
     * @param id
     * @param type
     */
    void          ChoosedObject(qint64 id, Scene::Scenes type);
    /**
     * @brief NewFactor
     * @param factor
     */
    void          NewFactor(qreal factor);
private:
    /**
     * @brief horScrollBar
     */
    qint32        horScrollBar;
    /**
     * @brief verScrollBar
     */
    qint32        verScrollBar;
    /**
     * @brief scaleFactor
     */
    qreal         scaleFactor;
};

#endif // VMAINGRAPHICSSCENE_H
