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
 * @brief The VMainGraphicsView class main scene view.
 */
class VMainGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief VMainGraphicsView constructor.
     * @param parent parent object.
     */
    explicit VMainGraphicsView(QWidget *parent = nullptr);
signals:
    /**
     * @brief NewFactor send new scale factor.
     * @param factor scene scale factor.
     */
    void     NewFactor(qreal factor);
public slots:
    /**
     * @brief scalingTime call each time when need handle scaling.
     * @param x
     */
    void     scalingTime(qreal x);
    /**
     * @brief scrollingTime call each time when need handle scrolling.
     * @param x
     */
    void     scrollingTime(qreal x);
    /**
     * @brief animFinished
     */
    void     animFinished();
protected:
    /**
     * @brief wheelEvent handle wheel events.
     * @param event wheel event.
     */
    void     wheelEvent ( QWheelEvent * event );
    /**
     * @brief mousePressEvent handle mouse press events.
     * @param mousePress mouse press event.
     */
    void     mousePressEvent(QMouseEvent *mousePress);
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    void     mouseReleaseEvent(QMouseEvent *event);
private:
    /**
     * @brief _numScheduledScalings keep number scheduled scalings.
     */
    qint32   _numScheduledScalings;
};

#endif // VMAINGRAPHICSVIEW_H
