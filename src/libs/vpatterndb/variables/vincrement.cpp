/************************************************************************
 **
 **  @file   vincrementtablerow.cpp
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

#include "vincrement.h"
#include "vincrement_p.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VIncrement create enpty increment
 */
VIncrement::VIncrement()
    :VVariable(), d(new VIncrementData)
{
    SetType(VarType::Increment);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VIncrementTableRow create increment
 * @param name increment's name
 * @param id id
 * @param base value
 * @param description description of increment
 */
VIncrement::VIncrement(VContainer *data, const QString &name, quint32 index, qreal base, const QString &formula,
                       bool ok, const QString description)
    :VVariable(name, base, description), d(new VIncrementData(data, index, formula, ok))
{
    SetType(VarType::Increment);
}

//---------------------------------------------------------------------------------------------------------------------
VIncrement::VIncrement(const VIncrement &incr)
    :VVariable(incr), d(incr.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VIncrement &VIncrement::operator=(const VIncrement &incr)
{
    if ( &incr == this )
    {
        return *this;
    }
    VVariable::operator=(incr);
    d = incr.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VIncrement::~VIncrement()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIndex return index of row
 * @return index
 */
quint32 VIncrement::getIndex() const
{
    return d->index;
}

//---------------------------------------------------------------------------------------------------------------------
QString VIncrement::GetFormula() const
{
    return d->formula;
}

//---------------------------------------------------------------------------------------------------------------------
bool VIncrement::IsFormulaOk() const
{
    return d->formulaOk;
}

//---------------------------------------------------------------------------------------------------------------------
VContainer *VIncrement::GetData()
{
    return &d->data;
}
