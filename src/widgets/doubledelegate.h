/************************************************************************
 **
 **  @file   doubledelegate.h
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

#ifndef DOUBLEDELEGATE_H
#define DOUBLEDELEGATE_H

#include <QItemDelegate>

/**
 * @brief The DoubleSpinBoxDelegate class a delegate that allows the user to change integer values from the model
 * using a spin box widget.
 */
class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
            /**
             * @brief DoubleSpinBoxDelegate constructor.
             * @param parent parent object.
             */
    DoubleSpinBoxDelegate(QObject *parent = 0): QItemDelegate(parent), lastValue(-10001.0){}
    /**
     * @brief createEditorReturns the widget used to edit the item specified by index for editing. The parent widget and
     * style option are used to control how the editor widget appears.
     * @param parent parent widget.
     * @param option item options.
     * @param index index of data item.
     * @return editor to be used for editing the data item.
     */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /**
     * @brief setEditorData sets the data to be displayed and edited by the editor from the data model item specified by
     * the model index.
     * @param editor editor.
     * @param index index of data item.
     */
    void    setEditorData(QWidget *editor, const QModelIndex &index) const;
    /**
     * @brief setModelData gets data from the editor widget and stores it in the specified model at the item index.
     * @param editor editor.
     * @param model model.
     * @param index index of data item.
     */
    void    setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    /**
     * @brief updateEditorGeometry updates the editor for the item specified by index according to the style option
     * given.
     * @param editor editor.
     * @param option item options.
     * @param index index of data item.
     */
    void    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    /**
     * @brief commitAndCloseEditor commit value if changed.
     */
    void    commitAndCloseEditor();
private:
    /**
     * @brief lastValue last saved value.
     */
    qreal lastValue;
};

#endif
