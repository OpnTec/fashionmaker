/************************************************************************
 **
 **  @file   calculator.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "vcontainer.h"

#include "../../libs/qmuparser/qmuparser.h"
using namespace qmu;

class Calculator:public QmuParser
{
public:
    explicit Calculator(const VContainer *data);
    ~Calculator();
    qreal         EvalFormula(const QString &formula);
private:
    Q_DISABLE_COPY(Calculator)
    qreal *vVarVal;
    static int iVal;
    void          InitVariables(const VContainer *data);
    static qreal  CmUnit(qreal val);
    static qreal  MmUnit(qreal val);
    static qreal  InchUnit(qreal val);
    static qreal* AddVariable(const QString &a_szName, void *a_pUserData);
};

#endif // CALCULATOR_H
