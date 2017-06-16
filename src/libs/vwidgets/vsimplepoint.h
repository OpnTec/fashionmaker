/************************************************************************
 **
 **  @file   vsimplepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VSIMPLEPOINT_H
#define VSIMPLEPOINT_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "vabstractsimple.h"
#include "../vwidgets/vscenepoint.h"

class VGraphicsSimpleTextItem;
class VPointF;

class VSimplePoint : public VAbstractSimple, public VScenePoint
{
    Q_OBJECT
public:
    VSimplePoint(quint32 id, const QColor &currentColor, QObject *parent = nullptr);
    virtual ~VSimplePoint() = default;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::SimplePoint)};

    using VScenePoint::SetOnlyPoint;
    using VScenePoint::IsOnlyPoint;

    void SetVisualizationMode(bool value);
    bool IsVisualizationMode() const;

    void SetPointHighlight(bool value);

    void SetEnabled(bool enabled);
    void EnableToolMove(bool move);
    void AllowLabelHover(bool enabled);
    void AllowLabelSelecting(bool enabled);
    virtual void ToolSelectionType(const SelectionType &type) Q_DECL_OVERRIDE;
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id point id.
     */
    void Choosed(quint32 id);
    void Selected(bool selected, quint32 id);
    void NameChangedPosition(const QPointF &pos, quint32 id);

public slots:
    void DeleteFromLabel();
    void PointChoosed();
    void PointSelected(bool selected);
    void ChangedPosition(const QPointF &pos);

protected:
    virtual void     mousePressEvent( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void     hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    virtual void     hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    virtual void     keyReleaseEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VSimplePoint)

    bool m_visualizationMode;
    bool m_alwaysHovered;
};

#endif // VSIMPLEPOINT_H
