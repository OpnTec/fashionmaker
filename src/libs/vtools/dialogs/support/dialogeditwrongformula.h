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

#include <qcompilerdetection.h>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QTableWidgetItem>
#include <QtGlobal>

#include "../tools/dialogtool.h"

template <class T> class QSharedPointer;
class VMeasurement;

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
    DialogEditWrongFormula(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogEditWrongFormula() Q_DECL_OVERRIDE;

    QString      GetFormula() const;
    void         SetFormula(const QString &value);
    void         setCheckZero(bool value);
    void         setCheckLessThanZero(bool value);
    void         setPostfix(const QString &value);
public slots:
    virtual void DialogAccepted() Q_DECL_OVERRIDE;
    virtual void DialogRejected() Q_DECL_OVERRIDE;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void         DeployFormulaTextEdit();
    virtual void EvalFormula() Q_DECL_OVERRIDE;
    void         ValChanged(int row);
    void         PutHere();
    void         PutVal(QTableWidgetItem * item);

    void         Measurements();
    void         LengthLines();
    void         RadiusArcs();
    void         AnglesCurves();
    void         LengthCurves();
    void         CurvesCLength();
    void         AngleLines();
    void         Increments();
    void         PreviewCalculations();
    void         Functions();
protected:
    virtual void CheckState() Q_DECL_FINAL;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent( QShowEvent *event ) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private slots:
    void FilterVariablesEdited(const QString &filter);
private:
    Q_DISABLE_COPY(DialogEditWrongFormula)
    Ui::DialogEditWrongFormula *ui;

    /** @brief formula string with formula */
    QString           formula;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;

    bool checkZero;
    bool checkLessThanZero;
    QString postfix;
    bool restoreCursor;

    void InitVariables();

    template <class key, class val>
    void ShowVariable(const QMap<key, val> &var);
    void ShowMeasurements(const QMap<QString, QSharedPointer<VMeasurement> > &var);
    void ShowFunctions();
    void ShowIncrementsInPreviewCalculation(bool show);

    void SetDescription(const QString &name, qreal value, const QString &unit, const QString &description);
};


#endif // DIALOGEDITWRONGFORMULA_H
