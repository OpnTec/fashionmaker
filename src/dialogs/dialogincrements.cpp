/************************************************************************
 **
 **  @file   dialogincrements.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include "../widgets/doubledelegate.h"
#include "../widgets/textdelegate.h"
#include "../exception/vexception.h"

#include <QPushButton>

DialogIncrements::DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogIncrements), data(data), doc(doc), row(0), column(0)
{
    ui->setupUi(this);
    TextDelegate *textDelegate = new TextDelegate(ui->tableWidgetIncrement);
    Q_CHECK_PTR(textDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(0, textDelegate);
    DoubleSpinBoxDelegate *doubleDelegate = new DoubleSpinBoxDelegate(ui->tableWidgetIncrement);
    Q_CHECK_PTR(doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(2, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(3, doubleDelegate);
    ui->tableWidgetIncrement->setItemDelegateForColumn(4, doubleDelegate);

    FillStandardTable();
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

    ui->tabWidget->setCurrentIndex(0);
}

void DialogIncrements::FillStandardTable()
{
    const QHash<QString, VStandardTableRow> *standardTable = data->DataStandardTable();
    qint32 currentRow = -1;
    QHashIterator<QString, VStandardTableRow> i(*standardTable);
    ui->tableWidgetStandard->setRowCount ( standardTable->size() );
    while (i.hasNext())
    {
        i.next();
        VStandardTableRow cell = i.value();
        currentRow++;

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidgetStandard->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString().setNum(data->GetValueStandardTableCell(i.key())));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandard->setItem(currentRow, 1, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandard->setItem(currentRow, 2, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKsize()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandard->setItem(currentRow, 3, item);

        item = new QTableWidgetItem(QString().setNum(cell.GetKgrowth()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandard->setItem(currentRow, 4, item);

        item = new QTableWidgetItem(cell.GetDescription());
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetStandard->setItem(currentRow, 5, item);
    }
    ui->tableWidgetStandard->resizeColumnsToContents();
    ui->tableWidgetStandard->resizeRowsToContents();
    ui->tableWidgetStandard->verticalHeader()->setDefaultSectionSize(20);
}

void DialogIncrements::FillIncrementTable()
{
    const QHash<QString, VIncrementTableRow> *incrementTable = data->DataIncrementTable();
    QHashIterator<QString, VIncrementTableRow> i(*incrementTable);
    QMap<qint64, QString> map;
    //Sorting QHash by id
    while (i.hasNext())
    {
        i.next();
        VIncrementTableRow cell = i.value();
        map.insert(cell.getId(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<qint64, QString> iMap(map);
    while (iMap.hasNext())
    {
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
        item->setTextAlignment(Qt::AlignLeft);
        ui->tableWidgetIncrement->setItem(currentRow, 5, item);
    }
    if (ui->tableWidgetIncrement->rowCount()>0)
    {
        ui->toolButtonRemove->setEnabled(true);
    }
    ui->tableWidgetIncrement->resizeColumnsToContents();
    ui->tableWidgetIncrement->resizeRowsToContents();
    ui->tableWidgetIncrement->setCurrentCell( row, column );
}

void DialogIncrements::FillLengthLines()
{
    const QHash<QString, qreal> *linesTable = data->DataLengthLines();
    QHashIterator<QString, qreal> iHash(*linesTable);
    QMap<QString, qreal> map;
    //Sorting QHash by name
    while (iHash.hasNext())
    {
        iHash.next();
        map.insert(iHash.key(), iHash.value());
    }

    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(map);
    while (i.hasNext())
    {
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

void DialogIncrements::FillLengthSplines()
{
    const QHash<QString, qreal> *splinesTable = data->DataLengthSplines();
    QHashIterator<QString, qreal> iHash(*splinesTable);
    QMap<QString, qreal> map;
    //Sorting QHash by name
    while (iHash.hasNext())
    {
        iHash.next();
        map.insert(iHash.key(), iHash.value());
    }

    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(map);
    while (i.hasNext())
    {
        i.next();
        qreal length = i.value();
        currentRow++;
        ui->tableWidgetSplines->setRowCount ( splinesTable->size() );

        QTableWidgetItem *item = new QTableWidgetItem(QString(i.key()));
        item->setTextAlignment(Qt::AlignLeft);
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

void DialogIncrements::FillLengthArcs()
{
    const QHash<QString, qreal> *arcsTable = data->DataLengthArcs();
    QHashIterator<QString, qreal> iHash(*arcsTable);
    QMap<QString, qreal> map;
    //Sorting QHash by name
    while (iHash.hasNext())
    {
        iHash.next();
        map.insert(iHash.key(), iHash.value());
    }

    qint32 currentRow = -1;
    QMapIterator<QString, qreal> i(map);
    while (i.hasNext())
    {
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

void DialogIncrements::FullUpdateFromFile()
{
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);

    ui->tableWidgetStandard->clearContents();
    FillStandardTable();

    ui->tableWidgetIncrement->clearContents();
    FillIncrementTable();

    ui->tableWidgetLines->clearContents();
    FillLengthLines();

    ui->tableWidgetSplines->clearContents();
    FillLengthSplines();

    ui->tableWidgetArcs->clearContents();
    FillLengthArcs();

    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
}

void DialogIncrements::clickedToolButtonAdd()
{
    ui->tableWidgetIncrement->setFocus(Qt::OtherFocusReason);
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);
    qint32 currentRow  = ui->tableWidgetIncrement->rowCount();
    ui->tableWidgetIncrement->insertRow( currentRow );

    qint32 num = 1;
    QString name;
    do
    {
        name = QString(tr("Name_%1")).arg(num);
        num++;
    } while (data->IncrementTableContains(name));

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

void DialogIncrements::clickedToolButtonRemove()
{
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::cellChanged);
    QTableWidgetItem *item = ui->tableWidgetIncrement->currentItem();
    qint32 row = item->row();
    QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    data->RemoveIncrementTableRow(itemName->text());
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        QDomNodeList list = doc->elementsByTagName("increments");
        list.at(0).removeChild(domElement);
    }
    ui->tableWidgetIncrement->removeRow(row);
    if (ui->tableWidgetIncrement->rowCount() == 0)
    {
        ui->toolButtonRemove->setEnabled(false);
    }
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::cellChanged);
    emit haveLiteChange();
}

void DialogIncrements::AddIncrementToFile(qint64 id, QString name, qreal base, qreal ksize, qreal kgrowth,
                                          QString description)
{
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

void DialogIncrements::cellChanged ( qint32 row, qint32 column )
{
    QTableWidgetItem *item = 0;
    QTableWidgetItem *itemName = 0;
    qint64 id;
    QDomElement domElement;
    this->row = row;
    switch (column)
    {
        case 0:
            item = ui->tableWidgetIncrement->item(row, 0);
            id = qvariant_cast<qint64>(item->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if (domElement.isElement())
            {
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
            if (domElement.isElement())
            {
                bool ok = false;
                qreal value = item->text().replace(",", ".").toDouble(&ok);
                if (ok)
                {
                    domElement.setAttribute("base", value);
                    this->column = 3;
                    emit FullUpdateTree();
                }
                else
                {
                    throw VException(tr("Can't convert toDouble value."));
                }
            }
            break;
        case 3:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if (domElement.isElement())
            {
                domElement.setAttribute("ksize", item->text().replace(",", ".").toDouble());
                this->column = 4;
                emit FullUpdateTree();
            }
            break;
        case 4:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if (domElement.isElement())
            {
                domElement.setAttribute("kgrowth", item->text().replace(",", ".").toDouble());
                this->column = 5;
                emit FullUpdateTree();
            }
            break;
        case 5:
            itemName = ui->tableWidgetIncrement->item(row, 0);
            item = ui->tableWidgetIncrement->item(row, column);
            id = qvariant_cast<qint64>(itemName->data(Qt::UserRole));
            domElement = doc->elementById(QString().setNum(id));
            if (domElement.isElement())
            {
                domElement.setAttribute("description", item->text());
                VIncrementTableRow incr = data->GetIncrementTableRow(itemName->text());
                incr.setDescription(item->text());
                data->UpdateIncrementTableRow(itemName->text(), incr);
                ui->tableWidgetIncrement->resizeColumnsToContents();
                ui->tableWidgetIncrement->resizeRowsToContents();
                ui->tableWidgetIncrement->setCurrentCell( row, 0 );
                emit haveLiteChange();
            }
            break;
        default:
            break;
    }
}

void DialogIncrements::closeEvent(QCloseEvent *event)
{
    emit DialogClosed(QDialog::Accepted);
    event->accept();
}

DialogIncrements::~DialogIncrements()
{
    delete ui;
}
