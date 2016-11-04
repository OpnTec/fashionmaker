/************************************************************************
 **
 **  @file   dialogseamallowance.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogseamallowance.h"
#include "ui_dialogseamallowance.h"
#include "../vpatterndb/vpiecenode.h"

#include <QBuffer>
#include <QMenu>

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSeamAllowance),
      m_piece(),
      applyAllowed(false)// By default disabled
{
    ui->setupUi(this);

    InitOkCancelApply(ui);
    EnableApply(applyAllowed);

    flagName = true;//We have default name of piece.
    flagError = MainPathIsValid();
    CheckState();

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &DialogSeamAllowance::ShowContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::~DialogSeamAllowance()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableApply(bool enable)
{
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(enable);
    applyAllowed = enable;
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::GetPiece() const
{
    return m_piece;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPiece(const VPiece &piece)
{
    m_piece = piece;
    ui->listWidget->clear();
    for (int i = 0; i < m_piece.CountNode(); ++i)
    {
        NewItem(m_piece.at(i));
    }

    ValidObjects(MainPathIsValid());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of objects (points, arcs, splines, spline paths)
 * @param type type of object
 */
void DialogSeamAllowance::ChosenObject(quint32 id, const SceneObject &type)
{
    bool reverse = false;
    if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
    {
        reverse = true;
    }
    switch (type)
    {
        case SceneObject::Arc:
            NewItem(VPieceNode(id, Tool::NodeArc, reverse));
            break;
        case SceneObject::Point:
            NewItem(VPieceNode(id, Tool::NodePoint));
            break;
        case SceneObject::Spline:
            NewItem(VPieceNode(id, Tool::NodeSpline, reverse));
            break;
        case SceneObject::SplinePath:
            NewItem(VPieceNode(id, Tool::NodeSplinePath, reverse));
            break;
        case (SceneObject::Line):
        case (SceneObject::Detail):
        case (SceneObject::Unknown):
        default:
            qDebug() << "Got wrong scene object. Ignore.";
            break;
    }

    ValidObjects(MainPathIsValid());
    show();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SaveData()
{
    m_piece.Clear();
    m_piece = CreatePiece();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagName && flagError);
    // In case dialog hasn't apply button
    if ( bApply != nullptr && applyAllowed)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowContextMenu(const QPoint &pos)
{
    const int row = ui->listWidget->currentRow();
    if (ui->listWidget->count() == 0 || row == -1 || row >= ui->listWidget->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QListWidgetItem *rowItem = ui->listWidget->item(row);
    SCASSERT(rowItem != nullptr);
    VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = nullptr;
    if (rowNode.GetTypeTool() != Tool::NodePoint)
    {
        actionReverse = menu->addAction(tr("Reverse"));
        actionReverse->setCheckable(true);
        actionReverse->setChecked(rowNode.GetReverse());
    }

    QAction *selectedAction = menu->exec(ui->listWidget->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidget->item(row);
        ValidObjects(MainPathIsValid());
    }
    else if (selectedAction == actionReverse)
    {
        rowNode.SetReverse(not rowNode.GetReverse());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode));
        ValidObjects(MainPathIsValid());
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::CreatePiece() const
{
    VPiece piece;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        piece.Append(qvariant_cast<VPieceNode>(item->data(Qt::UserRole)));
    }

    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewItem(const VPieceNode &node)
{
    SCASSERT(node.GetId() > NULL_ID);
    QString name;
    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
        case (Tool::NodeArc):
        case (Tool::NodeSpline):
        case (Tool::NodeSplinePath):
        {
            name = GetNodeName(node);
            break;
        }
        default:
            qDebug()<<"Got wrong tools. Ignore.";
            return;
    }



    bool canAddNewPoint = false;

    if(ui->listWidget->count() == 0)
    {
        canAddNewPoint = true;
    }
    else
    {
        if(RowId(ui->listWidget->count()-1) != node.GetId())
        {
            canAddNewPoint = true;
        }
    }

    if(canAddNewPoint)
    {
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(node));
        ui->listWidget->addItem(item);
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogSeamAllowance::RowId(int i) const
{
    const QListWidgetItem *rowItem = ui->listWidget->item(i);
    SCASSERT(rowItem != nullptr);
    const VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
    return rowNode.GetId();
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSeamAllowance::MainPathIsValid() const
{
    const QIcon icon = QIcon::fromTheme("dialog-warning",
                                  QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));

    const QPixmap pixmap = icon.pixmap(QSize(16, 16));
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    QString url = QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + QLatin1String("\"/> ");

    if(CreatePiece().MainPathPoints(data).count() < 3)
    {
        url += tr("You need more points!");
        ui->helpLabel->setText(url);
        return false;
    }
    else
    {
        if(not MainPathIsClockwise())
        {
            url += tr("You have to choose points in a clockwise direction!");
            ui->helpLabel->setText(url);
            return false;
        }
        if (FirstPointEqualLast())
        {
            url += tr("First point cannot be equal to the last point!");
            ui->helpLabel->setText(url);
            return false;
        }
        else
        {
            for (int i=0, sz = ui->listWidget->count()-1; i<sz; ++i)
            {
                if (RowId(i) == RowId(i+1))
                {
                    url += tr("You have double points!");
                    ui->helpLabel->setText(url);
                    return false;
                }
            }
        }
    }
    ui->helpLabel->setText(tr("Ready!"));
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ValidObjects(bool value)
{
    flagError = value;
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSeamAllowance::FirstPointEqualLast() const
{
    if (ui->listWidget->count() > 1)
    {
        if (RowId(0) == RowId(ui->listWidget->count()-1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSeamAllowance::MainPathIsClockwise() const
{
    const QVector<QPointF> points = CreatePiece().MainPathPoints(data);

    if(points.count() < 3)
    {
        return false;
    }

    const qreal res = VPiece::SumTrapezoids(points);
    if (res < 0)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetNodeName(const VPieceNode &node) const
{
    const QSharedPointer<VGObject> obj = data->GeometricObject<VGObject>(node.GetId());
    QString name = obj->name();

    if (node.GetTypeTool() != Tool::NodePoint && node.GetReverse())
    {
        name = QLatin1String("- ") + name;
    }

    return name;
}
