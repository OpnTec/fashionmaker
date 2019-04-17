/************************************************************************
 **
 **  @file   vformula_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 4, 2019
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
#ifndef VFORMULA_P_H
#define VFORMULA_P_H

#include "../vmisc/diagnostic.h"
#include "../vmisc/typedef.h"
#include "../vmisc/vmath.h"

#include <QSharedData>
#include <QCoreApplication>

class VContainer;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VFormulaData : public QSharedData
{
    Q_DECLARE_TR_FUNCTIONS(VFormulaData)
public:
  VFormulaData()
  {}

  VFormulaData(const QString &formula, const VContainer *container)
      : formula(formula),
        data(container),
        reason(tr("Not evaluated"))
  {}

  VFormulaData (const VFormulaData& formula)
      : QSharedData(formula),
        formula(formula.formula),
        strValue(formula.strValue),
        checkZero(formula.checkZero),
        checkLessThanZero(formula.checkLessThanZero),
        data(formula.data),
        toolId(formula.toolId),
        postfix(formula.postfix),
        error(formula.error),
        dValue(formula.dValue),
        reason(formula.reason)
  {}

  ~VFormulaData() {}

  QString formula{};
  QString strValue{tr("Error")};
  bool checkZero{true};
  bool checkLessThanZero{false};
  const VContainer *data{nullptr};
  quint32 toolId{NULL_ID};
  QString postfix{};
  bool error{true};
  qreal dValue{NAN};
  QString reason{tr("Formula is empty")};

private:
    VFormulaData &operator=(const VFormulaData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VFORMULA_P_H
