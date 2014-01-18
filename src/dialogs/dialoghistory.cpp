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
#include "../tools/drawTools/vtoolcutarc.h"
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
        {
            QString name = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%1 - Base point")).arg(name);
            break;
        }
        case Tool::EndLineTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrBasePoint, "0");
            }
            QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%1_%2 - Line from point %1 to point %2")).arg(basePointIdName, toolIdName);
            break;
        }
        case Tool::LineTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            record = QString(tr("%1_%2 - Line from point %1 to point %2")).arg(firstPointIdName, secondPointIdName);
            break;
        }
        case Tool::AlongLineTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%3 - Point along line %1_%2")).arg(basePointIdName, secondPointIdName, toolIdName);
            break;
        }
        case Tool::ShoulderPointTool:
        {
            QString name = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%1 - Point of shoulder")).arg(name);
            break;
        }
        case Tool::NormalTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                basePointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%3 - normal to line %1_%2")).arg(basePointIdName, secondPointIdName, toolIdName);
            break;
        }
        case Tool::BisectorTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
                thirdPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrThirdPoint, "0");
            }
            QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
            QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
            QString thirdPointIdName = data->GeometricObject<const VPointF *>(thirdPointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%4 - bisector of angle %1_%2_%3")).arg(firstPointIdName, basePointIdName,
                                                                        thirdPointIdName, toolIdName);
            break;
        }
        case Tool::LineIntersectTool:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                p1Line1 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP1Line1, "0");
                p2Line1 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP2Line1, "0");
                p1Line2 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP1Line2, "0");
                p2Line2 = doc->GetParametrLongLong(domElement, VAbstractTool::AttrP2Line2, "0");
            }
            QString p1Line1Name = data->GeometricObject<const VPointF *>(p1Line1)->name();
            QString p2Line1Name = data->GeometricObject<const VPointF *>(p2Line1)->name();
            QString p1Line2Name = data->GeometricObject<const VPointF *>(p1Line2)->name();
            QString p2Line2Name = data->GeometricObject<const VPointF *>(p2Line2)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%5 - intersection of lines %1_%2 and %3_%4")).arg(p1Line1Name, p2Line1Name,
                                                                                   p1Line2Name, p2Line2Name,
                                                                                   toolIdName);
            break;
        }
        case Tool::SplineTool:
        {
            const VSpline *spl = data->GeometricObject<const VSpline *>(tool.getId());
            QString splP1Name = data->GeometricObject<const VSpline *>(spl->GetP1().id())->name();
            QString splP4Name = data->GeometricObject<const VSpline *>(spl->GetP4().id())->name();
            record = QString(tr("Curve %1_%2")).arg(splP1Name, splP4Name);
        }
        break;
        case Tool::ArcTool:
        {
            const VArc *arc = data->GeometricObject<const VArc *>(tool.getId());
            QString arcCenterName = data->GeometricObject<const VArc *>(arc->GetCenter().id())->name();
            record = QString(tr("Arc with center in point %1")).arg(arcCenterName);
        }
        break;
        case Tool::SplinePathTool:
        {
            const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(tool.getId());
            QVector<VSplinePoint> points = splPath->GetSplinePath();
            if (points.size() != 0 )
            {
                QString pName = data->GeometricObject<const VPointF *>(points[0].P().id())->name();
                record = QString(tr("Curve point %1")).arg(pName);
                for (qint32 i = 1; i< points.size(); ++i)
                {
                    pName = data->GeometricObject<const VPointF *>(points[i].P().id())->name();
                    QString name = QString("_%1").arg(pName);
                    record.append(name);
                }
            }
        }
        break;
        case Tool::PointOfContact:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                center = doc->GetParametrLongLong(domElement, VAbstractTool::AttrCenter, "0");
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
            QString centerName = data->GeometricObject<const VPointF *>(center)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%4 - point of contact of arc with the center in point %1 and line %2_%3")).arg(
                        centerName, firstPointIdName, secondPointIdName, toolIdName);
            break;
        }
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
            QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
            QString p1LineIdName = data->GeometricObject<const VPointF *>(p1LineId)->name();
            QString p2LineIdName = data->GeometricObject<const VPointF *>(p2LineId)->name();
            record = QString(tr("Point of perpendicular from point %1 to line %2_%3")).arg( basePointIdName,
                                                                                            p1LineIdName, p2LineIdName);
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
            QString axisP1IdName = data->GeometricObject<const VPointF *>(axisP1Id)->name();
            QString axisP2IdName = data->GeometricObject<const VPointF *>(axisP2Id)->name();
            QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            record = QString(tr("Triangle: axis %1_%2, points %3 and %4")).arg( axisP1IdName, axisP2IdName,
                                                                                firstPointIdName, secondPointIdName);
            break;
        }
        case Tool::PointOfIntersection:
        {
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                firstPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
                secondPointId = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
            }
            QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
            QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%1 - point of intersection %2 and %3")).arg(toolIdName, firstPointIdName,
                                                                             secondPointIdName);
        }
        break;
        case Tool::CutArcTool:
        {
            qint64 arcId = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                arcId = doc->GetParametrLongLong(domElement, VToolCutArc::AttrArc, "0");
            }
            const VArc *arc = data->GeometricObject<const VArc *>(arcId);
            QString arcCenterName = data->GeometricObject<const VArc *>(arc->GetCenter().id())->name();
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            record = QString(tr("%1 - cut arc with center %2")).arg(toolIdName, arcCenterName);
        }
        break;
        case Tool::CutSplineTool:
        {
            qint64 splineId = 0;
            domElement = doc->elementById(QString().setNum(tool.getId()));
            if (domElement.isElement())
            {
                splineId = doc->GetParametrLongLong(domElement, VToolCutSpline::AttrSpline, "0");
            }
            const VSpline *spl = data->GeometricObject<const VSpline *>(splineId);
            QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
            QString splP1Name = data->GeometricObject<const VPointF *>(spl->GetP1().id())->name();
            QString splP4Name = data->GeometricObject<const VPointF *>(spl->GetP4().id())->name();
            record = QString(tr("%1 - cut curve %2_%3")).arg(toolIdName, splP1Name, splP4Name);
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
            const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(splinePathId);
            QVector<VSplinePoint> points = splPath->GetSplinePath();
            if (points.size() != 0 )
            {
                QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                QString pName = data->GeometricObject<const VPointF *>(points[0].P().id())->name();
                record = QString(tr("%1 - cut curve point %2")).arg(toolIdName, pName);
                for (qint32 i = 1; i< points.size(); ++i)
                {
                    pName = data->GeometricObject<const VPointF *>(points[i].P().id())->name();
                    QString name = QString("_%1").arg(pName);
                    record.append(name);
                }
            }
        }
        break;
        //Because "history" not only show history of pattern, but help restore current data for each pattern's piece, we
        //need add record about details and nodes, but don't show them.
        case Tool::Detail:
            break;
        case Tool::UnionDetails:
            break;
        case Tool::NodeArc:
            break;
        case Tool::NodePoint:
            break;
        case Tool::NodeSpline:
            break;
        case Tool::NodeSplinePath:
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
