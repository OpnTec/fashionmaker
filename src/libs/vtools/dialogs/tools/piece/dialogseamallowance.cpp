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
#include "ui_tabpaths.h"
#include "ui_tablabels.h"
#include "ui_tabgrainline.h"
#include "ui_tabpins.h"
#include "ui_tabpassmarks.h"
#include "../vwidgets/fancytabbar/fancytabbar.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/calculator.h"
#include "visualization/path/vistoolpiece.h"
#include "visualization/path/vispiecepins.h"
#include "../dialogpiecepath.h"
#include "../../../undocommands/savepiecepathoptions.h"
#include "../../support/dialogeditwrongformula.h"
#include "../../../tools/vtoolseamallowance.h"

#include <QMenu>
#include <QTimer>
#include <QtNumeric>

enum TabOrder {Paths=0, Labels=1, Grainline=2, Pins=3, Passmarks=4, Count=5};

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
      uiTabPaths(new Ui::TabPaths),
      uiTabLabels(new Ui::TabLabels),
      uiTabGrainline(new Ui::TabGrainline),
      uiTabPins(new Ui::TabPins),
      uiTabPassmarks(new Ui::TabPassmarks),
      m_tabPaths(new QWidget),
      m_tabLabels(new QWidget),
      m_tabGrainline(new QWidget),
      m_tabPins(new QWidget),
      m_tabPassmarks(new QWidget),
      m_ftb(new FancyTabBar(FancyTabBar::Left, this)),
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
    InitFancyTabBar();
    EnableApply(applyAllowed);

    InitMainPathTab();
    InitSeamAllowanceTab();
    InitInternalPathsTab();
    InitPatternPieceDataTab();
    InitLabelsTab();
    InitGrainlineTab();
    InitPinsTab();
    InitPassmarksTab();

    flagName = true;//We have default name of piece.
    ChangeColor(uiTabLabels->labelEditName, okColor);
    flagError = MainPathIsValid();
    CheckState();

    if (not applyAllowed)
    {
        vis = new VisToolPiece(data);
    }

    m_ftb->SetCurrentIndex(TabOrder::Paths);// Show always first tab active on start.
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::~DialogSeamAllowance()
{
    delete m_visPins;
    delete m_tabPassmarks;
    delete m_tabPins;
    delete m_tabGrainline;
    delete m_tabLabels;
    delete m_tabPaths;
    delete uiTabPassmarks;
    delete uiTabPins;
    delete uiTabGrainline;
    delete uiTabLabels;
    delete uiTabPaths;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableApply(bool enable)
{
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(enable);
    applyAllowed = enable;

    uiTabPaths->tabSeamAllowance->setEnabled(applyAllowed);
    uiTabPaths->tabInternalPaths->setEnabled(applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Labels, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Grainline, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Pins, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Passmarks, applyAllowed);
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::GetPiece() const
{
    return CreatePiece();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetPiece(const VPiece &piece)
{
    uiTabPaths->listWidgetMainPath->clear();
    for (int i = 0; i < piece.GetPath().CountNodes(); ++i)
    {
        NewMainPathItem(piece.GetPath().at(i));
    }

    uiTabPaths->checkBoxHideMainPath->setChecked(piece.IsHideMainPath());
    uiTabPaths->listWidgetCustomSA->blockSignals(true);
    uiTabPaths->listWidgetCustomSA->clear();
    for (int i = 0; i < piece.GetCustomSARecords().size(); ++i)
    {
        NewCustomSA(piece.GetCustomSARecords().at(i));
    }
    uiTabPaths->listWidgetCustomSA->blockSignals(false);

    uiTabPaths->listWidgetInternalPaths->clear();
    for (int i = 0; i < piece.GetInternalPaths().size(); ++i)
    {
        NewInternalPath(piece.GetInternalPaths().at(i));
    }

    uiTabPins->listWidgetPins->clear();
    for (int i = 0; i < piece.GetPins().size(); ++i)
    {
        NewPin(piece.GetPins().at(i));
    }

    InitAllPinComboboxes();

    uiTabPaths->comboBoxStartPoint->blockSignals(true);
    uiTabPaths->comboBoxStartPoint->clear();
    uiTabPaths->comboBoxStartPoint->blockSignals(false);

    uiTabPaths->comboBoxEndPoint->blockSignals(true);
    uiTabPaths->comboBoxEndPoint->clear();
    uiTabPaths->comboBoxEndPoint->blockSignals(false);

    CustomSAChanged(0);

    uiTabPaths->checkBoxForbidFlipping->setChecked(piece.IsForbidFlipping());
    uiTabPaths->checkBoxSeams->setChecked(piece.IsSeamAllowance());
    uiTabPaths->checkBoxBuiltIn->setChecked(piece.IsSeamAllowanceBuiltIn());
    uiTabLabels->lineEditName->setText(piece.GetName());

    const QString width = qApp->TrVars()->FormulaToUser(piece.GetFormulaSAWidth(), qApp->Settings()->GetOsSeparator());
    uiTabPaths->plainTextEditFormulaWidth->setPlainText(width);
    m_saWidth = piece.GetSAWidth();

    m_mx = piece.GetMx();
    m_my = piece.GetMy();

    uiTabLabels->lineEditLetter->setText(piece.GetPatternPieceData().GetLetter());

    m_conMCP.clear();
    for (int i = 0; i < piece.GetPatternPieceData().GetMCPCount(); ++i)
    {
        m_conMCP << piece.GetPatternPieceData().GetMCP(i);
    }

    UpdateList();

    uiTabGrainline->comboBoxArrow->setCurrentIndex(int(piece.GetGrainlineGeometry().GetArrowType()));

    m_oldData = piece.GetPatternPieceData();
    uiTabLabels->groupBoxDetailLabel->setChecked(m_oldData.IsVisible());
    ChangeCurrentData(uiTabLabels->comboBoxDLCenterPin, m_oldData.CenterPin());
    ChangeCurrentData(uiTabLabels->comboBoxDLTopLeftPin, m_oldData.TopLeftPin());
    ChangeCurrentData(uiTabLabels->comboBoxDLBottomRightPin, m_oldData.BottomRightPin());
    SetDLWidth(m_oldData.GetLabelWidth());
    SetDLHeight(m_oldData.GetLabelHeight());
    SetDLAngle(m_oldData.GetRotation());

    m_oldGeom = piece.GetPatternInfo();
    uiTabLabels->groupBoxPatternLabel->setChecked(m_oldGeom.IsVisible());
    ChangeCurrentData(uiTabLabels->comboBoxPLCenterPin, m_oldGeom.CenterPin());
    ChangeCurrentData(uiTabLabels->comboBoxPLTopLeftPin, m_oldGeom.TopLeftPin());
    ChangeCurrentData(uiTabLabels->comboBoxPLBottomRightPin, m_oldGeom.BottomRightPin());
    SetPLWidth(m_oldGeom.GetLabelWidth());
    SetPLHeight(m_oldGeom.GetLabelHeight());
    SetPLAngle(m_oldGeom.GetRotation());

    m_oldGrainline = piece.GetGrainlineGeometry();
    uiTabGrainline->groupBoxGrainline->setChecked(m_oldGrainline.IsVisible());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineCenterPin, m_oldGrainline.CenterPin());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineTopPin, m_oldGrainline.TopPin());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineBottomPin, m_oldGrainline.BottomPin());
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
            if (uiTabPaths->listWidgetMainPath->count() > 1)
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
    uiTabPaths->plainTextEditFormulaWidth->blockSignals(true);
    uiTabPaths->plainTextEditFormulaWidthBefore->blockSignals(true);
    uiTabPaths->plainTextEditFormulaWidthAfter->blockSignals(true);
    uiTabGrainline->lineEditRotFormula->blockSignals(true);
    uiTabGrainline->lineEditLenFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::showEvent(QShowEvent *event)
{
    // Skip DialotTool implementation. For this tool don't need disable resizing because it uses a scroll area.
    QDialog::showEvent( event );
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateList()
{
    uiTabLabels->listWidgetMCP->clear();
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

        uiTabLabels->listWidgetMCP->addItem(qsText);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::AddUpdate()
{
    MaterialCutPlacement mcp;
    QStringList qslUserMaterials = qApp->Settings()->GetUserDefinedMaterials();

    const int i = CURRENT_DATA(uiTabLabels->comboBoxMaterial).toInt();
    QString qsMat = uiTabLabels->comboBoxMaterial->currentText();
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
            uiTabLabels->comboBoxMaterial->addItem(mcp.m_qsMaterialUserDef, int(MaterialType::mtUserDefined));
        }
    }

    mcp.m_iCutNumber = uiTabLabels->spinBoxCutNumber->value();
    mcp.m_ePlacement = PlacementType(uiTabLabels->comboBoxPlacement->currentIndex());

    if (m_bAddMode == true)
    {
        m_conMCP << mcp;
    }
    else
    {
        int iR = uiTabLabels->listWidgetMCP->currentRow();
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
    int iR = uiTabLabels->listWidgetMCP->currentRow();
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
            ChangeColor(uiTabLabels->labelEditName, Qt::red);
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + QLatin1String("*"));
            QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabPieceLabelData), icon);
        }
        else
        {
            flagName = true;
            ChangeColor(uiTabLabels->labelEditName, okColor);
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabPieceLabelData),
                                               QIcon());
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::MaterialChanged()
{
    uiTabLabels->pushButtonAdd->setEnabled(uiTabLabels->comboBoxMaterial->currentText().isEmpty() == false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowMainPathContextMenu(const QPoint &pos)
{
    const int row = uiTabPaths->listWidgetMainPath->currentRow();
    if (uiTabPaths->listWidgetMainPath->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetMainPath->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());

    QListWidgetItem *rowItem = uiTabPaths->listWidgetMainPath->item(row);
    SCASSERT(rowItem != nullptr);
    VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

    QAction *actionPassmark = nullptr;
    QAction *actionReverse = nullptr;
    if (rowNode.GetTypeTool() != Tool::NodePoint)
    {
        actionReverse = menu->addAction(tr("Reverse"));
        actionReverse->setCheckable(true);
        actionReverse->setChecked(rowNode.GetReverse());
    }
    else
    {
        actionPassmark = menu->addAction(tr("Passmark"));
        actionPassmark->setCheckable(true);
        actionPassmark->setChecked(rowNode.IsPassmark());
    }

    QAction *actionExcluded = menu->addAction(tr("Excluded"));
    actionExcluded->setCheckable(true);
    actionExcluded->setChecked(rowNode.IsExcluded());

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPaths->listWidgetMainPath->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPaths->listWidgetMainPath->item(row);
    }
    else if (rowNode.GetTypeTool() != Tool::NodePoint && selectedAction == actionReverse)
    {
        rowNode.SetReverse(not rowNode.GetReverse());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
    }
    else if (selectedAction == actionExcluded)
    {
        rowNode.SetExcluded(not rowNode.IsExcluded());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
        rowItem->setFont(NodeFont(rowNode.IsExcluded()));
    }
    else if (selectedAction == actionPassmark)
    {
        rowNode.SetPassmark(not rowNode.IsPassmark());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
    }

    ValidObjects(MainPathIsValid());
    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowCustomSAContextMenu(const QPoint &pos)
{
    const int row = uiTabPaths->listWidgetCustomSA->currentRow();
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetCustomSA->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionOption = menu->addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

    QListWidgetItem *rowItem = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = menu->addAction(tr("Reverse"));
    actionReverse->setCheckable(true);
    actionReverse->setChecked(record.reverse);

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPaths->listWidgetCustomSA->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPaths->listWidgetCustomSA->item(row);
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
    const int row = uiTabPaths->listWidgetInternalPaths->currentRow();
    if (uiTabPaths->listWidgetInternalPaths->count() == 0 || row == -1
            || row >= uiTabPaths->listWidgetInternalPaths->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionOption = menu->addAction(QIcon::fromTheme("preferences-other"), tr("Options"));
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPaths->listWidgetInternalPaths->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPaths->listWidgetInternalPaths->item(row);
    }
    else if (selectedAction == actionOption)
    {
        QListWidgetItem *rowItem = uiTabPaths->listWidgetInternalPaths->item(row);
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
    const int row = uiTabPins->listWidgetPins->currentRow();
    if (uiTabPins->listWidgetPins->count() == 0 || row == -1 || row >= uiTabPins->listWidgetPins->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPins->listWidgetPins->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPins->listWidgetPins->item(row);
        FancyTabChanged(m_ftb->CurrentIndex());
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
    InitPassmarksList();
    CustomSAChanged(uiTabPaths->listWidgetCustomSA->currentRow());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableSeamAllowance(bool enable)
{
    uiTabPaths->checkBoxBuiltIn->setEnabled(enable);
    uiTabPaths->groupBoxAutomatic->setEnabled(enable);
    uiTabPaths->groupBoxCustom->setEnabled(enable);

    if (enable)
    {
        InitNodesList();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeChanged(int index)
{
    uiTabPaths->plainTextEditFormulaWidthBefore->setDisabled(true);
    uiTabPaths->toolButtonExprBefore->setDisabled(true);
    uiTabPaths->pushButtonDefBefore->setDisabled(true);

    uiTabPaths->plainTextEditFormulaWidthAfter->setDisabled(true);
    uiTabPaths->toolButtonExprAfter->setDisabled(true);
    uiTabPaths->pushButtonDefAfter->setDisabled(true);

    uiTabPaths->comboBoxAngle->setDisabled(true);

    uiTabPaths->comboBoxAngle->blockSignals(true);

    if (index != -1)
    {
        const VPiece piece = CreatePiece();
        const int nodeIndex = piece.GetPath().indexOfNode(CURRENT_DATA(uiTabPaths->comboBoxNodes).toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = piece.GetPath().at(nodeIndex);

            // Seam alowance before
            uiTabPaths->plainTextEditFormulaWidthBefore->setEnabled(true);
            uiTabPaths->toolButtonExprBefore->setEnabled(true);

            QString w1Formula = node.GetFormulaSABefore();
            EnableDefButton(uiTabPaths->pushButtonDefBefore, w1Formula);
            w1Formula = qApp->TrVars()->FormulaToUser(w1Formula, qApp->Settings()->GetOsSeparator());
            if (w1Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthBeforeFormulaTextEdit();
            }
            uiTabPaths->plainTextEditFormulaWidthBefore->setPlainText(w1Formula);
            MoveCursorToEnd(uiTabPaths->plainTextEditFormulaWidthBefore);

            // Seam alowance after
            uiTabPaths->plainTextEditFormulaWidthAfter->setEnabled(true);
            uiTabPaths->toolButtonExprAfter->setEnabled(true);

            QString w2Formula = node.GetFormulaSAAfter();
            EnableDefButton(uiTabPaths->pushButtonDefAfter, w2Formula);
            w2Formula = qApp->TrVars()->FormulaToUser(w2Formula, qApp->Settings()->GetOsSeparator());
            if (w2Formula.length() > 80)// increase height if needed.
            {
                this->DeployWidthAfterFormulaTextEdit();
            }
            uiTabPaths->plainTextEditFormulaWidthAfter->setPlainText(w2Formula);
            MoveCursorToEnd(uiTabPaths->plainTextEditFormulaWidthAfter);

            // Angle type
            uiTabPaths->comboBoxAngle->setEnabled(true);
            const int index = uiTabPaths->comboBoxAngle->findData(static_cast<unsigned char>(node.GetAngleType()));
            if (index != -1)
            {
                uiTabPaths->comboBoxAngle->setCurrentIndex(index);
            }
        }
    }
    else
    {
        uiTabPaths->plainTextEditFormulaWidthBefore->setPlainText("");
        uiTabPaths->plainTextEditFormulaWidthAfter->setPlainText("");
        uiTabPaths->comboBoxAngle->setCurrentIndex(-1);
    }
    uiTabPaths->comboBoxAngle->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PassmarkChanged(int index)
{
    uiTabPassmarks->radioButtonOneLine->setDisabled(true);
    uiTabPassmarks->radioButtonTwoLines->setDisabled(true);
    uiTabPassmarks->radioButtonThreeLines->setDisabled(true);
    uiTabPassmarks->radioButtonTMark->setDisabled(true);
    uiTabPassmarks->radioButtonVMark->setDisabled(true);

    uiTabPassmarks->radioButtonStraightforward->setDisabled(true);
    uiTabPassmarks->radioButtonBisector->setDisabled(true);
    uiTabPassmarks->radioButtonIntersection->setDisabled(true);

    uiTabPassmarks->checkBoxShowSecondPassmark->setDisabled(true);
    uiTabPassmarks->checkBoxShowSecondPassmark->blockSignals(true);

    uiTabPassmarks->groupBoxMarkType->blockSignals(true);
    uiTabPassmarks->groupBoxAngleType->blockSignals(true);

    if (index != -1)
    {
        const VPiece piece = CreatePiece();
        const int nodeIndex = piece.GetPath().indexOfNode(CURRENT_DATA(uiTabPassmarks->comboBoxPassmarks).toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = piece.GetPath().at(nodeIndex);

            // Line type
            uiTabPassmarks->radioButtonOneLine->setEnabled(true);
            uiTabPassmarks->radioButtonTwoLines->setEnabled(true);
            uiTabPassmarks->radioButtonThreeLines->setEnabled(true);
            uiTabPassmarks->radioButtonTMark->setEnabled(true);
            uiTabPassmarks->radioButtonVMark->setEnabled(true);

            switch(node.GetPassmarkLineType())
            {
                case PassmarkLineType::OneLine:
                    uiTabPassmarks->radioButtonOneLine->setChecked(true);
                    break;
                case PassmarkLineType::TwoLines:
                    uiTabPassmarks->radioButtonTwoLines->setChecked(true);
                    break;
                case PassmarkLineType::ThreeLines:
                    uiTabPassmarks->radioButtonThreeLines->setChecked(true);
                    break;
                case PassmarkLineType::TMark:
                    uiTabPassmarks->radioButtonTMark->setChecked(true);
                    break;
                case PassmarkLineType::VMark:
                    uiTabPassmarks->radioButtonVMark->setChecked(true);
                    break;
                default:
                    break;
            }

            // Angle type
            uiTabPassmarks->radioButtonStraightforward->setEnabled(true);
            uiTabPassmarks->radioButtonBisector->setEnabled(true);
            uiTabPassmarks->radioButtonIntersection->setEnabled(true);

            switch(node.GetPassmarkAngleType())
            {
                case PassmarkAngleType::Straightforward:
                    uiTabPassmarks->radioButtonStraightforward->setChecked(true);
                    break;
                case PassmarkAngleType::Bisector:
                    uiTabPassmarks->radioButtonBisector->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection:
                    uiTabPassmarks->radioButtonIntersection->setChecked(true);
                    break;
                default:
                    break;
            }

            // Show the second option
            uiTabPassmarks->checkBoxShowSecondPassmark->setEnabled(true);
            uiTabPassmarks->checkBoxShowSecondPassmark->setChecked(node.IsShowSecondPassmark());
        }
    }

    uiTabPassmarks->checkBoxShowSecondPassmark->blockSignals(false);

    uiTabPassmarks->groupBoxMarkType->blockSignals(false);
    uiTabPassmarks->groupBoxAngleType->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAStartPointChanged(int index)
{
    Q_UNUSED(index);

    const int row = uiTabPaths->listWidgetCustomSA->currentRow();
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.startPoint = CURRENT_DATA(uiTabPaths->comboBoxStartPoint).toUInt();
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAEndPointChanged(int index)
{
    Q_UNUSED(index);

    const int row = uiTabPaths->listWidgetCustomSA->currentRow();
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.endPoint = CURRENT_DATA(uiTabPaths->comboBoxEndPoint).toUInt();
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CSAIncludeTypeChanged(int index)
{
    Q_UNUSED(index);

    const int row = uiTabPaths->listWidgetCustomSA->currentRow();
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetCustomSA->count())
    {
        return;
    }

    QListWidgetItem *rowItem = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));
    record.includeType = static_cast<PiecePathIncludeType>(CURRENT_DATA(uiTabPaths->comboBoxIncludeType).toUInt());
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeAngleChanged(int index)
{
    const int i = uiTabPaths->comboBoxNodes->currentIndex();
    if (i != -1 && index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPaths->comboBoxNodes).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetAngleType(static_cast<PieceNodeAngle>(CURRENT_DATA(uiTabPaths->comboBoxAngle).toUInt()));
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefBefore()
{
    uiTabPaths->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefAfter()
{
    uiTabPaths->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::CustomSAChanged(int row)
{
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1 || row >= uiTabPaths->listWidgetCustomSA->count())
    {
        uiTabPaths->comboBoxStartPoint->blockSignals(true);
        uiTabPaths->comboBoxStartPoint->clear();
        uiTabPaths->comboBoxStartPoint->blockSignals(false);

        uiTabPaths->comboBoxEndPoint->blockSignals(true);
        uiTabPaths->comboBoxEndPoint->clear();
        uiTabPaths->comboBoxEndPoint->blockSignals(false);

        uiTabPaths->comboBoxIncludeType->blockSignals(true);
        uiTabPaths->comboBoxIncludeType->clear();
        uiTabPaths->comboBoxIncludeType->blockSignals(false);
        return;
    }

    const QListWidgetItem *item = uiTabPaths->listWidgetCustomSA->item( row );
    SCASSERT(item != nullptr);
    const CustomSARecord record = qvariant_cast<CustomSARecord>(item->data(Qt::UserRole));

    uiTabPaths->comboBoxStartPoint->blockSignals(true);
    InitCSAPoint(uiTabPaths->comboBoxStartPoint);
    {
        const int index = uiTabPaths->comboBoxStartPoint->findData(record.startPoint);
        if (index != -1)
        {
            uiTabPaths->comboBoxStartPoint->setCurrentIndex(index);
        }
    }
    uiTabPaths->comboBoxStartPoint->blockSignals(false);

    uiTabPaths->comboBoxEndPoint->blockSignals(true);
    InitCSAPoint(uiTabPaths->comboBoxEndPoint);
    {
        const int index = uiTabPaths->comboBoxEndPoint->findData(record.endPoint);
        if (index != -1)
        {
            uiTabPaths->comboBoxEndPoint->setCurrentIndex(index);
        }
    }
    uiTabPaths->comboBoxEndPoint->blockSignals(false);

    uiTabPaths->comboBoxIncludeType->blockSignals(true);
    InitSAIncludeType();
    {
        const int index = uiTabPaths->comboBoxIncludeType->findData(static_cast<unsigned char>(record.includeType));
        if (index != -1)
        {
            uiTabPaths->comboBoxIncludeType->setCurrentIndex(index);
        }
    }
    uiTabPaths->comboBoxIncludeType->blockSignals(false);
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
void DialogSeamAllowance::FancyTabChanged(int index)
{
    if (index < 0 || index >= TabOrder::Count)
    {
        return;
    }

    m_tabPaths->hide();
    m_tabLabels->hide();
    m_tabGrainline->hide();
    m_tabPins->hide();
    m_tabPassmarks->hide();

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
    switch(index)
    {
        case TabOrder::Paths:
            m_tabPaths->show();
            break;
        case TabOrder::Labels:
            m_tabLabels->show();
            break;
        case TabOrder::Grainline:
            m_tabGrainline->show();
            break;
        case TabOrder::Pins:
            m_tabPins->show();
            break;
        case TabOrder::Passmarks:
            m_tabPassmarks->show();
            break;
    }
QT_WARNING_POP

    if (index == TabOrder::Pins || index == TabOrder::Grainline
            || (index == TabOrder::Labels &&
                uiTabLabels->tabWidget->currentIndex() == uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels)))
    {
        ShowPins();
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
void DialogSeamAllowance::TabChanged(int index)
{
    if (index == uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels))
    {
        ShowPins();
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
void DialogSeamAllowance::PassmarkLineTypeChanged(int id)
{
    const int i = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPassmarks->comboBoxPassmarks).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

            PassmarkLineType lineType = PassmarkLineType::OneLine;
            if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonOneLine))
            {
                lineType = PassmarkLineType::OneLine;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonTwoLines))
            {
                lineType = PassmarkLineType::TwoLines;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonThreeLines))
            {
                lineType = PassmarkLineType::ThreeLines;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonTMark))
            {
                lineType = PassmarkLineType::TMark;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonVMark))
            {
                lineType = PassmarkLineType::VMark;
            }

            rowNode.SetPassmarkLineType(lineType);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
            rowItem->setText(GetNodeName(rowNode, true));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PassmarkAngleTypeChanged(int id)
{
    const int i = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPassmarks->comboBoxPassmarks).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

            PassmarkAngleType angleType = PassmarkAngleType::Straightforward;
            if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonStraightforward))
            {
                angleType = PassmarkAngleType::Straightforward;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonBisector))
            {
                angleType = PassmarkAngleType::Bisector;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersection))
            {
                angleType = PassmarkAngleType::Intersection;
            }

            rowNode.SetPassmarkAngleType(angleType);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PassmarkShowSecondChanged(int state)
{
    const int i = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPassmarks->comboBoxPassmarks).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetShowSecondPassmark(state);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateGrainlineValues()
{
    QPlainTextEdit* apleSender[2] = {uiTabGrainline->lineEditRotFormula, uiTabGrainline->lineEditLenFormula};
    bool bFormulasOK[2] = {true, true};

    for (int i = 0; i < 2; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = uiTabGrainline->labelRot;
            plbText = uiTabGrainline->labelEditRot;
            qsUnit = degreeSymbol;
        }
        else
        {
            plbVal = uiTabGrainline->labelLen;
            plbText = uiTabGrainline->labelEditLen;
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
        m_ftb->SetTabText(TabOrder::Grainline, tr("Grainline") + QLatin1String("*"));
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
    QPlainTextEdit* apleSender[3] = {uiTabLabels->lineEditDLWidthFormula, uiTabLabels->lineEditDLHeightFormula,
                                     uiTabLabels->lineEditDLAngleFormula};
    bool bFormulasOK[3] = {true, true, true};

    for (int i = 0; i < 3; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = uiTabLabels->labelDLWidth;
            plbText = uiTabLabels->labelEditDLWidth;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = uiTabLabels->labelDLHeight;
            plbText = uiTabLabels->labelEditDLHeight;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else
        {
            plbVal = uiTabLabels->labelDLAngle;
            plbText = uiTabLabels->labelEditDLAngle;
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
    if (not flagDLAngle || not (flagDLFormulas || flagDPin) || not flagPLAngle || not (flagPLFormulas || flagPPin))
    {
        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + QLatin1String("*"));
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
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
    QPlainTextEdit* apleSender[3] = {uiTabLabels->lineEditPLWidthFormula, uiTabLabels->lineEditPLHeightFormula,
                                     uiTabLabels->lineEditPLAngleFormula};
    bool bFormulasOK[3] = {true, true, true};

    for (int i = 0; i < 3; ++i)
    {
        QLabel* plbVal;
        QLabel* plbText;
        QString qsUnit;
        if (i == 0)
        {
            plbVal = uiTabLabels->labelPLWidth;
            plbText = uiTabLabels->labelEditPLWidth;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = uiTabLabels->labelPLHeight;
            plbText = uiTabLabels->labelEditPLHeight;
            qsUnit = QLatin1String(" ") + VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        else
        {
            plbVal = uiTabLabels->labelPLAngle;
            plbText = uiTabLabels->labelEditPLAngle;
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
    if (not flagDLAngle || not (flagDLFormulas || flagDPin) || not flagPLAngle || not (flagPLFormulas || flagPPin))
    {
        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + QLatin1String("*"));
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
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
    uiTabLabels->pushButtonAdd->setText(tr("Add"));
    uiTabLabels->pushButtonCancel->hide();
    uiTabLabels->pushButtonRemove->hide();
    uiTabLabels->listWidgetMCP->setCurrentRow(-1);
    m_bAddMode = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetEditMode()
{
    int iR = uiTabLabels->listWidgetMCP->currentRow();
    // this method can be called by clicking on item or by update. In the latter case there is nothing else to do!
    if (iR < 0 || iR >= m_conMCP.count())
    {
        return;
    }

    uiTabLabels->pushButtonAdd->setText(tr("Update"));
    uiTabLabels->pushButtonCancel->show();
    uiTabLabels->pushButtonRemove->show();

    MaterialCutPlacement mcp = m_conMCP.at(iR);
    if (mcp.m_eMaterial == MaterialType::mtUserDefined)
    {
        int iRow = qApp->Settings()->GetUserDefinedMaterials().indexOf(mcp.m_qsMaterialUserDef);
        if (iRow >= 0)
        {
            uiTabLabels->comboBoxMaterial->setCurrentIndex(iRow + m_qslMaterials.count());
        }
        else
        {
            uiTabLabels->comboBoxMaterial->setCurrentText(mcp.m_qsMaterialUserDef);
        }
    }
    else
    {
        uiTabLabels->comboBoxMaterial->setCurrentIndex(int(mcp.m_eMaterial));
    }
    uiTabLabels->spinBoxCutNumber->setValue(mcp.m_iCutNumber);
    uiTabLabels->comboBoxPlacement->setCurrentIndex(int(mcp.m_ePlacement));

    m_bAddMode = false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnabledGrainline()
{
    if (uiTabGrainline->groupBoxGrainline->isChecked() == true)
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
    if (uiTabLabels->groupBoxDetailLabel->isChecked() == true)
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
    if (uiTabLabels->groupBoxPatternLabel->isChecked() == true)
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

    if (sender() == uiTabGrainline->pushButtonLen)
    {
        pleFormula = uiTabGrainline->lineEditLenFormula;
        bCheckZero = true;
        title = tr("Edit length");
    }
    else if (sender() == uiTabGrainline->pushButtonRot)
    {
        pleFormula = uiTabGrainline->lineEditRotFormula;
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

        if (sender() == uiTabGrainline->pushButtonLen)
        {
            SetGrainlineLength(qsFormula);
        }
        else if (sender() == uiTabGrainline->pushButtonRot)
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

    if (sender() == uiTabLabels->pushButtonDLHeight)
    {
        pleFormula = uiTabLabels->lineEditDLHeightFormula;
        bCheckZero = true;
        title = tr("Edit height");
    }
    else if (sender() == uiTabLabels->pushButtonDLWidth)
    {
        pleFormula = uiTabLabels->lineEditDLWidthFormula;
        bCheckZero = true;
        title = tr("Edit width");
    }
    else if (sender() == uiTabLabels->pushButtonDLAngle)
    {
        pleFormula = uiTabLabels->lineEditDLAngleFormula;
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
        if (sender() == uiTabLabels->pushButtonDLHeight)
        {
            SetDLHeight(qsFormula);
        }
        else if (sender() == uiTabLabels->pushButtonDLWidth)
        {
            SetDLWidth(qsFormula);
        }
        else if (sender() == uiTabLabels->pushButtonDLAngle)
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

    if (sender() == uiTabLabels->pushButtonPLHeight)
    {
        pleFormula = uiTabLabels->lineEditPLHeightFormula;
        bCheckZero = true;
        title = tr("Edit height");
    }
    else if (sender() == uiTabLabels->pushButtonPLWidth)
    {
        pleFormula = uiTabLabels->lineEditPLWidthFormula;
        bCheckZero = true;
        title = tr("Edit width");
    }
    else if (sender() == uiTabLabels->pushButtonPLAngle)
    {
        pleFormula = uiTabLabels->lineEditPLAngleFormula;
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
        if (sender() == uiTabLabels->pushButtonPLHeight)
        {
            SetPLHeight(qsFormula);
        }
        else if (sender() == uiTabLabels->pushButtonPLWidth)
        {
            SetPLWidth(qsFormula);
        }
        else if (sender() == uiTabLabels->pushButtonPLAngle)
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
    DeployFormula(uiTabGrainline->lineEditRotFormula, uiTabGrainline->pushButtonShowRot, m_iRotBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployGrainlineLength()
{
    DeployFormula(uiTabGrainline->lineEditLenFormula, uiTabGrainline->pushButtonShowLen, m_iLenBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLWidth()
{
    DeployFormula(uiTabLabels->lineEditDLWidthFormula, uiTabLabels->pushButtonShowDLWidth, m_DLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLHeight()
{
    DeployFormula(uiTabLabels->lineEditDLHeightFormula, uiTabLabels->pushButtonShowDLHeight, m_DLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLAngle()
{
    DeployFormula(uiTabLabels->lineEditDLAngleFormula, uiTabLabels->pushButtonShowDLAngle, m_DLAngleBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLWidth()
{
    DeployFormula(uiTabLabels->lineEditPLWidthFormula, uiTabLabels->pushButtonShowPLWidth, m_PLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLHeight()
{
    DeployFormula(uiTabLabels->lineEditPLHeightFormula, uiTabLabels->pushButtonShowPLHeight, m_PLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLAngle()
{
    DeployFormula(uiTabLabels->lineEditPLAngleFormula, uiTabLabels->pushButtonShowPLAngle, m_PLAngleBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ResetGrainlineWarning()
{
    if (flagGFormulas || flagGPin)
    {
        m_ftb->SetTabText(TabOrder::Grainline, tr("Grainline"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ResetLabelsWarning()
{
    if (flagDLAngle && (flagDLFormulas || flagDPin) && flagPLAngle && (flagPLFormulas || flagPPin))
    {
        m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), QIcon());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidth()
{
    labelEditFormula = uiTabPaths->labelEditWidth;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = uiTabPaths->plainTextEditFormulaWidth->toPlainText();
    m_saWidth = Eval(formula, flagFormula, uiTabPaths->labelResultWidth, postfix, false, true);

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
    labelEditFormula = uiTabPaths->labelEditBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = uiTabPaths->plainTextEditFormulaWidthBefore->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, uiTabPaths->labelResultBefore, postfix, false, true);

    const QString formulaSABefore = GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthBefore);
    UpdateNodeSABefore(formulaSABefore);
    EnableDefButton(uiTabPaths->pushButtonDefBefore, formulaSABefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthAfter()
{
    labelEditFormula = uiTabPaths->labelEditAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = uiTabPaths->plainTextEditFormulaWidthAfter->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, uiTabPaths->labelResultAfter, postfix, false, true);

    const QString formulaSAAfter = GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthAfter);
    UpdateNodeSAAfter(formulaSAAfter);
    EnableDefButton(uiTabPaths->pushButtonDefAfter, formulaSAAfter);
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
    dialog->SetFormula(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthBefore));
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
    dialog->SetFormula(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthAfter));
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
    labelEditFormula = uiTabPaths->labelEditWidth;
    labelResultCalculation = uiTabPaths->labelResultWidth;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagFormula, uiTabPaths->plainTextEditFormulaWidth, m_timerWidth, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::WidthBeforeChanged()
{
    labelEditFormula = uiTabPaths->labelEditBefore;
    labelResultCalculation = uiTabPaths->labelResultBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, uiTabPaths->plainTextEditFormulaWidthBefore, m_timerWidthBefore, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::WidthAfterChanged()
{
    labelEditFormula = uiTabPaths->labelEditAfter;
    labelResultCalculation = uiTabPaths->labelResultAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, uiTabPaths->plainTextEditFormulaWidthAfter, m_timerWidthAfter, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthFormulaTextEdit()
{
    DeployFormula(uiTabPaths->plainTextEditFormulaWidth, uiTabPaths->pushButtonGrowWidth, m_formulaBaseWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthBeforeFormulaTextEdit()
{
    DeployFormula(uiTabPaths->plainTextEditFormulaWidthBefore, uiTabPaths->pushButtonGrowWidthBefore,
                  m_formulaBaseWidthBefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthAfterFormulaTextEdit()
{
    DeployFormula(uiTabPaths->plainTextEditFormulaWidthAfter, uiTabPaths->pushButtonGrowWidthAfter,
                  m_formulaBaseWidthAfter);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::GrainlinePinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(uiTabGrainline->comboBoxGrainlineTopPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabGrainline->comboBoxGrainlineBottomPin);
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
            m_ftb->SetTabText(TabOrder::Grainline, tr("Grainline"));
        }
    }
    UpdateGrainlineValues();
    ChangeColor(uiTabGrainline->labelGrainlineTopPin, color);
    ChangeColor(uiTabGrainline->labelGrainlineBottomPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DetailPinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(uiTabLabels->comboBoxDLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabLabels->comboBoxDLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagDPin = true;
        color = okColor;

        if (flagPPin)
        {
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), QIcon());
        }
    }
    else
    {
        flagDPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = okColor : color = errorColor;

        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + QLatin1String("*"));
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
    }
    UpdateDetailLabelValues();
    ChangeColor(uiTabLabels->labelDLTopLeftPin, color);
    ChangeColor(uiTabLabels->labelDLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PatternPinPointChanged()
{
    QColor color = okColor;
    const quint32 topPinId = getCurrentObjectId(uiTabLabels->comboBoxPLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabLabels->comboBoxPLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagPPin = true;
        color = okColor;

        if (flagDPin)
        {
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), QIcon());
        }
    }
    else
    {
        flagPPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = okColor : color = errorColor;

        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + QLatin1String("*"));
        QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
    }
    UpdatePatternLabelValues();
    ChangeColor(uiTabLabels->labelPLTopLeftPin, color);
    ChangeColor(uiTabLabels->labelPLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::CreatePiece() const
{
    VPiece piece;
    piece.GetPath().SetNodes(GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath));
    piece.SetCustomSARecords(GetListInternals<CustomSARecord>(uiTabPaths->listWidgetCustomSA));
    piece.SetInternalPaths(GetListInternals<quint32>(uiTabPaths->listWidgetInternalPaths));
    piece.SetPins(GetListInternals<quint32>(uiTabPins->listWidgetPins));
    piece.SetForbidFlipping(uiTabPaths->checkBoxForbidFlipping->isChecked());
    piece.SetSeamAllowance(uiTabPaths->checkBoxSeams->isChecked());
    piece.SetSeamAllowanceBuiltIn(uiTabPaths->checkBoxBuiltIn->isChecked());
    piece.SetHideMainPath(uiTabPaths->checkBoxHideMainPath->isChecked());
    piece.SetName(uiTabLabels->lineEditName->text());
    piece.SetMx(m_mx);
    piece.SetMy(m_my);
    piece.SetFormulaSAWidth(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidth), m_saWidth);
    piece.GetPatternPieceData().SetLetter(uiTabLabels->lineEditLetter->text());

    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        piece.GetPatternPieceData().Append(m_conMCP[i]);
    }

    piece.GetPatternPieceData().SetPos(m_oldData.GetPos());
    piece.GetPatternPieceData().SetLabelWidth(GetFormulaFromUser(uiTabLabels->lineEditDLWidthFormula));
    piece.GetPatternPieceData().SetLabelHeight(GetFormulaFromUser(uiTabLabels->lineEditDLHeightFormula));
    piece.GetPatternPieceData().SetFontSize(m_oldData.GetFontSize());
    piece.GetPatternPieceData().SetRotation(GetFormulaFromUser(uiTabLabels->lineEditDLAngleFormula));
    piece.GetPatternPieceData().SetVisible(uiTabLabels->groupBoxDetailLabel->isChecked());
    piece.GetPatternPieceData().SetCenterPin(getCurrentObjectId(uiTabLabels->comboBoxDLCenterPin));
    piece.GetPatternPieceData().SetTopLeftPin(getCurrentObjectId(uiTabLabels->comboBoxDLTopLeftPin));
    piece.GetPatternPieceData().SetBottomRightPin(getCurrentObjectId(uiTabLabels->comboBoxDLBottomRightPin));

    piece.GetPatternInfo() = m_oldGeom;
    piece.GetPatternInfo().SetVisible(uiTabLabels->groupBoxPatternLabel->isChecked());
    piece.GetPatternInfo().SetCenterPin(getCurrentObjectId(uiTabLabels->comboBoxPLCenterPin));
    piece.GetPatternInfo().SetTopLeftPin(getCurrentObjectId(uiTabLabels->comboBoxPLTopLeftPin));
    piece.GetPatternInfo().SetBottomRightPin(getCurrentObjectId(uiTabLabels->comboBoxPLBottomRightPin));
    piece.GetPatternInfo().SetLabelWidth(GetFormulaFromUser(uiTabLabels->lineEditPLWidthFormula));
    piece.GetPatternInfo().SetLabelHeight(GetFormulaFromUser(uiTabLabels->lineEditPLHeightFormula));
    piece.GetPatternInfo().SetRotation(GetFormulaFromUser(uiTabLabels->lineEditPLAngleFormula));

    piece.GetGrainlineGeometry() = m_oldGrainline;
    piece.GetGrainlineGeometry().SetVisible(uiTabGrainline->groupBoxGrainline->isChecked());
    piece.GetGrainlineGeometry().SetRotation(GetFormulaFromUser(uiTabGrainline->lineEditRotFormula));
    piece.GetGrainlineGeometry().SetLength(GetFormulaFromUser(uiTabGrainline->lineEditLenFormula));
    piece.GetGrainlineGeometry().SetArrowType(static_cast<ArrowType>(uiTabGrainline->comboBoxArrow->currentIndex()));
    piece.GetGrainlineGeometry().SetCenterPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineCenterPin));
    piece.GetGrainlineGeometry().SetTopPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineTopPin));
    piece.GetGrainlineGeometry().SetBottomPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineBottomPin));

    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewMainPathItem(const VPieceNode &node)
{
    NewNodeItem(uiTabPaths->listWidgetMainPath, node);
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
        uiTabPaths->listWidgetCustomSA->addItem(item);
        uiTabPaths->listWidgetCustomSA->setCurrentRow(uiTabPaths->listWidgetCustomSA->count()-1);
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
        uiTabPaths->listWidgetInternalPaths->addItem(item);
        uiTabPaths->listWidgetInternalPaths->setCurrentRow(uiTabPaths->listWidgetInternalPaths->count()-1);
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
        uiTabPins->listWidgetPins->addItem(item);
        uiTabPins->listWidgetPins->setCurrentRow(uiTabPins->listWidgetPins->count()-1);
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
    bool valid = true;

    if(CreatePiece().MainPathPoints(data).count() < 3)
    {
        url += tr("You need more points!");
        uiTabPaths->helpLabel->setText(url);
        valid = false;
    }
    else
    {
        if(not MainPathIsClockwise())
        {
            url += tr("You have to choose points in a clockwise direction!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
        if (FirstPointEqualLast(uiTabPaths->listWidgetMainPath))
        {
            url += tr("First point cannot be equal to the last point!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
        else if (DoublePoints(uiTabPaths->listWidgetMainPath))
        {
            url += tr("You have double points!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
        else if (not EachPointLabelIsUnique(uiTabPaths->listWidgetMainPath))
        {
            url += tr("Each point in the path must be unique!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
    }

    if (valid)
    {
        m_ftb->SetTabText(TabOrder::Paths, tr("Paths"));
        QString tooltip = tr("Ready!");
        if (not applyAllowed)
        {
            tooltip = tooltip + QLatin1String("  <b>") +
                    tr("To open all detail's features complete creating the main path.") + QLatin1String("</b>");
        }
        uiTabPaths->helpLabel->setText(tooltip);
    }
    else
    {
        m_ftb->SetTabText(TabOrder::Paths, tr("Paths") + QLatin1String("*"));
    }

    return valid;
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
    const quint32 id = CURRENT_DATA(uiTabPaths->comboBoxNodes).toUInt();

    uiTabPaths->comboBoxNodes->blockSignals(true);
    uiTabPaths->comboBoxNodes->clear();

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath);

    for (int i = 0; i < nodes.size(); ++i)
    {
        const VPieceNode node = nodes.at(i);
        if (node.GetTypeTool() == Tool::NodePoint && not node.IsExcluded())
        {
            const QString name = GetNodeName(node);

            uiTabPaths->comboBoxNodes->addItem(name, node.GetId());
        }
    }
    uiTabPaths->comboBoxNodes->blockSignals(false);

    const int index = uiTabPaths->comboBoxNodes->findData(id);
    if (index != -1)
    {
        uiTabPaths->comboBoxNodes->setCurrentIndex(index);
        NodeChanged(index);// Need in case combox index was not changed
    }
    else
    {
        uiTabPaths->comboBoxNodes->count() > 0 ? NodeChanged(0) : NodeChanged(-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPassmarksList()
{
    const quint32 id = CURRENT_DATA(uiTabPassmarks->comboBoxPassmarks).toUInt();

    uiTabPassmarks->comboBoxPassmarks->blockSignals(true);
    uiTabPassmarks->comboBoxPassmarks->clear();

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath);

    for (int i = 0; i < nodes.size(); ++i)
    {
        const VPieceNode node = nodes.at(i);
        if (node.GetTypeTool() == Tool::NodePoint && node.IsPassmark())
        {
            const QString name = GetNodeName(node);

            uiTabPassmarks->comboBoxPassmarks->addItem(name, node.GetId());
        }
    }
    uiTabPassmarks->comboBoxPassmarks->blockSignals(false);

    const int index = uiTabPassmarks->comboBoxPassmarks->findData(id);
    if (index != -1)
    {
        uiTabPassmarks->comboBoxPassmarks->setCurrentIndex(index);
        PassmarkChanged(index);// Need in case combox index was not changed
    }
    else
    {
        uiTabPassmarks->comboBoxPassmarks->count() > 0 ? PassmarkChanged(0) : PassmarkChanged(-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QListWidgetItem *DialogSeamAllowance::GetItemById(quint32 id)
{
    for (qint32 i = 0; i < uiTabPaths->listWidgetMainPath->count(); ++i)
    {
        QListWidgetItem *item = uiTabPaths->listWidgetMainPath->item(i);
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
    const int count = uiTabPaths->listWidgetMainPath->count();
    if (count > 0)
    {
        QListWidgetItem *item = uiTabPaths->listWidgetMainPath->item(count-1);
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
    const int index = uiTabPaths->comboBoxNodes->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPaths->comboBoxNodes).toUInt());
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
    const int index = uiTabPaths->comboBoxNodes->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(CURRENT_DATA(uiTabPaths->comboBoxNodes).toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetFormulaSAAfter(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitFancyTabBar()
{
    m_ftb->InsertTab(TabOrder::Paths, QIcon("://icon/32x32/paths.png"), tr("Paths"));
    m_ftb->InsertTab(TabOrder::Labels, QIcon("://icon/32x32/labels.png"), tr("Labels"));
    m_ftb->InsertTab(TabOrder::Grainline, QIcon("://icon/32x32/grainline.png"), tr("Grainline"));
    m_ftb->InsertTab(TabOrder::Pins, QIcon("://icon/32x32/pins.png"), tr("Pins"));
    m_ftb->InsertTab(TabOrder::Passmarks, QIcon("://icon/32x32/passmark.png"), tr("Passmarks"));

    ui->horizontalLayout->addWidget(m_ftb, 0, Qt::AlignLeft);

    m_ftb->SetTabEnabled(TabOrder::Paths, true);

    m_tabPaths->hide();
    uiTabPaths->setupUi(m_tabPaths);
    ui->horizontalLayout->addWidget(m_tabPaths, 1);

    m_tabLabels->hide();
    uiTabLabels->setupUi(m_tabLabels);
    ui->horizontalLayout->addWidget(m_tabLabels, 1);

    m_tabGrainline->hide();
    uiTabGrainline->setupUi(m_tabGrainline);
    ui->horizontalLayout->addWidget(m_tabGrainline, 1);

    m_tabPins->hide();
    uiTabPins->setupUi(m_tabPins);
    ui->horizontalLayout->addWidget(m_tabPins, 1);

    m_tabPassmarks->hide();
    uiTabPassmarks->setupUi(m_tabPassmarks);
    ui->horizontalLayout->addWidget(m_tabPassmarks, 1);

    connect(m_ftb, &FancyTabBar::CurrentChanged, this, &DialogSeamAllowance::FancyTabChanged);
    connect(uiTabLabels->tabWidget, &QTabWidget::currentChanged, this, &DialogSeamAllowance::TabChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitMainPathTab()
{
    uiTabPaths->checkBoxForbidFlipping->setChecked(qApp->Settings()->GetForbidWorkpieceFlipping());

    uiTabPaths->listWidgetMainPath->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPaths->listWidgetMainPath, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowMainPathContextMenu);
    connect(uiTabPaths->listWidgetMainPath->model(), &QAbstractItemModel::rowsMoved, this,
            &DialogSeamAllowance::ListChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitSeamAllowanceTab()
{
    plainTextEditFormula = uiTabPaths->plainTextEditFormulaWidth;
    this->m_formulaBaseWidth = uiTabPaths->plainTextEditFormulaWidth->height();
    this->m_formulaBaseWidthBefore = uiTabPaths->plainTextEditFormulaWidthBefore->height();
    this->m_formulaBaseWidthAfter = uiTabPaths->plainTextEditFormulaWidthAfter->height();

    uiTabPaths->plainTextEditFormulaWidth->installEventFilter(this);
    uiTabPaths->plainTextEditFormulaWidthBefore->installEventFilter(this);
    uiTabPaths->plainTextEditFormulaWidthAfter->installEventFilter(this);

    m_timerWidth = new QTimer(this);
    connect(m_timerWidth, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidth);

    m_timerWidthBefore = new QTimer(this);
    connect(m_timerWidthBefore, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthBefore);

    m_timerWidthAfter = new QTimer(this);
    connect(m_timerWidthAfter, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthAfter);

    connect(uiTabPaths->checkBoxSeams, &QCheckBox::toggled, this, &DialogSeamAllowance::EnableSeamAllowance);

    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    m_saWidth = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
    uiTabPaths->plainTextEditFormulaWidth->setPlainText(qApp->LocaleToString(m_saWidth));

    InitNodesList();
    connect(uiTabPaths->comboBoxNodes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeChanged);

    connect(uiTabPaths->pushButtonDefBefore, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefBefore);
    connect(uiTabPaths->pushButtonDefAfter, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefAfter);

    InitNodeAngles(uiTabPaths->comboBoxAngle);
    connect(uiTabPaths->comboBoxAngle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeAngleChanged);

    uiTabPaths->listWidgetCustomSA->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPaths->listWidgetCustomSA, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowCustomSAContextMenu);
    connect(uiTabPaths->listWidgetCustomSA, &QListWidget::currentRowChanged, this,
            &DialogSeamAllowance::CustomSAChanged);
    connect(uiTabPaths->comboBoxStartPoint, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::CSAStartPointChanged);
    connect(uiTabPaths->comboBoxEndPoint, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::CSAEndPointChanged);
    connect(uiTabPaths->comboBoxIncludeType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::CSAIncludeTypeChanged);

    connect(uiTabPaths->toolButtonExprWidth, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidth);
    connect(uiTabPaths->toolButtonExprBefore, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthBefore);
    connect(uiTabPaths->toolButtonExprAfter, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthAfter);

    connect(uiTabPaths->plainTextEditFormulaWidth, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::WidthChanged);
    connect(uiTabPaths->plainTextEditFormulaWidthBefore, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::WidthBeforeChanged);
    connect(uiTabPaths->plainTextEditFormulaWidthAfter, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::WidthAfterChanged);

    connect(uiTabPaths->pushButtonGrowWidth, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployWidthFormulaTextEdit);
    connect(uiTabPaths->pushButtonGrowWidthBefore, &QPushButton::clicked,
            this, &DialogSeamAllowance::DeployWidthBeforeFormulaTextEdit);
    connect(uiTabPaths->pushButtonGrowWidthAfter, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployWidthAfterFormulaTextEdit);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitCSAPoint(QComboBox *box)
{
    SCASSERT(box != nullptr);
    box->clear();
    box->addItem(tr("Empty"), NULL_ID);

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath);

    for (int i = 0; i < nodes.size(); ++i)
    {
        const VPieceNode &node = nodes.at(i);
        if (node.GetTypeTool() == Tool::NodePoint && not node.IsExcluded())
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

    const QVector<quint32> pins = GetListInternals<quint32>(uiTabPins->listWidgetPins);

    for (int i = 0; i < pins.size(); ++i)
    {
        const QSharedPointer<VGObject> pin = data->GetGObject(pins.at(i));
        box->addItem(pin->name(), pins.at(i));
    }

    const int index = uiTabPaths->comboBoxNodes->findData(currentId);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitSAIncludeType()
{
    uiTabPaths->comboBoxIncludeType->clear();

    uiTabPaths->comboBoxIncludeType->addItem(tr("main path"),
                                             static_cast<unsigned char>(PiecePathIncludeType::AsMainPath));
    uiTabPaths->comboBoxIncludeType->addItem(tr("custom seam allowance"),
                                             static_cast<unsigned char>(PiecePathIncludeType::AsCustomSA));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitInternalPathsTab()
{
    uiTabPaths->listWidgetInternalPaths->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPaths->listWidgetInternalPaths, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowInternalPathsContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPatternPieceDataTab()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    uiTabLabels->lineEditName->setClearButtonEnabled(true);
    uiTabLabels->lineEditLetter->setClearButtonEnabled(true);
#endif

    connect(uiTabLabels->lineEditName, &QLineEdit::textChanged, this, &DialogSeamAllowance::NameDetailChanged);

    m_qslMaterials << QApplication::translate("Detail", "Fabric", 0)
                   << QApplication::translate("Detail", "Lining", 0)
                   << QApplication::translate("Detail", "Interfacing", 0)
                   << QApplication::translate("Detail", "Interlining", 0);

    for (int i = 0; i < m_qslMaterials.count(); ++i)
    {
        uiTabLabels->comboBoxMaterial->addItem(m_qslMaterials[i], i);
    }

    const QStringList qsl = qApp->Settings()->GetUserDefinedMaterials();
    for (int i = 0; i < qsl.count(); ++i)
    {
        uiTabLabels->comboBoxMaterial->addItem(qsl.at(i), int(MaterialType::mtUserDefined));
    }

    m_qslPlacements << tr("None") << tr("Cut on fold");
    uiTabLabels->comboBoxPlacement->addItems(m_qslPlacements);

    connect(uiTabLabels->pushButtonAdd, &QPushButton::clicked, this, &DialogSeamAllowance::AddUpdate);
    connect(uiTabLabels->pushButtonCancel, &QPushButton::clicked, this, &DialogSeamAllowance::Cancel);
    connect(uiTabLabels->pushButtonRemove, &QPushButton::clicked, this, &DialogSeamAllowance::Remove);
    connect(uiTabLabels->listWidgetMCP, &QListWidget::itemClicked, this, &DialogSeamAllowance::SetEditMode);
    connect(uiTabLabels->comboBoxMaterial, &QComboBox::currentTextChanged, this, &DialogSeamAllowance::MaterialChanged);

    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitLabelsTab()
{
    m_DLWidthBaseHeight = uiTabLabels->lineEditDLWidthFormula->height();
    m_DLHeightBaseHeight = uiTabLabels->lineEditDLHeightFormula->height();
    m_DLAngleBaseHeight = uiTabLabels->lineEditDLAngleFormula->height();

    connect(uiTabLabels->groupBoxDetailLabel, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledDetailLabel);
    InitPinPoint(uiTabLabels->comboBoxDLCenterPin);
    InitPinPoint(uiTabLabels->comboBoxDLTopLeftPin);
    InitPinPoint(uiTabLabels->comboBoxDLBottomRightPin);

    connect(uiTabLabels->comboBoxDLTopLeftPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::DetailPinPointChanged);
    connect(uiTabLabels->comboBoxDLBottomRightPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::DetailPinPointChanged);

    connect(uiTabLabels->pushButtonDLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);
    connect(uiTabLabels->pushButtonDLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);
    connect(uiTabLabels->pushButtonDLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::EditDLFormula);

    connect(uiTabLabels->lineEditDLWidthFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);
    connect(uiTabLabels->lineEditDLHeightFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);
    connect(uiTabLabels->lineEditDLAngleFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateDetailLabelValues);

    connect(uiTabLabels->pushButtonShowDLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLWidth);
    connect(uiTabLabels->pushButtonShowDLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLHeight);
    connect(uiTabLabels->pushButtonShowDLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::DeployDLAngle);

    EnabledDetailLabel();

    m_PLWidthBaseHeight = uiTabLabels->lineEditPLWidthFormula->height();
    m_PLHeightBaseHeight = uiTabLabels->lineEditPLHeightFormula->height();
    m_PLAngleBaseHeight = uiTabLabels->lineEditPLAngleFormula->height();

    connect(uiTabLabels->groupBoxPatternLabel, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledPatternLabel);
    InitPinPoint(uiTabLabels->comboBoxPLCenterPin);
    InitPinPoint(uiTabLabels->comboBoxPLTopLeftPin);
    InitPinPoint(uiTabLabels->comboBoxPLBottomRightPin);

    connect(uiTabLabels->comboBoxPLTopLeftPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PatternPinPointChanged);
    connect(uiTabLabels->comboBoxPLBottomRightPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PatternPinPointChanged);

    connect(uiTabLabels->pushButtonPLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);
    connect(uiTabLabels->pushButtonPLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);
    connect(uiTabLabels->pushButtonPLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::EditPLFormula);

    connect(uiTabLabels->lineEditPLWidthFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);
    connect(uiTabLabels->lineEditPLHeightFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);
    connect(uiTabLabels->lineEditPLAngleFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdatePatternLabelValues);

    connect(uiTabLabels->pushButtonShowPLWidth, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLWidth);
    connect(uiTabLabels->pushButtonShowPLHeight, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLHeight);
    connect(uiTabLabels->pushButtonShowPLAngle, &QPushButton::clicked, this, &DialogSeamAllowance::DeployPLAngle);

    EnabledPatternLabel();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitGrainlineTab()
{
    connect(uiTabGrainline->groupBoxGrainline, &QGroupBox::toggled, this, &DialogSeamAllowance::EnabledGrainline);
    connect(uiTabGrainline->pushButtonRot, &QPushButton::clicked, this, &DialogSeamAllowance::EditGrainlineFormula);
    connect(uiTabGrainline->pushButtonLen, &QPushButton::clicked, this, &DialogSeamAllowance::EditGrainlineFormula);
    connect(uiTabGrainline->lineEditLenFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateGrainlineValues);
    connect(uiTabGrainline->lineEditRotFormula, &QPlainTextEdit::textChanged, this,
            &DialogSeamAllowance::UpdateGrainlineValues);

    connect(uiTabGrainline->pushButtonShowRot, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployGrainlineRotation);
    connect(uiTabGrainline->pushButtonShowLen, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployGrainlineLength);

    EnabledGrainline();

    uiTabGrainline->comboBoxArrow->addItem(tr("Both"));
    uiTabGrainline->comboBoxArrow->addItem(tr("Just front"));
    uiTabGrainline->comboBoxArrow->addItem(tr("Just rear"));

    m_iRotBaseHeight = uiTabGrainline->lineEditRotFormula->height();
    m_iLenBaseHeight = uiTabGrainline->lineEditLenFormula->height();

    InitPinPoint(uiTabGrainline->comboBoxGrainlineCenterPin);
    InitPinPoint(uiTabGrainline->comboBoxGrainlineTopPin);
    InitPinPoint(uiTabGrainline->comboBoxGrainlineBottomPin);

    connect(uiTabGrainline->comboBoxGrainlineTopPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::GrainlinePinPointChanged);
    connect(uiTabGrainline->comboBoxGrainlineBottomPin,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::GrainlinePinPointChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPinsTab()
{
    uiTabPins->listWidgetPins->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPins->listWidgetPins, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowPinsContextMenu);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPassmarksTab()
{
    InitPassmarksList();
    connect(uiTabPassmarks->comboBoxPassmarks, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PassmarkChanged);

    connect(uiTabPassmarks->buttonGroupLineType, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &DialogSeamAllowance::PassmarkLineTypeChanged);
    connect(uiTabPassmarks->buttonGroupAngleType, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &DialogSeamAllowance::PassmarkAngleTypeChanged);
    connect(uiTabPassmarks->checkBoxShowSecondPassmark, &QCheckBox::stateChanged, this,
            &DialogSeamAllowance::PassmarkShowSecondChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitAllPinComboboxes()
{
    InitPinPoint(uiTabGrainline->comboBoxGrainlineCenterPin);
    InitPinPoint(uiTabGrainline->comboBoxGrainlineTopPin);
    InitPinPoint(uiTabGrainline->comboBoxGrainlineBottomPin);

    InitPinPoint(uiTabLabels->comboBoxDLCenterPin);
    InitPinPoint(uiTabLabels->comboBoxDLTopLeftPin);
    InitPinPoint(uiTabLabels->comboBoxDLBottomRightPin);

    InitPinPoint(uiTabLabels->comboBoxPLCenterPin);
    InitPinPoint(uiTabLabels->comboBoxPLTopLeftPin);
    InitPinPoint(uiTabLabels->comboBoxPLBottomRightPin);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetFormulaSAWidth() const
{
    QString width = uiTabPaths->plainTextEditFormulaWidth->toPlainText();
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
    uiTabPaths->plainTextEditFormulaWidth->setPlainText(width);

    VisToolPiece *path = qobject_cast<VisToolPiece *>(vis);
    SCASSERT(path != nullptr)
    const VPiece p = CreatePiece();
    path->SetPiece(p);

    MoveCursorToEnd(uiTabPaths->plainTextEditFormulaWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateCurrentCustomSARecord()
{
    const int row = uiTabPaths->listWidgetCustomSA->currentRow();
    if (uiTabPaths->listWidgetCustomSA->count() == 0 || row == -1)
    {
        return;
    }

    QListWidgetItem *item = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(item != nullptr);
    const CustomSARecord record = qvariant_cast<CustomSARecord>(item->data(Qt::UserRole));
    item->setText(GetPathName(record.path, record.reverse));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateCurrentInternalPathRecord()
{
    const int row = uiTabPaths->listWidgetInternalPaths->currentRow();
    if (uiTabPaths->listWidgetInternalPaths->count() == 0 || row == -1)
    {
        return;
    }

    QListWidgetItem *item = uiTabPaths->listWidgetInternalPaths->item(row);
    SCASSERT(item != nullptr);
    const quint32 path = qvariant_cast<quint32>(item->data(Qt::UserRole));
    item->setText(GetPathName(path));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ClearFields()
{
    uiTabLabels->comboBoxMaterial->setCurrentIndex(0);
    uiTabLabels->spinBoxCutNumber->setValue(0);
    uiTabLabels->comboBoxPlacement->setCurrentIndex(0);
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
    uiTabGrainline->lineEditRotFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabGrainline->lineEditRotFormula);
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

    uiTabGrainline->lineEditLenFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabGrainline->lineEditLenFormula);
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

    uiTabLabels->lineEditDLWidthFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditDLWidthFormula);
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

    uiTabLabels->lineEditDLHeightFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditDLHeightFormula);
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

    uiTabLabels->lineEditDLAngleFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditDLAngleFormula);
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

    uiTabLabels->lineEditPLWidthFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditPLWidthFormula);
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

    uiTabLabels->lineEditPLHeightFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditPLHeightFormula);
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

    uiTabLabels->lineEditPLAngleFormula->setPlainText(formula);

    MoveCursorToEnd(uiTabLabels->lineEditPLAngleFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowPins()
{
    if (m_visPins.isNull())
    {
        m_visPins = new VisPiecePins(data);
    }

    m_visPins->SetPins(GetListInternals<quint32>(uiTabPins->listWidgetPins));

    if (not qApp->getCurrentScene()->items().contains(m_visPins))
    {
        m_visPins->VisualMode(NULL_ID);
        m_visPins->setZValue(10); // pins should be on top
        VToolSeamAllowance *tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(toolId));
        SCASSERT(tool != nullptr);
        m_visPins->setParentItem(tool);
    }
    else
    {
        m_visPins->RefreshGeometry();
    }
}
