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

#include <QByteArray>
#include <QFlags>
#include <QForeachContainer>
#include <QItemEditorFactory>
#include <QLineEdit>
#include <QList>
#include <QMetaProperty>
#include <QObject>
#include <QStandardItemEditorCreator>
#include <QWidget>

#include "vproperty_p.h"

//! Standard constructor, takes a name and a parent property as argument
VPE::VProperty::VProperty(const QString& name, QVariant::Type type)
    : QObject(), d_ptr(new VPropertyPrivate(name, type))
{

}

VPE::VProperty::VProperty(VPropertyPrivate *d)
    : d_ptr(d)
{
}


VPE::VProperty::~VProperty()
{
    setParent(nullptr);

    while (!d_ptr->Children.isEmpty())
    {
        VProperty* tmpChild = d_ptr->Children.takeLast();
        delete tmpChild;
    }

    delete d_ptr;
}

QString VPE::VProperty::type() const
{
    return "string";
}

//! Get the data how it should be displayed
QVariant VPE::VProperty::data (int column, int role) const
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

bool VPE::VProperty::setData(const QVariant &data, int role)
{
    bool tmpResult = false;
    if (Qt::EditRole == role)
    {
        tmpResult = (d_ptr->VariantValue != data);
        setValue(data);
    }

    return tmpResult;
}

bool VPE::VProperty::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,
                           const QAbstractItemDelegate *delegate) const
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(index)
    Q_UNUSED(delegate)

    return false;
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VPE::VProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                      const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *lineCreator = new QStandardItemEditorCreator<QLineEdit>();
    factory->registerEditor(QVariant::String, lineCreator);
    QItemEditorFactory::setDefaultFactory(factory);

    d_ptr->editor = factory->createEditor(static_cast<int>(d_ptr->PropertyVariantType), parent);
    return d_ptr->editor;
}

bool VPE::VProperty::setEditorData(QWidget* editor)
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
QVariant VPE::VProperty::getEditorData(const QWidget* editor) const
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
Qt::ItemFlags VPE::VProperty::flags(int column) const
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


void VPE::VProperty::setValue(const QVariant &value)
{
    d_ptr->VariantValue = value;
    d_ptr->VariantValue.convert(static_cast<int>(d_ptr->PropertyVariantType));
    if (d_ptr->editor != nullptr)
    {
        setEditorData(d_ptr->editor);
    }
}

QVariant VPE::VProperty::getValue() const
{
    return d_ptr->VariantValue;
}

// cppcheck-suppress unusedFunction
QString VPE::VProperty::serialize() const
{
    return getValue().toString();
}

void VPE::VProperty::deserialize(const QString& value)
{
    setValue(QVariant(value));
}


void VPE::VProperty::setName(const QString& name)
{
    d_ptr->Name = name;
}


QString VPE::VProperty::getName() const
{
    return d_ptr->Name;
}


void VPE::VProperty::setDescription(const QString& desc)
{
    d_ptr->Description = desc;
}


QString VPE::VProperty::getDescription() const
{
    return d_ptr->Description;
}

//! Returns a reference to the list of children
QList<VPE::VProperty*>& VPE::VProperty::getChildren()
{
    return d_ptr->Children;
}

//! Returns a reference to the list of children
const QList<VPE::VProperty*>& VPE::VProperty::getChildren() const
{
    return d_ptr->Children;
}

//! Returns the child at a certain row
VPE::VProperty* VPE::VProperty::getChild(int row) const
{
    if (row >= 0 && row < getRowCount())
    {
        return d_ptr->Children.at(row);
    }
    else
        return nullptr;
}

//! Gets the number of children
int VPE::VProperty::getRowCount() const
{
   return d_ptr->Children.count();
}

//! Gets the parent of this property
VPE::VProperty* VPE::VProperty::getParent() const
{
    return d_ptr->Parent;
}

//! Sets the parent of this property
void VPE::VProperty::setParent(VProperty* parent)
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

int VPE::VProperty::addChild(VProperty *child)
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
void VPE::VProperty::removeChild(VProperty* child)
{
    d_ptr->Children.removeAll(child);

    if (child && child->getParent() == this)
    {
        child->setParent(nullptr);
    }
}

//! Returns the row the child has
int VPE::VProperty::getChildRow(VProperty* child) const
{
    return d_ptr->Children.indexOf(child);
}

//! Returns whether the views have to update the parent of this property if it changes
bool VPE::VProperty::getUpdateParent() const
{
    return d_ptr->UpdateParent;
}

//! Returns whether the views have to update the children of this property if it changes
bool VPE::VProperty::getUpdateChildren() const
{
    return d_ptr->UpdateChildren;
}

//! Sets whether the views should update Parents or children after this property changes
void VPE::VProperty::setUpdateBehaviour(bool update_parent, bool update_children)
{
    d_ptr->UpdateParent = update_parent;
    d_ptr->UpdateChildren = update_children;
}


void VPE::VProperty::setSettings(const QMap<QString, QVariant>& settings)
{
    QMap<QString, QVariant>::const_iterator tmpIterator = settings.constBegin();
    for (; tmpIterator != settings.constEnd(); ++tmpIterator)
    {
        setSetting(tmpIterator.key(), tmpIterator.value());
    }
}

QMap<QString, QVariant> VPE::VProperty::getSettings() const
{
    QMap<QString, QVariant> tmpResult;

    QStringList tmpKeyList = getSettingKeys();
    foreach(const QString& tmpKey, tmpKeyList)
        tmpResult.insert(tmpKey, getSetting(tmpKey));

    return tmpResult;
}

void VPE::VProperty::setSetting(const QString& key, const QVariant& value)
{
    Q_UNUSED(key)
    Q_UNUSED(value)
    // Not needed in the Standard property
}

QVariant VPE::VProperty::getSetting(const QString& key) const
{
    // Not needed in the Standard property
    Q_UNUSED(key)
    return QVariant();
}

QStringList VPE::VProperty::getSettingKeys() const
{
    return QStringList();
}

VPE::VProperty* VPE::VProperty::clone(bool include_children, VProperty* container) const
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

VPE::Property VPE::VProperty::propertyType() const
{
    return d_ptr->type;
}

void VPE::VProperty::setPropertyType(const Property &type)
{
    d_ptr->type = type;
}

void VPE::VProperty::UpdateParent(const QVariant &value)
{
    Q_UNUSED(value)
}

void VPE::VProperty::ValueChildChanged(const QVariant &value, int typeForParent)
{
    Q_UNUSED(value)
    Q_UNUSED(typeForParent)
}

VPE::UserChangeEvent::~UserChangeEvent()
{}

VPE::VPropertyPrivate::~VPropertyPrivate()
{}
