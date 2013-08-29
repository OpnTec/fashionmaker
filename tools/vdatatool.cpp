#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "vdatatool.h"
#pragma GCC diagnostic pop

VDataTool::VDataTool(VContainer *data, QObject *parent) :
    QObject(parent), data(*data){
}

VDataTool::~VDataTool(){
}

const VDataTool &VDataTool::operator =(const VDataTool &tool){
   data = tool.getData();
   return *this;
}

VContainer VDataTool::getData() const{
    return data;
}

void VDataTool::setData(const VContainer *value){
    data.Clear();
    data = *value;
}
