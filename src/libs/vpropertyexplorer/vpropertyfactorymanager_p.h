#ifndef VPROPERTYFACTORYMANAGER_P_H
#define VPROPERTYFACTORYMANAGER_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE {

class VAbstractPropertyFactory;

class VPropertyFactoryManagerPrivate {
public:
    QMap<QString, VAbstractPropertyFactory*> Factories;

};

}

#endif // VPROPERTYFACTORYMANAGER_P_H
