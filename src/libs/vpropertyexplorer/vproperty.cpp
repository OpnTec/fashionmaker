/************************************************************************
 **
 **  @file   vproperty.cpp
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

#include "vproperty.h"
#include <QObject>
#include <QMetaProperty>
#include <QItemEditorFactory>
#include <QLineEdit>

#include "vproperty_p.h"

using namespace VPE;

//! Standard constructor, takes a name and a parent property as argument
VProperty::VProperty(const QString& name, QVariant::Type type)
    : QObject(), d_ptr(new VPropertyPrivate(name, type))
{

}

VProperty::VProperty(VPropertyPrivate *d)
    : d_ptr(d)
{
}


VProperty::~VProperty()
{
    setParent(nullptr);

    while (!d_ptr->Children.isEmpty())
    {
        VProperty* tmpChild = d_ptr->Children.takeLast();
        delete tmpChild;
    }

    delete d_ptr;
}

QString VProperty::type() const
{
    return "string";
}

//! Get the data how it should be displayed
QVariant VProperty::data (int column, int role) const
{
    if (column == DPC_Name && Qt::DisplayRole == role)
    {
        return QVariant(d_ptr->Name);
    }
    else if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        return d_ptr->VariantValue;
    }
    else if (Qt::ToolTipRole == role)
    {
        return QVariant(d_ptr->Description);
    }
    else
        return QVariant();
}

bool VProperty::setData(const QVariant &data, int role)
{
    bool tmpResult = false;
    if (Qt::EditRole == role)
    {
        tmpResult = (d_ptr->VariantValue != data);
        setValue(data);
    }

    return tmpResult;
}

bool VProperty::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,
                      const QAbstractItemDelegate *delegate) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(index);
    Q_UNUSED(delegate);

    return false;
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                 const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *lineCreator = new QStandardItemEditorCreator<QLineEdit>();
    factory->registerEditor(QVariant::String, lineCreator);
    QItemEditorFactory::setDefaultFactory(factory);

    d_ptr->editor = factory->createEditor(d_ptr->PropertyVariantType, parent);
    return d_ptr->editor;
}

bool VProperty::setEditorData(QWidget* editor)
{
    if (!editor)
    {
        return false;
    }

    QByteArray n = editor->metaObject()->userProperty().name();

    if (!n.isEmpty())
    {
        editor->blockSignals(true);
        editor->setProperty(n, d_ptr->VariantValue);
        editor->blockSignals(false);
        return true;
    }

    return false;
}

//! Gets the data from the widget
QVariant VProperty::getEditorData(QWidget* editor) const
{
    if (!editor)
    {
        return QVariant();
    }

    QByteArray n = editor->metaObject()->userProperty().name();

    if (!n.isEmpty())
    {
        return editor->property(n);
    }
    else
        return QVariant();
}

//! Returns item flags
Qt::ItemFlags VProperty::flags(int column) const
{
    if (column == DPC_Name)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else if (column == DPC_Data)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    }
    else
        return Qt::NoItemFlags;
}


void VProperty::setValue(const QVariant &value)
{
    d_ptr->VariantValue = value;
    d_ptr->VariantValue.convert(d_ptr->PropertyVariantType);
    if (d_ptr->editor != nullptr)
    {
        setEditorData(d_ptr->editor);
    }
}

QVariant VProperty::getValue() const
{
    return d_ptr->VariantValue;
}

QString VProperty::serialize() const
{
    return getValue().toString();
}

void VProperty::deserialize(const QString& value)
{
    setValue(QVariant(value));
}


void VProperty::setName(const QString& name)
{
    d_ptr->Name = name;
}


QString VProperty::getName() const
{
    return d_ptr->Name;
}


void VProperty::setDescription(const QString& desc)
{
    d_ptr->Description = desc;
}


QString VProperty::getDescription() const
{
    return d_ptr->Description;
}

//! Returns a reference to the list of children
QList<VProperty*>& VProperty::getChildren()
{
    return d_ptr->Children;
}

//! Returns a reference to the list of children
const QList<VProperty*>& VProperty::getChildren() const
{
    return d_ptr->Children;
}

//! Returns the child at a certain row
VProperty* VProperty::getChild(int row) const
{
    if (row >= 0 && row < getRowCount())
    {
        return d_ptr->Children.at(row);
    }
    else
        return nullptr;
}

//! Gets the number of children
int VProperty::getRowCount() const
{
   return d_ptr->Children.count();
}

//! Gets the parent of this property
VProperty* VProperty::getParent() const
{
    return d_ptr->Parent;
}

//! Sets the parent of this property
void VProperty::setParent(VProperty* parent)
{
    if (d_ptr->Parent == parent)
    {
        return;
    }

    VProperty* oldParent = d_ptr->Parent;
    d_ptr->Parent = parent;

    if (oldParent)
    {
        oldParent->removeChild(this);
    }

    if (d_ptr->Parent && d_ptr->Parent->getChildRow(this) == -1)
    {
        d_ptr->Parent->addChild(this);
    }
}

int VProperty::addChild(VProperty *child)
{
    if (child && child->getParent() != this)
    {
        child->setParent(this);
    }

    if (!d_ptr->Children.contains(child) && child != nullptr)
    {
        d_ptr->Children.push_back(child);
        return d_ptr->Children.count()-1;
    }
    else
    {
        return d_ptr->Children.indexOf(child);
    }
}

//! Removes a child from the children list
void VProperty::removeChild(VProperty* child)
{
    d_ptr->Children.removeAll(child);

    if (child && child->getParent() == this)
    {
        child->setParent(nullptr);
    }
}

//! Returns the row the child has
int VProperty::getChildRow(VProperty* child) const
{
    return d_ptr->Children.indexOf(child);
}

//! Returns whether the views have to update the parent of this property if it changes
bool VProperty::getUpdateParent() const
{
    return d_ptr->UpdateParent;
}

//! Returns whether the views have to update the children of this property if it changes
bool VProperty::getUpdateChildren() const
{
    return d_ptr->UpdateChildren;
}

//! Sets whether the views should update Parents or children after this property changes
void VProperty::setUpdateBehaviour(bool update_parent, bool update_children)
{
    d_ptr->UpdateParent = update_parent;
    d_ptr->UpdateChildren = update_children;
}


void VProperty::setSettings(const QMap<QString, QVariant>& settings)
{
    QMap<QString, QVariant>::const_iterator tmpIterator = settings.constBegin();
    for (; tmpIterator != settings.constEnd(); ++tmpIterator)
    {
        setSetting(tmpIterator.key(), tmpIterator.value());
    }
}

QMap<QString, QVariant> VProperty::getSettings() const
{
    QMap<QString, QVariant> tmpResult;

    QStringList tmpKeyList = getSettingKeys();
    foreach(const QString& tmpKey, tmpKeyList)
        tmpResult.insert(tmpKey, getSetting(tmpKey));

    return tmpResult;
}

void VProperty::setSetting(const QString& key, const QVariant& value)
{
    Q_UNUSED(key)
    Q_UNUSED(value)
    // Not needed in the Standard property
}

QVariant VProperty::getSetting(const QString& key) const
{
    // Not needed in the Standard property
    Q_UNUSED(key)
    return QVariant();
}

QStringList VProperty::getSettingKeys() const
{
    return QStringList();
}

VProperty* VProperty::clone(bool include_children, VProperty* container) const
{
    if (!container)
    {
        container = new VProperty(getName(), d_ptr->PropertyVariantType);
    }

    container->setName(getName());
    container->setDescription(getDescription());
    container->setValue(getValue());
    container->setSettings(getSettings());
    container->setUpdateBehaviour(getUpdateParent(), getUpdateChildren());
    container->setPropertyType(propertyType());

    if (include_children)
    {
        foreach(VProperty* tmpChild, d_ptr->Children)
            container->addChild(tmpChild->clone(true));
    }

    return container;
}

Property VProperty::propertyType() const
{
    return d_ptr->type;
}

void VProperty::setPropertyType(const Property &type)
{
    d_ptr->type = type;
}

void VProperty::UpdateParent(const QVariant &value)
{
    Q_UNUSED(value);
}

void VProperty::ValueChildChanged(const QVariant &value, int typeForParent)
{
    Q_UNUSED(value)
    Q_UNUSED(typeForParent)
}

UserChangeEvent::~UserChangeEvent()
{}

VPE::VPropertyPrivate::~VPropertyPrivate()
{}
