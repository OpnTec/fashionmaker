/************************************************************************
 **
 **  @file   vtoolpoint.h
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

#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#include "vdrawtool.h"
#include <QGraphicsEllipseItem>

class VPointF;
class VGraphicsSimpleTextItem;

/**
 * @brief The VToolPoint class parent for all tools what create points.
 */
class VToolPoint: public VDrawTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolPoint(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VToolPoint();
    virtual void            paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QString                 name() const;
    void                    setName(const QString &name);
    virtual QString         getTagName() const;
    static const QString    TagName;

    void                    setEnabled(bool enabled);
public slots:
    void                    NameChangePosition(const QPointF &pos);
    virtual void            ShowTool(quint32 id, bool enable);
    virtual void            SetFactor(qreal factor);
    virtual void            Disable(bool disable, const QString &namePP);
    void                    DeleteFromLabel();
    virtual void            EnableToolMove(bool move);
    void                    PointChoosed();
    virtual void            FullUpdateFromFile();
protected:
    /** @brief radius radius circle. */
    qreal                   radius;

    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *lineName;

    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void            keyReleaseEvent(QKeyEvent * event);
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);

    template <typename T>
    void ShowToolVisualization(bool show)
    {
        if (show)
        {
            if (vis == nullptr)
            {
                AddVisualization<T>();
                SetVisualization();
            }
            else
            {
                if (T *visual = qobject_cast<T *>(vis))
                {
                    visual->show();
                }
            }
        }
        else
        {
            delete vis;
            vis = nullptr;
        }
    }
private:
    Q_DISABLE_COPY(VToolPoint)
};

#endif // VTOOLPOINT_H
