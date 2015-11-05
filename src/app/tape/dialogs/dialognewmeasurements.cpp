/************************************************************************
 **
 **  @file   dialognewmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "dialognewmeasurements.h"
#include "ui_dialognewmeasurements.h"

#include "../vpatterndb/variables/vmeasurement.h"

#include <QShowEvent>

//---------------------------------------------------------------------------------------------------------------------
DialogNewMeasurements::DialogNewMeasurements(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::DialogNewMeasurements),
      isInitialized(false)
{
    ui->setupUi(this);

    InitMTypes();
    InitUnits(MeasurementsType::Individual);
    InitHeightsList();
    InitSizesList();

    connect(ui->comboBoxMType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogNewMeasurements::CurrentTypeChanged);

    connect(ui->comboBoxUnit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogNewMeasurements::CurrentUnitChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogNewMeasurements::~DialogNewMeasurements()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType DialogNewMeasurements::Type() const
{
    return static_cast<MeasurementsType>(ui->comboBoxMType->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogNewMeasurements::MUnit() const
{
    return static_cast<Unit>(ui->comboBoxUnit->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
int DialogNewMeasurements::BaseSize() const
{
    return ui->comboBoxBaseSize->currentText().toInt();
}

//---------------------------------------------------------------------------------------------------------------------
int DialogNewMeasurements::BaseHeight() const
{
    return ui->comboBoxBaseHeight->currentText().toInt();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        InitMTypes();
        const MeasurementsType type = static_cast<MeasurementsType>(ui->comboBoxMType->currentData().toInt());
        InitUnits(type);
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::showEvent(QShowEvent *event)
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
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::CurrentTypeChanged(int index)
{
    const MeasurementsType type = static_cast<MeasurementsType>(ui->comboBoxMType->itemData(index).toInt());
    if (type == MeasurementsType::Standard)
    {
        ui->comboBoxBaseSize->setEnabled(true);
        ui->comboBoxBaseHeight->setEnabled(true);
    }
    else
    {
        ui->comboBoxBaseSize->setEnabled(false);
        ui->comboBoxBaseHeight->setEnabled(false);
    }
    InitUnits(type);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::CurrentUnitChanged(int index)
{
    Q_UNUSED(index);
    InitHeightsList();
    InitSizesList();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::InitMTypes()
{
    int val = static_cast<int>(MeasurementsType::Unknown);
    if (ui->comboBoxMType->currentIndex() != -1)
    {
        val = ui->comboBoxMType->currentData().toInt();
    }

    ui->comboBoxMType->blockSignals(true);
    ui->comboBoxMType->clear();
    ui->comboBoxMType->addItem(tr("Individual"), static_cast<int>(MeasurementsType::Individual));
    ui->comboBoxMType->addItem(tr("Standard"), static_cast<int>(MeasurementsType::Standard));
    ui->comboBoxMType->blockSignals(false);

    int index = ui->comboBoxMType->findData(val);
    if (index != -1)
    {
        ui->comboBoxMType->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::InitHeightsList()
{
    const QStringList list = VMeasurement::WholeListHeights(MUnit());
    ui->comboBoxBaseHeight->clear();
    ui->comboBoxBaseHeight->addItems(list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::InitSizesList()
{
    const QStringList list = VMeasurement::WholeListSizes(MUnit());
    ui->comboBoxBaseSize->clear();
    ui->comboBoxBaseSize->addItems(list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewMeasurements::InitUnits(const MeasurementsType &type)
{
    int val = static_cast<int>(Unit::Cm);
    if (ui->comboBoxUnit->currentIndex() != -1)
    {
        val = ui->comboBoxUnit->currentData().toInt();
    }

    ui->comboBoxUnit->blockSignals(true);
    ui->comboBoxUnit->clear();
    ui->comboBoxUnit->addItem(tr("Centimeters"), static_cast<int>(Unit::Cm));
    ui->comboBoxUnit->addItem(tr("Millimiters"), static_cast<int>(Unit::Mm));
    if (type == MeasurementsType::Individual)
    {
        ui->comboBoxUnit->addItem(tr("Inches"), static_cast<int>(Unit::Inch));
    }
    ui->comboBoxUnit->blockSignals(false);

    int index = ui->comboBoxUnit->findData(val);
    if (index != -1)
    {
        ui->comboBoxUnit->setCurrentIndex(index);
    }
}
