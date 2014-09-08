#ifndef VPROPERTYSET_P_H
#define VPROPERTYSET_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE {

class VProperty;

class VPropertySetPrivate {
public:
    //! Property map (ID, Property)
    QMap<QString, VProperty*> Properties; // All the Properties managed by this model are being stored in this map for quick access

    //! List containing the root properties
    QList<VProperty*> RootProperties;

    //! Constructor
    VPropertySetPrivate()
    {
    }
};

}

#endif // VPROPERTYMODEL_P_H
