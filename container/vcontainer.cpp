#include "vcontainer.h"
#include <QDebug>

#include "../options.h"

VContainer::VContainer(){
    _id = 0;
    SetSize(500);
    SetGrowth(1760);
}

VPointF VContainer::GetPoint(qint64 id) const{
    if(points.contains(id)){
        return points.value(id);
    } else {
        qCritical()<<"Не можу знайти id = "<<id<<" в таблиці.";
        throw"Не можу знайти точку за id.";
    }
    return VPointF();
}

VStandartTableCell VContainer::GetStandartTableCell(const QString &name) const{
    if(standartTable.contains(name)){
        return standartTable.value(name);
    } else {
        qCritical()<<"Не можу знайти змінну за імям = "<<name<<" в таблиці.";
        throw"Не можу знайти змінну в стандартній таблиці вимірів за ім'ям.";
    }
    return VStandartTableCell();
}

VIncrementTableRow VContainer::GetIncrementTableRow(const QString& name) const{
    if(incrementTable.contains(name)){
        return incrementTable.value(name);
    } else {
        qCritical()<<"Не можу знайти змінну за імям = "<<name<<" в таблиці.";
        throw"Не можу знайти змінну в таблиці прибавок за ім'ям.";
    }
    return VIncrementTableRow();
}

qint64 VContainer::AddPoint(const VPointF& point){
    qint64 id = getNextId();
    points[id] = point;
    return id;
}

void VContainer::AddStandartTableCell(const QString& name, const VStandartTableCell& cell){
    standartTable[name] = cell;
}

void VContainer::AddIncrementTableRow(const QString& name, const VIncrementTableRow& cell){
    incrementTable[name] = cell;
}

qint64 VContainer::getId(){
    return _id;
}

qint64 VContainer::getNextId(){
    ++_id;
    return _id;
}

void VContainer::RemoveIncrementTableRow(const QString& name){
    incrementTable.remove(name);
}

void VContainer::UpdatePoint(qint64 id, const VPointF& point){
    points[id] = point;
    if(id > _id){
        _id = id;
    }
}

void VContainer::UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell){
    standartTable[name] = cell;
}

void VContainer::UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell){
    incrementTable[name] = cell;
}

qreal VContainer::GetValueStandartTableCell(const QString& name) const{
    VStandartTableCell cell =  GetStandartTableCell(name);
    qreal k_size 	= ( ( qreal ) (size()/10) - 50.0 ) / 2;
    qreal k_growth  = ( ( qreal ) (growth()/10) - 176.0 ) / 6;
    qreal value = cell.GetBase() + k_size*cell.GetKsize() 	+ k_growth*cell.GetKgrowth();
    return value;
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const{
    VIncrementTableRow cell =  GetIncrementTableRow(name);
    qreal k_size 	= ( ( qreal ) (size()/10) - 50.0 ) / 2;
    qreal k_growth  = ( ( qreal ) (growth()/10) - 176.0 ) / 6;
    qreal value = cell.getBase() + k_size*cell.getKsize() + k_growth*cell.getKgrowth();
    return value;
}

void VContainer::Clear(){
    _id = 0;
    points.clear();
    standartTable.clear();
    incrementTable.clear();
    lengthLines.clear();
}

void VContainer::ClearIncrementTable(){
    incrementTable.clear();
}

void VContainer::ClearLengthLines(){
    lengthLines.clear();
}

void VContainer::SetSize(qint32 size){
    base["Сг"] = size;
}

void VContainer::SetGrowth(qint32 growth){
    base["Р"] = growth;
}

qint32 VContainer::size() const{
    return base.value("Сг");
}

qint32 VContainer::growth() const{
    return base.value("Р");
}

bool VContainer::IncrementTableContains(const QString& name){
    return incrementTable.contains(name);
}

qreal VContainer::FindVar(const QString &name, bool *ok)const{
    if(base.contains(name)){
        *ok = true;
        return base.value(name);
    }

    if(standartTable.contains(name)){
        *ok = true;
        return GetValueStandartTableCell(name);
    }
    if(incrementTable.contains(name)){
        *ok = true;
        return GetValueIncrementTableRow(name);
    }
    if(lengthLines.contains(name)){
        *ok = true;
        return lengthLines.value(name);
    }
    *ok = false;
    return 0;
}

const QMap<qint64, VPointF> *VContainer::DataPoints() const{
    return &points;
}

const QMap<QString, qint32> *VContainer::DataBase() const{
    return &base;
}

const QMap<QString, VStandartTableCell> *VContainer::DataStandartTable() const{
    return &standartTable;
}

const QMap<QString, VIncrementTableRow> *VContainer::DataIncrementTable() const{
    return &incrementTable;
}

const QMap<QString, qreal> *VContainer::DataLengthLines() const{
    return &lengthLines;
}

void VContainer::AddLine(const qint64 &firstPointId, const qint64 &secondPointId){
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    VPointF firstPoint = GetPoint(firstPointId);
    VPointF secondPoint = GetPoint(secondPointId);
    AddLine(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}

QString VContainer::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("Line_%1_%2").arg(first.name(), second.name());
}

void VContainer::AddLine(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthLines[name] = value/PrintDPI*25.4;
}

qreal VContainer::GetLine(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    if(lengthLines.contains(name)){
        return lengthLines.value(name);
    } else {
        qCritical()<<"Не можу знайти лінію за імям = "<<name<<" в таблиці.";
        throw"Не можу знайти лінію таблиці.";
    }
    return 0;
}
