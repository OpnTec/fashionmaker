#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include <QHeaderView>
#include <QCloseEvent>
#include "widgets/delegate.h"
#include "widgets/doubledelegate.h"
#include "container/vincrementtablerow.h"

DialogIncrements::DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent) :
    DialogTool(data, Draw::Calculation, parent), ui(new Ui::DialogIncrements), data(data), doc(doc){
    ui->setupUi(this);
    InitialStandartTable();
    InitialIncrementTable();
    InitialLinesTable();
    DoubleSpinBoxDelegate *doubleDelegate = new DoubleSpinBoxDelegate(ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(2, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(3, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(4, doubleDelegate);
    FillStandartTable();
    FillIncrementTable();
    FillLengthLines();

    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonAdd);
    connect(ui->toolButtonRemove, &QPushButton::clicked, this,
            &DialogIncrements::clickedToolButtonRemove);

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    connect(this, &DialogIncrements::haveLiteChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this,
            &DialogIncrements::FullUpdateFromFile);

    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogIncrements::DialogAccepted);
}

void DialogIncrements::FillStandartTable(){
    const QMap<QString, VStandartTableCell> *standartTable = data->DataStandartTable();
    qint32 currentRow = -1;
    QMapIterator<QString, VStandartTableCell> i(*standartTable);
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
    const QMap<QString, VIncrementTableRow> *incrementTable = data->DataIncrementTable();
    qint32 currentRow = -1;
    QMapIterator<QString, VIncrementTableRow> i(*incrementTable);
    while (i.hasNext()) {
        i.next();
        VIncrementTableRow cell = i.value();
        currentRow++;
        ui->tableWidgetIncrement->setRowCount ( incrementTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, cell.getId());
        ui->tableWidgetIncrement->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(data->GetValueIncrementTableRow(i.key())));
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
}

void DialogIncrements::FillLengthLines(){
    const QMap<QString, qreal> *linesTable = data->DataLengthLines();
    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(*linesTable);
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
    const QMap<QString, qreal> *splinesTable = data->DataLengthSplines();
    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(*splinesTable);
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
    const QMap<QString, qreal> *arcsTable = data->DataLengthArcs();
    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(*arcsTable);
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

void DialogIncrements::AddIncrementToFile(qint64 id, QString name, qint32 base, qreal ksize,
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

void DialogIncrements::InitialStandartTable(){
    ui->tableWidgetStandart->setSortingEnabled(false);
    ui->tableWidgetStandart->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetStandart->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetStandart->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
}

void DialogIncrements::InitialIncrementTable(){
    ui->tableWidgetIncrement->setSortingEnabled(false);
    ui->tableWidgetIncrement->setHorizontalHeaderItem(0, new QTableWidgetItem("Позначення"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(1, new QTableWidgetItem("Розрах. знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(2, new QTableWidgetItem("Базове знач."));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(3, new QTableWidgetItem("В розмірі"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(4, new QTableWidgetItem("В рості"));
    ui->tableWidgetIncrement->setHorizontalHeaderItem(5, new QTableWidgetItem("Опис"));
}

void DialogIncrements::InitialLinesTable(){
    ui->tableWidgetLines->setHorizontalHeaderItem(0, new QTableWidgetItem("Лінія"));
    ui->tableWidgetLines->setHorizontalHeaderItem(1, new QTableWidgetItem("Довжина"));
}

void DialogIncrements::InitialSplinesTable(){
    ui->tableWidgetSplines->setHorizontalHeaderItem(0, new QTableWidgetItem("Лінія"));
    ui->tableWidgetSplines->setHorizontalHeaderItem(1, new QTableWidgetItem("Довжина"));
}

void DialogIncrements::InitialArcsTable(){
    ui->tableWidgetArcs->setHorizontalHeaderItem(0, new QTableWidgetItem("Лінія"));
    ui->tableWidgetArcs->setHorizontalHeaderItem(1, new QTableWidgetItem("Довжина"));
}

void DialogIncrements::DialogAccepted(){
    emit DialogClosed(QDialog::Accepted);
}

DialogIncrements::~DialogIncrements(){
    delete ui;
}
