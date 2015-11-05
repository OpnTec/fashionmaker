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
#include "../core/vapplication.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vmisc/vsettings.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

#include <QPushButton>

//must be the same order as PaperSizeTemplate constants
const DialogLayoutSettings::FormatsVector DialogLayoutSettings::pageFormatNames =
        DialogLayoutSettings::FormatsVector () << QLatin1Literal("A0")
                                               << QLatin1Literal("A1")
                                               << QLatin1Literal("A2")
                                               << QLatin1Literal("A3")
                                               << QLatin1Literal("A4")
                                               << QApplication::translate("DialogLayoutSettings", "Letter")
                                               << QApplication::translate("DialogLayoutSettings", "Legal")
                                               << QApplication::translate("DialogLayoutSettings", "Roll 24in")
                                               << QApplication::translate("DialogLayoutSettings", "Roll 30in")
                                               << QApplication::translate("DialogLayoutSettings", "Roll 36in")
                                               << QApplication::translate("DialogLayoutSettings", "Roll 42in")
                                               << QApplication::translate("DialogLayoutSettings", "Roll 44in")
                                               << QApplication::translate("DialogLayoutSettings", "Custom");

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutSettings::DialogLayoutSettings(VLayoutGenerator *generator, QWidget *parent, bool disableSettings)
    : QDialog(parent), disableSettings(disableSettings), ui(new Ui::DialogLayoutSettings), oldPaperUnit(Unit::Mm),
      oldLayoutUnit(Unit::Mm), generator(generator), isInitialized(false)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    //moved from ReadSettings - well...it seems it can be done once only (i.e. constructor) because Init funcs dont
    //even cleanse lists before adding
    InitPaperUnits();
    InitLayoutUnits();
    InitTemplates();
    MinimumPaperSize();
    MinimumLayoutSize();

    //in export console mode going to use defaults
    if (disableSettings == false)
    {
        ReadSettings();
    }
    else
    {
        RestoreDefaults();
    }

    connect(ui->comboBoxTemplates,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::TemplateSelected);
    connect(ui->comboBoxPaperSizeUnit,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::ConvertPaperSize);

    connect(ui->doubleSpinBoxPaperWidth,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);
    connect(ui->doubleSpinBoxPaperHeight,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);

    connect(ui->doubleSpinBoxPaperWidth,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::FindTemplate);
    connect(ui->doubleSpinBoxPaperHeight,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::FindTemplate);

    connect(ui->toolButtonPortrate, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->toolButtonLandscape, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->comboBoxLayoutUnit,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::ConvertLayoutSize);

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogLayoutSettings::DialogAccepted);

    QPushButton *bRestoreDefaults = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(bRestoreDefaults, &QPushButton::clicked, this, &DialogLayoutSettings::RestoreDefaults);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutSettings::~DialogLayoutSettings()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::GetPaperHeight() const
{
    return UnitConvertor(ui->doubleSpinBoxPaperHeight->value(), oldPaperUnit, Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetPaperHeight(qreal value)
{
    ui->doubleSpinBoxPaperHeight->setValue(UnitConvertor(value, Unit::Px, PaperUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::GetPaperWidth() const
{
    return UnitConvertor(ui->doubleSpinBoxPaperWidth->value(), oldPaperUnit, Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetPaperWidth(qreal value)
{
    ui->doubleSpinBoxPaperWidth->setValue(UnitConvertor(value, Unit::Px, PaperUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::GetShift() const
{
    return UnitConvertor(ui->doubleSpinBoxShift->value(), oldLayoutUnit, Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetShift(qreal value)
{
    ui->doubleSpinBoxShift->setValue(UnitConvertor(value, Unit::Px, LayoutUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::GetLayoutWidth() const
{
    return UnitConvertor(ui->doubleSpinBoxLayoutWidth->value(), oldLayoutUnit, Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetLayoutWidth(qreal value)
{
    ui->doubleSpinBoxLayoutWidth->setValue(UnitConvertor(value, Unit::Px, LayoutUnit()));
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
bool DialogLayoutSettings::SetIncrease(int increase)
{
    int index = ui->comboBoxIncrease->findText(QString::number(increase));
    bool failed = (index == -1);
    if (failed)
    {
        index = 21;//180 degree
    }

    ui->comboBoxIncrease->setCurrentIndex(index);
    return failed;
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
bool DialogLayoutSettings::IsUnitePages() const
{
    return ui->checkBoxUnitePages->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetUnitePages(bool save)
{
    ui->checkBoxUnitePages->setChecked(save);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::TemplateSelected()
{
    SheetSize(Template());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::FindTemplate()
{
    const qreal width = ui->doubleSpinBoxPaperWidth->value();
    const qreal height = ui->doubleSpinBoxPaperHeight->value();
    QSizeF size(width, height);

    const int max = static_cast<int>(PaperSizeTemplate::Custom);
    for (int i=0; i < max; ++i)
    {
        const QSizeF tmplSize = TemplateSize(static_cast<PaperSizeTemplate>(i));
        if (size == tmplSize)
        {
            ui->comboBoxTemplates->blockSignals(true);
            const int index = ui->comboBoxTemplates->findData(i);
            if (index != -1)
            {
                ui->comboBoxTemplates->setCurrentIndex(index);
            }
            ui->comboBoxTemplates->blockSignals(false);
            return;
        }
    }

    ui->comboBoxTemplates->blockSignals(true);
    const int index = ui->comboBoxTemplates->findData(max);
    if (index != -1)
    {
        ui->comboBoxTemplates->setCurrentIndex(index);
    }
    ui->comboBoxTemplates->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::ConvertPaperSize()
{
    const Unit paperUnit = PaperUnit();
    const qreal width = ui->doubleSpinBoxPaperWidth->value();
    const qreal height = ui->doubleSpinBoxPaperHeight->value();

    ui->doubleSpinBoxPaperWidth->blockSignals(true);
    ui->doubleSpinBoxPaperHeight->blockSignals(true);
    ui->doubleSpinBoxPaperWidth->setMaximum(FromPixel(QIMAGE_MAX, paperUnit));
    ui->doubleSpinBoxPaperHeight->setMaximum(FromPixel(QIMAGE_MAX, paperUnit));
    ui->doubleSpinBoxPaperWidth->blockSignals(false);
    ui->doubleSpinBoxPaperHeight->blockSignals(false);

    const qreal newWidth = UnitConvertor(width, oldPaperUnit, paperUnit);
    const qreal newHeight = UnitConvertor(height, oldPaperUnit, paperUnit);

    oldPaperUnit = paperUnit;
    CorrectPaperDecimals();
    MinimumPaperSize();

    ui->doubleSpinBoxPaperWidth->setValue(newWidth);
    ui->doubleSpinBoxPaperHeight->setValue(newHeight);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::SelectPaperUnit(const QString& units)
{
    qint32 indexUnit = ui->comboBoxPaperSizeUnit->findData(units);
    if (indexUnit != -1)
    {
        ui->comboBoxPaperSizeUnit->setCurrentIndex(indexUnit);
    }
    return indexUnit != -1;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::SelectLayoutUnit(const QString &units)
{
    qint32 indexUnit = ui->comboBoxLayoutUnit->findData(units);
    if (indexUnit != -1)
    {
        ui->comboBoxLayoutUnit->setCurrentIndex(indexUnit);
    }
    return indexUnit != -1;
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::LayoutToPixels(qreal value) const
{
    return UnitConvertor(value, LayoutUnit(), Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::PageToPixels(qreal value) const
{
    return UnitConvertor(value, PaperUnit(), Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLayoutSettings::MakeGroupsHelp()
{
    //that is REALLY dummy ... can't figure fast how to automate generation... :/
    return tr("\n\tThree groups: big, middle, small = 0\n\tTwo groups: big, small = 1\n\tDescending area = 2\n");
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::showEvent(QShowEvent *event)
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
void DialogLayoutSettings::ConvertLayoutSize()
{
    const Unit unit = LayoutUnit();
    const qreal layoutWidth = ui->doubleSpinBoxLayoutWidth->value();
    const qreal shift = ui->doubleSpinBoxShift->value();

    ui->doubleSpinBoxLayoutWidth->setMaximum(FromPixel(QIMAGE_MAX, unit));
    ui->doubleSpinBoxShift->setMaximum(FromPixel(QIMAGE_MAX, unit));

    const qreal newLayoutWidth = UnitConvertor(layoutWidth, oldLayoutUnit, unit);
    const qreal newShift = UnitConvertor(shift, oldLayoutUnit, unit);

    oldLayoutUnit = unit;
    CorrectLayoutDecimals();
    MinimumLayoutSize();

    ui->doubleSpinBoxLayoutWidth->setValue(newLayoutWidth);
    ui->doubleSpinBoxShift->setValue(newShift);
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
bool DialogLayoutSettings::SelectTemplate(const PaperSizeTemplate& id)
{
    int index = ui->comboBoxTemplates->findData(static_cast<VIndexType>(id));
    if (index > -1)
    {
        ui->comboBoxTemplates->setCurrentIndex(index);
    }

    return (index > -1);
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
    generator->SetPaperHeight(qFloor(GetPaperHeight()));
    generator->SetPaperWidth(qFloor(GetPaperWidth()));
    generator->SetShift(qFloor(GetShift()));
    generator->SetRotate(GetRotate());
    generator->SetRotationIncrease(GetIncrease());
    generator->SetAutoCrop(GetAutoCrop());
    generator->SetSaveLength(IsSaveLength());
    generator->SetUnitePages(IsUnitePages());

    //don't want to break visual settings when cmd used
    if (disableSettings == false)
    {
        WriteSettings();
    }
    accepted();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::RestoreDefaults()
{
    ui->comboBoxTemplates->blockSignals(true);
    ui->comboBoxTemplates->setCurrentIndex(0);//A0
    TemplateSelected();
    ui->comboBoxTemplates->blockSignals(false);

    SetLayoutWidth(VSettings::GetDefLayoutWidth());
    SetShift(VSettings::GetDefLayoutShift());
    SetGroup(VSettings::GetDefLayoutGroup());
    SetRotate(VSettings::GetDefLayoutRotate());
    SetIncrease(VSettings::GetDefLayoutRotationIncrease());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitPaperUnits()
{
    ui->comboBoxPaperSizeUnit->addItem(tr("Millimiters"), QVariant(VDomDocument::UnitsToStr(Unit::Mm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Centimeters"), QVariant(VDomDocument::UnitsToStr(Unit::Cm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Inches"), QVariant(VDomDocument::UnitsToStr(Unit::Inch)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Pixels"), QVariant(VDomDocument::UnitsToStr(Unit::Px)));

    // set default unit
    oldPaperUnit = VDomDocument::StrToUnits(qApp->ValentinaSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxPaperSizeUnit->findData(qApp->ValentinaSettings()->GetUnit());
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
    oldLayoutUnit = VDomDocument::StrToUnits(qApp->ValentinaSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxLayoutUnit->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxLayoutUnit->setCurrentIndex(indexUnit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitTemplates()
{
    const QIcon icoPaper("://icon/16x16/template.png");
    const QIcon icoRoll("://icon/16x16/roll.png");
    const QString pdi = QString("(%1ppi)").arg(PrintDPI);

    auto cntr = static_cast<VIndexType>(PaperSizeTemplate::A0);
    foreach(const auto& v, pageFormatNames)
    {
        if (cntr <= static_cast<int>(PaperSizeTemplate::Legal))
        {
            ui->comboBoxTemplates->addItem(icoPaper, v+" "+pdi, QVariant(cntr++));
        }
        else if (cntr <= static_cast<int>(PaperSizeTemplate::Roll44in))
        {
            ui->comboBoxTemplates->addItem(icoRoll, v+" "+pdi, QVariant(cntr++));
        }
        else
        {
            ui->comboBoxTemplates->addItem(v+" "+pdi, QVariant(cntr++));
        }
    }
    ui->comboBoxTemplates->setCurrentIndex(-1);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLayoutSettings::MakeHelpTemplateList()
{
    QString out = "\n";

    auto cntr = static_cast<VIndexType>(PaperSizeTemplate::A0);
    foreach(const auto& v,  pageFormatNames)
    {
        if (cntr <= static_cast<int>(PaperSizeTemplate::Roll44in))// Don't include custom template
        {
            out += "\t"+v+" = "+ QString::number(cntr++)+"\n";
        }
    }
    return out;
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

    switch (temp)
    {
        case PaperSizeTemplate::A0:
        case PaperSizeTemplate::A1:
        case PaperSizeTemplate::A2:
        case PaperSizeTemplate::A3:
        case PaperSizeTemplate::A4:
        case PaperSizeTemplate::Letter:
            SetAdditionalOptions(false);
            return TemplateSize(temp);
        case PaperSizeTemplate::Legal:
        case PaperSizeTemplate::Roll24in:
        case PaperSizeTemplate::Roll30in:
        case PaperSizeTemplate::Roll36in:
        case PaperSizeTemplate::Roll42in:
        case PaperSizeTemplate::Roll44in:
            SetAdditionalOptions(true);
            return TemplateSize(temp);
        case PaperSizeTemplate::Custom:
            return TemplateSize(temp);
        default:
            break;
    }
    return QSizeF();
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF DialogLayoutSettings::TemplateSize(const PaperSizeTemplate &tmpl) const
{
    const Unit paperUnit = PaperUnit();
    qreal width = 0;
    qreal height = 0;

    switch (tmpl)
    {
        case PaperSizeTemplate::A0:
            width = UnitConvertor(841, Unit::Mm, paperUnit);
            height = UnitConvertor(1189, Unit::Mm, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::A1:
            width = UnitConvertor(594, Unit::Mm, paperUnit);
            height = UnitConvertor(841, Unit::Mm, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::A2:
            width = UnitConvertor(420, Unit::Mm, paperUnit);
            height = UnitConvertor(594, Unit::Mm, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::A3:
            width = UnitConvertor(297, Unit::Mm, paperUnit);
            height = UnitConvertor(420, Unit::Mm, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::A4:
            width = UnitConvertor(210, Unit::Mm, paperUnit);
            height = UnitConvertor(297, Unit::Mm, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Letter:
            width = UnitConvertor(8.5, Unit::Inch, paperUnit);
            height = UnitConvertor(11, Unit::Inch, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Legal:
            width = UnitConvertor(11, Unit::Inch, paperUnit);
            height = UnitConvertor(17, Unit::Inch, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Roll24in:
            width = UnitConvertor(24, Unit::Inch, paperUnit);
            height = UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Roll30in:
            width = UnitConvertor(30, Unit::Inch, paperUnit);
            height = UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Roll36in:
            width = UnitConvertor(36, Unit::Inch, paperUnit);
            height = UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Roll42in:
            width = UnitConvertor(42, Unit::Inch, paperUnit);
            height = UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Roll44in:
            width = UnitConvertor(44, Unit::Inch, paperUnit);
            height = UnitConvertor(QIMAGE_MAX, Unit::Px, paperUnit);
            return RoundTemplateSize(width, height);
        case PaperSizeTemplate::Custom:
            width = ui->doubleSpinBoxPaperWidth->value();
            height = ui->doubleSpinBoxPaperHeight->value();
            return RoundTemplateSize(width, height);
        default:
            break;
    }
    return QSizeF();
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF DialogLayoutSettings::RoundTemplateSize(qreal width, qreal height) const
{
    qreal w = 0;
    qreal h = 0;

    switch (PaperUnit())
    {
        case Unit::Cm:
        case Unit::Mm:
        case Unit::Px:
            w = qRound(width * 100.0) / 100.0;
            h = qRound(height * 100.0) / 100.0;
            return QSizeF(w, h);
        case Unit::Inch:
            w = qRound(width * 100000.0) / 100000.0;
            h = qRound(height * 100000.0) / 100000.0;
            return QSizeF(w, h);
        default:
            break;
    }

    return QSizeF(width, height);
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
            ui->doubleSpinBoxPaperWidth->setDecimals(2);
            ui->doubleSpinBoxPaperHeight->setDecimals(2);
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
            ui->doubleSpinBoxLayoutWidth->setDecimals(2);
            ui->doubleSpinBoxShift->setDecimals(2);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::Label()
{
    const int width = qFloor(UnitConvertor(ui->doubleSpinBoxPaperWidth->value(), PaperUnit(), Unit::Px));
    const int height = qFloor(UnitConvertor(ui->doubleSpinBoxPaperHeight->value(), PaperUnit(), Unit::Px));
    QString text = QString("%1 x %2 px, \n%3 ppi").arg(width).arg(height).arg(PrintDPI);
    ui->labelSizeDescription->setText(text);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::MinimumPaperSize()
{
    const qreal value = UnitConvertor(1, Unit::Px, oldPaperUnit);
    ui->doubleSpinBoxPaperWidth->setMinimum(value);
    ui->doubleSpinBoxPaperHeight->setMinimum(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::MinimumLayoutSize()
{
    const qreal value = UnitConvertor(1, Unit::Px, oldLayoutUnit);
    ui->doubleSpinBoxLayoutWidth->setMinimum(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::ReadSettings()
{
    const VSettings *settings = qApp->ValentinaSettings();
    SetLayoutWidth(settings->GetLayoutWidth());
    SetShift(settings->GetLayoutShift());

    const qreal width = UnitConvertor(settings->GetLayoutPaperWidth(), Unit::Px, LayoutUnit());
    const qreal height = UnitConvertor(settings->GetLayoutPaperHeight(), Unit::Px, LayoutUnit());
    SheetSize(QSizeF(width, height));
    SetGroup(settings->GetLayoutGroup());
    SetRotate(settings->GetLayoutRotate());
    SetIncrease(settings->GetLayoutRotationIncrease());
    SetAutoCrop(settings->GetLayoutAutoCrop());
    SetSaveLength(settings->GetLayoutSaveLength());
    SetUnitePages(settings->GetLayoutUnitePages());

    FindTemplate();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::WriteSettings() const
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetLayoutWidth(GetLayoutWidth());
    settings->SetLayoutGroup(GetGroup());
    settings->SetLayoutPaperHeight(GetPaperHeight());
    settings->SetLayoutPaperWidth(GetPaperWidth());
    settings->SetLayoutShift(GetShift());
    settings->SetLayoutRotate(GetRotate());
    settings->SetLayoutRotationIncrease(GetIncrease());
    settings->SetLayoutAutoCrop(GetAutoCrop());
    settings->SetLayoutSaveLength(IsSaveLength());
    settings->SetLayoutUnitePages(IsUnitePages());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SheetSize(const QSizeF &size)
{
    oldPaperUnit = PaperUnit();
    ui->doubleSpinBoxPaperWidth->setMaximum(FromPixel(QIMAGE_MAX, oldPaperUnit));
    ui->doubleSpinBoxPaperHeight->setMaximum(FromPixel(QIMAGE_MAX, oldPaperUnit));

    ui->doubleSpinBoxPaperWidth->setValue(size.width());
    ui->doubleSpinBoxPaperHeight->setValue(size.height());

    CorrectPaperDecimals();
    PaperSizeChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetAdditionalOptions(bool value)
{
    SetAutoCrop(value);
    SetSaveLength(value);
    SetUnitePages(value);
}
