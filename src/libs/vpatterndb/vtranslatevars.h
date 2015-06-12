/************************************************************************
 **
 **  @file   vtranslatevars.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VTRANSLATEVARS_H
#define VTRANSLATEVARS_H

#include <QMap>
#include "../qmuparser/qmutranslation.h"

class VTranslateVars
{
public:
    VTranslateVars(bool osSeparator);
    ~VTranslateVars();

    bool MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const;

    QString VarToUser(const QString &var) const;
    QString VarFromUser(const QString &var) const;

    QString GuiText(const QString &measurement) const;
    QString Description(const QString &measurement) const;
    QString PostfixOperator(const QString &name) const;
    QString STDescription(const QString &id) const;

    QString FormulaFromUser(const QString &formula, bool osSeparator) const;
    QString FormulaToUser(const QString &formula) const;

private:
    Q_DISABLE_COPY(VTranslateVars)
    QMap<QString, qmu::QmuTranslation> measurements;
    QMap<QString, qmu::QmuTranslation> guiTexts;
    QMap<QString, qmu::QmuTranslation> descriptions;
    QMap<QString, qmu::QmuTranslation> variables;
    QMap<QString, qmu::QmuTranslation> functions;
    QMap<QString, qmu::QmuTranslation> postfixOperators;
    QMap<QString, qmu::QmuTranslation> stDescriptions;
    bool osSeparator;

    void InitMeasurements();
    void InitVariables();
    void InitFunctions();
    void InitPostfixOperators();
    void InitSTDescriptions();

    void InitMeasurement(const QString &name, const qmu::QmuTranslation &m, const qmu::QmuTranslation &g,
                         const qmu::QmuTranslation &d);

    void CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens, QMap<int, QString> &numbers) const;
    void BiasTokens(int position, int bias, QMap<int, QString> &tokens) const;
};

#endif // VTRANSLATEVARS_H
