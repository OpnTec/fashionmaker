/************************************************************************
 **
 **  @file   vincrementtablerow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "../vmisc/def.h"
#include "vvariable.h"
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
 */
VIncrement::VIncrement(VContainer *data, const QString &name, IncrementType incrType)
    :VVariable(name, QString()), d(new VIncrementData(data, incrType))
{
    incrType == IncrementType::Separator ? SetType(VarType::IncrementSeparator) : SetType(VarType::Increment);
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

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VIncrement::VIncrement(const VIncrement &&incr) Q_DECL_NOTHROW
    :VVariable(incr), d(incr.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VIncrement &VIncrement::operator=(VIncrement &&incr) Q_DECL_NOTHROW
{
    VVariable::operator=(incr);
    std::swap(d, incr.d);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
VIncrement::~VIncrement()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIndex return index of row. Row index for increments and preview calucalations is unique. Check type before
 * using.
 * @return index
 */
quint32 VIncrement::GetIndex() const
{
    return d->index;
}

//---------------------------------------------------------------------------------------------------------------------
void VIncrement::SetFormula(qreal base, const QString &formula, bool ok)
{
    VInternalVariable::SetValue(base);
    d->formula = formula;
    d->formulaOk = ok;
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
void VIncrement::SetIndex(quint32 index)
{
    d->index = index;
}

//---------------------------------------------------------------------------------------------------------------------
VContainer *VIncrement::GetData()
{
    return d->data.data();
}

//---------------------------------------------------------------------------------------------------------------------
IncrementType VIncrement::GetIncrementType() const
{
    return d->incrType;
}

//---------------------------------------------------------------------------------------------------------------------
bool VIncrement::IsPreviewCalculation() const
{
    return d->previewCalculation;
}

//---------------------------------------------------------------------------------------------------------------------
void VIncrement::SetPreviewCalculation(bool value)
{
    d->previewCalculation = value;
}
