/************************************************************************
 **
 **  @file   dialogpatterntype.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 2, 2014
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

#include "dialogmeasurements.h"
#include "ui_dialogmeasurements.h"
#include "../../core/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
DialogMeasurements::DialogMeasurements(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogMeasurements), result(MeasurementsType::Individual)
{
    ui->setupUi(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    connect(ui->toolButtonStandard, &QToolButton::clicked, this, &DialogMeasurements::StandardMeasurements);
    connect(ui->toolButtonIndividual, &QToolButton::clicked, this, &DialogMeasurements::IndividualMeasurements);
}

//---------------------------------------------------------------------------------------------------------------------
DialogMeasurements::~DialogMeasurements()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType DialogMeasurements::type() const
{
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMeasurements::StandardMeasurements()
{
    result = MeasurementsType::Standard;
    accept();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMeasurements::IndividualMeasurements()
{
    result = MeasurementsType::Individual;
    accept();
}
