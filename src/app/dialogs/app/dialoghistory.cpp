/************************************************************************
 **
 **  @file   dialoghistory.cpp
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

#include "dialoghistory.h"
#include "ui_dialoghistory.h"
#include "../../geometry/varc.h"
#include "../../geometry/vsplinepath.h"
#include "../../geometry/vpointf.h"
#include "../../tools/vabstracttool.h"
#include "../../tools/drawTools/vtoolcutspline.h"
#include "../../tools/drawTools/vtoolcutsplinepath.h"
#include "../../tools/drawTools/vtoolcutarc.h"
#include "../../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogHistory create dialog
 * @param data container with data
 * @param doc dom document container
 * @param parent parent widget
 */
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
    connect(doc, &VPattern::ChangedActivPP, this, &DialogHistory::UpdateHistory);
    ShowPoint();
}

//---------------------------------------------------------------------------------------------------------------------
DialogHistory::~DialogHistory()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogHistory::DialogAccepted()
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief cellClicked changed history record
 * @param row number row in table
 * @param column number column in table
 */
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedCursor changed cursor of input. Cursor show after what record we will insert new object
 * @param id id of object
 */
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateHistory update history table
 */
void DialogHistory::UpdateHistory()
{
    FillTable();
    InitialTable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillTable fill table
 */
void DialogHistory::FillTable()
{
    ui->tableWidget->clear();
    const QVector<VToolRecord> *history = doc->getHistory();
    SCASSERT(history != nullptr);
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
        SCASSERT(item != nullptr);
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Record return description for record
 * @param tool record data
 * @return description
 */
QString DialogHistory::Record(const VToolRecord &tool)
{
    const QDomElement domElem = doc->elementById(QString().setNum(tool.getId()));
    if (domElem.isElement() == false)
    {
        qDebug()<<"Can't find element by id"<<Q_FUNC_INFO;
        return QString(tr("Can't create record."));
    }
    try
    {
        switch ( tool.getTypeTool() )
        {
            case Tool::ArrowTool:
                Q_UNREACHABLE();
                break;
            case Tool::SinglePointTool:
            {
                return QString(tr("%1 - Base point")).arg(PointName(tool.getId()));
            }
            case Tool::EndLineTool:
            {
                return QString(tr("%1_%2 - Line from point %1 to point %2"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrBasePoint)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::LineTool:
            {
                return QString(tr("%1_%2 - Line from point %1 to point %2"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)));
            }
            case Tool::AlongLineTool:
            {
                return QString(tr("%3 - Point along line %1_%2"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::ShoulderPointTool:
            {
                return QString(tr("%1 - Point of shoulder")).arg(PointName(tool.getId()));
            }
            case Tool::NormalTool:
            {
                return QString(tr("%3 - normal to line %1_%2"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::BisectorTool:
            {
                return QString(tr("%4 - bisector of angle %1_%2_%3"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrThirdPoint)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::LineIntersectTool:
            {
                return QString(tr("%5 - intersection of lines %1_%2 and %3_%4"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP1Line1)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP2Line1)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP1Line2)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP2Line2)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::SplineTool:
            {
                const VSpline *spl = data->GeometricObject<const VSpline *>(tool.getId());
                SCASSERT(spl != nullptr);
                return QString(tr("Curve %1_%2")).arg(PointName(spl->GetP1().id())).arg(PointName(spl->GetP4().id()));
            }
            case Tool::ArcTool:
            {
                const VArc *arc = data->GeometricObject<const VArc *>(tool.getId());
                SCASSERT(arc != nullptr);
                return QString(tr("Arc with center in point %1")).arg(PointName(arc->GetCenter().id()));
            }
            case Tool::SplinePathTool:
            {
                const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(tool.getId());
                SCASSERT(splPath != nullptr);
                const QVector<VSplinePoint> points = splPath->GetSplinePath();
                QString record;
                if (points.size() != 0 )
                {
                    // We use only first and last point name in curve
                    record = QString(tr("Curve point %1")).arg(PointName(points.at(0).P().id()));
                    if (points.size() > 1)
                    {
                        record.append(QString("_%1").arg(PointName(points.last().P().id())));
                    }
                }
                else
                {
                    qDebug()<<"Not enough points in splinepath"<<Q_FUNC_INFO;
                    return QString(tr("Can't create record."));
                }
                return record;
            }
            case Tool::PointOfContact:
            {
                return QString(tr("%4 - point of contact of arc with the center in point %1 and line %2_%3"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrCenter)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            }
            case Tool::Height:
            {
                return QString(tr("Point of perpendicular from point %1 to line %2_%3"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrBasePoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP1Line)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrP2Line)));
            }
            case Tool::Triangle:
            {
                return QString(tr("Triangle: axis %1_%2, points %3 and %4"))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrAxisP1)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrAxisP2)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)));
            }
            case Tool::PointOfIntersection:
            {
                return QString(tr("%1 - point of intersection %2 and %3"))
                        .arg(PointName(tool.getId()))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, VAbstractTool::AttrSecondPoint)));
            }
            case Tool::CutArcTool:
            {
                const VArc *arc = data->GeometricObject<const VArc *>(AttrUInt(domElem, VToolCutArc::AttrArc));
                SCASSERT(arc != nullptr);
                return QString(tr("%1 - cut arc with center %2"))
                        .arg(PointName(tool.getId()))
                        .arg(PointName(arc->GetCenter().id()));
            }
            case Tool::CutSplineTool:
            {
                const quint32 splineId = AttrUInt(domElem, VToolCutSpline::AttrSpline);
                const VSpline *spl = data->GeometricObject<const VSpline *>(splineId);
                SCASSERT(spl != nullptr);
                return QString(tr("%1 - cut curve %2_%3"))
                        .arg(PointName(tool.getId()))
                        .arg(PointName(spl->GetP1().id()))
                        .arg(PointName(spl->GetP4().id()));
            }
            case Tool::CutSplinePathTool:
            {
                const quint32 splinePathId = AttrUInt(domElem, VToolCutSplinePath::AttrSplinePath);
                const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(splinePathId);
                SCASSERT(splPath != nullptr);
                const QVector<VSplinePoint> points = splPath->GetSplinePath();
                QString record;
                if (points.size() != 0 )
                {
                    record = QString(tr("%1 - cut curve path %2"))
                            .arg(PointName(tool.getId()))
                            .arg(PointName(points.at(0).P().id()));
                    if (points.size() > 1)
                    {
                        record.append(QString("_%1").arg(PointName(points.last().P().id())));
                    }
                }
                else
                {
                    qDebug()<<"Not enough points in splinepath"<<Q_FUNC_INFO;
                    return QString(tr("Can't create record."));
                }
                return record;
            }
            //Because "history" not only show history of pattern, but help restore current data for each pattern's
            //piece, we need add record about details and nodes, but don't show them.
            case Tool::DetailTool:
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
                qDebug()<<"Got wrong tool type. Ignore.";
                return QString(tr("Can't create record."));
                break;
        }
    }
    catch (const VExceptionBadId &e)
    {
        qDebug()<<e.ErrorMessage()<<Q_FUNC_INFO;
        return QString(tr("Can't create record."));
    }
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief InitialTable set initial option of table
 */
void DialogHistory::InitialTable()
{
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(" "));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Tool")));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowPoint show selected point
 */
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogHistory::PointName return point name by id.
 *
 * Refacoring what hide ugly string getting point name by id.
 * @param pointId point if in data.
 * @return point name.
 */
QString DialogHistory::PointName(quint32 pointId)
{
    return data->GeometricObject<const VPointF *>(pointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogHistory::AttrUInt(const QDomElement &domElement, const QString &name)
{
    return doc->GetParametrUInt(domElement, name, "0");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle when windows is closing
 * @param event event
 */
void DialogHistory::closeEvent(QCloseEvent *event)
{
    QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
    quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
    emit ShowHistoryTool(id, Qt::green, false);
    DialogTool::closeEvent(event);
}
