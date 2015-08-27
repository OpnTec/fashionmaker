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

#include "pathpage.h"
#include "../../options.h"
#include "../../core/vapplication.h"
#include "../vmisc/vsettings.h"
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
#include <QHeaderView>

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
    qApp->ValentinaSettings()->SetPathIndividualMeasurements(pathTable->item(0, 1)->text());
    qApp->ValentinaSettings()->SetPathStandardMeasurements(pathTable->item(1, 1)->text());
    qApp->ValentinaSettings()->SetPathPattern(pathTable->item(2, 1)->text());
    qApp->ValentinaSettings()->SetPathLayout(pathTable->item(3, 1)->text());
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
    const int row = pathTable->currentRow();
    QTableWidgetItem *item = pathTable->item(row, 1);
    SCASSERT(item != nullptr);

    switch (row)
    {
        case 1: // standard measurements
            item->setText(qApp->ValentinaSettings()->StandardTablesPath());
            item->setToolTip(qApp->ValentinaSettings()->StandardTablesPath());
            break;
        case 0: // individual measurements
        case 2: // pattern path
        case 3: // layout path
            item->setText(QDir::homePath());
            item->setToolTip(QDir::homePath());
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PathPage::EditPath()
{
    const int row = pathTable->currentRow();
    QTableWidgetItem *item = pathTable->item(row, 1);
    SCASSERT(item != nullptr);

    QString path;
    switch (row)
    {
        case 0: // individual measurements
            path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();
            break;
        case 1: // standard measurements
            path = qApp->ValentinaSettings()->GetPathStandardMeasurements();
            break;
        case 2: // pattern path
            path = qApp->ValentinaSettings()->GetPathPattern();
            break;
        case 3: // layout path
            path = qApp->ValentinaSettings()->GetPathLayout();
            break;
        default:
            break;
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
    {
        DefaultPath();
        return;
    }

    item->setText(dir);
    item->setToolTip(dir);
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

//---------------------------------------------------------------------------------------------------------------------
void PathPage::InitTable()
{
    pathTable = new QTableWidget();
    pathTable->setRowCount(4);
    pathTable->setColumnCount(2);
    pathTable->verticalHeader()->setVisible(false);
    pathTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pathTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pathTable->setSelectionMode(QAbstractItemView::SingleSelection);
    pathTable->setShowGrid(false);

    QStringList tableHeader = QStringList() << tr("Type") << tr("Path");
    pathTable->setHorizontalHeaderLabels(tableHeader);

    {
        pathTable->setItem(0, 0, new QTableWidgetItem(tr("Individual measurements")));
        QTableWidgetItem *item = new QTableWidgetItem(qApp->ValentinaSettings()->GetPathIndividualMeasurements());
        item->setToolTip(qApp->ValentinaSettings()->GetPathIndividualMeasurements());
        pathTable->setItem(0, 1, item);
    }

    {
        pathTable->setItem(1, 0, new QTableWidgetItem(tr("Standard measurements")));
        QTableWidgetItem *item = new QTableWidgetItem(qApp->ValentinaSettings()->GetPathStandardMeasurements());
        item->setToolTip(qApp->ValentinaSettings()->GetPathStandardMeasurements());
        pathTable->setItem(1, 1, item);
    }

    {
        pathTable->setItem(2, 0, new QTableWidgetItem(tr("Patterns")));
        QTableWidgetItem *item = new QTableWidgetItem(qApp->ValentinaSettings()->GetPathPattern());
        item->setToolTip(qApp->ValentinaSettings()->GetPathPattern());
        pathTable->setItem(2, 1, item);
    }

    {
        pathTable->setItem(3, 0, new QTableWidgetItem(tr("Layout")));
        QTableWidgetItem *item = new QTableWidgetItem(qApp->ValentinaSettings()->GetPathLayout());
        item->setToolTip(qApp->ValentinaSettings()->GetPathLayout());
        pathTable->setItem(3, 1, item);
    }

    pathTable->verticalHeader()->setDefaultSectionSize(20);
    pathTable->resizeColumnsToContents();
    pathTable->resizeRowsToContents();
    pathTable->horizontalHeader()->setStretchLastSection(true);

    connect(pathTable, &QTableWidget::itemSelectionChanged, this, &PathPage::TableActivated);
}
