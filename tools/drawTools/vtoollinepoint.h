/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#include "vtoolpoint.h"

class VToolLinePoint : public VToolPoint{
    Q_OBJECT
public:
                      VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                     const QString &typeLine, const QString &formula,
                                     const qint64 &basePointId, const qreal &angle, QGraphicsItem * parent = 0);
public slots:
    virtual void      ChangedActivDraw(const QString newName); 
protected:
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    virtual void      RefreshGeometry();
    virtual void      RemoveReferens();
private:
    Q_DISABLE_COPY(VToolLinePoint)
};

#endif // VTOOLLINEPOINT_H
