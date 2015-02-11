/************************************************************************
 **
 **  @file   vlinecolorproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 2, 2015
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

#include "vlinecolorproperty.h"
#include "../vproperty_p.h"

#include <QComboBox>
#include <QCoreApplication>

using namespace VPE;

VLineColorProperty::VLineColorProperty(const QString &name)
    : VProperty(name, QVariant::Int), colors(), indexList()
{
    VProperty::d_ptr->VariantValue = 0;
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);
}

QVariant VLineColorProperty::data(int column, int role) const
{
    if (colors.empty())
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

QWidget *VLineColorProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options,
                                          const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QComboBox* tmpEditor = new QComboBox(parent);
    tmpEditor->clear();

    QMap<QString, QString>::const_iterator i = colors.constBegin();
    while (i != colors.constEnd())
    {
        QPixmap pix(16, 16);
        pix.fill(QColor(i.key()));
        tmpEditor->addItem(QIcon(pix), i.value(), QVariant(i.key()));
        ++i;
    }

    tmpEditor->setLocale(parent->locale());
    tmpEditor->setCurrentIndex(VProperty::d_ptr->VariantValue.toInt());
    connect(tmpEditor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                     &VLineColorProperty::currentIndexChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

QVariant VLineColorProperty::getEditorData(const QWidget *editor) const
{
    const QComboBox* tmpEditor = qobject_cast<const QComboBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->currentIndex();
    }

    return QVariant(0);
}

void VLineColorProperty::setColors(const QMap<QString, QString> &colors)
{
    this->colors = colors;
    indexList.clear();
    QMap<QString, QString>::const_iterator i = colors.constBegin();
    while (i != colors.constEnd())
    {
        indexList.append(i.key());
        ++i;
    }
}

QMap<QString, QString> VLineColorProperty::getColors() const
{
    return colors;
}

void VLineColorProperty::setValue(const QVariant &value)
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

QString VLineColorProperty::type() const
{
    return QStringLiteral("lineColor");
}

VProperty *VLineColorProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VLineColorProperty(getName()));
}

int VLineColorProperty::IndexOfColor(const QMap<QString, QString> &colors, const QString &color)
{
    QVector<QString> indexList;
    QMap<QString, QString>::const_iterator i = colors.constBegin();
    while (i != colors.constEnd())
    {
        indexList.append(i.key());
        ++i;
    }
    return indexList.indexOf(color);
}

void VLineColorProperty::currentIndexChanged(int index)
{
    Q_UNUSED(index)
    UserChangeEvent *event = new UserChangeEvent();
    QCoreApplication::postEvent ( VProperty::d_ptr->editor, event );
}
