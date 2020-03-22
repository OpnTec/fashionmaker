/************************************************************************
 **
 **  @file   dialoglayoutsettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "dialoglayoutsettings.h"
#include "ui_dialoglayoutsettings.h"
#include "../core/vapplication.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/vmath.h"
#include "../vlayout/vlayoutgenerator.h"

#include <QMessageBox>
#include <QPushButton>
#include <QPrinterInfo>

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutSettings::DialogLayoutSettings(VLayoutGenerator *generator, QWidget *parent, bool disableSettings)
    : VAbstractLayoutDialog(parent), 
      disableSettings(disableSettings), 
      ui(new Ui::DialogLayoutSettings), 
      oldPaperUnit(Unit::Mm),
      oldLayoutUnit(Unit::Mm),
      generator(generator),
      isInitialized(false)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    //moved from ReadSettings - well...it seems it can be done once only (i.e. constructor) because Init funcs dont
    //even cleanse lists before adding
    InitPaperUnits();
    InitLayoutUnits();
    InitTemplates(ui->comboBoxTemplates);
    MinimumPaperSize();
    MinimumLayoutSize();
    InitPrinter();

    //in export console mode going to use defaults
    if (disableSettings == false)
    {
        ReadSettings();
    }
    else
    {
        RestoreDefaults();
    }

    connect(ui->comboBoxPrinter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::PrinterMargins);

    connect(ui->comboBoxTemplates, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::TemplateSelected);
    connect(ui->comboBoxPaperSizeUnit, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogLayoutSettings::ConvertPaperSize);

    connect(ui->doubleSpinBoxPaperWidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);
    connect(ui->doubleSpinBoxPaperHeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::PaperSizeChanged);

    connect(ui->doubleSpinBoxPaperWidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::FindTemplate);
    connect(ui->doubleSpinBoxPaperHeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::FindTemplate);

    connect(ui->doubleSpinBoxPaperWidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::CorrectMaxFileds);
    connect(ui->doubleSpinBoxPaperHeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DialogLayoutSettings::CorrectMaxFileds);

    connect(ui->checkBoxIgnoreFileds, &QCheckBox::stateChanged, this, &DialogLayoutSettings::IgnoreAllFields);

    connect(ui->toolButtonPortrait, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->toolButtonLandscape, &QToolButton::toggled, this, &DialogLayoutSettings::Swap);
    connect(ui->comboBoxLayoutUnit,  QOverload<int>::of(&QComboBox::currentIndexChanged),
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
    ui->doubleSpinBoxPaperHeight->setMaximum(FromPixel(QIMAGE_MAX, PaperUnit()));
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
    ui->doubleSpinBoxPaperWidth->setMaximum(FromPixel(QIMAGE_MAX, PaperUnit()));
    ui->doubleSpinBoxPaperWidth->setValue(UnitConvertor(value, Unit::Px, PaperUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
int DialogLayoutSettings::GetNestingTime() const
{
    return ui->spinBoxNestingTime->value();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetNestingTime(int value)
{
    ui->spinBoxNestingTime->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogLayoutSettings::GetEfficiencyCoefficient() const
{
    return ui->doubleSpinBoxEfficiency->value();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetEfficiencyCoefficient(qreal ration)
{
    ui->doubleSpinBoxEfficiency->setValue(ration);
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
QMarginsF DialogLayoutSettings::GetFields() const
{
    QMarginsF fields;
    fields.setLeft(UnitConvertor(ui->doubleSpinBoxLeftField->value(), oldLayoutUnit, Unit::Px));
    fields.setRight(UnitConvertor(ui->doubleSpinBoxRightField->value(), oldLayoutUnit, Unit::Px));
    fields.setTop(UnitConvertor(ui->doubleSpinBoxTopField->value(), oldLayoutUnit, Unit::Px));
    fields.setBottom(UnitConvertor(ui->doubleSpinBoxBottomField->value(), oldLayoutUnit, Unit::Px));
    return fields;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetFields(const QMarginsF &value)
{
    ui->doubleSpinBoxLeftField->setValue(UnitConvertor(value.left(), Unit::Px, LayoutUnit()));
    ui->doubleSpinBoxRightField->setValue(UnitConvertor(value.right(), Unit::Px, LayoutUnit()));
    ui->doubleSpinBoxTopField->setValue(UnitConvertor(value.top(), Unit::Px, LayoutUnit()));
    ui->doubleSpinBoxBottomField->setValue(UnitConvertor(value.bottom(), Unit::Px, LayoutUnit()));
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
        default:
            ui->radioButtonDescendingArea->setChecked(true);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetFollowGrainline() const
{
    return ui->checkBoxFollowGrainline->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetFollowGrainline(bool state)
{
    ui->checkBoxFollowGrainline->setChecked(state);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetManualPriority() const
{
    return ui->checkBoxManualPriority->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetManualPriority(bool state)
{
    ui->checkBoxManualPriority->setChecked(state);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetAutoCropLength() const
{
    return ui->checkBoxAutoCropLength->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetAutoCropLength(bool autoCropLength)
{
    ui->checkBoxAutoCropLength->setChecked(autoCropLength);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::GetAutoCropWidth() const
{
    return ui->checkBoxAutoCropWidth->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetAutoCropWidth(bool autoCropWidth)
{
    ui->checkBoxAutoCropWidth->setChecked(autoCropWidth);
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
bool DialogLayoutSettings::IsPreferOneSheetSolution() const
{
    return ui->checkBoxOneSheetSolution->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetPreferOneSheetSolution(bool prefer)
{
    ui->checkBoxOneSheetSolution->setChecked(prefer);
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
bool DialogLayoutSettings::IsStripOptimization() const
{
    return ui->groupBoxStrips->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetStripOptimization(bool save)
{
    ui->groupBoxStrips->setChecked(save);
}

//---------------------------------------------------------------------------------------------------------------------
quint8 DialogLayoutSettings::GetMultiplier() const
{
    return static_cast<quint8>(ui->spinBoxMultiplier->value());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetMultiplier(const quint8 &value)
{
    ui->spinBoxMultiplier->setValue(static_cast<int>(value));
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::IsIgnoreAllFields() const
{
    return ui->checkBoxIgnoreFileds->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetIgnoreAllFields(bool value)
{
    ui->checkBoxIgnoreFileds->setChecked(value);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::IsTextAsPaths() const
{
    return ui->checkBoxTextAsPaths->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetTextAsPaths(bool value)
{
    ui->checkBoxTextAsPaths->setChecked(value);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogLayoutSettings::IsNestQuantity() const
{
    return ui->checkBoxNestQuantity->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::SetNestQuantity(bool state)
{
    ui->checkBoxNestQuantity->setChecked(state);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLayoutSettings::SelectedPrinter() const
{
    return ui->comboBoxPrinter->currentText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::EnableLandscapeOrientation()
{
    if (static_cast<PaperSizeTemplate>(ui->comboBoxTemplates->currentData().toInt()) != PaperSizeTemplate::Custom)
    { // Force landscape only for not custom page format
        ui->toolButtonLandscape->setChecked(true);
    }
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

    const Unit paperUnit = PaperUnit();

    const int max = static_cast<int>(PaperSizeTemplate::Custom);
    for (int i=0; i < max; ++i)
    {
        const QSizeF tmplSize = GetTemplateSize(static_cast<PaperSizeTemplate>(i), paperUnit);
        if (QSizeF(width, height) == tmplSize || QSizeF(height, width) == tmplSize)
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

    const qreal left = ui->doubleSpinBoxLeftField->value();
    const qreal right = ui->doubleSpinBoxRightField->value();
    const qreal top = ui->doubleSpinBoxTopField->value();
    const qreal bottom = ui->doubleSpinBoxBottomField->value();

    ui->doubleSpinBoxPaperWidth->blockSignals(true);
    ui->doubleSpinBoxPaperHeight->blockSignals(true);
    ui->doubleSpinBoxPaperWidth->setMaximum(FromPixel(QIMAGE_MAX, paperUnit));
    ui->doubleSpinBoxPaperHeight->setMaximum(FromPixel(QIMAGE_MAX, paperUnit));
    ui->doubleSpinBoxPaperWidth->blockSignals(false);
    ui->doubleSpinBoxPaperHeight->blockSignals(false);

    const qreal newWidth = UnitConvertor(width, oldPaperUnit, paperUnit);
    const qreal newHeight = UnitConvertor(height, oldPaperUnit, paperUnit);

    const qreal newLeft = UnitConvertor(left, oldPaperUnit, paperUnit);
    const qreal newRight = UnitConvertor(right, oldPaperUnit, paperUnit);
    const qreal newTop = UnitConvertor(top, oldPaperUnit, paperUnit);
    const qreal newBottom = UnitConvertor(bottom, oldPaperUnit, paperUnit);

    oldPaperUnit = paperUnit;
    CorrectPaperDecimals();
    MinimumPaperSize();

    ui->doubleSpinBoxPaperWidth->setValue(newWidth);
    ui->doubleSpinBoxPaperHeight->setValue(newHeight);

    ui->doubleSpinBoxLeftField->setValue(newLeft);
    ui->doubleSpinBoxRightField->setValue(newRight);
    ui->doubleSpinBoxTopField->setValue(newTop);
    ui->doubleSpinBoxBottomField->setValue(newBottom);
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
    return tr("\n\tThree groups: big, middle, small = 0;\n\tTwo groups: big, small = 1;\n\tDescending area = 2");
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

    ui->doubleSpinBoxLayoutWidth->setMaximum(FromPixel(QIMAGE_MAX, unit));

    const qreal newLayoutWidth = UnitConvertor(layoutWidth, oldLayoutUnit, unit);

    oldLayoutUnit = unit;
    CorrectLayoutDecimals();
    MinimumLayoutSize();

    ui->doubleSpinBoxLayoutWidth->setValue(newLayoutWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::PaperSizeChanged()
{
    if (ui->doubleSpinBoxPaperHeight->value() > ui->doubleSpinBoxPaperWidth->value())
    {
        ui->toolButtonPortrait->blockSignals(true);
        ui->toolButtonPortrait->setChecked(true);
        ui->toolButtonPortrait->blockSignals(false);
    }
    else
    {
        ui->toolButtonLandscape->blockSignals(true);
        ui->toolButtonLandscape->setChecked(true);
        ui->toolButtonLandscape->blockSignals(false);
    }
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
    generator->SetNestingTime(GetNestingTime());
    generator->SetEfficiencyCoefficient(GetEfficiencyCoefficient());
    generator->SetFollowGrainline(GetFollowGrainline());
    generator->SetManualPriority(GetManualPriority());
    generator->SetAutoCropLength(GetAutoCropLength());
    generator->SetAutoCropWidth(GetAutoCropWidth());
    generator->SetSaveLength(IsSaveLength());
    generator->SetPreferOneSheetSolution(IsPreferOneSheetSolution());
    generator->SetUnitePages(IsUnitePages());
    generator->SetStripOptimization(IsStripOptimization());
    generator->SetMultiplier(GetMultiplier());
    generator->SetTextAsPaths(IsTextAsPaths());
    generator->SetNestQuantity(IsNestQuantity());

    if (IsIgnoreAllFields())
    {
        generator->SetPrinterFields(false, QMarginsF());
    }
    else
    {
        QPrinterInfo printer = QPrinterInfo::printerInfo(ui->comboBoxPrinter->currentText());
        if (printer.isNull())
        {
            generator->SetPrinterFields(true, GetFields());
        }
        else
        {
            const QMarginsF minFields = MinPrinterFields();
            const QMarginsF fields = GetFields();
            if (fields.left() < minFields.left() || fields.right() < minFields.right() ||
                fields.top() < minFields.top() || fields.bottom() < minFields.bottom())
            {
                QMessageBox::StandardButton answer;
                answer = QMessageBox::question(this, tr("Wrong fields."),
                                               tr("Margins go beyond printing. \n\nApply settings anyway?"),
                                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
                if (answer == QMessageBox::No)
                {
                    if (fields.left() < minFields.left())
                    {
                        ui->doubleSpinBoxLeftField->setValue(UnitConvertor(minFields.left(), Unit::Px, LayoutUnit()));
                    }

                    if (fields.right() < minFields.right())
                    {
                        ui->doubleSpinBoxRightField->setValue(UnitConvertor(minFields.right(), Unit::Px, LayoutUnit()));
                    }

                    if (fields.top() < minFields.top())
                    {
                        ui->doubleSpinBoxTopField->setValue(UnitConvertor(minFields.top(), Unit::Px, LayoutUnit()));
                    }

                    if (fields.bottom() < minFields.bottom())
                    {
                        ui->doubleSpinBoxBottomField->setValue(UnitConvertor(minFields.bottom(), Unit::Px,
                                                                             LayoutUnit()));
                    }

                    generator->SetPrinterFields(true, GetFields());
                }
                else
                {
                    generator->SetPrinterFields(false, GetFields());
                }
            }
            else
            {
                generator->SetPrinterFields(true, GetFields());
            }
        }
    }

    //don't want to break visual settings when cmd used
    if (disableSettings == false)
    {
        WriteSettings();
    }
    emit accepted();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::RestoreDefaults()
{
    ui->comboBoxTemplates->blockSignals(true);
    ui->comboBoxTemplates->setCurrentIndex(0);//A0
    TemplateSelected();
    ui->comboBoxTemplates->blockSignals(false);

    ui->comboBoxPrinter->blockSignals(true);
    InitPrinter();
    ui->comboBoxPrinter->blockSignals(false);

    SetLayoutWidth(VSettings::GetDefLayoutWidth());
    SetGroup(VSettings::GetDefLayoutGroup());
    SetFollowGrainline(VSettings::GetDefLayoutFollowGrainline());
    SetManualPriority(VSettings::GetDefLayoutManualPriority());
    SetFields(GetDefPrinterFields());
    SetIgnoreAllFields(VSettings::GetDefIgnoreAllFields());
    SetMultiplier(VSettings::GetDefMultiplier());
    SetNestingTime(VSettings::GetDefNestingTime());
    SetEfficiencyCoefficient(VSettings::GetDefEfficiencyCoefficient());
    SetNestQuantity(VSettings::GetDefLayoutNestQuantity());
    SetPreferOneSheetSolution(VSettings::GetDefLayoutPreferOneSheetSolution());

    CorrectMaxFileds();
    IgnoreAllFields(ui->checkBoxIgnoreFileds->isChecked());

    ui->checkBoxTextAsPaths->setChecked(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::PrinterMargins()
{
    QPrinterInfo printer = QPrinterInfo::printerInfo(ui->comboBoxPrinter->currentText());
    if (not printer.isNull())
    {
        SetFields(GetPrinterFields(QSharedPointer<QPrinter>(new QPrinter(printer))));
    }
    else
    {
        SetFields(QMarginsF());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::CorrectMaxFileds()
{
    const qreal width = ui->doubleSpinBoxPaperWidth->value();
    const qreal height = ui->doubleSpinBoxPaperHeight->value();

    // 80%/2 of paper size for each field
    const qreal widthField = (width*80.0/100.0)/2.0;
    const qreal heightField = (height*80.0/100.0)/2.0;

    ui->doubleSpinBoxLeftField->setMaximum(widthField);
    ui->doubleSpinBoxRightField->setMaximum(widthField);
    ui->doubleSpinBoxTopField->setMaximum(heightField);
    ui->doubleSpinBoxBottomField->setMaximum(heightField);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::IgnoreAllFields(int state)
{
    ui->doubleSpinBoxLeftField->setDisabled(state);
    ui->doubleSpinBoxRightField->setDisabled(state);
    ui->doubleSpinBoxTopField->setDisabled(state);
    ui->doubleSpinBoxBottomField->setDisabled(state);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitPaperUnits()
{
    ui->comboBoxPaperSizeUnit->addItem(tr("Millimiters"), QVariant(UnitsToStr(Unit::Mm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Centimeters"), QVariant(UnitsToStr(Unit::Cm)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Inches"), QVariant(UnitsToStr(Unit::Inch)));
    ui->comboBoxPaperSizeUnit->addItem(tr("Pixels"), QVariant(UnitsToStr(Unit::Px)));

    // set default unit
    oldPaperUnit = StrToUnits(qApp->ValentinaSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxPaperSizeUnit->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxPaperSizeUnit->setCurrentIndex(indexUnit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitLayoutUnits()
{
    ui->comboBoxLayoutUnit->addItem(tr("Centimeters"), QVariant(UnitsToStr(Unit::Cm)));
    ui->comboBoxLayoutUnit->addItem(tr("Millimiters"), QVariant(UnitsToStr(Unit::Mm)));
    ui->comboBoxLayoutUnit->addItem(tr("Inches"), QVariant(UnitsToStr(Unit::Inch)));

    // set default unit
    oldLayoutUnit = StrToUnits(qApp->ValentinaSettings()->GetUnit());
    const qint32 indexUnit = ui->comboBoxLayoutUnit->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->comboBoxLayoutUnit->setCurrentIndex(indexUnit);
    }
}


//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::InitPrinter()
{
    ui->comboBoxPrinter->clear();
    ui->comboBoxPrinter->addItems(QPrinterInfo::availablePrinterNames());

    if (ui->comboBoxPrinter->count() == 0)
    {
        ui->comboBoxPrinter->addItem(tr("None", "Printer"));
    }
    else
    {
        const int index = ui->comboBoxPrinter->findText(QPrinterInfo::defaultPrinterName());
        if(index != -1)
        {
            ui->comboBoxPrinter->setCurrentIndex(index);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLayoutSettings::MakeHelpTemplateList()
{
    QString out = "\n";

    auto cntr = static_cast<VIndexType>(PaperSizeTemplate::A0);
    for (int i = 0; i < VAbstractLayoutDialog::pageFormatNames.size(); ++i)
    {
        if (cntr < static_cast<int>(PaperSizeTemplate::Custom))// Don't include custom template
        {
            out += "\t* "+VAbstractLayoutDialog::pageFormatNames.at(i)+" = "+ QString::number(cntr++);

            if (i < VAbstractLayoutDialog::pageFormatNames.size() - 2)
            {
               out += ",\n";
            }
            else
            {
               out += ".\n";
            }
        }
    }
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLayoutSettings::MakeHelpTiledPdfTemplateList()
{
    QString out = "\n";

    for (int i = 0; i <= static_cast<int>(PaperSizeTemplate::Tabloid); ++i)
    {
        out += "\t* "+VAbstractLayoutDialog::pageFormatNames.at(i)+" = "+ QString::number(i);

        if (i < static_cast<int>(PaperSizeTemplate::Tabloid))
        {
           out += ",\n";
        }
        else
        {
           out += ".\n";
        }
    }
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF DialogLayoutSettings::Template()
{
    const PaperSizeTemplate temp = static_cast<PaperSizeTemplate>(ui->comboBoxTemplates->currentData().toInt());

    const Unit paperUnit = PaperUnit();

    switch (temp)
    {
        case PaperSizeTemplate::A0:
        case PaperSizeTemplate::A1:
        case PaperSizeTemplate::A2:
        case PaperSizeTemplate::A3:
        case PaperSizeTemplate::A4:
        case PaperSizeTemplate::Letter:
        case PaperSizeTemplate::Legal:
        case PaperSizeTemplate::Tabloid:
            SetAdditionalOptions(false);
            return GetTemplateSize(temp, paperUnit);
        case PaperSizeTemplate::Roll24in:
        case PaperSizeTemplate::Roll30in:
        case PaperSizeTemplate::Roll36in:
        case PaperSizeTemplate::Roll42in:
        case PaperSizeTemplate::Roll44in:
        case PaperSizeTemplate::Roll48in:
        case PaperSizeTemplate::Roll62in:
        case PaperSizeTemplate::Roll72in:
            SetAdditionalOptions(true);
            return GetTemplateSize(temp, paperUnit);
        case PaperSizeTemplate::Custom:
            return GetTemplateSize(temp, paperUnit);
        default:
            break;
    }
    return QSizeF();
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF DialogLayoutSettings::GetTemplateSize(const PaperSizeTemplate &tmpl, const Unit &unit) const
{
    qreal width = 0;
    qreal height = 0;

    switch (tmpl)
    {
        case PaperSizeTemplate::Custom:
            width = ui->doubleSpinBoxPaperWidth->value();
            height = ui->doubleSpinBoxPaperHeight->value();
            return RoundTemplateSize(width, height, unit);
        default:
            return VAbstractLayoutDialog::GetTemplateSize(tmpl, unit);
    }
}




//---------------------------------------------------------------------------------------------------------------------
QMarginsF DialogLayoutSettings::MinPrinterFields() const
{
    QPrinterInfo printer = QPrinterInfo::printerInfo(ui->comboBoxPrinter->currentText());
    if (not printer.isNull())
    {
        QSharedPointer<QPrinter> pr = QSharedPointer<QPrinter>(new QPrinter(printer));
        return GetMinPrinterFields(pr);
    }
    else
    {
        return QMarginsF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF DialogLayoutSettings::GetDefPrinterFields() const
{
    QPrinterInfo printer = QPrinterInfo::printerInfo(ui->comboBoxPrinter->currentText());
    if (not printer.isNull())
    {
        return GetPrinterFields(QSharedPointer<QPrinter>(new QPrinter(printer)));
    }
    else
    {
        return QMarginsF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogLayoutSettings::PaperUnit() const
{
    return StrToUnits(ui->comboBoxPaperSizeUnit->currentData().toString());
}

//---------------------------------------------------------------------------------------------------------------------
Unit DialogLayoutSettings::LayoutUnit() const
{
    return StrToUnits(ui->comboBoxLayoutUnit->currentData().toString());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::CorrectPaperDecimals()
{
    switch (oldPaperUnit)
    {
        case Unit::Cm:
        case Unit::Mm:
        case Unit::Px:
            ui->doubleSpinBoxPaperWidth->setDecimals(2);
            ui->doubleSpinBoxPaperHeight->setDecimals(2);

            ui->doubleSpinBoxLeftField->setDecimals(4);
            ui->doubleSpinBoxRightField->setDecimals(4);
            ui->doubleSpinBoxTopField->setDecimals(4);
            ui->doubleSpinBoxBottomField->setDecimals(4);
            break;
        case Unit::Inch:
            ui->doubleSpinBoxPaperWidth->setDecimals(5);
            ui->doubleSpinBoxPaperHeight->setDecimals(5);

            ui->doubleSpinBoxLeftField->setDecimals(5);
            ui->doubleSpinBoxRightField->setDecimals(5);
            ui->doubleSpinBoxTopField->setDecimals(5);
            ui->doubleSpinBoxBottomField->setDecimals(5);
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
        case Unit::Px:
            ui->doubleSpinBoxLayoutWidth->setDecimals(2);
            break;
        case Unit::Inch:
            ui->doubleSpinBoxLayoutWidth->setDecimals(5);
            break;
        default:
            break;
    }
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
    SetNestingTime(settings->GetNestingTime());
    SetEfficiencyCoefficient(settings->GetEfficiencyCoefficient());

    const qreal width = UnitConvertor(settings->GetLayoutPaperWidth(), Unit::Px, LayoutUnit());
    const qreal height = UnitConvertor(settings->GetLayoutPaperHeight(), Unit::Px, LayoutUnit());
    SheetSize(QSizeF(width, height));
    SetGroup(settings->GetLayoutGroup());
    SetFollowGrainline(settings->GetLayoutFollowGrainline());
    SetManualPriority(settings->GetLayoutManualPriority());
    SetAutoCropLength(settings->GetLayoutAutoCropLength());
    SetAutoCropWidth(settings->GetLayoutAutoCropWidth());
    SetSaveLength(settings->GetLayoutSaveLength());
    SetPreferOneSheetSolution(settings->GetLayoutPreferOneSheetSolution());
    SetUnitePages(settings->GetLayoutUnitePages());
    SetFields(settings->GetFields(GetDefPrinterFields()));
    SetIgnoreAllFields(settings->GetIgnoreAllFields());
    SetStripOptimization(settings->GetStripOptimization());
    SetMultiplier(settings->GetMultiplier());
    SetTextAsPaths(settings->GetTextAsPaths());
    SetNestQuantity(settings->GetLayoutNestQuantity());

    FindTemplate();

    CorrectMaxFileds();
    IgnoreAllFields(ui->checkBoxIgnoreFileds->isChecked());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutSettings::WriteSettings() const
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetLayoutWidth(GetLayoutWidth());
    settings->SetLayoutGroup(GetGroup());
    settings->SetLayoutPaperHeight(GetPaperHeight());
    settings->SetLayoutPaperWidth(GetPaperWidth());
    settings->SetLayoutFollowGrainline(GetFollowGrainline());
    settings->SetLayoutManualPriority(GetManualPriority());
    settings->SetLayoutAutoCropLength(GetAutoCropLength());
    settings->SetLayoutAutoCropWidth(GetAutoCropWidth());
    settings->SetLayoutSaveLength(IsSaveLength());
    settings->SetLayoutPreferOneSheetSolution(IsPreferOneSheetSolution());
    settings->SetLayoutUnitePages(IsUnitePages());
    settings->SetFields(GetFields());
    settings->SetIgnoreAllFields(IsIgnoreAllFields());
    settings->SetStripOptimization(IsStripOptimization());
    settings->SetMultiplier(GetMultiplier());
    settings->SetTextAsPaths(IsTextAsPaths());
    settings->SetNestingTime(GetNestingTime());
    settings->SetEfficiencyCoefficient(GetEfficiencyCoefficient());
    settings->SetLayoutNestQuantity(IsNestQuantity());
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
    SetAutoCropLength(value);
    SetAutoCropWidth(value);
    SetSaveLength(value);
    SetUnitePages(value);
    SetStripOptimization(value);
}
