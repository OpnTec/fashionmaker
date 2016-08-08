/************************************************************************
 **
 **  @file   dialogspline.cpp
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

#include "dialogspline.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QToolButton>
#include <Qt>
#include <new>

#include "../../visualization/path/vistoolspline.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "dialogs/tools/../../visualization/path/../visualization.h"
#include "ui_dialogspline.h"
#include "vtranslatevars.h"

class QCloseEvent;
class QWidget;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogSpline create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogSpline::DialogSpline(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSpline),
      spl(),
      newDuplicate(-1),
      formulaBaseHeightAngle1(0),
      formulaBaseHeightAngle2(0),
      formulaBaseHeightLength1(0),
      formulaBaseHeightLength2(0),
      timerAngle1(new QTimer(this)),
      timerAngle2(new QTimer(this)),
      timerLength1(new QTimer(this)),
      timerLength2(new QTimer(this)),
      flagAngle1(false),
      flagAngle2(false),
      flagLength1(false),
      flagLength2(false)
{
    ui->setupUi(this);

    plainTextEditFormula = ui->plainTextEditAngle1F;

    formulaBaseHeightAngle1 = ui->plainTextEditAngle1F->height();
    formulaBaseHeightAngle2 = ui->plainTextEditAngle2F->height();
    formulaBaseHeightLength1 = ui->plainTextEditLength1F->height();
    formulaBaseHeightLength2 = ui->plainTextEditLength2F->height();

    ui->plainTextEditAngle1F->installEventFilter(this);
    ui->plainTextEditAngle2F->installEventFilter(this);
    ui->plainTextEditLength1F->installEventFilter(this);
    ui->plainTextEditLength2F->installEventFilter(this);

    connect(timerAngle1, &QTimer::timeout, this, &DialogSpline::EvalAngle1);
    connect(timerAngle2, &QTimer::timeout, this, &DialogSpline::EvalAngle2);
    connect(timerLength1, &QTimer::timeout, this, &DialogSpline::EvalLength1);
    connect(timerLength2, &QTimer::timeout, this, &DialogSpline::EvalLength2);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP4);
    FillComboBoxLineColors(ui->comboBoxColor);

    CheckState();

    connect(ui->comboBoxP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);
    connect(ui->comboBoxP4, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);

    connect(ui->toolButtonExprAngle1, &QPushButton::clicked, this, &DialogSpline::FXAngle1);
    connect(ui->toolButtonExprAngle2, &QPushButton::clicked, this, &DialogSpline::FXAngle2);
    connect(ui->toolButtonExprLength1, &QPushButton::clicked, this, &DialogSpline::FXLength1);
    connect(ui->toolButtonExprLength2, &QPushButton::clicked, this, &DialogSpline::FXLength2);

    connect(ui->plainTextEditAngle1F, &QPlainTextEdit::textChanged, this, &DialogSpline::Angle1Changed);
    connect(ui->plainTextEditAngle2F, &QPlainTextEdit::textChanged, this, &DialogSpline::Angle2Changed);
    connect(ui->plainTextEditLength1F, &QPlainTextEdit::textChanged, this, &DialogSpline::Length1Changed);
    connect(ui->plainTextEditLength2F, &QPlainTextEdit::textChanged, this, &DialogSpline::Length2Changed);

    connect(ui->pushButtonGrowAngle1, &QPushButton::clicked, this, &DialogSpline::DeployAngle1TextEdit);
    connect(ui->pushButtonGrowAngle2, &QPushButton::clicked, this, &DialogSpline::DeployAngle2TextEdit);
    connect(ui->pushButtonGrowLength1, &QPushButton::clicked, this, &DialogSpline::DeployLength1TextEdit);
    connect(ui->pushButtonGrowLength2, &QPushButton::clicked, this, &DialogSpline::DeployLength2TextEdit);

    vis = new VisToolSpline(data);
    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    auto scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);
    connect(scene, &VMainGraphicsScene::MouseLeftPressed, path, &VisToolSpline::MouseLeftPressed);
    connect(scene, &VMainGraphicsScene::MouseLeftReleased, path, &VisToolSpline::MouseLeftReleased);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSpline::~DialogSpline()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogSpline::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            auto *path = qobject_cast<VisToolSpline *>(vis);
            SCASSERT(path != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP1, tr("Select last point of curve")))
                    {
                        ++number;
                        path->VisualMode(id);
                    }
                    break;
                case 1:
                {
                    if (getCurrentObjectId(ui->comboBoxP1) != id)
                    {
                        if (SetObject(id, ui->comboBoxP4, ""))
                        {
                            ++number;

                            path->setObject4Id(id);
                            path->RefreshGeometry();
                            prepare = true;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SaveData()
{
    const quint32 d = spl.GetDuplicate();//Save previous value
    spl = CurrentSpline();

    newDuplicate <= -1 ? spl.SetDuplicate(d) : spl.SetDuplicate(static_cast<quint32>(newDuplicate));

    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetP1()->id());
    path->setObject4Id(GetP4()->id());
    path->SetAngle1(spl.GetStartAngle());
    path->SetAngle2(spl.GetEndAngle());
    path->SetKAsm1(spl.GetKasm1());
    path->SetKAsm2(spl.GetKasm2());
    path->SetKCurve(spl.GetKcurve());
    path->SetMode(Mode::Show);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditAngle1F->blockSignals(true);
    ui->plainTextEditAngle2F->blockSignals(true);
    ui->plainTextEditLength1F->blockSignals(true);
    ui->plainTextEditLength2F->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::DeployAngle1TextEdit()
{
    DeployFormula(ui->plainTextEditAngle1F, ui->pushButtonGrowAngle1, formulaBaseHeightAngle1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::DeployAngle2TextEdit()
{
    DeployFormula(ui->plainTextEditAngle2F, ui->pushButtonGrowAngle2, formulaBaseHeightAngle2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::DeployLength1TextEdit()
{
    DeployFormula(ui->plainTextEditLength1F, ui->pushButtonGrowLength1, formulaBaseHeightLength1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::DeployLength2TextEdit()
{
    DeployFormula(ui->plainTextEditLength2F, ui->pushButtonGrowLength2, formulaBaseHeightLength2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::Angle1Changed()
{
    labelEditFormula = ui->labelEditAngle1;
    labelResultCalculation = ui->labelResultAngle1;
    ValFormulaChanged(flagAngle1, ui->plainTextEditAngle1F, timerAngle1, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::Angle2Changed()
{
    labelEditFormula = ui->labelEditAngle2;
    labelResultCalculation = ui->labelResultAngle2;
    ValFormulaChanged(flagAngle2, ui->plainTextEditAngle2F, timerAngle2, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::Length1Changed()
{
    labelEditFormula = ui->labelEditLength1;
    labelResultCalculation = ui->labelResultLength1;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagLength1, ui->plainTextEditLength1F, timerLength1, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::Length2Changed()
{
    labelEditFormula = ui->labelEditLength2;
    labelResultCalculation = ui->labelResultLength2;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagLength2, ui->plainTextEditLength2F, timerLength2, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::FXAngle1()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first control point angle"));
    QString angle1F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditAngle1F->toPlainText(),
                                                         qApp->Settings()->GetOsSeparator());
    dialog->SetFormula(angle1F);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        angle1F = qApp->TrVars()->FormulaToUser(dialog->GetFormula(), qApp->Settings()->GetOsSeparator());
        // increase height if needed.
        if (angle1F.length() > 80)
        {
            DeployAngle1TextEdit();
        }
        ui->plainTextEditAngle1F->setPlainText(angle1F);
        MoveCursorToEnd(ui->plainTextEditAngle1F);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::FXAngle2()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second control point angle"));
    QString angle2F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditAngle2F->toPlainText(),
                                                         qApp->Settings()->GetOsSeparator());
    dialog->SetFormula(angle2F);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        angle2F = qApp->TrVars()->FormulaToUser(dialog->GetFormula(), qApp->Settings()->GetOsSeparator());
        // increase height if needed.
        if (angle2F.length() > 80)
        {
            DeployAngle1TextEdit();
        }
        ui->plainTextEditAngle2F->setPlainText(angle2F);
        MoveCursorToEnd(ui->plainTextEditAngle2F);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::FXLength1()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first control point length"));
    QString length1F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditLength1F->toPlainText(),
                                                          qApp->Settings()->GetOsSeparator());
    dialog->SetFormula(length1F);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        length1F = qApp->TrVars()->FormulaToUser(dialog->GetFormula(), qApp->Settings()->GetOsSeparator());
        // increase height if needed.
        if (length1F.length() > 80)
        {
            DeployLength1TextEdit();
        }
        ui->plainTextEditLength1F->setPlainText(length1F);
        MoveCursorToEnd(ui->plainTextEditLength1F);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::FXLength2()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second control point length"));
    QString length2F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditLength2F->toPlainText(),
                                                          qApp->Settings()->GetOsSeparator());
    dialog->SetFormula(length2F);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        length2F = qApp->TrVars()->FormulaToUser(dialog->GetFormula(), qApp->Settings()->GetOsSeparator());
        // increase height if needed.
        if (length2F.length() > 80)
        {
            DeployLength2TextEdit();
        }
        ui->plainTextEditLength2F->setPlainText(length2F);
        MoveCursorToEnd(ui->plainTextEditLength2F);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogSpline::GetP1() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP1));
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogSpline::GetP4() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP4));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::EvalAngle1()
{
    labelEditFormula = ui->labelEditAngle1;
    Eval(ui->plainTextEditAngle1F->toPlainText(), flagAngle1, ui->labelResultAngle1, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::EvalAngle2()
{
    labelEditFormula = ui->labelEditAngle2;
    Eval(ui->plainTextEditAngle2F->toPlainText(), flagAngle2, ui->labelResultAngle2, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::EvalLength1()
{
    labelEditFormula = ui->labelEditLength1;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal length1 = Eval(ui->plainTextEditLength1F->toPlainText(), flagLength1, ui->labelResultLength1, postfix);

    if (length1 < 0)
    {
        flagLength1 = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultLength1->setText(tr("Error") + " (" + postfix + ")");
        ui->labelResultLength1->setToolTip(tr("Length can't be negative"));

        DialogSpline::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::EvalLength2()
{
    labelEditFormula = ui->labelEditLength2;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal length2 = Eval(ui->plainTextEditLength2F->toPlainText(), flagLength2, ui->labelResultLength2, postfix);

    if (length2 < 0)
    {
        flagLength2 = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultLength2->setText(tr("Error") + " (" + postfix + ")");
        ui->labelResultLength2->setToolTip(tr("Length can't be negative"));

        DialogSpline::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VSpline DialogSpline::CurrentSpline() const
{
    QString angle1F = ui->plainTextEditAngle1F->toPlainText();
    angle1F.replace("\n", " ");

    QString angle2F = ui->plainTextEditAngle2F->toPlainText();
    angle2F.replace("\n", " ");

    QString length1F = ui->plainTextEditLength1F->toPlainText();
    length1F.replace("\n", " ");

    QString length2F = ui->plainTextEditLength2F->toPlainText();
    length2F.replace("\n", " ");

    const QHash<QString, qreal *> vars = data->PlainVariables();

    const qreal angle1 = Visualization::FindVal(angle1F, vars);
    const qreal angle2 = Visualization::FindVal(angle2F, vars);
    const qreal length1 = Visualization::FindLength(length1F, vars);
    const qreal length2 = Visualization::FindLength(length2F, vars);

    const bool separator = qApp->Settings()->GetOsSeparator();

    angle1F = qApp->TrVars()->TryFormulaFromUser(angle1F, separator);
    angle2F = qApp->TrVars()->TryFormulaFromUser(angle2F, separator);
    length1F = qApp->TrVars()->TryFormulaFromUser(length1F, separator);
    length2F = qApp->TrVars()->TryFormulaFromUser(length2F, separator);

    VSpline spline(*GetP1(), *GetP4(), angle1, angle1F, angle2, angle2F, length1, length1F,  length2, length2F);

    return spline;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxP1));
    set.insert(getCurrentObjectId(ui->comboBoxP4));

    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxP1) == getCurrentObjectId(ui->comboBoxP4))
    {
        flagError = false;
        color = errorColor;

        ui->lineEditSplineName->setText(tr("Invalid spline"));
    }
    else
    {
        flagError = true;
        color = okColor;

        if (getCurrentObjectId(ui->comboBoxP1) == spl.GetP1().id() &&
            getCurrentObjectId(ui->comboBoxP4) == spl.GetP4().id())
        {
            newDuplicate = -1;
            ui->lineEditSplineName->setText(spl.name());
        }
        else
        {
            VSpline spline(*GetP1(), *GetP4(), spl.GetStartAngle(), spl.GetEndAngle(), spl.GetKasm1(), spl.GetKasm2(),
                           spl.GetKcurve());

            if (not data->IsUnique(spline.name()))
            {
                newDuplicate = static_cast<qint32>(DNumber(spline.name()));
                spline.SetDuplicate(static_cast<quint32>(newDuplicate));
            }
            ui->lineEditSplineName->setText(spline.name());
        }
    }
    ChangeColor(ui->labelName, color);
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::ShowDialog(bool click)
{
    if (prepare && click)
    {
        auto *path = qobject_cast<VisToolSpline *>(vis);
        SCASSERT(path != nullptr);

        spl = VSpline(*GetP1(), path->GetP2(), path->GetP3(), *GetP4());

        const QString angle1F = qApp->TrVars()->FormulaToUser(spl.GetStartAngleFormula(),
                                                              qApp->Settings()->GetOsSeparator());
        const QString angle2F = qApp->TrVars()->FormulaToUser(spl.GetEndAngleFormula(),
                                                              qApp->Settings()->GetOsSeparator());

        ui->plainTextEditAngle1F->setPlainText(angle1F);
        ui->plainTextEditAngle2F->setPlainText(angle2F);
        ui->plainTextEditLength1F->setPlainText(qApp->TrVars()->FormulaToUser(spl.GetC1LengthFormula(),
                                                                              qApp->Settings()->GetOsSeparator()));
        ui->plainTextEditLength2F->setPlainText(qApp->TrVars()->FormulaToUser(spl.GetC2LengthFormula(),
                                                                              qApp->Settings()->GetOsSeparator()));

        if (not data->IsUnique(spl.name()))
        {
            spl.SetDuplicate(DNumber(spl.name()));
        }

        ui->lineEditSplineName->setText(spl.name());

        DialogAccepted();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagAngle1 && flagAngle2 && flagLength1 && flagLength2 && flagError);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::ShowVisualization()
{
    AddVisualization<VisToolSpline>();
}

//---------------------------------------------------------------------------------------------------------------------
VSpline DialogSpline::GetSpline() const
{
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SetSpline(const VSpline &spline)
{
    spl = spline;

    setCurrentPointId(ui->comboBoxP1, spl.GetP1().id());
    setCurrentPointId(ui->comboBoxP4, spl.GetP4().id());

    const QString angle1F = qApp->TrVars()->FormulaToUser(spl.GetStartAngleFormula(),
                                                          qApp->Settings()->GetOsSeparator());
    const QString angle2F = qApp->TrVars()->FormulaToUser(spl.GetEndAngleFormula(),
                                                          qApp->Settings()->GetOsSeparator());

    ui->plainTextEditAngle1F->setPlainText(angle1F);
    ui->plainTextEditAngle2F->setPlainText(angle2F);

    const QString length1F = qApp->TrVars()->FormulaToUser(spl.GetC1LengthFormula(),
                                                           qApp->Settings()->GetOsSeparator());
    const QString length2F = qApp->TrVars()->FormulaToUser(spl.GetC2LengthFormula(),
                                                           qApp->Settings()->GetOsSeparator());

    ui->plainTextEditLength1F->setPlainText(length1F);
    ui->plainTextEditLength2F->setPlainText(length2F);
    ui->lineEditSplineName->setText(spl.name());

    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(spl.GetP1().id());
    path->setObject4Id(spl.GetP4().id());
    path->SetAngle1(spl.GetStartAngle());
    path->SetAngle2(spl.GetEndAngle());
    path->SetKAsm1(spl.GetKasm1());
    path->SetKAsm2(spl.GetKasm2());
    path->SetKCurve(spl.GetKcurve());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSpline::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}
