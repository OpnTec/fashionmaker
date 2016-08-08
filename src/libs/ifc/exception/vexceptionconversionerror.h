/************************************************************************
 **
 **  @file   vexceptionconversionerror.h
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

#ifndef VEXCEPTIONCONVERSIONERROR_H
#define VEXCEPTIONCONVERSIONERROR_H

#include <qcompilerdetection.h>
#include <QString>

#include "exception/../ifcdef.h"
#include "vexception.h"

/**
 * @brief The VExceptionConversionError class for exception of conversion error
 */
class VExceptionConversionError : public VException
{
public:
    VExceptionConversionError(const QString &error, const QString &str);
    VExceptionConversionError(const VExceptionConversionError &e);
    virtual         ~VExceptionConversionError() V_NOEXCEPT_EXPR (true) Q_DECL_OVERRIDE;
    virtual QString ErrorMessage() const Q_DECL_OVERRIDE;
    QString         String() const;
protected:
    /** @brief str string, where happend error */
    QString         str;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief String return string, where happend error
 * @return string
 */
inline QString VExceptionConversionError::String() const
{
    return str;
}

#endif // VEXCEPTIONCONVERSIONERROR_H
