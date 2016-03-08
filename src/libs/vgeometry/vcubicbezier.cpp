/************************************************************************
 **
 **  @file   vcubicbezier.cpp
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

#include "vcubicbezier.h"
#include "vcubicbezier_p.h"

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier::VCubicBezier()
    : VAbstractCubicBezier(GOType::CubicBezier), d(new VCubicBezierData)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier::VCubicBezier(const VCubicBezier &curve)
    : VAbstractCubicBezier(curve), d(curve.d)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier::VCubicBezier(const VPointF &p1, const VPointF &p2, const VPointF &p3, const VPointF &p4, quint32 idObject,
                           Draw mode)
    : VAbstractCubicBezier(GOType::CubicBezier, idObject, mode), d(new VCubicBezierData(p1, p2, p3, p4))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier &VCubicBezier::operator=(const VCubicBezier &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VAbstractCubicBezier::operator=(curve);
    d = curve.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezier::GetP1() const
{
    return d->p1;
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezier::SetP1(const VPointF &p)
{
    d->p1 = p;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezier::GetP2() const
{
    return d->p2;
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezier::SetP2(const VPointF &p)
{
    d->p2 = p;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezier::GetP3() const
{
    return d->p3;
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezier::SetP3(const VPointF &p)
{
    d->p3 = p;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezier::GetP4() const
{
    return d->p4;
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezier::SetP4(const VPointF &p)
{
    d->p4 = p;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezier::GetStartAngle() const
{
    return QLineF(GetP1().toQPointF(), GetP2().toQPointF()).angle();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezier::GetEndAngle() const
{
    return QLineF(GetP4().toQPointF(), GetP3().toQPointF()).angle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return length of cubic bezier curve.
 * @return length.
 */
qreal VCubicBezier::GetLength() const
{
    return LengthBezier (GetP1().toQPointF(), GetP2().toQPointF(), GetP3().toQPointF(), GetP4().toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list with cubic bezier curve points.
 * @return list of points.
 */
QVector<QPointF> VCubicBezier::GetPoints() const
{
    return GetCubicBezierPoints(GetP1().toQPointF(), GetP2().toQPointF(), GetP3().toQPointF(), GetP4().toQPointF());
}
