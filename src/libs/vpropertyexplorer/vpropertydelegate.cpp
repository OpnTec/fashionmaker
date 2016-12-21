/************************************************************************
 **
 **  @file   vpropertydelegate.cpp
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

#include "vpropertydelegate.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QColor>
#include <QModelIndex>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QRgb>
#include <QStyle>
#include <QVariant>

#include "vproperty.h"

class QStyleOptionViewItem;
class QWidget;

VPE::VPropertyDelegate::VPropertyDelegate(QObject *parent) :
    QStyledItemDelegate(parent), RowHeight(0), AddRowHeight(false)
{
}

VPE::VPropertyDelegate::~VPropertyDelegate()
{
    //
}

QWidget* VPE::VPropertyDelegate::createEditor (QWidget* parent, const QStyleOptionViewItem& option,
                                               const QModelIndex& index) const
{
    QWidget* tmpWidget = nullptr;
    if (index.isValid())
    {
        VProperty* tmpProperty = reinterpret_cast<VProperty*>(index.internalPointer());
        tmpWidget = tmpProperty->createEditor(parent, option, this);
    }

    return tmpWidget ? tmpWidget : QStyledItemDelegate::createEditor(parent, option, index);
}


//! Sets the index data to the editor
void VPE::VPropertyDelegate::setEditorData (QWidget * editor, const QModelIndex & index) const
{
    bool done = false;
    if (index.isValid() && editor)
    {
        VProperty* tmpProperty = reinterpret_cast<VProperty*>(index.internalPointer());
        done = tmpProperty->setEditorData(editor);
    }

    if (!done)
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

//! Updates the index data
void VPE::VPropertyDelegate::setModelData (QWidget * editor, QAbstractItemModel * model,
                                           const QModelIndex & index) const
{
    QVariant tmpData;
    if (index.isValid() && editor)
    {
        VProperty* tmpProperty = reinterpret_cast<VProperty*>(index.internalPointer());
        tmpData = tmpProperty->getEditorData(editor);
    }

    if (tmpData.isNull())
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
    else
        model->setData(index, tmpData);
}

QSize VPE::VPropertyDelegate::sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize tmpStandardSizeHint = QStyledItemDelegate::sizeHint(option, index);
    tmpStandardSizeHint.setHeight(tmpStandardSizeHint.height() + 1);

    if (RowHeight > 0)
    {
        return QSize(tmpStandardSizeHint.width(), AddRowHeight ? tmpStandardSizeHint.height() + RowHeight : RowHeight);
    }
    else
        return tmpStandardSizeHint;
}

void VPE::VPropertyDelegate::setRowHeight(int height, bool add_to_standard)
{
    RowHeight = height;
    AddRowHeight = add_to_standard;
}

void VPE::VPropertyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                   const QModelIndex& index ) const
{
    bool done = false;
    if (index.isValid() && index.column() == 1)
    {
        done = reinterpret_cast<VProperty*>(index.internalPointer())->paint(painter, option, index, this);
    }

    if (!done)
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    QColor tmpPenColor = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));

    QPen tmpOldPen = painter->pen();
    painter->setPen(QPen(tmpPenColor));
    painter->drawLine(option.rect.right(), option.rect.y(), option.rect.right(), option.rect.bottom());
    painter->drawLine(option.rect.x(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
    painter->setPen(tmpOldPen);
}
