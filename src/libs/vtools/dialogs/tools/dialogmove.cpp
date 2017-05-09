/************************************************************************
 **
 **  @file   dialogmove.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogmove.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLineF>
#include <QPlainTextEdit>
#include <QPointF>
#include <QPointer>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSharedPointer>
#include <QStringList>
#include <QTimer>
#include <QToolButton>
#include <Qt>
#include <new>

#include "../../visualization/visualization.h"
#include "../../visualization/line/operation/vistoolmove.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../qmuparser/qmudef.h"
#include "../support/dialogeditwrongformula.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "ui_dialogmove.h"

//---------------------------------------------------------------------------------------------------------------------
DialogMove::DialogMove(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogMove),
      flagAngle(false),
      flagLength(false),
      timerAngle(nullptr),
      timerLength(nullptr),
      formulaAngle(),
      formulaLength(),
      formulaBaseHeightAngle(0),
      formulaBaseHeightLength(0),
      objects(),
      stage1(true),
      m_suffix()
{
    ui->setupUi(this);

    this->formulaBaseHeightAngle = ui->plainTextEditAngle->height();
    ui->plainTextEditAngle->installEventFilter(this);

    this->formulaBaseHeightLength = ui->plainTextEditLength->height();
    ui->plainTextEditLength->installEventFilter(this);

    ui->lineEditSuffix->setText(qApp->getCurrentDocument()->GenerateSuffix());

    timerAngle = new QTimer(this);
    connect(timerAngle, &QTimer::timeout, this, &DialogMove::EvalAngle);

    timerLength = new QTimer(this);
    connect(timerLength, &QTimer::timeout, this, &DialogMove::EvalLength);

    InitOkCancelApply(ui);

    flagName = true;
    CheckState();

    connect(ui->lineEditSuffix, &QLineEdit::textChanged, this, &DialogMove::SuffixChanged);
    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogMove::FXAngle);
    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogMove::FXLength);
    connect(ui->plainTextEditAngle, &QPlainTextEdit::textChanged, this, &DialogMove::AngleChanged);
    connect(ui->plainTextEditLength, &QPlainTextEdit::textChanged, this, &DialogMove::LengthChanged);
    connect(ui->pushButtonGrowAngle, &QPushButton::clicked, this, &DialogMove::DeployAngleTextEdit);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogMove::DeployLengthTextEdit);

    vis = new VisToolMove(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogMove::~DialogMove()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogMove::GetAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(formulaAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SetAngle(const QString &value)
{
    formulaAngle = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditAngle->setPlainText(formulaAngle);

    VisToolMove *operation = qobject_cast<VisToolMove *>(vis);
    SCASSERT(operation != nullptr)
    operation->SetAngle(formulaAngle);

    MoveCursorToEnd(ui->plainTextEditAngle);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogMove::GetLength() const
{
    return qApp->TrVars()->TryFormulaFromUser(formulaLength, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SetLength(const QString &value)
{
    formulaLength = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formulaLength.length() > 80)
    {
        this->DeployLengthTextEdit();
    }
    ui->plainTextEditLength->setPlainText(formulaLength);

    VisToolMove *operation = qobject_cast<VisToolMove *>(vis);
    SCASSERT(operation != nullptr)
    operation->SetLength(formulaLength);

    MoveCursorToEnd(ui->plainTextEditLength);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogMove::GetSuffix() const
{
    return m_suffix;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SetSuffix(const QString &value)
{
    m_suffix = value;
    ui->lineEditSuffix->setText(value);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> DialogMove::GetObjects() const
{
    return objects.toVector();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::ShowDialog(bool click)
{
    if (stage1 && not click)
    {
        if (objects.isEmpty())
        {
            return;
        }

        stage1 = false;
        prepare = true;

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr)
        scene->clearSelection();

        VisToolMove *operation = qobject_cast<VisToolMove *>(vis);
        SCASSERT(operation != nullptr)
        operation->SetObjects(objects.toVector());
        operation->VisualMode();

        VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
        SCASSERT(window != nullptr)
        connect(operation, &VisToolMove::ToolTip, window, &VAbstractMainWindow::ShowToolTip);

        scene->ToggleArcSelection(false);
        scene->ToggleElArcSelection(false);
        scene->ToggleSplineSelection(false);
        scene->ToggleSplinePathSelection(false);

        scene->ToggleArcHover(false);
        scene->ToggleElArcHover(false);
        scene->ToggleSplineHover(false);
        scene->ToggleSplinePathHover(false);
    }
    else if (not stage1 && prepare && click)
    {
        VisToolMove *operation = qobject_cast<VisToolMove *>(vis);
        SCASSERT(operation != nullptr)

        if (operation->LengthValue() > 0)
        {
            SetAngle(operation->Angle());//Show in dialog angle that a user choose
            SetLength(operation->Length());
            setModal(true);
            emit ToolTip("");
            timerAngle->start();
            timerLength->start();
            show();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::ChosenObject(quint32 id, const SceneObject &type)
{
    Q_UNUSED(id)
    Q_UNUSED(type)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SelectedObject(bool selected, quint32 object, quint32 tool)
{
    Q_UNUSED(tool)
    if (stage1)
    {
        if (selected)
        {
            if (not objects.contains(object))
            {
                objects.append(object);
            }
        }
        else
        {
            objects.removeOne(object);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditAngle, ui->pushButtonGrowAngle, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::DeployLengthTextEdit()
{
    DeployFormula(ui->plainTextEditLength, ui->pushButtonGrowLength, formulaBaseHeightLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::AngleChanged()
{
    labelEditFormula = ui->labelEditAngle;
    labelResultCalculation = ui->labelResultAngle;
    ValFormulaChanged(flagAngle, ui->plainTextEditAngle, timerAngle, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::LengthChanged()
{
    labelEditFormula = ui->labelEditLength;
    labelResultCalculation = ui->labelResultLength;
    ValFormulaChanged(flagLength, ui->plainTextEditLength, timerLength, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::FXAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit angle"));
    dialog->SetFormula(GetAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(GetLength());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetLength(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SuffixChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        const QString suffix = edit->text();
        if (suffix.isEmpty())
        {
            flagName = false;
            ChangeColor(ui->labelSuffix, Qt::red);
            CheckState();
            return;
        }
        else
        {
            if (m_suffix != suffix)
            {
                QRegularExpression rx(NameRegExp());
                const QStringList uniqueNames = VContainer::AllUniqueNames();
                for (int i=0; i < uniqueNames.size(); ++i)
                {
                    const QString name = uniqueNames.at(i) + suffix;
                    if (not rx.match(name).hasMatch() || not data->IsUnique(name))
                    {
                        flagName = false;
                        ChangeColor(ui->labelSuffix, Qt::red);
                        CheckState();
                        return;
                    }
                }
            }
        }

        flagName = true;
        ChangeColor(ui->labelSuffix, okColor);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(flagAngle && flagLength && flagName);
    SCASSERT(bApply != nullptr)
    bApply->setEnabled(bOk->isEnabled());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::ShowVisualization()
{
    AddVisualization<VisToolMove>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::SaveData()
{
    m_suffix = ui->lineEditSuffix->text();

    formulaAngle = ui->plainTextEditAngle->toPlainText();
    formulaAngle.replace("\n", " ");

    formulaLength = ui->plainTextEditLength->toPlainText();
    formulaLength.replace("\n", " ");

    VisToolMove *operation = qobject_cast<VisToolMove *>(vis);
    SCASSERT(operation != nullptr)

    operation->SetObjects(objects.toVector());
    operation->SetAngle(formulaAngle);
    operation->SetLength(formulaLength);
    operation->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditAngle->blockSignals(true);
    ui->plainTextEditLength->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::EvalAngle()
{
    labelEditFormula = ui->labelEditAngle;
    Eval(ui->plainTextEditAngle->toPlainText(), flagAngle, ui->labelResultAngle, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMove::EvalLength()
{
    labelEditFormula = ui->labelEditLength;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    Eval(ui->plainTextEditLength->toPlainText(), flagLength, ui->labelResultLength, postfix);
}
