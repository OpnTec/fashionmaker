/************************************************************************
 **
 **  @file   dialogincrements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../../widgets/doubledelegate.h"
#include "../../widgets/textdelegate.h"
#include "../../exception/vexception.h"
#include "../../xml/vstandardmeasurements.h"

#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>

DialogIncrements::DialogIncrements(VContainer *data, VPattern *doc, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogIncrements), data(data), doc(doc), row(0), column(0), m(nullptr)
{
    ui->setupUi(this);

    if (qApp->patternType() == Pattern::Individual)
    {
        const QString filePath = doc->MPath();
        try
        {
            VDomDocument::ValidateXML("://schema/individual_measurements.xsd", filePath);
            m = new VIndividualMeasurements(data);
            m->setContent(filePath);
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            emit DialogClosed(QDialog::Rejected);
            return;
        }
    }

    //Same regex in each shema files. Don't forget synchronize.
    TextDelegate *textDelegate = new TextDelegate("^([^0-9-*/^+=\\s\\(\\)%:;!]){1,1}([^-*/^+=\\s\\(\\)%:;!]){0,}$",
                                                  ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(0, textDelegate);// name
    DoubleSpinBoxDelegate *doubleDelegate = new DoubleSpinBoxDelegate(ui->tableWidgetIncrement);
    ui->tableWidgetIncrement->setItemDelegateForColumn(2, doubleDelegate);// base value
    ui->tableWidgetIncrement->setItemDelegateForColumn(3, doubleDelegate);// in sizes
    ui->tableWidgetIncrement->setItemDelegateForColumn(4, doubleDelegate);// in heights

    FillMeasurements();
    FillIncrements();
    FillLengthLines();
    FillLengthSplines();
    FillLengthArcs();

    if (qApp->patternType() == Pattern::Standard)
    {
        ui->pagePersonalInformation->setVisible(false);
    }
    else
    {
        QRegExpValidator *reg = new QRegExpValidator(QRegExp("\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*"));
        ui->lineEditMail->setValidator(reg);

        ui->tableWidgetMeasurements->setColumnHidden( 1, true );// calculated value
        ui->tableWidgetMeasurements->setColumnHidden( 3, true );// in sizes
        ui->tableWidgetMeasurements->setColumnHidden( 4, true );// in heights

        ui->tableWidgetIncrement->setColumnHidden( 1, true );// calculated value
        ui->tableWidgetIncrement->setColumnHidden( 3, true );// in sizes
        ui->tableWidgetIncrement->setColumnHidden( 4, true );// in heights

        ui->tableWidgetMeasurements->setItemDelegateForColumn(2, doubleDelegate);// base value

        connect(ui->tableWidgetMeasurements, &QTableWidget::cellChanged, this, &DialogIncrements::MeasurementChanged);

        ui->lineEditGivenName->setText(m->GivenName());
        ui->lineEditFamilyName->setText(m->FamilyName());

        ui->comboBoxSex->addItem(tr("male"), QVariant(m->GenderToStr(VIndividualMeasurements::Male)));
        ui->comboBoxSex->addItem(tr("female"), QVariant(m->GenderToStr(VIndividualMeasurements::Female)));
        qint32 index = ui->comboBoxSex->findData(m->GenderToStr(m->Sex()));
        if (index != -1)
        {
            ui->comboBoxSex->setCurrentIndex(index);
        }

        ui->dateEditBirthDate->setDate(m->BirthDate());
        ui->lineEditMail->setText(m->Mail());

        connect(ui->lineEditGivenName, &QLineEdit::editingFinished, this, &DialogIncrements::SaveGivenName);
        connect(ui->lineEditFamilyName, &QLineEdit::editingFinished, this, &DialogIncrements::SaveFamilyName);
        connect(ui->lineEditMail, &QLineEdit::editingFinished, this, &DialogIncrements::SaveEmail);
        connect(ui->comboBoxSex, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                &DialogIncrements::SaveSex);
        connect(ui->dateEditBirthDate, &QDateEdit::dateChanged, this, &DialogIncrements::SaveBirthDate);
    }

    ui->toolBoxMeasurements->setCurrentIndex(1);

    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this, &DialogIncrements::IncrementChanged);
    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonAdd);
    connect(ui->toolButtonRemove, &QPushButton::clicked, this, &DialogIncrements::clickedToolButtonRemove);

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VPattern::FullUpdateTree);
    connect(this, &DialogIncrements::haveLiteChange, this->doc, &VPattern::haveLiteChange);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &DialogIncrements::FullUpdateFromFile);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->toolButtonOpenMeasurements, &QToolButton::clicked, this, &DialogIncrements::OpenTable);
}

void DialogIncrements::FillMeasurements()
{
    const QHash<QString, VMeasurement> *table = data->DataMeasurements();
    QHashIterator<QString, VMeasurement> i(*table);
    QMap<QString, VMeasurement> map;
    //Sorting QHash by id
    while (i.hasNext())
    {
        i.next();
        map.insert(i.key(), i.value());
    }
    qint32 currentRow = -1;
    QMapIterator<QString, VMeasurement> iMap(map);
    ui->tableWidgetMeasurements->setRowCount ( table->size() );
    while (iMap.hasNext())
    {
        iMap.next();
        VMeasurement m = iMap.value();
        currentRow++;

        QTableWidgetItem *item = new QTableWidgetItem(QString(iMap.key()));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setToolTip(m.GetDescription());
        // set the item non-editable (view only), and non-selectable
        Qt::ItemFlags flags = item->flags();
        flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);
        ui->tableWidgetMeasurements->setItem(currentRow, 0, item);

        if (qApp->patternType() == Pattern::Standard)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString().setNum(data->GetValueStandardTableRow(iMap.key())));
            item->setTextAlignment(Qt::AlignHCenter);
            // set the item non-editable (view only), and non-selectable
            Qt::ItemFlags flags = item->flags();
            flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
            item->setFlags(flags);
            ui->tableWidgetMeasurements->setItem(currentRow, 1, item);
        }

        item = new QTableWidgetItem(QString().setNum(m.GetBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetMeasurements->setItem(currentRow, 2, item);

        if (qApp->patternType() == Pattern::Standard)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString().setNum(m.GetKsize()));
            item->setTextAlignment(Qt::AlignHCenter);
            // set the item non-editable (view only), and non-selectable
            Qt::ItemFlags flags = item->flags();
            flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
            item->setFlags(flags);
            ui->tableWidgetMeasurements->setItem(currentRow, 3, item);

            item = new QTableWidgetItem(QString().setNum(m.GetKheight()));
            item->setTextAlignment(Qt::AlignHCenter);
            // set the item non-editable (view only), and non-selectable
            flags = item->flags();
            flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
            item->setFlags(flags);
            ui->tableWidgetMeasurements->setItem(currentRow, 4, item);
        }

        item = new QTableWidgetItem(m.GetNumber());
        item->setTextAlignment(Qt::AlignHCenter);
        // set the item non-editable (view only), and non-selectable
        flags = item->flags();
        flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);
        ui->tableWidgetMeasurements->setItem(currentRow, 5, item);
    }
    ui->tableWidgetMeasurements->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetMeasurements->resizeColumnsToContents();
    ui->tableWidgetMeasurements->resizeRowsToContents();
}

void DialogIncrements::FillIncrements()
{
    const QHash<QString, VIncrement> *increments = data->DataIncrements();
    QHashIterator<QString, VIncrement> i(*increments);
    QMap<quint32, QString> map;
    //Sorting QHash by id
    while (i.hasNext())
    {
        i.next();
        VIncrement incr = i.value();
        map.insert(incr.getId(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<quint32, QString> iMap(map);
    while (iMap.hasNext())
    {
        iMap.next();
        VIncrement incr = increments->value(iMap.value());
        currentRow++;
        ui->tableWidgetIncrement->setRowCount ( increments->size() );

        QTableWidgetItem *item = new QTableWidgetItem(iMap.value());
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, incr.getId());
        ui->tableWidgetIncrement->setItem(currentRow, 0, item);

        if (qApp->patternType() == Pattern::Standard)
        {
            item = new QTableWidgetItem(QString().setNum(data->GetValueIncrementTableRow(iMap.value())));
            item->setTextAlignment(Qt::AlignHCenter);
            // set the item non-editable (view only), and non-selectable
            Qt::ItemFlags flags = item->flags();
            flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
            item->setFlags(flags);
            ui->tableWidgetIncrement->setItem(currentRow, 1, item);
        }

        item = new QTableWidgetItem(QString().setNum(incr.getBase()));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetIncrement->setItem(currentRow, 2, item);

        if (qApp->patternType() == Pattern::Standard)
        {
            item = new QTableWidgetItem(QString().setNum(incr.getKsize()));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidgetIncrement->setItem(currentRow, 3, item);

            item = new QTableWidgetItem(QString().setNum(incr.getKheight()));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidgetIncrement->setItem(currentRow, 4, item);
        }

        item = new QTableWidgetItem(incr.getDescription());
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
    ui->tableWidgetArcs->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidgetArcs->resizeColumnsToContents();
    ui->tableWidgetArcs->resizeRowsToContents();
}

void DialogIncrements::FullUpdateFromFile()
{
    disconnect(ui->tableWidgetMeasurements, &QTableWidget::cellChanged, this, &DialogIncrements::MeasurementChanged);
    ui->tableWidgetMeasurements->clearContents();
    FillMeasurements();
    connect(ui->tableWidgetMeasurements, &QTableWidget::cellChanged, this, &DialogIncrements::MeasurementChanged);

    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this, &DialogIncrements::IncrementChanged);
    ui->tableWidgetIncrement->clearContents();
    FillIncrements();
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this, &DialogIncrements::IncrementChanged);

    ui->tableWidgetLines->clearContents();
    FillLengthLines();

    ui->tableWidgetSplines->clearContents();
    FillLengthSplines();

    ui->tableWidgetArcs->clearContents();
    FillLengthArcs();
}

void DialogIncrements::SaveGivenName()
{
    m->setGivenName(ui->lineEditGivenName->text());
    if (m->SaveDocument(doc->MPath()) == false)
    {
        qDebug()<<"Can't save GivenName";
    }
}

void DialogIncrements::SaveFamilyName()
{

    m->setFamilyName(ui->lineEditFamilyName->text());
    if (m->SaveDocument(doc->MPath()) == false)
    {
        qDebug()<<"Can't save FamilyName";
    }
}

void DialogIncrements::SaveEmail()
{
    m->setMail(ui->lineEditMail->text());
    if (m->SaveDocument(doc->MPath()) == false)
    {
        qDebug()<<"Can't save Email";
    }
}

void DialogIncrements::SaveSex(int index)
{
    m->setSex(m->StrToGender(ui->comboBoxSex->itemData(index).toString()));
    if (m->SaveDocument(doc->MPath()) == false)
    {
        qDebug()<<"Can't save Sex";
    }
}

void DialogIncrements::SaveBirthDate(const QDate & date)
{
    m->setBirthDate(date);
    if (m->SaveDocument(doc->MPath()) == false)
    {
        qDebug()<<"Can't save BirthDate";
    }
}

void DialogIncrements::OpenTable()
{
    QString text = tr("Measurements use different units than pattern. This pattern required measurements in %1")
            .arg(doc->UnitsToStr(qApp->patternUnit()));
    if (qApp->patternType() == Pattern::Individual)
    {
        const QString filter(tr("Individual measurements (*.vit)"));
        const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);
        if (filePath.isEmpty())
        {
            return;
        }

        VIndividualMeasurements *m1 = nullptr;
        try
        {
            VDomDocument::ValidateXML("://schema/individual_measurements.xsd", filePath);

            m1 = new VIndividualMeasurements(data);
            m1->setContent(filePath);
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            delete m1;
            emit DialogClosed(QDialog::Rejected);
            return;
        }
        Valentina::Units mUnit = m1->Unit();
        if (qApp->patternUnit() != mUnit)
        {
            QMessageBox::critical(this, tr("Wrong units."), text);
            delete m1;
            return;
        }
        delete m;
        m = m1;
        data->ClearMeasurements();
        m->Measurements();
        emit FullUpdateTree();

        doc->SetPath(filePath);
        emit haveLiteChange();
    }
    else
    {
        const QString filter(tr("Standard measurements (*.vst)"));
        const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);
        if (filePath.isEmpty())
        {
            return;
        }

        VStandardMeasurements *m1 = nullptr;
        try
        {
            VDomDocument::ValidateXML("://schema/standard_measurements.xsd", filePath);

            m1 = new VStandardMeasurements(data);
            m1->setContent(filePath);
            Valentina::Units mUnit = m1->Unit();
            if (qApp->patternUnit() != mUnit)
            {
                QMessageBox::critical(this, tr("Wrong units."), text);
                delete m1;
                return;
            }
            m1->SetSize();
            m1->SetHeight();
            data->ClearMeasurements();
            m1->Measurements();
            delete m1;
            emit FullUpdateTree();

            doc->SetPath(filePath);
            emit haveLiteChange();
        }
        catch (VException &e)
        {
            e.CriticalMessageBox(tr("File error."), this);
            delete m1;
            emit DialogClosed(QDialog::Rejected);
            return;
        }
    }
}

void DialogIncrements::clickedToolButtonAdd()
{
    ui->tableWidgetIncrement->setFocus(Qt::OtherFocusReason);
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::IncrementChanged);
    qint32 currentRow  = ui->tableWidgetIncrement->rowCount();
    ui->tableWidgetIncrement->insertRow( currentRow );

    qint32 num = 1;
    QString name;
    do
    {
        name = QString(tr("Name_%1")).arg(num);
        num++;
    } while (data->IncrementTableContains(name));

    const quint32 id = data->getNextId();
    const QString description(tr("Description"));
    VIncrement incr = VIncrement(id, 0, 0, 0, description);
    data->AddIncrement(name, incr);

    AddIncrementToFile(id, name, 0, 0, 0, description);

    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFont(QFont("Times", 12, QFont::Bold));
    item->setData(Qt::UserRole, id);
    ui->tableWidgetIncrement->setItem(currentRow, 0, item);
    ui->tableWidgetIncrement->setCurrentCell(currentRow, 0, QItemSelectionModel::ClearAndSelect);

    item = new QTableWidgetItem("0");// calculated value
    item->setTextAlignment(Qt::AlignHCenter);
    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
    ui->tableWidgetIncrement->setItem(currentRow, 1, item);

    item = new QTableWidgetItem("0");// base value
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 2, item);

    item = new QTableWidgetItem("0");// in sizes
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 3, item);

    item = new QTableWidgetItem("0"); // in heights
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 4, item);

    item = new QTableWidgetItem(description);
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidgetIncrement->setItem(currentRow, 5, item);

    ui->toolButtonRemove->setEnabled(true);
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::IncrementChanged);
    emit haveLiteChange();
}

void DialogIncrements::clickedToolButtonRemove()
{
    disconnect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
               &DialogIncrements::IncrementChanged);
    QTableWidgetItem *item = ui->tableWidgetIncrement->currentItem();
    qint32 row = item->row();
    QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    data->RemoveIncrementTableRow(itemName->text());
    quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        QDomNodeList list = doc->elementsByTagName(VPattern::TagIncrements);
        list.at(0).removeChild(domElement);
    }
    ui->tableWidgetIncrement->removeRow(row);
    if (ui->tableWidgetIncrement->rowCount() == 0)
    {
        ui->toolButtonRemove->setEnabled(false);
    }
    connect(ui->tableWidgetIncrement, &QTableWidget::cellChanged, this,
            &DialogIncrements::IncrementChanged);
    emit haveLiteChange();
}

void DialogIncrements::AddIncrementToFile(const quint32 &id, const QString &name, const qreal &base, const qreal &ksize,
                                          const qreal &kheight, const QString &description)
{
    QDomElement element = doc->createElement(VPattern::TagIncrement);

    doc->SetAttribute(element, VDomDocument::AttrId, id);
    doc->SetAttribute(element, VPattern::IncrementName, name);
    doc->SetAttribute(element, VPattern::IncrementBase, base);
    doc->SetAttribute(element, VPattern::IncrementKsize, ksize);
    doc->SetAttribute(element, VPattern::IncrementKgrowth, kheight);
    doc->SetAttribute(element, VPattern::IncrementDescription, description);

    QDomNodeList list = doc->elementsByTagName(VPattern::TagIncrements);
    list.at(0).appendChild(element);
}

void DialogIncrements::IncrementChanged ( qint32 row, qint32 column )
{

    const QTableWidgetItem *itemName = ui->tableWidgetIncrement->item(row, 0);
    const QTableWidgetItem *item = ui->tableWidgetIncrement->item(row, column);
    const quint32 id = qvariant_cast<quint32>(itemName->data(Qt::UserRole));
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement() == false)
    {
        qDebug()<<"Cant't find increment with id = "<<id<<Q_FUNC_INFO;
        return;
    }
    this->row = row;
    switch (column)
    {
        case 0: // VPattern::IncrementName
            doc->SetAttribute(domElement, VPattern::IncrementName, item->text());
            data->ClearIncrementTable();
            this->column = 2;
            emit FullUpdateTree();
            break;
        case 2: // VPattern::IncrementBase
            doc->SetAttribute(domElement, VPattern::IncrementBase, item->text());
            this->column = 3;
            emit FullUpdateTree();
            break;
        case 3: // VPattern::IncrementKsize
            doc->SetAttribute(domElement, VPattern::IncrementKsize, item->text());
            this->column = 4;
            emit FullUpdateTree();
            break;
        case 4: // VPattern::IncrementKgrowth
            doc->SetAttribute(domElement, VPattern::IncrementKgrowth, item->text());
            this->column = 5;
            emit FullUpdateTree();
            break;
        case 5: // VPattern::IncrementDescription
        {
            doc->SetAttribute(domElement, VPattern::IncrementDescription, item->text());
            VIncrement incr = data->GetIncrement(itemName->text());
            incr.setDescription(item->text());
            data->UpdateIncrement(itemName->text(), incr);
            ui->tableWidgetIncrement->resizeColumnsToContents();
            ui->tableWidgetIncrement->resizeRowsToContents();
            this->column = 0;
            ui->tableWidgetIncrement->setCurrentCell( row, this->column );
            break;
        }
        default:
            break;
    }
    emit haveLiteChange();
}

void DialogIncrements::MeasurementChanged(qint32 row, qint32 column)
{
    switch (column)
    {
        case 2:// value column
        {
            const QTableWidgetItem *itemName = ui->tableWidgetMeasurements->item(row, 0);
            QTableWidgetItem *item = ui->tableWidgetMeasurements->item(row, 2);

            VMeasurement measur = data->GetMeasurement(itemName->text());
            const QString tag = measur.TagName();
            QDomNodeList list = m->elementsByTagName(tag);
            QDomElement domElement = list.at(0).toElement();
            if (domElement.isElement() == false)
            {
                qDebug()<<"Cant't find measurement "<<tag<<Q_FUNC_INFO;
                return;
            }

            m->SetAttribute(domElement, VIndividualMeasurements::AttrValue, item->text());
            if (m->SaveDocument(doc->MPath()) == false)
            {
                qDebug()<<"Can't save measurement";
            }
            bool ok = false;
            const qreal base = item->text().replace(",", ".").toDouble(&ok);
            if (ok == false)
            {
                measur.setBase(0);
                item->setText("0");
                qDebug()<<"Can't convert toDouble measurement value"<<Q_FUNC_INFO;
            }
            else
            {
                measur.setBase(base);
            }
            data->ClearMeasurements();
            m->Measurements();
            emit FullUpdateTree();
            break;
        }
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
    delete m;
}
