/************************************************************************
 **
 **  @file   configdialog.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   12 2, 2014
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

#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent), contentsWidget(nullptr), pagesWidget(nullptr), configurationPage(nullptr), patternPage(nullptr)
{
    contentsWidget = new QListWidget;
    Q_CHECK_PTR(contentsWidget);
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setMinimumHeight(250);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    Q_CHECK_PTR(pagesWidget);
    configurationPage = new ConfigurationPage();
    Q_CHECK_PTR(configurationPage);
    pagesWidget->addWidget(configurationPage);
    patternPage = new PatternPage();
    Q_CHECK_PTR(patternPage);
    pagesWidget->addWidget(patternPage);

    QPushButton *applyButton = new QPushButton(tr("Apply"));
    Q_CHECK_PTR(applyButton);
    QPushButton *canselButton = new QPushButton(tr("&Cancel"));
    Q_CHECK_PTR(canselButton);
    QPushButton *okButton = new QPushButton(tr("&Ok"));
    Q_CHECK_PTR(okButton);

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(canselButton, &QPushButton::clicked, this, &ConfigDialog::close);
    connect(applyButton, &QPushButton::clicked, this, &ConfigDialog::Apply);
    connect(okButton, &QPushButton::clicked, this, &ConfigDialog::Ok);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    Q_CHECK_PTR(horizontalLayout);
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    Q_CHECK_PTR(buttonsLayout);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(canselButton);
    buttonsLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    Q_CHECK_PTR(mainLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == false)
    {
        current = previous;
    }
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    if (result() == QDialog::Accepted)
    {
        done(QDialog::Accepted);
    }
    event->accept();
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    Q_CHECK_PTR(configButton);
    configButton->setIcon(QIcon("://icon/config.png"));
    configButton->setText(tr("Configuration"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *patternButton = new QListWidgetItem(contentsWidget);
    Q_CHECK_PTR(patternButton);
    patternButton->setIcon(QIcon("://icon/pattern_config.png"));
    patternButton->setText(tr("Pattern"));
    patternButton->setTextAlignment(Qt::AlignHCenter);
    patternButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget, &QListWidget::currentItemChanged, this, &ConfigDialog::changePage);
}

void ConfigDialog::Apply()
{
    switch (contentsWidget->currentRow())
    {
        case (0):
            configurationPage->Apply();
            break;
        case (1):
            patternPage->Apply();
            break;
        default:
            break;
    }
    setResult(QDialog::Accepted);
}

void ConfigDialog::Ok()
{
    Apply();
    done(QDialog::Accepted);
}
