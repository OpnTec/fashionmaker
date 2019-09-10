/************************************************************************
 **
 **  @file   vcomboboxdelegate.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 9, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#include "vcomboboxdelegate.h"

#include <QComboBox>

//---------------------------------------------------------------------------------------------------------------------
VComboBoxDelegate::VComboBoxDelegate(const QStringList &items, QObject *parent)
    : QItemDelegate(parent),
      m_items(items)
{
    const QString dashDash = QStringLiteral("--");
    m_items.prepend(dashDash + tr("Select material") + dashDash);
}

//---------------------------------------------------------------------------------------------------------------------
QWidget *VComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *editor = new QComboBox(parent);
    editor->addItems(m_items);

    return editor;
}

//---------------------------------------------------------------------------------------------------------------------
void VComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    const int cIndex = comboBox->findText(value);

    if (cIndex != -1)
    {
        comboBox->setCurrentIndex(cIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value;
    const int cIndex = comboBox->currentIndex();

    if (cIndex > 0)
    {
        value = comboBox->currentText();
    }

    model->setData(index, value, Qt::EditRole);
}

//---------------------------------------------------------------------------------------------------------------------
void VComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
