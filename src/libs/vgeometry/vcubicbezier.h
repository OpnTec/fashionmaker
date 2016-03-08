/************************************************************************
 **
 **  @file   vcubicbezier.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VCUBICBEZIER_H
#define VCUBICBEZIER_H

#include "vabstractcubicbezier.h"
#include "vpointf.h"

class VCubicBezierData;

class VCubicBezier : public VAbstractCubicBezier
{
public:
    VCubicBezier();
    VCubicBezier(const VCubicBezier &curve);
    VCubicBezier(const VPointF &p1, const VPointF &p2, const VPointF &p3, const VPointF &p4, quint32 idObject = 0,
                 Draw mode = Draw::Calculation);
    VCubicBezier &operator=(const VCubicBezier &curve);

    virtual VPointF GetP1() const Q_DECL_OVERRIDE;
    void            SetP1(const VPointF &p);

    VPointF GetP2() const;
    void    SetP2(const VPointF &p);

    VPointF GetP3() const;
    void    SetP3(const VPointF &p);

    virtual VPointF GetP4() const Q_DECL_OVERRIDE;
    void            SetP4(const VPointF &p);

    virtual qreal            GetStartAngle() const Q_DECL_OVERRIDE;
    virtual qreal            GetEndAngle() const Q_DECL_OVERRIDE;
    virtual qreal            GetLength() const Q_DECL_OVERRIDE;
    virtual QVector<QPointF> GetPoints() const Q_DECL_OVERRIDE;

private:
    QSharedDataPointer<VCubicBezierData> d;
};

Q_DECLARE_TYPEINFO(VCubicBezier, Q_MOVABLE_TYPE);

#endif // VCUBICBEZIER_H
