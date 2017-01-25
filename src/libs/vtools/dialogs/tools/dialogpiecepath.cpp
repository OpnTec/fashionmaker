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
#include "../../tools/vabstracttool.h"
#include "../../tools/vtoolseamallowance.h"
#include "../support/dialogeditwrongformula.h"

#include <QMenu>
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogPiecePath::DialogPiecePath(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogPiecePath),
      m_showMode(false),
      m_saWidth(0),
      m_timerWidth(nullptr),
      m_timerWidthBefore(nullptr),
      m_timerWidthAfter(nullptr),
      m_formulaBaseWidth(0),
      m_formulaBaseWidthBefore(0),
      m_formulaBaseWidthAfter(0)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    InitPathTab();
    InitSeamAllowanceTab();

    flagName = true;//We have default name of piece.
    flagError = PathIsValid();
    CheckState();

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
        if (id != GetLastId())
        {
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
void DialogPiecePath::ShowContextMenu(const QPoint &pos)
{
    const int row = ui->listWidget->currentRow();
    if (ui->listWidget->count() == 0 || row == -1 || row >= ui->listWidget->count())
    {
        return;
    }

    QMenu *menu = new QMenu(this);

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

    QAction *actionDelete = menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));

    QAction *selectedAction = menu->exec(ui->listWidget->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete ui->listWidget->item(row);
        ValidObjects(PathIsValid());
    }
    else if (rowNode.GetTypeTool() != Tool::NodePoint && selectedAction == actionReverse)
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
    ui->plainTextEditFormulaWidthBefore->setDisabled(true);
    ui->toolButtonExprBefore->setDisabled(true);
    ui->pushButtonDefBefore->setDisabled(true);

    ui->plainTextEditFormulaWidthAfter->setDisabled(true);
    ui->toolButtonExprAfter->setDisabled(true);
    ui->pushButtonDefAfter->setDisabled(true);

    ui->comboBoxAngle->setDisabled(true);

    ui->plainTextEditFormulaWidthBefore->blockSignals(true);
    ui->plainTextEditFormulaWidthAfter->blockSignals(true);
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
                ui->pushButtonDefBefore->setEnabled(true);
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

    ui->plainTextEditFormulaWidthBefore->blockSignals(false);
    ui->plainTextEditFormulaWidthAfter->blockSignals(false);
    ui->comboBoxAngle->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefBefore()
{
    ui->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::ReturnDefAfter()
{
    ui->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidth()
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
                                                                  tr("Current seam aloowance")));

        EvalWidthBefore();
        EvalWidthAfter();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidthBefore()
{
    labelEditFormula = ui->labelEditBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidthBefore->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, ui->labelResultBefore, postfix, false, true);

    UpdateNodeSABefore(GetFormulaSAWidthBefore());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::EvalWidthAfter()
{
    labelEditFormula = ui->labelEditAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const QString formula = ui->plainTextEditFormulaWidthAfter->toPlainText();
    bool flagFormula = false; // fake flag
    Eval(formula, flagFormula, ui->labelResultAfter, postfix, false, true);

    UpdateNodeSABefore(GetFormulaSAWidthAfter());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::FXWidth()
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
void DialogPiecePath::FXWidthBefore()
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
void DialogPiecePath::FXWidthAfter()
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
void DialogPiecePath::WidthChanged()
{
    labelEditFormula = ui->labelEditWidth;
    labelResultCalculation = ui->labelResultWidth;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidth, m_timerWidth, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::WidthBeforeChanged()
{
    labelEditFormula = ui->labelEditBefore;
    labelResultCalculation = ui->labelResultBefore;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidthBefore, m_timerWidthBefore, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::WidthAfterChanged()
{
    labelEditFormula = ui->labelEditAfter;
    labelResultCalculation = ui->labelResultAfter;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    bool flagFormula = false;
    ValFormulaChanged(flagFormula, ui->plainTextEditFormulaWidthAfter, m_timerWidthAfter, postfix);
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
void DialogPiecePath::InitPathTab()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditName->setClearButtonEnabled(true);
#endif

    FillComboBoxTypeLine(ui->comboBoxPenType, VAbstractTool::LineStylesPics());

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogPiecePath::NameChanged);

    InitPathTypes();
    connect(ui->comboBoxType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this]()
    {
        ui->comboBoxPenType->setEnabled(GetType() == PiecePathType::InternalPath);
        ValidObjects(PathIsValid());
    });

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &DialogPiecePath::ShowContextMenu);
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

    m_timerWidth = new QTimer(this);
    connect(m_timerWidth, &QTimer::timeout, this, &DialogPiecePath::EvalWidth);

    m_timerWidthBefore = new QTimer(this);
    connect(m_timerWidthBefore, &QTimer::timeout, this, &DialogPiecePath::EvalWidthBefore);

    m_timerWidthAfter = new QTimer(this);
    connect(m_timerWidthAfter, &QTimer::timeout, this, &DialogPiecePath::EvalWidthAfter);

    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    m_saWidth = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
    ui->plainTextEditFormulaWidth->setPlainText(qApp->LocaleToString(m_saWidth));

    InitNodesList();
    connect(ui->comboBoxNodes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogPiecePath::NodeChanged);

    connect(ui->pushButtonDefBefore, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefBefore);
    connect(ui->pushButtonDefAfter, &QPushButton::clicked, this, &DialogPiecePath::ReturnDefAfter);

    InitNodeAngles(ui->comboBoxAngle);
    connect(ui->comboBoxAngle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
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
void DialogPiecePath::InitPathTypes()
{
    ui->comboBoxType->addItem(tr("Internal path"), static_cast<int>(PiecePathType::InternalPath));
    ui->comboBoxType->addItem(tr("Custom seam allowance"), static_cast<int>(PiecePathType::CustomSeamAllowance));

    ui->comboBoxPenType->setEnabled(GetType() == PiecePathType::InternalPath);
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

    VisToolPiecePath *visPath = qobject_cast<VisToolPiecePath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->SetPath(path);
    SetPenType(path.GetPenType());

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

    ui->comboBoxPenType->setEnabled(type == PiecePathType::InternalPath);
}

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle DialogPiecePath::GetPenType() const
{
    return VAbstractTool::LineStyleToPenStyle(GetComboBoxCurrentData(ui->comboBoxPenType, TypeLineLine));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPiecePath::SetPenType(const Qt::PenStyle &type)
{
    ChangeCurrentData(ui->comboBoxPenType, VAbstractTool::PenStyleToLineStyle(type));
    vis->setLineStyle(type);
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
void DialogPiecePath::UpdateNodeSAAfter(const QString &formula)
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
void DialogPiecePath::SetFormulaSAWidth(const QString &formula)
{
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

    MoveCursorToEnd(ui->plainTextEditFormulaWidth);
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
QString DialogPiecePath::GetFormulaSAWidth() const
{
    QString width = ui->plainTextEditFormulaWidth->toPlainText();
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
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
    path.SetPenType(GetType() == PiecePathType::InternalPath ? GetPenType() : Qt::SolidLine);

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
            url += tr("First point of <b>custom seam allowance</b> cannot be equal to the last point!");
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
        url += tr("List of objects is empty!");
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
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPiecePath::GetFormulaSAWidthAfter() const
{
    QString width = ui->plainTextEditFormulaWidthAfter->toPlainText();
    width.replace("\n", " ");
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}
