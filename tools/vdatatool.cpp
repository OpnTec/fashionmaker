#pragma GCC diagnostic ignored "-Weffc++"
#include "vdatatool.h"
#pragma GCC diagnostic warning "-Weffc++"

VDataTool::VDataTool(VContainer *data, QObject *parent) :
    QObject(parent){
    this->data = *data;
}

VDataTool::~VDataTool(){
}

const VDataTool &VDataTool::operator =(const VDataTool &tool){
   data = tool.getData();
}

VContainer VDataTool::getData() const{
    return data;
}

void VDataTool::setData(const VContainer *value){
    data = *value;
}
