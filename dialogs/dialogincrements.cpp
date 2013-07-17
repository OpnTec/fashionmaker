#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include <QHeaderView>
#include <QCloseEvent>

#include "../container/vincrementtablerow.h"

DialogIncrements::DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogIncrements){
    ui->setupUi(this);
    this->data = data;
    this->doc = doc;
    ui->tableWidgetStandart->resizeColumnsToContents();
    ui->tableWidgetStandart->resizeRowsToContents();
    ui->tableWidgetStandart->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetStandart->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
    ui->tableWidgetIncrement->resizeColumnsToContents();
    ui->tableWidgetIncrement->resizeRowsToContents();
    ui->tableWidgetIncrement->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetIncrement->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
    FillStandartTable();
    FillIncrementTable();

    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonAdd);
    connect(ui->toolButtonRemove, &QPushButton::clicked, this,
            &DialogIncrements::clickedToolButtonRemove);

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    connect(this, &DialogIncrements::haveLiteChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this,
            &DialogIncrements::FullUpdateFromFile);

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogIncrements::clickedButtonOk);
}

void DialogIncrements::FillStandartTable(){
    data->FillStandartTable(ui->tableWidgetStandart);
}

void DialogIncrements::FillIncrementTable(){
    data->FillIncrementTable(ui->tableWidgetIncrement);
    if(ui->tableWidgetIncrement->rowCount()>0){
        ui->toolButtonRemove->setEnabled(true);
    }
}

void DialogIncrements::FullUpdateFromFile(){
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);
    ui->tableWidgetStandart->clear();
    ui->tableWidgetStandart->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
    ui->tableWidgetIncrement->clear();
    ui->tableWidgetIncrement->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
    FillStandartTable();
    FillIncrementTable();
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
        name = QString("Позначення %1").arg(num);
        num++;
    }while(data->IncrementTableContains(name));

    qint64 id = data->getNextId();
    qint32 base = 0;
    qreal ksize = 0;
    qreal kgrowth = 0;
    QString description = QString("Опис");
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

    item = new QTableWidgetItem("Опис");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 5, item);

    ui->toolButtonRemove->setEnabled(true);
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
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
}

void DialogIncrements::AddIncrementToFile(quint64 id, QString name, qint32 base, qreal ksize,
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
    QTableWidgetItem *item;
    QTableWidgetItem *itemName;
    qint64 id;
    QDomElement domElement;
    switch(column) {
        case 0:
            item = ui->tableWidgetIncrement->item(row, 0);
            id = qvariant_cast<qint64>(item->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("name", item->text());
                data->ClearIncrementTable();
                emit FullUpdateTree();
            }
            break;
        case 2:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("base", item->text().toDouble());
                emit FullUpdateTree();
            }
            break;
        case 3:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if(domElement.isElement()){
                domElement.setAttribute("ksize", item->text().toDouble());
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
                emit haveLiteChange();
            }
            break;
    }
}

void DialogIncrements::closeEvent ( QCloseEvent * event ){
    emit closedActionTable();
    event->accept();
}

void DialogIncrements::clickedButtonOk(){
    emit closedActionTable();
}

DialogIncrements::~DialogIncrements(){
    delete ui;
}
