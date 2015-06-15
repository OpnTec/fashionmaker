/************************************************************************
 **
 **  @file   dialogstandardmeasurements.cpp
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

#include "dialogstandardmeasurements.h"
#include "ui_dialogstandardmeasurements.h"
#include "../../xml/vstandardmeasurements.h"
#include "../../core/vapplication.h"
#include "../../core/vsettings.h"
#include "../../libs/vpatterndb/vcontainer.h"
#include "../vmisc/logging.h"

#include <QDir>
#include <QPushButton>
#include <QDesktopWidget>

Q_LOGGING_CATEGORY(vStMeasur, "v.st.measurements")

//---------------------------------------------------------------------------------------------------------------------
DialogStandardMeasurements::DialogStandardMeasurements(VContainer *data, const QString &patternPieceName,
                                                       QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogStandardMeasurements), data(data), _name(patternPieceName), _tablePath(QString())
{
    ui->setupUi(this);

    qApp->getSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    {
        const QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        SCASSERT(bOk != nullptr);
        connect(bOk, &QPushButton::clicked, this, &DialogStandardMeasurements::DialogAccepted);
    }
    {
        const QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        SCASSERT(bCansel != nullptr);
        connect(bCansel, &QPushButton::clicked, this, &DialogStandardMeasurements::DialogRejected);
    }

    qCDebug(vStMeasur, "Pattern piece name %s", _name.toUtf8().constData());
    ui->lineEditName->setText(_name);

    LoadStandardTables();

    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogStandardMeasurements::CheckState);
}

//---------------------------------------------------------------------------------------------------------------------
DialogStandardMeasurements::~DialogStandardMeasurements()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogStandardMeasurements::name() const
{
    return _name;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogStandardMeasurements::tablePath() const
{
    return _tablePath;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogStandardMeasurements::DialogAccepted()
{
    _name = ui->lineEditName->text();
    const qint32 index = ui->comboBoxTables->currentIndex();
    _tablePath = ui->comboBoxTables->itemData(index).toString();

    try
    {
        VDomDocument::ValidateXML("://schema/standard_measurements.xsd", _tablePath);
        VStandardMeasurements m(data);
        m.setXMLContent(_tablePath);
        qApp->setPatternUnit(m.MUnit());
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("File error."), this);
        qDebug()<<"File error."<<e.ErrorMessage()<<e.DetailedInformation()<<Q_FUNC_INFO;
        return;
    }

    accept();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogStandardMeasurements::DialogRejected()
{
    _name.clear();
    _tablePath.clear();
    reject();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogStandardMeasurements::CheckState()
{
    bool flagName = false;
    if (ui->lineEditName->text().isEmpty() == false)
    {
        flagName = true;
    }

    bool flagTable = false;
    {
        const QComboBox *box = ui->comboBoxTables;
        SCASSERT(box != nullptr);
        if (box->count() > 0 && box->currentIndex() != -1)
        {
            flagTable = true;
        }
    }

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagTable && flagName);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogStandardMeasurements::LoadStandardTables()
{
    qCDebug(vStMeasur, "Loading standard table.");
    QStringList filters{"*.vst"};
    //Use standard path to standard measurements
    const QString path = qApp->getSettings()->GetPathStandardMeasurements();
    QDir tablesDir(path);
    tablesDir.setNameFilters(filters);
    tablesDir.setCurrent(path);

    const QStringList allFiles = tablesDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    if (allFiles.isEmpty() == true)
    {
        qCDebug(vStMeasur, "Can't find standard measurements in path %s", path.toUtf8().constData());
        ui->comboBoxTables->clear();
        CheckState();
        return;
    }

    qCDebug(vStMeasur, "Was found %d tables.", allFiles.size());
    for (int i = 0; i < allFiles.size(); ++i)
    {
        QFileInfo fi(allFiles.at(i));
        try
        {
            VDomDocument::ValidateXML("://schema/standard_measurements.xsd", fi.absoluteFilePath());
            VStandardMeasurements m(data);
            m.setXMLContent(fi.absoluteFilePath());
            if (m.MUnit() == Unit::Inch)
            {
                qCWarning(vStMeasur, "We do not support inches for standard table. Ignore table %s .",
                          fi.absoluteFilePath().toUtf8().constData());
            }
            else
            {
                const QString desc = m.TrDescription();
                if (desc.isEmpty() == false)
                {
                    qCDebug(vStMeasur, "Adding table from %s", fi.absoluteFilePath().toUtf8().constData());
                    ui->comboBoxTables->addItem(desc, QVariant(fi.absoluteFilePath()));
                }
            }
        }
        catch (VException &e)
        {
            qCDebug(vStMeasur, "File error. %s %s", e.ErrorMessage().toUtf8().constData(),
                    e.DetailedInformation().toUtf8().constData());
            continue;
        }
    }
}
