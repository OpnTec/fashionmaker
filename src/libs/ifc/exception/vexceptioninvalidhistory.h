/************************************************************************
 **
 **  @file   vexceptioninvalidhistory.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef VEXCEPTIONINVALIDHISTORY_H
#define VEXCEPTIONINVALIDHISTORY_H

#include "vexception.h"

class VExceptionInvalidHistory : public VException
{
public:
    explicit VExceptionInvalidHistory(const QString &error) V_NOEXCEPT_EXPR (true);
    VExceptionInvalidHistory(const VExceptionInvalidHistory &e) V_NOEXCEPT_EXPR (true);
    VExceptionInvalidHistory &operator=(const VExceptionInvalidHistory &e) V_NOEXCEPT_EXPR (true);
    virtual ~VExceptionInvalidHistory() V_NOEXCEPT_EXPR (true) Q_DECL_EQ_DEFAULT;

    Q_NORETURN virtual void raise() const override { throw *this; }

    Q_REQUIRED_RESULT virtual VExceptionInvalidHistory *clone() const override
    { return new VExceptionInvalidHistory(*this); }
};

#endif // VEXCEPTIONINVALIDHISTORY_H
