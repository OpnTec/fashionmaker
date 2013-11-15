/************************************************************************
 **
 **  @file   vexceptionconversionerror.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vexception.h"

class VExceptionConversionError : public VException
{
public:
                    VExceptionConversionError(const QString &what, const QString &str);
                    VExceptionConversionError(const VExceptionConversionError &e)
                        :VException(e), str(e.String()){}
    virtual         ~VExceptionConversionError() Q_DECL_NOEXCEPT_EXPR(true) {}
    virtual QString ErrorMessage() const;
    inline QString  String() const {return str;}
protected:
    QString         str;
};

#endif // VEXCEPTIONCONVERSIONERROR_H
