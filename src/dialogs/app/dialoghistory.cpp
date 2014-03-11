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
#include "../../geometry/varc.h"
#include "../../geometry/vspline.h"
#include "../../geometry/vsplinepath.h"
#include "../../tools/vabstracttool.h"
#include "../../tools/drawTools/vtoolcutspline.h"
#include "../../tools/drawTools/vtoolcutsplinepath.h"
#include "../../tools/drawTools/vtoolcutarc.h"
#include <QDebug>
#include <QPushButton>

DialogHistory::DialogHistory(VContainer *data, VPattern *doc, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogHistory), doc(doc), cursorRow(0),
    cursorToolRecordRow(0)
{
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogHistory::DialogAccepted);
    FillTable();
    InitialTable();
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &DialogHistory::cellClicked);
    connect(this, &DialogHistory::ShowHistoryTool, doc, &VPattern::ShowHistoryTool);
    connect(doc, &VPattern::ChangedCursor, this, &DialogHistory::ChangedCursor);
    connect(doc, &VPattern::patternChanged, this, &DialogHistory::UpdateHistory);
    connect(doc, &VPattern::ChangedActivDraw, this, &DialogHistory::UpdateHistory);
    ShowPoint();
}

DialogHistory::~DialogHistory()
{
    delete ui;
}

void DialogHistory::DialogAccepted()
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
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
        quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        disconnect(doc, &VPattern::ChangedCursor, this, &DialogHistory::ChangedCursor);
        doc->setCursor(id);
        connect(doc, &VPattern::ChangedCursor, this, &DialogHistory::ChangedCursor);
    }
    else
    {
        QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
        quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, false);

        cursorToolRecordRow = row;
        item = ui->tableWidget->item(cursorToolRecordRow, 0);
        id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}

void DialogHistory::ChangedCursor(quint32 id)
{
    for (qint32 i = 0; i< ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        quint32 rId = qvariant_cast<quint32>(item->data(Qt::UserRole));
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
    const QVector<VToolRecord> *history = doc->getHistory();
    Q_CHECK_PTR(history);
    qint32 currentRow = -1;
    qint32 count = 0;
    ui->tableWidget->setRowCount(history->size());
    for (qint32 i = 0; i< history->size(); ++i)
    {
        const VToolRecord tool = history->at(i);
        if (tool.getNameDraw() != doc->GetNameActivDraw())
        {
            continue;
        }
        const QString historyRecord = Record(tool);
        if (historyRecord.isEmpty() ==false)
        {
            currentRow++;

            {
                QTableWidgetItem *item = new QTableWidgetItem(QString());
                item->setTextAlignment(Qt::AlignHCenter);
                item->setData(Qt::UserRole, tool.getId());
                ui->tableWidget->setItem(currentRow, 0, item);
            }

            QTableWidgetItem *item = new QTableWidgetItem(historyRecord);
            item->setFont(QFont("Times", 12, QFont::Bold));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(currentRow, 1, item);
            ++count;
        }
    }
    ui->tableWidget->setRowCount(count);
    if (history->size()>0)
    {
        cursorRow = currentRow;
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        Q_CHECK_PTR(item);
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

QString DialogHistory::Record(const VToolRecord &tool)
{
    const QDomElement domElement = doc->elementById(QString().setNum(tool.getId()));
    if (domElement.isElement() == false)
    {
        qWarning()<<"Can't find element by id"<<Q_FUNC_INFO;
        return QString(tr("Can't create record."));
    }
    try
    {
        switch ( tool.getTypeTool() )
        {
            case Valentina::ArrowTool:
                Q_UNREACHABLE();
                break;
            case Valentina::SinglePointTool:
            {
                const QString name = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%1 - Base point")).arg(name);
            }
            case Valentina::EndLineTool:
            {
                const quint32 basePointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, "0");
                const QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%1_%2 - Line from point %1 to point %2")).arg(basePointIdName, toolIdName);
            }
            case Valentina::LineTool:
            {
                const quint32 firstPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                return QString(tr("%1_%2 - Line from point %1 to point %2")).arg(firstPointIdName, secondPointIdName);
            }
            case Valentina::AlongLineTool:
            {
                const quint32 basePointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%3 - Point along line %1_%2")).arg(basePointIdName, secondPointIdName, toolIdName);
            }
            case Valentina::ShoulderPointTool:
            {
                const QString name = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%1 - Point of shoulder")).arg(name);
            }
            case Valentina::NormalTool:
            {
                const quint32 basePointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%3 - normal to line %1_%2")).arg(basePointIdName, secondPointIdName, toolIdName);
            }
            case Valentina::BisectorTool:
            {
                const quint32 firstPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const quint32 thirdPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrThirdPoint, "0");
                const QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                const QString thirdPointIdName = data->GeometricObject<const VPointF *>(thirdPointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%4 - bisector of angle %1_%2_%3")).arg(firstPointIdName, secondPointIdName,
                                                                          thirdPointIdName, toolIdName);
            }
            case Valentina::LineIntersectTool:
            {
                const quint32 p1Line1 = doc->GetParametrUInt(domElement, VAbstractTool::AttrP1Line1, "0");
                const quint32 p2Line1 = doc->GetParametrUInt(domElement, VAbstractTool::AttrP2Line1, "0");
                const quint32 p1Line2 = doc->GetParametrUInt(domElement, VAbstractTool::AttrP1Line2, "0");
                const quint32 p2Line2 = doc->GetParametrUInt(domElement, VAbstractTool::AttrP2Line2, "0");
                const QString p1Line1Name = data->GeometricObject<const VPointF *>(p1Line1)->name();
                const QString p2Line1Name = data->GeometricObject<const VPointF *>(p2Line1)->name();
                const QString p1Line2Name = data->GeometricObject<const VPointF *>(p1Line2)->name();
                const QString p2Line2Name = data->GeometricObject<const VPointF *>(p2Line2)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%5 - intersection of lines %1_%2 and %3_%4")).arg(p1Line1Name, p2Line1Name,
                                                                                     p1Line2Name, p2Line2Name,
                                                                                     toolIdName);
            }
            case Valentina::SplineTool:
            {
                const VSpline *spl = data->GeometricObject<const VSpline *>(tool.getId());
                Q_CHECK_PTR(spl);
                const QString splP1Name = data->GeometricObject<const VPointF *>(spl->GetP1().id())->name();
                const QString splP4Name = data->GeometricObject<const VPointF *>(spl->GetP4().id())->name();
                return QString(tr("Curve %1_%2")).arg(splP1Name, splP4Name);
            }
            case Valentina::ArcTool:
            {
                const VArc *arc = data->GeometricObject<const VArc *>(tool.getId());
                Q_CHECK_PTR(arc);
                const QString arcCenterName = data->GeometricObject<const VArc *>(arc->GetCenter().id())->name();
                return QString(tr("Arc with center in point %1")).arg(arcCenterName);
            }
            case Valentina::SplinePathTool:
            {
                const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(tool.getId());
                Q_CHECK_PTR(splPath);
                const QVector<VSplinePoint> points = splPath->GetSplinePath();
                QString record;
                if (points.size() != 0 )
                {
                    const QString pName = data->GeometricObject<const VPointF *>(points.at(0).P().id())->name();
                    record = QString(tr("Curve point %1")).arg(pName);
                    if (points.size() > 1)
                    {
                        const QString pName = data->GeometricObject<const VPointF *>(points.last().P().id())->name();
                        record.append(QString("_%1").arg(pName));
                    }
                }
                else
                {
                    qWarning()<<"Not enough points in splinepath"<<Q_FUNC_INFO;
                    return QString(tr("Can't create record."));
                }
                return record;
            }
            case Valentina::PointOfContact:
            {
                const quint32 center = doc->GetParametrUInt(domElement, VAbstractTool::AttrCenter, "0");
                const quint32 firstPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
                const QString centerName = data->GeometricObject<const VPointF *>(center)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%4 - point of contact of arc with the center in point %1 and line %2_%3")).arg(
                            centerName, firstPointIdName, secondPointIdName, toolIdName);
            }
            case Valentina::Height:
            {
                const quint32 basePointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrBasePoint, "0");
                const quint32 p1LineId = doc->GetParametrUInt(domElement, VAbstractTool::AttrP1Line, "0");
                const quint32 p2LineId = doc->GetParametrUInt(domElement, VAbstractTool::AttrP2Line, "0");
                const QString basePointIdName = data->GeometricObject<const VPointF *>(basePointId)->name();
                const QString p1LineIdName = data->GeometricObject<const VPointF *>(p1LineId)->name();
                const QString p2LineIdName = data->GeometricObject<const VPointF *>(p2LineId)->name();
                return QString(tr("Point of perpendicular from point %1 to line %2_%3")).arg(basePointIdName,
                                                                                             p1LineIdName,
                                                                                             p2LineIdName);
            }
            case Valentina::Triangle:
            {
                const quint32 axisP1Id = doc->GetParametrUInt(domElement, VAbstractTool::AttrAxisP1, "0");
                const quint32 axisP2Id = doc->GetParametrUInt(domElement, VAbstractTool::AttrAxisP2, "0");
                const quint32 firstPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString axisP1IdName = data->GeometricObject<const VPointF *>(axisP1Id)->name();
                const QString axisP2IdName = data->GeometricObject<const VPointF *>(axisP2Id)->name();
                const QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                return QString(tr("Triangle: axis %1_%2, points %3 and %4")).arg(axisP1IdName, axisP2IdName,
                                                                                 firstPointIdName, secondPointIdName);
            }
            case Valentina::PointOfIntersection:
            {
                const quint32 firstPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrFirstPoint, "0");
                const quint32 secondPointId = doc->GetParametrUInt(domElement, VAbstractTool::AttrSecondPoint, "0");
                const QString firstPointIdName = data->GeometricObject<const VPointF *>(firstPointId)->name();
                const QString secondPointIdName = data->GeometricObject<const VPointF *>(secondPointId)->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%1 - point of intersection %2 and %3")).arg(toolIdName, firstPointIdName,
                                                                               secondPointIdName);
            }
            case Valentina::CutArcTool:
            {
                const quint32 arcId = doc->GetParametrUInt(domElement, VToolCutArc::AttrArc, "0");
                const VArc *arc = data->GeometricObject<const VArc *>(arcId);
                Q_CHECK_PTR(arc);
                const QString arcCenterName = data->GeometricObject<const VArc *>(arc->GetCenter().id())->name();
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                return QString(tr("%1 - cut arc with center %2")).arg(toolIdName, arcCenterName);
            }
            case Valentina::CutSplineTool:
            {
                const quint32 splineId = doc->GetParametrUInt(domElement, VToolCutSpline::AttrSpline, "0");
                const VSpline *spl = data->GeometricObject<const VSpline *>(splineId);
                Q_CHECK_PTR(spl);
                const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                const QString splP1Name = data->GeometricObject<const VPointF *>(spl->GetP1().id())->name();
                const QString splP4Name = data->GeometricObject<const VPointF *>(spl->GetP4().id())->name();
                return QString(tr("%1 - cut curve %2_%3")).arg(toolIdName, splP1Name, splP4Name);
            }
            case Valentina::CutSplinePathTool:
            {
                const quint32 splinePathId = doc->GetParametrUInt(domElement, VToolCutSplinePath::AttrSplinePath, "0");
                const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(splinePathId);
                Q_CHECK_PTR(splPath);
                const QVector<VSplinePoint> points = splPath->GetSplinePath();
                QString record;
                if (points.size() != 0 )
                {
                    const QString toolIdName = data->GeometricObject<const VPointF *>(tool.getId())->name();
                    const QString pName = data->GeometricObject<const VPointF *>(points.at(0).P().id())->name();
                    record = QString(tr("%1 - cut curve path %2")).arg(toolIdName, pName);
                    if (points.size() > 1)
                    {
                        const QString pName = data->GeometricObject<const VPointF *>(points.last().P().id())->name();
                        const QString name = QString("_%1").arg(pName);
                        record.append(name);
                    }
                }
                else
                {
                    qWarning()<<"Not enough points in splinepath"<<Q_FUNC_INFO;
                    return QString(tr("Can't create record."));
                }
                return record;
            }
            //Because "history" not only show history of pattern, but help restore current data for each pattern's
            //piece, we need add record about details and nodes, but don't show them.
            case Valentina::DetailTool:
                break;
            case Valentina::UnionDetails:
                break;
            case Valentina::NodeArc:
                break;
            case Valentina::NodePoint:
                break;
            case Valentina::NodeSpline:
                break;
            case Valentina::NodeSplinePath:
                break;
            default:
                qWarning()<<tr("Got wrong tool type. Ignore.");
                break;
        }
    }
    catch (const VExceptionBadId &e)
    {
        qWarning()<<e.ErrorMessage()<<Q_FUNC_INFO;
        return QString(tr("Can't create record."));
    }
    return QString();
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
        quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, Qt::green, true);
    }
}

void DialogHistory::closeEvent(QCloseEvent *event)
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    DialogTool::closeEvent(event);
}
