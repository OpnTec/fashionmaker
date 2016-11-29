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

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditName->setClearButtonEnabled(true);
#endif

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogPiecePath::NameChanged);

    InitPathTypes();
    connect(ui->comboBoxType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](){ValidObjects(PathIsValid());});

    flagName = true;//We have default name of piece.
    flagError = PathIsValid();
    CheckState();

    const QString suffix = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ui->doubleSpinBoxSeams->setSuffix(suffix);
    ui->doubleSpinBoxSABefore->setSuffix(suffix);
    ui->doubleSpinBoxSAAfter->setSuffix(suffix);

    if(qApp->patternUnit() == Unit::Inch)
    {
        ui->doubleSpinBoxSeams->setDecimals(5);
        ui->doubleSpinBoxSABefore->setDecimals(5);
        ui->doubleSpinBoxSAAfter->setDecimals(5);
    }

    InitNodesList();
    connect(ui->comboBoxNodes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogPiecePath::NodeChanged);

    connect(ui->pushButtonDefBefore, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefBefore);
    connect(ui->pushButtonDefAfter, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefAfter);

    connect(ui->doubleSpinBoxSeams, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](){NodeChanged(ui->comboBoxNodes->currentIndex());});

    connect(ui->doubleSpinBoxSABefore, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogPiecePath::ChangedSABefore);
    connect(ui->doubleSpinBoxSAAfter, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogPiecePath::ChangedSAAfter);

    InitNodeAngles(ui->comboBoxAngle);
    connect(ui->comboBoxAngle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogPiecePath::NodeAngleChanged);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &DialogPiecePath::ShowContextMenu);

    vis = new VisToolPiecePath(data);

    ui->tabWidget->removeTab(1);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPiecePath::~DialogPiecePath()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EnbleShowMode(bool disable)
{
    m_showMode = disable;
    ui->comboBoxType->setDisabled(m_showMode);
    ui->comboBoxPiece->setDisabled(m_showMode);
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
        if (CreatePath().CountNodes() > 0)
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

    InitNodesList();
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
void DialogPiecePath::NodeChanged(int index)
{
    ui->doubleSpinBoxSABefore->setDisabled(true);
    ui->doubleSpinBoxSAAfter->setDisabled(true);
    ui->pushButtonDefBefore->setDisabled(true);
    ui->pushButtonDefAfter->setDisabled(true);
    ui->comboBoxAngle->setDisabled(true);

    ui->doubleSpinBoxSABefore->blockSignals(true);
    ui->doubleSpinBoxSAAfter->blockSignals(true);
    ui->comboBoxAngle->blockSignals(true);

    if (index != -1)
    {
    #if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
        const quint32 id = ui->comboBoxNodes->itemData(index).toUInt();
    #else
        const quint32 id = ui->comboBoxNodes->currentData().toUInt();
    #endif
        const VPiecePath path = CreatePath();
        const int nodeIndex = path.indexOfNode(id);
        if (nodeIndex != -1)
        {
            const VPieceNode &node = path.at(nodeIndex);

            ui->doubleSpinBoxSABefore->setEnabled(true);
            ui->doubleSpinBoxSAAfter->setEnabled(true);
            ui->comboBoxAngle->setEnabled(true);

            qreal w1 = node.GetSABefore();
            if (w1 < 0)
            {
                w1 = ui->doubleSpinBoxSeams->value();
            }
            else
            {
                ui->pushButtonDefBefore->setEnabled(true);
            }
            ui->doubleSpinBoxSABefore->setValue(w1);

            qreal w2 = node.GetSAAfter();
            if (w2 < 0)
            {
                w2 = ui->doubleSpinBoxSeams->value();
            }
            else
            {
                ui->pushButtonDefAfter->setEnabled(true);
            }
            ui->doubleSpinBoxSAAfter->setValue(w2);

            const int index = ui->comboBoxAngle->findData(static_cast<unsigned char>(node.GetAngleType()));
            if (index != -1)
            {
                ui->comboBoxAngle->setCurrentIndex(index);
            }
        }
    }
    else
    {
        ui->doubleSpinBoxSABefore->setValue(0);
        ui->doubleSpinBoxSAAfter->setValue(0);
        ui->comboBoxAngle->setCurrentIndex(-1);
    }

    ui->doubleSpinBoxSABefore->blockSignals(false);
    ui->doubleSpinBoxSAAfter->blockSignals(false);
    ui->comboBoxAngle->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefBefore()
{
    SetCurrentSABefore(-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefAfter()
{
    SetCurrentSAAfter(-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ChangedSABefore(double d)
{
    SetCurrentSABefore(d);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ChangedSAAfter(double d)
{
    SetCurrentSAAfter(d);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitPathTypes()
{
    ui->comboBoxType->addItem(tr("Custom seam allowance"), static_cast<int>(PiecePathType::CustomSeamAllowance));
    //ui->comboBoxType->addItem(tr("Internal path"), static_cast<int>(PiecePathType::InternalPath));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitNodesList()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const quint32 id = ui->comboBoxNodes->itemData(ui->comboBoxNodes->currentIndex()).toUInt();
#else
    const quint32 id = ui->comboBoxNodes->currentData().toUInt();
#endif

    ui->comboBoxNodes->blockSignals(true);
    ui->comboBoxNodes->clear();

    const VPiecePath path = CreatePath();

    for (int i = 0; i < path.CountNodes(); ++i)
    {
        const VPieceNode node = path.at(i);
        if (node.GetTypeTool() == Tool::NodePoint)
        {
            const QString name = GetNodeName(node);

            ui->comboBoxNodes->addItem(name, node.GetId());
        }
    }
    ui->comboBoxNodes->blockSignals(false);

    const int index = ui->comboBoxNodes->findData(id);
    if (index != -1)
    {
        ui->comboBoxNodes->setCurrentIndex(index);
        NodeChanged(index);// Need in case combox index was not changed
    }
    else
    {
        ui->comboBoxNodes->count() > 0 ? NodeChanged(0) : NodeChanged(-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::NodeAngleChanged(int index)
{
    const int i = ui->comboBoxNodes->currentIndex();
    if (i != -1 && index != -1)
    {
    #if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
        const quint32 id = ui->comboBoxNodes->itemData(i).toUInt();
    #else
        const quint32 id = ui->comboBoxNodes->currentData().toUInt();
    #endif

        QListWidgetItem *rowItem = GetItemById(id);
        if (rowItem)
        {
        #if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
            const PieceNodeAngle angle = static_cast<PieceNodeAngle>(ui->comboBoxAngle->itemData(index).toUInt());
        #else
            const PieceNodeAngle angle = static_cast<PieceNodeAngle>(ui->comboBoxAngle->currentData().toUInt());
        #endif

            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetAngleType(angle);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
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

    SetType(path.GetType());
    ui->lineEditName->setText(path.GetName());

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
QListWidgetItem *DialogPiecePath::GetItemById(quint32 id)
{
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        const VPieceNode node = qvariant_cast<VPieceNode>(item->data(Qt::UserRole));

        if (node.GetId() == id)
        {
            return item;
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetCurrentSABefore(qreal value)
{
    const int index = ui->comboBoxNodes->currentIndex();
    if (index != -1)
    {
    #if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
        const quint32 id = ui->comboBoxNodes->itemData(index).toUInt();
    #else
        const quint32 id = ui->comboBoxNodes->currentData().toUInt();
    #endif

        QListWidgetItem *rowItem = GetItemById(id);
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetSABefore(value);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetCurrentSAAfter(qreal value)
{
    const int index = ui->comboBoxNodes->currentIndex();
    if (index != -1)
    {
    #if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
        const quint32 id = ui->comboBoxNodes->itemData(index).toUInt();
    #else
        const quint32 id = ui->comboBoxNodes->currentData().toUInt();
    #endif

        QListWidgetItem *rowItem = GetItemById(id);
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetSAAfter(value);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPiecePath::GetPieceId() const
{
    quint32 id = NULL_ID;

    if (ui->comboBoxPiece->count() > 0)
    {
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    id = ui->comboBoxPiece->itemData(ui->comboBoxPiece->currentIndex()).toUInt();
#else
    id = ui->comboBoxPiece->currentData().toUInt();
#endif
    }

    return id;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPieceId(quint32 id)
{
    if (ui->comboBoxPiece->count() <= 0)
    {
        const VPiece piece = data->GetPiece(id);
        ui->comboBoxPiece->addItem(piece.GetName(), id);
    }
    else
    {
        const qint32 index = ui->comboBoxPiece->findData(id);
        if (index != -1)
        {
            ui->comboBoxType->setCurrentIndex(index);
        }
    }

    ValidObjects(PathIsValid());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetSAWidth(qreal width)
{
    if (width >=0)
    {
        ui->tabWidget->addTab(ui->tabSeamAllowance, tr("Seam allowance"));
        ui->doubleSpinBoxSeams->setValue(width);
    }
    else
    {
        ui->tabWidget->removeTab(1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPiecesList(const QVector<quint32> &list)
{
    for (int i=0; i < list.size(); ++i)
    {
        const VPiece piece = data->GetPiece(list.at(i));
        ui->comboBoxPiece->addItem(piece.GetName(), list.at(i));
    }
    ValidObjects(PathIsValid());
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

    path.SetType(GetType());
    path.SetName(ui->lineEditName->text());

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
        if (GetType() == PiecePathType::CustomSeamAllowance && FirstPointEqualLast(ui->listWidget))
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
