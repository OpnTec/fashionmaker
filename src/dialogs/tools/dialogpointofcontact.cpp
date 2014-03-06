/************************************************************************
 **
 **  @file   dialogpointofcontact.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "dialogpointofcontact.h"

#include <QPushButton>

DialogPointOfContact::DialogPointOfContact(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(), number(0), pointName(QString()), radius(QString()), center(0),
    firstPoint(0), secondPoint(0)
{
    ui.setupUi(this);
    listWidget = ui.listWidget;
    labelResultCalculation = ui.labelResultCalculation;
    labelDescription = ui.labelDescription;
    radioButtonSizeGrowth = ui.radioButtonSizeGrowth;
    radioButtonStandardTable = ui.radioButtonStandardTable;
    radioButtonIncrements = ui.radioButtonIncrements;
    radioButtonLengthLine = ui.radioButtonLengthLine;
    radioButtonLengthArc = ui.radioButtonLengthArc;
    radioButtonLengthCurve = ui.radioButtonLengthSpline;
    lineEditFormula = ui.lineEditFormula;
    labelEditFormula = ui.labelEditFormula;
    labelEditNamePoint = ui.labelEditNamePoint;

    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    Q_CHECK_PTR(bOk);
    connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);
    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    Q_CHECK_PTR(bCansel);
    connect(bCansel, &QPushButton::clicked, this, &DialogTool::DialogRejected);

    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxPoints(ui.comboBoxCenter);
    FillComboBoxPoints(ui.comboBoxFirstPoint);
    FillComboBoxPoints(ui.comboBoxSecondPoint);

    connect(ui.toolButtonPutHere, &QPushButton::clicked, this, &DialogPointOfContact::PutHere);
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &DialogPointOfContact::PutVal);
    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogPointOfContact::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui.radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogPointOfContact::SizeGrowth);
    connect(ui.radioButtonStandardTable, &QRadioButton::clicked, this, &DialogPointOfContact::StandardTable);
    connect(ui.radioButtonIncrements, &QRadioButton::clicked, this, &DialogPointOfContact::Increments);
    connect(ui.radioButtonLengthLine, &QRadioButton::clicked, this, &DialogPointOfContact::LengthLines);
    connect(ui.radioButtonLengthArc, &QRadioButton::clicked, this, &DialogPointOfContact::LengthArcs);
    connect(ui.radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogPointOfContact::LengthCurves);
    connect(ui.toolButtonEqual, &QPushButton::clicked, this, &DialogPointOfContact::EvalFormula);
    connect(ui.lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfContact::NamePointChanged);
    connect(ui.lineEditFormula, &QLineEdit::textChanged, this, &DialogPointOfContact::FormulaChanged);
}

void DialogPointOfContact::ChoosedObject(quint32 id, const Scene::Scenes &type)
{
    if (type == Scene::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui.comboBoxFirstPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui.comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select second point of line"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui.comboBoxSecondPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui.comboBoxSecondPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select point of center of arc"));
                return;
            }
        }
        if (number == 2)
        {
            qint32 index = ui.comboBoxCenter->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui.comboBoxCenter->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
            }
            if (isInitialized == false)
            {
                this->show();
            }
        }
    }
}

void DialogPointOfContact::DialogAccepted()
{
    pointName = ui.lineEditNamePoint->text();
    radius = ui.lineEditFormula->text();
    center = getCurrentObjectId(ui.comboBoxCenter);
    firstPoint = getCurrentObjectId(ui.comboBoxFirstPoint);
    secondPoint = getCurrentObjectId(ui.comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

void DialogPointOfContact::setSecondPoint(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui.comboBoxSecondPoint, secondPoint, value, id);
}

void DialogPointOfContact::setFirstPoint(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui.comboBoxFirstPoint, firstPoint, value, id);
}

void DialogPointOfContact::setCenter(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui.comboBoxCenter, center, value, id);
    center = value;
}

void DialogPointOfContact::setRadius(const QString &value)
{
    radius = value;
    ui.lineEditFormula->setText(radius);
}

void DialogPointOfContact::setPointName(const QString &value)
{
    pointName = value;
    ui.lineEditNamePoint->setText(pointName);
}
