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
}

void VContainer::ClearIncrementTable(){
    incrementTable.clear();
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
        return base.value(name)/PrintDPI*25.4;
    }

    if(standartTable.contains(name)){
        *ok = true;
        return GetValueStandartTableCell(name)/PrintDPI*25.4;
    }
    if(incrementTable.contains(name)){
        *ok = true;
        return GetValueIncrementTableRow(name)/PrintDPI*25.4;
    }
    *ok = false;
    return 0;
}

void VContainer::FillStandartTable(QTableWidget *table) const{
    qint32 currentRow = -1;
    QMapIterator<QString, VStandartTableCell> i(standartTable);
    while (i.hasNext()) {
        i.next();
        VStandartTableCell cell = i.value();
        currentRow++;
        table->setRowCount ( standartTable.size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        table->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(GetValueStandartTableCell(i.key())));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 1, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 2, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKsize()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 3, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKgrowth()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 4, item);

        item = new QTableWidgetItem(cell.GetDescription());
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 5, item);
    }
}

void VContainer::FillIncrementTable(QTableWidget *table) const{
    qint32 currentRow = -1;
    QMapIterator<QString, VIncrementTableRow> i(incrementTable);
    while (i.hasNext()) {
        i.next();
        VIncrementTableRow cell = i.value();
        currentRow++;
        table->setRowCount ( incrementTable.size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, cell.getId());
        table->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(GetValueIncrementTableRow(i.key())));
        item->setTextAlignment(Qt::AlignHCenter);
        // set the item non-editable (view only), and non-selectable
        Qt::ItemFlags flags = item->flags();
        flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);
        table->setItem(currentRow, 1, item);

        item = new QTableWidgetItem(QString().setNum(cell.getBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 2, item);

        item = new QTableWidgetItem(QString().setNum(cell.getKsize()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 3, item);

        item = new QTableWidgetItem(QString().setNum(cell.getKgrowth()));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 4, item);

        item = new QTableWidgetItem(cell.getDescription());
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 5, item);
    }
}
