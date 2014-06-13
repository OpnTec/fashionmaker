/************************************************************************
 **
 **  @file   textdelegate.h
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

#ifndef TEXTDELEGATE_H
#define TEXTDELEGATE_H

#include <QItemDelegate>

/**
 * @brief The TextDelegate class a delegate that allows the user to change text values from the model
 * using a edit line widget.
 */
class TextDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TextDelegate(const QString &regex, QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void    setEditorData(QWidget *editor, const QModelIndex &index) const;
    void    setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    /**
     * @brief SaveText help save text value of lineedit.
     * @param text text.
     */
    void    SaveText(const QString &text) const;
public slots:
    void    commitAndCloseEditor();
    void    InitText(const QString &text);
private:
    /** @brief lastValue last saved value. */
    QString lastText;
    QString regex;
};

#endif // TEXTDELEGATE_H
