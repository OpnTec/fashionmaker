/************************************************************************
 **
 **  @file   vwidgetdetails.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vtools/undocommands/togglepiecestate.h"
#include "../vtools/tools/vtoolseamallowance.h"

#include <QMenu>
#include <QUndoStack>

namespace
{
    enum PieceColumn
    {
        InLayout = 0,
        PieceName = 1
    };

    Q_GLOBAL_STATIC_WITH_ARGS(const QString, allowDetailIcon, (QLatin1String("://icon/16x16/allow_detail.png")))
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, forbidDetailIcon, (QLatin1String("://icon/16x16/forbid_detail.png")))
}

//---------------------------------------------------------------------------------------------------------------------
VWidgetDetails::VWidgetDetails(VContainer *data, VAbstractPattern *doc, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VWidgetDetails),
      m_doc(doc),
      m_data(data)
{
    ui->setupUi(this);

    ui->checkBoxHideNotInLayout->setChecked(false);

    FillTable(m_data->DataPieces());

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &VWidgetDetails::InLayoutStateChanged);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &VWidgetDetails::ShowContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
VWidgetDetails::~VWidgetDetails()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::UpdateList()
{
    FillTable(m_data->DataPieces());
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::SelectDetail(quint32 id)
{
    const int rowCount = ui->tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, PieceColumn::InLayout);

        if (item->data(Qt::UserRole).toUInt() == id)
        {
            ui->tableWidget->setCurrentItem(item);
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::ToggledPiece(quint32 id)
{
    const int rowCount = ui->tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, PieceColumn::InLayout);

        if (item && item->data(Qt::UserRole).toUInt() == id)
        {
            ToggledPieceItem(item);
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::InLayoutStateChanged(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, PieceColumn::InLayout);
    const quint32 id = item->data(Qt::UserRole).toUInt();
    emit Highlight(id);

    if (column != 0)
    {
        return;
    }

    const QHash<quint32, VPiece> *allDetails = m_data->DataPieces();
    const bool inLayout = not allDetails->value(id).IsInLayout();

    TogglePieceInLayout *togglePrint = new TogglePieceInLayout(id, inLayout, m_data, m_doc);
    connect(togglePrint, &TogglePieceInLayout::Toggled, this, &VWidgetDetails::ToggledPiece);
    qApp->getUndoStack()->push(togglePrint);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::FillTable(const QHash<quint32, VPiece> *details)
{
    const int selectedRow = ui->tableWidget->currentRow();
    ui->tableWidget->clearContents();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(details->size());
    qint32 currentRow = -1;
    auto i = details->constBegin();
    while (i != details->constEnd())
    {
        ++currentRow;
        const VPiece det = i.value();

        ui->tableWidget->setItem(currentRow, PieceColumn::InLayout, PrepareInLayoutColumnCell(det, i.key()));
        ui->tableWidget->setItem(currentRow, PieceColumn::PieceName, PreparePieceNameColumnCell(det));
        ++i;
    }
    ui->tableWidget->sortItems(PieceColumn::PieceName, Qt::AscendingOrder);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    ui->tableWidget->setCurrentCell(selectedRow, 0);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::ToggleSectionDetails(bool select)
{
    const QHash<quint32, VPiece> *allDetails = m_data->DataPieces();
    if (allDetails->count() == 0)
    {
        return;
    }

    for (int i = 0; i<ui->tableWidget->rowCount(); ++i)
    {
        const quint32 id = ui->tableWidget->item(i, PieceColumn::InLayout)->data(Qt::UserRole).toUInt();
        if (allDetails->contains(id))
        {
            if (not (select == allDetails->value(id).IsInLayout()))
            {
                TogglePieceInLayout *togglePrint = new TogglePieceInLayout(id, select, m_data, m_doc);
                connect(togglePrint, &TogglePieceInLayout::Toggled, this, &VWidgetDetails::ToggledPiece);
                qApp->getUndoStack()->push(togglePrint);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::ToggledPieceItem(QTableWidgetItem *item)
{
    SCASSERT(item != nullptr)

    quint32 id = item->data(Qt::UserRole).toUInt();
    const QHash<quint32, VPiece> *details = m_data->DataPieces();

    if (details->contains(id))
    {
        const bool inLayout = details->value(id).IsInLayout();
        inLayout ? item->setIcon(QIcon(*allowDetailIcon))
                 : item->setIcon(QIcon(*forbidDetailIcon));

        VToolSeamAllowance *tool = nullptr;
        try
        {
            tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(id));
        }
        catch (VExceptionBadId &)
        {
            // do nothing
        }

        tool->setVisible(ui->checkBoxHideNotInLayout->isChecked() ? inLayout : true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QTableWidgetItem *VWidgetDetails::PrepareInLayoutColumnCell(const VPiece &det, quint32 id) const
{
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignHCenter);
    item->setIcon(det.IsInLayout() ? QIcon(*allowDetailIcon) : QIcon(*forbidDetailIcon));
    item->setData(Qt::UserRole, id);

    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
QTableWidgetItem *VWidgetDetails::PreparePieceNameColumnCell(const VPiece &det) const
{
    QString name = det.GetName();
    if (name.isEmpty())
    {
        name = tr("Unnamed");
    }

    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetDetails::ShowContextMenu(const QPoint &pos)
{
    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionSelectAll = menu->addAction(tr("Select all"));
    QAction *actionSelectNone = menu->addAction(tr("Select none"));

    QAction *actionSeparator = new QAction(this);
    actionSeparator->setSeparator(true);
    menu->addAction(actionSeparator);

    QAction *actionInvertSelection = menu->addAction(tr("Invert selection"));

    const QHash<quint32, VPiece> *allDetails = m_data->DataPieces();
    if (allDetails->count() == 0)
    {
        return;
    }

    int selectedDetails = 0;

    auto iter = allDetails->constBegin();
    while (iter != allDetails->constEnd())
    {
        if(iter.value().IsInLayout())
        {
            selectedDetails++;
        }
        ++iter;
    }

    if (selectedDetails == 0)
    {
        actionSelectNone->setDisabled(true);
    }
    else if (selectedDetails == allDetails->size())
    {
        actionSelectAll->setDisabled(true);
    }

    QAction *selectedAction = menu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));

    bool select;
    if (selectedAction == actionSelectAll)
    {
        select = true;
        qApp->getUndoStack()->beginMacro(tr("select all details"));
        ToggleSectionDetails(select);
        qApp->getUndoStack()->endMacro();
    }
    else if (selectedAction == actionSelectNone)
    {
        select = false;
        qApp->getUndoStack()->beginMacro(tr("select none details"));
        ToggleSectionDetails(select);
        qApp->getUndoStack()->endMacro();
    }
    else if (selectedAction == actionInvertSelection)
    {
        qApp->getUndoStack()->beginMacro(tr("invert selection"));

        for (int i = 0; i<ui->tableWidget->rowCount(); ++i)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, PieceColumn::InLayout);
            const quint32 id = item->data(Qt::UserRole).toUInt();
            if (allDetails->contains(id))
            {
                select = not allDetails->value(id).IsInLayout();

                TogglePieceInLayout *togglePrint = new TogglePieceInLayout(id, select, m_data, m_doc);
                connect(togglePrint, &TogglePieceInLayout::Toggled, this, &VWidgetDetails::ToggledPiece);
                qApp->getUndoStack()->push(togglePrint);
            }
        }

        qApp->getUndoStack()->endMacro();
    }
}

//------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 * enable "in layout" details visible or "not in layout" hidden
 */
void VWidgetDetails::on_checkBoxHideNotInLayout_stateChanged()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        if (QTableWidgetItem *item = ui->tableWidget->item(i, PieceColumn::InLayout))
        {
            ToggledPieceItem(item);
        }
    }
}
