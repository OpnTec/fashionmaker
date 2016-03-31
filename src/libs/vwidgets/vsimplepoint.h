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

#include <QGraphicsEllipseItem>

#include "vabstractsimple.h"

class VPointF;
class VGraphicsSimpleTextItem;

class VSimplePoint : public VAbstractSimple, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VSimplePoint(quint32 id, const QColor &currentColor, Unit patternUnit, qreal *factor = nullptr,
                 QObject *parent = nullptr);
    virtual ~VSimplePoint() Q_DECL_OVERRIDE;

    virtual void ChangedActivDraw(const bool &flag) Q_DECL_OVERRIDE;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::SimplePoint)};

    void RefreshLine();
    void RefreshGeometry(const VPointF &point);
    void SetEnabled(bool enabled);
    void EnableToolMove(bool move);
    void AllowLabelHover(bool enabled);
    void AllowLabelSelecting(bool enabled);
    void ToolSelectionType(const SelectionType &type);

    QColor GetCurrentColor() const;
    void   SetCurrentColor(const QColor &value);
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id point id.
     */
    void Choosed(quint32 id);
    void ShowContextMenu(QGraphicsSceneContextMenuEvent * event);
    void Delete();
    void NameChangedPosition(const QPointF &pos);

public slots:
    void DeleteFromLabel();
    void PointChoosed();
    void ChangedPosition(const QPointF &pos);
    void ContextMenu(QGraphicsSceneContextMenuEvent * event);

protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VSimplePoint)

    /** @brief radius radius circle. */
    qreal                    radius;

    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *lineName;

    SelectionType selectionType;

};

#endif // VSIMPLEPOINT_H
