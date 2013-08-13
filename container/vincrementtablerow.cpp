#include "vincrementtablerow.h"

VIncrementTableRow::VIncrementTableRow(){
    this->id = 0;
    this->base = 0;
    this->ksize = 0;
    this->kgrowth = 0;
    this->description = QString();
}

VIncrementTableRow::VIncrementTableRow(qint64 id, qreal base, qreal ksize, qreal kgrowth,
                                         QString description){
    this->id = id;
    this->base = base;
    this->ksize = ksize;
    this->kgrowth = kgrowth;
    this->description = description;
}

QString VIncrementTableRow::getDescription() const{
    return description;
}

void VIncrementTableRow::setDescription(const QString &value){
    description = value;
}

qreal VIncrementTableRow::getKgrowth() const{
    return kgrowth;
}

void VIncrementTableRow::setKgrowth(const qreal &value){
    kgrowth = value;
}

qreal VIncrementTableRow::getKsize() const{
    return ksize;
}

void VIncrementTableRow::setKsize(const qreal &value){
    ksize = value;
}

qreal VIncrementTableRow::getBase() const{
    return base;
}

void VIncrementTableRow::setBase(const qreal &value){
    base = value;
}

qint64 VIncrementTableRow::getId() const{
    return id;
}

void VIncrementTableRow::setId(const qint64 &value){
    id = value;
}
