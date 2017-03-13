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
#include "visualization/path/vispiecepins.h"
#include "dialogpiecepath.h"
#include "../../undocommands/savepiecepathoptions.h"
#include "../support/dialogeditwrongformula.h"

#include <QMenu>
#include <QTimer>
#include <QtNumeric>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
void EnableDefButton(QPushButton *defButton, const QString &formula)
{
    SCASSERT(defButton != nullptr)

    if (formula != currentSeamAllowance)
    {
        defButton->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString GetFormulaFromUser(QPlainTextEdit *textEdit)
{
    SCASSERT(textEdit != nullptr)

    QString formula = textEdit->toPlainText();
    formula.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSeamAllowance),
      applyAllowed(false),// By default disabled
      flagGPin(true),
      flagDPin(true),
      flagPPin(true),
      flagGFormulas(true),
      flagDLAngle(true),
      flagDLFormulas(true),
      flagPLAngle(true),
      flagPLFormulas(true),
      m_bAddMode(true),
      m_mx(0),
      m_my(0),
      m_dialog(),
      m_visPins(),
      m_qslMaterials(),
      m_qslPlacements(),
      m_conMCP(),
      m_oldData(),
      m_oldGeom(),
      m_oldGrainline(),
      m_iRotBaseHeight(0),
      m_iLenBaseHeight(0),
      m_DLWidthBaseHeight(0),
      m_DLHeightBaseHeight(0),
      m_DLAngleBaseHeight(0),
      m_PLWidthBaseHeight(0),
      m_PLHeightBaseHeight(0),
      m_PLAngleBaseHeight(0),
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
    InitLabelsTab();
    InitGrainlineTab();
    InitPinsTab();

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &DialogSeamAllowance::TabChanged);

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
    delete m_visPins;
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
    ui->tabPieceLabelData->setEnabled(applyAllowed);
    ui->tabLabels->setEnabled(applyAllowed);
    ui->tabGrainline->setEnabled(applyAllowed);
    ui->tabPins->setEnabled(applyAllowed);
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

    ui->listWidgetPins->clear();
    for (int i = 0; i < piece.GetPins().size(); ++i)
    {
        NewPin(piece.GetPins().at(i));
    }

    InitAllPinComboboxes();

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

    const QString width = qApp->TrVars()->FormulaToUser(piece.GetFormulaSAWidth(), qApp->Settings()->GetOsSeparator());
    ui->plainTextEditFormulaWidth->setPlainText(width);
    m_saWidth = piece.GetSAWidth();

    m_mx = piece.GetMx();
    m_my = piece.GetMy();

    ui->lineEditLetter->setText(piece.GetPatternPieceData().GetLetter());

    m_conMCP.clear();
    for (int i = 0; i < piece.GetPatternPieceData().GetMCPCount(); ++i)
    {
        m_conMCP << piece.GetPatternPieceData().GetMCP(i);
    }

    UpdateList();

    ui->comboBoxArrow->setCurrentIndex(int(piece.GetGrainlineGeometry().GetArrowType()));

    m_oldData = piece.GetPatternPieceData();
    ui->groupBoxDetailLabel->setChecked(m_oldData.IsVisible());
    ChangeCurrentData(ui->comboBoxDLCenterPin, m_oldData.CenterPin());
    ChangeCurrentData(ui->comboBoxDLTopLeftPin, m_oldData.TopLeftPin());
    ChangeCurrentData(ui->comboBoxDLBottomRightPin, m_oldData.BottomRightPin());
    SetDLWidth(m_oldData.GetLabelWidth());
    SetDLHeight(m_oldData.GetLabelHeight());
    SetDLHeight(m_oldData.GetRotation());

    m_oldGeom = piece.GetPatternInfo();
    ui->groupBoxPatternLabel->setChecked(m_oldGeom.IsVisible());
    ChangeCurrentData(ui->comboBoxPLCenterPin, m_oldGeom.CenterPin());
    ChangeCurrentData(ui->comboBoxPLTopLeftPin, m_oldGeom.TopLeftPin());
    ChangeCurrentData(ui->comboBoxPLBottomRightPin, m_oldGeom.BottomRightPin());
    SetPLWidth(m_oldGeom.GetLabelWidth());
    SetPLHeight(m_oldGeom.GetLabelHeight());
    SetPLHeight(m_oldGeom.GetRotation());

    m_oldGrainline = piece.GetGrainlineGeometry();
    ui->groupBoxGrainline->setChecked(m_oldGrainline.IsVisible());
    ChangeCurrentData(ui->comboBoxGrainlineCenterPin, m_oldGrainline.CenterPin());
    ChangeCurrentData(ui->comboBoxGrainlineTopPin, m_oldGrainline.TopPin());
    ChangeCurrentData(ui->comboBoxGrainlineBottomPin, m_oldGrainline.BottomPin());
    SetGrainlineAngle(m_oldGrainline.GetRotation());
    SetGrainlineLength(m_oldGrainline.GetLength());

    ValidObjects(MainPathIsValid());
    EnabledGrainline();
    EnabledDetailLabel();
    EnabledPatternLabel();

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
                case SceneObject::ElArc:
                    NewMainPathItem(VPieceNode(id, Tool::NodeElArc, reverse));
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
    bOk->setEnabled(flagName && flagError && flagFormula && (flagGFormulas || flagGPin)
                    && flagDLAngle && (flagDLFormulas || flagDPin) && flagPLAngle && (flagPLFormulas || flagPPin));
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

    const int i = CURRENT_DATA(ui->comboBoxMaterial).toInt();
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
            ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabSeamAllowance), icon);
        }
        else
        {
            flagName = true;
            ChangeColor(ui->labelEditName, okColor);
            ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabSeamAllowance), QIcon());
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

    QScopedPointer<QMenu> menu(new QMenu());

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

    QScopedPointer<QMenu> menu(new QMenu());
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

    QScopedPointer<QMenu> menu(new QMenu());
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
void DialogSeamAllowance::ShowPinsContextMenu(const QPoint &pos)
{
    const int row = ui->listWidgetPins->currentRow();
    if (ui->listWidgetPins->count() == 0 || row == -1 || row >= ui->listWidgetPins->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidgetPins->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidgetPins->item(row);
        TabChanged(ui->tabWidget->currentIndex());
        InitAllPinComboboxes();
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
        const VPiece piece = CreatePiece();
        const int nodeIndex = piece.GetPath().indexOfNode(CURRENT_DATA(ui->comboBoxNodes).toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = piece.GetPath().at(nodeIndex);

            // Seam alowance before
            ui->plainTextEditFormulaWidthBefore->setEnabled(true);
            ui->toolButtonExprBefore->setEnabled(true);

            QString w1Formula = node.GetFormulaSABefore();
            EnableDefButton(ui->pushButtonDefBefore, w1Formula);
            w1Formula = qApp->TrVars()->FormulaToUser(w1Formula, qApp->Settings()->GetOsSeparator());
            if (w1Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthBeforeFormulaTextEdit();
            }
            ui->plainTextEditFormulaWidthBefore->setPlainText(w1Formula);
            MoveCursorToEnd(ui->plainTextEditFormulaWidthBefore);

            // Seam alowance after
            ui->plainTextEditFormulaWidthAfter->setEnabled(true);
            ui->toolButtonExprAfter->setEnabled(true);

            QString w2Formula = node.GetFormulaSAAfter();
            EnableDefButton(ui->pushButtonDefAfter, w2Formula);
            w2Formula = qApp->TrVars()->FormulaToUser(w2Formula, qApp->Settings()->GetOsSeparator());
            if (w2Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthAfterFormulaTextEdit();
            }
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
    Q_UNUSED(index);

    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.startPoint = CURRENT_DATA(ui->comboBoxStartPoint).toUInt();
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAEndPointChanged(int index)
{
    Q_UNUSED(index);

    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.endPoint = CURRENT_DATA(ui->comboBoxEndPoint).toUInt();
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAIncludeTypeChanged(int index)
{
    Q_UNUSED(index);

    const int row = ui->listWidgetCustomSA->currentRow();
    if (ui->listWidgetCustomSA->count() == 0 || row == -1 || row >= ui->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = ui->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.includeType = static_cast<PiecePathIncludeType>(CURRENT_DATA(ui->comboBoxIncludeType).toUInt());
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeAngleChanged(int index)
{
    const int i = ui->comboBoxNodes->currentIndex();
    if (i != -1 && index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(ui->comboBoxNodes).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetAngleType(static_cast<PieceNodeAngle>(CURRENT_DATA(ui->comboBoxAngle).toUInt()));
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefBefore()
{
    ui->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefAfter()
{
    ui->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
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
void DialogSeamAllowance::TabChanged(int index)
{
    if (index == ui->tabWidget->indexOf(ui->tabPins)
            || index == ui->tabWidget->indexOf(ui->tabGrainline)
            || index == ui->tabWidget->indexOf(ui->tabLabels))
    {
        if (m_visPins.isNull())
        {
            m_visPins = new VisPiecePins(data);
        }

        m_visPins->SetPins(GetPieceInternals<quint32>(ui->listWidgetPins));

        if (not qApp->getCurrentScene()->items().contains(m_visPins))
        {
            m_visPins->VisualMode(NULL_ID);
        }
        else
        {
            m_visPins->RefreshGeometry();
        }
    }
    else
    {
        if (not m_visPins.isNull())
        {
            delete m_visPins;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateGrainlineValues()
{
    QPlainTextEdit* apleSender[2] = {ui->lineEditRotFormula, ui->lineEditLenFormula};
    bool bFormulasOK[2] = {true, true};

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
        QString qsVal;
        try
        {
            qsFormula.replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->PlainVariables(), qsFormula);
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
            qsVal = tr("Error");
            not flagGPin ? ChangeColor(plbText, Qt::red) : ChangeColor(plbText, okColor);
            bFormulasOK[i] = false;
            plbVal->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
        }

        if (bFormulasOK[i] && qsVal.isEmpty() == false)
        {
            qsVal += qsUnit;
        }
        plbVal->setText(qsVal);
    }

    flagGFormulas = bFormulasOK[0] && bFormulasOK[1];
    if (not flagGFormulas && not flagGPin)
    {
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabGrainline), icon);
    }
    else
    {
        ResetGrainlineWarning();
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateDetailLabelValues()
{
    QPlainTextEdit* apleSender[3] = {ui->lineEditDLWidthFormula, ui->lineEditDLHeightFormula,
                                     ui->lineEditDLAngleFormula};
    bool bFormulasOK[3] = {true, true, true};

    for (int i = 0; i < 3; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = ui->labelDLWidth;
            plbText = ui->labelEditDLWidth;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = ui->labelDLHeight;
            plbText = ui->labelEditDLHeight;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else
        {
            plbVal = ui->labelDLAngle;
            plbText = ui->labelEditDLAngle;
            qsUnit = degreeSymbol;
        }

        plbVal->setToolTip(tr("Value"));

        QString qsFormula = apleSender[i]->toPlainText().simplified();
        QString qsVal;
        try
        {
            qsFormula.replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->PlainVariables(), qsFormula);
            if (qIsInf(dVal) == true || qIsNaN(dVal) == true)
            {
                throw qmu::QmuParserError(tr("Infinite/undefined result"));
            }
            else if ((i == 0 || i == 1) && dVal <= 0.0)
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
            qsVal = tr("Error");
            not flagDPin ? ChangeColor(plbText, Qt::red) : ChangeColor(plbText, okColor);
            bFormulasOK[i] = false;
            plbVal->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
        }

        if (bFormulasOK[i] && qsVal.isEmpty() == false)
        {
            qsVal += qsUnit;
        }
        plbVal->setText(qsVal);
    }

    flagDLAngle = bFormulasOK[2];
    flagDLFormulas = bFormulasOK[0] && bFormulasOK[1];
    if (not flagDLAngle && not (flagDLFormulas || flagDPin) && not flagPLAngle && not (flagPLFormulas || flagPPin))
    {
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), icon);
    }
    else
    {
        ResetLabelsWarning();
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdatePatternLabelValues()
{
    QPlainTextEdit* apleSender[3] = {ui->lineEditPLWidthFormula, ui->lineEditPLHeightFormula,
                                     ui->lineEditPLAngleFormula};
    bool bFormulasOK[3] = {true, true, true};

    for (int i = 0; i < 3; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = ui->labelPLWidth;
            plbText = ui->labelEditPLWidth;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = ui->labelPLHeight;
            plbText = ui->labelEditPLHeight;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else
        {
            plbVal = ui->labelPLAngle;
            plbText = ui->labelEditPLAngle;
            qsUnit = degreeSymbol;
        }

        plbVal->setToolTip(tr("Value"));

        QString qsFormula = apleSender[i]->toPlainText().simplified();
        QString qsVal;
        try
        {
            qsFormula.replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->PlainVariables(), qsFormula);
            if (qIsInf(dVal) == true || qIsNaN(dVal) == true)
            {
                throw qmu::QmuParserError(tr("Infinite/undefined result"));
            }
            else if ((i == 0 || i == 1) && dVal <= 0.0)
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
            qsVal = tr("Error");
            not flagPPin ? ChangeColor(plbText, Qt::red) : ChangeColor(plbText, okColor);
            bFormulasOK[i] = false;
            plbVal->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
        }

        if (bFormulasOK[i] && qsVal.isEmpty() == false)
        {
            qsVal += qsUnit;
        }
        plbVal->setText(qsVal);
    }

    flagPLAngle = bFormulasOK[2];
    flagPLFormulas = bFormulasOK[0] && bFormulasOK[1];
    if (not flagDLAngle && not (flagDLFormulas || flagDPin) && not flagPLAngle && not (flagPLFormulas || flagPPin))
    {
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), icon);
    }
    else
    {
        ResetLabelsWarning();
    }
    CheckState();
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
void DialogSeamAllowance::EnabledGrainline()
{
    if (ui->groupBoxGrainline->isChecked() == true)
    {
        UpdateGrainlineValues();
        GrainlinePinPointChanged();
    }
    else
    {
        flagGFormulas = true;
        ResetGrainlineWarning();
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnabledDetailLabel()
{
    if (ui->groupBoxDetailLabel->isChecked() == true)
    {
        UpdateDetailLabelValues();
        DetailPinPointChanged();
    }
    else
    {
        flagDLAngle = true;
        flagDLFormulas = true;
        ResetLabelsWarning();
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnabledPatternLabel()
{
    if (ui->groupBoxPatternLabel->isChecked() == true)
    {
        UpdatePatternLabelValues();
        PatternPinPointChanged();
    }
    else
    {
        flagPLAngle = true;
        flagPLFormulas = true;
        ResetLabelsWarning();
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EditGrainlineFormula()
{
    QPlainTextEdit* pleFormula;
    bool bCheckZero;
    QString title;

    if (sender() == ui->pushButtonLen)
    {
        pleFormula = ui->lineEditLenFormula;
        bCheckZero = true;
        title = tr("Edit length");
    }
    else if (sender() == ui->pushButtonRot)
    {
        pleFormula = ui->lineEditRotFormula;
        bCheckZero = false;
        title = tr("Edit angle");
    }
    else
    {
        // should not get here!
        return;
    }

    DialogEditWrongFormula dlg(data, NULL_ID, this);
    dlg.setWindowTitle(title);
    dlg.SetFormula(qApp->TrVars()->TryFormulaFromUser(pleFormula->toPlainText(), qApp->Settings()->GetOsSeparator()));
    dlg.setCheckZero(bCheckZero);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString qsFormula = dlg.GetFormula();
        qsFormula.replace("\n", " ");

        if (sender() == ui->pushButtonLen)
        {
            SetGrainlineLength(qsFormula);
        }
        else if (sender() == ui->pushButtonRot)
        {
            SetGrainlineAngle(qsFormula);
        }
        else
        {
            // should not get here!
            pleFormula->setPlainText(qsFormula);
        }
        UpdateGrainlineValues();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EditDLFormula()
{
    QPlainTextEdit* pleFormula;
    bool bCheckZero;
    QString title;

    if (sender() == ui->pushButtonDLHeight)
    {
        pleFormula = ui->lineEditDLHeightFormula;
        bCheckZero = true;
        title = tr("Edit height");
    }
    else if (sender() == ui->pushButtonDLWidth)
    {
        pleFormula = ui->lineEditDLWidthFormula;
        bCheckZero = true;
        title = tr("Edit width");
    }
    else if (sender() == ui->pushButtonDLAngle)
    {
        pleFormula = ui->lineEditDLAngleFormula;
        bCheckZero = false;
        title = tr("Edit angle");
    }
    else
    {
        // should not get here!
        return;
    }

    DialogEditWrongFormula dlg(data, NULL_ID, this);
    dlg.setWindowTitle(title);
    dlg.SetFormula(qApp->TrVars()->TryFormulaFromUser(pleFormula->toPlainText(), qApp->Settings()->GetOsSeparator()));
    dlg.setCheckZero(bCheckZero);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString qsFormula = dlg.GetFormula();
        qsFormula.replace("\n", " ");
        if (sender() == ui->pushButtonDLHeight)
        {
            SetDLHeight(qsFormula);
        }
        else if (sender() == ui->pushButtonDLWidth)
        {
            SetDLWidth(qsFormula);
        }
        else if (sender() == ui->pushButtonDLAngle)
        {
            SetDLAngle(qsFormula);
        }
        else
        {
            // should not get here!
            pleFormula->setPlainText(qsFormula);
        }
        UpdateDetailLabelValues();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EditPLFormula()
{
    QPlainTextEdit* pleFormula;
    bool bCheckZero;
    QString title;

    if (sender() == ui->pushButtonPLHeight)
    {
        pleFormula = ui->lineEditPLHeightFormula;
        bCheckZero = true;
        title = tr("Edit height");
    }
    else if (sender() == ui->pushButtonPLWidth)
    {
        pleFormula = ui->lineEditPLWidthFormula;
        bCheckZero = true;
        title = tr("Edit width");
    }
    else if (sender() == ui->pushButtonPLAngle)
    {
        pleFormula = ui->lineEditPLAngleFormula;
        bCheckZero = false;
        title = tr("Edit angle");
    }
    else
    {
        // should not get here!
        return;
    }

    DialogEditWrongFormula dlg(data, NULL_ID, this);
    dlg.setWindowTitle(title);
    dlg.SetFormula(qApp->TrVars()->TryFormulaFromUser(pleFormula->toPlainText(), qApp->Settings()->GetOsSeparator()));
    dlg.setCheckZero(bCheckZero);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString qsFormula = dlg.GetFormula();
        qsFormula.replace("\n", " ");
        if (sender() == ui->pushButtonPLHeight)
        {
            SetPLHeight(qsFormula);
        }
        else if (sender() == ui->pushButtonPLWidth)
        {
            SetPLWidth(qsFormula);
        }
        else if (sender() == ui->pushButtonPLAngle)
        {
            SetPLAngle(qsFormula);
        }
        else
        {
            // should not get here!
            pleFormula->setPlainText(qsFormula);
        }
        UpdatePatternLabelValues();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployGrainlineRotation()
{
    DeployFormula(ui->lineEditRotFormula, ui->pushButtonShowRot, m_iRotBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployGrainlineLength()
{
    DeployFormula(ui->lineEditLenFormula, ui->pushButtonShowLen, m_iLenBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLWidth()
{
    DeployFormula(ui->lineEditDLWidthFormula, ui->pushButtonShowDLWidth, m_DLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLHeight()
{
    DeployFormula(ui->lineEditDLHeightFormula, ui->pushButtonShowDLHeight, m_DLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLAngle()
{
    DeployFormula(ui->lineEditDLAngleFormula, ui->pushButtonShowDLAngle, m_DLAngleBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLWidth()
{
    DeployFormula(ui->lineEditPLWidthFormula, ui->pushButtonShowPLWidth, m_PLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLHeight()
{
    DeployFormula(ui->lineEditPLHeightFormula, ui->pushButtonShowPLHeight, m_PLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLAngle()
{
    DeployFormula(ui->lineEditPLAngleFormula, ui->pushButtonShowPLAngle, m_PLAngleBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ResetGrainlineWarning()
{
    if (flagGFormulas || flagGPin)
    {
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabGrainline), QIcon());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ResetLabelsWarning()
{
    if (flagDLAngle && (flagDLFormulas || flagDPin) && flagPLAngle && (flagPLFormulas || flagPPin))
    {
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), QIcon());
    }
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

    const QString formulaSABefore = GetFormulaFromUser(ui->plainTextEditFormulaWidthBefore);
    UpdateNodeSABefore(formulaSABefore);
    EnableDefButton(ui->pushButtonDefBefore, formulaSABefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthAfter()
{
    labelEditFormula = ui->labelEditAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidthAfter->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, ui->labelResultAfter, postfix, false, true);

    const QString formulaSAAfter = GetFormulaFromUser(ui->plainTextEditFormulaWidthAfter);
    UpdateNodeSAAfter(formulaSAAfter);
    EnableDefButton(ui->pushButtonDefAfter, formulaSAAfter);
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
    dialog->SetFormula(GetFormulaFromUser(ui->plainTextEditFormulaWidthBefore));
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
    dialog->SetFormula(GetFormulaFromUser(ui->plainTextEditFormulaWidthAfter));
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
void DialogSeamAllowance::GrainlinePinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(ui->comboBoxGrainlineTopPin);
    const quint32 bottomPinId = getCurrentObjectId(ui->comboBoxGrainlineBottomPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagGPin = true;
        color = okColor;

        ResetGrainlineWarning();
    }
    else
    {
        flagGPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = okColor : color = errorColor;

        if (not flagGFormulas && not flagGPin)
        {
            QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
            ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabGrainline), icon);
        }
    }
    UpdateGrainlineValues();
    ChangeColor(ui->labelGrainlineTopPin, color);
    ChangeColor(ui->labelGrainlineBottomPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DetailPinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(ui->comboBoxDLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(ui->comboBoxDLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagDPin = true;
        color = okColor;

        if (flagPPin)
        {
            ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), QIcon());
        }
    }
    else
    {
        flagDPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = okColor : color = errorColor;

        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), icon);
    }
    UpdateDetailLabelValues();
    ChangeColor(ui->labelDLTopLeftPin, color);
    ChangeColor(ui->labelDLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PatternPinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(ui->comboBoxPLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(ui->comboBoxPLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagPPin = true;
        color = okColor;

        if (flagDPin)
        {
            ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), QIcon());
        }
    }
    else
    {
        flagPPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = okColor : color = errorColor;

        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabLabels), icon);
    }
    UpdatePatternLabelValues();
    ChangeColor(ui->labelPLTopLeftPin, color);
    ChangeColor(ui->labelPLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::CreatePiece() const
{
    VPiece piece;
    piece.GetPath().SetNodes(GetPieceInternals<VPieceNode>(ui->listWidgetMainPath));
    piece.SetCustomSARecords(GetPieceInternals<CustomSARecord>(ui->listWidgetCustomSA));
    piece.SetInternalPaths(GetPieceInternals<quint32>(ui->listWidgetInternalPaths));
    piece.SetPins(GetPieceInternals<quint32>(ui->listWidgetPins));
    piece.SetForbidFlipping(ui->checkBoxForbidFlipping->isChecked());
    piece.SetSeamAllowance(ui->checkBoxSeams->isChecked());
    piece.SetName(ui->lineEditName->text());
    piece.SetMx(m_mx);
    piece.SetMy(m_my);
    piece.SetFormulaSAWidth(GetFormulaFromUser(ui->plainTextEditFormulaWidth), m_saWidth);
    piece.GetPatternPieceData().SetLetter(ui->lineEditLetter->text());

    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        piece.GetPatternPieceData().Append(m_conMCP[i]);
    }

    piece.GetPatternPieceData().SetPos(m_oldData.GetPos());
    piece.GetPatternPieceData().SetLabelWidth(GetFormulaFromUser(ui->lineEditDLWidthFormula));
    piece.GetPatternPieceData().SetLabelHeight(GetFormulaFromUser(ui->lineEditDLHeightFormula));
    piece.GetPatternPieceData().SetFontSize(m_oldData.GetFontSize());
    piece.GetPatternPieceData().SetRotation(GetFormulaFromUser(ui->lineEditDLAngleFormula));
    piece.GetPatternPieceData().SetVisible(ui->groupBoxDetailLabel->isChecked());
    piece.GetPatternPieceData().SetCenterPin(getCurrentObjectId(ui->comboBoxDLCenterPin));
    piece.GetPatternPieceData().SetTopLeftPin(getCurrentObjectId(ui->comboBoxDLTopLeftPin));
    piece.GetPatternPieceData().SetBottomRightPin(getCurrentObjectId(ui->comboBoxDLBottomRightPin));

    piece.GetPatternInfo() = m_oldGeom;
    piece.GetPatternInfo().SetVisible(ui->groupBoxPatternLabel->isChecked());
    piece.GetPatternInfo().SetCenterPin(getCurrentObjectId(ui->comboBoxPLCenterPin));
    piece.GetPatternInfo().SetTopLeftPin(getCurrentObjectId(ui->comboBoxPLTopLeftPin));
    piece.GetPatternInfo().SetBottomRightPin(getCurrentObjectId(ui->comboBoxPLBottomRightPin));
    piece.GetPatternInfo().SetLabelWidth(GetFormulaFromUser(ui->lineEditPLWidthFormula));
    piece.GetPatternInfo().SetLabelHeight(GetFormulaFromUser(ui->lineEditPLHeightFormula));
    piece.GetPatternInfo().SetRotation(GetFormulaFromUser(ui->lineEditPLAngleFormula));

    piece.GetGrainlineGeometry() = m_oldGrainline;
    piece.GetGrainlineGeometry().SetVisible(ui->groupBoxGrainline->isChecked());
    piece.GetGrainlineGeometry().SetRotation(GetFormulaFromUser(ui->lineEditRotFormula));
    piece.GetGrainlineGeometry().SetLength(GetFormulaFromUser(ui->lineEditLenFormula));
    piece.GetGrainlineGeometry().SetArrowType(static_cast<ArrowType>(ui->comboBoxArrow->currentIndex()));
    piece.GetGrainlineGeometry().SetCenterPin(getCurrentObjectId(ui->comboBoxGrainlineCenterPin));
    piece.GetGrainlineGeometry().SetTopPin(getCurrentObjectId(ui->comboBoxGrainlineTopPin));
    piece.GetGrainlineGeometry().SetBottomPin(getCurrentObjectId(ui->comboBoxGrainlineBottomPin));

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
void DialogSeamAllowance::NewPin(quint32 pinPoint)
{
    if (pinPoint > NULL_ID)
    {
        const QSharedPointer<VGObject> pin = data->GetGObject(pinPoint);

        QListWidgetItem *item = new QListWidgetItem(pin->name());
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(pinPoint));
        ui->listWidgetPins->addItem(item);
        ui->listWidgetPins->setCurrentRow(ui->listWidgetPins->count()-1);
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
    const quint32 id = CURRENT_DATA(ui->comboBoxNodes).toUInt();

    ui->comboBoxNodes->blockSignals(true);
    ui->comboBoxNodes->clear();

    const QVector<VPieceNode> nodes = GetPieceInternals<VPieceNode>(ui->listWidgetMainPath);

    for (int i = 0; i < nodes.size(); ++i)
    {
        const VPieceNode node = nodes.at(i);
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
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(ui->comboBoxNodes).toUInt());
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
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(ui->comboBoxNodes).toUInt());
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

    const QVector<VPieceNode> nodes = GetPieceInternals<VPieceNode>(ui->listWidgetMainPath);

    for (int i = 0; i < nodes.size(); ++i)
    {
        const VPieceNode &node = nodes.at(i);
        if (node.GetTypeTool() == Tool::NodePoint)
        {
            const QString name = GetNodeName(node);
            box->addItem(name, node.GetId());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPinPoint(QComboBox *box)
{
    SCASSERT(box != nullptr);

    quint32 currentId = NULL_ID;
    if (box->count() > 0)
    {
        currentId = CURRENT_DATA(box).toUInt();
    }

    box->clear();
    box->addItem(QLatin1String("<") + tr("no pin") + QLatin1String(">"), NULL_ID);

    const QVector<quint32> pins = GetPieceInternals<quint32>(ui->listWidgetPins);

    for (int i = 0; i < pins.size(); ++i)
    {
        const QSharedPointer<VGObject> pin = data->GetGObject(pins.at(i));
        box->addItem(pin->name(), pins.at(i));
    }

    const int index = ui->comboBoxNodes->findData(currentId);
    if (index != -1)
    {
        box->setCurrentIndex(index);
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

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &DialogSeamAllowance::AddUpdate);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &DialogSeamAllowance::Cancel);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &DialogSeamAllowance::Remove);
    connect(ui->listWidgetMCP, &QListWidget::itemClicked, this, &DialogSeamAllowance::SetEditMode);
    connect(ui->comboBoxMaterial, &QComboBox::currentTextChanged, this, &DialogSeamAllowance::MaterialChanged);

    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitLabelsTab()
{
    m_DLWidthBaseHeight = ui->lineEditDLWidthFormula->height();
    m_DLHeightBaseHeight = ui->lineEditDLHeightFormula->height();
    m_DLAngleBaseHeight = ui->lineEditDLAngleFormula->height();

    connect(ui->groupBoxDetailLabel, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledDetailLabel);
    InitPinPoint(ui->comboBoxDLCenterPin);
    InitPinPoint(ui->comboBoxDLTopLeftPin);
    InitPinPoint(ui->comboBoxDLBottomRightPin);

    connect(ui->comboBoxDLTopLeftPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::DetailPinPointChanged);
    connect(ui->comboBoxDLBottomRightPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::DetailPinPointChanged);

    connect(ui->pushButtonDLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);
    connect(ui->pushButtonDLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);
    connect(ui->pushButtonDLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);

    connect(ui->lineEditDLWidthFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);
    connect(ui->lineEditDLHeightFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);
    connect(ui->lineEditDLAngleFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);

    connect(ui->pushButtonShowDLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLWidth);
    connect(ui->pushButtonShowDLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLHeight);
    connect(ui->pushButtonShowDLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLAngle);

    EnabledDetailLabel();

    m_PLWidthBaseHeight = ui->lineEditPLWidthFormula->height();
    m_PLHeightBaseHeight = ui->lineEditPLHeightFormula->height();
    m_PLAngleBaseHeight = ui->lineEditPLAngleFormula->height();

    connect(ui->groupBoxPatternLabel, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledPatternLabel);
    InitPinPoint(ui->comboBoxPLCenterPin);
    InitPinPoint(ui->comboBoxPLTopLeftPin);
    InitPinPoint(ui->comboBoxPLBottomRightPin);

    connect(ui->comboBoxPLTopLeftPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PatternPinPointChanged);
    connect(ui->comboBoxPLBottomRightPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PatternPinPointChanged);

    connect(ui->pushButtonPLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);
    connect(ui->pushButtonPLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);
    connect(ui->pushButtonPLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);

    connect(ui->lineEditPLWidthFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);
    connect(ui->lineEditPLHeightFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);
    connect(ui->lineEditPLAngleFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);

    connect(ui->pushButtonShowPLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLWidth);
    connect(ui->pushButtonShowPLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLHeight);
    connect(ui->pushButtonShowPLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLAngle);

    EnabledPatternLabel();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitGrainlineTab()
{
    connect(ui->groupBoxGrainline, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledGrainline);
    connect(ui->pushButtonRot, &QPushButton::clicked, this, &DialogSeamAllowance::EditGrainlineFormula);
    connect(ui->pushButtonLen, &QPushButton::clicked, this, &DialogSeamAllowance::EditGrainlineFormula);
    connect(ui->lineEditLenFormula, &QPlainTextEdit::textChanged, this, &DialogSeamAllowance::UpdateGrainlineValues);
    connect(ui->lineEditRotFormula, &QPlainTextEdit::textChanged, this, &DialogSeamAllowance::UpdateGrainlineValues);

    connect(ui->pushButtonShowRot, &QPushButton::clicked, this, &DialogSeamAllowance::DeployGrainlineRotation);
    connect(ui->pushButtonShowLen, &QPushButton::clicked, this, &DialogSeamAllowance::DeployGrainlineLength);

    EnabledGrainline();

    ui->comboBoxArrow->addItem(tr("Both"));
    ui->comboBoxArrow->addItem(tr("Just front"));
    ui->comboBoxArrow->addItem(tr("Just rear"));

    m_iRotBaseHeight = ui->lineEditRotFormula->height();
    m_iLenBaseHeight = ui->lineEditLenFormula->height();

    InitPinPoint(ui->comboBoxGrainlineCenterPin);
    InitPinPoint(ui->comboBoxGrainlineTopPin);
    InitPinPoint(ui->comboBoxGrainlineBottomPin);

    connect(ui->comboBoxGrainlineTopPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::GrainlinePinPointChanged);
    connect(ui->comboBoxGrainlineBottomPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::GrainlinePinPointChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPinsTab()
{
    ui->listWidgetPins->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetPins, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowPinsContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitAllPinComboboxes()
{
    InitPinPoint(ui->comboBoxGrainlineCenterPin);
    InitPinPoint(ui->comboBoxGrainlineTopPin);
    InitPinPoint(ui->comboBoxGrainlineBottomPin);

    InitPinPoint(ui->comboBoxDLCenterPin);
    InitPinPoint(ui->comboBoxDLTopLeftPin);
    InitPinPoint(ui->comboBoxDLBottomRightPin);

    InitPinPoint(ui->comboBoxPLCenterPin);
    InitPinPoint(ui->comboBoxPLTopLeftPin);
    InitPinPoint(ui->comboBoxPLBottomRightPin);
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

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QVector<T> DialogSeamAllowance::GetPieceInternals(const QListWidget *list) const
{
    SCASSERT(list != nullptr)
    QVector<T> internals;
    for (qint32 i = 0; i < list->count(); ++i)
    {
        QListWidgetItem *item = list->item(i);
        internals.append(qvariant_cast<T>(item->data(Qt::UserRole)));
    }
    return internals;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetGrainlineAngle(QString angleFormula)
{
    if (angleFormula.isEmpty())
    {
        angleFormula = QString("0");
    }

    const QString formula = qApp->TrVars()->FormulaToUser(angleFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployGrainlineRotation();
    }
    ui->lineEditRotFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditRotFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetGrainlineLength(QString lengthFormula)
{
    if (lengthFormula.isEmpty())
    {
        lengthFormula = QString().setNum(UnitConvertor(1, Unit::Cm, *data->GetPatternUnit()));
    }

    const QString formula = qApp->TrVars()->FormulaToUser(lengthFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployGrainlineLength();
    }

    ui->lineEditLenFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditLenFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetDLWidth(QString widthFormula)
{
    if (widthFormula.isEmpty())
    {
        widthFormula = QString().setNum(UnitConvertor(1, Unit::Cm, *data->GetPatternUnit()));
    }

    const QString formula = qApp->TrVars()->FormulaToUser(widthFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployDLWidth();
    }

    ui->lineEditDLWidthFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditDLWidthFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetDLHeight(QString heightFormula)
{
    if (heightFormula.isEmpty())
    {
        heightFormula = QString().setNum(UnitConvertor(1, Unit::Cm, *data->GetPatternUnit()));
    }

    const QString formula = qApp->TrVars()->FormulaToUser(heightFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployDLHeight();
    }

    ui->lineEditDLHeightFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditDLHeightFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetDLAngle(QString angleFormula)
{
    if (angleFormula.isEmpty())
    {
        angleFormula = QString("0");
    }

    const QString formula = qApp->TrVars()->FormulaToUser(angleFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployDLAngle();
    }

    ui->lineEditDLAngleFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditDLAngleFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPLWidth(QString widthFormula)
{
    if (widthFormula.isEmpty())
    {
        widthFormula = QString().setNum(UnitConvertor(1, Unit::Cm, *data->GetPatternUnit()));
    }

    const QString formula = qApp->TrVars()->FormulaToUser(widthFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployPLWidth();
    }

    ui->lineEditPLWidthFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditPLWidthFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPLHeight(QString heightFormula)
{
    if (heightFormula.isEmpty())
    {
        heightFormula = QString().setNum(UnitConvertor(1, Unit::Cm, *data->GetPatternUnit()));
    }

    const QString formula = qApp->TrVars()->FormulaToUser(heightFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployPLHeight();
    }

    ui->lineEditPLHeightFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditPLHeightFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPLAngle(QString angleFormula)
{
    if (angleFormula.isEmpty())
    {
        angleFormula = QString("0");
    }

    const QString formula = qApp->TrVars()->FormulaToUser(angleFormula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployPLAngle();
    }

    ui->lineEditPLAngleFormula->setPlainText(formula);

    MoveCursorToEnd(ui->lineEditPLAngleFormula);
}
