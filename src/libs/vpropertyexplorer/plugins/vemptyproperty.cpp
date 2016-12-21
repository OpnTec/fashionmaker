/************************************************************************
 **
 **  @file   vemptyproperty.cpp
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

#include "vemptyproperty.h"

#include <stddef.h>
#include <QBrush>
#include <QColor>
#include <QFlags>
#include <QFont>

#include "../vproperty.h"

class QAbstractItemDelegate;
class QStyleOptionViewItem;
class QWidget;
namespace VPE {
class VPropertyPrivate;
}  // namespace VPE

VPE::VEmptyProperty::VEmptyProperty(const QString& name)
    : VProperty(name, QVariant::Invalid)
{
}


VPE::VEmptyProperty::VEmptyProperty(VPropertyPrivate *d)
    : VProperty(d)
{
}


VPE::VEmptyProperty::~VEmptyProperty()
{
    //
}

//! Get the data how it should be displayed
QVariant VPE::VEmptyProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        return QVariant();
    }
    else if (role == Qt::BackgroundRole)
    {
        return QBrush(QColor(217, 217, 217));
    }
    else if (role == Qt::FontRole)
    {
        QFont tmpFont; tmpFont.setBold(true); return tmpFont;
    }
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VPE::VEmptyProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                           const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options)
    Q_UNUSED(parent)
    Q_UNUSED(delegate)

    return NULL;
}


//! Gets the data from the widget
QVariant VPE::VEmptyProperty::getEditorData(const QWidget *editor) const
{
    Q_UNUSED(editor)

    return QVariant();
}

//! Returns item flags
Qt::ItemFlags VPE::VEmptyProperty::flags(int column) const
{
    Q_UNUSED(column)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QString VPE::VEmptyProperty::type() const
{
    return "empty";
}

VPE::VProperty* VPE::VEmptyProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VEmptyProperty(getName()));
}
