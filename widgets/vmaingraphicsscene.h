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
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VMAINGRAPHICSSCENE_H
#define VMAINGRAPHICSSCENE_H

#include <QGraphicsScene>

class VMainGraphicsScene : public QGraphicsScene{
    Q_OBJECT
public:
                  VMainGraphicsScene();
                  VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    inline qint32 getHorScrollBar() const {return horScrollBar;}
    inline void   setHorScrollBar(const qint32 &value) {horScrollBar = value;}
    inline qint32 getVerScrollBar() const {return verScrollBar;}
    inline void   setVerScrollBar(const qint32 &value) {verScrollBar = value;}
public slots:
    void          ChoosedItem(qint64 id, Scene::Scenes type);
    inline void   RemoveTool(QGraphicsItem *tool) {this->removeItem(tool);}
    void          SetFactor(qreal factor);
protected:
    void          mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void          mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void          mouseMove(QPointF scenePos);
    void          mousePress(QPointF scenePos);
    void          ChoosedObject(qint64 id, Scene::Scenes type);
    void          NewFactor(qreal factor);
private:
    qint32        horScrollBar;
    qint32        verScrollBar;
    qreal         scaleFactor;
};

#endif // VMAINGRAPHICSSCENE_H
