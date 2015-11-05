/************************************************************************
 **
 **  @file   dialogtool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogtool.h"
#include "../../../vpatterndb/calculator.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../../vgeometry/varc.h"
#include "../../../vgeometry/vpointf.h"
#include "../../../vgeometry/vsplinepath.h"
#include "../../tools/vabstracttool.h"
#include "../../../qmuparser/qmuparsererror.h"
#include "../../../qmuparser/qmudef.h"
#include "../../../ifc/xml/vdomdocument.h"

#include <QTimer>
#include <QCloseEvent>
#include <QShowEvent>
#include <QComboBox>
#include <QListWidgetItem>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QLabel>
#include <QSettings>
#include <QPushButton>
#include <QDoubleSpinBox>

Q_LOGGING_CATEGORY(vDialog, "v.dialog")

#define DIALOG_MAX_FORMULA_HEIGHT 64

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTool create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTool::DialogTool(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :QDialog(parent), data(data), isInitialized(false), flagName(true), flagFormula(true), flagError(true),
      timerFormula(nullptr), bOk(nullptr), bApply(nullptr), spinBoxAngle(nullptr), plainTextEditFormula(nullptr),
      labelResultCalculation(nullptr), labelEditNamePoint(nullptr), labelEditFormula(nullptr),
      okColor(QColor(76, 76, 76)), errorColor(Qt::red), associatedTool(nullptr),
      toolId(toolId), prepare(false), pointName(QString()), number(0), vis(nullptr)
{
    SCASSERT(data != nullptr);
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
}


//---------------------------------------------------------------------------------------------------------------------
DialogTool::~DialogTool()
{
    emit ToolTip("");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle when dialog cloded
 * @param event event
 */
void DialogTool::closeEvent(QCloseEvent *event)
{
    DialogRejected();
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle when window show
 * @param event event
 */
void DialogTool::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }
    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    setMaximumSize(size());
    setMinimumSize(size());

    isInitialized = true;//first show windows are held
    ShowVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxPoints fill comboBox list of points
 * @param box comboBox
 */
void DialogTool::FillComboBoxPoints(QComboBox *box, FillComboBox rule, const quint32 &ch1, const quint32 &ch2) const
{
    FillCombo<VPointF>(box, GOType::Point, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxArcs(QComboBox *box, FillComboBox rule, const quint32 &ch1, const quint32 &ch2) const
{
    FillCombo<VArc>(box, GOType::Arc, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxSplines(QComboBox *box, FillComboBox rule, const quint32 &ch1, const quint32 &ch2) const
{
    FillCombo<VSpline>(box, GOType::Spline, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxSplinesPath(QComboBox *box, FillComboBox rule, const quint32 &ch1,
                                         const quint32 &ch2) const
{
    FillCombo<VSplinePath>(box, GOType::SplinePath, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxCurves(QComboBox *box) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != toolId)
        {
            QSharedPointer<VGObject> obj = i.value();
            if ((obj->getType() == GOType::Arc || obj->getType() == GOType::Spline ||
                    obj->getType() == GOType::SplinePath) && obj->getMode() == Draw::Calculation)
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(i.key());

                QString newName = curve->name();
                int bias = 0;
                if (qApp->TrVars()->VariablesToUser(newName, 0, curve->name(), bias))
                {
                    list[newName] = i.key();
                }
                else
                {
                    list[curve->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxTypeLine fill comboBox list of type lines
 * @param box comboBox
 */
void DialogTool::FillComboBoxTypeLine(QComboBox *box, const QMap<QString, QIcon> &stylesPics) const
{
    SCASSERT(box != nullptr);
    QMap<QString, QIcon>::const_iterator i = stylesPics.constBegin();
    while (i != stylesPics.constEnd())
    {
        box->addItem(i.value(), "", QVariant(i.key()));
        ++i;
    }

    box->setCurrentIndex(4);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxLineColors(QComboBox *box) const
{
    SCASSERT(box != nullptr);

    const QMap<QString, QString> map = VAbstractTool::ColorsList();
    QMap<QString, QString>::const_iterator i = map.constBegin();
    while (i != map.constEnd())
    {
        QPixmap pix(16, 16);
        pix.fill(QColor(i.key()));
        box->addItem(QIcon(pix), i.value(), QVariant(i.key()));
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxCrossCirclesPoints(QComboBox *box) const
{
    SCASSERT(box != nullptr);

    box->addItem(tr("First point"), QVariant(static_cast<int>(CrossCirclesPoint::FirstPoint)));
    box->addItem(tr("Second point"), QVariant(static_cast<int>(CrossCirclesPoint::SecondPoint)));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::GetComboBoxCurrentData(const QComboBox *box) const
{
    SCASSERT(box != nullptr)
    QString value;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    value = box->itemData(box->currentIndex()).toString();
#else
    value = box->currentData().toString();
#endif
    if (value.isEmpty())
    {
        value = TypeLineLine;
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeCurrentData select item in combobox by id
 * @param box combobox
 * @param value id of item
 */
void DialogTool::ChangeCurrentData(QComboBox *box, const QVariant &value) const
{
    SCASSERT(box != nullptr)
    const qint32 index = box->findData(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::MoveCursorToEnd(QPlainTextEdit *plainTextEdit)
{
    SCASSERT(plainTextEdit != nullptr);
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    plainTextEdit->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::eventFilter(QObject *object, QEvent *event)
{
    QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>(object);
    if (plainTextEdit != nullptr)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return))
            {
                // Ignore Enter key
                return true;
            }
        }
    }
    else
    {
        // pass the event on to the parent class
        return QDialog::eventFilter(object, event);
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValFormulaChanged handle change formula
 * @param flag flag state of formula
 * @param edit LineEdit
 * @param timer timer of formula
 */
void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer)
{
    SCASSERT(edit != nullptr);
    SCASSERT(timer != nullptr);
    SCASSERT(labelEditFormula != nullptr);
    SCASSERT(labelResultCalculation != nullptr);
    if (edit->text().isEmpty())
    {
        flag = false;
        CheckState();
        ChangeColor(labelEditFormula, Qt::red);
        labelResultCalculation->setText(tr("Error"));
        labelResultCalculation->setToolTip(tr("Empty field"));
        return;
    }
    timer->start(1000);
}
//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ValFormulaChanged(bool &flag, QPlainTextEdit *edit, QTimer *timer)
{
    SCASSERT(edit != nullptr);
    SCASSERT(timer != nullptr);
    SCASSERT(labelEditFormula != nullptr);
    SCASSERT(labelResultCalculation != nullptr);
    if (edit->toPlainText().isEmpty())
    {
        flag = false;
        CheckState();
        ChangeColor(labelEditFormula, Qt::red);
        labelResultCalculation->setText(tr("Error"));
        labelResultCalculation->setToolTip(tr("Empty field"));
        return;
    }
    timer->setSingleShot(true);
    timer->start(1000);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Eval evaluate formula and show result
 * @param text expresion that we parse
 * @param flag flag state of eval formula
 * @param label label for signal error
 * @param postfix unit name
 * @param checkZero true - if formula can't be equal zero
 */
qreal DialogTool::Eval(const QString &text, bool &flag, QLabel *label, const QString& postfix, bool checkZero)
{
    SCASSERT(label != nullptr);
    SCASSERT(labelEditFormula != nullptr);

    qreal result = INT_MIN;//Value can be 0, so use max imposible value

    if (text.isEmpty())
    {
        flag = false;
        ChangeColor(labelEditFormula, Qt::red);
        label->setText(tr("Error") + " (" + postfix + ")");
        label->setToolTip(tr("Empty field"));
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            QString formula = text;
            formula.replace("\n", " ");
            // Translate to internal look.
            formula = qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
            Calculator *cal = new Calculator();
            result = cal->EvalFormula(data->PlainVariables(), formula);
            delete cal;

            //if result equal 0
            if (checkZero && qFuzzyCompare(1 + result, 1 + 0))
            {
                flag = false;
                ChangeColor(labelEditFormula, Qt::red);
                label->setText(tr("Error") + " (" + postfix + ")");
                label->setToolTip(tr("Value can't be 0"));
            }
            else
            {
                label->setText(qApp->LocaleToString(result) + " " +postfix);
                flag = true;
                ChangeColor(labelEditFormula, okColor);
                label->setToolTip(tr("Value"));
                emit ToolTip("");
            }
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error") + " (" + postfix + ")");
            flag = false;
            ChangeColor(labelEditFormula, Qt::red);
            emit ToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            label->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    CheckState(); // Disable Ok and Apply buttons if something wrong.
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentPointId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                   const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr);

    box->blockSignals(true);

    FillComboBoxPoints(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);

    box->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplineId set current spline id in combobox
 */
void DialogTool::setCurrentSplineId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                    const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplines(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentArcId
 */
void DialogTool::setCurrentArcId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                 const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr);
    FillComboBoxArcs(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplinePathId set current splinePath id in combobox
 * @param box combobox
 * @param value splinePath id
 */
void DialogTool::setCurrentSplinePathId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                        const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplinesPath(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentCurveId(QComboBox *box, const quint32 &value) const
{
    SCASSERT(box != nullptr);
    FillComboBoxCurves(box);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getCurrentPointId return current point id stored in combobox
 * @param box combobox
 * @return id or 0 if combobox is empty
 */
quint32 DialogTool::getCurrentObjectId(QComboBox *box) const
{
    SCASSERT(box != nullptr);
    qint32 index = box->currentIndex();
    if (index != -1)
    {
        return qvariant_cast<quint32>(box->itemData(index));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint DialogTool::getCurrentCrossPoint(QComboBox *box) const
{
    int value;
    bool ok = false;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    value = box->itemData(box->currentIndex()).toInt(&ok);
#else
    value = box->currentData().toInt(&ok);
#endif
    if (not ok)
    {
        return CrossCirclesPoint::FirstPoint;
    }

    switch(value)
    {
        case 1:
            return CrossCirclesPoint::FirstPoint;
            break;
        case 2:
            return CrossCirclesPoint::SecondPoint;
            break;
        default:
            return CrossCirclesPoint::FirstPoint;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::SetObject(const quint32 &id, QComboBox *box, const QString &toolTip)
{
    SCASSERT(box != nullptr);
    const qint32 index = box->findData(id);
    if ( index != -1 )
    { // -1 for not found
        box->setCurrentIndex(index);
        emit ToolTip(toolTip);
        return true;
    }
    else
    {
        qWarning()<<"Can't find object by id"<<id;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DeployFormula(QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight)
{
    SCASSERT(formula != nullptr);
    SCASSERT(buttonGrowLength != nullptr);

    const QTextCursor cursor = formula->textCursor();

    if (formula->height() < DIALOG_MAX_FORMULA_HEIGHT)
    {
        formula->setFixedHeight(DIALOG_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        buttonGrowLength->setIcon(QIcon::fromTheme("go-next",
                                                   QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       formula->setFixedHeight(formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       buttonGrowLength->setIcon(QIcon::fromTheme("go-down",
                                                  QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    setUpdatesEnabled(false);
    repaint();
    setUpdatesEnabled(true);

    formula->setFocus();
    formula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillList fill combobox list
 * @param box combobox
 * @param list list with ids and names
 */
void DialogTool::FillList(QComboBox *box, const QMap<QString, quint32> &list) const
{
    SCASSERT(box != nullptr);
    box->clear();

    QMapIterator<QString, quint32> iter(list);
    while (iter.hasNext())
    {
        iter.next();
        box->addItem(iter.key(), iter.value());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckState enable, when all is correct, or disable, when something wrong, button ok
 */
void DialogTool::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula && flagName && flagError);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChosenObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTool::ChosenObject(quint32 id, const SceneObject &type)
{
    Q_UNUSED(id);
    Q_UNUSED(type);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NamePointChanged check name of point
 */
void DialogTool::NamePointChanged()
{
    SCASSERT(labelEditNamePoint != nullptr);
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        QString name = edit->text();
        QRegularExpression rx(NameRegExp());
        if (name.isEmpty() || (pointName != name && data->IsUnique(name) == false) ||
            rx.match(name).hasMatch() == false)
        {
            flagName = false;
            ChangeColor(labelEditNamePoint, Qt::red);
        }
        else
        {
            flagName = true;
            ChangeColor(labelEditNamePoint, okColor);
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ChangeColor(QWidget *widget, const QColor &color)
{
    SCASSERT(widget != nullptr);
    QPalette palette = widget->palette();
    palette.setColor(widget->foregroundRole(), color);
    widget->setPalette(palette);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogTool::DialogAccepted()
{
    SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DialogApply()
{
    SaveData();
    emit DialogApplied();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogRejected emit signal dialog rejected
 */
void DialogTool::DialogRejected()
{
    emit DialogClosed(QDialog::Rejected);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief formula check formula
 */
void DialogTool::FormulaChanged()
{
    QPlainTextEdit* edit = qobject_cast<QPlainTextEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FormulaChangedPlainText() //-V524
{
    QPlainTextEdit* edit = qobject_cast<QPlainTextEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowUp set angle value 90 degree
 */
void DialogTool::ArrowUp()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(90);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowDown set angle value 270 degree
 */
void DialogTool::ArrowDown()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(270);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeft set angle value 180 degree
 */
void DialogTool::ArrowLeft()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(180);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRight set angle value 0 degree
 */
void DialogTool::ArrowRight()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeftUp set angle value 135 degree
 */
void DialogTool::ArrowLeftUp()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(135);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeftDown set angle value 225 degree
 */
void DialogTool::ArrowLeftDown()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(225);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRightUp set angle value 45 degree
 */
void DialogTool::ArrowRightUp()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(45);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRightDown set angle value 315 degree
 */
void DialogTool::ArrowRightDown()
{
    SCASSERT(spinBoxAngle != nullptr);
    spinBoxAngle->setValue(315);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalFormula evaluate formula
 */
void DialogTool::EvalFormula()
{
    SCASSERT(plainTextEditFormula != nullptr);
    SCASSERT(labelResultCalculation != nullptr);
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());//Show unit in dialog lable (cm, mm or inch)
    Eval(plainTextEditFormula->toPlainText(), flagFormula, labelResultCalculation, postfix, false);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 DialogTool::GetToolId() const
{
    return toolId;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SetToolId(const quint32 &value)
{
    toolId = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ShowVisToolTip(const QString &toolTip)
{
    emit ToolTip(toolTip);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ShowDialog(bool click)
{
    Q_UNUSED(click);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SetAssociatedTool(VAbstractTool *tool)
{
    this->associatedTool=tool;
    SetToolId(tool->getId());
}

//---------------------------------------------------------------------------------------------------------------------
template <typename GObject>
void DialogTool::FillCombo(QComboBox *box, GOType gType, FillComboBox rule, const quint32 &ch1,
                           const quint32 &ch2) const
{
    SCASSERT(box != nullptr);
    box->blockSignals(true);

    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (rule == FillComboBox::NoChildren)
        {
            if (i.key() != toolId && i.key() != ch1 && i.key() != ch2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == gType && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<GObject> arc = data->GeometricObject<GObject>(i.key());

                    QString newName = arc->name();
                    int bias = 0;
                    if (qApp->TrVars()->VariablesToUser(newName, 0, arc->name(), bias))
                    {
                        list[newName] = i.key();
                    }
                    else
                    {
                        list[arc->name()] = i.key();
                    }
                }
            }
        }
        else
        {
            if (i.key() != toolId)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == gType && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<GObject> arc = data->GeometricObject<GObject>(i.key());

                    QString newName = arc->name();
                    int bias = 0;
                    if (qApp->TrVars()->VariablesToUser(newName, 0, arc->name(), bias))
                    {
                        list[newName] = i.key();
                    }
                    else
                    {
                        list[arc->name()] = i.key();
                    }
                }
            }
        }
    }
    FillList(box, list);

    box->blockSignals(false);
}
