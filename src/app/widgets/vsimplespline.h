/************************************************************************
 **
 **  @file   vsimplespline.h
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

#ifndef VSIMPLESPLINE_H
#define VSIMPLESPLINE_H

#include <QGraphicsPathItem>

namespace SimpleSpline
{
    /**
     * @brief The Translation enum position point.
     */
    enum Translation { FirstPoint, ForthPoint };
    Q_DECLARE_FLAGS(Translations, Translation)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( SimpleSpline::Translations )

/**
 * @brief The VSimpleSpline class for simple spline. This object used when we cut spline and want show peaces.
 */
class VSimpleSpline : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    /**
     * @brief VSimpleSpline constructor.
     * @param id spline id.
     * @param factor scene scale factor.
     * @param currentColor current color.
     * @param parent parent object.
     */
    VSimpleSpline(quint32 id, Qt::GlobalColor *currentColor, qreal *factor = nullptr, QObject *parent = 0);
    void ChangedActivDraw(const bool &flag);
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id spline id.
     */
    void Choosed(quint32 id);
protected:
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    virtual void    mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void    hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void    hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    Q_DISABLE_COPY(VSimpleSpline)
    /**
     * @brief id spline id.
     */
    quint32          id;
    /**
     * @brief factor scale factor.
     */
    qreal           *factor;
    /**
     * @brief currentColor current color.
     */
    Qt::GlobalColor *currentColor;
};

#endif // VSIMPLESPLINE_H
