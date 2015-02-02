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
#include "../../core/vapplication.h"
#include "../../core/vsettings.h"
#include "../../libs/ifc/xml/vdomdocument.h"
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
      listWidget(nullptr), labelResultCalculation(nullptr), labelDescription(nullptr), labelEditNamePoint(nullptr),
      labelEditFormula(nullptr), radioButtonSizeGrowth(nullptr), radioButtonStandardTable(nullptr),
      radioButtonIncrements(nullptr), radioButtonLengthLine(nullptr), radioButtonLengthArc(nullptr),
      radioButtonLengthCurve(nullptr), radioButtonAngleLine(nullptr), checkBoxHideEmpty(nullptr),
      okColor(QColor(76, 76, 76)), errorColor(Qt::red), associatedTool(nullptr),
      toolId(toolId), prepare(false), pointName(QString())
{
    SCASSERT(data != nullptr);
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
    this->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
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
    isInitialized = true;//first show windows are held
    ShowVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxPoints fill comboBox list of points
 * @param box comboBox
 */
void DialogTool::FillComboBoxPoints(QComboBox *box) const
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
            if (obj->getType() == GOType::Point && obj->getMode() == Draw::Calculation)
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxArcs(QComboBox *box, ComboBoxCutArc cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (cut == ComboBoxCutArc::CutArc)
        {
            if (i.key() != toolId + 1 && i.key() != toolId + 2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::Arc && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(i.key());
                    list[arc->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != toolId)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::Arc && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(i.key());
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
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::FillComboBoxSplines(QComboBox *box, ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (cut == ComboBoxCutSpline::CutSpline)
        {
            if (i.key() != toolId + 1 && i.key() != toolId + 2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::Spline && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VSpline> spl = data->GeometricObject<VSpline>(i.key());
                    list[spl->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != toolId)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::Spline && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VSpline> spl = data->GeometricObject<VSpline>(i.key());
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
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::FillComboBoxSplinesPath(QComboBox *box, ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (cut == ComboBoxCutSpline::CutSpline)
        {
            if (i.key() != toolId + 1 && i.key() != toolId + 2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(i.key());
                    list[splPath->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != toolId)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == GOType::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(i.key());
                    list[splPath->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
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
                list[curve->name()] = i.key();
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

    box->setCurrentIndex(1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxLineColors(QComboBox *box) const
{
    SCASSERT(box != nullptr);

    const QStringList colorNames = VAbstractTool::Colors();

    for (int i = 0; i < colorNames.size(); ++i)
    {
        QString name;
        switch (i)
        {
            case 0: // ColorBlack
                name = tr("black");
                break;
            case 1: // ColorGreen
                name = tr("green");
                break;
            case 2: // ColorBlue
                name = tr("blue");
                break;
            case 3: // ColorDarkRed
                name = tr("dark red");
                break;
            case 4: // ColorDarkGreen
                name = tr("dark green");
                break;
            case 5: // ColorDarkBlue
                name = tr("dark blue");
                break;
            case 6: // ColorYellow
                name = tr("yellow");
                break;
            default:
                name = tr("black");
                break;
        }

        QPixmap pix(16, 16);
        pix.fill(QColor(colorNames.at(i)));
        box->insertItem(i, QIcon(pix), name, QVariant(colorNames.at(i)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @param box combobox
 * @return type
 */
QString DialogTool::GetTypeLine(const QComboBox *box) const
{
    SCASSERT(box != nullptr)
    QString value = box->currentData().toString();
    if (value.isEmpty())
    {
        value = VAbstractTool::TypeLineLine;
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::GetLineColor(const QComboBox *box) const
{
    SCASSERT(box != nullptr)
    QString value = box->currentData().toString();
    if (value.isEmpty())
    {
        value = VAbstractTool::ColorBlack;
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetupTypeLine setupe type of line
 * @param box combobox
 * @param value string from pattern file
 */
void DialogTool::SetupTypeLine(QComboBox *box, const QString &value)
{
    SCASSERT(box != nullptr)
    const qint32 index = box->findData(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
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
    SCASSERT(box != nullptr)
    const qint32 index = box->findData(value);
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
    if (item != nullptr)
    {
        int pos = lineEdit->cursorPosition();
        lineEdit->setText(lineEdit->text().insert(lineEdit->cursorPosition(), item->text()));
        lineEdit->setFocus();
        lineEdit->setCursorPosition(pos + item->text().size());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::PutValHere(QPlainTextEdit *plainTextEdit, QListWidget *listWidget)
{
    SCASSERT(plainTextEdit != nullptr);
    SCASSERT(listWidget != nullptr);
    QListWidgetItem *item = listWidget->currentItem();
    if (item != nullptr)
    {
        QTextCursor cursor = plainTextEdit->textCursor();
        cursor.insertText(item->text());
        plainTextEdit->setTextCursor(cursor);
        plainTextEdit->setFocus();
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
 * @param text formula
 * @param flag flag state of formula
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
        label->setText(tr("Error"));
        label->setToolTip(tr("Empty field"));
    }
    else
    {
        try
        {
            // Replace line return with spaces for calc if exist
            QString formula = text;
            formula.replace("\n", " ");
            formula = qApp->FormulaFromUser(formula);
            Calculator *cal = new Calculator(data);
            result = cal->EvalFormula(formula);
            delete cal;

            //if result equal 0
            if (checkZero && qFuzzyCompare(1 + result, 1 + 0))
            {
                flag = false;
                ChangeColor(labelEditFormula, Qt::red);
                label->setText(tr("Error"));
                label->setToolTip(tr("Value can't be 0"));
            }
            else
            {
                QLocale loc;
                if (qApp->getSettings()->GetOsSeparator())
                {
                    loc = QLocale::system();
                }
                else
                {
                    loc = QLocale(QLocale::C);
                }
                label->setText(loc.toString(result) + postfix);
                flag = true;
                ChangeColor(labelEditFormula, okColor);
                label->setToolTip(tr("Value"));
                emit ToolTip("");
            }
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error"));
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
    CheckState();
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentPointId(QComboBox *box, quint32 &pointId, const quint32 &value) const
{
    SCASSERT(box != nullptr);
    FillComboBoxPoints(box);
    pointId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplineId set current spline id in combobox
 * @param box combobox
 * @param splineId save current spline id
 * @param value spline id
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::setCurrentSplineId(QComboBox *box, quint32 &splineId, const quint32 &value,
                                    ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplines(box, cut);
    splineId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentArcId
 * @param box combobox
 * @param arcId save current arc id
 * @param value arc id
 * @param cut if set to ComboMode::CutArc don't show id+1 and id+2
 */
void DialogTool::setCurrentArcId(QComboBox *box, quint32 &arcId, const quint32 &value, ComboBoxCutArc cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxArcs(box, cut);
    arcId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplinePathId set current splinePath id in combobox
 * @param box combobox
 * @param splinePathId save current splinePath id
 * @param value splinePath id
 * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
 */
void DialogTool::setCurrentSplinePathId(QComboBox *box, quint32 &splinePathId, const quint32 &value,
                                        ComboBoxCutSpline cut) const
{
    SCASSERT(box != nullptr);
    FillComboBoxSplinesPath(box, cut);
    splinePathId = value;
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentCurveId(QComboBox *box, quint32 &curveId, const quint32 &value) const
{
    SCASSERT(box != nullptr);
    FillComboBoxCurves(box);
    curveId = value;
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
bool DialogTool::SetObject(const quint32 &id, QComboBox *box, const QString &toolTip)
{
    SCASSERT(box != nullptr);
    const QSharedPointer<VGObject> obj = data->GetGObject(id);
    SCASSERT(obj.isNull() == false);
    const qint32 index = box->findText(obj->name());
    if ( index != -1 )
    { // -1 for not found
        box->setCurrentIndex(index);
        emit ToolTip(toolTip);
        return true;
    }
    else
    {
        qWarning()<<"Can't find object by name"<<obj->name();
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DeployFormula(QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight)
{
    SCASSERT(formula != nullptr);
    SCASSERT(buttonGrowLength != nullptr)
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
        name.replace(" ", "");
        QRegExpValidator v(QRegExp(nameRegExp), this);
        int pos = 0;
        if (name.isEmpty() || (pointName != name && data->IsUnique(name) == false) ||
                v.validate(name, pos) == QValidator::Invalid)
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
void DialogTool::setPointId(QComboBox *box, quint32 &pointId, const quint32 &value)
{
    SCASSERT(box != nullptr);
    box->blockSignals(true);
    setCurrentPointId(box, pointId, value);
    box->blockSignals(false);
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
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());
    Eval(plainTextEditFormula->toPlainText(), flagFormula, labelResultCalculation, postfix, false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SizeHeight show in list base variables
 */
void DialogTool::SizeHeight()
{
    SCASSERT(listWidget != nullptr);
    listWidget->blockSignals(true);
    listWidget->clear();

    {
    QListWidgetItem *item = new QListWidgetItem(data->HeightName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    listWidget->addItem(item);
    }

    QListWidgetItem *item = new QListWidgetItem(data->SizeName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    listWidget->addItem(item);

    listWidget->blockSignals(false);
    listWidget->setCurrentRow (0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Measurements show in list measurements
 */
void DialogTool::Measurements()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(true);
    ShowVariable(data->DataMeasurements());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthLines show in list lengths of lines variables
 */
void DialogTool::LengthLines()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthLines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthArcs show in list lengths of arcs variables
 */
void DialogTool::LengthArcs()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthArcs());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthCurves show in list lengths of curves variables
 */
void DialogTool::LengthCurves()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthSplines());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::AngleLines()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataAngleLines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Increments show in list increment variables
 */
void DialogTool::Increments()
{
    SCASSERT(checkBoxHideEmpty != nullptr);
    checkBoxHideEmpty->setEnabled(false);
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
        QSharedPointer<VMeasurement> stable = data->GetVariable<VMeasurement>(name);
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetTableValue(name))
                .arg(stable->GetGuiText());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonIncrements->isChecked())
    {
        QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetTableValue(item->text()))
                .arg(incr->GetDescription());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthLine->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VLengthLine>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Line length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthArc->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VArcLength>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Arc length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthCurve->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VSplineLength>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Curve length"));
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
/**
 * @brief ShowVariable show variables in list
 * @param var container with variables
 */
template <class key, class val>
void DialogTool::ShowVariable(const QMap<key, val> var)
{
    SCASSERT(listWidget != nullptr);
    SCASSERT(checkBoxHideEmpty != nullptr);
    listWidget->blockSignals(true);
    listWidget->clear();

    QMapIterator<key, val> iMap(var);
    while (iMap.hasNext())
    {
        iMap.next();
        if (checkBoxHideEmpty->isEnabled() && checkBoxHideEmpty->isChecked() && iMap.value()->IsNotUsed())
        {
            continue; //skip this measurement
        }
        if (iMap.value()->Filter(toolId) == false)
        {// If we create this variable don't show
            QListWidgetItem *item = new QListWidgetItem(iMap.key());
            item->setFont(QFont("Times", 12, QFont::Bold));
            listWidget->addItem(item);
        }
    }
    listWidget->blockSignals(false);
    listWidget->setCurrentRow (0);
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
