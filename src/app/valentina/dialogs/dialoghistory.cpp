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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../vgeometry/varc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vpointf.h"
#include "../vtools/tools/vabstracttool.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutspline.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutsplinepath.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutarc.h"
#include "../xml/vpattern.h"
#include "../vmisc/diagnostic.h"
#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogHistory create dialog
 * @param data container with data
 * @param doc dom document container
 * @param parent parent widget
 */
DialogHistory::DialogHistory(VContainer *data, VPattern *doc, QWidget *parent)
    :DialogTool(data, 0, parent), ui(new Ui::DialogHistory), doc(doc), cursorRow(0),
    cursorToolRecordRow(0)
{
    ui->setupUi(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());

    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogHistory::DialogAccepted);
    FillTable();
    InitialTable();
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &DialogHistory::cellClicked);
    connect(this, &DialogHistory::ShowHistoryTool, [doc](quint32 id, bool enable){emit doc->ShowTool(id, enable);});
    connect(doc, &VPattern::ChangedCursor, this, &DialogHistory::ChangedCursor);
    connect(doc, &VPattern::patternChanged, this, &DialogHistory::UpdateHistory);
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
    emit ShowHistoryTool(id, false);
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
        const quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        doc->blockSignals(true);
        row == ui->tableWidget->rowCount()-1 ? doc->setCursor(0) : doc->setCursor(id);
        doc->blockSignals(false);
    }
    else
    {
        QTableWidgetItem *item = ui->tableWidget->item(cursorToolRecordRow, 0);
        quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, false);

        cursorToolRecordRow = row;
        item = ui->tableWidget->item(cursorToolRecordRow, 0);
        id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedCursor changed cursor of input. Cursor show after which record we will insert new object
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
    QVector<VToolRecord> history = doc->getLocalHistory();
    qint32 currentRow = -1;
    qint32 count = 0;
    ui->tableWidget->setRowCount(history.size());//Make row count max possible number
    for (qint32 i = 0; i< history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        const QString historyRecord = Record(tool);
        if (not historyRecord.isEmpty())
        {
            currentRow++;

            {
                QTableWidgetItem *item = new QTableWidgetItem(QString());
                item->setTextAlignment(Qt::AlignHCenter);
                item->setData(Qt::UserRole, tool.getId());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(currentRow, 0, item);
            }

            QTableWidgetItem *item = new QTableWidgetItem(historyRecord);
            item->setFont(QFont("Times", 12, QFont::Bold));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(currentRow, 1, item);
            ++count;
        }
    }
    ui->tableWidget->setRowCount(count);//Real row count
    if (count>0)
    {
        cursorRow = CursorRow();
        QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
        SCASSERT(item != nullptr);
        item->setIcon(QIcon("://icon/32x32/put_after.png"));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
/**
 * @brief Record return description for record
 * @param tool record data
 * @return description
 */
QString DialogHistory::Record(const VToolRecord &tool)
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 49, "Not all tools was used in history.");

    const QDomElement domElem = doc->elementById(tool.getId());
    if (domElem.isElement() == false)
    {
        qDebug()<<"Can't find element by id"<<Q_FUNC_INFO;
        return tr("Can't create record.");
    }
    try
    {
        switch ( tool.getTypeTool() )
        {
            case Tool::Arrow:
            case Tool::SinglePoint:
            case Tool::DoublePoint:
            case Tool::LinePoint:
            case Tool::AbstractSpline:
            case Tool::Cut:
            case Tool::Midpoint:// Same as Tool::AlongLine, but tool will never has such type
            case Tool::ArcIntersectAxis:// Same as Tool::CurveIntersectAxis, but tool will never has such type
            case Tool::LAST_ONE_DO_NOT_USE:
                Q_UNREACHABLE(); //-V501
                break;
            case Tool::BasePoint:
                return tr("%1 - Base point").arg(PointName(tool.getId()));
            case Tool::EndLine:
                return tr("%1_%2 - Line from point %1 to point %2")
                        .arg(PointName(AttrUInt(domElem, AttrBasePoint)))
                        .arg(PointName(tool.getId()));
            case Tool::Line:
                return tr("%1_%2 - Line from point %1 to point %2")
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)));
            case Tool::AlongLine:
                return tr("%3 - Point along line %1_%2")
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            case Tool::ShoulderPoint:
                return tr("%1 - Point of shoulder").arg(PointName(tool.getId()));
            case Tool::Normal:
                return tr("%3 - normal to line %1_%2")
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            case Tool::Bisector:
                return tr("%4 - bisector of angle %1_%2_%3")
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrThirdPoint)))
                        .arg(PointName(tool.getId()));
            case Tool::LineIntersect:
                return tr("%5 - intersection of lines %1_%2 and %3_%4")
                        .arg(PointName(AttrUInt(domElem, AttrP1Line1)))
                        .arg(PointName(AttrUInt(domElem, AttrP2Line1)))
                        .arg(PointName(AttrUInt(domElem, AttrP1Line2)))
                        .arg(PointName(AttrUInt(domElem, AttrP2Line2)))
                        .arg(PointName(tool.getId()));
            case Tool::Spline:
            {
                const QSharedPointer<VSpline> spl = data->GeometricObject<VSpline>(tool.getId());
                SCASSERT(spl != nullptr);
                return spl->NameForHistory(tr("Curve"));
            }
            case Tool::CubicBezier:
            {
                const QSharedPointer<VCubicBezier> spl = data->GeometricObject<VCubicBezier>(tool.getId());
                SCASSERT(spl != nullptr);
                return spl->NameForHistory(tr("Cubic bezier curve"));
            }
            case Tool::Arc:
            {
                const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(tool.getId());
                SCASSERT(arc != nullptr);
                return arc->NameForHistory(tr("Arc"));
            }
            case Tool::ArcWithLength:
            {
                const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(tool.getId());
                SCASSERT(arc != nullptr);
                return tr("%1 with length %2")
                        .arg(arc->NameForHistory(tr("Arc")))
                        .arg(arc->GetLength());
            }
            case Tool::SplinePath:
            {
                const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(tool.getId());
                SCASSERT(splPath != nullptr);
                return splPath->NameForHistory(tr("Spline path"));
            }
            case Tool::CubicBezierPath:
            {
                const QSharedPointer<VCubicBezierPath> splPath = data->GeometricObject<VCubicBezierPath>(tool.getId());
                SCASSERT(splPath != nullptr);
                return splPath->NameForHistory(tr("Cubic bezier curve path"));
            }
            case Tool::PointOfContact:
                return tr("%4 - point of contact of arc with the center in point %1 and line %2_%3")
                        .arg(PointName(AttrUInt(domElem, AttrCenter)))
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)))
                        .arg(PointName(tool.getId()));
            case Tool::Height:
                return tr("Point of perpendicular from point %1 to line %2_%3")
                        .arg(PointName(AttrUInt(domElem, AttrBasePoint)))
                        .arg(PointName(AttrUInt(domElem, AttrP1Line)))
                        .arg(PointName(AttrUInt(domElem, AttrP2Line)));
            case Tool::Triangle:
                return tr("Triangle: axis %1_%2, points %3 and %4")
                        .arg(PointName(AttrUInt(domElem, AttrAxisP1)))
                        .arg(PointName(AttrUInt(domElem, AttrAxisP2)))
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)));
            case Tool::PointOfIntersection:
                return tr("%1 - point of intersection %2 and %3")
                        .arg(PointName(tool.getId()))
                        .arg(PointName(AttrUInt(domElem, AttrFirstPoint)))
                        .arg(PointName(AttrUInt(domElem, AttrSecondPoint)));
            case Tool::CutArc:
            {
                const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(AttrUInt(domElem, AttrArc));
                SCASSERT(arc != nullptr);
                return tr("%1 - cut %2")
                        .arg(PointName(tool.getId()))
                        .arg(arc->NameForHistory(tr("arc")));
            }
            case Tool::CutSpline:
            {
                const quint32 splineId = AttrUInt(domElem, VToolCutSpline::AttrSpline);
                const QSharedPointer<VAbstractCubicBezier> spl = data->GeometricObject<VAbstractCubicBezier>(splineId);
                SCASSERT(spl != nullptr);
                return tr("%1 - cut %2")
                        .arg(PointName(tool.getId()))
                        .arg(spl->NameForHistory(tr("curve")));
            }
            case Tool::CutSplinePath:
            {
                const quint32 splinePathId = AttrUInt(domElem, VToolCutSplinePath::AttrSplinePath);
                const QSharedPointer<VAbstractCubicBezierPath> splPath =
                        data->GeometricObject<VAbstractCubicBezierPath>(splinePathId);
                SCASSERT(splPath != nullptr);
                return tr("%1 - cut %2")
                        .arg(PointName(tool.getId()))
                        .arg(splPath->NameForHistory(tr("curve path")));
            }
            case Tool::LineIntersectAxis:
                return tr("%1 - point of intersection line %2_%3 and axis through point %4")
                        .arg(PointName(tool.getId()))
                        .arg(PointName(AttrUInt(domElem, AttrP1Line)))
                        .arg(PointName(AttrUInt(domElem, AttrP2Line)))
                        .arg(PointName(AttrUInt(domElem, AttrBasePoint)));
            case Tool::CurveIntersectAxis:
                return tr("%1 - point of intersection curve and axis through point %2")
                        .arg(PointName(tool.getId()))
                        .arg(PointName(AttrUInt(domElem, AttrBasePoint)));
            case Tool::PointOfIntersectionArcs:
                return tr("%1 - point of arcs intersection").arg(PointName(tool.getId()));
            case Tool::PointOfIntersectionCircles:
                return tr("%1 - point of circles intersection").arg(PointName(tool.getId()));
            case Tool::PointOfIntersectionCurves:
                return tr("%1 - point of curves intersection").arg(PointName(tool.getId()));
            case Tool::PointFromCircleAndTangent:
                return tr("%1 - point from circle and tangent").arg(PointName(tool.getId()));
            case Tool::PointFromArcAndTangent:
                return tr("%1 - point from arc and tangent").arg(PointName(tool.getId()));
            case Tool::TrueDarts:
                return tr("Correction the dart %1_%2_%3")
                        .arg(PointName(AttrUInt(domElem, AttrDartP1)))
                        .arg(PointName(AttrUInt(domElem, AttrDartP2)))
                        .arg(PointName(AttrUInt(domElem, AttrDartP2)));
            //Because "history" not only show history of pattern, but help restore current data for each pattern's
            //piece, we need add record about details and nodes, but don't show them.
            case Tool::Detail:
            case Tool::Piece:
            case Tool::UnionDetails:
            case Tool::NodeArc:
            case Tool::NodePoint:
            case Tool::NodeSpline:
            case Tool::NodeSplinePath:
            case Tool::Group:
            case Tool::Rotation:
            case Tool::FlippingByLine:
            case Tool::FlippingByAxis:
            case Tool::Move:
                return QString();
        }
    }
    catch (const VExceptionBadId &e)
    {
        qDebug()<<e.ErrorMessage()<<Q_FUNC_INFO;
        return tr("Can't create record.");
    }
    qDebug()<<"Can't create history record for the tool.";
    return tr("Can't create record.");
}

QT_WARNING_POP

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
    const QVector<VToolRecord> *history = doc->getHistory();
    if (history->size()>0)
    {
        QTableWidgetItem *item = ui->tableWidget->item(0, 1);
        item->setSelected(true);
        cursorToolRecordRow = 0;
        item = ui->tableWidget->item(0, 0);
        quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        emit ShowHistoryTool(id, true);
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
    return data->GeometricObject<VPointF>(pointId)->name();
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
    emit ShowHistoryTool(id, false);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogHistory::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        RetranslateUi();
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogHistory::RetranslateUi()
{
    qint32 currentRow = cursorRow;
    UpdateHistory();

    QTableWidgetItem *item = ui->tableWidget->item(cursorRow, 0);
    SCASSERT(item != nullptr);
    item->setIcon(QIcon(""));

    cursorRow = currentRow;
    cellClicked(cursorRow, 0);
}

//---------------------------------------------------------------------------------------------------------------------
int DialogHistory::CursorRow() const
{
    const quint32 cursor = doc->getCursor();
    if (cursor == 0)
    {
        return ui->tableWidget->rowCount()-1;
    }

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        const quint32 id = qvariant_cast<quint32>(item->data(Qt::UserRole));
        if (cursor == id)
        {
            return i;
        }
    }
    return ui->tableWidget->rowCount()-1;
}
