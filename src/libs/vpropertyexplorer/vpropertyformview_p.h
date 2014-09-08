#ifndef VPROPERTYFORMVIEW_P_H
#define VPROPERTYFORMVIEW_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vpropertyformwidget_p.h"

namespace VPE {

class VPropertyModel;
class VPropertySet;

class VPropertyFormViewPrivate : public VPropertyFormWidgetPrivate
{
public:
    //! The current property model
    VPropertyModel* Model;

    //! The currently used property set
    VPropertySet* PropertySet;

    //! Determines whether the widget needs to be rebuild
    bool NeedsRebuild;

    //! Helper variable
    bool IgnoreDataChangedSignal;

    VPropertyFormViewPrivate()
        : VPropertyFormWidgetPrivate(), Model(NULL), PropertySet(NULL), NeedsRebuild(false) {}

    VPropertyFormViewPrivate(VPropertyModel* prop_model)
        : VPropertyFormWidgetPrivate(), Model(prop_model), PropertySet(NULL), NeedsRebuild(false) {}

    VPropertyFormViewPrivate(VPropertySet* prop_set)
        : VPropertyFormWidgetPrivate(), Model(NULL), PropertySet(prop_set), NeedsRebuild(false) {}
};

}

#endif // VPROPERTYFORMVIEW_P_H
