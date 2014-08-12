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

    VTableGraphicsView(QWidget *parent = nullptr);
signals:
    /**
     * @brief itemChect emit after change selection state detail.
     * @param flag false - selected, true - don't selected.
     */
    void        itemChect( bool flag );
public slots:
    void        selectionChanged();
    void        rotateItems();
    void        MirrorItem();
    void        ZoomIn();
    void        ZoomOut();
protected:
    void        wheelEvent ( QWheelEvent * event );
    void        mousePressEvent(QMouseEvent *mousePress);
    void        mouseReleaseEvent ( QMouseEvent * event );
    void        keyPressEvent ( QKeyEvent * event );
    void        rotateIt();
    void        MoveItem( VTableGraphicsView::typeMove_e move );
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief rotateItems handle rotatation detail.
 */
inline void VTableGraphicsView::rotateItems()
{
    rotateIt();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ZoomIn zoom in paper scale.
 */
inline void VTableGraphicsView::ZoomIn()
{
    scale(1.1, 1.1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ZoomOut zoom out paper scale.
 */
inline void VTableGraphicsView::ZoomOut()
{
    scale(1/1.1, 1/1.1);
}

#endif // VTABLEGRAPHICSVIEW_H
