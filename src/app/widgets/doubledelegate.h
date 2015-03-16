/************************************************************************
 **
 **  @file   doubledelegate.h
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

#ifndef DOUBLEDELEGATE_H
#define DOUBLEDELEGATE_H

#include <QItemDelegate>

/**
 * @brief The DoubleSpinBoxDelegate class a delegate that allows the user to change double values from the model
 * using a double spin box widget.
 */
class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /**
     * @brief DoubleSpinBoxDelegate constructor.
     * @param parent parent object.
     */
    DoubleSpinBoxDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void    setEditorData(QWidget *editor, const QModelIndex &index) const;
    void    setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    /**
     * @brief NewLastValue help reset last value in const method (hack).
     */
    void    NewLastValue(const qreal &newLastValue) const;
public slots:
    void    commitAndCloseEditor();
private slots:
    void    SetLastValue(const qreal &newLastValue);
private:
    /** @brief lastValue last saved value. */
    qreal lastValue;
};

#endif
