/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vexceptionconversionerror.h"

VExceptionConversionError::VExceptionConversionError(const QString &what, const QString &str)
    :VException(what), str(str){
    Q_ASSERT_X(!str.isEmpty(), Q_FUNC_INFO, "Error converting string is empty");
}

VExceptionConversionError::VExceptionConversionError(const VExceptionConversionError &e):
    VException(e), str(e.String()){
}

QString VExceptionConversionError::ErrorMessage() const{
    QString error = QString("ExceptionConversionError: %1 %2").arg(what, str);
    return error;
}
