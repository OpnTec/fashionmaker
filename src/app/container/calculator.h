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

#include "../../libs/qmuparser/qmuparser.h"

class VContainer;

/**
 * @brief The Calculator class for calculation formula.
 *
 * Main purpose make easy evaluate value of formula and get tokens.
 * Note. If created to many parser for different purpes in the same time parser can work wrong.
 * Example:
 * DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data);
 * dialog->setFormula(formula);
 * if (dialog->exec() == QDialog::Accepted)
 * {
 *     formula = dialog->getFormula();
 *     //Need delete dialog here because parser in dialog don't allow use correct separator for parsing here.
 *     //Don't know why.
 *     delete dialog;
 *     Calculator *cal = new Calculator(data);
 *     result = cal->EvalFormula(formula);
 *     delete cal;//Here can be memory leak, but dialog already check this formula and probability very low.
 * }
 */
class Calculator:public qmu::QmuParser
{
public:
    Calculator(const VContainer *data);
    Calculator(const QString &formula, bool fromUser = true);
    ~Calculator();
    qreal         EvalFormula(const QString &formula);
private:
    Q_DISABLE_COPY(Calculator)
    qreal *vVarVal;
    const VContainer *data;
    void          InitVariables(const VContainer *data, const QMap<int, QString> &tokens, const QString &formula);
    void          InitCharacterSets();
    static qreal* AddVariable(const QString &a_szName, void *a_pUserData);
    void          SetSepForEval();
    void          SetSepForTr(bool fromUser);
    void          RemoveAll(QMap<int, QString> &map, const QString &val);
};

#endif // CALCULATOR_H
