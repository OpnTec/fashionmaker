/************************************************************************
 **
 **  @file   dialogpiecepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
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

#include "dialogpiecepath.h"
#include "ui_dialogpiecepath.h"
#include "../vpatterndb/vpiecenode.h"
#include "visualization/path/vistoolpiecepath.h"

#include <QMenu>

//---------------------------------------------------------------------------------------------------------------------
DialogPiecePath::DialogPiecePath(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
    ui(new Ui::DialogPiecePath),
    m_showMode(false)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogPiecePath::NameChanged);

    InitPathTypes();
    connect(ui->comboBoxType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](){ValidObjects(PathIsValid());});

    flagName = true;//We have default name of piece.
    flagError = PathIsValid();
    CheckState();

    if (not m_showMode)
    {
        vis = new VisToolPiecePath(data);
    }
    else
    {
        ui->comboBoxType->setDisabled(true);
        ui->comboBoxPiece->setDisabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DialogPiecePath::~DialogPiecePath()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::DisableShowMode(bool disable)
{
    m_showMode = disable;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not prepare)
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

        ValidObjects(PathIsValid());

        if (not m_showMode)
        {
            auto visPath = qobject_cast<VisToolPiecePath *>(vis);
            SCASSERT(visPath != nullptr);
            const VPiecePath p = CreatePath();
            visPath->SetPath(p);

            if (p.CountNodes() == 1)
            {
                emit ToolTip(tr("Select main path objects, <b>Shift</b> - reverse direction curve, "
                                "<b>Enter</b> - finish creation"));

                visPath->VisualMode(NULL_ID);
            }
            else
            {
                visPath->RefreshGeometry();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ShowDialog(bool click)
{
    if (click == false)
    {
        emit ToolTip("");
        prepare = true;

        if (not m_showMode)
        {
            auto visPath = qobject_cast<VisToolPiecePath *>(vis);
            SCASSERT(visPath != nullptr);
            visPath->SetMode(Mode::Show);
            visPath->RefreshGeometry();
        }
        setModal(true);
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SaveData()
{}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagName && flagError);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ShowContextMenu(const QPoint &pos)
{
    const int row = ui->listWidget->currentRow();
    if (ui->listWidget->count() == 0 || row == -1 || row >= ui->listWidget->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    actionDelete->setEnabled(m_showMode);//Because we can't undo this operation when creating a piece.

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
        ValidObjects(PathIsValid());
    }
    else if (selectedAction == actionReverse)
    {
        rowNode.SetReverse(not rowNode.GetReverse());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode));
        ValidObjects(PathIsValid());
    }

    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ListChanged()
{
    if (not m_showMode)
    {
        auto visPath = qobject_cast<VisToolPiecePath *>(vis);
        SCASSERT(visPath != nullptr);
        visPath->SetPath(CreatePath());
        visPath->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::NameChanged()
{
    if (ui->lineEditName->text().isEmpty())
    {
        flagName = false;
        ChangeColor(ui->labelName, Qt::red);
    }
    else
    {
        flagName = true;
        ChangeColor(ui->labelName, okColor);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitPathTypes()
{
    ui->comboBoxType->addItem(tr("Custom seam allowance"), static_cast<int>(PiecePathType::CutomSeamAllowance));
    //ui->comboBoxType->addItem(tr("Internal path"), static_cast<int>(PiecePathType::InternalPath));
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath DialogPiecePath::GetPiecePath() const
{
    return CreatePath();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPiecePath(const VPiecePath &path)
{
    ui->listWidget->clear();
    for (int i = 0; i < path.CountNodes(); ++i)
    {
        NewItem(path.at(i));
    }

    ValidObjects(PathIsValid());

    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
PiecePathType DialogPiecePath::GetType() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const PiecePathType type =
            static_cast<PiecePathType>(ui->comboBoxType->itemData(ui->comboBoxType->currentIndex()).toInt());
#else
    const PiecePathType type = static_cast<PiecePathType>(ui->comboBoxType->currentData().toInt());
#endif

    return type;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetType(PiecePathType type)
{
    const qint32 index = ui->comboBoxType->findData(static_cast<int>(type));
    if (index != -1)
    {
        ui->comboBoxType->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath DialogPiecePath::CreatePath() const
{
    VPiecePath path;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        path.Append(qvariant_cast<VPieceNode>(item->data(Qt::UserRole)));
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPiecePath::PathIsValid() const
{
    QString url = DialogWarningIcon();

    if(CreatePath().PathPoints(data).count() < 2)
    {
        url += tr("You need more points!");
        ui->helpLabel->setText(url);
        return false;
    }
    else
    {
        if (GetType() == PiecePathType::CutomSeamAllowance && FirstPointEqualLast(ui->listWidget))
        {
            url += tr("First point cannot be equal to the last point!");
            ui->helpLabel->setText(url);
            return false;
        }
        else if (DoublePoints(ui->listWidget))
        {
            url += tr("You have double points!");
            ui->helpLabel->setText(url);
            return false;
        }
    }

    if (not m_showMode && ui->comboBoxPiece->count() <= 0)
    {
        url += tr("List of pieces is empty!");
        ui->helpLabel->setText(url);
        return false;
    }

    ui->helpLabel->setText(tr("Ready!"));
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ValidObjects(bool value)
{
    flagError = value;
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::NewItem(const VPieceNode &node)
{
    NewNodeItem(ui->listWidget, node);
}
