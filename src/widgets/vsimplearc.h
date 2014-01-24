/************************************************************************
 **
 **  @file   vsimplearc.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   7 1, 2014
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

#ifndef VSIMPLEARC_H
#define VSIMPLEARC_H

#include <QGraphicsPathItem>

namespace SimpleArc
{
    /**
     * @brief The Translation enum
     */
    enum Translation { FirstPoint, ForthPoint };
    Q_DECLARE_FLAGS(Translations, Translation)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( SimpleArc::Translations )

class VSimpleArc : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VSimpleArc(qint64 id, Qt::GlobalColor *currentColor, qreal *factor = 0, QObject *parent = 0);
signals:
    /**
     * @brief Choosed
     * @param id
     */
    void Choosed(qint64 id);
protected:
    /**
     * @brief mouseReleaseEvent  handle mouse release events.
     * @param event
     */
    virtual void    mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event
     */
    virtual void    hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event
     */
    virtual void    hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    Q_DISABLE_COPY(VSimpleArc)
    /**
     * @brief id
     */
    qint64          id;
    /**
     * @brief factor
     */
    qreal           *factor;
    /**
     * @brief currentColor
     */
    Qt::GlobalColor *currentColor;
};

#endif // VSIMPLEARC_H
