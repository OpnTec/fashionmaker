/************************************************************************
 **
 **  @file   dialogabouttape.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "dialogabouttape.h"
#include "ui_dialogabouttape.h"
#include "../version.h"
#include "../vmisc/def.h"
#include "../fervor/fvupdater.h"

#include <QDate>
#include <QDesktopServices>
#include <QMessageBox>
#include <QShowEvent>
#include <QUrl>
#include <QtDebug>

//---------------------------------------------------------------------------------------------------------------------
DialogAboutTape::DialogAboutTape(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::DialogAboutTape),
      isInitialized(false)
{
    ui->setupUi(this);

    //mApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());

    RetranslateUi();
    connect(ui->pushButton_Web_Site, &QPushButton::clicked, RECEIVER(this)[this]()
    {
        if ( QDesktopServices::openUrl(QUrl(VER_COMPANYDOMAIN_STR)) == false)
        {
            qWarning() << tr("Cannot open your default browser");
        }
    });
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogAboutTape::close);
    connect(ui->pushButtonCheckUpdate, &QPushButton::clicked, []()
    {
        FvUpdater::sharedUpdater()->CheckForUpdatesNotSilent();
    });

    // By default on Windows font point size 8 points we need 11 like on Linux.
    FontPointSize(ui->label_Legal_Stuff, 11);
    FontPointSize(ui->label_Tape_Built, 11);
    FontPointSize(ui->label_QT_Version, 11);
}

//---------------------------------------------------------------------------------------------------------------------
DialogAboutTape::~DialogAboutTape()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAboutTape::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        RetranslateUi();
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAboutTape::showEvent(QShowEvent *event)
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
void DialogAboutTape::FontPointSize(QWidget *w, int pointSize)
{
    SCASSERT(w != nullptr)

    QFont font = w->font();
    font.setPointSize(pointSize);
    w->setFont(font);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAboutTape::RetranslateUi()
{
    ui->label_Tape_Version->setText(QString("Tape %1").arg(APP_VERSION_STR));
    ui->labelBuildRevision->setText(tr("Build revision: %1").arg(BUILD_REVISION));
    ui->label_QT_Version->setText(buildCompatibilityString());

    const QDate date = QLocale::c().toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    ui->label_Tape_Built->setText(tr("Built on %1 at %2").arg(date.toString()).arg(__TIME__));

    ui->label_Legal_Stuff->setText(QApplication::translate("InternalStrings",
                                                           "The program is provided AS IS with NO WARRANTY OF ANY "
                                                           "KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY "
                                                           "AND FITNESS FOR A PARTICULAR PURPOSE."));

    ui->pushButton_Web_Site->setText(tr("Web site : %1").arg(VER_COMPANYDOMAIN_STR));
}
