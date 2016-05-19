/************************************************************************
 **
 **  @file   vabstractarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
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

#include "vabstractarc.h"
#include "vabstractarc_p.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const GOType &type, const quint32 &idObject, const Draw &mode)
    : VAbstractCurve(type, idObject, mode), d (new VAbstractArcData())
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const GOType &type, const VPointF &center, qreal f1, const QString &formulaF1, qreal f2,
                           const QString &formulaF2, quint32 idObject, Draw mode)
    : VAbstractCurve(type, idObject, mode), d (new VAbstractArcData(center, f1, formulaF1, f2, formulaF2))
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const GOType &type, const VPointF &center, qreal f1, qreal f2, quint32 idObject, Draw mode)
    : VAbstractCurve(type, idObject, mode), d (new VAbstractArcData(center, f1, f2))
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const GOType &type, const QString &formulaLength, const VPointF &center,
                           qreal f1, const QString &formulaF1, quint32 idObject, Draw mode)
    : VAbstractCurve(type, idObject, mode), d (new VAbstractArcData(formulaLength, center, f1, formulaF1))
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const GOType &type, const VPointF &center, qreal f1, quint32 idObject, Draw mode)
    : VAbstractCurve(type, idObject, mode), d (new VAbstractArcData(center, f1))
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::VAbstractArc(const VAbstractArc &arc)
    : VAbstractCurve(arc), d (arc.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc &VAbstractArc::operator=(const VAbstractArc &arc)
{
    if ( &arc == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(arc);
    d = arc.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArc::~VAbstractArc()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractArc::GetFormulaF1() const
{
    return d->formulaF1;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetFormulaF1(const QString &formula, qreal value)
{
    d->formulaF1 = formula;
    d->f1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractArc::GetStartAngle() const
{
    return d->f1;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractArc::GetFormulaF2() const
{
    return d->formulaF2;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetFormulaF2(const QString &formula, qreal value)
{
    d->formulaF2 = formula;
    d->f2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractArc::GetEndAngle() const
{
    return d->f2;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VAbstractArc::GetCenter() const
{
    return d->center;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetCenter(const VPointF &point)
{
    d->center = point;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractArc::GetFormulaLength() const
{
    return d->formulaLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetFormulaLength(const QString &formula, qreal value)
{
    d->formulaLength = formula;
    FindF2(value);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::setId(const quint32 &id)
{
    VAbstractCurve::setId(id);
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractArc::NameForHistory(const QString &toolName) const
{
    QString name = toolName + QString(" %1").arg(GetCenter().name());

    if (VAbstractCurve::id() != NULL_ID)
    {
        name += QString("_%1").arg(VAbstractCurve::id());
    }

    if (GetDuplicate() > 0)
    {
        name += QString("_%1").arg(GetDuplicate());
    }
    return name;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractArc::IsFlipped() const
{
    return d->isFlipped;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractArc::AngleArc() const
{
    {
        const qreal angleDiff = qAbs(GetStartAngle() - GetEndAngle());
        if (VFuzzyComparePossibleNulls(angleDiff, 0) || VFuzzyComparePossibleNulls(angleDiff, 360))
        {
            return 360;
        }
    }
    QLineF l1(0, 0, 100, 0);
    l1.setAngle(GetStartAngle());
    QLineF l2(0, 0, 100, 0);
    l2.setAngle(GetEndAngle());

    qreal ang = l1.angleTo(l2);

    if (IsFlipped())
    {
        ang = 360 - ang;
    }

    return ang;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetFlipped(bool value)
{
    d->isFlipped = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractArc::SetFormulaLength(const QString &formula)
{
    d->formulaLength = formula;
}
