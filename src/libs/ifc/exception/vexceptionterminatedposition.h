/************************************************************************
 **
 **  @file   vexceptionterminatedposition.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 6, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef VEXCEPTIONTERMINATEDPOSITION_H
#define VEXCEPTIONTERMINATEDPOSITION_H

#include "vexception.h"

class VExceptionTerminatedPosition : public VException
{
public:
    explicit VExceptionTerminatedPosition(const QString &error) V_NOEXCEPT_EXPR (true);
    VExceptionTerminatedPosition(const VExceptionTerminatedPosition &e) V_NOEXCEPT_EXPR (true);
    VExceptionTerminatedPosition &operator=(const VExceptionTerminatedPosition &e) V_NOEXCEPT_EXPR (true);
    virtual ~VExceptionTerminatedPosition() V_NOEXCEPT_EXPR (true);

    Q_NORETURN virtual void raise() const override;

    // cppcheck-suppress unusedFunction
    Q_REQUIRED_RESULT virtual VExceptionTerminatedPosition *clone() const override;
};

#endif // VEXCEPTIONTERMINATEDPOSITION_H
