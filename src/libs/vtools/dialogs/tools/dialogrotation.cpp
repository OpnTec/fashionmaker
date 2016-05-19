/************************************************************************
 **
 **  @file   dialogrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
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

#include "dialogrotation.h"
#include "ui_dialogrotation.h"

#include <QTimer>

#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../ifc/xml/vdomdocument.h"
#include "../../visualization/line/vistoolrotation.h"
#include "../support/dialogeditwrongformula.h"
#include "../qmuparser/qmudef.h"

//---------------------------------------------------------------------------------------------------------------------
DialogRotation::DialogRotation(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogRotation),
      flagAngle(false),
      timerAngle(nullptr),
      formulaAngle(),
      formulaBaseHeightAngle(0),
      objects(),
      stage1(true)
{
    ui->setupUi(this);

    this->formulaBaseHeightAngle = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    ui->lineEditSuffix->setText(qApp->getCurrentDocument()->GenerateSuffix());

    timerAngle = new QTimer(this);
    connect(timerAngle, &QTimer::timeout, this, &DialogRotation::EvalAngle);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxOriginPoint);

    flagName = true;
    CheckState();

    connect(ui->lineEditSuffix, &QLineEdit::textChanged, this, &DialogRotation::SuffixChanged);
    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogRotation::FXAngle);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogRotation::AngleChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogRotation::DeployAngleTextEdit);
    connect(ui->comboBoxOriginPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogRotation::PointChanged);

    vis = new VisToolRotation(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogRotation::~DialogRotation()
{
    DeleteVisualization<VisToolRotation>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogRotation::GetOrigPointId() const
{
    return getCurrentObjectId(ui->comboBoxOriginPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetOrigPointId(const quint32 &value)
{
    ChangeCurrentData(ui->comboBoxOriginPoint, value);
    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr);
    operation->SetOriginPointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogRotation::GetAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(formulaAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetAngle(const QString &value)
{
    formulaAngle = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed.
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaAngle);

    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr);
    operation->SetAngle(formulaAngle);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogRotation::GetSuffix() const
{
    return ui->lineEditSuffix->text();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetSuffix(const QString &value)
{
    ui->lineEditSuffix->setText(value);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> DialogRotation::GetObjects() const
{
    return objects.toVector();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::ShowDialog(bool click)
{
    if (stage1 && not click)
    {
        if (objects.isEmpty())
        {
            return;
        }

        stage1 = false;

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr);
        scene->clearSelection();

        VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(operation != nullptr);
        operation->SetObjects(objects.toVector());
        operation->VisualMode();

        scene->ToggleArcSelection(false);
        scene->ToggleSplineSelection(false);
        scene->ToggleSplinePathSelection(false);

        scene->ToggleArcHover(false);
        scene->ToggleSplineHover(false);
        scene->ToggleSplinePathHover(false);

        emit ToolTip("Select origin point");
    }
    else if (not stage1 && prepare && click)
    {
        /*We will ignore click if pointer is in point circle*/
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr);
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(GetOrigPointId());
        const QLineF line = QLineF(*point, scene->getScenePos());

        //Radius of point circle, but little bigger. Need handle with hover sizes.
        const qreal radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;
        if (line.length() <= radius)
        {
            return;
        }

        VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(operation != nullptr);

        SetAngle(operation->Angle());//Show in dialog angle that a user choose
        setModal(true);
        emit ToolTip("");
        timerAngle->start();
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not stage1 && not prepare)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (objects.contains(id))
            {
                return;
            }

            if (SetObject(id, ui->comboBoxOriginPoint, ""))
            {
                VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
                SCASSERT(operation != nullptr);

                connect(operation, &Visualization::ToolTip, this, &DialogTool::ShowVisToolTip);

                operation->SetOriginPointId(id);
                operation->RefreshGeometry();

                prepare = true;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SelectedObject(bool selected, quint32 object, quint32 tool)
{
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
void DialogRotation::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::AngleChanged()
{
    labelEditFormula = ui->labelEditAngle;
    labelResultCalculation = ui->labelResultAngle;
    ValFormulaChanged(flagAngle, ui->plainTextEditFormula, timerAngle, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::FXAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit angle"));
    dialog->SetFormula(GetAngle());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SuffixChanged()
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
            QRegularExpression rx(NameRegExp());
            const QStringList uniqueNames = data->AllUniqueNames();
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

        flagName = true;
        ChangeColor(ui->labelSuffix, okColor);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagAngle && flagName);
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(bOk->isEnabled());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::ShowVisualization()
{
    AddVisualization<VisToolRotation>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SaveData()
{
    formulaAngle = ui->plainTextEditFormula->toPlainText();
    formulaAngle.replace("\n", " ");

    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr);

    operation->SetObjects(objects.toVector());
    operation->SetOriginPointId(GetOrigPointId());
    operation->SetAngle(formulaAngle);
    operation->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::PointChanged()
{
    QColor color = okColor;
    if (objects.contains(getCurrentObjectId(ui->comboBoxOriginPoint)))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelOriginPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::EvalAngle()
{
    labelEditFormula = ui->labelEditAngle;
    Eval(ui->plainTextEditFormula->toPlainText(), flagAngle, ui->labelResultAngle, degreeSymbol, false);
}
