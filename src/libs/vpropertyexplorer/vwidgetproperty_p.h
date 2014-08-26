#ifndef VWIDGETPROPERTY_P_H
#define VWIDGETPROPERTY_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vproperty_p.h"

#include <QPointer>
#include <QWidget>

namespace VPE {

class VWidgetPropertyPrivate : public VPropertyPrivate {
public:
    //! The widget to show
    QPointer<QWidget> Widget;

    //! Constructor passing name and type
    VWidgetPropertyPrivate(const QString& name, QVariant::Type type, QWidget* widget = nullptr)
        : VPropertyPrivate(name, type), Widget(widget) {}

    //! Constructor
    VWidgetPropertyPrivate()
        : VPropertyPrivate(), Widget(nullptr) {}

    //! Destructor
    ~VWidgetPropertyPrivate() {
        if(Widget)
            Widget->deleteLater();
    }
};

}

#endif // VWIDGETPROPERTY_P_H
