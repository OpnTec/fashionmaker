/************************************************************************
 **
 **  @file   vinternalvariable_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VINTERNALVARIABLE_P_H
#define VINTERNALVARIABLE_P_H

#include <QSharedData>
#include "../options.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VInternalVariableData : public QSharedData
{
public:

    VInternalVariableData()
        :type(VarType::Unknown), value(0), name(QString())
    {}

    VInternalVariableData(const VInternalVariableData &var)
        :QSharedData(var), type(var.type), value(var.value), name(var.name)
    {}

    virtual ~VInternalVariableData() {}

    VarType type;

    /** @brief value variable's value */
    qreal   value;

    QString name;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VINTERNALVARIABLE_P_H
