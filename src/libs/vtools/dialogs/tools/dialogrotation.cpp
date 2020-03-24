/************************************************************************
 **
 **  @file   dialogrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include <QColor>
#include <QComboBox>
#include <QCompleter>
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
#include "../../visualization/line/operation/vistoolrotation.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../qmuparser/qmudef.h"
#include "../support/dialogeditwrongformula.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vmisc/compatibility.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "ui_dialogrotation.h"

//---------------------------------------------------------------------------------------------------------------------
DialogRotation::DialogRotation(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogRotation),
      timerAngle(new QTimer(this)),
      formulaAngle(),
      formulaBaseHeightAngle(0),
      objects(),
      stage1(true),
      m_suffix(),
      m_firstRelease(false),
      flagAngle(false),
      flagName(true),
      flagGroupName(true),
      flagError(false)
{
    ui->setupUi(this);

    this->formulaBaseHeightAngle = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    ui->lineEditSuffix->setText(qApp->getCurrentDocument()->GenerateSuffix());

    timerAngle->setSingleShot(true);
    connect(timerAngle, &QTimer::timeout, this, &DialogRotation::EvalAngle);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxOriginPoint);

    connect(ui->lineEditSuffix, &QLineEdit::textChanged, this, &DialogRotation::SuffixChanged);
    connect(ui->lineEditVisibilityGroup, &QLineEdit::textChanged, this, &DialogRotation::GroupNameChanged);
    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogRotation::FXAngle);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerAngle->start(formulaTimerTimeout);
    });
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogRotation::DeployAngleTextEdit);
    connect(ui->comboBoxOriginPoint, &QComboBox::currentTextChanged,
            this, &DialogRotation::PointChanged);

    vis = new VisToolRotation(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogRotation::~DialogRotation()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogRotation::GetOrigPointId() const
{
    return getCurrentObjectId(ui->comboBoxOriginPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetOrigPointId(quint32 value)
{
    ChangeCurrentData(ui->comboBoxOriginPoint, value);
    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr)
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
    formulaAngle = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaAngle);

    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr)
    operation->SetAngle(formulaAngle);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogRotation::GetSuffix() const
{
    return m_suffix;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetSuffix(const QString &value)
{
    m_suffix = value;
    ui->lineEditSuffix->setText(value);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> DialogRotation::GetObjects() const
{
    return ConvertToVector(objects);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogRotation::GetVisibilityGroupName() const
{
    return ui->lineEditVisibilityGroup->text();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetVisibilityGroupName(const QString &name)
{
    ui->lineEditVisibilityGroup->setText(name.isEmpty() ? tr("Rotation") : name);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogRotation::HasLinkedVisibilityGroup() const
{
    return ui->groupBoxVisibilityGroup->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetHasLinkedVisibilityGroup(bool linked)
{
    ui->groupBoxVisibilityGroup->setChecked(linked);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetVisibilityGroupTags(const QStringList &tags)
{
    ui->lineEditGroupTags->setText(tags.join(", "));
}

//---------------------------------------------------------------------------------------------------------------------
QStringList DialogRotation::GetVisibilityGroupTags() const
{
    return ui->lineEditGroupTags->text().split(',');
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SetGroupCategories(const QStringList &categories)
{
    m_groupTags = categories;
    ui->lineEditGroupTags->SetCompletion(m_groupTags);
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
        SCASSERT(scene != nullptr)
        scene->clearSelection();

        VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(operation != nullptr)
        operation->SetObjects(ConvertToVector(objects));
        operation->VisualMode();

        scene->ToggleArcSelection(false);
        scene->ToggleElArcSelection(false);
        scene->ToggleSplineSelection(false);
        scene->ToggleSplinePathSelection(false);

        scene->ToggleArcHover(false);
        scene->ToggleElArcHover(false);
        scene->ToggleSplineHover(false);
        scene->ToggleSplinePathHover(false);

        qApp->getSceneView()->AllowRubberBand(false);

        emit ToolTip(tr("Select origin point"));
    }
    else if (not stage1 && prepare && click)
    {
        // The check need to ignore first release of mouse button.
        // User can select point by clicking on a label.
        if (not m_firstRelease)
        {
            m_firstRelease = true;
            return;
        }

        /*We will ignore click if pointer is in point circle*/
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr)
        try
        {
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(GetOrigPointId());
            const QLineF line = QLineF(static_cast<QPointF>(*point), scene->getScenePos());

            //Radius of point circle, but little bigger. Need handle with hover sizes.
            if (line.length() <= ScaledRadius(SceneScale(qApp->getCurrentScene()))*1.5)
            {
                return;
            }
        }
        catch (const VExceptionBadId &)
        {
            return;
        }

        VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(operation != nullptr)

        SetAngle(operation->Angle());//Show in dialog angle that a user choose
        setModal(true);
        emit ToolTip(QString());
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
            VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
            SCASSERT(operation != nullptr)

            if (objects.contains(id))
            {
                if (objects.size() > 1)
                {
                    // It's not really logical for a user that a center of rotation no need to select.
                    // To fix this issue we just silently remove it from the list.
                    objects.removeOne(id);
                    operation->SetObjects(ConvertToVector(objects));
                }
                else
                {
                    emit ToolTip(tr("This point cannot be origin point. Please, select another origin point"));
                    return;
                }
            }

            if (SetObject(id, ui->comboBoxOriginPoint, QString()))
            {
                VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
                SCASSERT(window != nullptr)
                connect(operation, &Visualization::ToolTip, window, &VAbstractMainWindow::ShowToolTip);

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
void DialogRotation::DeployAngleTextEdit()
{
    DeployFormula(this, ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::FXAngle()
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
void DialogRotation::SuffixChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        const QString suffix = edit->text();
        if (suffix.isEmpty())
        {
            flagName = false;
            ChangeColor(ui->labelSuffix, errorColor);
            CheckState();
            return;
        }
        else
        {
            if (m_suffix != suffix)
            {
                QRegularExpression rx(NameRegExp());
                const QStringList uniqueNames = data->AllUniqueNames();
                for (auto &uniqueName : uniqueNames)
                {
                    const QString name = uniqueName + suffix;
                    if (not rx.match(name).hasMatch() || not data->IsUnique(name))
                    {
                        flagName = false;
                        ChangeColor(ui->labelSuffix, errorColor);
                        CheckState();
                        return;
                    }
                }
            }
        }

        flagName = true;
        ChangeColor(ui->labelSuffix, OkColor(this));
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::GroupNameChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        const QString name = edit->text();
        if (name.isEmpty())
        {
            flagGroupName = false;
            ChangeColor(ui->labelGroupName, errorColor);
            CheckState();
            return;
        }

        flagGroupName = true;
        ChangeColor(ui->labelGroupName, OkColor(this));
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::ShowVisualization()
{
    AddVisualization<VisToolRotation>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::SaveData()
{
    m_suffix = ui->lineEditSuffix->text();
    formulaAngle = ui->plainTextEditFormula->toPlainText();

    VisToolRotation *operation = qobject_cast<VisToolRotation *>(vis);
    SCASSERT(operation != nullptr)

    operation->SetObjects(ConvertToVector(objects));
    operation->SetOriginPointId(GetOrigPointId());
    operation->SetAngle(formulaAngle);
    operation->RefreshGeometry();

    QStringList groupTags = ui->lineEditGroupTags->text().split(',');
    for (auto &tag : groupTags)
    {
        tag = tag.trimmed();
        if (not m_groupTags.contains(tag))
        {
            m_groupTags.append(tag);
        }
    }

    ui->lineEditGroupTags->SetCompletion(m_groupTags);
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
    QColor color;
    if (objects.contains(getCurrentObjectId(ui->comboBoxOriginPoint)))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = OkColor(this);
    }
    ChangeColor(ui->labelOriginPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogRotation::EvalAngle()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditFormula->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditAngle;
    formulaData.labelResult = ui->labelResultAngle;
    formulaData.postfix = degreeSymbol;
    formulaData.checkZero = false;

    Eval(formulaData, flagAngle);
}
