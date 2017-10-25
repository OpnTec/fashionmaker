/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "../vmisc/def.h"

class VGraphicsSimpleTextItem;
class VPointF;
class VScaledLine;

class VScenePoint: public QGraphicsEllipseItem
{
public:
    explicit VScenePoint(QGraphicsItem *parent = nullptr);
    virtual ~VScenePoint() = default;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ScenePoint)};

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) Q_DECL_OVERRIDE;
    virtual void RefreshPointGeometry(const VPointF &point);

    void RefreshLine();

protected:
    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *m_namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    VScaledLine             *m_lineName;

    bool m_onlyPoint;
    bool m_isHovered;
    bool m_showLabel;

    /** @brief m_baseColor base color of point. */
    QColor m_baseColor;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

    void SetOnlyPoint(bool value);
    bool IsOnlyPoint() const;
private:
    Q_DISABLE_COPY(VScenePoint)

    void ScaleMainPenWidth(qreal scale);
};

#endif // VSCENEPOINT_H
