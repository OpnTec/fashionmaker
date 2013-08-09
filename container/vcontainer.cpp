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

void VContainer::UpdateSplinePath(qint64 id, const VSplinePath &splPath){
    splinePaths[id] = splPath;
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
    lineArcs.clear();
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

void VContainer::ClearLineArcs(){
    lineArcs.clear();
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
    if(lengthArcs.contains(name)){
        *ok = true;
        return lengthArcs.value(name);
    }
    if(lineArcs.contains(name)){
        *ok = true;
        return lineArcs.value(name);
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

qint64 VContainer::AddSplinePath(const VSplinePath &splPath){
    qint64 id = getNextId();
    splinePaths[id] = splPath;
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

QString VContainer::GetNameLineArc(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("ArcLine_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);
    return QString("Spl_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameSplinePath(const VSplinePath &path) const{
    if(path.Count() == 0){
        return QString();
    }
    QString name("SplPath");
    for(qint32 i = 1; i <= path.Count(); ++i){
        VSpline spl = path.GetSpline(i);
        VPointF first = GetPoint(spl.GetP1());
        VPointF second = GetPoint(spl.GetP4());
        QString splName = QString("_%1_%2").arg(first.name(), second.name());
        name.append(splName);
    }
    return name;
}

QString VContainer::GetNameArc(const qint64 &center, const qint64 &id) const{
    VPointF centerPoint = GetPoint(center);
    return QString ("Arc(%1)%2").arg(centerPoint.name(), id);
}

void VContainer::AddLengthLine(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthLines[name] = value;
}

void VContainer::AddLengthSpline(const qint64 &firstPointId, const qint64 &secondPointId){
    QString nameLine = GetNameSpline(firstPointId, secondPointId);
    VPointF firstPoint = GetPoint(firstPointId);
    VPointF secondPoint = GetPoint(secondPointId);
    AddLengthSpline(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}

void VContainer::AddLengthSpline(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthSplines[name] = value;
}

void VContainer::AddLengthArc(const qint64 &center, const qint64 &id){
    AddLengthArc(GetNameArc(center, id), GetArc(id).GetLength());
}

void VContainer::AddLengthArc(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthArcs[name] = value;
}

void VContainer::AddLineArc(const QString &name, const qint32 &value){
    Q_ASSERT(!name.isEmpty());
    lineArcs[name] = value;
}

qreal VContainer::GetLine(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    if(lengthLines.contains(name)){
        return lengthLines.value(name);
    } else {
        qCritical()<<"Не можу знайти лінію за імям = "<<name<<" в таблиці.";
        throw"Не можу знайти лінію в таблиці.";
    }
    return 0;
}

qint32 VContainer::GetLineArc(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    if(lineArcs.contains(name)){
        return lineArcs.value(name);
    } else {
        qCritical()<<"Не можу знайти кут за імям = "<<name<<" в таблиці.";
        throw"Не можу знайти кут в таблиці.";
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

VSplinePath VContainer::GetSplinePath(qint64 id) const{
    if(splinePaths.contains(id)){
        return splinePaths.value(id);
    } else {
        qCritical()<<"Не можу знайти id = "<<id<<" в таблиці.";
        throw"Не можу знайти дугу за id.";
    }
    return VSplinePath();
}


const QMap<QString, qreal> *VContainer::DataLengthArcs() const{
    return &lengthArcs;
}

const QMap<QString, qreal> *VContainer::DataLineArcs() const{
    return &lineArcs;
}

const QMap<qint64, VSplinePath> *VContainer::DataSplinePaths() const{
    return &splinePaths;
}
