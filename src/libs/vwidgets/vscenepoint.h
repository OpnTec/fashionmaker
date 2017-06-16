/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef VSCENEPOINT_H
#define VSCENEPOINT_H

#include <QtGlobal>
#include <QGraphicsEllipseItem>

class VGraphicsSimpleTextItem;
class VPointF;

class VScenePoint: public QGraphicsEllipseItem
{
public:
    explicit VScenePoint(QGraphicsItem *parent = nullptr);
    ~VScenePoint() = default;

    virtual void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget = nullptr) Q_DECL_OVERRIDE;

    virtual void   RefreshPointGeometry(const VPointF &point);
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *m_namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *m_lineName;

    bool m_onlyPoint;
    bool m_isHovered;

    /** @brief m_baseColor base color of point. */
    QColor m_baseColor;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

    void RefreshLine();
    void ScaleLinePenWidth(QGraphicsLineItem *line, qreal scale);

    void SetOnlyPoint(bool value);
    bool IsOnlyPoint() const;
private:
    Q_DISABLE_COPY(VScenePoint)

    void ScaleMainPenWidth(qreal scale);
    void ScaleLabelFontSize(qreal scale) const;
};

#endif // VSCENEPOINT_H
