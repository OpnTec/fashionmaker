/************************************************************************
 **
 **  @file   dialogarc.cpp
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

#include "dialogarc.h"
#include "ui_dialogarc.h"

#include <QPushButton>
#include <QtWidgets>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogArc create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogArc::DialogArc(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogArc), flagRadius(false), flagF1(false), flagF2(false),
    timerRadius(nullptr), timerF1(nullptr), timerF2(nullptr), center(0), radius(QString()), f1(QString()), f2(QString())
{
    ui->setupUi(this);

    timerRadius = new QTimer(this);
    connect(timerRadius, &QTimer::timeout, this, &DialogArc::EvalRadius);

    timerF1 = new QTimer(this);
    connect(timerF1, &QTimer::timeout, this, &DialogArc::EvalF1);

    timerF2 = new QTimer(this);
    connect(timerF2, &QTimer::timeout, this, &DialogArc::EvalF2);

    InitOkCancel(ui);

    FillComboBoxPoints(ui->comboBoxBasePoint);

    CheckState();

    InitVariables(ui);

    connect(ui->toolButtonPutHereRadius, &QPushButton::clicked, this, &DialogArc::PutRadius);
    connect(ui->toolButtonPutHereF1, &QPushButton::clicked, this, &DialogArc::PutF1);
    connect(ui->toolButtonPutHereF2, &QPushButton::clicked, this, &DialogArc::PutF2);

    connect(ui->toolButtonEqualRadius, &QPushButton::clicked, this, &DialogArc::EvalRadius);
    connect(ui->toolButtonEqualF1, &QPushButton::clicked, this, &DialogArc::EvalF1);
    connect(ui->toolButtonEqualF2, &QPushButton::clicked, this, &DialogArc::EvalF2);

    connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &DialogArc::RadiusChanged);
    connect(ui->lineEditF1, &QLineEdit::textChanged, this, &DialogArc::F1Changed);
    connect(ui->lineEditF2, &QLineEdit::textChanged, this, &DialogArc::F2Changed);
}

//---------------------------------------------------------------------------------------------------------------------
DialogArc::~DialogArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogArc::SetCenter(const quint32 &value)
{
    center = value;
    ChangeCurrentData(ui->comboBoxBasePoint, center);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF2 set formula second angle of arc
 * @param value formula
 */
void DialogArc::SetF2(const QString &value)
{
    f2 = value;
    ui->lineEditF2->setText(f2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF1 set formula first angle of arc
 * @param value formula
 */
void DialogArc::SetF1(const QString &value)
{
    f1 = value;
    ui->lineEditF1->setText(f1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius set formula of radius
 * @param value formula
 */
void DialogArc::SetRadius(const QString &value)
{
    radius = value;
    ui->lineEditRadius->setText(radius);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogArc::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);

        ChangeCurrentText(ui->comboBoxBasePoint, point->name());
        emit ToolTip("");
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogArc::DialogAccepted()
{
    radius = ui->lineEditRadius->text();
    f1 = ui->lineEditF1->text();
    f2 = ui->lineEditF2->text();
    center = getCurrentObjectId(ui->comboBoxBasePoint);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValChenged show description angles of lines
 * @param row number of row
 */
void DialogArc::ValChenged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if (ui->radioButtonLineAngles->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLineAngle(item->text()))
                .arg(tr("Value of angle of line."));
        ui->labelDescription->setText(desc);
        return;
    }
    DialogTool::ValChenged(row);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutRadius put variable into formula of radius
 */
void DialogArc::PutRadius()
{
    PutValHere(ui->lineEditRadius, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutF1 put variable into formula of first angle
 */
void DialogArc::PutF1()
{
    PutValHere(ui->lineEditF1, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutF2 put variable into formula of second angle
 */
void DialogArc::PutF2()
{
    PutValHere(ui->lineEditF2, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineAngles show variable angles of lines
 */
// cppcheck-suppress unusedFunction
void DialogArc::LineAngles()
{
    ShowLineAngles();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RadiusChanged after change formula of radius calculate value and show result
 */
void DialogArc::RadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    ValFormulaChanged(flagRadius, ui->lineEditRadius, timerRadius);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F1Changed after change formula of first angle calculate value and show result
 */
void DialogArc::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    ValFormulaChanged(flagF1, ui->lineEditF1, timerF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F2Changed after change formula of second angle calculate value and show result
 */
void DialogArc::F2Changed()
{
    labelEditFormula = ui->labelEditF2;
    ValFormulaChanged(flagF2, ui->lineEditF2, timerF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckState if all is right enable button ok
 */
void DialogArc::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagRadius && flagF1 && flagF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalRadius calculate value of radius
 */
void DialogArc::EvalRadius()
{
    labelEditFormula = ui->labelEditRadius;
    Eval(ui->lineEditRadius, flagRadius, timerRadius, ui->labelResultRadius);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalF1 calculate value of first angle
 */
void DialogArc::EvalF1()
{
    labelEditFormula = ui->labelEditF1;
    Eval(ui->lineEditF1, flagF1, timerF1, ui->labelResultF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalF2 calculate value of second angle
 */
void DialogArc::EvalF2()
{
    labelEditFormula = ui->labelEditF2;
    Eval(ui->lineEditF2, flagF2, timerF2, ui->labelResultF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowLineAngles show varibles angles of lines
 */
void DialogArc::ShowLineAngles()
{
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    const QHash<QString, qreal> *lineAnglesTable = data->DataLineAngles();
    SCASSERT(lineAnglesTable != nullptr);
    QHashIterator<QString, qreal> i(*lineAnglesTable);
    while (i.hasNext())
    {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());

        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}
