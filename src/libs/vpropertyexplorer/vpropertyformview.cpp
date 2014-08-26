#include "vpropertyformview.h"
#include "vpropertymodel.h"
#include "vpropertyset.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QShowEvent>

#include "vpropertyformview_p.h"

using namespace VPE;

VPropertyFormView::VPropertyFormView(QWidget* parent)
    : VPropertyFormWidget(new VPropertyFormViewPrivate(), parent)
{
    //
}

VPropertyFormView::VPropertyFormView(VPropertyModel* model, QWidget *parent)
    : VPropertyFormWidget(new VPropertyFormViewPrivate(), parent)
{
    setModel(model);
}

VPropertyFormView::VPropertyFormView(VPropertySet* property_set, QWidget *parent)
    : VPropertyFormWidget(new VPropertyFormViewPrivate(), parent)
{
    setPropertySet(property_set);
}

VPropertyFormView::~VPropertyFormView()
{
    // Nothing to do
}

void VPropertyFormView::build()
{
    VPropertyFormWidget::build();

    // Go through all sub widgets and connect
    connectPropertyFormWidget(this);
}

void VPropertyFormView::setModel(VPropertyModel *model)
{
    // Remove old model or set
    removeModelAndSet();

    // Set model
    static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model = model;
    if(model) {
        // Set the property list
        if(model->getPropertySet())
            d_ptr->Properties = model->getPropertySet()->getRootProperties();

        // Connect signals // todo: more signals neccesary!!!
        connect(model, SIGNAL(destroyed()), this, SLOT(modelDestroyed()));
        connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
        connect(model, SIGNAL(modelReset()), this, SLOT(modelReset()));
        connect(model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
    }

    // Build the widget
    updatePropertyList();
}

void VPropertyFormView::setPropertySet(VPropertySet* property_set)
{
    // Remove old model or set
    removeModelAndSet();

    // Set property set
    static_cast<VPropertyFormViewPrivate*>(d_ptr)->PropertySet = property_set;
    if(property_set) {
        // Set the property list
        d_ptr->Properties = property_set->getRootProperties();
    }

    // Build the widget
    updatePropertyList();
}

void VPropertyFormView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    // todo: Only rebuild the neccessary parts
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
    updatePropertyList();
}

void VPropertyFormView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    // todo: Only rebuild the neccessary parts
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
    updatePropertyList();
}

void VPropertyFormView::modelReset()
{
    updatePropertyList();
}

void VPropertyFormView::modelDestroyed()
{
    removeModelAndSet();
    updatePropertyList();
}

void VPropertyFormView::dataChanged(const QModelIndex &top_left, const QModelIndex &bottom_right)
{
    if(static_cast<VPropertyFormViewPrivate*>(d_ptr)->IgnoreDataChangedSignal)
        return;
    // todo: handle data changes
}

void VPropertyFormView::dataSubmitted(VProperty *property)
{
    VPropertyModel* tmpModel = static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model;

    if(tmpModel && d_ptr->UpdateEditors) {
        static_cast<VPropertyFormViewPrivate*>(d_ptr)->IgnoreDataChangedSignal = true;
        //tmpModel->onDataChangedByModel(property);
        tmpModel->showDataChangedByEditor(property);

        static_cast<VPropertyFormViewPrivate*>(d_ptr)->IgnoreDataChangedSignal = false;
    }
}

void VPropertyFormView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(static_cast<VPropertyFormViewPrivate*>(d_ptr)->NeedsRebuild)
        build();
    static_cast<VPropertyFormViewPrivate*>(d_ptr)->NeedsRebuild = false;
}

void VPropertyFormView::updatePropertyList()
{
    VPropertyModel* tmpModel = static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model;
    VPropertySet* tmpSet = static_cast<VPropertyFormViewPrivate*>(d_ptr)->PropertySet;

    if(tmpModel && tmpModel->getPropertySet())
        d_ptr->Properties = tmpModel->getPropertySet()->getRootProperties();
    else if(tmpSet)
        d_ptr->Properties = tmpSet->getRootProperties();
    else
        d_ptr->Properties.clear();

    if(isVisible())
        build();
    else
        static_cast<VPropertyFormViewPrivate*>(d_ptr)->NeedsRebuild = true;
}

void VPropertyFormView::removeModelAndSet()
{
    if(static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model) {
        disconnect(static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model, 0, this, 0);
        static_cast<VPropertyFormViewPrivate*>(d_ptr)->Model = nullptr;
    }

    static_cast<VPropertyFormViewPrivate*>(d_ptr)->NeedsRebuild = true;
    d_ptr->Properties.clear();
    static_cast<VPropertyFormViewPrivate*>(d_ptr)->PropertySet = nullptr;
}

void VPropertyFormView::connectPropertyFormWidget(VPropertyFormWidget *widget)
{
    if(!widget)
        return;

    connect(widget, SIGNAL(propertyDataSubmitted(VProperty*)), this, SLOT(dataSubmitted(VProperty*)));
    QList<VPropertyFormWidget*> tmpList = widget->getChildPropertyFormWidgets();

    foreach(VPropertyFormWidget* tmpEditorWidget, tmpList) {
        connectPropertyFormWidget(tmpEditorWidget);
    }
}
