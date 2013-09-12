#include "dialoghistory.h"
#include "ui_dialoghistory.h"
#include "geometry/varc.h"
#include "geometry/vspline.h"
#include "geometry/vsplinepath.h"
#include <QDebug>

DialogHistory::DialogHistory(VContainer *data, VDomDocument *doc, QWidget *parent) :
    DialogTool(data, Draw::Calculation, parent), ui(new Ui::DialogHistory), doc(doc), cursorRow(0),
    cursorToolRecordRow(0){
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogHistory::DialogAccepted);
    FillTable();
    InitialTable();
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &DialogHistory::cellClicked);
    connect(this, &DialogHistory::ShowHistoryTool, doc, &VDomDocument::ShowHistoryTool);
    connect(doc, &VDomDocument::ChangedCursor, this, &DialogHistory::ChangedCursor);
    connect(doc, &VDomDocument::haveChange, this, &DialogHistory::UpdateHistory);
    connect(doc, &VDomDocument::ChangedActivDraw, this, &DialogHistory::UpdateHistory);
    ShowPoint();
}

DialogHistory::~DialogHistory(){
    delete ui;
}

void DialogHistory::DialogAccepted(){
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    emit DialogClosed(QDialog::Accepted);
}

void DialogHistory::cellClicked(int row, int column){
    if(column == 0){
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        item->setIcon(QIcon());

        item = ui->tableWidget->item(row, 0);
        cursorRow = row;
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        disconnect(doc, &VDomDocument::ChangedCursor, this, &DialogHistory::ChangedCursor);
        doc->setCursor(id);
        connect(doc, &VDomDocument::ChangedCursor, this, &DialogHistory::ChangedCursor);
    } else {
        QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, false);

        cursorToolRecordRow = row;
        item = ui->tableWidget->item(cursorToolRecordRow, 0);
        id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}

void DialogHistory::ChangedCursor(qint64 id){
    for(qint32 i = 0; i< ui->tableWidget->rowCount(); ++i){
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        qint64 rId = qvariant_cast<qint64>(item->data(Qt::UserRole));
        if(rId == id){
            QTableWidgetItem *oldCursorItem = ui->tableWidget->item(cursorRow, 0);
            oldCursorItem->setIcon(QIcon());
            cursorRow = i;
            item->setIcon(QIcon("://icon/32x32/put_after.png"));
        }
    }
}

void DialogHistory::UpdateHistory(){
    FillTable();
    InitialTable();
}

void DialogHistory::FillTable(){
    ui->tableWidget->clear();
    QVector<VToolRecord> *history = doc->getHistory();
    qint32 currentRow = -1;
    qint32 count = 0;
    ui->tableWidget->setRowCount(history->size());
    for(qint32 i = 0; i< history->size(); ++i){
        VToolRecord tool = history->at(i);
        if(tool.getNameDraw() != doc->GetNameActivDraw()){
            continue;
        }
        currentRow++;

        QTableWidgetItem *item = new QTableWidgetItem(QString());
        item->setTextAlignment(Qt::AlignHCenter);
        item->setData(Qt::UserRole, tool.getId());
        ui->tableWidget->setItem(currentRow, 0, item);

        QString historyRecord = Record(tool);
        item = new QTableWidgetItem(historyRecord);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(currentRow, 1, item);
        ++count;
    }
    ui->tableWidget->setRowCount(count);
    if(history->size()>0){
        cursorRow = currentRow;
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

QString DialogHistory::Record(const VToolRecord &tool){
    QString record = QString();
    qint64 basePointId = 0;
    qint64 secondPointId = 0;
    qint64 firstPointId = 0;
    qint64 thirdPointId = 0;
    qint64 p1Line1 = 0;
    qint64 p2Line1 = 0;
    qint64 p1Line2 = 0;
    qint64 p2Line2 = 0;
    qint64 center = 0;
    QDomElement domElement;
    switch( tool.getTypeTool() ){
        case Tools::ArrowTool:
            break;
        case Tools::SinglePointTool:
            record = QString("%1 - Базова точка").arg(data->GetPoint(tool.getId()).name());
            break;
        case Tools::EndLineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                basePointId = domElement.attribute("basePoint", "").toLongLong();
            }
            record = QString("%1_%2 - Відрізок з точки %1 до точки %2").arg(data->GetPoint(basePointId).name(),
                                                                            data->GetPoint(tool.getId()).name());
            break;
        case Tools::LineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            }
            record = QString("%1_%2 - Лінія з точки %1 до точки %2").arg(data->GetPoint(firstPointId).name(),
                                                                         data->GetPoint(secondPointId).name());
            break;
        case Tools::AlongLineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                basePointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            }
            record = QString("%3 - Точка на відрізку %1_%2").arg(data->GetPoint(basePointId).name(),
                                                                 data->GetPoint(secondPointId).name(),
                                                                 data->GetPoint(tool.getId()).name());
            break;
        case Tools::ShoulderPointTool:
            record = QString("%1 - Плечева точка").arg(data->GetPoint(tool.getId()).name());
            break;
        case Tools::NormalTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                basePointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            }
            record = QString("%3 - Перпендикуляр до відрузку %1_%2").arg(data->GetPoint(basePointId).name(),
                                                                         data->GetPoint(secondPointId).name(),
                                                                         data->GetPoint(tool.getId()).name());
            break;
        case Tools::BisectorTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                basePointId = domElement.attribute("secondPoint", "").toLongLong();
                thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
            }
            record = QString("%4 - Бісектриса кута %1_%2_%3").arg(data->GetPoint(firstPointId).name(),
                                                                  data->GetPoint(basePointId).name(),
                                                                  data->GetPoint(thirdPointId).name(),
                                                                  data->GetPoint(tool.getId()).name());
            break;
        case Tools::LineIntersectTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                p1Line1 = domElement.attribute("p1Line1", "").toLongLong();
                p2Line1 = domElement.attribute("p2Line1", "").toLongLong();
                p1Line2 = domElement.attribute("p1Line2", "").toLongLong();
                p2Line2 = domElement.attribute("p2Line2", "").toLongLong();
            }
            record = QString("%5 - Точка перетину відрузку %1_%2 і %3_%4").arg(data->GetPoint(p1Line1).name(),
                                                                               data->GetPoint(p2Line1).name(),
                                                                               data->GetPoint(p1Line2).name(),
                                                                               data->GetPoint(p2Line2).name(),
                                                                               data->GetPoint(tool.getId()).name());
            break;
        case Tools::SplineTool:{
            VSpline spl = data->GetSpline(tool.getId());
            record = QString("Сплайн %1_%2").arg(data->GetPoint(spl.GetP1()).name(),
                                                 data->GetPoint(spl.GetP4()).name());
        }
        break;
        case Tools::ArcTool:{
            VArc arc = data->GetArc(tool.getId());
            record = QString("Дуга з центром в точці %1").arg(data->GetPoint(arc.GetCenter()).name());
        }
        break;
        case Tools::SplinePathTool:{
            VSplinePath splPath = data->GetSplinePath(tool.getId());
            QVector<VSplinePoint> points = splPath.GetSplinePath();
            if(points.size() != 0 ){
                record = QString("Шлях сплайну %1").arg(data->GetPoint(points[0].P()).name());
                for(qint32 i = 1; i< points.size(); ++i){
                    QString name = QString("_%1").arg(data->GetPoint(points[i].P()).name());
                    record.append(name);
                }
            }
        }
         break;
        case Tools::PointOfContact:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if(domElement.isElement()){
                center = domElement.attribute("center", "").toLongLong();
                firstPointId = domElement.attribute("firstPoint", "").toLongLong();
                secondPointId = domElement.attribute("secondPoint", "").toLongLong();
            }
            record = QString("%4 - Точка дотику дуги з центром в точці %1 і відрізку %2_%3").arg(data->GetPoint(center).name(),
                                                                               data->GetPoint(firstPointId).name(),
                                                                               data->GetPoint(secondPointId).name(),
                                                                               data->GetPoint(tool.getId()).name());
            break;
        default:
            qWarning()<<"Get wrong tool type. Ignore.";
            break;
    }
    return record;
}

void DialogHistory::InitialTable(){
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(" "));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Інструмент"));
}

void DialogHistory::ShowPoint(){
    QVector<VToolRecord> *history = doc->getHistory();
    if(history->size()>0){
        QTableWidgetItem *item = ui->tableWidget->item(0, 1);
        item->setSelected(true);
        cursorToolRecordRow = 0;
        item = ui->tableWidget->item(0, 0);
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}


void DialogHistory::closeEvent(QCloseEvent *event){
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    DialogTool::closeEvent(event);
}
