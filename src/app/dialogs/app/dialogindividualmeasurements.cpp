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
#include "../../xml/vindividualmeasurements.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QDesktopWidget>
#include "../../core/vapplication.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
DialogIndividualMeasurements::DialogIndividualMeasurements(VContainer *data, const QString &patternPieceName,
                                                           QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogIndividualMeasurements), _name(patternPieceName), _tablePath(QString()),
    data(data)
{
    ui->setupUi(this);

    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    {
        const QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        SCASSERT(bOk != nullptr);
        connect(bOk, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogAccepted);
    }
    {
        const QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        SCASSERT(bCansel != nullptr);
        connect(bCansel, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogRejected);
    }

    ui->lineEditName->setText(_name);

    InitUnits();
    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogIndividualMeasurements::CheckState);
    connect(ui->buttonGroupPath, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this,
            &DialogIndividualMeasurements::CheckState);
    connect(ui->toolButtonOpenExist, &QToolButton::clicked, this, &DialogIndividualMeasurements::OpenTable);
    connect(ui->toolButtonOpenNew, &QToolButton::clicked, this, &DialogIndividualMeasurements::NewTable);
    this->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
}

//---------------------------------------------------------------------------------------------------------------------
DialogIndividualMeasurements::~DialogIndividualMeasurements()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
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

        //just in case
        VDomDocument::ValidateXML("://schema/individual_measurements.xsd", qApp->pathToTables());
        QFile iMeasur(qApp->pathToTables());
        //TODO maybe make copy save?
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
        Unit unit = VDomDocument::StrToUnits(ui->comboBoxUnits->itemData(index).toString());
        m.setUnit(unit);
        qApp->setPatternUnit( m.MUnit());
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

//---------------------------------------------------------------------------------------------------------------------
void DialogIndividualMeasurements::DialogRejected()
{
    _name.clear();
    _tablePath.clear();
    reject();
}

//---------------------------------------------------------------------------------------------------------------------
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
        ui->labelGivenName->setEnabled(true);
        ui->labelFamilyName->setEnabled(true);
        ui->lineEditPathExistM->setEnabled(true);
        ui->toolButtonOpenExist->setEnabled(true);

        ui->lineEditPathNewM->setEnabled(false);
        ui->toolButtonOpenNew->setEnabled(false);
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
        ui->comboBoxUnits->setEnabled(true);

        ui->labelGivenName->setEnabled(false);
        ui->labelFamilyName->setEnabled(false);
        ui->toolButtonOpenExist->setEnabled(false);
        ui->lineEditPathExistM->setEnabled(false);

        if (ui->lineEditPathNewM->text().isEmpty() == false)
        {
            flagPath = true;
        }
    }


    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagName && flagPath);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIndividualMeasurements::OpenTable()
{
    const QString filter(tr("Individual measurements (*.vit)"));
    QString path = qApp->getSettings()->value("paths/individual_measurements", QDir::homePath()).toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
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
    VIndividualMeasurements m(data);
    m.setContent(fileName);
    ui->labelGivenName->setText(m.GivenName());
    ui->labelFamilyName->setText(m.FamilyName());
    ui->lineEditPathExistM->setText(fileName);
    ui->lineEditPathExistM->setToolTip(fileName);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
void DialogIndividualMeasurements::InitUnits()
{
    ui->comboBoxUnits->addItem(tr("Centimeters"), QVariant(VDomDocument::UnitsToStr(Unit::Cm)));
    ui->comboBoxUnits->addItem(tr("Millimiters"), QVariant(VDomDocument::UnitsToStr(Unit::Mm)));
    ui->comboBoxUnits->addItem(tr("Inches"), QVariant(VDomDocument::UnitsToStr(Unit::Inch)));

    const QString checkedUnit = qApp->getSettings()->value("configuration/unit", "cm").toString();
    // set default unit
    const qint32 indexUnit = ui->comboBoxUnits->findData(checkedUnit);
    if (indexUnit != -1)
    {
        ui->comboBoxUnits->setCurrentIndex(indexUnit);
    }
}
