/************************************************************************
 **
 **  @file   dialogstandardmeasurements.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   21 2, 2014
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

#include "dialogstandardmeasurements.h"
#include "ui_dialogstandardmeasurements.h"
#include <QDir>

DialogStandardMeasurements::DialogStandardMeasurements(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogStandardMeasurements), _name(QString()), _tablePath(QString())
{
    ui->setupUi(this);

    {
        const QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        Q_CHECK_PTR(bOk);
        connect(bOk, &QPushButton::clicked, this, &DialogStandardMeasurements::DialogAccepted);
    }
    {
        const QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        Q_CHECK_PTR(bCansel);
        connect(bCansel, &QPushButton::clicked, this, &DialogStandardMeasurements::DialogRejected);
    }

    LoadStandardTables();

    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogStandardMeasurements::CheckState);
}

DialogStandardMeasurements::~DialogStandardMeasurements()
{
    delete ui;
}

QString DialogStandardMeasurements::name() const
{
    return _name;
}

QString DialogStandardMeasurements::tablePath() const
{
    return _tablePath;
}

void DialogStandardMeasurements::DialogAccepted()
{
    _name = ui->lineEditName->text();
    _tablePath = "path";
    accept();
}

void DialogStandardMeasurements::DialogRejected()
{
    _name = "";
    _tablePath = "";
    reject();
}

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
        Q_CHECK_PTR(box);
        if (box->count() > 0 && box->currentIndex() != -1)
        {
            flagTable = true;
        }
    }

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagTable && flagName);
}

void DialogStandardMeasurements::LoadStandardTables()
{
#ifdef Q_OS_WIN
    const QString pathToTables = QString("/standard_tables");
#else
    #ifdef QT_DEBUG
        const QString pathToTables = QString("/standard_tables");
    #else
        const QString pathToTables = QString("/usr/share/valentina/standard_tables");
    #endif
#endif
    QDir tablesDir(pathToTables);
    {
        QStringList filters;
        filters << "*.cpp";
        tablesDir.setNameFilters(filters);
    }
    const QStringList allFiles = tablesDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    if (allFiles.isEmpty() == true)
    {
        ui->comboBoxTables->clear();
        CheckState();
        return;
    }

    for (int i = 0; i < allFiles.size(); ++i)
    {

    }
}
