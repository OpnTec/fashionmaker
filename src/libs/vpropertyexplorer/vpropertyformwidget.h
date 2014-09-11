/************************************************************************
 **
 **  @file   vpropertyformwidget.h
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

#ifndef VPROPERTYFORMWIDGET_H
#define VPROPERTYFORMWIDGET_H

#include <QGroupBox>
#include <QLabel>
#include "vproperty.h"

class QFormLayout;

namespace VPE
{

class VPropertyFormWidgetPrivate;
class VPropertySet;

//! Class that displays the sub properties of a property using a form layout
class VPROPERTYEXPLORERSHARED_EXPORT VPropertyFormWidget : public QGroupBox
{
    Q_OBJECT
public:
    //! Constructor
    VPropertyFormWidget(const QString& title, const QString& description, const QList<VProperty*>& properties,
                        QWidget* parent);

    //! Constructor
    VPropertyFormWidget(VProperty* parent_property, QWidget* parent);

    //! Destructor
    ~VPropertyFormWidget();


    //! Returns a list of all child property form widgets (note that indirect children will not be in the list)
    QList<VPropertyFormWidget*> getChildPropertyFormWidgets() const;

public slots:
    //! Rebuilds the whole form
    virtual void build();

    void buildEditor(VProperty *property, QFormLayout *formLayout, Property type = Property::Simple);

    //! Reads the data from the editors and commits it to the properties
    void commitData();

    //! Refills the editors with the propertie's data
    void loadData();

    //! Reads the data from row'th editor and commits it to the property
    void commitData(int row);

    //! Reads the data from row'th property
    void loadData(int row);

    //! Sets the update behaviour
    //! \param auto_commit If set to true, whenever an event like focusOut is triggered on an editor, the data will be
    //! submitted to the property.
    void setCommitBehaviour(bool auto_commit = true);

signals:
    //! Emitted whenever a property data changes
    void propertyDataSubmitted(VProperty* property);

protected:
    //! Protected Constructor
    VPropertyFormWidget(VPropertyFormWidgetPrivate* d_pointer, QWidget* parent, const QString &title = QString(),
                        const QString &description = QString());

    //! The protected data
    VPropertyFormWidgetPrivate* d_ptr;

    //! Event filter for the editor widgets
    bool eventFilter(QObject *object, QEvent *event);

    //! Commits data of an editor
    void commitData(QWidget* editor);

private:
    Q_DISABLE_COPY(VPropertyFormWidget)
};

}   // Namespace VPE

#endif // VPROPERTYFORMWIDGET_H
