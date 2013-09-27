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

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"
#include <QVector>
#include <QString>
#include <QPainterPath>

namespace Detail {
    enum Contour { OpenContour, CloseContour };
    Q_DECLARE_FLAGS(Contours, Contour)

    enum Equidistant { OpenEquidistant, CloseEquidistant };
    Q_DECLARE_FLAGS(Equidistants, Equidistant)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Contours)
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Equidistants)

class VDetail
{
public:
    VDetail();
    VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
    void append(const VNodeDetail &node);
    void Clear();
    qint32 CountNode() const;
    bool Containes(const qint64 &id)const;
    VNodeDetail & operator[](int indx);
    QString getName() const;
    void setName(const QString &value);

    qreal getMx() const;
    void setMx(const qreal &value);

    qreal getMy() const;
    void setMy(const qreal &value);

private:
    QVector<VNodeDetail> nodes;
    QString name;
    qreal mx;
    qreal my;
};

#endif // VDETAIL_H
