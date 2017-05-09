/************************************************************************
 **
 **  @file   vpropertydelegate.h
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#ifndef VPROPERTYDELEGATE_H
#define VPROPERTYDELEGATE_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QSize>
#include <QString>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

#include "vpropertyexplorer_global.h"

namespace VPE
{

class VPROPERTYEXPLORERSHARED_EXPORT VPropertyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit VPropertyDelegate(QObject *parent = 0);
    virtual ~VPropertyDelegate() Q_DECL_OVERRIDE;

    //! Creates the editor widget
    virtual QWidget* createEditor (QWidget* parent, const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const Q_DECL_OVERRIDE;

    //! Sets the index data to the editor
    virtual void setEditorData (QWidget * editor, const QModelIndex & index) const Q_DECL_OVERRIDE;

    //! Updates the index data
    virtual void setModelData (QWidget * editor, QAbstractItemModel * model,
                               const QModelIndex & index) const Q_DECL_OVERRIDE;

    //! Returns the size hint
    virtual QSize sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;

    //! Sets the row height. Set this to 0 and the standard will be taken
    void setRowHeight(int height = 0, bool add_to_standard = false);

    //! Renders the delegate using the given painter and style option for the item specified by index.
    virtual void paint (QPainter* painter, const QStyleOptionViewItem& option,
                        const QModelIndex& index ) const Q_DECL_OVERRIDE;

protected:
    int RowHeight;
    bool AddRowHeight;


};

}

#endif // VPROPERTYDELEGATE_H
