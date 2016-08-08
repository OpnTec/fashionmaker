/************************************************************************
 **
 **  @file   vemptyproperty.h
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

#ifndef VEMPTYPROPERTY_H
#define VEMPTYPROPERTY_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QStyleOptionViewItem>
#include <QVariant>
#include <Qt>
#include <QtGlobal>

#include "../vproperty.h"
#include "../vpropertyexplorer_global.h"

class QAbstractItemDelegate;
class QStyleOptionViewItem;
class QWidget;
namespace VPE {
class VPropertyPrivate;
}  // namespace VPE

namespace VPE
{

class VPROPERTYEXPLORERSHARED_EXPORT VEmptyProperty : public VProperty
{
    Q_OBJECT
public:
    //! Standard constructor, takes a name and a parent property as argument
    explicit VEmptyProperty(const QString& name);

    //! Destructor
    virtual ~VEmptyProperty() Q_DECL_OVERRIDE;

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and
    //! slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                  const QAbstractItemDelegate* delegate) Q_DECL_OVERRIDE;

    //! Gets the data from the widget
    virtual QVariant getEditorData(const QWidget* editor) const Q_DECL_OVERRIDE;

    //! Returns item flags
    virtual Qt::ItemFlags flags(int column = DPC_Name) const Q_DECL_OVERRIDE;

    //! Returns a string containing the type of the property
    virtual QString type() const Q_DECL_OVERRIDE;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true,
                             VProperty* container = nullptr) const Q_DECL_OVERRIDE Q_REQUIRED_RESULT;

protected:
    //! Protected constructor
    explicit VEmptyProperty(VPropertyPrivate* d);

private:
    Q_DISABLE_COPY(VEmptyProperty)
};

}

#endif // VEMPTYPROPERTY_H
