/************************************************************************
 **
 **  @file   vmaingraphicsview.h
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

#ifndef VMAINGRAPHICSVIEW_H
#define VMAINGRAPHICSVIEW_H

#include <QGraphicsView>

/**
 * @brief The VMainGraphicsView class
 */
class VMainGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief VMainGraphicsView
     * @param parent
     */
    explicit VMainGraphicsView(QWidget *parent = 0);
signals:
    /**
     * @brief NewFactor
     * @param factor
     */
    void     NewFactor(qreal factor);
public slots:
    /**
     * @brief scalingTime
     * @param x
     */
    void     scalingTime(qreal x);
    /**
     * @brief animFinished
     */
    void     animFinished();
protected:
    /**
     * @brief wheelEvent обробник повороту колеса мишки.
     * @param event передається подія.
     */
    void     wheelEvent ( QWheelEvent * event );
    /**
     * @brief mousePressEvent
     * @param mousePress
     */
    void     mousePressEvent(QMouseEvent *mousePress);
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    void     mouseReleaseEvent(QMouseEvent *event);
private:
    /**
     * @brief _numScheduledScalings
     */
    qint32   _numScheduledScalings;
};

#endif // VMAINGRAPHICSVIEW_H
