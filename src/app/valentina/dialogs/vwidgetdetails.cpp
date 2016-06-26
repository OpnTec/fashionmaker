/************************************************************************
 **
 **  @file   vwidgetdetails.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vwidgetdetails.h"
#include "ui_vwidgetdetails.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/vcontainer.h"
#include "../vmisc/vabstractapplication.h"
#include "../vtools/undocommands/toggledetailinlayout.h"

#include <QUndoStack>

//---------------------------------------------------------------------------------------------------------------------
VWidgetDetails::VWidgetDetails(VContainer *data, VAbstractPattern *doc, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VWidgetDetails),
      m_doc(doc),
      m_data(data)
{
    ui->setupUi(this);

    FillTable(m_data->DataDetails());

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &VWidgetDetails::InLayoutStateChanged);
}

//---------------------------------------------------------------------------------------------------------------------
VWidgetDetails::~VWidgetDetails()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::UpdateList()
{
    FillTable(m_data->DataDetails());
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::InLayoutStateChanged(int row, int column)
{
    if (column != 0)
    {
        return;
    }

    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    const quint32 id = item->data(Qt::UserRole).toUInt();
    const QHash<quint32, VDetail> *allDetails = m_data->DataDetails();
    const bool inLayout = not allDetails->value(id).IsInLayout();

    ToggleDetailInLayout *togglePrint = new ToggleDetailInLayout(id, inLayout, m_data, m_doc);
    connect(togglePrint, &ToggleDetailInLayout::NeedLiteParsing, m_doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(togglePrint);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::FillTable(const QHash<quint32, VDetail> *details)
{
    const int selectedRow = ui->tableWidget->currentRow();
    ui->tableWidget->clear();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(details->size());
    qint32 currentRow = -1;
    auto i = details->constBegin();
    while (i != details->constEnd())
    {
        ++currentRow;
        const VDetail det = i.value();

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignHCenter);
        if (det.IsInLayout())
        {
            item->setIcon(QIcon("://icon/16x16/allow_detail.png"));
        }
        else
        {
            item->setIcon(QIcon("://icon/16x16/forbid_detail.png"));
        }
        item->setData(Qt::UserRole, i.key());
        ui->tableWidget->setItem(currentRow, 0, item);

        QString name = det.getName();
        if (name.isEmpty())
        {
            name = tr("Unnamed");
        }

        item = new QTableWidgetItem(name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget->setItem(currentRow, 1, item);
        ++i;
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    ui->tableWidget->setCurrentCell(selectedRow, 0);
}
