/************************************************************************
 **
 **  @file   tapeconfigdialog.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   02 08, 2015
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

#include "tapeconfigdialog.h"
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QCloseEvent>
#include "../mapplication.h"

//---------------------------------------------------------------------------------------------------------------------
TapeConfigDialog::TapeConfigDialog(QWidget *parent)
    :QDialog(parent),
      contentsWidget(nullptr),
      pagesWidget(nullptr),
      configurationPage(nullptr),
      pathPage(nullptr),
      applyButton(nullptr),
      cancelButton(nullptr),
      okButton(nullptr),
      isInitialized(false)
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(130);
    contentsWidget->setMinimumWidth(130);
    contentsWidget->setMinimumHeight(260);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    pagesWidget->setMinimumWidth(550);

    configurationPage = new TapeConfigurationPage();
    pagesWidget->addWidget(configurationPage);

    pathPage = new TapePathPage();
    pagesWidget->addWidget(pathPage);

    applyButton = new QPushButton(tr("Apply"));
    cancelButton = new QPushButton(tr("&Cancel"));
    okButton = new QPushButton(tr("&Ok"));

    createIcons();
    connect(contentsWidget, &QListWidget::currentItemChanged,
            [this](QListWidgetItem *current, QListWidgetItem *previous)
    {
        if (current == nullptr)
        {
            current = previous;
        }
        pagesWidget->setCurrentIndex(contentsWidget->row(current));
    });
    contentsWidget->setCurrentRow(0);

    connect(cancelButton, &QPushButton::clicked, this, &TapeConfigDialog::close);
    connect(applyButton, &QPushButton::clicked, this, &TapeConfigDialog::Apply);
    connect(okButton, &QPushButton::clicked, this, &TapeConfigDialog::Ok);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    mainLayout->setStretch(0, 1);

    setWindowTitle(tr("Config Dialog"));

    qApp->TapeSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::closeEvent(QCloseEvent *event)
{
    if (result() == QDialog::Accepted)
    {
        done(QDialog::Accepted);
    }
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        RetranslateUi();
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::showEvent(QShowEvent *event)
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

    setMinimumSize(size());

    QSize sz = qApp->Settings()->GetPreferenceDialogSize();
    if (sz.isEmpty() == false)
    {
        resize(sz);
    }

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    // remember the size for the next time this dialog is opened, but only
    // if widget was already initialized, which rules out the resize at
    // dialog creating, which would
    if (isInitialized == true)
    {
        qApp->Settings()->SetPreferenceDialogSize(size());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::createIcons()
{
    createIcon("://icon/config.png", tr("Configuration"));
    createIcon("://icon/path_config.png", tr("Paths"));
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::createIcon(const QString &icon, const QString &text)
{
    QListWidgetItem *button = new QListWidgetItem(contentsWidget);
    button->setIcon(QIcon(icon));
    button->setText(text);
    button->setTextAlignment(Qt::AlignHCenter);
    button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::Apply()
{
    switch (contentsWidget->currentRow())
    {
        case (0):
            configurationPage->Apply();
            break;
        case (1):
            pathPage->Apply();
            break;
        default:
            break;
    }
    qApp->TapeSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());
    emit UpdateProperties();
    setResult(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::Ok()
{
    Apply();
    done(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigDialog::RetranslateUi()
{
    applyButton->setText(tr("Apply"));
    cancelButton->setText(tr("&Cancel"));
    okButton->setText(tr("&Ok"));
    setWindowTitle(tr("Config Dialog"));
    contentsWidget->item(0)->setText(tr("Configuration"));
    contentsWidget->item(1)->setText(tr("Paths"));
}
