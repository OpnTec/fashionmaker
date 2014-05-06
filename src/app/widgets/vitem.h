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
    /**
     * @brief VItem default constructor.
     */
                VItem ();
    /**
     * @brief VItem constructor.
     * @param numInList index in list of details.
     * @param parent parent object.
     */
                VItem (int numInList, QGraphicsItem * parent = nullptr);
    /**
     * @brief VItem constructor.
     * @param path detail path.
     * @param numInList index in list of details.
     * @param parent parent object.
     */
                VItem ( const QPainterPath & path, int numInList, QGraphicsItem * parent = nullptr );
    /**
     * @brief Rotate rotate detail on angle in degree.
     * @param angle angle in degree.
     */
    void        Rotate ( qreal angle );
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
    /**
     * @brief LengthChanged handle signal change paper length.
     */
    void        LengthChanged();
    /**
     * @brief SetIndexInList set detail index in list.
     * @param index index in list.
     */
    void        SetIndexInList( qint32 index );
protected:
    /**
     * @brief itemChange handle item change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    QVariant    itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief checkItemChange change item change. If detail create colission or moved out paper emit signal.
     */
    void        checkItemChange ();
private:
    Q_DISABLE_COPY(VItem)
    /**
     * @brief numInOutList index in list.
     */
    qint32      numInOutList;
    /**
     * @brief paper pointer to paper item.
     */
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

inline void VItem::SetIndexInList(qint32 index)
{
    numInOutList = index;
}
#endif // VITEM_H
