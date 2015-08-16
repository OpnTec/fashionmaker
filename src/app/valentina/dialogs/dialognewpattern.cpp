/************************************************************************
 **
 **  @file   dialognewpattern.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 2, 2014
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

#include "dialognewpattern.h"
#include "ui_dialognewpattern.h"
#include "../xml/vindividualmeasurements.h"
#include "../core/vapplication.h"
#include "../../libs/vmisc/vsettings.h"
#include "../../libs/vpatterndb/vcontainer.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QDesktopWidget>

//---------------------------------------------------------------------------------------------------------------------
DialogNewPattern::DialogNewPattern(VContainer *data, const QString &patternPieceName, QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogNewPattern), data(data)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    ui->lineEditName->setText(patternPieceName);

    InitUnits();
    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogNewPattern::CheckState);

    adjustSize();
    setMaximumSize(size());
    setMinimumSize(size());
}

//---------------------------------------------------------------------------------------------------------------------
DialogNewPattern::~DialogNewPattern()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogNewPattern::PatternUnit() const
{
    const qint32 index = ui->comboBoxUnits->currentIndex();
    return VDomDocument::StrToUnits(ui->comboBoxUnits->itemData(index).toString());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewPattern::CheckState()
{
    bool flagName = false;
    if (ui->lineEditName->text().isEmpty() == false)
    {
        flagName = true;
    }

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagName);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNewPattern::InitUnits()
{
    ui->comboBoxUnits->addItem(tr("Centimeters"), QVariant(VDomDocument::UnitsToStr(Unit::Cm)));
    ui->comboBoxUnits->addItem(tr("Millimiters"), QVariant(VDomDocument::UnitsToStr(Unit::Mm)));
    ui->comboBoxUnits->addItem(tr("Inches"), QVariant(VDomDocument::UnitsToStr(Unit::Inch)));

    // set default unit
    const qint32 indexUnit = ui->comboBoxUnits->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxUnits->setCurrentIndex(indexUnit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogNewPattern::name() const
{
    return ui->lineEditName->text();
}
