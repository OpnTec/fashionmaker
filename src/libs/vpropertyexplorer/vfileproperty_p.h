#ifndef VFILEPROPERTY_P_H
#define VFILEPROPERTY_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vproperty_p.h"

namespace VPE {

class VFilePropertyPrivate : public VPropertyPrivate {
public:
    //! File filters
    QString FileFilters;

    //! Determines whether the file property is a file or a directory. Default: false
    bool Directory;


    //! Constructor passing name and type
    VFilePropertyPrivate(const QString& name, QVariant::Type type, bool directory = false)
        : VPropertyPrivate(name, type), FileFilters(), Directory(directory) {}

    //! Constructor
    VFilePropertyPrivate()
        : VPropertyPrivate(), FileFilters(), Directory(false) {}
};

}

#endif // VFILEPROPERTY_P_H
