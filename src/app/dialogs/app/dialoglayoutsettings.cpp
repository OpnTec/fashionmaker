/************************************************************************
 **
 **  @file   dialoglayoutsettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 1, 2015
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

#include "dialoglayoutsettings.h"
#include "ui_dialoglayoutsettings.h"
#include "../../core/vapplication.h"
#include "../../libs/ifc/xml/vdomdocument.h"
#include "../../core/vsettings.h"
#include "../../xml/vabstractmeasurements.h"
#include "../../libs/vlayout/vlayoutgenerator.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../../../utils/vmath.h"
#else
#   include <QtMath>
#endif


enum class PaperSizeTemplate : char { A0, A1, A2, A3, A4, Letter, Legal, Roll24in, Roll30in, Roll36in, Roll42in,
                                      Roll44in};

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutSettings::DialogLayoutSettings(VLayoutGenerator *generator, QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogLayoutSettings), oldPaperUnit(Unit::Mm), oldLayoutUnit(Unit::Mm),
      generator(generator)
{
    ui->setupUi(this);

    qApp->getSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    InitPaperUnits();
    InitLayoutUnits();
    InitTemplates();
    MinimumPaperSize();
    MinimumLayoutSize();

    connect(ui->comboBoxTemplates,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::TemplateSelected);
    connect(ui->comboBoxPaperSizeUnit,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::ConvertPaperSize);
    connect(ui->doubleSpinBoxPaperWidth,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);
    connect(ui->doubleSpinBoxPaperHeight,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);
    connect(ui->toolButtonPortrate, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->toolButtonLandscape, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->comboBoxLayoutUnit,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::ConvertLayoutSize);

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogLayoutSettings::DialogAccepted);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutSettings::~DialogLayoutSettings()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
int DialogLayoutSettings::GetPaperHeight() const
{
    return qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxPaperHeight->value(), oldPaperUnit, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetPaperHeight(int value)
{
    ui->doubleSpinBoxPaperHeight->setValue(VAbstractMeasurements::UnitConvertor(value, Unit::Px, PaperUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
int DialogLayoutSettings::GetPaperWidth() const
{
    return qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxPaperWidth->value(), oldPaperUnit, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetPaperWidth(int value)
{
    ui->doubleSpinBoxPaperWidth->setValue(VAbstractMeasurements::UnitConvertor(value, Unit::Px, PaperUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int DialogLayoutSettings::GetShift() const
{
    return static_cast<quint32>(qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxShift->value(),
                                                                            oldLayoutUnit, Unit::Px)));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetShift(unsigned int value)
{
    ui->doubleSpinBoxShift->setValue(VAbstractMeasurements::UnitConvertor(value, Unit::Px, LayoutUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int DialogLayoutSettings::GetLayoutWidth() const
{
    return static_cast<quint32>(qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxLayoutWidth->value(),
                                                                            oldLayoutUnit, Unit::Px)));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetLayoutWidth(unsigned int value)
{
    ui->doubleSpinBoxLayoutWidth->setValue(VAbstractMeasurements::UnitConvertor(value, Unit::Px, LayoutUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
Cases DialogLayoutSettings::GetGroup() const
{
    if (ui->radioButtonThreeGroups->isChecked())
    {
        return Cases::CaseThreeGroup;
    }
    else if (ui->radioButtonTwoGroups->isChecked())
    {
        return Cases::CaseTwoGroup;
    }
    else
    {
        return Cases::CaseDesc;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void DialogLayoutSettings::SetGroup(const Cases &value)
{
    switch (value)
    {
        case Cases::CaseThreeGroup:
            ui->radioButtonThreeGroups->setChecked(true);
            break;
        case Cases::CaseTwoGroup:
            ui->radioButtonTwoGroups->setChecked(true);
            break;
        case Cases::CaseDesc:
            ui->radioButtonDescendingArea->setChecked(true);
            break;
        default:
            ui->radioButtonDescendingArea->setChecked(true);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetRotate() const
{
    return ui->groupBoxRotate->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetRotate(bool state)
{
    ui->groupBoxRotate->setChecked(state);
}

//---------------------------------------------------------------------------------------------------------------------
int DialogLayoutSettings::GetIncrease() const
{
    return ui->comboBoxIncrease->currentText().toInt();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void DialogLayoutSettings::SetIncrease(int increase)
{
    int index = ui->comboBoxIncrease->findText(QString::number(increase));

    if (index == -1)
    {
        index = 21;
    }

    ui->comboBoxIncrease->setCurrentIndex(index);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetAutoCrop() const
{
    return ui->checkBoxAutoCrop->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetAutoCrop(bool autoCrop)
{
    ui->checkBoxAutoCrop->setChecked(autoCrop);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::IsSaveLength() const
{
    return ui->checkBoxSaveLength->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetSaveLength(bool save)
{
    ui->checkBoxSaveLength->setChecked(save);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::TemplateSelected()
{
    const QSizeF size = Template();

    oldPaperUnit = PaperUnit();
    ui->doubleSpinBoxPaperWidth->setMaximum(qApp->fromPixel(QIMAGE_MAX, oldPaperUnit));
    ui->doubleSpinBoxPaperHeight->setMaximum(qApp->fromPixel(QIMAGE_MAX, oldPaperUnit));

    ui->doubleSpinBoxPaperWidth->setValue(size.width());
    ui->doubleSpinBoxPaperHeight->setValue(size.height());

    CorrectPaperDecimals();
    PaperSizeChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::ConvertPaperSize()
{
    const Unit paperUnit = PaperUnit();
    const qreal width = ui->doubleSpinBoxPaperWidth->value();
    const qreal height = ui->doubleSpinBoxPaperHeight->value();

    ui->doubleSpinBoxPaperWidth->setMaximum(qApp->fromPixel(QIMAGE_MAX, paperUnit));
    ui->doubleSpinBoxPaperHeight->setMaximum(qApp->fromPixel(QIMAGE_MAX, paperUnit));

    ui->doubleSpinBoxPaperWidth->setValue(VAbstractMeasurements::UnitConvertor(width, oldPaperUnit, paperUnit));
    ui->doubleSpinBoxPaperHeight->setValue(VAbstractMeasurements::UnitConvertor(height, oldPaperUnit, paperUnit));
    oldPaperUnit = paperUnit;
    CorrectPaperDecimals();
    MinimumPaperSize();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::ConvertLayoutSize()
{
    const Unit unit = LayoutUnit();
    const qreal layoutWidth = ui->doubleSpinBoxLayoutWidth->value();
    const qreal shift = ui->doubleSpinBoxShift->value();

    ui->doubleSpinBoxLayoutWidth->setMaximum(qApp->fromPixel(QIMAGE_MAX, unit));
    ui->doubleSpinBoxShift->setMaximum(qApp->fromPixel(QIMAGE_MAX, unit));

    ui->doubleSpinBoxLayoutWidth->setValue(VAbstractMeasurements::UnitConvertor(layoutWidth, oldLayoutUnit, unit));
    ui->doubleSpinBoxShift->setValue(VAbstractMeasurements::UnitConvertor(shift, oldLayoutUnit, unit));
    oldLayoutUnit = unit;
    CorrectLayoutDecimals();
    MinimumLayoutSize();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::PaperSizeChanged()
{
    if (ui->doubleSpinBoxPaperHeight->value() > ui->doubleSpinBoxPaperWidth->value())
    {
        ui->toolButtonPortrate->blockSignals(true);
        ui->toolButtonPortrate->setChecked(true);
        ui->toolButtonPortrate->blockSignals(false);
    }
    else
    {
        ui->toolButtonLandscape->blockSignals(true);
        ui->toolButtonLandscape->setChecked(true);
        ui->toolButtonLandscape->blockSignals(false);
    }

    Label();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::Swap(bool checked)
{
    if (checked)
    {
        const qreal width = ui->doubleSpinBoxPaperWidth->value();
        const qreal height = ui->doubleSpinBoxPaperHeight->value();

        ui->doubleSpinBoxPaperWidth->blockSignals(true);
        ui->doubleSpinBoxPaperWidth->setValue(height);
        ui->doubleSpinBoxPaperWidth->blockSignals(false);

        ui->doubleSpinBoxPaperHeight->blockSignals(true);
        ui->doubleSpinBoxPaperHeight->setValue(width);
        ui->doubleSpinBoxPaperHeight->blockSignals(false);

        Label();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::DialogAccepted()
{
    SCASSERT(generator != nullptr)
    generator->SetLayoutWidth(GetLayoutWidth());
    generator->SetCaseType(GetGroup());
    generator->SetPaperHeight(GetPaperHeight());
    generator->SetPaperWidth(GetPaperWidth());
    generator->SetShift(GetShift());
    generator->SetRotate(GetRotate());
    generator->SetRotationIncrease(GetIncrease());
    generator->SetAutoCrop(GetAutoCrop());
    generator->SetSaveLength(IsSaveLength());

    accepted();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitPaperUnits()
{
    ui->comboBoxPaperSizeUnit->addItem(tr("Millimiters"), QVariant(VDomDocument::UnitsToStr(Unit::Mm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Centimeters"), QVariant(VDomDocument::UnitsToStr(Unit::Cm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Inches"), QVariant(VDomDocument::UnitsToStr(Unit::Inch)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Pixels"), QVariant(VDomDocument::UnitsToStr(Unit::Px)));

    // set default unit
    oldPaperUnit = VDomDocument::StrToUnits(qApp->getSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxPaperSizeUnit->findData(qApp->getSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxPaperSizeUnit->setCurrentIndex(indexUnit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitLayoutUnits()
{
    ui->comboBoxLayoutUnit->addItem(tr("Centimeters"), QVariant(VDomDocument::UnitsToStr(Unit::Cm)));
    ui->comboBoxLayoutUnit->addItem(tr("Millimiters"), QVariant(VDomDocument::UnitsToStr(Unit::Mm)));
    ui->comboBoxLayoutUnit->addItem(tr("Inches"), QVariant(VDomDocument::UnitsToStr(Unit::Inch)));

    // set default unit
    oldLayoutUnit = VDomDocument::StrToUnits(qApp->getSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxLayoutUnit->findData(qApp->getSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxLayoutUnit->setCurrentIndex(indexUnit);
    }

    ui->doubleSpinBoxLayoutWidth->setValue(VAbstractMeasurements::UnitConvertor(2.5, Unit::Mm, oldLayoutUnit));
    ui->doubleSpinBoxShift->setValue(VAbstractMeasurements::UnitConvertor(50, Unit::Mm, oldLayoutUnit));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitTemplates()
{
    const QIcon icoPaper("://icon/16x16/template.png");
    const QIcon icoRoll("://icon/16x16/roll.png");
    const QString pdi = QString("(%1ppi)").arg(VApplication::PrintDPI);

    ui->comboBoxTemplates->addItem(icoPaper, "A0 "+pdi, QVariant(static_cast<char>(PaperSizeTemplate::A0)));
    ui->comboBoxTemplates->addItem(icoPaper, "A1 "+pdi, QVariant(static_cast<char>(PaperSizeTemplate::A1)));
    ui->comboBoxTemplates->addItem(icoPaper, "A2 "+pdi, QVariant(static_cast<char>(PaperSizeTemplate::A2)));
    ui->comboBoxTemplates->addItem(icoPaper, "A3 "+pdi, QVariant(static_cast<char>(PaperSizeTemplate::A3)));
    ui->comboBoxTemplates->addItem(icoPaper, "A4 "+pdi, QVariant(static_cast<char>(PaperSizeTemplate::A4)));
    ui->comboBoxTemplates->addItem(icoPaper, tr("Letter ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Letter)));
    ui->comboBoxTemplates->addItem(icoPaper, tr("Legal ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Legal)));
    ui->comboBoxTemplates->addItem(icoRoll,
                                   tr("Roll 24in ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Roll24in)));
    ui->comboBoxTemplates->addItem(icoRoll,
                                   tr("Roll 30in ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Roll30in)));
    ui->comboBoxTemplates->addItem(icoRoll,
                                   tr("Roll 36in ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Roll36in)));
    ui->comboBoxTemplates->addItem(icoRoll,
                                   tr("Roll 42in ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Roll42in)));
    ui->comboBoxTemplates->addItem(icoRoll,
                                   tr("Roll 44in ")+pdi, QVariant(static_cast<char>(PaperSizeTemplate::Roll44in)));

    TemplateSelected();
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF DialogLayoutSettings::Template()
{
    PaperSizeTemplate temp;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    temp = static_cast<PaperSizeTemplate>(ui->comboBoxTemplates->itemData(ui->comboBoxTemplates->currentIndex())
                                          .toInt());
#else
    temp = static_cast<PaperSizeTemplate>(ui->comboBoxTemplates->currentData().toInt());
#endif
    const Unit paperUnit = PaperUnit();

    qreal width = 0;
    qreal height = 0;

    switch (temp)
    {
        case PaperSizeTemplate::A0:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(841, Unit::Mm, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(1189, Unit::Mm, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::A1:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(594, Unit::Mm, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(841, Unit::Mm, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::A2:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(420, Unit::Mm, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(594, Unit::Mm, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::A3:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(297, Unit::Mm, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(420, Unit::Mm, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::A4:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(210, Unit::Mm, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(297, Unit::Mm, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Letter:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(8.5, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(11, Unit::Inch, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Legal:
            SetAutoCrop(false);
            SetSaveLength(false);

            width = VAbstractMeasurements::UnitConvertor(11, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(17, Unit::Inch, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Roll24in:
            SetAutoCrop(true);
            SetSaveLength(true);

            width = VAbstractMeasurements::UnitConvertor(24, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Roll30in:
            SetAutoCrop(true);
            SetSaveLength(true);

            width = VAbstractMeasurements::UnitConvertor(30, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Roll36in:
            SetAutoCrop(true);
            SetSaveLength(true);

            width = VAbstractMeasurements::UnitConvertor(36, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Roll42in:
            SetAutoCrop(true);
            SetSaveLength(true);

            width = VAbstractMeasurements::UnitConvertor(42, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return QSizeF(width, height);
        case PaperSizeTemplate::Roll44in:
            SetAutoCrop(true);
            SetSaveLength(true);

            width = VAbstractMeasurements::UnitConvertor(44, Unit::Inch, paperUnit);
            height = VAbstractMeasurements::UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return QSizeF(width, height);
        default:
            break;
    }
    return QSizeF();
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogLayoutSettings::PaperUnit() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    return VDomDocument::StrToUnits(ui->comboBoxPaperSizeUnit->itemData(ui->comboBoxPaperSizeUnit->currentIndex())
                                    .toString());
#else
    return VDomDocument::StrToUnits(ui->comboBoxPaperSizeUnit->currentData().toString());
#endif
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogLayoutSettings::LayoutUnit() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    return VDomDocument::StrToUnits(ui->comboBoxLayoutUnit->itemData(ui->comboBoxLayoutUnit->currentIndex())
                                    .toString());
#else
    return VDomDocument::StrToUnits(ui->comboBoxLayoutUnit->currentData().toString());
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::CorrectPaperDecimals()
{
    switch (oldPaperUnit)
    {
        case Unit::Cm:
        case Unit::Mm:
            ui->doubleSpinBoxPaperWidth->setDecimals(2);
            ui->doubleSpinBoxPaperHeight->setDecimals(2);
            break;
        case Unit::Inch:
            ui->doubleSpinBoxPaperWidth->setDecimals(5);
            ui->doubleSpinBoxPaperHeight->setDecimals(5);
            break;
        case Unit::Px:
            ui->doubleSpinBoxPaperWidth->setDecimals(0);
            ui->doubleSpinBoxPaperHeight->setDecimals(0);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::CorrectLayoutDecimals()
{
    switch (oldLayoutUnit)
    {
        case Unit::Cm:
        case Unit::Mm:
            ui->doubleSpinBoxLayoutWidth->setDecimals(2);
            ui->doubleSpinBoxShift->setDecimals(2);
            break;
        case Unit::Inch:
            ui->doubleSpinBoxLayoutWidth->setDecimals(5);
            ui->doubleSpinBoxShift->setDecimals(5);
            break;
        case Unit::Px:
            ui->doubleSpinBoxLayoutWidth->setDecimals(0);
            ui->doubleSpinBoxShift->setDecimals(0);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::Label()
{
    const int width = qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxPaperWidth->value(), PaperUnit(),
                                                                  Unit::Px));
    const int height = qFloor(VAbstractMeasurements::UnitConvertor(ui->doubleSpinBoxPaperHeight->value(),
                                                                   PaperUnit(), Unit::Px));
    QString text = QString("%1 x %2 px, \n%3 ppi").arg(width).arg(height).arg(VApplication::PrintDPI);
    ui->labelSizeDescription->setText(text);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::MinimumPaperSize()
{
    const qreal value = VAbstractMeasurements::UnitConvertor(1, Unit::Px, oldPaperUnit);
    ui->doubleSpinBoxPaperWidth->setMinimum(value);
    ui->doubleSpinBoxPaperHeight->setMinimum(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::MinimumLayoutSize()
{
    const qreal value = VAbstractMeasurements::UnitConvertor(1, Unit::Px, oldLayoutUnit);
    ui->doubleSpinBoxLayoutWidth->setMinimum(value);
}
