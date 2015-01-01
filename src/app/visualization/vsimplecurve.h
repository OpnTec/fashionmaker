/************************************************************************
 **
 **  @file   vsimplecurve.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 12, 2013
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

#ifndef VSIMPLECURVE_H
#define VSIMPLECURVE_H

#include <QGraphicsPathItem>
#include "../geometry/vabstractcurve.h"

enum class SimpleCurvePoint : char { FirstPoint, ForthPoint };

/**
 * @brief The VSimpleSpline class for simple spline. This object used when we cut spline and want show peaces.
 */
class VSimpleCurve : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VSimpleCurve(quint32 id, Qt::GlobalColor *currentColor, SimpleCurvePoint curvePosition,
                 qreal *factor = nullptr, QObject *parent = 0);
    void            ChangedActivDraw(const bool &flag);
    virtual void    paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual int     type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::SimpleSplinePath)};
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id spline id.
     */
    void            Choosed(quint32 id);
    void            HoverPath(quint32 id, SimpleCurvePoint curvePosition, PathDirection direction);
protected:
    virtual void    mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void    hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void    hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    Q_DISABLE_COPY(VSimpleCurve)
    /** @brief id spline id. */
    quint32           id;

    /** @brief factor scale factor. */
    qreal             *factor;

    /** @brief currentColor current color. */
    Qt::GlobalColor   *currentColor;

    SimpleCurvePoint  curvePosition;
};

#endif // VSIMPLECURVE_H
