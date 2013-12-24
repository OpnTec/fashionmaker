/************************************************************************
 **
 **  @file   dialoghistory.cpp
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

#include "dialoghistory.h"
#include "ui_dialoghistory.h"
#include "../geometry/varc.h"
#include "../geometry/vspline.h"
#include "../geometry/vsplinepath.h"
#include "../tools/vabstracttool.h"
#include "../tools/drawTools/vtoolcutspline.h"
#include "../tools/drawTools/vtoolcutsplinepath.h"
#include <QDebug>
#include <QPushButton>

DialogHistory::DialogHistory(VContainer *data, VDomDocument *doc, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogHistory), doc(doc), cursorRow(0),
    cursorToolRecordRow(0)
{
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

DialogHistory::~DialogHistory()
{
    delete ui;
}

void DialogHistory::DialogAccepted()
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    emit DialogClosed(QDialog::Accepted);
}

void DialogHistory::cellClicked(int row, int column)
{
    if (column == 0)
    {
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        item->setIcon(QIcon());

        item = ui->tableWidget->item(row, 0);
        cursorRow = row;
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        disconnect(doc, &VDomDocument::ChangedCursor, this, &DialogHistory::ChangedCursor);
        doc->setCursor(id);
        connect(doc, &VDomDocument::ChangedCursor, this, &DialogHistory::ChangedCursor);
    }
    else
    {
        QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, false);

        cursorToolRecordRow = row;
        item = ui->tableWidget->item(cursorToolRecordRow, 0);
        id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}

void DialogHistory::ChangedCursor(qint64 id)
{
    for (qint32 i = 0; i< ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        qint64 rId = qvariant_cast<qint64>(item->data(Qt::UserRole));
        if (rId == id)
        {
            QTableWidgetItem *oldCursorItem = ui->tableWidget->item(cursorRow, 0);
            oldCursorItem->setIcon(QIcon());
            cursorRow = i;
            item->setIcon(QIcon("://icon/32x32/put_after.png"));
        }
    }
}

void DialogHistory::UpdateHistory()
{
    FillTable();
    InitialTable();
}

void DialogHistory::FillTable()
{
    ui->tableWidget->clear();
    QVector<VToolRecord> *history = doc->getHistory();
    qint32 currentRow = -1;
    qint32 count = 0;
    ui->tableWidget->setRowCount(history->size());
    for (qint32 i = 0; i< history->size(); ++i)
    {
        VToolRecord tool = history->at(i);
        if (tool.getNameDraw() != doc->GetNameActivDraw())
        {
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
    if (history->size()>0)
    {
        cursorRow = currentRow;
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

QString DialogHistory::Record(const VToolRecord &tool)
{
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
    switch ( tool.getTypeTool() )
    {
        case Tool::ArrowTool:
            break;
        case Tool::SinglePointTool:
            record = QString(tr("%1 - Base point")).arg(data->GetPoint(tool.getId()).name());
            break;
        case Tool::EndLineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrBasePoint, "0");
            }
            record = QString(tr("%1_%2 - Line from point %1 to point %2")).arg(data->GetPoint(basePointId).name(),
                                                                               data->GetPoint(tool.getId()).name());
            break;
        case Tool::LineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            record = QString(tr("%1_%2 - Line from point %1 to point %2")).arg(data->GetPoint(firstPointId).name(),
                                                                               data->GetPoint(secondPointId).name());
            break;
        case Tool::AlongLineTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            record = QString(tr("%3 - Point along line %1_%2")).arg(data->GetPoint(basePointId).name(),
                                                                    data->GetPoint(secondPointId).name(),
                                                                    data->GetPoint(tool.getId()).name());
            break;
        case Tool::ShoulderPointTool:
            record = QString(tr("%1 - Point of shoulder")).arg(data->GetPoint(tool.getId()).name());
            break;
        case Tool::NormalTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            record = QString(tr("%3 - normal to line %1_%2")).arg(data->GetPoint(basePointId).name(),
                                                                  data->GetPoint(secondPointId).name(),
                                                                  data->GetPoint(tool.getId()).name());
            break;
        case Tool::BisectorTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
                thirdPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrThirdPoint, "0");
            }
            record = QString(tr("%4 - bisector of angle %1_%2_%3")).arg(data->GetPoint(firstPointId).name(),
                                                                        data->GetPoint(basePointId).name(),
                                                                        data->GetPoint(thirdPointId).name(),
                                                                        data->GetPoint(tool.getId()).name());
            break;
        case Tool::LineIntersectTool:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                p1Line1 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP1Line1, "0");
                p2Line1 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP2Line1, "0");
                p1Line2 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP1Line2, "0");
                p2Line2 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP2Line2, "0");
            }
            record = QString(tr("%5 - intersection of lines %1_%2 and %3_%4")).arg(data->GetPoint(p1Line1).name(),
                                                                                   data->GetPoint(p2Line1).name(),
                                                                                   data->GetPoint(p1Line2).name(),
                                                                                   data->GetPoint(p2Line2).name(),
                                                                                   data->GetPoint(tool.getId()).name());
            break;
        case Tool::SplineTool:
        {
            VSpline spl = data->GetSpline(tool.getId());
            record = QString(tr("Curve %1_%2")).arg(data->GetPoint(spl.GetP1()).name(),
                                                    data->GetPoint(spl.GetP4()).name());
        }
        break;
        case Tool::ArcTool:
        {
            VArc arc = data->GetArc(tool.getId());
            record = QString(tr("Arc with center in point %1")).arg(data->GetPoint(arc.GetCenter()).name());
        }
        break;
        case Tool::SplinePathTool:
        {
            VSplinePath splPath = data->GetSplinePath(tool.getId());
            QVector<VSplinePoint> points = splPath.GetSplinePath();
            if (points.size() != 0 )
            {
                record = QString(tr("Curve point %1")).arg(data->GetPoint(points[0].P()).name());
                for (qint32 i = 1; i< points.size(); ++i)
                {
                    QString name = QString("_%1").arg(data->GetPoint(points[i].P()).name());
                    record.append(name);
                }
            }
        }
        break;
        case Tool::PointOfContact:
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                center = doc->GetParametrLongLong(domElement, VAbstractTool::AttrCenter, "0");
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            record = QString(tr("%4 - point of contact of arc with the center in point %1 and line %2_%3")).arg(
                        data->GetPoint(center).name(), data->GetPoint(firstPointId).name(),
                        data->GetPoint(secondPointId).name(), data->GetPoint(tool.getId()).name());
            break;
        case Tool::Height:
        {
            qint64 p1LineId = 0;
            qint64 p2LineId = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrBasePoint, "0");
                p1LineId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP1Line, "0");
                p2LineId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP2Line, "0");
            }
            record = QString(tr("Point of perpendicular from point %1 to line %2_%3")).arg(
                        data->GetPoint(basePointId).name(), data->GetPoint(p1LineId).name(),
                        data->GetPoint(p2LineId).name());
            break;
        }
        case Tool::Triangle:
        {
            qint64 axisP1Id = 0;
            qint64 axisP2Id = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                axisP1Id = doc->GetParametrLongLong(domElement, VAbstractTool::AttrAxisP1, "0");
                axisP2Id = doc->GetParametrLongLong(domElement, VAbstractTool::AttrAxisP2, "0");
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            record = QString(tr("Triangle: axis %1_%2, points %3 and %4")).arg(
                        data->GetPoint(axisP1Id).name(), data->GetPoint(axisP2Id).name(),
                        data->GetPoint(firstPointId).name(), data->GetPoint(secondPointId).name());
            break;
        }
        case Tool::CutSplineTool:
        {
            qint64 splineId = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                splineId = doc->GetParametrLongLong(domElement, VToolCutSpline::AttrSpline, "0");
            }
            VSpline spl = data->GetSpline(splineId);
            record = QString(tr("%1 - cut curve %2_%3")).arg(data->GetPoint(tool.getId()).name(),
                                                             data->GetPoint(spl.GetP1()).name(),
                                                             data->GetPoint(spl.GetP4()).name());
        }
        break;
        case Tool::CutSplinePathTool:
        {
            qint64 splinePathId = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                splinePathId = doc->GetParametrLongLong(domElement, VToolCutSplinePath::AttrSplinePath, "0");
            }
            VSplinePath splPath = data->GetSplinePath(splinePathId);
            QVector<VSplinePoint> points = splPath.GetSplinePath();
            if (points.size() != 0 )
            {
                record = QString(tr("%1 - cut curve point %2")).arg(data->GetPoint(tool.getId()).name(),
                                                                    data->GetPoint(points[0].P()).name());
                for (qint32 i = 1; i< points.size(); ++i)
                {
                    QString name = QString("_%1").arg(data->GetPoint(points[i].P()).name());
                    record.append(name);
                }
            }
        }
        break;
        default:
            qWarning()<<tr("Got wrong tool type. Ignore.");
            break;
    }
    return record;
}

void DialogHistory::InitialTable()
{
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(" "));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Tool")));
}

void DialogHistory::ShowPoint()
{
    QVector<VToolRecord> *history = doc->getHistory();
    if (history->size()>0)
    {
        QTableWidgetItem *item = ui->tableWidget->item(0, 1);
        item->setSelected(true);
        cursorToolRecordRow = 0;
        item = ui->tableWidget->item(0, 0);
        qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}

void DialogHistory::closeEvent(QCloseEvent *event)
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    qint64 id = qvariant_cast<qint64>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    DialogTool::closeEvent(event);
}
