/************************************************************************
 **
 **  @file   dialogeditwrongformula.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2014
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

#ifndef DIALOGEDITWRONGFORMULA_H
#define DIALOGEDITWRONGFORMULA_H

#include "../tools/dialogtool.h"

namespace Ui
{
class DialogEditWrongFormula;
}

/**
 * @brief The DialogEditWrongFormula class dialog for editing wrong formula.
 *
 * When math parser find in formula error user can try fix issue. Dialog will show all variables that user can use in
 * this formula. Dialog check fixed variant of formula.
 *
 * Don't implemant button "Apply" for this dialog!!
 */
class DialogEditWrongFormula : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogEditWrongFormula(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogEditWrongFormula();

    QString      GetFormula() const;
    void         SetFormula(const QString &value);
    void         setCheckZero(bool value);
    void         setPostfix(const QString &value);
public slots:
    virtual void DialogAccepted();
    virtual void DialogRejected();
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void         DeployFormulaTextEdit();
    virtual void EvalFormula();
    void         ValChenged(int row);
    void         SizeHeight();
    void         PutHere();
    void         PutVal(QListWidgetItem * item);

    void         Measurements();
    void         LengthLines();
    void         RadiusArcs();
    void         AnglesArcs();
    void         AnglesCurves();
    void         LengthArcs();
    void         LengthCurves();
    void         AngleLines();
    void         Increments();
protected:
    virtual void CheckState();
    virtual void closeEvent(QCloseEvent *event);
private:
    Q_DISABLE_COPY(DialogEditWrongFormula)
    Ui::DialogEditWrongFormula *ui;

    /** @brief formula string with formula */
    QString           formula;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;

    bool checkZero;
    QString postfix;
    bool restoreCursor;

    void InitVariables();

    template <class key, class val>
    void ShowVariable(const QMap<key, val> var);
};


#endif // DIALOGEDITWRONGFORMULA_H
