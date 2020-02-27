/************************************************************************
 **
 **  @file   vtranslatevars.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VTRANSLATEVARS_H
#define VTRANSLATEVARS_H

#include <qcompilerdetection.h>
#include <QtGlobal>

#include "vtranslatemeasurements.h"

class VTranslateVars : public VTranslateMeasurements
{
public:
    explicit VTranslateVars();
    virtual ~VTranslateVars() = default;

    bool VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const;

    QString InternalVarToUser(const QString &var) const;
    QString PlaceholderToUser(QString var) const;

    QString PlaceholderToUserText(QString text) const;
    QString PlaceholderFromUserText(QString text) const;

    QString VarToUser(const QString &var) const;
    QString VarFromUser(const QString &var) const;

    QString PMSystemName(const QString &code) const;
    QString PMSystemAuthor(const QString &code) const;
    QString PMSystemBook(const QString &code) const;

    QString FormulaFromUser(const QString &formula, bool osSeparator) const;
    static QString TryFormulaFromUser(const QString &formula, bool osSeparator);

    QString FormulaToUser(const QString &formula, bool osSeparator) const;
    static QString TryFormulaToUser(const QString &formula, bool osSeparator);

    virtual void Retranslate() override;

    QMap<QString, QString> GetTranslatedFunctions() const;
    QMap<QString, qmu::QmuTranslation> GetFunctions() const;
    QMap<QString, qmu::QmuTranslation> GetFunctionsDescriptions() const;

    static void BiasTokens(int position, int bias, QMap<int, QString> &tokens);

private:
    Q_DISABLE_COPY(VTranslateVars)
    QMap<QString, qmu::QmuTranslation> PMSystemNames{};
    QMap<QString, qmu::QmuTranslation> PMSystemAuthors{};
    QMap<QString, qmu::QmuTranslation> PMSystemBooks{};
    QMap<QString, qmu::QmuTranslation> variables{};
    QMap<QString, qmu::QmuTranslation> functions{};
    QMap<QString, qmu::QmuTranslation> functionsDescriptions{};
    QMap<QString, qmu::QmuTranslation> placeholders{};
    QMap<QString, qmu::QmuTranslation> stDescriptions{};
    QMap<QString, QString> translatedFunctions{};

    void InitPatternMakingSystems();
    void InitVariables();
    void InitFunctions();
    void InitPlaceholder();

    void PrepareFunctionTranslations();

    void InitSystem(const QString &code, const qmu::QmuTranslation &name, const qmu::QmuTranslation &author,
                    const qmu::QmuTranslation &book);

    void CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens, QMap<int, QString> &numbers) const;

};

#endif // VTRANSLATEVARS_H
