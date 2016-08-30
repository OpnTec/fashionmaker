/************************************************************************
 **
 **  @file   configdialog.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 2, 2014
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

#include "configdialog.h"
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QCloseEvent>
#include "../core/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent), contentsWidget(nullptr), pagesWidget(nullptr), configurationPage(nullptr), patternPage(nullptr),
    communityPage(nullptr), pathPage(nullptr), applyButton(nullptr), cancelButton(nullptr), okButton(nullptr),
    isInitialized(false)
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setMinimumWidth(128);
    contentsWidget->setMinimumHeight(500);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;

    configurationPage = new ConfigurationPage();
    pagesWidget->addWidget(configurationPage);

    patternPage = new PatternPage();
    pagesWidget->addWidget(patternPage);

    communityPage = new CommunityPage();
    pagesWidget->addWidget(communityPage);

    pathPage = new PathPage();
    pagesWidget->addWidget(pathPage);

    applyButton = new QPushButton(tr("Apply"));
    cancelButton = new QPushButton(tr("&Cancel"));
    okButton = new QPushButton(tr("&Ok"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(cancelButton, &QPushButton::clicked, this, &ConfigDialog::close);
    connect(applyButton, &QPushButton::clicked, this, &ConfigDialog::Apply);
    connect(okButton, &QPushButton::clicked, this, &ConfigDialog::Ok);

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
    mainLayout->setStretch(0, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::closeEvent(QCloseEvent *event)
{
    if (result() == QDialog::Accepted)
    {
        done(QDialog::Accepted);
    }
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::changeEvent(QEvent *event)
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
void ConfigDialog::showEvent(QShowEvent *event)
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
void ConfigDialog::resizeEvent(QResizeEvent *event)
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
void ConfigDialog::createIcons()
{
    createIcon("://icon/config.png", tr("Configuration"));
    createIcon("://icon/pattern_config.png", tr("Pattern"));
    createIcon("://icon/community_config.png", tr("Community"));
    createIcon("://icon/path_config.png", tr("Paths"));

    connect(contentsWidget, &QListWidget::currentItemChanged,
            [this](QListWidgetItem *current, QListWidgetItem *previous)
    {
        if (current == nullptr)
        {
            current = previous;
        }
        pagesWidget->setCurrentIndex(contentsWidget->row(current));
    });
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::createIcon(const QString &icon, const QString &text)
{
    QListWidgetItem *button = new QListWidgetItem(contentsWidget);
    button->setIcon(QIcon(icon));
    button->setText(text);
    button->setTextAlignment(Qt::AlignHCenter);
    button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::Apply()
{
    configurationPage->Apply();
    patternPage->Apply();
    communityPage->Apply();
    pathPage->Apply();

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());
    emit UpdateProperties();
    setResult(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::Ok()
{
    Apply();
    done(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigDialog::RetranslateUi()
{
    applyButton->setText(tr("Apply"));
    cancelButton->setText(tr("&Cancel"));
    okButton->setText(tr("&Ok"));
    setWindowTitle(tr("Config Dialog"));
    contentsWidget->item(0)->setText(tr("Configuration"));
    contentsWidget->item(1)->setText(tr("Pattern"));
    contentsWidget->item(2)->setText(tr("Community"));
    contentsWidget->item(3)->setText(tr("Paths"));
}

