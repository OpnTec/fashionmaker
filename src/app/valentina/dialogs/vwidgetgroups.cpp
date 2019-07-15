/************************************************************************
 **
 **  @file   vwidgetgroups.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 4, 2016
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

#include "vwidgetgroups.h"
#include "ui_vwidgetgroups.h"
#include "../vtools/dialogs/tools/dialoggroup.h"
#include "../vtools/undocommands/delgroup.h"
#include "../vtools/undocommands/changegroupvisibility.h"
#include "../vtools/undocommands/changemultiplegroupsvisibility.h"
#include "../vpatterndb/vcontainer.h"

#include <QMenu>
#include <QTableWidget>

//---------------------------------------------------------------------------------------------------------------------
VWidgetGroups::VWidgetGroups(VAbstractPattern *doc, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VWidgetGroups),
      doc(doc)
{
    ui->setupUi(this);

    SCASSERT(doc != nullptr)
    FillTable(doc->GetGroups());

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &VWidgetGroups::GroupVisibilityChanged);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &VWidgetGroups::RenameGroup);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &VWidgetGroups::CtxMenu);
}

//---------------------------------------------------------------------------------------------------------------------
VWidgetGroups::~VWidgetGroups()
{
    delete ui;
}
//----------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::SetGroupVisibility(vidtype id, bool visible) const
{
    ChangeGroupVisibility *changeGroup = new ChangeGroupVisibility(doc, id, visible);
    connect(changeGroup, &ChangeGroupVisibility::UpdateGroup, this, [this](vidtype id, bool visible)
    {
        int row = GroupRow(id);
        if (row == -1)
        {
            return;
        }

        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item)
        {
            (visible) ? item->setIcon(QIcon(QStringLiteral("://icon/16x16/open_eye.png")))
                      : item->setIcon(QIcon(QStringLiteral("://icon/16x16/closed_eye.png")));
        }
    });
    qApp->getUndoStack()->push(changeGroup);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::SetMultipleGroupsVisibility(const QVector<vidtype> &groups, bool visible) const
{
    auto *changeGroups = new ChangeMultipleGroupsVisibility(doc, groups, visible);
    connect(changeGroups, &ChangeMultipleGroupsVisibility::UpdateMultipleGroups, this,
            [this](const QMap<vidtype, bool> &groups)
    {
        QMap<vidtype, bool>::const_iterator i = groups.constBegin();
        while (i != groups.constEnd())
        {
            int row = GroupRow(i.key());
            if (row == -1)
            {
                ++i;
                continue;
            }

            QTableWidgetItem *item = ui->tableWidget->item(row, 0);
            if (item)
            {
                (i.value()) ? item->setIcon(QIcon(QStringLiteral("://icon/16x16/open_eye.png")))
                            : item->setIcon(QIcon(QStringLiteral("://icon/16x16/closed_eye.png")));
            }
            ++i;
        }

    });
    qApp->getUndoStack()->push(changeGroups);
}

//---------------------------------------------------------------------------------------------------------------------
int VWidgetGroups::GroupRow(vidtype id) const
{
    for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
    {
        QTableWidgetItem *item = ui->tableWidget->item(r, 0);
        SCASSERT(item != nullptr)

        if (id == item->data(Qt::UserRole).toUInt())
        {
            return r;
        }
    }

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::GroupVisibilityChanged(int row, int column)
{
    if (column != 0)
    {
        return;
    }
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    const quint32 id = item->data(Qt::UserRole).toUInt();
    SetGroupVisibility(id, not doc->GetGroupVisibility(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::RenameGroup(int row, int column)
{
    if (column != 1)
    {
        return;
    }

    const quint32 id = ui->tableWidget->item(row, 0)->data(Qt::UserRole).toUInt();
    doc->SetGroupName(id, ui->tableWidget->item(row, column)->text());

    UpdateGroups();
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::CtxMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    if(not item)
    {
        return;
    }

    const int row = item->row();
    item = ui->tableWidget->item(row, 0);
    const quint32 id = item->data(Qt::UserRole).toUInt();

    auto MultipleChangeVisibilityTo = [this](bool visibility)
    {
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
        {
            QTableWidgetItem *rowItem = ui->tableWidget->item(r, 0);
            if (rowItem and visibility != doc->GetGroupVisibility(rowItem->data(Qt::UserRole).toUInt()))
            {
                return true;
            }
        }

        return false;
    };

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *triggerVisibilityMenu = doc->GetGroupVisibility(id) ?
                menu->addAction(QIcon(QStringLiteral("://icon/16x16/closed_eye.png")), tr("Hide")) :
                menu->addAction(QIcon(QStringLiteral("://icon/16x16/open_eye.png")), tr("Show"));

    QAction *actionRename = menu->addAction(tr("Rename"));
    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));
    menu->addSeparator();
    QAction *actionHideAll = menu->addAction(tr("Hide All"));
    actionHideAll->setEnabled(MultipleChangeVisibilityTo(false));
    QAction *actionShowAll = menu->addAction(tr("Show All"));
    actionShowAll->setEnabled(MultipleChangeVisibilityTo(true));

    QAction *selectedAction = menu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));

    if (selectedAction == triggerVisibilityMenu)
    {
        SetGroupVisibility(id, not doc->GetGroupVisibility(id));
    }
    else if (selectedAction == actionRename)
    {
        QScopedPointer<VContainer> fackeContainer(new VContainer(qApp->TrVars(), qApp->patternUnitP(),
                                                                 VContainer::UniqueNamespace()));
        QScopedPointer<DialogGroup> dialog(new DialogGroup(fackeContainer.data(), NULL_ID, this));
        dialog->SetName(doc->GetGroupName(id));
        const int result = dialog->exec();

        if (result == QDialog::Accepted)
        {
            doc->SetGroupName(id, dialog->GetName());
            item = ui->tableWidget->item(row, 1);
            item->setText(dialog->GetName());
        }
    }
    else if (selectedAction == actionDelete)
    {
        DelGroup *delGroup = new DelGroup(doc, id);
        connect(delGroup, &DelGroup::UpdateGroups, this, &VWidgetGroups::UpdateGroups);
        qApp->getUndoStack()->push(delGroup);
    }
    else if (selectedAction == actionHideAll)
    {//all groups in "group" make unvisible
        if (ui->tableWidget->rowCount() < 1)
        {
            return;
        }

        QVector<vidtype> groups;
        groups.reserve(ui->tableWidget->rowCount());
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
        {
            QTableWidgetItem *rowItem = ui->tableWidget->item(r, 0);
            quint32 i = rowItem->data(Qt::UserRole).toUInt();
            if (doc->GetGroupVisibility(i))
            {
                groups.append(i);
            }
        }

        if (not groups.isEmpty())
        {
            SetMultipleGroupsVisibility(groups, false);
        }
    }
    else if (selectedAction == actionShowAll)
    {//all groups in "group" make visible
        if (ui->tableWidget->rowCount() < 1)
        {
            return;
        }

        QVector<vidtype> groups;
        groups.reserve(ui->tableWidget->rowCount());
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
        {
            QTableWidgetItem *rowItem = ui->tableWidget->item(r, 0);
            quint32 i = rowItem->data(Qt::UserRole).toUInt();
            if (not doc->GetGroupVisibility(i))
            {
                groups.append(i);
            }
        }

        if (not groups.isEmpty())
        {
            SetMultipleGroupsVisibility(groups, true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::UpdateGroups()
{
    FillTable(doc->GetGroups());
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetGroups::FillTable(const QMap<quint32, QPair<QString, bool> > &groups)
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clear();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(groups.size());
    qint32 currentRow = -1;
    auto i = groups.constBegin();
    while (i != groups.constEnd())
    {
        ++currentRow;
        const QPair<QString, bool> data = i.value();

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignHCenter);
        (data.second) ? item->setIcon(QIcon("://icon/16x16/open_eye.png"))
                      : item->setIcon(QIcon("://icon/16x16/closed_eye.png"));

        item->setData(Qt::UserRole, i.key());

        // set the item non-editable (view only), and non-selectable
        Qt::ItemFlags flags = item->flags();
        flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);

        ui->tableWidget->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(data.first);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        if(doc->GroupIsEmpty(i.key()))
        {
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
        }

        ui->tableWidget->setItem(currentRow, 1, item);
        ++i;
    }
    ui->tableWidget->sortItems(1, Qt::AscendingOrder);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->blockSignals(false);
}
