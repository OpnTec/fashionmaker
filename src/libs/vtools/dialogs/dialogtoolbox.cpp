/************************************************************************
 **
 **  @file   dialogtoolbox.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 1, 2019
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
#include "dialogtoolbox.h"

#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vcontainer.h"

#include <QDialog>
#include <QLabel>
#include <QLocale>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextCursor>
#include <QDebug>
#include <QTimer>
#include <QLineEdit>
#include <QRegularExpression>

const QColor errorColor = Qt::red;

namespace
{
    const int dialogMaxFormulaHeight = 80;
}

//---------------------------------------------------------------------------------------------------------------------
void MoveCursorToEnd(QPlainTextEdit *plainTextEdit)
{
    SCASSERT(plainTextEdit != nullptr)
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    plainTextEdit->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DeployFormula(QDialog *dialog, QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight)
{
    SCASSERT(dialog != nullptr)
    SCASSERT(formula != nullptr)
    SCASSERT(buttonGrowLength != nullptr)

    const QTextCursor cursor = formula->textCursor();

    //Before deploy ned to release dialog size
    //I don't know why, but don't need to fixate again.
    //A dialog will be lefted fixated. That's what we need.
    dialog->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    dialog->setMinimumSize(QSize(0, 0));

    if (formula->height() < dialogMaxFormulaHeight)
    {
        formula->setFixedHeight(dialogMaxFormulaHeight);
        //Set icon from theme (internal for Windows system)
        buttonGrowLength->setIcon(QIcon::fromTheme(QStringLiteral("go-next"),
                                                   QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       formula->setFixedHeight(formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       buttonGrowLength->setIcon(QIcon::fromTheme(QStringLiteral("go-down"),
                                                  QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    dialog->setUpdatesEnabled(false);
    dialog->repaint();
    dialog->setUpdatesEnabled(true);

    formula->setFocus();
    formula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
bool FilterObject(QObject *object, QEvent *event)
{
    if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>(object))
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Period) && (keyEvent->modifiers() & Qt::KeypadModifier))
            {
                if (qApp->Settings()->GetOsSeparator())
                {
                    plainTextEdit->insertPlainText(QLocale().decimalPoint());
                }
                else
                {
                    plainTextEdit->insertPlainText(QLocale::c().decimalPoint());
                }
                return true;
            }
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
qreal EvalToolFormula(QDialog *dialog, const FormulaData &data, bool &flag)
{
    SCASSERT(data.labelResult != nullptr)
    SCASSERT(data.labelEditFormula != nullptr)

    qreal result = INT_MIN;//Value can be 0, so use max imposible value

    if (data.formula.isEmpty())
    {
        flag = false;
        ChangeColor(data.labelEditFormula, errorColor);
        data.labelResult->setText(QObject::tr("Error") + " (" + data.postfix + ")");
        data.labelResult->setToolTip(QObject::tr("Empty formula"));
    }
    else
    {
        try
        {
            // Translate to internal look.
            QString formula = qApp->TrVars()->FormulaFromUser(data.formula, qApp->Settings()->GetOsSeparator());
            QScopedPointer<Calculator> cal(new Calculator());
            result = cal->EvalFormula(data.variables, formula);

            if (qIsInf(result) || qIsNaN(result))
            {
                flag = false;
                ChangeColor(data.labelEditFormula, errorColor);
                data.labelResult->setText(QObject::tr("Error") + " (" + data.postfix + ")");
                data.labelResult->setToolTip(QObject::tr("Invalid result. Value is infinite or NaN. Please, check "
                                                         "your calculations."));
            }
            else
            {
                //if result equal 0
                if (data.checkZero && qFuzzyIsNull(result))
                {
                    flag = false;
                    ChangeColor(data.labelEditFormula, errorColor);
                    data.labelResult->setText(QObject::tr("Error") + " (" + data.postfix + ")");
                    data.labelResult->setToolTip(QObject::tr("Value can't be 0"));
                }
                else if (data.checkLessThanZero && result < 0)
                {
                    flag = false;
                    ChangeColor(data.labelEditFormula, errorColor);
                    data.labelResult->setText(QObject::tr("Error") + " (" + data.postfix + ")");
                    data.labelResult->setToolTip(QObject::tr("Value can't be less than 0"));
                }
                else
                {
                    data.labelResult->setText(qApp->LocaleToString(result) + QChar(QChar::Space) + data.postfix);
                    flag = true;
                    ChangeColor(data.labelEditFormula, OkColor(dialog));
                    data.labelResult->setToolTip(QObject::tr("Value"));
                }
            }
        }
        catch (qmu::QmuParserError &e)
        {
            data.labelResult->setText(QObject::tr("Error") + " (" + data.postfix + ")");
            flag = false;
            ChangeColor(data.labelEditFormula, errorColor);
            data.labelResult->setToolTip(QObject::tr("Parser error: %1").arg(e.GetMsg()));
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeColor(QWidget *widget, const QColor &color)
{
    SCASSERT(widget != nullptr)
    QPalette palette = widget->palette();
    palette.setColor(QPalette::Active, widget->foregroundRole(), color);
    palette.setColor(QPalette::Inactive, widget->foregroundRole(), color);
    widget->setPalette(palette);
}

//---------------------------------------------------------------------------------------------------------------------
QColor OkColor(QWidget *widget)
{
    SCASSERT(widget != nullptr);
    return widget->palette().color(QPalette::Active, QPalette::WindowText);
}

//---------------------------------------------------------------------------------------------------------------------
void CheckPointLabel(QDialog *dialog, QLineEdit* edit, QLabel *labelEditNamePoint, const QString &pointName,
                     const VContainer *data, bool &flag)
{
    SCASSERT(dialog != nullptr)
    SCASSERT(edit != nullptr)
    SCASSERT(labelEditNamePoint != nullptr)

    const QString name = edit->text();
    QRegularExpression rx(NameRegExp());
    if (name.isEmpty() || (pointName != name && not data->IsUnique(name)) || not rx.match(name).hasMatch())
    {
        flag = false;
        ChangeColor(labelEditNamePoint, errorColor);
    }
    else
    {
        flag = true;
        ChangeColor(labelEditNamePoint, OkColor(dialog));
    }
}
