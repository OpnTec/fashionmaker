/************************************************************************
 **
 **  @file   vpointf.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VPOINTF_H
#define VPOINTF_H

#include "vgobject.h"

class QPointF;
class QString;
class VPointFData;

/**
 * @brief The VPointF class keep data of point.
 */
class VPointF:public VGObject
{
public:
    VPointF ();
    VPointF (const VPointF &point );
    VPointF (const QPointF &point );
    VPointF (qreal x, qreal y, const QString &name, qreal mx, qreal my, quint32 idObject = 0,
              const Draw &mode = Draw::Calculation);
    VPointF (const QPointF &point, const QString &name, qreal mx, qreal my, quint32 idObject = 0,
              const Draw &mode = Draw::Calculation);
    virtual ~VPointF() Q_DECL_OVERRIDE;
    VPointF &operator=(const VPointF &point);
    qreal   mx() const;
    qreal   my() const;
    void    setMx(qreal mx);
    void    setMy(qreal my);
    QPointF toQPointF()const;
    qreal   x() const;
    void    setX(const qreal &value);
    qreal   y() const;
    void    setY(const qreal &value);
private:
    QSharedDataPointer<VPointFData> d;
};

#endif // VPOINTF_H
