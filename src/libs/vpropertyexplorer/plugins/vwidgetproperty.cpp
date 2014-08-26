#include "vwidgetproperty.h"


#include "vwidgetproperty_p.h"

using namespace VPE;




VWidgetProperty::VWidgetProperty(const QString& name, QWidget* widget)
    : VEmptyProperty(new VWidgetPropertyPrivate(name, QVariant::Invalid, widget))
{
}

VWidgetProperty::~VWidgetProperty()
{
    //
}

QWidget *VWidgetProperty::getWidget() const
{
    return static_cast<VWidgetPropertyPrivate*>(d_ptr)->Widget.data();
}

void VWidgetProperty::setWidget(QWidget* widget)
{
    VWidgetPropertyPrivate* tmpDPtr = static_cast<VWidgetPropertyPrivate*>(d_ptr);
    QWidget* tmpOldWidget = tmpDPtr->Widget.data();
    if(tmpOldWidget)
        tmpOldWidget->deleteLater();

    tmpDPtr->Widget = widget;
}


QString VWidgetProperty::type() const
{
    return "widget";
}

VProperty* VWidgetProperty::clone(bool include_children, VProperty* container) const
{
    // todo: This is a tricky one to clone... don't know what would be the best way to do so... Maybe serialize the widget somehow?
    return VProperty::clone(include_children, container ? container : new VWidgetProperty(getName()));
}
