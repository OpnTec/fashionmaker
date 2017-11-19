/************************************************************************
 **
 **  @file   vincrement_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VINCREMENT_P_H
#define VINCREMENT_P_H

#include <QSharedData>

#include "../ifc/ifcdef.h"
#include "../vcontainer.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VIncrementData : public QSharedData
{
public:

    VIncrementData()
        : index(NULL_ID),
          formula(QString()),
          formulaOk(false),
          previewCalculation(false),
          data(VContainer(nullptr, nullptr))
    {}

    VIncrementData(VContainer *data, quint32 index, const QString &formula, bool ok)
        : index(index),
          formula(formula),
          formulaOk(ok),
          previewCalculation(false),
          data(*data)
    {
        // When we create an increment in the dialog it will get neccesary data. Such data must be removed because will
        // confuse a user. Increment should not know nothing about internal variables.
        Q_STATIC_ASSERT_X(static_cast<int>(VarType::Unknown) == 8, "Check that you used all types");
        this->data.ClearVariables(VarType::LineAngle);
        this->data.ClearVariables(VarType::LineLength);
        this->data.ClearVariables(VarType::CurveLength);
        this->data.ClearVariables(VarType::CurveCLength);
        this->data.ClearVariables(VarType::ArcRadius);
        this->data.ClearVariables(VarType::CurveAngle);
    }

    VIncrementData(const VIncrementData &incr)
        : QSharedData(incr),
          index(incr.index),
          formula(incr.formula),
          formulaOk(incr.formulaOk),
          previewCalculation(incr.previewCalculation),
          data(incr.data)
    {}

    virtual  ~VIncrementData();

    /** @brief id each increment have unique identificator */
    quint32 index;
    QString formula;
    bool    formulaOk;
    bool    previewCalculation;
    VContainer data;

private:
    VIncrementData &operator=(const VIncrementData &) Q_DECL_EQ_DELETE;
};

VIncrementData::~VIncrementData()
{}

QT_WARNING_POP

#endif // VINCREMENT_P_H
