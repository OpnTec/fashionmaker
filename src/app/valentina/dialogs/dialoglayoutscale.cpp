/************************************************************************
 **
 **  @file   dialoglayoutscale.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 3, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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

#include "dialoglayoutscale.h"
#include "ui_dialoglayoutscale.h"
#include "../core/vapplication.h"
#include "../vmisc/vsettings.h"

#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutScale::DialogLayoutScale(bool printTiled, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::DialogLayoutScale)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr)
    connect(bOk, &QPushButton::clicked, this, &DialogLayoutScale::Save);

    if (not printTiled)
    {
        ui->groupBoxMargins->setVisible(false);
    }

    connect(ui->toolButtonScaleConnected, &QToolButton::clicked, this, &DialogLayoutScale::ToggleScaleConnection);

    connect(ui->doubleSpinBoxHorizontalScale, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutScale::HorizontalScaleChanged);
    connect(ui->doubleSpinBoxVerticalScale, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutScale::VerticalScaleChanged);

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutScale::~DialogLayoutScale()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::SetTiledMargins(QMarginsF margins)
{
    // read Margins top, right, bottom, left
    margins = UnitConvertor(margins, Unit::Mm, qApp->patternUnit());

    ui->doubleSpinBoxLeftField->setValue(margins.left());
    ui->doubleSpinBoxTopField->setValue(margins.top());
    ui->doubleSpinBoxRightField->setValue(margins.right());
    ui->doubleSpinBoxBottomField->setValue(margins.bottom());
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF DialogLayoutScale::GetTiledMargins() const
{
    QMarginsF margins = QMarginsF(
        ui->doubleSpinBoxLeftField->value(),
        ui->doubleSpinBoxTopField->value(),
        ui->doubleSpinBoxRightField->value(),
        ui->doubleSpinBoxBottomField->value()
        );

    return UnitConvertor(margins, qApp->patternUnit(), Unit::Mm);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::SetXScale(qreal scale)
{
    ui->doubleSpinBoxHorizontalScale->setValue(scale * 100.);
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutScale::GetXScale() const
{
    return ui->doubleSpinBoxHorizontalScale->value() / 100.;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::SetYScale(qreal scale)
{
    ui->doubleSpinBoxVerticalScale->setValue(scale * 100.);
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutScale::GetYScale() const
{
    return ui->doubleSpinBoxVerticalScale->value() / 100.;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::showEvent(QShowEvent *event)
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

    setFixedSize(size());

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::Save()
{
    WriteSettings();

    accept();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::ToggleScaleConnection()
{
    m_scaleConnected = not m_scaleConnected;

    QIcon icon;
    icon.addFile(m_scaleConnected ? QStringLiteral(":/icon/32x32/link.png")
                                  : QStringLiteral(":/icon/32x32/broken_link.png"));
    ui->toolButtonScaleConnected->setIcon(icon);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::HorizontalScaleChanged(double d)
{
    if (m_scaleConnected)
    {
        ui->doubleSpinBoxVerticalScale->blockSignals(true);
        ui->doubleSpinBoxVerticalScale->setValue(d);
        ui->doubleSpinBoxVerticalScale->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::VerticalScaleChanged(double d)
{
    if (m_scaleConnected)
    {
        ui->doubleSpinBoxHorizontalScale->blockSignals(true);
        ui->doubleSpinBoxHorizontalScale->setValue(d);
        ui->doubleSpinBoxHorizontalScale->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::ReadSettings()
{
    VSettings *settings = qApp->ValentinaSettings();
    const Unit unit = qApp->patternUnit();

    // read Margins top, right, bottom, left
    const QMarginsF margins = settings->GetTiledPDFMargins(unit);

    ui->doubleSpinBoxLeftField->setValue(margins.left());
    ui->doubleSpinBoxTopField->setValue(margins.top());
    ui->doubleSpinBoxRightField->setValue(margins.right());
    ui->doubleSpinBoxBottomField->setValue(margins.bottom());

    ui->doubleSpinBoxLeftField->setSuffix(UnitsToStr(unit, true));
    ui->doubleSpinBoxTopField->setSuffix(UnitsToStr(unit, true));
    ui->doubleSpinBoxRightField->setSuffix(UnitsToStr(unit, true));
    ui->doubleSpinBoxBottomField->setSuffix(UnitsToStr(unit, true));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutScale::WriteSettings() const
{
    VSettings *settings = qApp->ValentinaSettings();
    const Unit unit = qApp->patternUnit();

    // write Margins top, right, bottom, left
    QMarginsF margins = QMarginsF(
        ui->doubleSpinBoxLeftField->value(),
        ui->doubleSpinBoxTopField->value(),
        ui->doubleSpinBoxRightField->value(),
        ui->doubleSpinBoxBottomField->value()
        );
    settings->SetTiledPDFMargins(margins,unit);
}
