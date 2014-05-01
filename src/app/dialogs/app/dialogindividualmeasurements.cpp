/************************************************************************
 **
 **  @file   dialogindividualmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 2, 2014
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

#include "dialogindividualmeasurements.h"
#include "ui_dialogindividualmeasurements.h"
#include <QButtonGroup>
#include "../../xml/vindividualmeasurements.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include "../../widgets/vapplication.h"
#include <QPushButton>

DialogIndividualMeasurements::DialogIndividualMeasurements(VContainer *data, const QString &patternPieceName,
                                                           QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogIndividualMeasurements), _name(patternPieceName), _tablePath(QString()),
    data(data)
{
    ui->setupUi(this);

    {
        const QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        Q_CHECK_PTR(bOk);
        connect(bOk, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogAccepted);
    }
    {
        const QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        Q_CHECK_PTR(bCansel);
        connect(bCansel, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogRejected);
    }

    ui->lineEditName->setText(_name);

    LoadIndividualTables();
    InitUnits();

    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogIndividualMeasurements::CheckState);
    connect(ui->buttonGroupPath, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this,
            &DialogIndividualMeasurements::CheckState);
    connect(ui->toolButtonOpenExist, &QToolButton::clicked, this, &DialogIndividualMeasurements::OpenTable);
    connect(ui->toolButtonOpenNew, &QToolButton::clicked, this, &DialogIndividualMeasurements::NewTable);
}

DialogIndividualMeasurements::~DialogIndividualMeasurements()
{
    delete ui;
}

void DialogIndividualMeasurements::DialogAccepted()
{
    _name = ui->lineEditName->text();
    if (ui->radioButtonExistM->isChecked())
    {
        _tablePath = ui->lineEditPathExistM->text();
    }
    else
    {
        _tablePath = ui->lineEditPathNewM->text();
        QFile table(_tablePath);
        if (table.exists())
        {
            table.remove();
        }

        const qint32 index = ui->comboBoxLang->currentIndex();
        QString path = ui->comboBoxLang->itemData(index).toString();
        QFile iMeasur(path);
        if ( iMeasur.copy(_tablePath) == false )
        {
            QMessageBox::warning(this, tr("Could not create measurements file"), tr("Please try again or change file"));
            DialogRejected();
        }
    }
    try
    {
        VDomDocument::ValidateXML("://schema/individual_measurements.xsd", _tablePath);
        VIndividualMeasurements m(data);
        m.setContent(_tablePath);
        const qint32 index = ui->comboBoxUnits->currentIndex();
        Valentina::Units unit = VDomDocument::StrToUnits(ui->comboBoxUnits->itemData(index).toString());
        m.setUnit(unit);
        qApp->setPatternUnit( m.Unit());
        QFile iMeasur(_tablePath);
        if (iMeasur.open(QIODevice::WriteOnly| QIODevice::Truncate))
        {
            const int indent = 4;
            QTextStream out(&iMeasur);
            out.setCodec("UTF-8");
            m.save(out, indent);
            iMeasur.close();
        }
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("File error."), this);
        qDebug()<<"File error."<<e.ErrorMessage()<<e.DetailedInformation()<<Q_FUNC_INFO;
        DialogRejected();
    }
    accept();
}

void DialogIndividualMeasurements::DialogRejected()
{
    _name.clear();
    _tablePath.clear();
    reject();
}

void DialogIndividualMeasurements::CheckState()
{
    bool flagName = false;
    if (ui->lineEditName->text().isEmpty() == false)
    {
        flagName = true;
    }

    bool flagPath = false;
    if (ui->radioButtonExistM->isChecked())
    {
        ui->lineEditPathExistM->setEnabled(true);
        ui->toolButtonOpenExist->setEnabled(true);

        ui->lineEditPathNewM->setEnabled(false);
        ui->toolButtonOpenNew->setEnabled(false);
        ui->comboBoxLang->setEnabled(false);
        ui->comboBoxUnits->setEnabled(false);

        if (ui->lineEditPathExistM->text().isEmpty() == false)
        {
            flagPath = true;
        }
    }
    else
    {
        ui->lineEditPathNewM->setEnabled(true);
        ui->toolButtonOpenNew->setEnabled(true);
        ui->comboBoxLang->setEnabled(true);
        ui->comboBoxUnits->setEnabled(true);

        ui->toolButtonOpenExist->setEnabled(false);
        ui->lineEditPathExistM->setEnabled(false);

        if (ui->lineEditPathNewM->text().isEmpty() == false)
        {
            flagPath = true;
        }
    }

    bool flagLang = false;
    {
        const QComboBox *box = ui->comboBoxLang;
        Q_CHECK_PTR(box);
        if (box->count() > 0 && box->currentIndex() != -1)
        {
            flagLang = true;
        }
    }

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagName && flagPath && flagLang);
}

void DialogIndividualMeasurements::LoadIndividualTables()
{
    QStringList filters;
    filters << "*.vit";
    QDir tablesDir(qApp->pathToTables());
    tablesDir.setNameFilters(filters);
    tablesDir.setCurrent(qApp->pathToTables());

    const QStringList allFiles = tablesDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    if (allFiles.isEmpty() == true)
    {
        ui->comboBoxLang->clear();
        CheckState();
        return;
    }

    for (int i = 0; i < allFiles.size(); ++i)
    {
        QFileInfo fi(allFiles.at(i));
        try
        {
            VDomDocument::ValidateXML("://schema/individual_measurements.xsd", fi.absoluteFilePath());
            VIndividualMeasurements m(data);
            m.setContent(fi.absoluteFilePath());
            const QString lang = QLocale(m.Language()).nativeLanguageName();
            ui->comboBoxLang->addItem(lang, QVariant(fi.absoluteFilePath()));
        }
        catch (VException &e)
        {
            qDebug()<<"File error."<<e.ErrorMessage()<<e.DetailedInformation()<<Q_FUNC_INFO;
            continue;
        }
    }
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QString defaultLocale = QLocale::system().name();       // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
    QString checkedLocale = settings.value("configuration/locale", defaultLocale).toString();

    //TODO make sure this part work.
    // set default translators and language checked
    qint32 index = ui->comboBoxLang->findData(checkedLocale);
    if (index != -1)
    {
        ui->comboBoxLang->setCurrentIndex(index);
    }
}

void DialogIndividualMeasurements::OpenTable()
{
    const QString filter(tr("Individual measurements (*.vit)"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);
    if (fileName.isEmpty())
    {
        return;
    }

    try
    {
        VDomDocument::ValidateXML("://schema/individual_measurements.xsd", fileName);
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("File error."), this);
        fileName.clear();
    }
    ui->lineEditPathExistM->setText(fileName);
    ui->lineEditPathExistM->setToolTip(fileName);
    CheckState();
}

void DialogIndividualMeasurements::NewTable()
{
    QString dir = QDir::homePath()+"/measurements.vit";
    QString name = QFileDialog::getSaveFileName(this, tr("Where save measurements?"), dir,
                                                tr("Individual measurements (*.vit)"));

    if (name.isEmpty())
    {
        return;
    }

    // what if the users did not specify a suffix...?
    QFileInfo f( name );
    if (f.suffix().isEmpty() && f.suffix() != "vit")
    {
        name += ".vit";
    }
    ui->lineEditPathNewM->setText(name);
    ui->lineEditPathNewM->setToolTip(name);
    CheckState();
}

void DialogIndividualMeasurements::InitUnits()
{
    ui->comboBoxUnits->addItem(tr("centimeter"), QVariant(VDomDocument::UnitsToStr(Valentina::Cm)));
    ui->comboBoxUnits->addItem(tr("inch"), QVariant(VDomDocument::UnitsToStr(Valentina::Inch)));
}
