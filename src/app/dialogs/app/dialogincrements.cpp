/************************************************************************
 **
 **  @file   dialogincrements.cpp
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

#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include "../../widgets/doubledelegate.h"
#include "../../widgets/textdelegate.h"
#include "../../widgets/vwidgetpopup.h"
#include "../../xml/vstandardmeasurements.h"
#include "../../xml/vindividualmeasurements.h"
#include "../../core/vsettings.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTableWidget>
#include <QSettings>
#include <QTableWidgetItem>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogIncrements create dialog
 * @param data container with data
 * @param doc dom document container
 * @param parent parent widget
 */
DialogIncrements::DialogIncrements(VContainer *data, VPattern *doc, QWidget *parent)
    :DialogTool(data, NULL_ID, parent), ui(new Ui::DialogIncrements), data(data), doc(doc), row(0), column(0),
      m(nullptr)
{
    ui->setupUi(this);

    qApp->getSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    qCDebug(vDialog, "Showing variables.");
    if (qApp->patternType() == MeasurementsType::Individual)
    {
        qCDebug(vDialog, "Pattern with individual measurements.");
        const QString filePath = doc->MPath();
        try
        {
            VDomDocument::ValidateXML("://schema/individual_measurements.xsd", filePath);
            m = new VIndividualMeasurements(data);
            m->setXMLContent(filePath);
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            emit DialogClosed(QDialog::Rejected);
            return;
        }
    }

    ShowUnits();

    TextDelegate *textDelegate = new TextDelegate(NameRegExp(), data, ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(0, textDelegate);// name
    DoubleSpinBoxDelegate *doubleDelegate = new DoubleSpinBoxDelegate(ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(2, doubleDelegate);// base value
    ui->tableWidgetIncrement->setItemDelegateForColumn(3, doubleDelegate);// in sizes
    ui->tableWidgetIncrement->setItemDelegateForColumn(4, doubleDelegate);// in heights

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        ui->checkBoxHideEmpty->setChecked(true);
    }

    FillMeasurements();
    FillIncrements();
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillLengthArcs();
    FillRadiusesArcs();
    FillAnglesArcs();
    FillAnglesCurves();

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        ui->toolBoxMeasurements->setItemEnabled(0, false);

        const QString filePath = doc->MPath();
        VStandardMeasurements *mSt = nullptr;
        try
        {
            VDomDocument::ValidateXML("://schema/standard_measurements.xsd", filePath);
            mSt = new VStandardMeasurements(data);
            mSt->setXMLContent(filePath);

            ui->labelBaseValues->setText(tr("Base size: %1 %3; Base height: %2 %3").arg(mSt->Size())
                                         .arg(mSt->Height()).arg(VDomDocument::UnitsToStr(qApp->patternUnit())));
            ui->labelDescription->setText(tr("Description: \"%1\"").arg(mSt->TrDescription()));
            delete mSt;
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            if (mSt != nullptr)
            {
                delete mSt;
            }
            emit DialogClosed(QDialog::Rejected);
            return;
        }
    }
    else
    {
        QRegularExpressionValidator *reg =
             new QRegularExpressionValidator(QRegularExpression("\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*"));
        ui->lineEditMail->setValidator(reg);

        HideColumns(ui->tableWidgetMeasurements);
        HideColumns(ui->tableWidgetIncrement);

        ui->tableWidgetMeasurements->setItemDelegateForColumn(2, doubleDelegate);// base value

        connect(ui->tableWidgetMeasurements, &QTableWidget::cellChanged, this, &DialogIncrements::MeasurementChanged);

        ui->lineEditGivenName->setText(m->GivenName());
        ui->lineEditFamilyName->setText(m->FamilyName());

        ui->comboBoxSex->addItem(tr("male"), QVariant(m->GenderToStr(SexType::Male)));
        ui->comboBoxSex->addItem(tr("female"), QVariant(m->GenderToStr(SexType::Female)));
        qint32 index = ui->comboBoxSex->findData(m->GenderToStr(m->Sex()));
        if (index != -1)
        {
            ui->comboBoxSex->setCurrentIndex(index);
        }

        ui->dateEditBirthDate->setDate(m->BirthDate());
        ui->lineEditMail->setText(m->Mail());

        connect(ui->lineEditGivenName, &QLineEdit::editingFinished, this, &DialogIncrements::SaveGivenName);
        connect(ui->lineEditFamilyName, &QLineEdit::editingFinished, this, &DialogIncrements::SaveFamilyName);
        connect(ui->lineEditMail, &QLineEdit::editingFinished, this, &DialogIncrements::SaveEmail);
        connect(ui->comboBoxSex, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                &DialogIncrements::SaveSex);
        connect(ui->dateEditBirthDate, &QDateEdit::dateChanged, this, &DialogIncrements::SaveBirthDate);

        // hide fileds that don't exist in individual measurements
        ui->labelBaseValues->setVisible(false);
        ui->labelDescription->setVisible(false);
    }

    ui->toolBoxMeasurements->setCurrentIndex(1);

    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this, &DialogIncrements::IncrementChanged);
    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonAdd);
    connect(ui->toolButtonRemove, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonRemove);

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VPattern::LiteParseTree);
    connect(this, &DialogIncrements::haveLiteChange, this->doc, &VPattern::haveLiteChange);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &DialogIncrements::FullUpdateFromFile);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->toolButtonOpenMeasurements, &QToolButton::clicked, this, &DialogIncrements::OpenTable);
    connect(ui->checkBoxHideEmpty, &QCheckBox::stateChanged, this, &DialogIncrements::ShowMeasurements);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillMeasurements load measurements data
 */
void DialogIncrements::FillMeasurements()
{
    const QMap<QString, QSharedPointer<VMeasurement> > table = data->DataMeasurements();
    qint32 currentRow = -1;
    QMap<QString, QSharedPointer<VMeasurement> >::const_iterator iMap;
    ui->tableWidgetMeasurements->setRowCount ( table.size() );
    for (iMap = table.constBegin(); iMap != table.constEnd(); ++iMap)
    {
        QSharedPointer<VMeasurement> m = iMap.value();
        if (ui->checkBoxHideEmpty->isChecked() && m->IsNotUsed())
        {
            continue; // skip this measurement
        }
        currentRow++;

        QTableWidgetItem *item = new QTableWidgetItem(QString(iMap.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setToolTip(m->GetGuiText());
        SetItemViewOnly(item);
        item->setTextAlignment(Qt::AlignLeft);
        ui->tableWidgetMeasurements->setItem(currentRow, 0, item);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            QTableWidgetItem *item = new QTableWidgetItem(qApp->LocaleToString(
                                                         data->GetTableValue(qApp->TrVars()->VarFromUser(iMap.key()),
                                                              qApp->patternType())));
            item->setTextAlignment(Qt::AlignHCenter);
            SetItemViewOnly(item);
            ui->tableWidgetMeasurements->setItem(currentRow, 1, item);// calculated value
        }

        item = new QTableWidgetItem(qApp->LocaleToString(m->GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        if (qApp->patternType() == MeasurementsType::Standard)
        {
            SetItemViewOnly(item);
        }
        ui->tableWidgetMeasurements->setItem(currentRow, 2, item);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            QTableWidgetItem *item = new QTableWidgetItem(qApp->LocaleToString(m->GetKsize()));
            item->setTextAlignment(Qt::AlignHCenter);
            SetItemViewOnly(item);
            ui->tableWidgetMeasurements->setItem(currentRow, 3, item);// in sizes

            item = new QTableWidgetItem(qApp->LocaleToString(m->GetKheight()));
            item->setTextAlignment(Qt::AlignHCenter);
            SetItemViewOnly(item);
            ui->tableWidgetMeasurements->setItem(currentRow, 4, item);// in heights
        }

        item = new QTableWidgetItem(m->GetDescription());
        item->setToolTip(m->GetDescription());
        SetItemViewOnly(item);
        item->setTextAlignment(Qt::AlignLeft);
        ui->tableWidgetMeasurements->setItem(currentRow, 5, item);
    }
    ui->tableWidgetMeasurements->setRowCount(currentRow+1);
    ui->tableWidgetMeasurements->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetMeasurements->resizeColumnsToContents();
    ui->tableWidgetMeasurements->resizeRowsToContents();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillIncrementTable fill data for increment table
 */
void DialogIncrements::FillIncrements()
{
    const QMap<QString, QSharedPointer<VIncrement> > increments = data->DataIncrements();
    QMap<QString, QSharedPointer<VIncrement> >::const_iterator i;
    QMap<quint32, QString> map;
    //Sorting QHash by id
    for (i = increments.constBegin(); i != increments.constEnd(); ++i)
    {
        QSharedPointer<VIncrement> incr = i.value();
        map.insert(incr->getId(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<quint32, QString> iMap(map);
    while (iMap.hasNext())
    {
        iMap.next();
        QSharedPointer<VIncrement> incr = increments.value(iMap.value());
        currentRow++;
        ui->tableWidgetIncrement->setRowCount ( increments.size() );

        QTableWidgetItem *item = new QTableWidgetItem(iMap.value());
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, incr->getId());
        ui->tableWidgetIncrement->setItem(currentRow, 0, item);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            item = new QTableWidgetItem(qApp->LocaleToString(data->GetTableValue(iMap.value(), qApp->patternType())));
            item->setTextAlignment(Qt::AlignHCenter);
            SetItemViewOnly(item);
            ui->tableWidgetIncrement->setItem(currentRow, 1, item);
        }

        item = new QTableWidgetItem(qApp->LocaleToString(incr->GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 2, item);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            item = new QTableWidgetItem(qApp->LocaleToString(incr->GetKsize()));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidgetIncrement->setItem(currentRow, 3, item);

            item = new QTableWidgetItem(qApp->LocaleToString(incr->GetKheight()));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidgetIncrement->setItem(currentRow, 4, item);
        }

        item = new QTableWidgetItem(incr->GetDescription());
        item->setToolTip(incr->GetDescription());
        item->setTextAlignment(Qt::AlignLeft);
        ui->tableWidgetIncrement->setItem(currentRow, 5, item);
    }
    if (ui->tableWidgetIncrement->rowCount()>0)
    {
        ui->toolButtonRemove->setEnabled(true);
    }
    ui->tableWidgetIncrement->resizeColumnsToContents();
    ui->tableWidgetIncrement->resizeRowsToContents();
    ui->tableWidgetIncrement->setCurrentCell( row, column );
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void DialogIncrements::FillTable(const QMap<QString, T> varTable, QTableWidget *table)
{
    SCASSERT(table != nullptr);

    qint32 currentRow = -1;
    QMapIterator<QString, T> i(varTable);
    while (i.hasNext())
    {
        i.next();
        qreal length = *i.value()->GetValue();
        currentRow++;
        table->setRowCount ( varTable.size() );

        QTableWidgetItem *item = new QTableWidgetItem(i.key());
        item->setTextAlignment(Qt::AlignLeft);
        item->setFont(QFont("Times", 12, QFont::Bold));
        table->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(qApp->LocaleToString(length));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 1, item);
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->verticalHeader()->setDefaultSectionSize(20);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthLines fill data for table of lines lengths
 */
void DialogIncrements::FillLengthsLines()
{
    FillTable(data->DataLengthLines(), ui->tableWidgetLines);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillLengthLinesAngles()
{
    FillTable(data->DataAngleLines(), ui->tableWidgetLinesAngles);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthSplines fill data for table of splines lengths
 */
void DialogIncrements::FillLengthsCurves()
{
    FillTable(data->DataLengthSplines(), ui->tableWidgetSplines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthArcs fill data for table of arcs lengths
 */
void DialogIncrements::FillLengthArcs()
{
    FillTable(data->DataLengthArcs(), ui->tableWidgetArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillRadiusesArcs()
{
    FillTable(data->DataRadiusesArcs(), ui->tableWidgetRadiusesArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillAnglesArcs()
{
    FillTable(data->DataAnglesArcs(), ui->tableWidgetAnglesArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillAnglesCurves()
{
    FillTable(data->DataAnglesCurves(), ui->tableWidgetAnglesCurves);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SetItemViewOnly(QTableWidgetItem *item)
{
    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowUnits()
{
    const QString unit = VDomDocument::UnitsToStr(qApp->patternUnit());

    ShowHeaderUnits(ui->tableWidgetIncrement, 2, unit);// base value
    ShowHeaderUnits(ui->tableWidgetIncrement, 3, unit);// in sizes
    ShowHeaderUnits(ui->tableWidgetIncrement, 4, unit);// in heights

    ShowHeaderUnits(ui->tableWidgetMeasurements, 2, unit);// base value
    ShowHeaderUnits(ui->tableWidgetMeasurements, 3, unit);// in sizes
    ShowHeaderUnits(ui->tableWidgetMeasurements, 4, unit);// in heights

    ShowHeaderUnits(ui->tableWidgetLines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetSplines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetArcs, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetLinesAngles, 1, "°");// angle
    ShowHeaderUnits(ui->tableWidgetRadiusesArcs, 1, unit);// radius
    ShowHeaderUnits(ui->tableWidgetAnglesArcs, 1, "°");// angle
    ShowHeaderUnits(ui->tableWidgetAnglesCurves, 1, "°");// angle
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowHeaderUnits(QTableWidget *table, int column, const QString &unit)
{
    SCASSERT(table != nullptr);

    const QString header = table->horizontalHeaderItem(column)->text();
    const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
    table->horizontalHeaderItem(column)->setText(unitHeader);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowSuccess() const
{
    VWidgetPopup *popup = new VWidgetPopup();
    QLabel *label = new QLabel(tr("Data successfully saved."));
    QFont f = label->font();
    f.setBold(true);
    f.setPixelSize(16);
    label->setFont(f);
    popup->SetWidget(label);
    popup->SetLifeTime(2000);
    popup->Show(frameGeometry().center());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowMeasurements()
{
    if (qApp->patternType() == MeasurementsType::Individual)
    {
        ui->tableWidgetMeasurements->blockSignals(true);
    }
    ui->tableWidgetMeasurements->clearContents();
    FillMeasurements();
    ui->tableWidgetMeasurements->horizontalHeader()->setStretchLastSection(true);
    if (qApp->patternType() == MeasurementsType::Individual)
    {
        ui->tableWidgetMeasurements->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update information in tables form file
 */
void DialogIncrements::FullUpdateFromFile()
{
    ShowMeasurements();

    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->clearContents();
    FillIncrements();
    ui->tableWidgetIncrement->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetIncrement->blockSignals(false);

    ui->tableWidgetLines->clearContents();
    FillLengthsLines();

    ui->tableWidgetSplines->clearContents();
    FillLengthsCurves();

    ui->tableWidgetArcs->clearContents();
    FillLengthArcs();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveGivenName()
{
    m->setGivenName(ui->lineEditGivenName->text());
    QString error;
    if (m->SaveDocument(doc->MPath(), error) == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save GivenName"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    else
    {
        ShowSuccess();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveFamilyName()
{

    m->setFamilyName(ui->lineEditFamilyName->text());
    QString error;
    if (m->SaveDocument(doc->MPath(), error) == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save FamilyName"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    else
    {
        ShowSuccess();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveEmail()
{
    m->setMail(ui->lineEditMail->text());
    QString error;
    if (m->SaveDocument(doc->MPath(), error) == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save Email"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    else
    {
        ShowSuccess();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveSex(int index)
{
    m->setSex(m->StrToGender(ui->comboBoxSex->itemData(index).toString()));
    QString error;
    if (m->SaveDocument(doc->MPath(), error) == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save Sex"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    else
    {
        ShowSuccess();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveBirthDate(const QDate & date)
{
    m->setBirthDate(date);
    QString error;
    if (m->SaveDocument(doc->MPath(), error) == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save BirthDate"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    else
    {
        ShowSuccess();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::OpenTable()
{
    const QString text = tr("Measurements use different units than pattern. This pattern required measurements in %1")
            .arg(doc->UnitsToStr(qApp->patternUnit()));
    if (qApp->patternType() == MeasurementsType::Individual)
    {
        const QString filter(tr("Individual measurements (*.vit)"));

        //Use standard path to individual measurements
        const QString path = qApp->getSettings()->GetPathIndividualMeasurements();

        const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
        if (filePath.isEmpty())
        {
            return;
        }

        VIndividualMeasurements *m1 = nullptr;
        try
        {
            VDomDocument::ValidateXML("://schema/individual_measurements.xsd", filePath);

            m1 = new VIndividualMeasurements(data);
            m1->setXMLContent(filePath);
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            delete m1;
            emit DialogClosed(QDialog::Rejected);
            return;
        }
        Unit mUnit = m1->MUnit();
        if (qApp->patternUnit() != mUnit)
        {
            QMessageBox::critical(this, tr("Wrong units."), text);
            delete m1;
            return;
        }
        delete m;
        m = m1;
        data->ClearVariables(VarType::Measurement);
        m->Measurements();
        emit FullUpdateTree(Document::LiteParse);

        doc->SetPath(filePath);
        emit haveLiteChange();
    }
    else
    {
        const QString filter(tr("Standard measurements (*.vst)"));
        //Use standard path to standard measurements
        const QString path = qApp->getSettings()->GetPathStandardMeasurements();
        const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
        if (filePath.isEmpty())
        {
            return;
        }

        VStandardMeasurements *m1 = nullptr;
        try
        {
            VDomDocument::ValidateXML("://schema/standard_measurements.xsd", filePath);

            m1 = new VStandardMeasurements(data);
            m1->setXMLContent(filePath);
            Unit mUnit = m1->MUnit();
            if (qApp->patternUnit() != mUnit)
            {
                QMessageBox::critical(this, tr("Wrong units."), text);
                delete m1;
                return;
            }
            m1->SetSize();
            m1->SetHeight();
            data->ClearVariables(VarType::Measurement);
            m1->Measurements();
            delete m1;
            emit FullUpdateTree(Document::LiteParse);

            doc->SetPath(filePath);
            emit haveLiteChange();
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            delete m1;
            emit DialogClosed(QDialog::Rejected);
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonAdd create new row in table
 */
void DialogIncrements::clickedToolButtonAdd()
{
    qCDebug(vDialog, "Add new increment");
    ui->tableWidgetIncrement->setFocus(Qt::OtherFocusReason);
    ui->tableWidgetIncrement->blockSignals(true);
    qint32 currentRow  = ui->tableWidgetIncrement->rowCount();
    ui->tableWidgetIncrement->insertRow( currentRow );

    qint32 num = 1;
    QString name;
    do
    {
        name = QString(tr("Name_%1")).arg(num);
        num++;
    } while (data->IsUnique(name)==false);

    const quint32 id = data->getNextId();
    const QString description(tr("Description"));
    VIncrement *incr = new VIncrement(name, id, 0, 0, 0, description);
    data->AddVariable(name, incr);

    AddIncrementToFile(id, name, 0, 0, 0, description);

    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFont(QFont("Times", 12, QFont::Bold));
    item->setData(Qt::UserRole, id);
    ui->tableWidgetIncrement->setItem(currentRow, 0, item);
    ui->tableWidgetIncrement->setCurrentCell(currentRow, 0, QItemSelectionModel::ClearAndSelect);

    item = new QTableWidgetItem("0");// calculated value
    item->setTextAlignment(Qt::AlignHCenter);
    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
    ui->tableWidgetIncrement->setItem(currentRow, 1, item);

    item = new QTableWidgetItem("0");// base value
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 2, item);

    item = new QTableWidgetItem("0");// in sizes
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 3, item);

    item = new QTableWidgetItem("0"); // in heights
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 4, item);

    item = new QTableWidgetItem(description);
    item->setTextAlignment(Qt::AlignLeft);
    ui->tableWidgetIncrement->setItem(currentRow, 5, item);

    ui->toolButtonRemove->setEnabled(true);
    ui->tableWidgetIncrement->blockSignals(false);
    emit haveLiteChange();
    ShowSuccess();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonRemove remove one row from table
 */
void DialogIncrements::clickedToolButtonRemove()
{
    ui->tableWidgetIncrement->blockSignals(true);

    QTableWidgetItem *item = ui->tableWidgetIncrement->currentItem();
    qint32 row = item->row();

    QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    qCDebug(vDialog, "Remove increment %s", itemName->text().toUtf8().constData());
    data->RemoveIncrement(itemName->text());

    quint32 id = qvariant_cast<quint32>(itemName->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        QDomNodeList list = doc->elementsByTagName(VPattern::TagIncrements);
        list.at(0).removeChild(domElement);
    }
    else
    {
        qCDebug(vDialog, "Could not find object with id %u", id);
        return;
    }

    ui->tableWidgetIncrement->removeRow(row);
    if (ui->tableWidgetIncrement->rowCount() == 0)
    {
        ui->toolButtonRemove->setEnabled(false);
    }

    ui->tableWidgetIncrement->blockSignals(false);
    emit haveLiteChange();
    ShowSuccess();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddIncrementToFile save created increment to file
 * @param id id of increment
 * @param name name
 * @param base base value
 * @param ksize increment in sizes
 * @param kheight increment in heights
 * @param description description of increment
 */
void DialogIncrements::AddIncrementToFile(const quint32 &id, const QString &name, const qreal &base, const qreal &ksize,
                                          const qreal &kheight, const QString &description)
{
    qCDebug(vDialog, "Saving new increment to file.");
    qCDebug(vDialog, "%s", QString("Increment: id(%1), name(%2), base(%3), ksize(%4), kheight(%5), description(%6)")
                      .arg(id).arg(name).arg(base).arg(ksize).arg(kheight).arg(description).toUtf8().constData());
    QDomElement element = doc->createElement(VPattern::TagIncrement);

    doc->SetAttribute(element, VDomDocument::AttrId, id);
    doc->SetAttribute(element, VPattern::IncrementName, name);
    doc->SetAttribute(element, VPattern::IncrementBase, base);
    doc->SetAttribute(element, VPattern::IncrementKsize, ksize);
    doc->SetAttribute(element, VPattern::IncrementKgrowth, kheight);
    doc->SetAttribute(element, VPattern::IncrementDescription, description);

    QDomNodeList list = doc->elementsByTagName(VPattern::TagIncrements);
    list.at(0).appendChild(element);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::HideColumns(QTableWidget *table)
{
    SCASSERT(table != nullptr);

    table->setColumnHidden( 1, true );// calculated value
    table->setColumnHidden( 3, true );// in sizes
    table->setColumnHidden( 4, true );// in heights
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief cellChanged cell in table was changed
 * @param row number of row
 * @param column number of column
 */
void DialogIncrements::IncrementChanged ( qint32 row, qint32 column )
{
    qCDebug(vDialog, "Increment changed.");
    const QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    const QTableWidgetItem *item = ui->tableWidgetIncrement->item(row, column);
    const quint32 id = qvariant_cast<quint32>(itemName->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement() == false)
    {
        qCDebug(vDialog, "Cant't find increment with id = %u", id);
        return;
    }
    this->row = row;
    switch (column)
    {
        case 0: // VPattern::IncrementName
            qCDebug(vDialog, "Changed name to %s", item->text().toUtf8().constData());
            doc->SetAttribute(domElement, VPattern::IncrementName, item->text());
            data->ClearVariables(VarType::Increment);
            this->column = 2;
            emit FullUpdateTree(Document::LiteParse);
            break;
        case 2: // VPattern::IncrementBase
            qCDebug(vDialog, "Changed base to %s", item->text().toUtf8().constData());
            doc->SetAttribute(domElement, VPattern::IncrementBase, item->text());
            this->column = 3;
            emit FullUpdateTree(Document::LiteParse);
            break;
        case 3: // VPattern::IncrementKsize
            qCDebug(vDialog, "Changed ksize to %s", item->text().toUtf8().constData());
            doc->SetAttribute(domElement, VPattern::IncrementKsize, item->text());
            this->column = 4;
            emit FullUpdateTree(Document::LiteParse);
            break;
        case 4: // VPattern::IncrementKgrowth
            qCDebug(vDialog, "Changed kheight to %s", item->text().toUtf8().constData());
            doc->SetAttribute(domElement, VPattern::IncrementKgrowth, item->text());
            this->column = 5;
            emit FullUpdateTree(Document::LiteParse);
            break;
        case 5: // VPattern::IncrementDescription
        {
            qCDebug(vDialog, "Changed description to %s", item->text().toUtf8().constData());
            doc->SetAttribute(domElement, VPattern::IncrementDescription, item->text());
            QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(itemName->text());
            incr->SetDescription(item->text());
            ui->tableWidgetIncrement->resizeColumnsToContents();
            ui->tableWidgetIncrement->resizeRowsToContents();
            this->column = 0;
            ui->tableWidgetIncrement->setCurrentCell( row, this->column );
            ui->tableWidgetIncrement->horizontalHeader()->setStretchLastSection(true);
            break;
        }
        default:
            break;
    }
    emit haveLiteChange();
    ShowSuccess();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::MeasurementChanged(qint32 row, qint32 column)
{
    qCDebug(vDialog, "Measurement changed.");
    switch (column)
    {
        case 2:// value column
        {
            const QTableWidgetItem *itemName = ui->tableWidgetMeasurements->item(row, 0);// name column
            QTableWidgetItem *item = ui->tableWidgetMeasurements->item(row, 2);

            QSharedPointer<VMeasurement> measur = data->GetVariable<VMeasurement>(
                                                                         qApp->TrVars()->VarFromUser(itemName->text()));
            const QString tag = measur->TagName();
            QDomNodeList list = m->elementsByTagName(tag);
            QDomElement domElement = list.at(0).toElement();
            if (domElement.isElement() == false)
            {
                qCDebug(vDialog, "Can't find measurement %s", tag.toUtf8().constData());
                return;
            }

            bool ok = false;
            qreal base = item->text().replace(",", ".").toDouble(&ok);
            if (ok == false)
            {
                measur->SetBase(0);
                item->setText("0");
                qCDebug(vDialog, "Can't convert toDouble measurement value");
            }
            else
            {
                measur->SetBase(base);
            }
            qCDebug(vDialog, "Changed value to %f", base);

            // Convert value to measurements table unit
            base = UnitConvertor(base, qApp->patternUnit(), m->MUnit());

            m->SetAttribute(domElement, VIndividualMeasurements::AttrValue, QString("%1").arg(base));
            QString error;
            if (m->SaveDocument(doc->MPath(), error) == false)
            {
                QMessageBox messageBox;
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.setInformativeText(tr("Can't save measurement"));
                messageBox.setDefaultButton(QMessageBox::Ok);
                messageBox.setDetailedText(error);
                messageBox.setStandardButtons(QMessageBox::Ok);
                messageBox.exec();
            }
            else
            {
                ShowSuccess();
            }
            data->ClearVariables();
            m->Measurements();

            emit FullUpdateTree(Document::LiteParse);
            break;
        }
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::closeEvent(QCloseEvent *event)
{
    emit DialogClosed(QDialog::Accepted);
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
DialogIncrements::~DialogIncrements()
{
    delete ui;
    delete m;
}
