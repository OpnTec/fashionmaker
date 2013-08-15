#include "vtoolrecord.h"

VToolRecord::VToolRecord():id(0), typeTool(Tools::ArrowTool), nameDraw(QString()){
}

VToolRecord::VToolRecord(const qint64 &id, const Tools::Enum &typeTool, const QString &nameDraw):id(id),
    typeTool(typeTool), nameDraw(nameDraw){
}

QString VToolRecord::getNameDraw() const{
    return nameDraw;
}

void VToolRecord::setNameDraw(const QString &value){
    nameDraw = value;
}

Tools::Enum VToolRecord::getTypeTool() const{
    return typeTool;
}

void VToolRecord::setTypeTool(const Tools::Enum &value){
    typeTool = value;
}

qint64 VToolRecord::getId() const{
    return id;
}

void VToolRecord::setId(const qint64 &value){
    id = value;
}
