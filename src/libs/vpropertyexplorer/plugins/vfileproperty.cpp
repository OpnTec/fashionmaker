/************************************************************************
 **
 **  @file   vfileproperty.cpp
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

#include "vfileproperty.h"

#include <QAbstractItemDelegate>
#include <QFileInfo>
#include <QLocale>
#include <QWidget>

#include "../vfileproperty_p.h"
#include "vfilepropertyeditor.h"
#include "../vproperty_p.h"

class QStyleOptionViewItem;

VPE::VFileProperty::VFileProperty(const QString& name)
    : VProperty(new VFilePropertyPrivate(name, QVariant::String))
{

}

VPE::VFileProperty::~VFileProperty()
{
    //
}


void VPE::VFileProperty::setFileFilters(const QString& filefilters)
{
    static_cast<VFilePropertyPrivate*>(d_ptr)->FileFilters = filefilters;
}


QString VPE::VFileProperty::getFileFilters() const
{
    return static_cast<VFilePropertyPrivate*>(d_ptr)->FileFilters;
}


void VPE::VFileProperty::setFile(const QString& file)
{
    d_ptr->VariantValue.setValue(file);
}


QString VPE::VFileProperty::getFile() const
{
    return d_ptr->VariantValue.toString();
}


QVariant VPE::VFileProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        QFileInfo tmpFile(d_ptr->VariantValue.toString());
        return tmpFile.fileName();
    }
    else
        return VProperty::data(column, role);
}


QWidget* VPE::VFileProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                     const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options)

    VFileEditWidget* tmpWidget = new VFileEditWidget(parent);
    if (delegate)
    {
        VFileEditWidget::connect(tmpWidget, SIGNAL(commitData(QWidget*)), delegate, SIGNAL(commitData(QWidget*)));

    }
    tmpWidget->setLocale(parent->locale());
    tmpWidget->setFilter(static_cast<VFilePropertyPrivate*>(d_ptr)->FileFilters);   // todo: parse this string
    tmpWidget->setFile(d_ptr->VariantValue.toString());
    tmpWidget->setDirectory(static_cast<VFilePropertyPrivate*>(d_ptr)->Directory);
    return tmpWidget;
}


bool VPE::VFileProperty::setEditorData(QWidget* editor)
{
    VFileEditWidget* tmpWidget = qobject_cast<VFileEditWidget*>(editor);
    if (tmpWidget)
    {
        tmpWidget->setFile(d_ptr->VariantValue.toString());
    }
    else
        return false;

    return true;
}


QVariant VPE::VFileProperty::getEditorData(const QWidget *editor) const
{
    const VFileEditWidget* tmpWidget = qobject_cast<const VFileEditWidget*>(editor);
    if (tmpWidget)
    {
        return tmpWidget->getFile();
    }

    return QVariant();
}

void VPE::VFileProperty::setSetting(const QString& key, const QVariant& value)
{
    if (key == "FileFilters")
    {
        setFileFilters(value.toString());
    }
    else if (key == "Directory")
    {
        setDirectory(value.toBool());
    }
}

QVariant VPE::VFileProperty::getSetting(const QString& key) const
{
    if (key == "FileFilters")
    {
        return getFileFilters();
    }
    else if (key == "Directory")
    {
        return isDirectory();
    }
    else
        return VProperty::getSetting(key);
}

QStringList VPE::VFileProperty::getSettingKeys() const
{
    return QStringList("FileFilters") << "Directory";
}

QString VPE::VFileProperty::type() const
{
    return "file";
}

VPE::VProperty* VPE::VFileProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VFileProperty(getName()));
}

bool VPE::VFileProperty::isDirectory() const
{
    return static_cast<VFilePropertyPrivate*>(d_ptr)->Directory;
}


void VPE::VFileProperty::setDirectory(bool is_directory)
{
    static_cast<VFilePropertyPrivate*>(d_ptr)->Directory = is_directory;
}
