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

#ifndef VMODELINGLINEPOINT_H
#define VMODELINGLINEPOINT_H

#include "vmodelingpoint.h"

class VModelingLinePoint : public VModelingPoint{
    Q_OBJECT
public:
                      VModelingLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                                         const QString &formula, const qint64 &basePointId, const qreal &angle,
                                         QGraphicsItem * parent = 0);
protected:
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    virtual void      RefreshGeometry();
    virtual void      RemoveReferens() {doc->DecrementReferens(basePointId);}
private:
    Q_DISABLE_COPY(VModelingLinePoint)
};

#endif // VMODELINGLINEPOINT_H
