/************************************************************************
 **
 **  @file   vexceptionconversionerror.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vexceptionconversionerror.h"

#include <QtGlobal>

#include "vexception.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionConversionError exception conversion error
 * @param error string with error
 * @param str string, where happend error
 */
VExceptionConversionError::VExceptionConversionError(const QString &error, const QString &str) V_NOEXCEPT_EXPR (true)
    :VException(error), str(str)
{
    Q_ASSERT_X(not str.isEmpty(), Q_FUNC_INFO, "Error converting string is empty");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionConversionError copy constructor
 * @param e exception
 */
VExceptionConversionError::VExceptionConversionError(const VExceptionConversionError &e) V_NOEXCEPT_EXPR (true)
    :VException(e), str(e.String())
{}

//---------------------------------------------------------------------------------------------------------------------
VExceptionConversionError &
VExceptionConversionError::operator=(const VExceptionConversionError &e) V_NOEXCEPT_EXPR (true)
{
    if ( &e == this )
    {
        return *this;
    }
    VException::operator=(e);
    str = e.String();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return main error message
 */
QString VExceptionConversionError::ErrorMessage() const
{
    return QString("ExceptionConversionError: %1 \"%2\"").arg(error, str);
}
