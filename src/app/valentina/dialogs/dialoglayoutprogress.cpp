/************************************************************************
 **
 **  @file   dialoglayoutprogress.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 1, 2015
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

#include "dialoglayoutprogress.h"
#include "ui_dialoglayoutprogress.h"
#include "../options.h"
#include "../core/vapplication.h"

#include <QMessageBox>
#include <QPushButton>
#include <QMovie>
#include <QtDebug>

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutProgress::DialogLayoutProgress(int count, QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogLayoutProgress), maxCount(count), movie(nullptr), isInitialized(false)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    ui->progressBar->setMaximum(maxCount);
    ui->progressBar->setValue(0);

    ui->labelMessage->setText(tr("Arranged workpieces: %1 from %2").arg(0).arg(count));

    movie = new QMovie("://icon/16x16/progress.gif");
    ui->labelProgress->setMovie (movie);
    movie->start ();

    QPushButton *bCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCancel != nullptr)
    connect(bCancel, &QPushButton::clicked, RECEIVER(this)[this](){emit Abort();});
    setModal(true);

    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLayoutProgress::~DialogLayoutProgress()
{
    delete ui;
    delete movie;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutProgress::Start()
{
    show();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutProgress::Arranged(int count)
{
    ui->progressBar->setValue(count);
    ui->labelMessage->setText(tr("Arranged workpieces: %1 from %2").arg(count).arg(maxCount));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutProgress::Error(const LayoutErrors &state)
{
    switch (state)
    {
        case LayoutErrors::NoError:
            return;
        case LayoutErrors::PrepareLayoutError:
            qCritical() << tr("Couldn't prepare data for creation layout");
            break;
        case LayoutErrors::EmptyPaperError:
            qCritical() << tr("Several workpieces left not arranged, but none of them match for paper");
            break;
        case LayoutErrors::ProcessStoped:
        default:
            break;
    }

    done(QDialog::Rejected);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutProgress::Finished()
{
    done(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLayoutProgress::showEvent(QShowEvent *event)
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
