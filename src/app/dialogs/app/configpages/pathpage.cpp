/************************************************************************
 **
 **  @file   pathpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "pathpage.h"
#include "../../../options.h"
#include "../../../widgets/vapplication.h"
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>
#include <QIcon>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QFileDialog>

//---------------------------------------------------------------------------------------------------------------------
PathPage::PathPage(QWidget *parent)
    : QWidget(parent), defaultButton(nullptr), editButton(nullptr), pathTable(nullptr)
{
    QGroupBox *pathGroup = PathGroup();
    SCASSERT(pathGroup != nullptr);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pathGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void PathPage::Apply()
{
    qApp->getSettings()->setValue("paths/individual_measurements", pathTable->item(0, 1)->text());
    qApp->getSettings()->setValue("paths/pattern", pathTable->item(1, 1)->text());
}

//---------------------------------------------------------------------------------------------------------------------
void PathPage::TableActivated()
{
    defaultButton->setEnabled(true);
    defaultButton->setDefault(false);

    editButton->setEnabled(true);
    editButton->setDefault(true);
}

//---------------------------------------------------------------------------------------------------------------------
void PathPage::DefaultPath()
{
    QTableWidgetItem *item = pathTable->item(pathTable->currentRow(), 1);
    SCASSERT(item != nullptr);
    item->setText(QDir::homePath());
}

//---------------------------------------------------------------------------------------------------------------------
void PathPage::EditPath()
{
    QTableWidgetItem *item = pathTable->item(pathTable->currentRow(), 1);
    SCASSERT(item != nullptr);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
    {
        dir = QDir::homePath();
    }

    item->setText(dir);
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PathPage::PathGroup()
{
    QGroupBox *pathGroup = new QGroupBox(tr("Path that use Valentina"));
    InitTable();

    defaultButton = new QPushButton(tr("Default"));
    defaultButton->setEnabled(false);
    connect(defaultButton, &QPushButton::clicked, this, &PathPage::DefaultPath);

    editButton = new QPushButton(tr("Edit"));
    editButton->setEnabled(false);
    connect(editButton, &QPushButton::clicked, this, &PathPage::EditPath);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(defaultButton);
    buttonsLayout->addWidget(editButton);

    QVBoxLayout *pathLayout = new QVBoxLayout;
    pathLayout->addWidget(pathTable);
    pathLayout->addLayout(buttonsLayout);

    pathGroup->setLayout(pathLayout);
    return pathGroup;
}

void PathPage::InitTable()
{
    pathTable = new QTableWidget();
    pathTable->setRowCount(2);
    pathTable->setColumnCount(2);
    pathTable->verticalHeader()->setVisible(false);
    pathTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pathTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pathTable->setSelectionMode(QAbstractItemView::SingleSelection);
    pathTable->setShowGrid(false);

    QStringList tableHeader{tr("Type"), tr("Path")};
    pathTable->setHorizontalHeaderLabels(tableHeader);

    QString path;

    pathTable->setItem(0, 0, new QTableWidgetItem(tr("Individual measurements")));
    path = qApp->getSettings()->value("paths/individual_measurements", QDir::homePath()).toString();
    pathTable->setItem(0, 1, new QTableWidgetItem(path));

    pathTable->setItem(1, 0, new QTableWidgetItem(tr("Patterns")));
    path = qApp->getSettings()->value("paths/pattern", QDir::homePath()).toString();
    pathTable->setItem(1, 1, new QTableWidgetItem(path));

    pathTable->verticalHeader()->setDefaultSectionSize(20);
    pathTable->resizeColumnsToContents();
    pathTable->resizeRowsToContents();
    pathTable->horizontalHeader()->setStretchLastSection(true);

    connect(pathTable, &QTableWidget::itemSelectionChanged, this, &PathPage::TableActivated);
}
