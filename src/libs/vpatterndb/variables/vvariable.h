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

#ifndef VVARIABLE_H
#define VVARIABLE_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"
#include "vinternalvariable.h"

class VVariableData;

class VVariable :public VInternalVariable
{
public:
    VVariable();
    VVariable(const QString &name, const QString &description = QString());
    VVariable(const VVariable &var);

    virtual ~VVariable() Q_DECL_OVERRIDE;

    VVariable &operator=(const VVariable &var);
#ifdef Q_COMPILER_RVALUE_REFS
    VVariable &operator=(VVariable &&var) Q_DECL_NOTHROW { Swap(var); return *this; }
#endif

    void Swap(VVariable &var) Q_DECL_NOTHROW
    { VInternalVariable::Swap(var); std::swap(d, var.d); }

    QString GetDescription() const;
    void    SetDescription(const QString &desc);

private:
    QSharedDataPointer<VVariableData> d;
};

Q_DECLARE_TYPEINFO(VVariable, Q_MOVABLE_TYPE);

#endif // VVARIABLE_H
