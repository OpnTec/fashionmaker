/************************************************************************
 **
 **  @file   doubledelegate.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "doubledelegate.h"
#include <QDoubleSpinBox>
#include "../vmisc/def.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief createEditorReturns the widget used to edit the item specified by index for editing. The parent widget and
 * style option are used to control how the editor widget appears.
 * @param parent parent widget.
 * @param option item options.
 * @param index index of data item.
 * @return editor to be used for editing the data item.
 */
//cppcheck-suppress unusedFunction
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent): QItemDelegate(parent), lastValue(-10001.0)
{
    //Little hack. Help save value in const method.
    connect(this, &DoubleSpinBoxDelegate::NewLastValue, this, &DoubleSpinBoxDelegate::SetLastValue);
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                             const QModelIndex &index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    emit NewLastValue(-10001.0);//Here need reset value to default because we begin work with new item
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setLocale(parent->locale());
    editor->setMinimum(-10000.0);
    editor->setMaximum(10000.0);
    connect(editor, &QDoubleSpinBox::editingFinished, this, &DoubleSpinBoxDelegate::commitAndCloseEditor);
    return editor;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setEditorData sets the data to be displayed and edited by the editor from the data model item specified by
 * the model index.
 * @param editor editor.
 * @param index index of data item.
 */
//cppcheck-suppress unusedFunction
void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qreal value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    SCASSERT(spinBox != nullptr);
    spinBox->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setModelData gets data from the editor widget and stores it in the specified model at the item index.
 * @param editor editor.
 * @param model model.
 * @param index index of data item.
 */
//cppcheck-suppress unusedFunction
void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    SCASSERT(spinBox != nullptr);
    spinBox->interpretText();
    qreal value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief updateEditorGeometry updates the editor for the item specified by index according to the style option
 * given.
 * @param editor editor.
 * @param option item options.
 * @param index index of data item.
 */
//cppcheck-suppress unusedFunction
void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief commitAndCloseEditor commit value if changed.
 */
void DoubleSpinBoxDelegate::commitAndCloseEditor()
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(sender());
    SCASSERT(spinBox != nullptr);
    qreal value = spinBox->value();
    if (qFuzzyCompare ( lastValue, value ) == false)
    {
        lastValue = value;
        emit commitData(spinBox);
    }
    emit closeEditor(spinBox);
}

//---------------------------------------------------------------------------------------------------------------------
void DoubleSpinBoxDelegate::SetLastValue(const qreal &newLastValue)
{
    lastValue = newLastValue;
}
