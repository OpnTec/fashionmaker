/************************************************************************
 **
 **  @file   dialogpiecepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../../../tools/vabstracttool.h"
#include "../../../tools/vtoolseamallowance.h"
#include "../../support/dialogeditwrongformula.h"

#include <QMenu>
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogPiecePath::DialogPiecePath(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogPiecePath),
      m_showMode(false),
      m_saWidth(0),
      m_timerWidth(new QTimer(this)),
      m_timerWidthBefore(new QTimer(this)),
      m_timerWidthAfter(new QTimer(this)),
      m_timerVisible(new QTimer(this)),
      m_formulaBaseWidth(0),
      m_formulaBaseWidthBefore(0),
      m_formulaBaseWidthAfter(0),
      m_formulaBaseVisible(0),
      m_flagFormulaBefore(true),
      m_flagFormulaAfter(true),
      m_flagFormulaVisible(true)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    InitPathTab();
    InitSeamAllowanceTab();
    InitPassmarksTab();
    InitControlTab();

    EvalVisible();

    flagName = true;//We have default name of piece.
    flagError = PathIsValid();
    CheckState();

    vis = new VisToolPiecePath(data);

    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabSeamAllowance));
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabPassmarks));

    connect(ui->comboBoxPiece, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        ValidObjects(PathIsValid());
    });
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
    ValidObjects(PathIsValid());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::HideVisibilityTrigger()
{
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabControl));
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
        if (id != GetLastId())
        {
            switch (type)
            {
                case SceneObject::Arc:
                    NewItem(VPieceNode(id, Tool::NodeArc, reverse));
                    break;
                case SceneObject::ElArc:
                    NewItem(VPieceNode(id, Tool::NodeElArc, reverse));
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
        }
        else
        {
            if (ui->listWidget->count() > 1)
            {
                delete GetItemById(id);
            }
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
void DialogPiecePath::ShowDialog(bool click)
{
    if (click == false)
    {
        if (CreatePath().CountNodes() > 0)
        {
            emit ToolTip(QString());
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
void DialogPiecePath::CheckState()
{
    SCASSERT(bOk != nullptr);
    if (GetType() != PiecePathType::InternalPath)
    {// Works only for internal paths
        m_flagFormulaVisible = true; 
    }
    else
    {
        flagFormula = true;
        m_flagFormulaBefore = true;
        m_flagFormulaAfter = true;
    }

    bOk->setEnabled(flagName && flagError && flagFormula && m_flagFormulaBefore && m_flagFormulaAfter
                    && m_flagFormulaVisible);

    const int tabSeamAllowanceIndex = ui->tabWidget->indexOf(ui->tabSeamAllowance);
    if (flagFormula && m_flagFormulaBefore && m_flagFormulaAfter)
    {
        ui->tabWidget->setTabIcon(tabSeamAllowanceIndex, QIcon());
    }
    else
    {
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        ui->tabWidget->setTabIcon(tabSeamAllowanceIndex, icon);
    }

    const int tabControlIndex = ui->tabWidget->indexOf(ui->tabControl);
    if (m_flagFormulaVisible)
    {
        ui->tabWidget->setTabIcon(tabControlIndex, QIcon());
    }
    else
    {
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        ui->tabWidget->setTabIcon(tabControlIndex, icon);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ShowVisualization()
{
    AddVisualization<VisToolPiecePath>();

    if (m_showMode)
    {
        VToolSeamAllowance *tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(GetPieceId()));
        SCASSERT(tool != nullptr);
        auto visPath = qobject_cast<VisToolPiecePath *>(vis);
        SCASSERT(visPath != nullptr);
        visPath->setParentItem(tool);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormulaWidth->blockSignals(true);
    ui->plainTextEditFormulaWidthBefore->blockSignals(true);
    ui->plainTextEditFormulaWidthAfter->blockSignals(true);
    ui->plainTextEditFormulaVisible->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ShowContextMenu(const QPoint &pos)
{
    const int row = ui->listWidget->currentRow();
    if (ui->listWidget->count() == 0 || row == -1 || row >= ui->listWidget->count())
    {
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu());

    QListWidgetItem *rowItem = ui->listWidget->item(row);
    SCASSERT(rowItem != nullptr);
    VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

    QAction *actionPassmark = nullptr;
    QAction *actionUniqueness = nullptr;
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

        actionUniqueness = menu->addAction(tr("Check uniqueness"));
        actionUniqueness->setCheckable(true);
        actionUniqueness->setChecked(rowNode.IsCheckUniqueness());
    }

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidget->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidget->item(row);
    }
    else if (rowNode.GetTypeTool() != Tool::NodePoint && selectedAction == actionReverse)
    {
        rowNode.SetReverse(not rowNode.GetReverse());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
    }
    else if (selectedAction == actionPassmark)
    {
        rowNode.SetPassmark(not rowNode.IsPassmark());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
    }
    else if (selectedAction == actionUniqueness)
    {
        rowNode.SetCheckUniqueness(not rowNode.IsCheckUniqueness());
        rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        rowItem->setText(GetNodeName(rowNode, true));
    }

    ValidObjects(PathIsValid());
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

    InitPassmarksList();
    InitNodesList();
    SetMoveControls();
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
        const VPiecePath path = CreatePath();
        const int nodeIndex = path.indexOfNode(ui->comboBoxNodes->currentData().toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = path.at(nodeIndex);

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
        ui->plainTextEditFormulaWidthBefore->setPlainText(QString());
        ui->plainTextEditFormulaWidthAfter->setPlainText(QString());
        ui->comboBoxAngle->setCurrentIndex(-1);
    }

    ui->comboBoxAngle->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::PassmarkChanged(int index)
{
    ui->radioButtonOneLine->setDisabled(true);
    ui->radioButtonTwoLines->setDisabled(true);
    ui->radioButtonThreeLines->setDisabled(true);
    ui->radioButtonTMark->setDisabled(true);
    ui->radioButtonVMark->setDisabled(true);

    ui->radioButtonStraightforward->setDisabled(true);
    ui->radioButtonBisector->setDisabled(true);
    ui->radioButtonIntersection->setDisabled(true);
    ui->radioButtonIntersectionOnlyLeft->setDisabled(true);
    ui->radioButtonIntersectionOnlyRight->setDisabled(true);
    ui->radioButtonIntersection2->setDisabled(true);
    ui->radioButtonIntersection2OnlyLeft->setDisabled(true);
    ui->radioButtonIntersection2OnlyRight->setDisabled(true);

    ui->groupBoxMarkType->blockSignals(true);
    ui->groupBoxAngleType->blockSignals(true);

    if (index != -1)
    {
        const VPiecePath path = CreatePath();
        const int nodeIndex = path.indexOfNode(ui->comboBoxPassmarks->currentData().toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = path.at(nodeIndex);

            // Line type
            ui->radioButtonOneLine->setEnabled(true);
            ui->radioButtonTwoLines->setEnabled(true);
            ui->radioButtonThreeLines->setEnabled(true);
            ui->radioButtonTMark->setEnabled(true);
            ui->radioButtonVMark->setEnabled(true);

            switch(node.GetPassmarkLineType())
            {
                case PassmarkLineType::OneLine:
                    ui->radioButtonOneLine->setChecked(true);
                    break;
                case PassmarkLineType::TwoLines:
                    ui->radioButtonTwoLines->setChecked(true);
                    break;
                case PassmarkLineType::ThreeLines:
                    ui->radioButtonThreeLines->setChecked(true);
                    break;
                case PassmarkLineType::TMark:
                    ui->radioButtonTMark->setChecked(true);
                    break;
                case PassmarkLineType::VMark:
                    ui->radioButtonVMark->setChecked(true);
                    break;
                default:
                    break;
            }

            // Angle type
            ui->radioButtonStraightforward->setEnabled(true);
            ui->radioButtonBisector->setEnabled(true);
            ui->radioButtonIntersection->setEnabled(true);
            ui->radioButtonIntersectionOnlyLeft->setEnabled(true);
            ui->radioButtonIntersectionOnlyRight->setEnabled(true);
            ui->radioButtonIntersection2->setEnabled(true);
            ui->radioButtonIntersection2OnlyLeft->setEnabled(true);
            ui->radioButtonIntersection2OnlyRight->setEnabled(true);

            switch(node.GetPassmarkAngleType())
            {
                case PassmarkAngleType::Straightforward:
                    ui->radioButtonStraightforward->setChecked(true);
                    break;
                case PassmarkAngleType::Bisector:
                    ui->radioButtonBisector->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection:
                    ui->radioButtonIntersection->setChecked(true);
                    break;
                case PassmarkAngleType::IntersectionOnlyLeft:
                    ui->radioButtonIntersectionOnlyLeft->setChecked(true);
                    break;
                case PassmarkAngleType::IntersectionOnlyRight:
                    ui->radioButtonIntersectionOnlyRight->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2:
                    ui->radioButtonIntersection2->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2OnlyLeft:
                    ui->radioButtonIntersection2OnlyLeft->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2OnlyRight:
                    ui->radioButtonIntersection2OnlyRight->setChecked(true);
                    break;
                default:
                    break;
            }
        }
    }
    ui->groupBoxMarkType->blockSignals(false);
    ui->groupBoxAngleType->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefBefore()
{
    ui->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefAfter()
{
    ui->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::PassmarkLineTypeChanged(int id)
{
    const int i = ui->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(ui->comboBoxPassmarks->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

            PassmarkLineType lineType = PassmarkLineType::OneLine;
            if (id == ui->buttonGroupMarkType->id(ui->radioButtonOneLine))
            {
                lineType = PassmarkLineType::OneLine;
            }
            else if (id == ui->buttonGroupMarkType->id(ui->radioButtonTwoLines))
            {
                lineType = PassmarkLineType::TwoLines;
            }
            else if (id == ui->buttonGroupMarkType->id(ui->radioButtonThreeLines))
            {
                lineType = PassmarkLineType::ThreeLines;
            }
            else if (id == ui->buttonGroupMarkType->id(ui->radioButtonTMark))
            {
                lineType = PassmarkLineType::TMark;
            }
            else if (id == ui->buttonGroupMarkType->id(ui->radioButtonVMark))
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
void DialogPiecePath::PassmarkAngleTypeChanged(int id)
{
    const int i = ui->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(ui->comboBoxPassmarks->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));

            PassmarkAngleType angleType = PassmarkAngleType::Straightforward;
            if (id == ui->buttonGroupAngleType->id(ui->radioButtonStraightforward))
            {
                angleType = PassmarkAngleType::Straightforward;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonBisector))
            {
                angleType = PassmarkAngleType::Bisector;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersection))
            {
                angleType = PassmarkAngleType::Intersection;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersectionOnlyLeft))
            {
                angleType = PassmarkAngleType::IntersectionOnlyLeft;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersectionOnlyRight))
            {
                angleType = PassmarkAngleType::IntersectionOnlyRight;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersection2))
            {
                angleType = PassmarkAngleType::Intersection2;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersection2OnlyLeft))
            {
                angleType = PassmarkAngleType::Intersection2OnlyLeft;
            }
            else if (id == ui->buttonGroupAngleType->id(ui->radioButtonIntersection2OnlyRight))
            {
                angleType = PassmarkAngleType::Intersection2OnlyRight;
            }

            rowNode.SetPassmarkAngleType(angleType);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
            rowItem->setText(GetNodeName(rowNode, true));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidth()
{
    labelEditFormula = ui->labelEditWidth;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidth->toPlainText();
    m_saWidth = Eval(formula, flagFormula, ui->labelResultWidth, postfix, false, true);

    if (m_saWidth >= 0)
    {
        VContainer *locData = const_cast<VContainer *> (data);
        locData->AddVariable(currentSeamAllowance, new VIncrement(locData, currentSeamAllowance, 0, m_saWidth,
                                                                  QString().setNum(m_saWidth), true,
                                                                  tr("Current seam aloowance")));

        EvalWidthBefore();
        EvalWidthAfter();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidthBefore()
{
    labelEditFormula = ui->labelEditBefore;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    QString formula = ui->plainTextEditFormulaWidthBefore->toPlainText();
    Eval(formula, m_flagFormulaBefore, ui->labelResultBefore, postfix, false, true);

    formula = GetFormulaSAWidthBefore();
    if (formula != currentSeamAllowance)
    {
        ui->pushButtonDefBefore->setEnabled(true);
    }

    UpdateNodeSABefore(formula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidthAfter()
{
    labelEditFormula = ui->labelEditAfter;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    QString formula = ui->plainTextEditFormulaWidthAfter->toPlainText();
    Eval(formula, m_flagFormulaAfter, ui->labelResultAfter, postfix, false, true);

    formula = GetFormulaSAWidthAfter();
    if (formula != currentSeamAllowance)
    {
        ui->pushButtonDefAfter->setEnabled(true);
    }

    UpdateNodeSAAfter(formula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalVisible()
{
    labelEditFormula = ui->labelEditVisible;
    QString formula = ui->plainTextEditFormulaVisible->toPlainText();
    Eval(formula, m_flagFormulaVisible, ui->labelResultVisible, QString(), false, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::FXWidth()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit seam allowance width"));
    dialog->SetFormula(GetFormulaSAWidth());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormulaSAWidth(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::FXWidthBefore()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit seam allowance width before"));
    dialog->SetFormula(GetFormulaSAWidthBefore());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSABefore(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::FXWidthAfter()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit seam allowance width after"));
    dialog->SetFormula(GetFormulaSAWidthAfter());
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSAAfter(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::FXVisible()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Control visibility"));
    dialog->SetFormula(GetFormulaVisible());
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormulaVisible(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::WidthChanged()
{
    labelEditFormula = ui->labelEditWidth;
    labelResultCalculation = ui->labelResultWidth;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidth, m_timerWidth, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::WidthBeforeChanged()
{
    labelEditFormula = ui->labelEditBefore;
    labelResultCalculation = ui->labelResultBefore;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(m_flagFormulaBefore, ui->plainTextEditFormulaWidthBefore, m_timerWidthBefore, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::WidthAfterChanged()
{
    labelEditFormula = ui->labelEditAfter;
    labelResultCalculation = ui->labelResultAfter;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(m_flagFormulaAfter, ui->plainTextEditFormulaWidthAfter, m_timerWidthAfter, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::VisibleChanged()
{
    labelEditFormula = ui->labelEditVisible;
    labelResultCalculation = ui->labelResultVisible;
    ValFormulaChanged(m_flagFormulaVisible, ui->plainTextEditFormulaVisible, m_timerVisible, QString());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::DeployWidthFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidth, ui->pushButtonGrowWidth, m_formulaBaseWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::DeployWidthBeforeFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidthBefore, ui->pushButtonGrowWidthBefore, m_formulaBaseWidthBefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::DeployWidthAfterFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidthAfter, ui->pushButtonGrowWidthAfter, m_formulaBaseWidthAfter);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::DeployVisibleFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormulaVisible, ui->pushButtonGrowVisible, m_formulaBaseVisible);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetMoveControls()
{
    ui->toolButtonTop->setEnabled(false);
    ui->toolButtonUp->setEnabled(false);
    ui->toolButtonDown->setEnabled(false);
    ui->toolButtonBottom->setEnabled(false);

    if (ui->listWidget->count() >= 2)
    {
        if (ui->listWidget->currentRow() == 0)
        {
            ui->toolButtonDown->setEnabled(true);
            ui->toolButtonBottom->setEnabled(true);
        }
        else if (ui->listWidget->currentRow() == ui->listWidget->count()-1)
        {
            ui->toolButtonTop->setEnabled(true);
            ui->toolButtonUp->setEnabled(true);
        }
        else
        {
            ui->toolButtonTop->setEnabled(true);
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(true);
            ui->toolButtonBottom->setEnabled(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitPathTab()
{
    ui->lineEditName->setClearButtonEnabled(true);

    FillComboBoxTypeLine(ui->comboBoxPenType, CurvePenStylesPics());

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogPiecePath::NameChanged);

    InitPathTypes();
    connect(ui->comboBoxType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        ui->comboBoxPenType->setEnabled(GetType() == PiecePathType::InternalPath);
        ui->checkBoxCut->setEnabled(GetType() == PiecePathType::InternalPath);
        ui->tabControl->setEnabled(GetType() == PiecePathType::InternalPath);
        ValidObjects(PathIsValid());
    });

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &DialogPiecePath::ShowContextMenu);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &DialogPiecePath::ListChanged);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &DialogPiecePath::SetMoveControls);

    connect(ui->toolButtonTop, &QToolButton::clicked, this, [this](){MoveListRowTop(ui->listWidget);});
    connect(ui->toolButtonUp, &QToolButton::clicked, this, [this](){MoveListRowUp(ui->listWidget);});
    connect(ui->toolButtonDown, &QToolButton::clicked, this, [this](){MoveListRowDown(ui->listWidget);});
    connect(ui->toolButtonBottom, &QToolButton::clicked, this, [this](){MoveListRowBottom(ui->listWidget);});
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitSeamAllowanceTab()
{
    plainTextEditFormula = ui->plainTextEditFormulaWidth;
    this->m_formulaBaseWidth = ui->plainTextEditFormulaWidth->height();
    this->m_formulaBaseWidthBefore = ui->plainTextEditFormulaWidthBefore->height();
    this->m_formulaBaseWidthAfter = ui->plainTextEditFormulaWidthAfter->height();

    ui->plainTextEditFormulaWidth->installEventFilter(this);
    ui->plainTextEditFormulaWidthBefore->installEventFilter(this);
    ui->plainTextEditFormulaWidthAfter->installEventFilter(this);

    connect(m_timerWidth, &QTimer::timeout, this, &DialogPiecePath::EvalWidth);
    connect(m_timerWidthBefore, &QTimer::timeout, this, &DialogPiecePath::EvalWidthBefore);
    connect(m_timerWidthAfter, &QTimer::timeout, this, &DialogPiecePath::EvalWidthAfter);

    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    m_saWidth = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
    ui->plainTextEditFormulaWidth->setPlainText(qApp->LocaleToString(m_saWidth));

    InitNodesList();
    connect(ui->comboBoxNodes, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogPiecePath::NodeChanged);

    connect(ui->pushButtonDefBefore, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefBefore);
    connect(ui->pushButtonDefAfter, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefAfter);

    InitNodeAngles(ui->comboBoxAngle);
    connect(ui->comboBoxAngle, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogPiecePath::NodeAngleChanged);

    connect(ui->toolButtonExprWidth, &QPushButton::clicked, this, &DialogPiecePath::FXWidth);
    connect(ui->toolButtonExprBefore, &QPushButton::clicked, this, &DialogPiecePath::FXWidthBefore);
    connect(ui->toolButtonExprAfter, &QPushButton::clicked, this, &DialogPiecePath::FXWidthAfter);

    connect(ui->plainTextEditFormulaWidth, &QPlainTextEdit::textChanged, this, &DialogPiecePath::WidthChanged);
    connect(ui->plainTextEditFormulaWidthBefore, &QPlainTextEdit::textChanged, this,
            &DialogPiecePath::WidthBeforeChanged);
    connect(ui->plainTextEditFormulaWidthAfter, &QPlainTextEdit::textChanged, this,
            &DialogPiecePath::WidthAfterChanged);

    connect(ui->pushButtonGrowWidth, &QPushButton::clicked, this, &DialogPiecePath::DeployWidthFormulaTextEdit);
    connect(ui->pushButtonGrowWidthBefore, &QPushButton::clicked,
            this, &DialogPiecePath::DeployWidthBeforeFormulaTextEdit);
    connect(ui->pushButtonGrowWidthAfter, &QPushButton::clicked, this,
            &DialogPiecePath::DeployWidthAfterFormulaTextEdit);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitPassmarksTab()
{
    InitPassmarksList();
    connect(ui->comboBoxPassmarks, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogPiecePath::PassmarkChanged);

    connect(ui->buttonGroupMarkType, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &DialogPiecePath::PassmarkLineTypeChanged);
    connect(ui->buttonGroupAngleType, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &DialogPiecePath::PassmarkAngleTypeChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitControlTab()
{
    this->m_formulaBaseVisible = ui->plainTextEditFormulaVisible->height();

    ui->plainTextEditFormulaVisible->installEventFilter(this);

    connect(m_timerVisible, &QTimer::timeout, this, &DialogPiecePath::EvalVisible);
    connect(ui->toolButtonExprVisible, &QPushButton::clicked, this, &DialogPiecePath::FXVisible);
    connect(ui->plainTextEditFormulaVisible, &QPlainTextEdit::textChanged, this, &DialogPiecePath::VisibleChanged);
    connect(ui->pushButtonGrowVisible, &QPushButton::clicked, this,
            &DialogPiecePath::DeployVisibleFormulaTextEdit);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitPathTypes()
{
    ui->comboBoxType->addItem(tr("Internal path"), static_cast<int>(PiecePathType::InternalPath));
    ui->comboBoxType->addItem(tr("Custom seam allowance"), static_cast<int>(PiecePathType::CustomSeamAllowance));

    ui->comboBoxPenType->setEnabled(GetType() == PiecePathType::InternalPath);
    ui->checkBoxCut->setEnabled(GetType() == PiecePathType::InternalPath);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::InitNodesList()
{
    const quint32 id = ui->comboBoxNodes->currentData().toUInt();

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
void DialogPiecePath::InitPassmarksList()
{
    const quint32 id = ui->comboBoxPassmarks->currentData().toUInt();

    ui->comboBoxPassmarks->blockSignals(true);
    ui->comboBoxPassmarks->clear();

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(ui->listWidget);

    for (auto &node : nodes)
    {
        if (node.GetTypeTool() == Tool::NodePoint && node.IsPassmark())
        {
            const QString name = GetNodeName(node);

            ui->comboBoxPassmarks->addItem(name, node.GetId());
        }
    }
    ui->comboBoxPassmarks->blockSignals(false);

    const int index = ui->comboBoxPassmarks->findData(id);
    if (index != -1)
    {
        ui->comboBoxPassmarks->setCurrentIndex(index);
        PassmarkChanged(index);// Need in case combox index was not changed
    }
    else
    {
        ui->comboBoxPassmarks->count() > 0 ? PassmarkChanged(0) : PassmarkChanged(-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::NodeAngleChanged(int index)
{
    const int i = ui->comboBoxNodes->currentIndex();
    if (i != -1 && index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(ui->comboBoxNodes->currentData().toUInt());
        if (rowItem)
        {
            const PieceNodeAngle angle = static_cast<PieceNodeAngle>(ui->comboBoxAngle->currentData().toUInt());
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

    VisToolPiecePath *visPath = qobject_cast<VisToolPiecePath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->SetPath(path);
    SetPenType(path.GetPenType());
    SetCutPath(path.IsCutPath());

    if (path.GetType() == PiecePathType::InternalPath)
    {
        SetFormulaVisible(path.GetVisibilityTrigger());
    }
    else
    {
        ui->plainTextEditFormulaVisible->setPlainText(QChar('1'));
    }

    ValidObjects(PathIsValid());

    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
PiecePathType DialogPiecePath::GetType() const
{
    return static_cast<PiecePathType>(ui->comboBoxType->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetType(PiecePathType type)
{
    const qint32 index = ui->comboBoxType->findData(static_cast<int>(type));
    if (index != -1)
    {
        ui->comboBoxType->setCurrentIndex(index);
    }

    ui->comboBoxPenType->setEnabled(type == PiecePathType::InternalPath);
    ui->checkBoxCut->setEnabled(type == PiecePathType::InternalPath);
}

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle DialogPiecePath::GetPenType() const
{
    return LineStyleToPenStyle(GetComboBoxCurrentData(ui->comboBoxPenType, TypeLineLine));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPenType(const Qt::PenStyle &type)
{
    ChangeCurrentData(ui->comboBoxPenType, PenStyleToLineStyle(type));
    vis->setLineStyle(type);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPiecePath::IsCutPath() const
{
    return ui->checkBoxCut->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetCutPath(bool value)
{
    ui->checkBoxCut->setChecked(value);
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
quint32 DialogPiecePath::GetLastId() const
{
    const int count = ui->listWidget->count();
    if (count > 0)
    {
        QListWidgetItem *item = ui->listWidget->item(count-1);
        const VPieceNode node = qvariant_cast<VPieceNode>(item->data(Qt::UserRole));
        return node.GetId();
    }
    else
    {
        return NULL_ID;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetCurrentSABefore(const QString &formula)
{
    UpdateNodeSABefore(formula);
    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetCurrentSAAfter(const QString &formula)
{
    UpdateNodeSAAfter(formula);
    ListChanged();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::UpdateNodeSABefore(const QString &formula)
{
    const int index = ui->comboBoxNodes->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(ui->comboBoxNodes->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetFormulaSABefore(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::UpdateNodeSAAfter(const QString &formula)
{
    const int index = ui->comboBoxNodes->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(ui->comboBoxNodes->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetFormulaSAAfter(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetFormulaSAWidth(const QString &formula)
{
    if (formula.isEmpty())
    {
        return;
    }

    const QString width = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (width.length() > 80)
    {
        this->DeployWidthFormulaTextEdit();
    }
    ui->plainTextEditFormulaWidth->setPlainText(width);

    VisToolPiecePath *path = qobject_cast<VisToolPiecePath *>(vis);
    SCASSERT(path != nullptr)
    path->SetPath(CreatePath());

    if (ui->tabWidget->indexOf(ui->tabSeamAllowance) == -1)
    {
        ui->tabWidget->addTab(ui->tabSeamAllowance, tr("Seam allowance"));
    }

    if (ui->tabWidget->indexOf(ui->tabPassmarks) == -1)
    {
        ui->tabWidget->addTab(ui->tabPassmarks, tr("Passmarks"));
    }

    MoveCursorToEnd(ui->plainTextEditFormulaWidth);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPiecePath::GetPieceId() const
{
    return getCurrentObjectId(ui->comboBoxPiece);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPieceId(quint32 id)
{
    if (ui->comboBoxPiece->count() <= 0)
    {
        ui->comboBoxPiece->addItem(data->GetPiece(id).GetName(), id);
    }
    else
    {
        const qint32 index = ui->comboBoxPiece->findData(id);
        if (index != -1)
        {
            ui->comboBoxPiece->setCurrentIndex(index);
        }
        else
        {
            ui->comboBoxPiece->setCurrentIndex(0);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPiecePath::GetFormulaSAWidth() const
{
    QString width = ui->plainTextEditFormulaWidth->toPlainText();
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPiecesList(const QVector<quint32> &list)
{
    FillComboBoxPiecesList(ui->comboBoxPiece, list);
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
    path.SetPenType(GetType() == PiecePathType::InternalPath ? GetPenType() : Qt::SolidLine);
    path.SetCutPath(GetType() == PiecePathType::InternalPath ? IsCutPath() : false);

    if (GetType() == PiecePathType::InternalPath)
    {
        path.SetVisibilityTrigger(GetFormulaVisible());
    }
    else
    {
        path.SetVisibilityTrigger(QChar('1'));
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
        if (GetType() == PiecePathType::CustomSeamAllowance && FirstPointEqualLast(ui->listWidget, data))
        {
            url += tr("First point of <b>custom seam allowance</b> cannot be equal to the last point!");
            ui->helpLabel->setText(url);
            return false;
        }
        else if (DoublePoints(ui->listWidget, data))
        {
            url += tr("You have double points!");
            ui->helpLabel->setText(url);
            return false;
        }
        else if (GetType() == PiecePathType::CustomSeamAllowance && not EachPointLabelIsUnique(ui->listWidget))
        {
            url += tr("Each point in the <b>custom seam allowance</b> path must be unique!");
            ui->helpLabel->setText(url);
            return false;
        }
    }

    if (not m_showMode && ui->comboBoxPiece->count() <= 0)
    {
        url += tr("List of details is empty!");
        ui->helpLabel->setText(url);
        return false;
    }
    else if (not m_showMode && ui->comboBoxPiece->currentIndex() == -1)
    {
        url += tr("Please, select a detail to insert into!");
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

//---------------------------------------------------------------------------------------------------------------------
QString DialogPiecePath::GetFormulaSAWidthBefore() const
{
    QString width = ui->plainTextEditFormulaWidthBefore->toPlainText();
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPiecePath::GetFormulaSAWidthAfter() const
{
    QString width = ui->plainTextEditFormulaWidthAfter->toPlainText();
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPiecePath::GetFormulaVisible() const
{
    QString formula = ui->plainTextEditFormulaVisible->toPlainText();
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetFormulaVisible(const QString &formula)
{
    const QString f = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f.length() > 80)
    {
        this->DeployVisibleFormulaTextEdit();
    }
    ui->plainTextEditFormulaVisible->setPlainText(f);
    MoveCursorToEnd(ui->plainTextEditFormulaVisible);
}
