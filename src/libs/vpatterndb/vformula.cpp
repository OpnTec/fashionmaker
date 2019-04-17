/************************************************************************
 **
 **  @file   vformula.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
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

#include "vformula.h"
#include "vformula_p.h"

#include <qnumeric.h>
#include <QMessageLogger>
#include <QScopedPointer>
#include <QtDebug>

#include "../qmuparser/qmuparsererror.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "calculator.h"
#include "vcontainer.h"
#include "vtranslatevars.h"

//VFormula
//---------------------------------------------------------------------------------------------------------------------
VFormula::VFormula()
    : d(new VFormulaData)
{}

//---------------------------------------------------------------------------------------------------------------------
VFormula::VFormula(const QString &formula, const VContainer *container)
    : d(new VFormulaData(formula, container))
{}

//---------------------------------------------------------------------------------------------------------------------
VFormula &VFormula::operator=(const VFormula &formula)
{
    if ( &formula == this )
    {
        return *this;
    }
    d = formula.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VFormula::VFormula(const VFormula &formula)
    : d (formula.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VFormula::~VFormula()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VFormula::operator==(const VFormula &formula) const
{
    return d->formula == formula.GetFormula() && d->strValue == formula.getStringValue() &&
            d->checkZero == formula.getCheckZero() && d->checkLessThanZero == formula.getCheckLessThanZero() &&
            d->data == formula.getData() && d->toolId == formula.getToolId() && d->postfix == formula.getPostfix() &&
            d->error == formula.error() && VFuzzyComparePossibleNulls(d->dValue, formula.getDoubleValue());
}

//---------------------------------------------------------------------------------------------------------------------
bool VFormula::operator!=(const VFormula &formula) const
{
    return !VFormula::operator==(formula);
}

//---------------------------------------------------------------------------------------------------------------------
QString VFormula::GetFormula(FormulaType type) const
{
    if (type == FormulaType::ToUser)
    {
        return qApp->TrVars()->TryFormulaToUser(d->formula, qApp->Settings()->GetOsSeparator());
    }
    else
    {
        return d->formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::SetFormula(const QString &value, FormulaType type)
{
    if (d->formula != value)
    {
        if (type == FormulaType::FromUser)
        {
            d->formula = qApp->TrVars()->FormulaFromUser(value, qApp->Settings()->GetOsSeparator());
        }
        else
        {
            d->formula = value;
        }

        ResetState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VFormula::getStringValue() const
{
    return d->strValue;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VFormula::getDoubleValue() const
{
    return d->dValue;
}

//---------------------------------------------------------------------------------------------------------------------
bool VFormula::getCheckZero() const
{
    return d->checkZero;
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::setCheckZero(bool value)
{
    d->checkZero = value;
    ResetState();
}

//---------------------------------------------------------------------------------------------------------------------
bool VFormula::getCheckLessThanZero() const
{
    return d->checkLessThanZero;
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::setCheckLessThanZero(bool value)
{
    d->checkLessThanZero = value;
    ResetState();
}

//---------------------------------------------------------------------------------------------------------------------
const VContainer *VFormula::getData() const
{
    return d->data;
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::setData(const VContainer *value)
{
    if (d->data != value && value != nullptr)
    {
        d->data = value;
        ResetState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VFormula::getToolId() const
{
    return d->toolId;
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::setToolId(quint32 value)
{
    d->toolId = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VFormula::getPostfix() const
{
    return d->postfix;
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::setPostfix(const QString &value)
{
    d->postfix = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VFormula::error() const
{
    return d->error;
}

//---------------------------------------------------------------------------------------------------------------------
QString VFormula::Reason() const
{
    return d->reason;
}

//---------------------------------------------------------------------------------------------------------------------
int VFormula::FormulaTypeId()
{
    return qMetaTypeId<VFormula>();
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::Eval()
{
    ResetState();

    if (d->data == nullptr)
    {
        d->reason = tr("Data container is empty");
        return;
    }

    qreal result = 0;

    if (not d->formula.isEmpty())
    {
        try
        {
            QScopedPointer<Calculator> cal(new Calculator());
            result = cal->EvalFormula(d->data->DataVariables(), d->formula);
        }
        catch (qmu::QmuParserError &e)
        {
            d->reason = tr("Math parser error: %1").arg(e.GetMsg());
            return;
        }

        d->dValue = result;

        if (qIsInf(result))
        {
            d->reason = tr("Result is infinite");
        }
        else if (qIsNaN(result))
        {
            d->reason = tr("Result is NaN");
        }
        else if (d->checkZero && qFuzzyIsNull(result))
        {
            d->reason = tr("Result is zero");
        }
        else if (d->checkLessThanZero && result < 0)
        {
            d->reason = tr("Result less than zero");
        }
        else
        {
            d->strValue = qApp->LocaleToString(result) + QLatin1Char(' ') + d->postfix;
            d->error = false;
        }
    }
    else
    {
        d->reason = tr("Formula is empty");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VFormula::ResetState()
{
    d->strValue = tr("Error");
    d->error = true;
    d->dValue = NAN;
    d->reason = tr("Not evaluated");
}
