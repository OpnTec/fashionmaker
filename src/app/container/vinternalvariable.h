/************************************************************************
 **
 **  @file   vinternalvariable.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VINTERNALVARIABLE_H
#define VINTERNALVARIABLE_H

#include <QString>

enum class VarType : char { Measurement, Increment, LengthLine, LengthSpline, LengthArc, LineAngle, Unknown };

class VInternalVariable
{
public:
    VInternalVariable();
    VInternalVariable(const VInternalVariable &var);
    VInternalVariable &operator=(const VInternalVariable &var);
    virtual ~VInternalVariable();

    qreal        GetValue() const;
    qreal*       GetValue();
    QString      GetName() const;
    VarType      GetType() const;

    virtual bool Filter(quint32 id);
protected:
    VarType type;

    /** @brief value variable's value */
    qreal   value;

    QString name;
};

//---------------------------------------------------------------------------------------------------------------------
inline qreal VInternalVariable::GetValue() const
{
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal *VInternalVariable::GetValue()
{
    return &value;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VInternalVariable::GetName() const
{
    return name;
}

//---------------------------------------------------------------------------------------------------------------------
inline VarType VInternalVariable::GetType() const
{
    return type;
}

#endif // VINTERNALVARIABLE_H
