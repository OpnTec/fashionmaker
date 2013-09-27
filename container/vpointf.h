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

#ifndef VPOINTF_H
#define VPOINTF_H

#include <QPointF>
#include <QString>
#include "options.h"

class VPointF
{
public:
    VPointF();
    VPointF (const VPointF &point );
    VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my, Draw::Draws mode = Draw::Calculation,
              qint64 idObject = 0);
    VPointF &operator=(const VPointF &point);
    ~VPointF();
    QString name() const;
    qreal mx() const;
    qreal my() const;
    void setName(const QString &name);
    void setMx(qreal mx);
    void setMy(qreal my);
    QPointF toQPointF()const;
    qreal x() const;
    void setX(const qreal &value);
    qreal y() const;
    void setY(const qreal &value);
    qint32 referens() const;
    void incrementReferens();
    void decrementReferens();
    Draw::Draws getMode() const;
    void setMode(const Draw::Draws &value);

    qint64 getIdObject() const;
    void setIdObject(const qint64 &value);

private:
    QString _name;
    qreal _mx;
    qreal _my;
    qreal _x;
    qreal _y;
    qint32 _referens;
    Draw::Draws mode;
    qint64 idObject;
};

#endif // VPOINTF_H
