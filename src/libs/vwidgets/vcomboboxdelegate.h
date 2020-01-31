/************************************************************************
 **
 **  @file   vcomboboxdelegate.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 9, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef VCOMBOBOXDELEGATE_H
#define VCOMBOBOXDELEGATE_H

#include <QItemDelegate>

class VComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    VComboBoxDelegate(const QStringList &items, QObject *parent = nullptr);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const override;

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const override;
private:
    Q_DISABLE_COPY(VComboBoxDelegate)
    QStringList m_items;
};

#endif // VCOMBOBOXDELEGATE_H
