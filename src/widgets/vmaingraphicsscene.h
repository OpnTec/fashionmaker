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
 * @brief The VMainGraphicsScene class main scene.
 */
class VMainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
                  /**
                   * @brief VMainGraphicsScene default constructor.
                   */
                  VMainGraphicsScene();
                  /**
                   * @brief VMainGraphicsScene constructor.
                   * @param sceneRect scene rect.
                   * @param parent parent object.
                   */
                  VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    /**
     * @brief getHorScrollBar return scene horizontal scrollbar.
     * @return horizontal scrollbar.
     */
    inline qint32 getHorScrollBar() const {return horScrollBar;}
    /**
     * @brief setHorScrollBar set scene horizontal scrollbar.
     * @param value horizontal scrollbar.
     */
    inline void   setHorScrollBar(const qint32 &value) {horScrollBar = value;}
    /**
     * @brief getVerScrollBar return scene vertical scrollbar.
     * @return vertical scrollbar.
     */
    inline qint32 getVerScrollBar() const {return verScrollBar;}
    /**
     * @brief setVerScrollBar set scene vertical scrollbar.
     * @param value vertical scrollbar.
     */
    inline void   setVerScrollBar(const qint32 &value) {verScrollBar = value;}
    /**
     * @brief transform return view transformation.
     * @return view transformation.
     */
    QTransform    transform() const;
    /**
     * @brief setTransform set view transformation.
     * @param transform view transformation.
     */
    void          setTransform(const QTransform &transform);
public slots:
    /**
     * @brief ChoosedItem emit ChoosedObject signal.
     * @param id object id.
     * @param type object scene type.
     */
    void          ChoosedItem(quint32 id, const Scene::Scenes &type);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor. scene scale factor.
     */
    void          SetFactor(qreal factor);
protected:
    /**
     * @brief mouseMoveEvent handle mouse move events.
     * @param event mouse move event.
     */
    void          mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    /**
     * @brief mousePressEvent mouse press events.
     * @param event mouse press event
     */
    void          mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    /**
     * @brief mouseMove send new mouse position.
     * @param scenePos new mouse position.
     */
    void          mouseMove(QPointF scenePos);
    /**
     * @brief mousePress send new mouse press position.
     * @param scenePos new mouse press position.
     */
    void          mousePress(QPointF scenePos);
    /**
     * @brief ChoosedObject send option choosed object.
     * @param id object id.
     * @param type object scene type.
     */
    void          ChoosedObject(quint32 id, Scene::Scenes type);
    /**
     * @brief NewFactor send new scale factor.
     * @param factor scene scale factor.
     */
    void          NewFactor(qreal factor);
private:
    /**
     * @brief horScrollBar value horizontal scroll bar.
     */
    qint32        horScrollBar;
    /**
     * @brief verScrollBar value vertical scroll bar.
     */
    qint32        verScrollBar;
    /**
     * @brief scaleFactor scale factor.
     */
    qreal         scaleFactor;
    /**
     * @brief _transform view transform value.
     */
    QTransform    _transform;
};

#endif // VMAINGRAPHICSSCENE_H
