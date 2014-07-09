/************************************************************************
 **
 **  @file   vvariable.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 7, 2014
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

#ifndef VVARIABLE_H
#define VVARIABLE_H

#include <QString>

class VVariable
{
public:
    VVariable();
    VVariable(const qreal &base, const qreal &ksize, const qreal &kheight, const QString &description = QString());
    VVariable(const qreal &base, const QString &description = QString());
    VVariable(const VVariable &var);
    VVariable &operator=(const VVariable &var);
    virtual ~VVariable();

    qreal   GetBase() const;
    void    SetBase(const qreal &value);
    qreal   GetKsize() const;
    qreal   GetKheight() const;
    QString GetDescription() const;
    qreal   GetValue() const;
    qreal   GetValue(const qreal &size, const qreal &height) const;
protected:
    /** @brief base value in base size and height */
    qreal          base;

    /** @brief ksize increment in sizes */
    qreal          ksize;

    /** @brief kgrowth increment in heights */
    qreal          kheight;

    /** @brief description description of increment */
    QString        description;
private:
    void    Init();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetBase return value in base size and height
 * @return value
 */
inline qreal VVariable::GetBase() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VVariable::SetBase(const qreal &value)
{
    base = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKsize return increment in sizes
 * @return increment
 */
inline qreal VVariable::GetKsize() const
{
    return ksize;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKheight return increment in heights
 * @return increment
 */
inline qreal VVariable::GetKheight() const
{
    return kheight;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VVariable::GetDescription() const
{
    return description;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VVariable::GetValue() const
{
    return base;
}

#endif // VVARIABLE_H
