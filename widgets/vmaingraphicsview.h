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

#ifndef VMAINGRAPHICSVIEW_H
#define VMAINGRAPHICSVIEW_H

#include <QGraphicsView>

class VMainGraphicsView : public QGraphicsView{
    Q_OBJECT
public:
    explicit VMainGraphicsView(QWidget *parent = 0);
signals:
    void     NewFactor(qreal factor);
public slots:
    void     scalingTime(qreal x);
    void     animFinished();
protected:
    /**
     * @brief wheelEvent обробник повороту колеса мишки.
     * @param event передається подія.
     */
    void     wheelEvent ( QWheelEvent * event );
    void     mousePressEvent(QMouseEvent *mousePress);
    void     mouseReleaseEvent(QMouseEvent *event);
private:
    qint32   _numScheduledScalings;
};

#endif // VMAINGRAPHICSVIEW_H
