/************************************************************************
 **
 **  @file   vsimplearc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

enum class SimpleArcPoint : char { FirstPoint, ForthPoint };

/**
 * @brief The VSimpleArc class for simple arc. This object used when we cut arc and want show peaces.
 */
class VSimpleArc : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VSimpleArc(quint32 id, Qt::GlobalColor *currentColor, qreal *factor = nullptr, QObject *parent = nullptr);
    void            ChangedActivDraw(const bool &flag);
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id arc id.
     */
    void Choosed(quint32 id);
protected:
    virtual void    mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void    hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void    hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    Q_DISABLE_COPY(VSimpleArc)
    /** @brief id arc id. */
    quint32          id;

    /** @brief factor scale factor. */
    qreal           *factor;

    /** @brief currentColor current color. */
    Qt::GlobalColor *currentColor;
};

#endif // VSIMPLEARC_H
