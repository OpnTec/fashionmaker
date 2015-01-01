/************************************************************************
 **
 **  @file   vitem.h
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

#ifndef VITEM_H
#define VITEM_H

#include <QGraphicsPathItem>
#include <QObject>

/**
 * @brief VItem class detail on layout table.
 */
class VItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VItem ();
    VItem (int numInList, QGraphicsItem * parent = nullptr);
    VItem ( const QPainterPath & path, int numInList, QGraphicsItem * parent = nullptr );
    /**
     * @brief getPaper return pointer to paper sheet.
     * @return pointer to paper sheet.
     */
    QGraphicsRectItem *getPaper() const {return paper;}
    /**
     * @brief setPaper set pointer to paper sheet.
     * @param value pointer to paper sheet.
     */
    void        setPaper(QGraphicsRectItem *value) {paper = value;}
public slots:
    void        LengthChanged();
    void        SetIndexInList( qint32 index );
protected:
    QVariant    itemChange ( GraphicsItemChange change, const QVariant &value );
    void        checkItemChange ();
private:
    Q_DISABLE_COPY(VItem)
    /** @brief numInOutList index in list. */
    qint32      numInOutList;

    /** @brief paper pointer to paper item. */
    QGraphicsRectItem*    paper;
signals:
    /**
     * @brief itemOut emit if detail moved out paper. Detail send this signal each time when was moved.
     * @param numInOutList index in list.
     * @param flag true if moved out. false if not.
     */
    void        itemOut ( int numInOutList, bool flag );
    /**
     * @brief itemColliding emit if change create colission.
     * @param list list with all colission detalis.
     * @param number 1 - colission exist, 0 - colission doesn't exist.
     */
    void        itemColliding ( QList<QGraphicsItem *> list, int number );
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetIndexInList set detail index in list.
 * @param index index in list.
 */
inline void VItem::SetIndexInList(qint32 index)
{
    numInOutList = index;
}
#endif // VITEM_H
