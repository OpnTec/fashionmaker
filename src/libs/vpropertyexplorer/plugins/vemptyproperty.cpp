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

using namespace VPE;

VEmptyProperty::VEmptyProperty(const QString& name)
    : VProperty(name, QVariant::Invalid)
{
}


VEmptyProperty::VEmptyProperty(VPropertyPrivate *d)
    : VProperty(d)
{
}


VEmptyProperty::~VEmptyProperty()
{
    //
}

//! Get the data how it should be displayed
QVariant VEmptyProperty::data (int column, int role) const
{
    if(column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
        return QVariant();
    else if(role == Qt::BackgroundRole)
        return QBrush(QColor(217,217,217));
    else if(role == Qt::FontRole)
    { QFont tmpFont; tmpFont.setBold(true); return tmpFont; }
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VEmptyProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(parent);
    Q_UNUSED(delegate);

    return NULL;
}


//! Gets the data from the widget
QVariant VEmptyProperty::getEditorData(QWidget* editor) const
{
    Q_UNUSED(editor);

    return QVariant();
}

//! Returns item flags
Qt::ItemFlags VEmptyProperty::flags(int column) const
{
    Q_UNUSED(column);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QString VEmptyProperty::type() const
{
    return "empty";
}

VProperty* VEmptyProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VEmptyProperty(getName()));
}
