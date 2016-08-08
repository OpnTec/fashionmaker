/************************************************************************
 **
 **  @file   vgraphicssimpletextitem.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vmisc/def.h"

class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QPointF;

/**
 * @brief The VGraphicsSimpleTextItem class pointer label.
 */
class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
    explicit VGraphicsSimpleTextItem(QGraphicsItem *parent = nullptr);
    explicit VGraphicsSimpleTextItem( const QString & text, QGraphicsItem *parent = nullptr );
    virtual ~VGraphicsSimpleTextItem() Q_DECL_OVERRIDE;

    qint32       FontSize()const;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::GraphicsSimpleTextItem)};

    void setEnabled(bool enabled);
    void LabelSelectionType(const SelectionType &type);
signals:
    /**
     * @brief NameChangePosition emit when label change position.
     * @param pos new posotion.
     */
    void         NameChangePosition(const QPointF &pos);
    /**
     * @brief ShowContextMenu emit when need show tool context menu.
     * @param event context menu event.
     */
    void         ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
    void         DeleteTool();
    void         PointChoosed();
    void         PointSelected(bool selected);
protected:
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent *event ) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent *event ) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent *event ) Q_DECL_OVERRIDE;
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;
private:
    /** @brief fontSize label font size. */
    qint32        fontSize;
    SelectionType selectionType;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FontSize return label font size.
 * @return font size.
 */
inline qint32 VGraphicsSimpleTextItem::FontSize() const
{
    return fontSize;
}

#endif // VGRAPHICSSIMPLETEXTITEM_H
