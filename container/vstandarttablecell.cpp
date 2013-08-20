#include "vstandarttablecell.h"

VStandartTableCell::VStandartTableCell():base(0), ksize(0), kgrowth(0), description(QString()){
}

VStandartTableCell::VStandartTableCell(qint32 base, qreal ksize, qreal kgrowth, QString description):base(base),
    ksize(ksize), kgrowth(kgrowth), description(description){
}

qint32 VStandartTableCell::GetBase() const{
    return base;
}

qreal VStandartTableCell::GetKsize() const{
    return ksize;
}

qreal VStandartTableCell::GetKgrowth() const{
    return kgrowth;
}

QString VStandartTableCell::GetDescription() const{
    return description;
}
