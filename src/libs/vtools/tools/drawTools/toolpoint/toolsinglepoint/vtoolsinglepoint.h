/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include <qcompilerdetection.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vabstractpoint.h"
#include "../vmisc/def.h"

template <class T> class QSharedPointer;

/**
 * @brief The VToolSinglePoint class parent for all tools what create points.
 */
class VToolSinglePoint: public VAbstractPoint, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolSinglePoint(VAbstractPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VToolSinglePoint() Q_DECL_OVERRIDE;

    virtual int   type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::SinglePoint)};

    QString                 name() const;
    void                    setName(const QString &name);

    void                    SetEnabled(bool enabled);

    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    void                    NameChangePosition(const QPointF &pos);
    virtual void            SetFactor(qreal factor) Q_DECL_OVERRIDE;
    virtual void            Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    virtual void            EnableToolMove(bool move) Q_DECL_OVERRIDE;
    void                    PointChoosed();
    void                    PointSelected(bool selected);
    virtual void            FullUpdateFromFile() Q_DECL_OVERRIDE;
    virtual void            DoChangePosition(quint32 id, qreal mx, qreal my) Q_DECL_OVERRIDE;
    virtual void            AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void            AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    void                    AllowLabelHover(bool enabled);
    void                    AllowLabelSelecting(bool enabled);
    virtual void            ToolSelectionType(const SelectionType &type) Q_DECL_OVERRIDE;
protected:
    /** @brief radius radius circle. */
    qreal                   radius;

    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *lineName;

    virtual void            UpdateNamePosition(quint32 id) Q_DECL_OVERRIDE;
    virtual void            mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void            hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void            RefreshPointGeometry(const VPointF &point);
    virtual void            RefreshLine(quint32 id) Q_DECL_OVERRIDE;
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void            keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void            SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolSinglePoint)
};

#endif // VTOOLSINGLEPOINT_H
