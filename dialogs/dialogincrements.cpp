/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include <QHeaderView>
#include <QCloseEvent>
#include "widgets/doubledelegate.h"
#include "container/vincrementtablerow.h"
#include <QDebug>
#include "exception/vexception.h"

DialogIncrements::DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent) :
    DialogTool(data, Draw::Calculation, parent), ui(new Ui::DialogIncrements), data(data), doc(doc),
    row(0), column(0){
    ui->setupUi(this);
    InitialStandartTable();
    InitialIncrementTable();
    InitialLinesTable();
    InitialSplinesTable();
    InitialArcsTable();
    DoubleSpinBoxDelegate *doubleDelegate = new DoubleSpinBoxDelegate(ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(2, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(3, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(4, doubleDelegate);
    FillStandartTable();
    FillIncrementTable();
    FillLengthLines();
    FillLengthSplines();
    FillLengthArcs();

    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this, &DialogIncrements::cellChanged);
    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonAdd);
    connect(ui->toolButtonRemove, &QPushButton::clicked, this,
            &DialogIncrements::clickedToolButtonRemove);

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    connect(this, &DialogIncrements::haveLiteChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this,
            &DialogIncrements::FullUpdateFromFile);

    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogIncrements::DialogAccepted);
    ui->tabWidget->setCurrentIndex(0);
}

void DialogIncrements::FillStandartTable(){
    const QHash<QString, VStandartTableCell> *standartTable = data->DataStandartTable();
    qint32 currentRow = -1;
    QHashIterator<QString, VStandartTableCell> i(*standartTable);
    ui->tableWidgetStandart->setRowCount ( standartTable->size() );
    while (i.hasNext()) {
        i.next();
        VStandartTableCell cell = i.value();
        currentRow++;

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidgetStandart->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(data->GetValueStandartTableCell(i.key())));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandart->setItem(currentRow, 1, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandart->setItem(currentRow, 2, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKsize()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandart->setItem(currentRow, 3, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKgrowth()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandart->setItem(currentRow, 4, item);

        item = new QTableWidgetItem(cell.GetDescription());
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandart->setItem(currentRow, 5, item);
    }
    ui->tableWidgetStandart->resizeColumnsToContents();
    ui->tableWidgetStandart->resizeRowsToContents();
    ui->tableWidgetStandart->verticalHeader()->setDefaultSectionSize(20);
}

void DialogIncrements::FillIncrementTable(){
    const QHash<QString, VIncrementTableRow> *incrementTable = data->DataIncrementTable();
    QHashIterator<QString, VIncrementTableRow> i(*incrementTable);
    QMap<qint64, QString> map;
    //Sorting QHash by id
    while (i.hasNext()) {
        i.next();
        VIncrementTableRow cell = i.value();
        map.insert(cell.getId(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<qint64, QString> iMap(map);
    while (iMap.hasNext()) {
        iMap.next();
        VIncrementTableRow cell = incrementTable->value(iMap.value());
        currentRow++;
        ui->tableWidgetIncrement->setRowCount ( incrementTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(iMap.value());
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, cell.getId());
        ui->tableWidgetIncrement->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(data->GetValueIncrementTableRow(iMap.value())));
        item->setTextAlignment(Qt::AlignHCenter);
        // set the item non-editable (view only), and non-selectable
        Qt::ItemFlags flags = item->flags();
        flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);
        ui->tableWidgetIncrement->setItem(currentRow, 1, item);

        item = new QTableWidgetItem(QString().setNum(cell.getBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 2, item);

        item = new QTableWidgetItem(QString().setNum(cell.getKsize()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 3, item);

        item = new QTableWidgetItem(QString().setNum(cell.getKgrowth()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 4, item);

        item = new QTableWidgetItem(cell.getDescription());
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 5, item);
    }
    if(ui->tableWidgetIncrement->rowCount()>0){
        ui->toolButtonRemove->setEnabled(true);
    }
    ui->tableWidgetIncrement->resizeColumnsToContents();
    ui->tableWidgetIncrement->resizeRowsToContents();
    ui->tableWidgetIncrement->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetIncrement->setCurrentCell( row, column );
}

void DialogIncrements::FillLengthLines(){
    const QHash<QString, qreal> *linesTable = data->DataLengthLines();
    qint32 currentRow = -1;
    QHashIterator<QString, qreal> i(*linesTable);
    while (i.hasNext()) {
        i.next();
        qreal length = i.value();
        currentRow++;
        ui->tableWidgetLines->setRowCount ( linesTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidgetLines->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(length));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetLines->setItem(currentRow, 1, item);
    }
    ui->tableWidgetLines->resizeColumnsToContents();
    ui->tableWidgetLines->resizeRowsToContents();
    ui->tableWidgetLines->verticalHeader()->setDefaultSectionSize(20);
}

void DialogIncrements::FillLengthSplines(){
    const QHash<QString, qreal> *splinesTable = data->DataLengthSplines();
    qint32 currentRow = -1;
    QHashIterator<QString, qreal> i(*splinesTable);
    while (i.hasNext()) {
        i.next();
        qreal length = i.value();
        currentRow++;
        ui->tableWidgetSplines->setRowCount ( splinesTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidgetSplines->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(length));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetSplines->setItem(currentRow, 1, item);
    }
    ui->tableWidgetSplines->resizeColumnsToContents();
    ui->tableWidgetSplines->resizeRowsToContents();
    ui->tableWidgetSplines->verticalHeader()->setDefaultSectionSize(20);
}

void DialogIncrements::FillLengthArcs(){
    const QHash<QString, qreal> *arcsTable = data->DataLengthArcs();
    qint32 currentRow = -1;
    QHashIterator<QString, qreal> i(*arcsTable);
    while (i.hasNext()) {
        i.next();
        qreal length = i.value();
        currentRow++;
        ui->tableWidgetArcs->setRowCount ( arcsTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidgetArcs->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(length));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetArcs->setItem(currentRow, 1, item);
    }
    ui->tableWidgetArcs->resizeColumnsToContents();
    ui->tableWidgetArcs->resizeRowsToContents();
    ui->tableWidgetArcs->verticalHeader()->setDefaultSectionSize(20);
}

void DialogIncrements::FullUpdateFromFile(){
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);

    ui->tableWidgetStandart->clear();
    InitialStandartTable();
    FillStandartTable();

    ui->tableWidgetIncrement->clear();
    InitialIncrementTable();
    FillIncrementTable();

    ui->tableWidgetLines->clear();
    InitialLinesTable();
    FillLengthLines();

    ui->tableWidgetSplines->clear();
    InitialSplinesTable();
    FillLengthSplines();

    ui->tableWidgetArcs->clear();
    InitialArcsTable();
    FillLengthArcs();

    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
}

void DialogIncrements::clickedToolButtonAdd(){
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);
    ui->tableWidgetIncrement->setFocus(Qt::OtherFocusReason);
    qint32 currentRow  = ui->tableWidgetIncrement->rowCount();
    ui->tableWidgetIncrement->insertRow( currentRow );

    qint32 num = 1;
    QString name;
    do{
        name = QString(tr("Denotation %1")).arg(num);
        num++;
    }while(data->IncrementTableContains(name));

    qint64 id = data->getNextId();
    qreal base = 0;
    qreal ksize = 0;
    qreal kgrowth = 0;
    QString description = QString(tr("Description"));
    VIncrementTableRow incrementRow = VIncrementTableRow(id, base, ksize, kgrowth, description);
    data->AddIncrementTableRow(name, incrementRow);

    AddIncrementToFile(id, name, base, ksize, kgrowth, description);

    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFont(QFont("Times", 12, QFont::Bold));
    item->setData(Qt::UserRole, id);
    ui->tableWidgetIncrement->setItem(currentRow, 0, item);
    ui->tableWidgetIncrement->setCurrentCell(currentRow, 0, QItemSelectionModel::ClearAndSelect);

    item = new QTableWidgetItem("0");
    item->setTextAlignment(Qt::AlignHCenter);
    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
    ui->tableWidgetIncrement->setItem(currentRow, 1, item);

    item = new QTableWidgetItem("0");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 2, item);

    item = new QTableWidgetItem("0");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 3, item);

    item = new QTableWidgetItem("0");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 4, item);

    item = new QTableWidgetItem(tr("Description"));
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 5, item);

    ui->toolButtonRemove->setEnabled(true);
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
    emit haveLiteChange();
}

void DialogIncrements::clickedToolButtonRemove(){
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);
    QTableWidgetItem *item = ui->tableWidgetIncrement->currentItem();
    qint32 row = item->row();
    QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    data->RemoveIncrementTableRow(itemName->text());
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        QDomNodeList list = doc->elementsByTagName("increments");
        list.at(0).removeChild(domElement);
    }
    ui->tableWidgetIncrement->removeRow(row);
    if(ui->tableWidgetIncrement->rowCount() == 0){
        ui->toolButtonRemove->setEnabled(false);
    }
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
    emit haveLiteChange();
}

void DialogIncrements::AddIncrementToFile(qint64 id, QString name, qreal base, qreal ksize,
                                          qreal kgrowth, QString description){
    QDomNodeList list = doc->elementsByTagName("increments");
    QDomElement element = doc->createElement("increment");

    QDomAttr drawAttr = doc->createAttribute("id");
    drawAttr.setValue(QString().setNum(id));
    element.setAttributeNode(drawAttr);

    drawAttr = doc->createAttribute("name");
    drawAttr.setValue(name);
    element.setAttributeNode(drawAttr);

    drawAttr = doc->createAttribute("base");
    drawAttr.setValue(QString().setNum(base));
    element.setAttributeNode(drawAttr);

    drawAttr = doc->createAttribute("ksize");
    drawAttr.setValue(QString().setNum(ksize));
    element.setAttributeNode(drawAttr);

    drawAttr = doc->createAttribute("kgrowth");
    drawAttr.setValue(QString().setNum(kgrowth));
    element.setAttributeNode(drawAttr);

    drawAttr = doc->createAttribute("description");
    drawAttr.setValue(description);
    element.setAttributeNode(drawAttr);

    list.at(0).appendChild(element);
}

void DialogIncrements::cellChanged ( qint32 row, qint32 column ){
    QTableWidgetItem *item = 0;
    QTableWidgetItem *itemName = 0;
    qint64 id;
    QDomElement domElement;
    this->row = row;
    switch(column) {
        case 0:
            item = ui->tableWidgetIncrement->item(row, 0);
            id = qvariant_cast<qint64>(item->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("name", item->text());
                data->ClearIncrementTable();
                this->column = 2;
                emit FullUpdateTree();
            }
            break;
        case 2:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                bool ok = false;
                qreal value = item->text().toDouble(&ok);
                if(ok){
                    domElement.setAttribute("base", value);
                    this->column = 3;
                    emit FullUpdateTree();
                } else {
                    throw VException(tr("Can't convert toDouble value."));
                }
            }
            break;
        case 3:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("ksize", item->text().toDouble());
                this->column = 4;
                emit FullUpdateTree();
            }
            break;
        case 4:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("kgrowth", item->text().toDouble());
                this->column = 5;
                emit FullUpdateTree();
            }
            break;
        case 5:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("description", item->text());
                VIncrementTableRow incr = data->GetIncrementTableRow(itemName->text());
                incr.setDescription(item->text());
                data->UpdateIncrementTableRow(itemName->text(), incr);
                ui->tableWidgetIncrement->setCurrentCell( row, 0 );
                emit haveLiteChange();
            }
            break;
    }
}

void DialogIncrements::InitialStandartTable(){
    ui->tableWidgetStandart->setSortingEnabled(false);
    ui->tableWidgetStandart->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Denotation")));
    ui->tableWidgetStandart->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Calculated value")));
    ui->tableWidgetStandart->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Base value")));
    ui->tableWidgetStandart->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("In size")));
    ui->tableWidgetStandart->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("In growth")));
    ui->tableWidgetStandart->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Description")));
}

void DialogIncrements::InitialIncrementTable(){
    ui->tableWidgetIncrement->setSortingEnabled(false);
    ui->tableWidgetIncrement->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Denotation")));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Calculated value")));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Base value")));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("In size")));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("In growth")));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Description")));
}

void DialogIncrements::InitialLinesTable(){
    ui->tableWidgetLines->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Line")));
    ui->tableWidgetLines->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Length")));
}

void DialogIncrements::InitialSplinesTable(){
    ui->tableWidgetSplines->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Curve")));
    ui->tableWidgetSplines->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Length")));
}

void DialogIncrements::InitialArcsTable(){
    ui->tableWidgetArcs->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Arc")));
    ui->tableWidgetArcs->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Length")));
}

void DialogIncrements::DialogAccepted(){
    emit DialogClosed(QDialog::Accepted);
}

DialogIncrements::~DialogIncrements(){
    delete ui;
}
