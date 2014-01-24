/************************************************************************
 **
 **  @file   vtoollinepoint.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#include "vtoolpoint.h"

/**
 * @brief The VToolLinePoint class
 */
class VToolLinePoint : public VToolPoint
{
    Q_OBJECT
public:
                      /**
                       * @brief VToolLinePoint
                       * @param doc dom document container
                       * @param data container with variables
                       * @param id object id in container
                       * @param typeLine
                       * @param formula
                       * @param basePointId
                       * @param angle
                       * @param parent parent object
                       */
                      VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                                     const QString &formula, const qint64 &basePointId, const qreal &angle,
                                     QGraphicsItem * parent = 0);
public slots:
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void      ChangedActivDraw(const QString &newName);
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void      SetFactor(qreal factor);
protected:
    /**
     * @brief typeLine
     */
    QString           typeLine;
    /**
     * @brief formula
     */
    QString           formula;
    /**
     * @brief angle
     */
    qreal             angle;
    /**
     * @brief basePointId
     */
    qint64            basePointId;
    /**
     * @brief mainLine
     */
    QGraphicsLineItem *mainLine;
    /**
     * @brief RefreshGeometry
     */
    virtual void      RefreshGeometry();
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void      RemoveReferens() {doc->DecrementReferens(basePointId);}
private:
    Q_DISABLE_COPY(VToolLinePoint)
};

#endif // VTOOLLINEPOINT_H
