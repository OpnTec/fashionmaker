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

void VContainer::UpdateSpline(qint64 id, const VSpline &spl){
    splines[id] = spl;
    if(id > _id){
        _id = id;
    }
}

void VContainer::UpdateArc(qint64 id, const VArc &arc){
    arcs[id] = arc;
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
    splines.clear();
    arcs.clear();
    lengthArcs.clear();
}

void VContainer::ClearIncrementTable(){
    incrementTable.clear();
}

void VContainer::ClearLengthLines(){
    lengthLines.clear();
}

void VContainer::ClearLengthSplines(){
    lengthSplines.clear();
}

void VContainer::ClearLengthArcs(){
    lengthArcs.clear();
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

const QMap<qint64, VSpline> *VContainer::DataSplines() const{
    return &splines;
}

const QMap<qint64, VArc> *VContainer::DataArcs() const{
    return &arcs;
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

const QMap<QString, qreal> *VContainer::DataLengthSplines() const{
    return &lengthSplines;
}

void VContainer::AddLine(const qint64 &firstPointId, const qint64 &secondPointId){
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    VPointF firstPoint = GetPoint(firstPointId);
    VPointF secondPoint = GetPoint(secondPointId);
    AddLengthLine(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}

qint64 VContainer::AddSpline(const VSpline &spl){
    qint64 id = getNextId();
    splines[id] = spl;
    return id;
}

qint64 VContainer::AddArc(const VArc &arc){
    qint64 id = getNextId();
    arcs[id] = arc;
    return id;
}

QString VContainer::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("Line_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("Spl_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameArc(const qint64 &firstPoint, const qint64 &centerPoint,
                               const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF center = GetPoint(centerPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("Arc_%1_%2_%3").arg(first.name(), center.name(), second.name());
}

void VContainer::AddLengthLine(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthLines[name] = value/PrintDPI*25.4;
}

void VContainer::AddLengthSpline(const qint64 &firstPointId, const qint64 &secondPointId){
    QString nameLine = GetNameSpline(firstPointId, secondPointId);
    VPointF firstPoint = GetPoint(firstPointId);
    VPointF secondPoint = GetPoint(secondPointId);
    AddLengthSpline(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}

void VContainer::AddLengthSpline(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthSplines[name] = value/PrintDPI*25.4;
}

void VContainer::AddLengthArc(const qint64 &firstPointId, const qint64 &centerPoint,
                              const qint64 &secondPointId){
    QString nameLine = GetNameArc(firstPointId, centerPoint, secondPointId);
    VPointF firstPoint = GetPoint(firstPointId);
    VPointF secondPoint = GetPoint(secondPointId);
    AddLengthArc(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}

void VContainer::AddLengthArc(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthArcs[name] = value/PrintDPI*25.4;
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

VSpline VContainer::GetSpline(qint64 id) const{
    if(splines.contains(id)){
        return splines.value(id);
    } else {
        qCritical()<<"Не можу знайти id = "<<id<<" в таблиці.";
        throw"Не можу знайти сплайн за id.";
    }
    return VSpline();
}

VArc VContainer::GetArc(qint64 id) const{
    if(arcs.contains(id)){
        return arcs.value(id);
    } else {
        qCritical()<<"Не можу знайти id = "<<id<<" в таблиці.";
        throw"Не можу знайти дугу за id.";
    }
    return VArc();
}


const QMap<QString, qreal> *VContainer::DataLengthArcs() const{
    return &lengthArcs;
}
