#ifndef VPROPERTYTREEVIEW_P_H
#define VPROPERTYTREEVIEW_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE {

class VPropertyDelegate;

class VPropertyTreeViewPrivate
{
public:
    //! Property Delegate
    VPropertyDelegate* PropertyDelegate;

    //! Constructor
    VPropertyTreeViewPrivate(VPropertyDelegate* delegate)
        : PropertyDelegate(delegate) {}

    //! Constructor
    VPropertyTreeViewPrivate()
        : PropertyDelegate(nullptr) {}
};

}

#endif // VPROPERTYTREEVIEW_P_H
