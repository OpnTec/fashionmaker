/************************************************************************
 **
 **  @file   doubledelegate.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

 /*
     doubledelegate.cpp

     A delegate that allows the user to change integer values from the model
     using a spin box widget.
 */

#include "doubledelegate.h"

#include <QDoubleSpinBox>

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                             const QModelIndex &index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    Q_ASSERT(editor != 0);
    editor->setMinimum(-10000.0);
    editor->setMaximum(10000.0);
    connect(editor, &QDoubleSpinBox::editingFinished, this, &DoubleSpinBoxDelegate::commitAndCloseEditor);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qreal value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    Q_ASSERT(spinBox != 0);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    Q_ASSERT(spinBox != 0);
    spinBox->interpretText();
    qreal value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

void DoubleSpinBoxDelegate::commitAndCloseEditor()
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(sender());
    Q_ASSERT(spinBox != 0);
    qreal value = spinBox->value();
    if(qFuzzyCompare ( lastValue, value ) == false)
    {
        lastValue = value;
        emit commitData(spinBox);
    }
    emit closeEditor(spinBox);
}
