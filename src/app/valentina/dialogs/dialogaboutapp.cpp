/************************************************************************
 **
 **  @file   dialogaboutapp.cpp
 **  @author Patrick Proy <patrick(at)proy.org>
 **  @date   6 5, 2014
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

#include "dialogaboutapp.h"
#include "ui_dialogaboutapp.h"
#include "../version.h"
#include <QDate>
#include <QDesktopServices>
#include <QMessageBox>
#include <QtDebug>
#include "../core/vapplication.h"
#include "../fervor/fvupdater.h"

//---------------------------------------------------------------------------------------------------------------------
DialogAboutApp::DialogAboutApp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutApp),
    isInitialized(false)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    ui->label_Valentina_Version->setText(QString("Valentina %1").arg(APP_VERSION_STR));
    ui->labelBuildRevision->setText(QString("Build revision: %1").arg(BUILD_REVISION));
    ui->label_QT_Version->setText(buildCompatibilityString());

    QDate date = QLocale::c().toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    ui->label_Valentina_Built->setText(tr("Built on %1 at %2").arg(date.toString(), __TIME__));

    ui->label_Legal_Stuff->setText(QApplication::translate("InternalStrings",
                                                           "The program is provided AS IS with NO WARRANTY OF ANY "
                                                           "KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY "
                                                           "AND FITNESS FOR A PARTICULAR PURPOSE."));


    ui->pushButton_Web_Site->setText(tr("Web site : %1").arg(VER_COMPANYDOMAIN_STR));
    connect(ui->pushButton_Web_Site, &QPushButton::clicked, this, []()
    {
        if ( QDesktopServices::openUrl(QUrl(VER_COMPANYDOMAIN_STR)) == false)
        {
            qWarning() << tr("Cannot open your default browser");
        }
    });

    connect(ui->pushButtonCheckUpdate, &QPushButton::clicked, []()
    {
        // Set feed URL before doing anything else
        FvUpdater::sharedUpdater()->SetFeedURL(FvUpdater::CurrentFeedURL());
        FvUpdater::sharedUpdater()->CheckForUpdatesNotSilent();
    });

    // By default on Windows font point size 8 points we need 11 like on Linux.
    FontPointSize(ui->label_Legal_Stuff, 11);
    FontPointSize(ui->label_contrib_label, 11);
    FontPointSize(ui->label_Valentina_Built, 11);
    FontPointSize(ui->label_QT_Version, 11);
}

//---------------------------------------------------------------------------------------------------------------------
DialogAboutApp::~DialogAboutApp()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAboutApp::showEvent(QShowEvent *event)
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
void DialogAboutApp::FontPointSize(QWidget *w, int pointSize)
{
    SCASSERT(w != nullptr)

    QFont font = w->font();
    font.setPointSize(pointSize);
    w->setFont(font);
}
