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
#include "../vpatterndb/vpiecenode.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/variables/vcurvelength.h"
#include "../ifc/exception/vexceptionbadid.h"
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
#include <qnumeric.h>
#include <QListWidget>
#include <QBuffer>

const QColor errorColor = Qt::red;

namespace
{
const int dialogMaxFormulaHeight = 80;

//---------------------------------------------------------------------------------------------------------------------
bool DoublePoint(const VPieceNode &firstNode, const VPieceNode &secondNode, const VContainer *data)
{
    if (firstNode.GetTypeTool() == Tool::NodePoint && not (firstNode.GetId() == NULL_ID)
            && secondNode.GetTypeTool() == Tool::NodePoint && not (secondNode.GetId() == NULL_ID))
    {
        // don't ignore the same point twice
        if (firstNode.GetId() == secondNode.GetId())
        {
            return true;
        }

        // But ignore the same coordinate if a user wants
        if (not firstNode.IsCheckUniqueness() || not secondNode.IsCheckUniqueness())
        {
            return false;
        }

        try
        {
            const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstNode.GetId());
            const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondNode.GetId());

            return firstPoint->toQPointF() == secondPoint->toQPointF();
        }
        catch(const VExceptionBadId &)
        {
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DoubleCurve(const VPieceNode &firstNode, const VPieceNode &secondNode)
{
    if (firstNode.GetTypeTool() != Tool::NodePoint && not (firstNode.GetId() == NULL_ID)
            && secondNode.GetTypeTool() != Tool::NodePoint && not (secondNode.GetId() == NULL_ID))
    {
        // don't ignore the same curve twice
        if (firstNode.GetId() == secondNode.GetId())
        {
            return true;
        }
    }

    return false;
}
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode RowNode(QListWidget *listWidget, int i)
{
    SCASSERT(listWidget != nullptr);

    if (i < 0 || i >= listWidget->count())
    {
        return VPieceNode();
    }

    const QListWidgetItem *rowItem = listWidget->item(i);
    SCASSERT(rowItem != nullptr);
    return qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
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

//---------------------------------------------------------------------------------------------------------------------
int FindNotExcludedNodeDown(QListWidget *listWidget, int candidate)
{
    SCASSERT(listWidget != nullptr);

    int index = -1;
    if (candidate < 0 || candidate >= listWidget->count())
    {
        return index;
    }

    int i = candidate;
    VPieceNode rowNode;
    do
    {
        const QListWidgetItem *rowItem = listWidget->item(i);
        SCASSERT(rowItem != nullptr);
        rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

        if (not rowNode.IsExcluded())
        {
            index = i;
        }

        ++i;
    }
    while (rowNode.IsExcluded() && i < listWidget->count());

    return index;
}

//---------------------------------------------------------------------------------------------------------------------
int FindNotExcludedNodeUp(QListWidget *listWidget, int candidate)
{
    SCASSERT(listWidget != nullptr);

    int index = -1;
    if (candidate < 0 || candidate >= listWidget->count())
    {
        return index;
    }

    int i = candidate;
    VPieceNode rowNode;
    do
    {
        const QListWidgetItem *rowItem = listWidget->item(i);
        SCASSERT(rowItem != nullptr);
        rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

        if (not rowNode.IsExcluded())
        {
            index = i;
        }

        --i;
    }
    while (rowNode.IsExcluded() && i > -1);

    return index;
}

//---------------------------------------------------------------------------------------------------------------------
bool FirstPointEqualLast(QListWidget *listWidget, const VContainer *data)
{
    SCASSERT(listWidget != nullptr);
    if (listWidget->count() > 1)
    {
        const VPieceNode topNode = RowNode(listWidget, FindNotExcludedNodeDown(listWidget, 0));
        const VPieceNode bottomNode = RowNode(listWidget, FindNotExcludedNodeUp(listWidget, listWidget->count()-1));

        return DoublePoint(topNode, bottomNode, data);
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DoublePoints(QListWidget *listWidget, const VContainer *data)
{
    SCASSERT(listWidget != nullptr);
    for (int i=0, sz = listWidget->count()-1; i<sz; ++i)
    {
        const int firstIndex = FindNotExcludedNodeDown(listWidget, i);
        const VPieceNode firstNode = RowNode(listWidget, firstIndex);
        const VPieceNode secondNode = RowNode(listWidget, FindNotExcludedNodeDown(listWidget, firstIndex+1));

        if (DoublePoint(firstNode, secondNode, data))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DoubleCurves(QListWidget *listWidget)
{
    SCASSERT(listWidget != nullptr);
    for (int i=0, sz = listWidget->count()-1; i<sz; ++i)
    {
        const int firstIndex = FindNotExcludedNodeDown(listWidget, i);
        const VPieceNode firstNode = RowNode(listWidget, firstIndex);
        const VPieceNode secondNode = RowNode(listWidget, FindNotExcludedNodeDown(listWidget, firstIndex+1));

        if (DoubleCurve(firstNode, secondNode))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool EachPointLabelIsUnique(QListWidget *listWidget)
{
    SCASSERT(listWidget != nullptr);
    QSet<quint32> pointLabels;
    int countPoints = 0;
    for (int i=0; i < listWidget->count(); ++i)
    {
        const QListWidgetItem *rowItem = listWidget->item(i);
        SCASSERT(rowItem != nullptr);
        const VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
        if (rowNode.GetTypeTool() == Tool::NodePoint)
        {
            ++countPoints;
            pointLabels.insert(rowNode.GetId());
        }
    }

    return countPoints == pointLabels.size();
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogWarningIcon()
{
    const QIcon icon = QIcon::fromTheme("dialog-warning",
                                  QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));

    const QPixmap pixmap = icon.pixmap(QSize(16, 16));
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    return QStringLiteral("<img src=\"data:image/png;base64,") + byteArray.toBase64() + QStringLiteral("\"/> ");
}

//---------------------------------------------------------------------------------------------------------------------
QFont NodeFont(QFont font, bool nodeExcluded)
{
    font.setPointSize(12);
    font.setWeight(QFont::Bold);
    font.setStrikeOut(nodeExcluded);
    return font;
}

//---------------------------------------------------------------------------------------------------------------------
void CurrentCurveLength(vidtype curveId, VContainer *data)
{
    SCASSERT(data != nullptr)
    VCurveLength *length = nullptr;
    try
    {
        const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(curveId);
        length = new VCurveLength(curveId, curveId, curve.data(), *data->GetPatternUnit());
    }
    catch (const VExceptionBadId &)
    {
        length = new VCurveLength();
    }

    SCASSERT(length != nullptr)
    length->SetName(currentLength);

    data->AddVariable(length);
}
