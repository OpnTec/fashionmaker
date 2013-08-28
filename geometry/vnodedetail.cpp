#include "vnodedetail.h"

VNodeDetail::VNodeDetail():id(0), typeTool(Scene::Point), mode(Draw::Calculation){
}

VNodeDetail::VNodeDetail(qint64 id, Scene::Type typeTool, Draw::Mode mode):id(id), typeTool(typeTool),
mode(mode){
}

VNodeDetail::VNodeDetail(const VNodeDetail &node):id(node.getId()), typeTool(node.getTypeTool()),
    mode(node.getMode()){
}

Scene::Type VNodeDetail::getTypeTool() const{
    return typeTool;
}

void VNodeDetail::setTypeTool(const Scene::Type &value){
    typeTool = value;
}

qint64 VNodeDetail::getId() const{
    return id;
}

void VNodeDetail::setId(const qint64 &value){
    id = value;
}

Draw::Mode VNodeDetail::getMode() const{
    return mode;
}

void VNodeDetail::setMode(const Draw::Mode &value){
    mode = value;
}
