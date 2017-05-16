/************************************************************************
 **
 **  @file   dialogsplinepath.cpp
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

#include "dialogsplinepath.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QLabel>
#include <QLatin1String>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QSharedPointer>
#include <QToolButton>
#include <QVariant>
#include <Qt>
#include <new>

#include "../../tools/vabstracttool.h"
#include "../../visualization/visualization.h"
#include "../../visualization/path/vistoolsplinepath.h"
#include "../ifc/xml/vdomdocument.h"
#include "../qmuparser/qmuparsererror.h"
#include "../support/dialogeditwrongformula.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vsplinepoint.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "ui_dialogsplinepath.h"
#include "vtranslatevars.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogSplinePath create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogSplinePath::DialogSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSplinePath),
      path(),
      newDuplicate(-1),
      formulaBaseHeightAngle1(0),
      formulaBaseHeightAngle2(0),
      formulaBaseHeightLength1(0),
      formulaBaseHeightLength2(0),
      flagAngle1(),
      flagAngle2(),
      flagLength1(),
      flagLength2()
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

    InitOkCancelApply(ui);
    bOk->setEnabled(false);

    FillComboBoxPoints(ui->comboBoxPoint);
    FillComboBoxLineColors(ui->comboBoxColor);
    FillComboBoxTypeLine(ui->comboBoxPenStyle, CurvePenStylesPics());

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogSplinePath::PointChanged);
    connect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSplinePath::currentPointChanged);

    connect(ui->toolButtonExprAngle1, &QPushButton::clicked, this, &DialogSplinePath::FXAngle1);
    connect(ui->toolButtonExprAngle2, &QPushButton::clicked, this, &DialogSplinePath::FXAngle2);
    connect(ui->toolButtonExprLength1, &QPushButton::clicked, this, &DialogSplinePath::FXLength1);
    connect(ui->toolButtonExprLength2, &QPushButton::clicked, this, &DialogSplinePath::FXLength2);

    connect(ui->plainTextEditAngle1F, &QPlainTextEdit::textChanged, this, &DialogSplinePath::Angle1Changed);
    connect(ui->plainTextEditAngle2F, &QPlainTextEdit::textChanged, this, &DialogSplinePath::Angle2Changed);
    connect(ui->plainTextEditLength1F, &QPlainTextEdit::textChanged, this, &DialogSplinePath::Length1Changed);
    connect(ui->plainTextEditLength2F, &QPlainTextEdit::textChanged, this, &DialogSplinePath::Length2Changed);

    connect(ui->pushButtonGrowAngle1, &QPushButton::clicked, this, &DialogSplinePath::DeployAngle1TextEdit);
    connect(ui->pushButtonGrowAngle2, &QPushButton::clicked, this, &DialogSplinePath::DeployAngle2TextEdit);
    connect(ui->pushButtonGrowLength1, &QPushButton::clicked, this, &DialogSplinePath::DeployLength1TextEdit);
    connect(ui->pushButtonGrowLength2, &QPushButton::clicked, this, &DialogSplinePath::DeployLength2TextEdit);

    vis = new VisToolSplinePath(data);
    auto path = qobject_cast<VisToolSplinePath *>(vis);
    SCASSERT(path != nullptr)

    auto scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr)
    connect(scene, &VMainGraphicsScene::MouseLeftPressed, path, &VisToolSplinePath::MouseLeftPressed);
    connect(scene, &VMainGraphicsScene::MouseLeftReleased, path, &VisToolSplinePath::MouseLeftReleased);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSplinePath::~DialogSplinePath()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPath set spline path
 * @param value path
 */
void DialogSplinePath::SetPath(const VSplinePath &value)
{
    flagAngle1.clear();
    flagAngle2.clear();
    flagLength1.clear();
    flagLength2.clear();

    this->path = value;
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    for (qint32 i = 0; i < path.CountPoints(); ++i)
    {
        NewItem(path.at(i));
    }
    ui->listWidget->setFocus(Qt::OtherFocusReason);
    ui->lineEditSplPathName->setText(qApp->TrVars()->VarToUser(path.name()));

    auto visPath = qobject_cast<VisToolSplinePath *>(vis);
    SCASSERT(visPath != nullptr)
    visPath->setPath(path);
    ui->listWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSplinePath::GetPenStyle() const
{
    return GetComboBoxCurrentData(ui->comboBoxPenStyle, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SetPenStyle(const QString &value)
{
    ChangeCurrentData(ui->comboBoxPenStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSplinePath::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type don't show this id in list
 */
void DialogSplinePath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        if (AllIds().contains(id))
        {
            return;
        }

        const auto point = data->GeometricObject<VPointF>(id);
        VSplinePoint p;
        p.SetP(*point);
        NewItem(p);
        emit ToolTip(tr("Select point of curve path"));

        SavePath();

        auto visPath = qobject_cast<VisToolSplinePath *>(vis);
        SCASSERT(visPath != nullptr)
        visPath->setPath(path);

        if (path.CountPoints() == 1)
        {
            visPath->VisualMode(NULL_ID);
            VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
            SCASSERT(window != nullptr)
            connect(visPath, &VisToolSplinePath::ToolTip, window, &VAbstractMainWindow::ShowToolTip);

            connect(visPath, &VisToolSplinePath::PathChanged, this, &DialogSplinePath::PathUpdated);
        }
        else
        {
            visPath->RefreshGeometry();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveData Put dialog data in local variables
 */
void DialogSplinePath::SaveData()
{
    const quint32 d = path.GetDuplicate();//Save previous value
    SavePath();
    newDuplicate <= -1 ? path.SetDuplicate(d) : path.SetDuplicate(static_cast<quint32>(newDuplicate));

    auto visPath = qobject_cast<VisToolSplinePath *>(vis);
    SCASSERT(visPath != nullptr)
    visPath->setPath(path);
    visPath->SetMode(Mode::Show);
    visPath->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::CheckState()
{
    SCASSERT(bOk != nullptr)

    bool fAngle1 = true, fAngle2 = true, fLength1 = true, fLength2 = true;

    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        fAngle1 = fAngle1 && flagAngle1.at(i);
        fAngle2 = fAngle2 && flagAngle2.at(i);
        fLength1 = fLength1 && flagLength1.at(i);
        fLength2 = fLength2 && flagLength2.at(i);
    }

    bOk->setEnabled(fAngle1 && fAngle2 && fLength1 && fLength2 && flagError);
    // In case dialog hasn't apply button
    if (bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditAngle1F->blockSignals(true);
    ui->plainTextEditAngle2F->blockSignals(true);
    ui->plainTextEditLength1F->blockSignals(true);
    ui->plainTextEditLength2F->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::DeployAngle1TextEdit()
{
    DeployFormula(ui->plainTextEditAngle1F, ui->pushButtonGrowAngle1, formulaBaseHeightAngle1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::DeployAngle2TextEdit()
{
    DeployFormula(ui->plainTextEditAngle2F, ui->pushButtonGrowAngle2, formulaBaseHeightAngle2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::DeployLength1TextEdit()
{
    DeployFormula(ui->plainTextEditLength1F, ui->pushButtonGrowLength1, formulaBaseHeightLength1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::DeployLength2TextEdit()
{
    DeployFormula(ui->plainTextEditLength2F, ui->pushButtonGrowLength2, formulaBaseHeightLength2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::Angle1Changed()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    if (row != 0)
    {
        QListWidgetItem *item = ui->listWidget->item(row);
        SCASSERT(item != nullptr)
        VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

        const QString angle1F = ui->plainTextEditAngle1F->toPlainText().replace("\n", " ");
        const qreal angle1 = Visualization::FindVal(angle1F, data->PlainVariables());

        try
        {
            p.SetAngle1(angle1, qApp->TrVars()->FormulaFromUser(angle1F, qApp->Settings()->GetOsSeparator()));
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            p.SetAngle1(angle1, angle1F);
        }

        item->setData(Qt::UserRole, QVariant::fromValue(p));

        EvalAngle1();

        if (row != ui->listWidget->count()-1)
        {
            ui->plainTextEditAngle2F->blockSignals(true);
            ui->plainTextEditAngle2F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle2Formula(),
                                                                                 qApp->Settings()->GetOsSeparator()));
            EvalAngle2();
            ui->plainTextEditAngle2F->blockSignals(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::Angle2Changed()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    if (row != ui->listWidget->count()-1)
    {
        QListWidgetItem *item = ui->listWidget->item(row);
        SCASSERT(item != nullptr)
        VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

        const QString angle2F = ui->plainTextEditAngle2F->toPlainText().replace("\n", " ");
        const qreal angle2 = Visualization::FindVal(angle2F, data->PlainVariables());

        try
        {
            p.SetAngle2(angle2, qApp->TrVars()->FormulaFromUser(angle2F, qApp->Settings()->GetOsSeparator()));
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            p.SetAngle2(angle2, angle2F);
        }

        item->setData(Qt::UserRole, QVariant::fromValue(p));

        EvalAngle2();

        if (row != 0)
        {
            ui->plainTextEditAngle1F->blockSignals(true);
            ui->plainTextEditAngle1F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle1Formula(),
                                                                                 qApp->Settings()->GetOsSeparator()));
            EvalAngle1();
            ui->plainTextEditAngle1F->blockSignals(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::Length1Changed()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    if (row != 0)
    {
        QListWidgetItem *item = ui->listWidget->item(row);
        SCASSERT(item != nullptr)
        VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

        const QString length1F = ui->plainTextEditLength1F->toPlainText().replace("\n", " ");
        const qreal length1 = Visualization::FindLength(length1F, data->PlainVariables());

        try
        {
            p.SetLength1(length1, qApp->TrVars()->FormulaFromUser(length1F, qApp->Settings()->GetOsSeparator()));
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            p.SetLength1(length1, length1F);
        }

        item->setData(Qt::UserRole, QVariant::fromValue(p));

        EvalLength1();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::Length2Changed()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    if (row != ui->listWidget->count()-1)
    {
        QListWidgetItem *item = ui->listWidget->item(row);
        SCASSERT(item != nullptr)
        VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

        const QString length2F = ui->plainTextEditLength2F->toPlainText().replace("\n", " ");
        const qreal length2 = Visualization::FindLength(length2F, data->PlainVariables());

        try
        {
            p.SetLength2(length2, qApp->TrVars()->FormulaFromUser(length2F, qApp->Settings()->GetOsSeparator()));
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            p.SetLength2(length2, length2F);
        }

        item->setData(Qt::UserRole, QVariant::fromValue(p));

        EvalLength2();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::FXAngle1()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first control point angle"));

    QString angle1F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditAngle1F->toPlainText().replace("\n", " "),
                                                         qApp->Settings()->GetOsSeparator());

    dialog->SetFormula(angle1F);
    dialog->setPostfix(degreeSymbol);
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
void DialogSplinePath::FXAngle2()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second control point angle"));

    QString angle2F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditAngle2F->toPlainText().replace("\n", " "),
                                                         qApp->Settings()->GetOsSeparator());

    dialog->SetFormula(angle2F);
    dialog->setPostfix(degreeSymbol);
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
void DialogSplinePath::FXLength1()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first control point length"));

    QString length1F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditLength1F->toPlainText().replace("\n", " "),
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
void DialogSplinePath::FXLength2()
{
    auto dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second control point length"));

    QString length2F = qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditLength2F->toPlainText().replace("\n", " "),
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
void DialogSplinePath::EvalAngle1()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    labelEditFormula = ui->labelEditAngle1;
    Eval(ui->plainTextEditAngle1F->toPlainText(), flagAngle1[row], ui->labelResultAngle1, degreeSymbol, false);

    QListWidgetItem *item = ui->listWidget->item(row);
    SCASSERT(item != nullptr)
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

    ShowPointIssue(p.P().name());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::EvalAngle2()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    labelEditFormula = ui->labelEditAngle2;
    Eval(ui->plainTextEditAngle2F->toPlainText(), flagAngle2[row], ui->labelResultAngle2, degreeSymbol, false);

    QListWidgetItem *item = ui->listWidget->item(row);
    SCASSERT(item != nullptr)
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

    ShowPointIssue(p.P().name());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::EvalLength1()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    labelEditFormula = ui->labelEditLength1;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal length1 = Eval(ui->plainTextEditLength1F->toPlainText(), flagLength1[row], ui->labelResultLength1,
                               postfix, false);

    if (length1 < 0)
    {
        flagLength1[row] = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultLength1->setText(tr("Error") + QLatin1String(" (") + postfix + QLatin1String(")"));
        ui->labelResultLength1->setToolTip(tr("Length can't be negative"));

        CheckState();
    }

    QListWidgetItem *item = ui->listWidget->item(row);
    SCASSERT(item != nullptr)
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

    ShowPointIssue(p.P().name());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::EvalLength2()
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    labelEditFormula = ui->labelEditLength2;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal length2 = Eval(ui->plainTextEditLength2F->toPlainText(), flagLength2[row], ui->labelResultLength2,
                               postfix, false);

    if (length2 < 0)
    {
        flagLength2[row] = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultLength2->setText(tr("Error") + QLatin1String(" (") + postfix + QLatin1String(")"));
        ui->labelResultLength2->setToolTip(tr("Length can't be negative"));

        CheckState();
    }

    QListWidgetItem *item = ui->listWidget->item(row);
    SCASSERT(item != nullptr)
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));

    ShowPointIssue(p.P().name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PointChanged selected another point in list
 * @param row number of row
 */
void DialogSplinePath::PointChanged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }

    const auto p = qvariant_cast<VSplinePoint>(ui->listWidget->item(row)->data(Qt::UserRole));
    DataPoint(p);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief currentPointChanged changed point in combo box
 * @param index index in list
 */
void DialogSplinePath::currentPointChanged(int index)
{
    const quint32 id = qvariant_cast<quint32>(ui->comboBoxPoint->itemData(index));
    QListWidgetItem *item = ui->listWidget->item( ui->listWidget->currentRow() );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    const auto point = data->GeometricObject<VPointF>(id);
    p.SetP(*point);
    DataPoint(p);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
    ShowPointIssue(p.P().name());

    QColor color = okColor;
    if (not IsPathValid())
    {
        flagError = false;
        color = errorColor;

        ui->lineEditSplPathName->setText(tr("Invalid spline path"));
    }
    else
    {
        flagError = true;
        color = okColor;

        auto first = qvariant_cast<VSplinePoint>(ui->listWidget->item(0)->data(Qt::UserRole));
        auto last = qvariant_cast<VSplinePoint>(ui->listWidget->item(ui->listWidget->count()-1)->data(Qt::UserRole));

        if (first.P().id() == path.at(0).P().id() && last.P().id() == path.at(path.CountPoints()-1).P().id())
        {
            newDuplicate = -1;
            ui->lineEditSplPathName->setText(qApp->TrVars()->VarToUser(path.name()));
        }
        else
        {
            VSplinePath newPath = ExtractPath();

            if (not data->IsUnique(newPath.name()))
            {
                newDuplicate = static_cast<qint32>(DNumber(newPath.name()));
                newPath.SetDuplicate(static_cast<quint32>(newDuplicate));
            }

            ui->lineEditSplPathName->setText(qApp->TrVars()->VarToUser(newPath.name()));
        }
    }
    ChangeColor(ui->labelName, color);
    ChangeColor(ui->labelPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::ShowDialog(bool click)
{
    if (click == false)
    {
        if (path.CountPoints() >= 3)
        {
            emit ToolTip("");

            if (not data->IsUnique(path.name()))
            {
                path.SetDuplicate(DNumber(path.name()));
            }

            DialogAccepted();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::PathUpdated(const VSplinePath &path)
{
    SetPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::ShowVisualization()
{
    AddVisualization<VisToolSplinePath>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewItem add point to list
 * @param point spline path point
 */
void DialogSplinePath::NewItem(const VSplinePoint &point)
{
    flagAngle1.append(true);
    flagLength1.append(true);
    flagAngle2.append(true);
    flagLength2.append(true);

    auto item = new QListWidgetItem(point.P().name());
    item->setFont(QFont("Times", 12, QFont::Bold));
    item->setData(Qt::UserRole, QVariant::fromValue(point));

    ui->listWidget->addItem(item);
    ui->listWidget->setCurrentItem(item);
    if (ui->listWidget->count() >= 2)
    {
        bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        bOk->setEnabled(true);
    }

    DataPoint(point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief dataPoint show data of point in fields
 * @param p spline path point
 */
void DialogSplinePath::DataPoint(const VSplinePoint &p)
{
    ui->comboBoxPoint->blockSignals(true);
    ChangeCurrentData(ui->comboBoxPoint, p.P().id());
    ui->comboBoxPoint->blockSignals(false);

    int row = ui->listWidget->currentRow();
    const QString field = tr("Not used");
    const QString emptyRes = QString("_");

    if (row == 0)
    {
        ui->toolButtonExprAngle1->setEnabled(false);
        ui->labelResultAngle1->setText(emptyRes);
        ui->labelResultAngle1->setToolTip(tr("Value"));
        ChangeColor(ui->labelEditAngle1, okColor);
        ui->plainTextEditAngle1F->blockSignals(true);
        ui->plainTextEditAngle1F->setPlainText(field);
        ui->plainTextEditAngle1F->setEnabled(false);
        ui->plainTextEditAngle1F->blockSignals(false);

        ui->toolButtonExprLength1->setEnabled(false);
        ui->labelResultLength1->setText(emptyRes);
        ui->labelResultLength1->setToolTip(tr("Value"));
        ChangeColor(ui->labelEditLength1, okColor);
        ui->plainTextEditLength1F->blockSignals(true);
        ui->plainTextEditLength1F->setPlainText(field);
        ui->plainTextEditLength1F->setEnabled(false);
        ui->plainTextEditLength1F->blockSignals(false);

        ui->plainTextEditAngle2F->setEnabled(true);
        ui->plainTextEditLength2F->setEnabled(true);

        ui->toolButtonExprAngle2->setEnabled(true);
        ui->toolButtonExprLength2->setEnabled(true);

        ui->plainTextEditAngle2F->blockSignals(true);
        ui->plainTextEditLength2F->blockSignals(true);
        ui->plainTextEditAngle2F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle2Formula(),
                                                                             qApp->Settings()->GetOsSeparator()));
        EvalAngle2();
        ui->plainTextEditLength2F->setPlainText(qApp->TrVars()->FormulaToUser(p.Length2Formula(),
                                                                              qApp->Settings()->GetOsSeparator()));
        EvalLength2();
        ui->plainTextEditAngle2F->blockSignals(false);
        ui->plainTextEditLength2F->blockSignals(false);
    }
    else if (row == ui->listWidget->count()-1)
    {
        ui->toolButtonExprAngle2->setEnabled(false);
        ui->labelResultAngle2->setText(emptyRes);
        ui->labelResultAngle2->setToolTip(tr("Value"));
        ChangeColor(ui->labelEditAngle2, okColor);
        ui->plainTextEditAngle2F->blockSignals(true);
        ui->plainTextEditAngle2F->setPlainText(field);
        ui->plainTextEditAngle2F->setEnabled(false);
        ui->plainTextEditAngle2F->blockSignals(false);

        ui->toolButtonExprLength2->setEnabled(false);
        ui->labelResultLength2->setText(emptyRes);
        ui->labelResultLength2->setToolTip(tr("Value"));
        ChangeColor(ui->labelEditLength2, okColor);
        ui->plainTextEditLength2F->blockSignals(true);
        ui->plainTextEditLength2F->setPlainText(field);
        ui->plainTextEditLength2F->setEnabled(false);
        ui->plainTextEditLength2F->blockSignals(false);

        ui->plainTextEditAngle1F->setEnabled(true);
        ui->plainTextEditLength1F->setEnabled(true);

        ui->toolButtonExprAngle1->setEnabled(true);
        ui->toolButtonExprLength1->setEnabled(true);

        ui->plainTextEditAngle1F->blockSignals(true);
        ui->plainTextEditLength1F->blockSignals(true);
        ui->plainTextEditAngle1F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle1Formula(),
                                                                             qApp->Settings()->GetOsSeparator()));
        EvalAngle1();
        ui->plainTextEditLength1F->setPlainText(qApp->TrVars()->FormulaToUser(p.Length1Formula(),
                                                                              qApp->Settings()->GetOsSeparator()));
        EvalLength1();
        ui->plainTextEditAngle1F->blockSignals(false);
        ui->plainTextEditLength1F->blockSignals(false);
    }
    else
    {
        ui->toolButtonExprAngle1->setEnabled(true);
        ui->toolButtonExprLength1->setEnabled(true);
        ui->toolButtonExprAngle2->setEnabled(true);
        ui->toolButtonExprLength2->setEnabled(true);

        ui->plainTextEditAngle1F->setEnabled(true);
        ui->plainTextEditLength1F->setEnabled(true);
        ui->plainTextEditAngle2F->setEnabled(true);
        ui->plainTextEditLength2F->setEnabled(true);

        ui->plainTextEditAngle1F->blockSignals(true);
        ui->plainTextEditLength1F->blockSignals(true);
        ui->plainTextEditAngle2F->blockSignals(true);
        ui->plainTextEditLength2F->blockSignals(true);

        ui->plainTextEditAngle1F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle1Formula(),
                                                                             qApp->Settings()->GetOsSeparator()));
        ui->plainTextEditAngle2F->setPlainText(qApp->TrVars()->FormulaToUser(p.Angle2Formula(),
                                                                             qApp->Settings()->GetOsSeparator()));
        ui->plainTextEditLength1F->setPlainText(qApp->TrVars()->FormulaToUser(p.Length1Formula(),
                                                                              qApp->Settings()->GetOsSeparator()));
        ui->plainTextEditLength2F->setPlainText(qApp->TrVars()->FormulaToUser(p.Length2Formula(),
                                                                              qApp->Settings()->GetOsSeparator()));

        EvalAngle1();
        EvalLength1();
        EvalAngle2();
        EvalLength2();

        ui->plainTextEditAngle1F->blockSignals(false);
        ui->plainTextEditLength1F->blockSignals(false);
        ui->plainTextEditAngle2F->blockSignals(false);
        ui->plainTextEditLength2F->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SavePath()
{
    path.Clear();
    path = ExtractPath();
}

//---------------------------------------------------------------------------------------------------------------------
QSet<quint32> DialogSplinePath::AllIds() const
{
    QSet<quint32> ids;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        ids.insert(qvariant_cast<VSplinePoint>(ui->listWidget->item(i)->data(Qt::UserRole)).P().id());
    }

    return ids;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSplinePath::IsPathValid() const
{
    if (path.CountPoints() < 3)
    {
        return false;
    }

    return (AllIds().size() == path.CountPoints());
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath DialogSplinePath::ExtractPath() const
{
    QVector<VSplinePoint> points;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        points.append(qvariant_cast<VSplinePoint>(ui->listWidget->item(i)->data(Qt::UserRole)));
    }
    return VSplinePath(points);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::ShowPointIssue(const QString &pName)
{
    const int row = ui->listWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    QListWidgetItem *item = ui->listWidget->item(row);
    SCASSERT(item != nullptr)

    if (flagAngle1.at(row) && flagAngle2.at(row) && flagLength1.at(row) && flagLength2.at(row))
    {
       item->setText(pName);
    }
    else
    {
       item->setText(pName + QLatin1String("(!)"));
    }
}
