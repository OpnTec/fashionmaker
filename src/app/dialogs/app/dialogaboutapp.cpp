/************************************************************************
 **
 **  @file   dialogaboutapp.cpp
 **  @author Patrick Proy <patrick(at)proy.org>
 **  @date   6 5, 2014
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

#include "dialogaboutapp.h"
#include "ui_dialogaboutapp.h"
#include "../../version.h"

DialogAboutApp::DialogAboutApp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutApp)
{
    ui->setupUi(this);

    ui->label_Valentina_Version->setText(QString("Valentina %1").arg(APP_VERSION));
    ui->label_QT_Version->setText(tr("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR));

    QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    ui->label_Valentina_Built->setText(tr("Built on %3 at %4").arg(date.toString()).arg(__TIME__));

    ui->label_Legal_Stuff->setText(WARRANTY);

    ui->pushButton_Web_Site->setText(tr("Web site : %1").arg(VER_COMPANYDOMAIN_STR));
    connect(ui->pushButton_Web_Site, &QPushButton::clicked,
            this, &DialogAboutApp::webButtonClicked );

}

DialogAboutApp::~DialogAboutApp()
{
    delete ui;
}

void DialogAboutApp::webButtonClicked() {
    if ( ! QDesktopServices::openUrl(QUrl(VER_COMPANYDOMAIN_STR))) {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Cannot open your default browser"));
    }

}
