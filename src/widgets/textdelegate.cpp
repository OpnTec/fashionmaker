/************************************************************************
 **
 **  @file   textdelegate.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

TextDelegate::TextDelegate(QObject *parent): QItemDelegate(parent), lastText(QString("Name_"))
{
    //Little hack. Help save lineedit text in const method.
    connect(this, &TextDelegate::SaveText, this, &TextDelegate::InitText);
}

QWidget *TextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLineEdit *editor = new QLineEdit(parent);
    Q_CHECK_PTR(editor);
    QRegExp re("^(([^+ -/()\\^*%:;\"\'=.,><0-9]){1,1}[^+ -/()\\^*%:;\"\'=><]([0-9]){0,}){0,}$");
    QRegExpValidator *v = new QRegExpValidator(re);
    Q_CHECK_PTR(v);
    editor->setValidator( v );
    connect(editor, &QLineEdit::editingFinished, this, &TextDelegate::commitAndCloseEditor);
    return editor;
}

void TextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    Q_CHECK_PTR(lineEdit);
    if ( lastText != text && text.isEmpty() == false)
    {
        //Here need save text, but method is const, so, we use signal instead.
        emit SaveText(text);
    }
    lineEdit->setText(text);
}

void TextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    Q_CHECK_PTR(lineEdit);
    QString text = lineEdit->text();
    if(text.isEmpty())
    {
        text = lastText;
    }

    model->setData(index, text, Qt::EditRole);
}

void TextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

void TextDelegate::commitAndCloseEditor()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    Q_CHECK_PTR(lineEdit);
    QString text = lineEdit->text();
    if ( lastText != text && text.isEmpty() == false)
    {
        lastText = text;
        emit commitData(lineEdit);
    }
    else if(text.isEmpty() == false)
    {
        lineEdit->setText(lastText);
        emit commitData(lineEdit);
    }
    emit closeEditor(lineEdit);
}

void TextDelegate::InitText(const QString &text)
{
    lastText = text;
}
