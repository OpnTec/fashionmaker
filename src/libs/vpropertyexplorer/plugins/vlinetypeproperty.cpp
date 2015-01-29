/************************************************************************
 **
 **  @file   vlinetypeproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vlinetypeproperty.h"
#include "../vproperty_p.h"

#include <QComboBox>
#include <QCoreApplication>

using namespace VPE;

VLineTypeProperty::VLineTypeProperty(const QString &name)
    : VProperty(name, QVariant::Int), styles(), indexList()
{
    VProperty::d_ptr->VariantValue = 0;
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);
}

QVariant VLineTypeProperty::data(int column, int role) const
{
    if (styles.empty())
    {
        return QVariant();
    }

    int tmpIndex = VProperty::d_ptr->VariantValue.toInt();

    if (tmpIndex < 0 || tmpIndex >= indexList.count())
    {
        tmpIndex = 0;
    }

    if (column == DPC_Data && Qt::DisplayRole == role)
    {
        return indexList.at(tmpIndex);
    }
    else if (column == DPC_Data && Qt::EditRole == role)
    {
        return tmpIndex;
    }
    else
    {
        return VProperty::data(column, role);
    }
}

QWidget *VLineTypeProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options,
                                         const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QComboBox* tmpEditor = new QComboBox(parent);
    tmpEditor->clear();
    tmpEditor->setIconSize(QSize(80, 14));
    tmpEditor->setMinimumWidth(80);
    tmpEditor->setMaximumWidth(110);

    QMap<QString, QIcon>::const_iterator i = styles.constBegin();
    while (i != styles.constEnd())
    {
        tmpEditor->addItem(i.value(), "", QVariant(i.key()));
        ++i;
    }

    tmpEditor->setCurrentIndex(VProperty::d_ptr->VariantValue.toInt());
    connect(tmpEditor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                     &VLineTypeProperty::currentIndexChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

QVariant VLineTypeProperty::getEditorData(const QWidget *editor) const
{
    const QComboBox* tmpEditor = qobject_cast<const QComboBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->currentIndex();
    }

    return QVariant(0);
}

void VLineTypeProperty::setStyles(const QMap<QString, QIcon> &styles)
{
    this->styles = styles;
    indexList.clear();
    QMap<QString, QIcon>::const_iterator i = styles.constBegin();
    while (i != styles.constEnd())
    {
        indexList.append(i.key());
        ++i;
    }
}

QMap<QString, QIcon> VLineTypeProperty::getStyles() const
{
    return styles;
}

void VLineTypeProperty::setValue(const QVariant &value)
{
    int tmpIndex = value.toInt();

    if (tmpIndex < 0 || tmpIndex >= indexList.count())
    {
        tmpIndex = 0;
    }

    VProperty::d_ptr->VariantValue = tmpIndex;
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);

    if (VProperty::d_ptr->editor != nullptr)
    {
        setEditorData(VProperty::d_ptr->editor);
    }
}

QString VLineTypeProperty::type() const
{
    return QStringLiteral("lineType");
}

VProperty *VLineTypeProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VLineTypeProperty(getName()));
}

int VLineTypeProperty::IndexOfStyle(const QMap<QString, QIcon> &styles, const QString &style)
{
    QVector<QString> indexList;
    QMap<QString, QIcon>::const_iterator i = styles.constBegin();
    while (i != styles.constEnd())
    {
        indexList.append(i.key());
        ++i;
    }
    return indexList.indexOf(style);
}

void VLineTypeProperty::currentIndexChanged(int index)
{
    Q_UNUSED(index)
    UserChangeEvent *event = new UserChangeEvent();
    QCoreApplication::postEvent ( VProperty::d_ptr->editor, event );
}
