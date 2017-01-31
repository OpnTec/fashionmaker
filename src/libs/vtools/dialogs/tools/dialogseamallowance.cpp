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
#include "../vpatterndb/calculator.h"
#include "visualization/path/vistoolpiece.h"
#include "dialogpiecepath.h"
#include "../../undocommands/savepiecepathoptions.h"
#include "../support/dialogeditwrongformula.h"

#include <QMenu>
#include <QTimer>
#include <QtNumeric>

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSeamAllowance),
      applyAllowed(false),// By default disabled
      m_bAddMode(true),
      m_mx(0),
      m_my(0),
      m_dialog(),
      m_qslMaterials(),
      m_qslPlacements(),
      m_conMCP(),
      m_oldData(),
      m_oldGeom(),
      m_oldGrainline(),
      m_iRotBaseHeight(0),
      m_iLenBaseHeight(0),
      m_formulaBaseWidth(0),
      m_formulaBaseWidthBefore(0),
      m_formulaBaseWidthAfter(0),
      m_timerWidth(nullptr),
      m_timerWidthBefore(nullptr),
      m_timerWidthAfter(nullptr),
      m_saWidth(0)
{
    ui->setupUi(this);

    InitOkCancelApply(ui);
    EnableApply(applyAllowed);

    InitMainPathTab();
    InitSeamAllowanceTab();
    InitInternalPathsTab();
    InitPatternPieceDataTab();
    InitGrainlineTab();

    flagName = true;//We have default name of piece.
    ChangeColor(ui->labelEditName, okColor);
    flagError = MainPathIsValid();
    CheckState();

    if (not applyAllowed)
    {
        vis = new VisToolPiece(data);
    }

    ui->tabWidget->setCurrentIndex(0);// Show always first tab active on start.
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::~DialogSeamAllowance()
{
    VContainer *locData = const_cast<VContainer *> (data);
    locData->RemoveVariable(currentSeamAllowance);

    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableApply(bool enable)
{
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(enable);
    applyAllowed = enable;
    ui->tabSeamAllowance->setEnabled(applyAllowed);
    ui->tabInternalPaths->setEnabled(applyAllowed);
    ui->tabPatternPieceData->setEnabled(applyAllowed);
    ui->tabGrainline->setEnabled(applyAllowed);
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
    for (int i = 0; i < piece.GetCustomSARecords().size(); ++i)
    {
        NewCustomSA(piece.GetCustomSARecords().at(i));
    }
    ui->listWidgetCustomSA->blockSignals(false);

    ui->listWidgetInternalPaths->clear();
    for (int i = 0; i < piece.GetInternalPaths().size(); ++i)
    {
        NewInternalPath(piece.GetInternalPaths().at(i));
    }

    ui->comboBoxStartPoint->blockSignals(true);
    ui->comboBoxStartPoint->clear();
    ui->comboBoxStartPoint->blockSignals(false);

    ui->comboBoxEndPoint->blockSignals(true);
    ui->comboBoxEndPoint->clear();
    ui->comboBoxEndPoint->blockSignals(false);

    CustomSAChanged(0);

    ui->checkBoxForbidFlipping->setChecked(piece.IsForbidFlipping());
    ui->checkBoxSeams->setChecked(piece.IsSeamAllowance());
    ui->lineEditName->setText(piece.GetName());

    ui->plainTextEditFormulaWidth->setPlainText(piece.GetFormulaSAWidth());
    m_saWidth = piece.GetSAWidth();

    m_mx = piece.GetMx();
    m_my = piece.GetMy();

    ui->lineEditLetter->setText(piece.GetPatternPieceData().GetLetter());
    ui->checkBoxDetail->setChecked(piece.GetPatternPieceData().IsVisible());
    ui->checkBoxPattern->setChecked(piece.GetPatternInfo().IsVisible());

    m_conMCP.clear();
    for (int i = 0; i < piece.GetPatternPieceData().GetMCPCount(); ++i)
    {
        m_conMCP << piece.GetPatternPieceData().GetMCP(i);
    }

    UpdateList();

    ui->checkBoxGrainline->setChecked(piece.GetGrainlineGeometry().IsVisible());
    ui->lineEditRotFormula->setPlainText(piece.GetGrainlineGeometry().GetRotation());
    ui->lineEditLenFormula->setPlainText(piece.GetGrainlineGeometry().GetLength());
    ui->comboBoxArrow->setCurrentIndex(int(piece.GetGrainlineGeometry().GetArrowType()));

    m_oldData = piece.GetPatternPieceData();
    m_oldGeom = piece.GetPatternInfo();
    m_oldGrainline = piece.GetGrainlineGeometry();

    ValidObjects(MainPathIsValid());
    EnableGrainlineRotation();

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
        if (id != GetLastId())
        {
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
        }
        else
        {
            if (ui->listWidgetMainPath->count() > 1)
            {
                delete GetItemById(id);
            }
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

                if (not qApp->getCurrentScene()->items().contains(visPath))
                {
                    visPath->VisualMode(NULL_ID);
                }
                else
                {
                    visPath->RefreshGeometry();
                }
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
    bOk->setEnabled(flagName && flagError && flagFormula);
    // In case dialog hasn't apply button
    if ( bApply != nullptr && applyAllowed)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormulaWidth->blockSignals(true);
    ui->plainTextEditFormulaWidthBefore->blockSignals(true);
    ui->plainTextEditFormulaWidthAfter->blockSignals(true);
    ui->lineEditRotFormula->blockSignals(true);
    ui->lineEditLenFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateList()
{
    ui->listWidgetMCP->clear();
    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        MaterialCutPlacement mcp = m_conMCP.at(i);
        QString qsText = tr("Cut %1 of %2%3").arg(mcp.m_iCutNumber);
        if (mcp.m_eMaterial < MaterialType::mtUserDefined)
        {
            qsText = qsText.arg(m_qslMaterials[int(mcp.m_eMaterial)]);
        }
        else
        {
            qsText = qsText.arg(mcp.m_qsMaterialUserDef);
        }
        if (mcp.m_ePlacement == PlacementType::ptCutOnFold)
        {
            qsText = qsText.arg(QLatin1String(" ") + tr("on Fold"));
        }
        else
        {
            qsText = qsText.arg("");
        }

        ui->listWidgetMCP->addItem(qsText);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::AddUpdate()
{
    MaterialCutPlacement mcp;
    QStringList qslUserMaterials = qApp->Settings()->GetUserDefinedMaterials();

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    int i = ui->comboBoxMaterial->itemData(ui->comboBoxMaterial->currentIndex()).toInt();
#else
    int i = ui->comboBoxMaterial->currentData().toInt();
#endif
    QString qsMat = ui->comboBoxMaterial->currentText();
    if (i < m_qslMaterials.count() && qsMat == m_qslMaterials[i])
    {
        mcp.m_eMaterial = MaterialType(i);
        mcp.m_qsMaterialUserDef.clear();
    }
    else
    {
        mcp.m_eMaterial = MaterialType::mtUserDefined;
        mcp.m_qsMaterialUserDef = qsMat;
        // check if we have new user defined material
        bool bFound = false;
        for (int i = 0; i < qslUserMaterials.count() && bFound == false; ++i)
        {
            if (mcp.m_qsMaterialUserDef == qslUserMaterials[i])
            {
                bFound = true;
            }
        }
        if (bFound == false)
        {
            qApp->Settings()->AddUserDefinedMaterial(mcp.m_qsMaterialUserDef);
            qApp->Settings()->sync();
            ui->comboBoxMaterial->addItem(mcp.m_qsMaterialUserDef, int(MaterialType::mtUserDefined));
        }
    }

    mcp.m_iCutNumber = ui->spinBoxCutNumber->value();
    mcp.m_ePlacement = PlacementType(ui->comboBoxPlacement->currentIndex());

    if (m_bAddMode == true)
    {
        m_conMCP << mcp;
    }
    else
    {
        int iR = ui->listWidgetMCP->currentRow();
        SCASSERT(iR >= 0)
        m_conMCP[iR] = mcp;
        SetAddMode();
    }
    UpdateList();
    ClearFields();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::Cancel()
{
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::Remove()
{
    int iR = ui->listWidgetMCP->currentRow();
    SCASSERT(iR >= 0)
    m_conMCP.removeAt(iR);
    UpdateList();
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NameDetailChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        if (edit->text().isEmpty())
        {
            flagName = false;
            ChangeColor(ui->labelEditName, Qt::red);
            QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
            ui->tabWidget->setTabIcon(1, icon);
        }
        else
        {
            flagName = true;
            ChangeColor(ui->labelEditName, okColor);
            QIcon icon;
            ui->tabWidget->setTabIcon(1, icon);
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::MaterialChanged()
{
    ui->pushButtonAdd->setEnabled(ui->comboBoxMaterial->currentText().isEmpty() == false);
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

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidgetMainPath->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetMainPath->item(row);
        ValidObjects(MainPathIsValid());
    }
    else if (rowNode.GetTypeTool() != Tool::NodePoint && selectedAction == actionReverse)
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
    QAction *actionOption = menu->addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = menu->addAction(tr("Reverse"));
    actionReverse->setCheckable(true);
    actionReverse->setChecked(record.reverse);

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidgetCustomSA->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetCustomSA->item(row);
    }
    else if (selectedAction == actionReverse)
    {
        record.reverse = not record.reverse;
        rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
        rowItem->setText(GetPathName(record.path, record.reverse));
    }
    else if (selectedAction == actionOption)
    {
        auto *dialog = new DialogPiecePath(data, record.path, this);
        dialog->SetPiecePath(data->GetPiecePath(record.path));
        dialog->SetPieceId(toolId);
        if (record.includeType == PiecePathIncludeType::AsMainPath)
        {
            dialog->SetFormulaSAWidth(GetFormulaSAWidth());
        }
        dialog->EnbleShowMode(true);
        m_dialog = dialog;
        m_dialog->setModal(true);
        connect(m_dialog.data(), &DialogTool::DialogClosed, this, &DialogSeamAllowance::PathDialogClosed);
        m_dialog->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowInternalPathsContextMenu(const QPoint &pos)
{
    const int row = ui->listWidgetInternalPaths->currentRow();
    if (ui->listWidgetInternalPaths->count() == 0 || row == -1 || row >= ui->listWidgetInternalPaths->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *actionOption = menu->addAction(QIcon::fromTheme("preferences-other"), tr("Options"));
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidgetInternalPaths->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetInternalPaths->item(row);
    }
    else if (selectedAction == actionOption)
    {
        QListWidgetItem *rowItem = ui->listWidgetInternalPaths->item(row);
        SCASSERT(rowItem != nullptr);
        const quint32 pathId = qvariant_cast<quint32>(rowItem->data(Qt::UserRole));

        auto *dialog = new DialogPiecePath(data, pathId, this);
        dialog->SetPiecePath(data->GetPiecePath(pathId));
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
    CustomSAChanged(ui->listWidgetCustomSA->currentRow());
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
    ui->plainTextEditFormulaWidthBefore->setDisabled(true);
    ui->toolButtonExprBefore->setDisabled(true);
    ui->pushButtonDefBefore->setDisabled(true);

    ui->plainTextEditFormulaWidthAfter->setDisabled(true);
    ui->toolButtonExprAfter->setDisabled(true);
    ui->pushButtonDefAfter->setDisabled(true);

    ui->comboBoxAngle->setDisabled(true);

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

            // Seam alowance before
            ui->plainTextEditFormulaWidthBefore->setEnabled(true);
            ui->toolButtonExprBefore->setEnabled(true);

            QString w1Formula = node.GetFormulaSABefore();
            if (w1Formula != currentSeamAllowance)
            {
                ui->pushButtonDefBefore->setEnabled(true);
            }
            if (w1Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthBeforeFormulaTextEdit();
            }
            w1Formula = qApp->TrVars()->FormulaToUser(w1Formula, qApp->Settings()->GetOsSeparator());
            ui->plainTextEditFormulaWidthBefore->setPlainText(w1Formula);
            MoveCursorToEnd(ui->plainTextEditFormulaWidthBefore);

            // Seam alowance after
            ui->plainTextEditFormulaWidthAfter->setEnabled(true);
            ui->toolButtonExprAfter->setEnabled(true);

            QString w2Formula = node.GetFormulaSAAfter();
            if (w2Formula != currentSeamAllowance)
            {
                ui->pushButtonDefAfter->setEnabled(true);
            }
            if (w2Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthAfterFormulaTextEdit();
            }
            w2Formula = qApp->TrVars()->FormulaToUser(w2Formula, qApp->Settings()->GetOsSeparator());
            ui->plainTextEditFormulaWidthAfter->setPlainText(w2Formula);
            MoveCursorToEnd(ui->plainTextEditFormulaWidthAfter);

            // Angle type
            ui->comboBoxAngle->setEnabled(true);
            const int index = ui->comboBoxAngle->findData(static_cast<unsigned char>(node.GetAngleType()));
            if (index != -1)
            {
                ui->comboBoxAngle->setCurrentIndex(index);
            }
        }
    }
    else
    {
        ui->plainTextEditFormulaWidthBefore->setPlainText("");
        ui->plainTextEditFormulaWidthAfter->setPlainText("");
        ui->comboBoxAngle->setCurrentIndex(-1);
    }
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
void DialogSeamAllowance::CSAIncludeTypeChanged(int index)
{
    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    const PiecePathIncludeType type =
            static_cast<PiecePathIncludeType>(ui->comboBoxIncludeType->itemData(index).toUInt());
#else
    Q_UNUSED(index);
    const PiecePathIncludeType type =
            static_cast<PiecePathIncludeType>(ui->comboBoxIncludeType->currentData().toUInt());
#endif

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.includeType = type;
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
    ui->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefAfter()
{
    ui->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CustomSAChanged(int row)
{
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        ui->comboBoxStartPoint->blockSignals(true);
        ui->comboBoxStartPoint->clear();
        ui->comboBoxStartPoint->blockSignals(false);

        ui->comboBoxEndPoint->blockSignals(true);
        ui->comboBoxEndPoint->clear();
        ui->comboBoxEndPoint->blockSignals(false);

        ui->comboBoxIncludeType->blockSignals(true);
        ui->comboBoxIncludeType->clear();
        ui->comboBoxIncludeType->blockSignals(false);
        return;
    }

    const QListWidgetItem *item = ui->listWidgetCustomSA->item( row );
    SCASSERT(item != nullptr);
    const CustomSARecord record = qvariant_cast<CustomSARecord>(item->data(Qt::UserRole));

    ui->comboBoxStartPoint->blockSignals(true);
    InitCSAPoint(ui->comboBoxStartPoint);
    {
        const int index = ui->comboBoxStartPoint->findData(record.startPoint);
        if (index != -1)
        {
            ui->comboBoxStartPoint->setCurrentIndex(index);
        }
    }
    ui->comboBoxStartPoint->blockSignals(false);

    ui->comboBoxEndPoint->blockSignals(true);
    InitCSAPoint(ui->comboBoxEndPoint);
    {
        const int index = ui->comboBoxEndPoint->findData(record.endPoint);
        if (index != -1)
        {
            ui->comboBoxEndPoint->setCurrentIndex(index);
        }
    }
    ui->comboBoxEndPoint->blockSignals(false);

    ui->comboBoxIncludeType->blockSignals(true);
    InitSAIncludeType();
    {
        const int index = ui->comboBoxIncludeType->findData(static_cast<unsigned char>(record.includeType));
        if (index != -1)
        {
            ui->comboBoxIncludeType->setCurrentIndex(index);
        }
    }
    ui->comboBoxIncludeType->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PathDialogClosed(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(not m_dialog.isNull());
        DialogPiecePath *dialogTool = qobject_cast<DialogPiecePath*>(m_dialog.data());
        SCASSERT(dialogTool != nullptr);
        try
        {
            const VPiecePath newPath = dialogTool->GetPiecePath();
            const VPiecePath oldPath = data->GetPiecePath(dialogTool->GetToolId());

            SavePiecePathOptions *saveCommand = new SavePiecePathOptions(oldPath, newPath, qApp->getCurrentDocument(),
                                                                         const_cast<VContainer *>(data),
                                                                         dialogTool->GetToolId());
            qApp->getUndoStack()->push(saveCommand);
            UpdateCurrentCustomSARecord();
            UpdateCurrentInternalPathRecord();
        }
        catch (const VExceptionBadId &e)
        {
            qCritical("%s\n\n%s\n\n%s", qUtf8Printable(tr("Error. Can't save piece path.")),
                       qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        }
    }
    delete m_dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateValues()
{
    QPlainTextEdit* apleSender[2];
    apleSender[0] = ui->lineEditRotFormula;
    apleSender[1] = ui->lineEditLenFormula;
    bool bFormulasOK = true;

    for (int i = 0; i < 2; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = ui->labelRot;
            plbText = ui->labelEditRot;
            qsUnit = degreeSymbol;
        }
        else
        {
            plbVal = ui->labelLen;
            plbText = ui->labelEditLen;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }

        plbVal->setToolTip(tr("Value"));

        QString qsFormula = apleSender[i]->toPlainText().simplified();
        Calculator cal;
        QString qsVal;
        try
        {
            qsFormula.replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            qreal dVal;
            dVal = cal.EvalFormula(data->PlainVariables(), qsFormula);
            if (qIsInf(dVal) == true || qIsNaN(dVal) == true)
            {
                throw qmu::QmuParserError(tr("Infinite/undefined result"));
            }
            else if (i == 1 && dVal <= 0.0)
            {
                throw qmu::QmuParserError(tr("Length should be positive"));
            }
            else
            {
                qsVal.setNum(dVal, 'f', 2);
                ChangeColor(plbText, okColor);
            }
        }
        catch (qmu::QmuParserError &e)
        {
            qsVal.clear();
            ChangeColor(plbText, Qt::red);
            bFormulasOK = false;
            plbVal->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
        }

        if (qsVal.isEmpty() == false)
        {
            qsVal += qsUnit;
        }
        plbVal->setText(qsVal);
    }

    bOk->setEnabled(bFormulasOK);
    if (bFormulasOK == false)
    {
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabGrainline), icon);
    }
    else
    {
        ResetWarning();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetAddMode()
{
    ui->pushButtonAdd->setText(tr("Add"));
    ui->pushButtonCancel->hide();
    ui->pushButtonRemove->hide();
    ui->listWidgetMCP->setCurrentRow(-1);
    m_bAddMode = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetEditMode()
{
    int iR = ui->listWidgetMCP->currentRow();
    // this method can be called by clicking on item or by update. In the latter case there is nothing else to do!
    if (iR < 0 || iR >= m_conMCP.count())
    {
        return;
    }

    ui->pushButtonAdd->setText(tr("Update"));
    ui->pushButtonCancel->show();
    ui->pushButtonRemove->show();

    MaterialCutPlacement mcp = m_conMCP.at(iR);
    if (mcp.m_eMaterial == MaterialType::mtUserDefined)
    {
        int iRow = qApp->Settings()->GetUserDefinedMaterials().indexOf(mcp.m_qsMaterialUserDef);
        if (iRow >= 0)
        {
            ui->comboBoxMaterial->setCurrentIndex(iRow + m_qslMaterials.count());
        }
        else
        {
            ui->comboBoxMaterial->setCurrentText(mcp.m_qsMaterialUserDef);
        }
    }
    else
    {
        ui->comboBoxMaterial->setCurrentIndex(int(mcp.m_eMaterial));
    }
    ui->spinBoxCutNumber->setValue(mcp.m_iCutNumber);
    ui->comboBoxPlacement->setCurrentIndex(int(mcp.m_ePlacement));

    m_bAddMode = false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableGrainlineRotation()
{
    ui->lineEditRotFormula->setEnabled(ui->checkBoxGrainline->isChecked());
    ui->lineEditLenFormula->setEnabled(ui->checkBoxGrainline->isChecked());
    ui->pushButtonRot->setEnabled(ui->checkBoxGrainline->isChecked());
    ui->pushButtonLen->setEnabled(ui->checkBoxGrainline->isChecked());
    ui->pushButtonShowLen->setEnabled(ui->checkBoxGrainline->isChecked());
    ui->pushButtonShowRot->setEnabled(ui->checkBoxGrainline->isChecked());

    if (ui->checkBoxGrainline->isChecked() == true)
    {
        UpdateValues();
    }
    else
    {
        ChangeColor(ui->labelEditLen, okColor);
        ChangeColor(ui->labelEditRot, okColor);
        bOk->setEnabled(true);
        ResetWarning();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EditFormula()
{
    QPlainTextEdit* pleFormula;
    bool bCheckZero;

    if (sender() == ui->pushButtonLen)
    {
        pleFormula = ui->lineEditLenFormula;
        bCheckZero = true;
    }
    else if (sender() == ui->pushButtonRot)
    {
        pleFormula = ui->lineEditRotFormula;
        bCheckZero = false;
    }
    else
    {
        // should not get here!
        return;
    }

    DialogEditWrongFormula dlg(data, NULL_ID, this);
    dlg.SetFormula(pleFormula->toPlainText());
    dlg.setCheckZero(bCheckZero);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString qsFormula = dlg.GetFormula();
        qsFormula.replace("\n", " ");
        pleFormula->setPlainText(qsFormula);
        UpdateValues();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployRotation()
{
    DeployFormula(ui->lineEditRotFormula, ui->pushButtonShowRot, m_iRotBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployLength()
{
    DeployFormula(ui->lineEditLenFormula, ui->pushButtonShowLen, m_iLenBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ResetWarning()
{
    QIcon icon;
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabGrainline), icon);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidth()
{
    labelEditFormula = ui->labelEditWidth;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidth->toPlainText();
    m_saWidth = Eval(formula, flagFormula, ui->labelResultWidth, postfix, false, true);

    if (m_saWidth >= 0)
    {
        VContainer *locData = const_cast<VContainer *> (data);
        locData->AddVariable(currentSeamAllowance, new VIncrement(locData, currentSeamAllowance, 0, m_saWidth,
                                                                  QString().setNum(m_saWidth), true,
                                                                  tr("Current seam allowance")));

        EvalWidthBefore();
        EvalWidthAfter();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthBefore()
{
    labelEditFormula = ui->labelEditBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidthBefore->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, ui->labelResultBefore, postfix, false, true);

    UpdateNodeSABefore(GetFormulaSAWidthBefore());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthAfter()
{
    labelEditFormula = ui->labelEditAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidthAfter->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, ui->labelResultAfter, postfix, false, true);

    UpdateNodeSAAfter(GetFormulaSAWidthAfter());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXWidth()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit seam allowance width"));
    dialog->SetFormula(GetFormulaSAWidth());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormulaSAWidth(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXWidthBefore()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit seam allowance width before"));
    dialog->SetFormula(GetFormulaSAWidthBefore());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSABefore(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXWidthAfter()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit seam allowance width after"));
    dialog->SetFormula(GetFormulaSAWidthAfter());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSAAfter(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::WidthChanged()
{
    labelEditFormula = ui->labelEditWidth;
    labelResultCalculation = ui->labelResultWidth;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidth, m_timerWidth, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::WidthBeforeChanged()
{
    labelEditFormula = ui->labelEditBefore;
    labelResultCalculation = ui->labelResultBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidthBefore, m_timerWidthBefore, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::WidthAfterChanged()
{
    labelEditFormula = ui->labelEditAfter;
    labelResultCalculation = ui->labelResultAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidthAfter, m_timerWidthAfter, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidth, ui->pushButtonGrowWidth, m_formulaBaseWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthBeforeFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidthBefore, ui->pushButtonGrowWidthBefore, m_formulaBaseWidthBefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthAfterFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidthAfter, ui->pushButtonGrowWidthAfter, m_formulaBaseWidthAfter);
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

    QVector<CustomSARecord> records;
    for (qint32 i = 0; i < ui->listWidgetCustomSA->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetCustomSA->item(i);
        records.append(qvariant_cast<CustomSARecord>(item->data(Qt::UserRole)));
    }
    piece.SetCustomSARecords(records);

    QVector<quint32> iPaths;
    for (qint32 i = 0; i < ui->listWidgetInternalPaths->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetInternalPaths->item(i);
        iPaths.append(qvariant_cast<quint32>(item->data(Qt::UserRole)));
    }
    piece.SetInternalPaths(iPaths);

    piece.SetForbidFlipping(ui->checkBoxForbidFlipping->isChecked());
    piece.SetSeamAllowance(ui->checkBoxSeams->isChecked());
    piece.SetName(ui->lineEditName->text());
    piece.SetMx(m_mx);
    piece.SetMy(m_my);

    QString width = ui->plainTextEditFormulaWidth->toPlainText();
    width.replace("\n", " ");
    piece.SetFormulaSAWidth(width, m_saWidth);

    piece.GetPatternPieceData().SetLetter(ui->lineEditLetter->text());

    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        piece.GetPatternPieceData().Append(m_conMCP[i]);
    }

    piece.GetPatternPieceData().SetPos(m_oldData.GetPos());
    piece.GetPatternPieceData().SetLabelWidth(m_oldData.GetLabelWidth());
    piece.GetPatternPieceData().SetLabelHeight(m_oldData.GetLabelHeight());
    piece.GetPatternPieceData().SetFontSize(m_oldData.GetFontSize());
    piece.GetPatternPieceData().SetRotation(m_oldData.GetRotation());
    piece.GetPatternPieceData().SetVisible(ui->checkBoxDetail->isChecked());

    piece.GetPatternInfo() = m_oldGeom;
    piece.GetPatternInfo().SetVisible(ui->checkBoxPattern->isChecked());

    piece.GetGrainlineGeometry() = m_oldGrainline;
    piece.GetGrainlineGeometry().SetVisible(ui->checkBoxGrainline->isChecked());
    piece.GetGrainlineGeometry().SetRotation(ui->lineEditRotFormula->toPlainText());
    piece.GetGrainlineGeometry().SetLength(ui->lineEditLenFormula->toPlainText());
    piece.GetGrainlineGeometry().SetArrowType(VGrainlineGeometry::ArrowType(ui->comboBoxArrow->currentIndex()));

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
        const QString name = GetPathName(record.path, record.reverse);

        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(record));
        ui->listWidgetCustomSA->addItem(item);
        ui->listWidgetCustomSA->setCurrentRow(ui->listWidgetCustomSA->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewInternalPath(quint32 path)
{
    if (path > NULL_ID)
    {
        const QString name = GetPathName(path);

        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(path));
        ui->listWidgetInternalPaths->addItem(item);
        ui->listWidgetInternalPaths->setCurrentRow(ui->listWidgetInternalPaths->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetPathName(quint32 path, bool reverse) const
{
    QString name;

    if (path > NULL_ID)
    {
        name = data->GetPiecePath(path).GetName();

        if (reverse)
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
quint32 DialogSeamAllowance::GetLastId() const
{
    const int count = ui->listWidgetMainPath->count();
    if (count > 0)
    {
        QListWidgetItem *item = ui->listWidgetMainPath->item(count-1);
        const VPieceNode node = qvariant_cast<VPieceNode>(item->data(Qt::UserRole));
        return node.GetId();
    }
    else
    {
        return NULL_ID;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetCurrentSABefore(const QString &formula)
{
    UpdateNodeSABefore(formula);
    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetCurrentSAAfter(const QString &formula)
{
    UpdateNodeSAAfter(formula);
    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateNodeSABefore(const QString &formula)
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
            rowNode.SetFormulaSABefore(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateNodeSAAfter(const QString &formula)
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
            rowNode.SetFormulaSAAfter(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitMainPathTab()
{
    ui->checkBoxForbidFlipping->setChecked(qApp->Settings()->GetForbidWorkpieceFlipping());

    ui->listWidgetMainPath->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetMainPath, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowMainPathContextMenu);
    connect(ui->listWidgetMainPath->model(), &QAbstractItemModel::rowsMoved, this, &DialogSeamAllowance::ListChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitSeamAllowanceTab()
{
    plainTextEditFormula = ui->plainTextEditFormulaWidth;
    this->m_formulaBaseWidth = ui->plainTextEditFormulaWidth->height();
    this->m_formulaBaseWidthBefore = ui->plainTextEditFormulaWidthBefore->height();
    this->m_formulaBaseWidthAfter = ui->plainTextEditFormulaWidthAfter->height();

    ui->plainTextEditFormulaWidth->installEventFilter(this);
    ui->plainTextEditFormulaWidthBefore->installEventFilter(this);
    ui->plainTextEditFormulaWidthAfter->installEventFilter(this);

    m_timerWidth = new QTimer(this);
    connect(m_timerWidth, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidth);

    m_timerWidthBefore = new QTimer(this);
    connect(m_timerWidthBefore, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthBefore);

    m_timerWidthAfter = new QTimer(this);
    connect(m_timerWidthAfter, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthAfter);

    connect(ui->checkBoxSeams, &QCheckBox::toggled, this, &DialogSeamAllowance::EnableSeamAllowance);

    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    m_saWidth = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
    ui->plainTextEditFormulaWidth->setPlainText(qApp->LocaleToString(m_saWidth));

    InitNodesList();
    connect(ui->comboBoxNodes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeChanged);

    connect(ui->pushButtonDefBefore, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefBefore);
    connect(ui->pushButtonDefAfter, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefAfter);

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
    connect(ui->comboBoxIncludeType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::CSAIncludeTypeChanged);

    connect(ui->toolButtonExprWidth, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidth);
    connect(ui->toolButtonExprBefore, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthBefore);
    connect(ui->toolButtonExprAfter, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthAfter);

    connect(ui->plainTextEditFormulaWidth, &QPlainTextEdit::textChanged, this, &DialogSeamAllowance::WidthChanged);
    connect(ui->plainTextEditFormulaWidthBefore, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::WidthBeforeChanged);
    connect(ui->plainTextEditFormulaWidthAfter, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::WidthAfterChanged);

    connect(ui->pushButtonGrowWidth, &QPushButton::clicked, this, &DialogSeamAllowance::DeployWidthFormulaTextEdit);
    connect(ui->pushButtonGrowWidthBefore, &QPushButton::clicked,
            this, &DialogSeamAllowance::DeployWidthBeforeFormulaTextEdit);
    connect(ui->pushButtonGrowWidthAfter, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployWidthAfterFormulaTextEdit);
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

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitSAIncludeType()
{
    ui->comboBoxIncludeType->clear();

    ui->comboBoxIncludeType->addItem(tr("main path"), static_cast<unsigned char>(PiecePathIncludeType::AsMainPath));
    ui->comboBoxIncludeType->addItem(tr("custom seam allowance"),
                                     static_cast<unsigned char>(PiecePathIncludeType::AsCustomSA));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitInternalPathsTab()
{
    ui->listWidgetInternalPaths->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetInternalPaths, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowInternalPathsContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPatternPieceDataTab()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditName->setClearButtonEnabled(true);
    ui->lineEditLetter->setClearButtonEnabled(true);
#endif

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogSeamAllowance::NameDetailChanged);

    m_qslMaterials << QApplication::translate("Detail", "Fabric", 0)
                   << QApplication::translate("Detail", "Lining", 0)
                   << QApplication::translate("Detail", "Interfacing", 0)
                   << QApplication::translate("Detail", "Interlining", 0);

    for (int i = 0; i < m_qslMaterials.count(); ++i)
    {
        ui->comboBoxMaterial->addItem(m_qslMaterials[i], i);
    }

    const QStringList qsl = qApp->Settings()->GetUserDefinedMaterials();
    for (int i = 0; i < qsl.count(); ++i)
    {
        ui->comboBoxMaterial->addItem(qsl.at(i), int(MaterialType::mtUserDefined));
    }

    m_qslPlacements << tr("None") << tr("Cut on fold");
    ui->comboBoxPlacement->addItems(m_qslPlacements);
    ui->pushButtonRot->setIcon(QIcon("://icon/16x16/fx.png"));
    ui->pushButtonLen->setIcon(QIcon("://icon/16x16/fx.png"));

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &DialogSeamAllowance::AddUpdate);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &DialogSeamAllowance::Cancel);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &DialogSeamAllowance::Remove);
    connect(ui->listWidgetMCP, &QListWidget::itemClicked, this, &DialogSeamAllowance::SetEditMode);
    connect(ui->comboBoxMaterial, &QComboBox::currentTextChanged, this, &DialogSeamAllowance::MaterialChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitGrainlineTab()
{
    connect(ui->checkBoxGrainline, &QCheckBox::toggled, this, &DialogSeamAllowance::EnableGrainlineRotation);
    connect(ui->pushButtonRot, &QPushButton::clicked, this, &DialogSeamAllowance::EditFormula);
    connect(ui->pushButtonLen, &QPushButton::clicked, this, &DialogSeamAllowance::EditFormula);
    connect(ui->lineEditLenFormula, &QPlainTextEdit::textChanged, this, &DialogSeamAllowance::UpdateValues);
    connect(ui->lineEditRotFormula, &QPlainTextEdit::textChanged, this, &DialogSeamAllowance::UpdateValues);

    connect(ui->pushButtonShowRot, &QPushButton::clicked, this, &DialogSeamAllowance::DeployRotation);
    connect(ui->pushButtonShowLen, &QPushButton::clicked, this, &DialogSeamAllowance::DeployLength);

    SetAddMode();
    EnableGrainlineRotation();

    ui->comboBoxArrow->addItem(tr("Both"));
    ui->comboBoxArrow->addItem(tr("Just front"));
    ui->comboBoxArrow->addItem(tr("Just rear"));

    m_iRotBaseHeight = ui->lineEditRotFormula->height();
    m_iLenBaseHeight = ui->lineEditLenFormula->height();
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetFormulaSAWidth() const
{
    QString width = ui->plainTextEditFormulaWidth->toPlainText();
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetFormulaSAWidth(const QString &formula)
{
    const QString width = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (width.length() > 80)
    {
        this->DeployWidthFormulaTextEdit();
    }
    ui->plainTextEditFormulaWidth->setPlainText(width);

    VisToolPiece *path = qobject_cast<VisToolPiece *>(vis);
    SCASSERT(path != nullptr)
    const VPiece p = CreatePiece();
    path->SetPiece(p);

    MoveCursorToEnd(ui->plainTextEditFormulaWidth);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetFormulaSAWidthBefore() const
{
    QString width = ui->plainTextEditFormulaWidthBefore->toPlainText();
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetFormulaSAWidthAfter() const
{
    QString width = ui->plainTextEditFormulaWidthAfter->toPlainText();
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateCurrentCustomSARecord()
{
    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1)
    {
        return;
    }

    QListWidgetItem *item = ui->listWidgetCustomSA->item(row);
    SCASSERT(item != nullptr);
    const CustomSARecord record = qvariant_cast<CustomSARecord>(item->data(Qt::UserRole));
    item->setText(GetPathName(record.path, record.reverse));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateCurrentInternalPathRecord()
{
    const int row = ui->listWidgetInternalPaths->currentRow();
    if (ui->listWidgetInternalPaths->count() == 0 || row == -1)
    {
        return;
    }

    QListWidgetItem *item = ui->listWidgetInternalPaths->item(row);
    SCASSERT(item != nullptr);
    const quint32 path = qvariant_cast<quint32>(item->data(Qt::UserRole));
    item->setText(GetPathName(path));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ClearFields()
{
    ui->comboBoxMaterial->setCurrentIndex(0);
    ui->spinBoxCutNumber->setValue(0);
    ui->comboBoxPlacement->setCurrentIndex(0);
}
