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

#ifndef VINTERNALVARIABLE_H
#define VINTERNALVARIABLE_H

#include <QString>
#include <QSharedDataPointer>
#include "../options.h"

class VInternalVariableData;

class VInternalVariable
{
public:
    VInternalVariable();
    VInternalVariable(const VInternalVariable &var);
    VInternalVariable &operator=(const VInternalVariable &var);
    virtual ~VInternalVariable();

    qreal        GetValue() const;
    qreal*       GetValue();
    void         SetValue(const qreal &value);

    QString      GetName() const;
    void         SetName(const QString &name);

    VarType      GetType() const;
    void         SetType(const VarType &type);

    virtual bool Filter(quint32 id);

    virtual bool IsNotUsed() const;
private:
    QSharedDataPointer<VInternalVariableData> d;
};

#endif // VINTERNALVARIABLE_H
