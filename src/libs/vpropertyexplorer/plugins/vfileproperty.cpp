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
#include <QFileInfo>
#include <QAbstractItemDelegate>

#include "vfilepropertyeditor.h"

#include "../vfileproperty_p.h"

using namespace VPE;

VFileProperty::VFileProperty(const QString& name)
    : VProperty(new VFilePropertyPrivate(name, QVariant::String))
{

}

VFileProperty::~VFileProperty()
{
    //
}


void VFileProperty::setFileFilters(const QString& filefilters)
{
    static_cast<VFilePropertyPrivate*>(d_ptr)->FileFilters = filefilters;
}


QString VFileProperty::getFileFilters() const
{
    return static_cast<VFilePropertyPrivate*>(d_ptr)->FileFilters;
}


void VFileProperty::setFile(const QString& file)
{
    d_ptr->VariantValue.setValue(file);
}


QString VFileProperty::getFile() const
{
    return d_ptr->VariantValue.toString();
}


QVariant VFileProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        QFileInfo tmpFile(d_ptr->VariantValue.toString());
        return tmpFile.fileName();
    }
    else
        return VProperty::data(column, role);
}


QWidget* VFileProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                     const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);

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


bool VFileProperty::setEditorData(QWidget* editor)
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


QVariant VFileProperty::getEditorData(const QWidget *editor) const
{
    const VFileEditWidget* tmpWidget = qobject_cast<const VFileEditWidget*>(editor);
    if (tmpWidget)
    {
        return tmpWidget->getFile();
    }

    return QVariant();
}

void VFileProperty::setSetting(const QString& key, const QVariant& value)
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

QVariant VFileProperty::getSetting(const QString& key) const
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

QStringList VFileProperty::getSettingKeys() const
{
    return QStringList("FileFilters") << "Directory";
}

QString VFileProperty::type() const
{
    return "file";
}

VProperty* VFileProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VFileProperty(getName()));
}

bool VFileProperty::isDirectory() const
{
    return static_cast<VFilePropertyPrivate*>(d_ptr)->Directory;
}


void VFileProperty::setDirectory(bool is_directory)
{
    static_cast<VFilePropertyPrivate*>(d_ptr)->Directory = is_directory;
}
