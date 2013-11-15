/************************************************************************
 **
 **  @file   vtablegraphicsview.h
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

#ifndef VTABLEGRAPHICSVIEW_H
#define VTABLEGRAPHICSVIEW_H

#include <QGraphicsView>

class VTableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    enum typeMove_e { Left, Right, Up, Down };
                VTableGraphicsView(QGraphicsScene* pScene, QWidget *parent = 0);
signals:
    /**
     * @brief itemChect Сигнал, що посилається коли змінюється стан вибору деталі.
     * @param flag Зберігає стан вибору деталі: false - знайдено, true - не знайдено.
     */
    void        itemChect( bool flag );
public slots:
    /**
     * @brief selectionChanged Слот виконується при зміні стану вибору деталей.
     */
    void        selectionChanged();
    /**
     * @brief rotateItems слот, який виконується при натисненні кнопки повороту деталі.
     */
    inline void rotateItems() {rotateIt();}
    /**
     * @brief MirrorItem дзеркалить об'єкт відносно вертикальної вісі семетрії об'єкта.
     */
    void        MirrorItem();
    /**
     * @brief ZoomIn збільшує масштаб листа.
     */
    inline void ZoomIn() {scale(1.1, 1.1);}
    /**
     * @brief ZoomOut зменшує масштаб листа.
     */
    inline void ZoomOut() {scale(1/1.1, 1/1.1);}
protected:
    /**
     * @brief wheelEvent обробник повороту колеса мишки.
     * @param event передається подія.
     */
    void        wheelEvent ( QWheelEvent * event );
    /**
     * @brief mousePressEvent обробник натиснення кнопки миші.
     * @param mousePress передається подія.
     */
    void        mousePressEvent(QMouseEvent *mousePress);
    /**
     * @brief mouseReleaseEvent обробник відпускання кнопки миші.
     * @param event передається подія
     */
    void        mouseReleaseEvent ( QMouseEvent * event );
    /**
     * @brief keyPressEvent обробник натиснення клавіші.
     * @param event передається подія.
     */
    void        keyPressEvent ( QKeyEvent * event );
    /**
     * @brief rotateIt виконує поворот вибраних деталей на 180 градусів.
     */
    void        rotateIt();
    /**
     * @brief MoveItem переміщує виділені об'єкти сцени.
     * @param move напрямок переміщення.
     */
    void        MoveItem( VTableGraphicsView::typeMove_e move );
    
};

#endif // VTABLEGRAPHICSVIEW_H
