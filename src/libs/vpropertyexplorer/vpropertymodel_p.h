#ifndef VPROPERTYMODEL_P_H
#define VPROPERTYMODEL_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>
#include <QObject>

namespace VPE {

class VProperty;
class VPropertySet;

class VPropertyModelPrivate {
public:
    //! The property set holding the properties
    VPropertySet* Properties;

    //! The header data for the property name column
    QString HeadlineProperty;

    //! The header data for the value column
    QString HeadlineValue;

    //! Constructor
    VPropertyModelPrivate()
        : Properties(nullptr)
    {
        //: The text that appears in the first column header
        HeadlineProperty = QObject::tr("Property");

        //: The text that appears in the second column header
        HeadlineValue = QObject::tr("Value");
    }
};

}

#endif // VPROPERTYMODEL_P_H
