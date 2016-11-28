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
#include "../vpatterndb/vpiecepath.h"
#include "visualization/path/vistoolpiece.h"
#include "dialogpiecepath.h"
#include "../../undocommands/savepiecepathoptions.h"

#include <QMenu>

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSeamAllowance),
      applyAllowed(false),// By default disabled
      m_mx(0),
      m_my(0),
      m_dialog()
{
    ui->setupUi(this);

    InitOkCancelApply(ui);
    EnableApply(applyAllowed);

    flagName = true;//We have default name of piece.
    flagError = MainPathIsValid();
    CheckState();

    ui->checkBoxForbidFlipping->setChecked(qApp->Settings()->GetForbidWorkpieceFlipping());

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
    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    ui->doubleSpinBoxSeams->setValue(UnitConvertor(1, Unit::Cm, qApp->patternUnit()));

    InitNodesList();
    connect(ui->comboBoxNodes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeChanged);

    connect(ui->pushButtonDefBefore, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefBefore);
    connect(ui->pushButtonDefAfter, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefAfter);

    connect(ui->doubleSpinBoxSeams, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](){NodeChanged(ui->comboBoxNodes->currentIndex());});

    connect(ui->doubleSpinBoxSABefore, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSeamAllowance::ChangedSABefore);
    connect(ui->doubleSpinBoxSAAfter, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSeamAllowance::ChangedSAAfter);

    ui->listWidgetMainPath->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetMainPath, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowMainPathContextMenu);
    connect(ui->listWidgetMainPath->model(), &QAbstractItemModel::rowsMoved, this, &DialogSeamAllowance::ListChanged);
    connect(ui->checkBoxSeams, &QCheckBox::toggled, this, &DialogSeamAllowance::EnableSeamAllowance);

    InitNodeAngles(ui->comboBoxAngle);
    connect(ui->comboBoxAngle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeAngleChanged);

    ui->listWidgetCustomSA->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetCustomSA, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowCustomSAContextMenu);
    connect(ui->listWidgetCustomSA, &QListWidget::currentRowChanged, this, &DialogSeamAllowance::CustomSAChanged);
    connect(ui->comboBoxStartPoint, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::CSAStartPointChanged);
    connect(ui->comboBoxEndPoint, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::CSAEndPointChanged);

    if (not applyAllowed)
    {
        vis = new VisToolPiece(data);
    }

    ui->tabWidget->setCurrentIndex(0);// Show always first tab active on start.
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
    ui->tabSeamAllowance->setEnabled(applyAllowed);
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::GetPiece() const
{
    return CreatePiece();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPiece(const VPiece &piece)
{
    ui->listWidgetMainPath->clear();
    for (int i = 0; i < piece.GetPath().CountNodes(); ++i)
    {
        NewMainPathItem(piece.GetPath().at(i));
    }

    ui->listWidgetCustomSA->blockSignals(true);
    ui->listWidgetCustomSA->clear();
    const QVector<CustomSARecord> records = piece.GetCustomSARecords();
    for (int i = 0; i < records.size(); ++i)
    {
        NewCustomSA(records.at(i));
    }
    ui->listWidgetCustomSA->blockSignals(false);

    ui->comboBoxStartPoint->blockSignals(true);
    ui->comboBoxStartPoint->clear();
    ui->comboBoxStartPoint->blockSignals(false);

    ui->comboBoxEndPoint->blockSignals(true);
    ui->comboBoxEndPoint->clear();
    ui->comboBoxEndPoint->blockSignals(false);

    CustomSAChanged(0);

    ui->checkBoxForbidFlipping->setChecked(piece.IsForbidFlipping());
    ui->doubleSpinBoxSeams->setValue(piece.GetSAWidth());
    ui->checkBoxSeams->setChecked(piece.IsSeamAllowance());

    m_mx = piece.GetMx();
    m_my = piece.GetMy();

    ValidObjects(MainPathIsValid());

    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of objects (points, arcs, splines, spline paths)
 * @param type type of object
 */
void DialogSeamAllowance::ChosenObject(quint32 id, const SceneObject &type)
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
                NewMainPathItem(VPieceNode(id, Tool::NodeArc, reverse));
                break;
            case SceneObject::Point:
                NewMainPathItem(VPieceNode(id, Tool::NodePoint));
                break;
            case SceneObject::Spline:
                NewMainPathItem(VPieceNode(id, Tool::NodeSpline, reverse));
                break;
            case SceneObject::SplinePath:
                NewMainPathItem(VPieceNode(id, Tool::NodeSplinePath, reverse));
                break;
            case (SceneObject::Line):
            case (SceneObject::Detail):
            case (SceneObject::Unknown):
            default:
                qDebug() << "Got wrong scene object. Ignore.";
                break;
        }

        ValidObjects(MainPathIsValid());

        if (not applyAllowed)
        {
            auto visPath = qobject_cast<VisToolPiece *>(vis);
            SCASSERT(visPath != nullptr);
            const VPiece p = CreatePiece();
            visPath->SetPiece(p);

            if (p.GetPath().CountNodes() == 1)
            {
                emit ToolTip(tr("Select main path objects clockwise, <b>Shift</b> - reverse direction curve, "
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
void DialogSeamAllowance::ShowDialog(bool click)
{
    if (click == false)
    {
        emit ToolTip("");
        prepare = true;

        if (not applyAllowed)
        {
            auto visPath = qobject_cast<VisToolPiece *>(vis);
            SCASSERT(visPath != nullptr);
            visPath->SetMode(Mode::Show);
            visPath->RefreshGeometry();
        }

        // Fix issue #526. Dialog Detail is not on top after selection second object on Mac.
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SaveData()
{}

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
void DialogSeamAllowance::ShowMainPathContextMenu(const QPoint &pos)
{
    const int row = ui->listWidgetMainPath->currentRow();
    if (ui->listWidgetMainPath->count() == 0 || row == -1 || row >= ui->listWidgetMainPath->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    actionDelete->setEnabled(applyAllowed);//Because we can't undo this operation when creating a piece.

    QListWidgetItem *rowItem = ui->listWidgetMainPath->item(row);
    SCASSERT(rowItem != nullptr);
    VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = nullptr;
    if (rowNode.GetTypeTool() != Tool::NodePoint)
    {
        actionReverse = menu->addAction(tr("Reverse"));
        actionReverse->setCheckable(true);
        actionReverse->setChecked(rowNode.GetReverse());
    }

    QAction *selectedAction = menu->exec(ui->listWidgetMainPath->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetMainPath->item(row);
        ValidObjects(MainPathIsValid());
    }
    else if (selectedAction == actionReverse)
    {
        rowNode.SetReverse(not rowNode.GetReverse());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode));
        ValidObjects(MainPathIsValid());
    }

    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowCustomSAContextMenu(const QPoint &pos)
{
    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = menu->addAction(tr("Reverse"));
    actionReverse->setCheckable(true);
    actionReverse->setChecked(record.reverse);

    QAction *actionOption = menu->addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

    QAction *selectedAction = menu->exec(ui->listWidgetCustomSA->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetCustomSA->item(row);
    }
    else if (selectedAction == actionReverse)
    {
        record.reverse = not record.reverse;
        rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
        rowItem->setText(GetCustomSARecordName(record));
    }
    else if (selectedAction == actionOption)
    {
        auto *dialog = new DialogPiecePath(data, record.path, this);
        dialog->SetPiecePath(data->GetPiecePath(record.path));
        dialog->SetPieceId(toolId);
        dialog->EnbleShowMode(true);
        m_dialog = dialog;
        m_dialog->setModal(true);
        connect(m_dialog.data(), &DialogTool::DialogClosed, this, &DialogSeamAllowance::PathDialogClosed);
        m_dialog->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ListChanged()
{
    if (not applyAllowed)
    {
        auto visPath = qobject_cast<VisToolPiece *>(vis);
        SCASSERT(visPath != nullptr);
        visPath->SetPiece(CreatePiece());
        visPath->RefreshGeometry();
    }
    InitNodesList();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableSeamAllowance(bool enable)
{
    ui->groupBoxAutomatic->setEnabled(enable);
    ui->groupBoxCustom->setEnabled(enable);

    if (enable)
    {
        InitNodesList();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeChanged(int index)
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
        const VPiece piece = CreatePiece();
        const int nodeIndex = piece.GetPath().indexOfNode(id);
        if (nodeIndex != -1)
        {
            const VPieceNode &node = piece.GetPath().at(nodeIndex);

            ui->doubleSpinBoxSABefore->setEnabled(true);
            ui->doubleSpinBoxSAAfter->setEnabled(true);
            ui->comboBoxAngle->setEnabled(true);

            qreal w1 = node.GetSABefore();
            if (w1 < 0)
            {
                w1 = piece.GetSAWidth();
            }
            else
            {
                ui->pushButtonDefBefore->setEnabled(true);
            }
            ui->doubleSpinBoxSABefore->setValue(w1);

            qreal w2 = node.GetSAAfter();
            if (w2 < 0)
            {
                w2 = piece.GetSAWidth();
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
void DialogSeamAllowance::CSAStartPointChanged(int index)
{
    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const quint32 startPoint = ui->comboBoxStartPoint->itemData(index).toUInt();
#else
    Q_UNUSED(index);
    const quint32 startPoint = ui->comboBoxStartPoint->currentData().toUInt();
#endif

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.startPoint = startPoint;
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAEndPointChanged(int index)
{
    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const quint32 endPoint = ui->comboBoxEndPoint->itemData(index).toUInt();
#else
    Q_UNUSED(index);
    const quint32 endPoint = ui->comboBoxEndPoint->currentData().toUInt();
#endif

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.endPoint = endPoint;
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeAngleChanged(int index)
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
void DialogSeamAllowance::ReturnDefBefore()
{
    SetCurrentSABefore(-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefAfter()
{
    SetCurrentSAAfter(-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ChangedSABefore(double d)
{
    SetCurrentSABefore(d);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ChangedSAAfter(double d)
{
    SetCurrentSAAfter(d);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CustomSAChanged(int row)
{
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

    ui->comboBoxStartPoint->blockSignals(true);
    ui->comboBoxEndPoint->blockSignals(true);

    InitCSAPoint(ui->comboBoxStartPoint);
    InitCSAPoint(ui->comboBoxEndPoint);

    const QListWidgetItem *item = ui->listWidgetCustomSA->item( row );
    SCASSERT(item != nullptr);
    const CustomSARecord record = qvariant_cast<CustomSARecord>(item->data(Qt::UserRole));

    {
        const int index = ui->comboBoxStartPoint->findData(record.startPoint);
        if (index != -1)
        {
            ui->comboBoxStartPoint->setCurrentIndex(index);
        }
    }

    {
        const int index = ui->comboBoxEndPoint->findData(record.endPoint);
        if (index != -1)
        {
            ui->comboBoxEndPoint->setCurrentIndex(index);
        }
    }

    ui->comboBoxStartPoint->blockSignals(false);
    ui->comboBoxEndPoint->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PathDialogClosed(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(not m_dialog.isNull());
        DialogPiecePath *dialogTool = qobject_cast<DialogPiecePath*>(m_dialog.data());
        SCASSERT(dialogTool != nullptr);
        const VPiecePath newPath = dialogTool->GetPiecePath();
        const VPiecePath oldPath = data->GetPiecePath(dialogTool->GetPieceId());

        SavePiecePathOptions *saveCommand = new SavePiecePathOptions(newPath, oldPath, qApp->getCurrentDocument(),
                                                                     const_cast<VContainer *>(data), toolId);
        qApp->getUndoStack()->push(saveCommand);
    }
    delete m_dialog;
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::CreatePiece() const
{
    VPiece piece;
    for (qint32 i = 0; i < ui->listWidgetMainPath->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetMainPath->item(i);
        piece.GetPath().Append(qvariant_cast<VPieceNode>(item->data(Qt::UserRole)));
    }

    piece.SetForbidFlipping(ui->checkBoxForbidFlipping->isChecked());
    piece.SetSeamAllowance(ui->checkBoxSeams->isChecked());
    piece.SetSAWidth(ui->doubleSpinBoxSeams->value());
    piece.SetMx(m_mx);
    piece.SetMy(m_my);

    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewMainPathItem(const VPieceNode &node)
{
    NewNodeItem(ui->listWidgetMainPath, node);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewCustomSA(const CustomSARecord &record)
{
    if (record.path > NULL_ID)
    {
        const QString name = GetCustomSARecordName(record);

        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(record));
        ui->listWidgetCustomSA->addItem(item);
        ui->listWidgetCustomSA->setCurrentRow(ui->listWidgetCustomSA->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetCustomSARecordName(const CustomSARecord &record) const
{
    QString name;

    if (record.path > NULL_ID)
    {
        const VPiecePath path = data->GetPiecePath(record.path);
        name = path.GetName();

        if (record.reverse)
        {
            name = QLatin1String("- ") + name;
        }
    }

    return name;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSeamAllowance::MainPathIsValid() const
{
    QString url = DialogWarningIcon();

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
        if (FirstPointEqualLast(ui->listWidgetMainPath))
        {
            url += tr("First point cannot be equal to the last point!");
            ui->helpLabel->setText(url);
            return false;
        }
        else if (DoublePoints(ui->listWidgetMainPath))
        {
            url += tr("You have double points!");
            ui->helpLabel->setText(url);
            return false;
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
void DialogSeamAllowance::InitNodesList()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const quint32 id = ui->comboBoxNodes->itemData(ui->comboBoxNodes->currentIndex()).toUInt();
#else
    const quint32 id = ui->comboBoxNodes->currentData().toUInt();
#endif

    ui->comboBoxNodes->blockSignals(true);
    ui->comboBoxNodes->clear();

    const VPiece piece = CreatePiece();

    for (int i = 0; i < piece.GetPath().CountNodes(); ++i)
    {
        const VPieceNode node = piece.GetPath().at(i);
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
QListWidgetItem *DialogSeamAllowance::GetItemById(quint32 id)
{
    for (qint32 i = 0; i < ui->listWidgetMainPath->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetMainPath->item(i);
        const VPieceNode node = qvariant_cast<VPieceNode>(item->data(Qt::UserRole));

        if (node.GetId() == id)
        {
            return item;
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetCurrentSABefore(qreal value)
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
void DialogSeamAllowance::SetCurrentSAAfter(qreal value)
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
void DialogSeamAllowance::InitCSAPoint(QComboBox *box)
{
    SCASSERT(box != nullptr);
    box->clear();
    box->addItem(tr("Empty"), NULL_ID);

    const VPiece piece = CreatePiece();

    for (int i = 0; i < piece.GetPath().CountNodes(); ++i)
    {
        const VPieceNode &node = piece.GetPath().at(i);
        if (node.GetTypeTool() == Tool::NodePoint)
        {
            const QString name = GetNodeName(node);
            box->addItem(name, node.GetId());
        }
    }
}
