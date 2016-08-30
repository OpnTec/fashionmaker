/************************************************************************
 **
 **  @file   vstringproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 8, 2014
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

#include "vstringproperty.h"

#include <QKeyEvent>
#include <QLatin1String>
#include <QLineEdit>
#include <QLocale>
#include <QSizePolicy>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QWidget>

#include "../vproperty_p.h"

class QAbstractItemDelegate;
class QStyleOptionViewItem;

using namespace VPE;


VPE::VStringProperty::VStringProperty(const QString &name, const QMap<QString, QVariant> &settings)
    : VProperty(name, QVariant::String), readOnly(false), typeForParent(0), clearButton(false), m_osSeparator(false)
{
    VProperty::setSettings(settings);
    d_ptr->VariantValue.setValue(QString());
    d_ptr->VariantValue.convert(QVariant::String);
}

VPE::VStringProperty::VStringProperty(const QString &name)
    : VProperty(name), readOnly(false), typeForParent(0), clearButton(false), m_osSeparator(false)
{
    d_ptr->VariantValue.setValue(QString());
    d_ptr->VariantValue.convert(QVariant::String);
}

QWidget *VPE::VStringProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options,
                                            const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QLineEdit* tmpEditor = new QLineEdit(parent);
    tmpEditor->setLocale(parent->locale());
    tmpEditor->setReadOnly(readOnly);
    tmpEditor->installEventFilter(this);
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    tmpEditor->setClearButtonEnabled(clearButton);
#endif
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setText(d_ptr->VariantValue.toString());

    d_ptr->editor = tmpEditor;
    return d_ptr->editor;
}

QVariant VPE::VStringProperty::getEditorData(const QWidget *editor) const
{
    const QLineEdit* tmpEditor = qobject_cast<const QLineEdit*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->text();
    }

    return QVariant(QString());
}

void VPE::VStringProperty::setReadOnly(bool readOnly)
{
    this->readOnly = readOnly;
}

void VStringProperty::setOsSeparator(bool separator)
{
    m_osSeparator = separator;
}

void VStringProperty::setClearButtonEnable(bool value)
{
    this->clearButton = value;
}

void VPE::VStringProperty::setSetting(const QString &key, const QVariant &value)
{
    if (key == QLatin1String("ReadOnly"))
    {
        setReadOnly(value.toBool());
    }
    if (key == QLatin1String("TypeForParent"))
    {
        setTypeForParent(value.toInt());
    }
}

QVariant VPE::VStringProperty::getSetting(const QString &key) const
{
    if (key == QLatin1String("ReadOnly"))
    {
        return readOnly;
    }
    else if (key == QLatin1String("TypeForParent"))
    {
        return typeForParent;
    }
    else
        return VProperty::getSetting(key);
}

QStringList VPE::VStringProperty::getSettingKeys() const
{
    QStringList settings;
    settings << QStringLiteral("ReadOnly") << QStringLiteral("TypeForParent");
    return settings;
}

QString VPE::VStringProperty::type() const
{
    return QStringLiteral("string");
}

VPE::VProperty *VPE::VStringProperty::clone(bool include_children, VPE::VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VStringProperty(getName(), getSettings()));
}

void VStringProperty::UpdateParent(const QVariant &value)
{
    emit childChanged(value, typeForParent);
}

// cppcheck-suppress unusedFunction
int VStringProperty::getTypeForParent() const
{
    return typeForParent;
}

void VStringProperty::setTypeForParent(int value)
{
    typeForParent = value;
}

bool VStringProperty::eventFilter(QObject *object, QEvent *event)
{
    if (QLineEdit *textEdit = qobject_cast<QLineEdit *>(object))
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Period) && (keyEvent->modifiers() & Qt::KeypadModifier))
            {
                if (m_osSeparator)
                {
                    textEdit->insert(QLocale::system().decimalPoint());
                }
                else
                {
                    textEdit->insert(QLocale::c().decimalPoint());
                }
                return true;
            }
        }
    }
    else
    {
        // pass the event on to the parent class
        return VProperty::eventFilter(object, event);
    }
    return false;// pass the event to the widget
}
