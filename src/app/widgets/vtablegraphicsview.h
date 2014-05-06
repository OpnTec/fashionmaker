/************************************************************************
 **
 **  @file   vtablegraphicsview.h
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

#ifndef VTABLEGRAPHICSVIEW_H
#define VTABLEGRAPHICSVIEW_H

#include <QGraphicsView>

/**
 * @brief The VTableGraphicsView class table layout view.
 */
class VTableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief The typeMove_e enum type items moves.
     */
    enum typeMove_e { Left, Right, Up, Down };

    VTableGraphicsView(QGraphicsScene* pScene, QWidget *parent = nullptr);
signals:
    /**
     * @brief itemChect emit after change selection state detail.
     * @param flag false - selected, true - don't selected.
     */
    void        itemChect( bool flag );
public slots:
    /**
     * @brief selectionChanged handle selection change.
     */
    void        selectionChanged();
    /**
     * @brief rotateItems handle rotatation detail.
     */
    void        rotateItems();
    /**
     * @brief MirrorItem mirror detail.
     */
    void        MirrorItem();
    /**
     * @brief ZoomIn zoom in paper scale.
     */
    void        ZoomIn();
    /**
     * @brief ZoomOut zoom out paper scale.
     */
    void        ZoomOut();
protected:
    /**
     * @brief wheelEvent handle wheel events.
     * @param event wheel event.
     */
    void        wheelEvent ( QWheelEvent * event );
    /**
     * @brief mousePressEvent handle mouse press events.
     * @param mousePress mouse press event.
     */
    void        mousePressEvent(QMouseEvent *mousePress);
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    void        mouseReleaseEvent ( QMouseEvent * event );
    /**
     * @brief keyPressEvent handle key press events.
     * @param event key press event.
     */
    void        keyPressEvent ( QKeyEvent * event );
    /**
     * @brief rotateIt rotate selected details on 180 degree.
     */
    void        rotateIt();
    /**
     * @brief MoveItem move selected detail.
     * @param move type of move.
     */
    void        MoveItem( VTableGraphicsView::typeMove_e move );
    
};

inline void VTableGraphicsView::rotateItems()
{
    rotateIt();
}

inline void VTableGraphicsView::ZoomIn()
{
    scale(1.1, 1.1);
}

inline void VTableGraphicsView::ZoomOut()
{
    scale(1/1.1, 1/1.1);
}

#endif // VTABLEGRAPHICSVIEW_H
