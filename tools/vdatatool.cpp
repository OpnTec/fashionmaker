#include "vdatatool.h"

VDataTool::VDataTool(VContainer *data, QObject *parent) :
    QObject(parent), data(*data){
}

VDataTool::~VDataTool(){
}

VDataTool &VDataTool::operator =(const VDataTool &tool){
   data = tool.getData();
   return *this;
}

VContainer VDataTool::getData() const{
    return data;
}

void VDataTool::setData(const VContainer *value){
    data = *value;
}
