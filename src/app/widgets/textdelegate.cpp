/************************************************************************
 **
 **  @file   textdelegate.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 1, 2014
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

#include "textdelegate.h"
#include <QLineEdit>
#include "../options.h"
#include "../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TextDelegate constructor.
 * @param parent parent widget.
 */
TextDelegate::TextDelegate(const QString &regex, VContainer *data, QObject *parent)
    : QItemDelegate(parent), lastText(QString("Name_")), regex(regex), data(data)
{
    SCASSERT(data);
    //Little hack. Help save lineedit text in const method.
    connect(this, &TextDelegate::SaveText, this, &TextDelegate::InitText);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief createEditorReturns the widget used to edit the item specified by index for editing. The parent widget and
 * style option are used to control how the editor widget appears.
 * @param parent parent widget.
 * @param option item options.
 * @param index index of data item.
 * @return editor to be used for editing the data item.
 */
QWidget *TextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLineEdit *editor = new QLineEdit(parent);
    //Same regex pattern in xsd file
    editor->setValidator( new QRegExpValidator(QRegExp(regex)) );
    connect(editor, &QLineEdit::editingFinished, this, &TextDelegate::commitAndCloseEditor);
    return editor;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setEditorData sets the data to be displayed and edited by the editor from the data model item specified by
 * the model index.
 * @param editor editor.
 * @param index index of data item.
 */
void TextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    SCASSERT(lineEdit != nullptr);
    if ( lastText != text && text.isEmpty() == false)
    {
        //Here need save text, but method is const, so, we use signal instead.
        emit SaveText(text);
    }
    lineEdit->setText(text);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setModelData gets data from the editor widget and stores it in the specified model at the item index.
 * @param editor editor.
 * @param model model.
 * @param index index of data item.
 */
void TextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    SCASSERT(lineEdit != nullptr);
    QString text = lineEdit->text();
    if (text.isEmpty())
    {
        text = lastText;
    }

    model->setData(index, text, Qt::EditRole);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief updateEditorGeometry updates the editor for the item specified by index according to the style option
 * given.
 * @param editor editor.
 * @param option item options.
 * @param index index of data item.
 */
void TextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief commitAndCloseEditor commit value if changed.
 */
void TextDelegate::commitAndCloseEditor()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    SCASSERT(lineEdit != nullptr);
    QString text = lineEdit->text();
    if ( lastText != text && text.isEmpty() == false && data->VariableExist(text) == false)
    {
        lastText = text;
        emit commitData(lineEdit);
    }
    else if (text.isEmpty() == false)
    {
        lineEdit->setText(lastText);
        emit commitData(lineEdit);
    }
    emit closeEditor(lineEdit);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief InitText initialize text.
 * @param text text.
 */
void TextDelegate::InitText(const QString &text)
{
    lastText = text;
}
