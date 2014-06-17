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

#include "dialogtool.h"
#include "../../container/calculator.h"
#include "../../container/vcontainer.h"
#include "../../geometry/varc.h"
#include "../../geometry/vpointf.h"
#include "../../geometry/vsplinepath.h"
#include "../../tools/vabstracttool.h"
#include "../../../libs/qmuparser/qmuparsererror.h"
#include "../../widgets/vapplication.h"
#include "../../xml/vdomdocument.h"
#include <QTimer>
#include <QCloseEvent>
#include <QShowEvent>
#include <QComboBox>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QLabel>
#include <QSettings>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QRadioButton>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTool create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTool::DialogTool(const VContainer *data, QWidget *parent)
    :QDialog(parent), data(data), isInitialized(false), flagName(true), flagFormula(true), timerFormula(nullptr),
      bOk(nullptr), bApply(nullptr), spinBoxAngle(nullptr), lineEditFormula(nullptr), plainTextEditFormula(nullptr),
      listWidget(nullptr), labelResultCalculation(nullptr), labelDescription(nullptr), labelEditNamePoint(nullptr),
      labelEditFormula(nullptr), radioButtonSizeGrowth(nullptr), radioButtonStandardTable(nullptr),
      radioButtonIncrements(nullptr), radioButtonLengthLine(nullptr), radioButtonLengthArc(nullptr),
      radioButtonLengthCurve(nullptr), lineStyles(QStringList()), associatedTool(nullptr)
{
    SCASSERT(data != nullptr);
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
    //Keep synchronize with VAbstractTool styles list!!!
    lineStyles<<tr("No line")<<tr("Line")<<tr("Dash Line")<<tr("Dot Line")<<tr("Dash Dot Line")
             <<tr("Dash Dot Dot Line");
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowStaysOnTopHint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle when dialog cloded
 * @param event event
 */
void DialogTool::closeEvent(QCloseEvent *event)
{
    DialogClosed(QDialog::Rejected);
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
    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxPoints fill comboBox list of points
 * @param box comboBox
 * @param id don't show this id in list
 */
void DialogTool::FillComboBoxPoints(QComboBox *box, const quint32 &id) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, VGObject*> *objs = data->DataGObjects();
    QHashIterator<quint32, VGObject*> i(*objs);
    QMap<QString, quint32> list;
    while (i.hasNext())
    {
        i.next();
        if (i.key() != id)
        {
            VGObject *obj = i.value();
            if (obj->getType() == GOType::Point && obj->getMode() == Draw::Calculation)
            {
                const VPointF *point = data->GeometricObject<const VPointF *>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxArcs(QComboBox *box, const quint32 &id, ComboBoxCutArc cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, VGObject *> *objs = data->DataGObjects();
    QHashIterator<quint32, VGObject*> i(*objs);
    QMap<QString, quint32> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboBoxCutArc::CutArc)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::Arc && obj->getMode() == Draw::Calculation)
                {
                    const VArc *arc = data->GeometricObject<const VArc *>(i.key());
                    list[arc->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::Arc && obj->getMode() == Draw::Calculation)
                {
                    const VArc *arc = data->GeometricObject<const VArc *>(i.key());
                    list[arc->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxSplines fill comboBox list of splines
 * @param box comboBox
 * @param id don't show id in list
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::FillComboBoxSplines(QComboBox *box, const quint32 &id, ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, VGObject *> *objs = data->DataGObjects();
    QHashIterator<quint32, VGObject*> i(*objs);
    QMap<QString, quint32> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboBoxCutSpline::CutSpline)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::Spline && obj->getMode() == Draw::Calculation)
                {
                    const VSpline *spl = data->GeometricObject<const VSpline *>(i.key());
                    list[spl->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::Spline && obj->getMode() == Draw::Calculation)
                {
                    const VSpline *spl = data->GeometricObject<const VSpline *>(i.key());
                    list[spl->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxSplinesPath
 * @param box comboBox
 * @param id don't show id in list
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::FillComboBoxSplinesPath(QComboBox *box, const quint32 &id, ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, VGObject *> *objs = data->DataGObjects();
    QHashIterator<quint32, VGObject *> i(*objs);
    QMap<QString, quint32> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboBoxCutSpline::CutSpline)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(i.key());
                    list[splPath->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GOType::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(i.key());
                    list[splPath->name()] = i.key();
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
void DialogTool::FillComboBoxTypeLine(QComboBox *box) const
{
    SCASSERT(box != nullptr);
    box->addItems(lineStyles);
    box->setCurrentIndex(1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @param box combobox
 * @return type
 */
QString DialogTool::GetTypeLine(const QComboBox *box) const
{
    switch (lineStyles.indexOf(box->currentText()))
    {
        case 0: //No line
            return VAbstractTool::TypeLineNone;
            break;
        case 1: //Line
            return VAbstractTool::TypeLineLine;
            break;
        case 2: //Dash Line
            return VAbstractTool::TypeLineDashLine;
            break;
        case 3: //Dot Line
            return VAbstractTool::TypeLineDotLine;
            break;
        case 4: //Dash Dot Line
            return VAbstractTool::TypeLineDashDotLine;
            break;
        case 5: //Dash Dot Dot Line
            return VAbstractTool::TypeLineDashDotDotLine;
            break;
        default:
            return VAbstractTool::TypeLineLine;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetupTypeLine setupe type of line
 * @param box combobox
 * @param value string from pattern file
 */
void DialogTool::SetupTypeLine(QComboBox *box, const QString &value)
{
    QStringList styles = VAbstractTool::Styles();
    qint32 index = box->findText(lineStyles.at(styles.indexOf(value)));
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeCurrentText select item in combobox by name
 * @param box combobox
 * @param value name of item
 */
void DialogTool::ChangeCurrentText(QComboBox *box, const QString &value)
{
    qint32 index = box->findText(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
    else
    {
        qDebug()<<"Can't find object by name"<<value;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeCurrentData select item in combobox by id
 * @param box combobox
 * @param value id of item
 */
void DialogTool::ChangeCurrentData(QComboBox *box, const quint32 &value) const
{
    qint32 index = box->findData(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutValHere put variable into line edit from list
 * @param lineEdit lineEdit
 * @param listWidget listWidget
 */
void DialogTool::PutValHere(QLineEdit *lineEdit, QListWidget *listWidget)
{
    SCASSERT(lineEdit != nullptr);
    SCASSERT(listWidget != nullptr);
    QListWidgetItem *item = listWidget->currentItem();
    SCASSERT(item != nullptr);

    int pos = lineEdit->cursorPosition();
    lineEdit->setText(lineEdit->text().insert(lineEdit->cursorPosition(), item->text()));
    lineEdit->setFocus();
    lineEdit->setCursorPosition(pos + item->text().size());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::PutValHere(QPlainTextEdit *plainTextEdit, QListWidget *listWidget)
{
    SCASSERT(plainTextEdit != nullptr);
    SCASSERT(listWidget != nullptr);
    QListWidgetItem *item = listWidget->currentItem();
    SCASSERT(item != nullptr);

    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.insertText(item->text());
    plainTextEdit->setTextCursor(cursor);
    /*
    int pos = lineEdit->cursorPosition();
    lineEdit->setText(lineEdit->text().insert(lineEdit->cursorPosition(), item->text()));
    lineEdit->setFocus();
    lineEdit->setCursorPosition(pos + item->text().size());
    */
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
    if (edit->text().isEmpty())
    {
        flag = false;
        CheckState();
        QPalette palette = labelEditFormula->palette();
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        labelEditFormula->setPalette(palette);
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
    if (edit->toPlainText().isEmpty())
    {
        flag = false;
        CheckState();
        QPalette palette = labelEditFormula->palette();
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        labelEditFormula->setPalette(palette);
        return;
    }
    timer->start(1000);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Eval evaluate formula and show result
 * @param edit lineEdit of formula
 * @param flag flag state of formula
 * @param timer timer of formula
 * @param label label for signal error
 */
void DialogTool::Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label)
{
    SCASSERT(edit != nullptr);
    SCASSERT(timer != nullptr);
    SCASSERT(label != nullptr);
    SCASSERT(labelEditFormula != nullptr);
    QPalette palette = labelEditFormula->palette();
    if (edit->text().isEmpty())
    {
        flag = false;
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
    }
    else
    {
        try
        {
            const QString formula = qApp->FormulaFromUser(edit->text());
            Calculator *cal = new Calculator(data);
            const qreal result = cal->EvalFormula(formula);
            delete cal;

            QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                               QApplication::applicationName());
            bool osSeparatorValue = settings.value("configuration/osSeparator", 1).toBool();

            if (osSeparatorValue)
            {
                QLocale loc = QLocale::system();
                label->setText(loc.toString(result) + VDomDocument::UnitsToStr(qApp->patternUnit(), true));
            }
            else
            {
                QLocale loc = QLocale(QLocale::C);
                label->setText(loc.toString(result) + VDomDocument::UnitsToStr(qApp->patternUnit(), true));
            }
            flag = true;
            palette.setColor(labelEditFormula->foregroundRole(), QColor(76, 76, 76));
            emit ToolTip("");
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error"));
            flag = false;
            palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
            emit ToolTip("Parser error: "+e.GetMsg());
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    CheckState();
    timer->stop();
    labelEditFormula->setPalette(palette);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::Eval(QPlainTextEdit *edit, bool &flag, QTimer *timer, QLabel *label)
{
    SCASSERT(edit != nullptr);
    SCASSERT(timer != nullptr);
    SCASSERT(label != nullptr);
    SCASSERT(labelEditFormula != nullptr);
    QPalette palette = labelEditFormula->palette();
    if (edit->toPlainText().isEmpty())
    {
        flag = false;
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
    }
    else
    {
        try
        {
            // Replace line return with spaces for calc
            QString formula = edit->toPlainText();
            formula.replace("\n", " ");
            formula = qApp->FormulaFromUser(formula);
            Calculator *cal = new Calculator(data);
            const qreal result = cal->EvalFormula(formula);
            delete cal;

            QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                               QApplication::applicationName());
            bool osSeparatorValue = settings.value("configuration/osSeparator", 1).toBool();

            if (osSeparatorValue)
            {
                QLocale loc = QLocale::system();
                label->setText(loc.toString(result) + VDomDocument::UnitsToStr(qApp->patternUnit(), true));
            }
            else
            {
                QLocale loc = QLocale(QLocale::C);
                label->setText(loc.toString(result) + VDomDocument::UnitsToStr(qApp->patternUnit(), true));
            }
            flag = true;
            palette.setColor(labelEditFormula->foregroundRole(), QColor(76, 76, 76));
            emit ToolTip("");
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error"));
            flag = false;
            palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
            emit ToolTip("Parser error: "+e.GetMsg());
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    CheckState();
    timer->stop();
    labelEditFormula->setPalette(palette);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentPointId(QComboBox *box, quint32 &pointId, const quint32 &value, const quint32 &id) const
{
    SCASSERT(box != nullptr);
    FillComboBoxPoints(box, id);
    pointId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplineId set current spline id in combobox
 * @param box combobox
 * @param splineId save current spline id
 * @param value spline id
 * @param id don't show this id in list
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::setCurrentSplineId(QComboBox *box, quint32 &splineId, const quint32 &value, const quint32 &id,
                                    ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplines(box, id, cut);
    splineId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentArcId
 * @param box combobox
 * @param arcId save current arc id
 * @param value arc id
 * @param id don't show this id in list
 * @param cut if set to ComboMode::CutArc don't show id+1 and id+2
 */
void DialogTool::setCurrentArcId(QComboBox *box, quint32 &arcId, const quint32 &value, const quint32 &id,
                                 ComboBoxCutArc cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxArcs(box, id, cut);
    arcId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplinePathId set current splinePath id in combobox
 * @param box combobox
 * @param splinePathId save current splinePath id
 * @param value splinePath id
 * @param id don't show this id in list
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::setCurrentSplinePathId(QComboBox *box, quint32 &splinePathId, const quint32 &value,
                                        const quint32 &id, ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplinesPath(box, id, cut);
    splinePathId = value;
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
    Q_ASSERT(index != -1);
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
bool DialogTool::ChoosedPoint(const quint32 &id, QComboBox *box, const QString &toolTip)
{
    SCASSERT(box != nullptr);
    const VPointF *point = data->GeometricObject<const VPointF *>(id);
    SCASSERT(point != nullptr);
    const qint32 index = box->findText(point->name());
    if ( index != -1 )
    { // -1 for not found
        box->setCurrentIndex(index);
        emit ToolTip(toolTip);
        return true;
    }
    return false;
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
    bOk->setEnabled(flagFormula && flagName);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(flagFormula && flagName);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTool::ChoosedObject(quint32 id, const SceneObject &type)
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
        if (name.isEmpty() || name.contains(" "))
        {
            flagName = false;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), Qt::red);
            labelEditNamePoint->setPalette(palette);
        }
        else
        {
            flagName = true;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), QColor(76, 76, 76));
            labelEditNamePoint->setPalette(palette);
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogTool::DialogAccepted()
{
    emit DialogClosed(QDialog::Accepted);
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
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FormulaChangedPlainText()
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
    Eval(plainTextEditFormula, flagFormula, timerFormula, labelResultCalculation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SizeHeight show in list base variables
 */
void DialogTool::SizeHeight()
{
    SCASSERT(listWidget != nullptr);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();

    {
    QListWidgetItem *item = new QListWidgetItem(data->HeightName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    listWidget->addItem(item);
    }

    QListWidgetItem *item = new QListWidgetItem(data->SizeName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    listWidget->addItem(item);

    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->setCurrentRow (0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Measurements show in list measurements
 */
void DialogTool::Measurements()
{
    ShowVariable(data->DataMeasurements());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthLines show in list lengths of lines variables
 */
void DialogTool::LengthLines()
{
    ShowVariable(data->DataLengthLines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthArcs show in list lengths of arcs variables
 */
void DialogTool::LengthArcs()
{
    ShowVariable(data->DataLengthArcs());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthCurves show in list lengths of curves variables
 */
void DialogTool::LengthCurves()
{
    ShowVariable(data->DataLengthSplines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Increments show in list increment variables
 */
void DialogTool::Increments()
{
    ShowVariable(data->DataIncrements());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutHere put variable into edit
 */
void DialogTool::PutHere()
{
    PutValHere(plainTextEditFormula, listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutVal put variable into edit
 * @param item chosen item of list widget
 */
void DialogTool::PutVal(QListWidgetItem *item)
{
    SCASSERT(plainTextEditFormula != nullptr);
    SCASSERT(item != nullptr);
    QTextCursor cursor = plainTextEditFormula->textCursor();
    cursor.insertText(item->text());
    plainTextEditFormula->setTextCursor(cursor);
    /*int pos = plainTextEditFormula->cursorPosition();
    lineEditFormula->setText(lineEditFormula->text().insert(lineEditFormula->cursorPosition(),
                                                            item->text()));
    lineEditFormula->setFocus();
    lineEditFormula->setCursorPosition(pos + item->text().size());
    */
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValChenged show description when current variable changed
 * @param row number of row
 */
void DialogTool::ValChenged(int row)
{
    SCASSERT(listWidget != nullptr);
    SCASSERT(labelDescription != nullptr);
    SCASSERT(radioButtonSizeGrowth != nullptr);
    SCASSERT(radioButtonStandardTable != nullptr);
    SCASSERT(radioButtonIncrements != nullptr);
    SCASSERT(radioButtonLengthLine != nullptr);
    SCASSERT(radioButtonLengthArc != nullptr);
    SCASSERT(radioButtonLengthCurve != nullptr);
    if (listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = listWidget->item( row );
    if (radioButtonSizeGrowth->isChecked())
    {
        if (item->text()==data->HeightName())
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->height()).arg(tr("Height"));
            labelDescription->setText(desc);
        }
        if (item->text()==data->SizeName())
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg(tr("Size"));
            labelDescription->setText(desc);
        }
        return;
    }
    if (radioButtonStandardTable->isChecked())
    {
        QString name = qApp->VarFromUser(item->text());
        VMeasurement stable = data->GetMeasurement(name);
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueStandardTableRow(name))
                .arg(stable.GetGuiText());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonIncrements->isChecked())
    {
        VIncrement incr = data->GetIncrement(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueIncrementTableRow(item->text()))
                .arg(incr.getDescription());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthLine->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLine(qApp->VarFromUser(item->text())))
                .arg(tr("Line length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthArc->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLengthArc(qApp->VarFromUser(item->text())))
                .arg(tr("Arc length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthCurve->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(data->GetLengthSpline(qApp->VarFromUser(item->text()))).arg(tr("Curve length"));
        labelDescription->setText(desc);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateList update lists of variables
 */
void DialogTool::UpdateList()
{
    SCASSERT(radioButtonSizeGrowth != nullptr);
    SCASSERT(radioButtonStandardTable != nullptr);
    SCASSERT(radioButtonIncrements != nullptr);
    SCASSERT(radioButtonLengthLine != nullptr);
    SCASSERT(radioButtonLengthArc != nullptr);
    SCASSERT(radioButtonLengthCurve != nullptr);

    if (radioButtonSizeGrowth->isChecked())
    {
        SizeHeight();
    }
    if (radioButtonStandardTable->isChecked())
    {
        ShowVariable(data->DataMeasurements());
    }
    if (radioButtonIncrements->isChecked())
    {
        ShowVariable(data->DataIncrements());
    }
    if (radioButtonLengthLine->isChecked())
    {
        ShowVariable(data->DataLengthLines());
    }
    if (radioButtonLengthArc->isChecked())
    {
        ShowVariable(data->DataLengthArcs());
    }
    if (radioButtonLengthCurve->isChecked())
    {
        ShowVariable(data->DataLengthSplines());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowVariable show variables in list
 * @param var container with variables
 */
template <class key, class val>
void DialogTool::ShowVariable(const QHash<key, val> *var)
{
    SCASSERT(listWidget != nullptr);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();

    QHashIterator<key, val> i(*var);
    QMap<key, val> map;
    while (i.hasNext())
    {
        i.next();
        map.insert(qApp->VarToUser(i.key()), i.value());
    }

    QMapIterator<key, val> iMap(map);
    while (iMap.hasNext())
    {
        iMap.next();
        QListWidgetItem *item = new QListWidgetItem(iMap.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->setCurrentRow (0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DialogApply()
{

}
