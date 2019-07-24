/************************************************************************
 **
 **  @file   dialogseamallowance.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
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

#include "dialogseamallowance.h"
#include "ui_dialogseamallowance.h"
#include "ui_tabpaths.h"
#include "ui_tablabels.h"
#include "ui_tabgrainline.h"
#include "ui_tabpins.h"
#include "ui_tabpassmarks.h"
#include "ui_tabplacelabels.h"
#include "../vwidgets/fancytabbar/fancytabbar.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/calculator.h"
#include "visualization/path/vistoolpiece.h"
#include "visualization/path/vispiecespecialpoints.h"
#include "dialogpiecepath.h"
#include "dialogplacelabel.h"
#include "../../../undocommands/savepiecepathoptions.h"
#include "../../../undocommands/saveplacelabeloptions.h"
#include "../../support/dialogeditwrongformula.h"
#include "../../support/dialogeditlabel.h"
#include "../../../tools/vtoolseamallowance.h"
#include "../vgeometry/vplacelabelitem.h"
#include "../../dialogtoolbox.h"
#include "../vmisc/vmodifierkey.h"

#include <QMenu>
#include <QTimer>
#include <QtNumeric>

enum TabOrder {Paths=0, Pins=1, Labels=2, Grainline=3,  Passmarks=4, PlaceLabels=5, Count=6};

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
    return qApp->TrVars()->TryFormulaFromUser(textEdit->toPlainText(), qApp->Settings()->GetOsSeparator());
}
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, const VAbstractPattern *doc, quint32 toolId,
                                         QWidget *parent)
    : DialogSeamAllowance(data, toolId, parent)
{
    SCASSERT(doc != nullptr)
    uiTabLabels->groupBoxPatternLabel->setEnabled(not doc->GetPatternLabelTemplate().isEmpty());
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::DialogSeamAllowance(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogSeamAllowance),
      uiTabPaths(new Ui::TabPaths),
      uiTabLabels(new Ui::TabLabels),
      uiTabGrainline(new Ui::TabGrainline),
      uiTabPins(new Ui::TabPins),
      uiTabPassmarks(new Ui::TabPassmarks),
      uiTabPlaceLabels(new Ui::TabPlaceLabels),
      m_tabPaths(new QWidget),
      m_tabLabels(new QWidget),
      m_tabGrainline(new QWidget),
      m_tabPins(new QWidget),
      m_tabPassmarks(new QWidget),
      m_tabPlaceLabels(new QWidget),
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
      flagFormulaBefore(true),
      flagFormulaAfter(true),
      flagMainPathIsValid(true),
      flagName(true), //We have default name of piece.
      flagFormula(true),
      m_bAddMode(true),
      m_dialog(),
      m_visSpecialPoints(),
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
      m_timerWidth(new QTimer(this)),
      m_timerWidthBefore(new QTimer(this)),
      m_timerWidthAfter(new QTimer(this)),
      m_timerPassmarkLength(new QTimer(this)),
      m_saWidth(0),
      m_templateLines(),
      m_undoStack(),
      m_newPlaceLabels(),
      m_newPaths()
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
    InitPlaceLabelsTab();

    ChangeColor(uiTabPaths->labelEditName, OkColor(this));
    flagMainPathIsValid = MainPathIsValid();

    m_ftb->SetCurrentIndex(TabOrder::Paths);// Show always first tab active on start.
}

//---------------------------------------------------------------------------------------------------------------------
DialogSeamAllowance::~DialogSeamAllowance()
{
    delete m_visSpecialPoints;
    delete m_tabPlaceLabels;
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

    for (auto &command : m_undoStack)
    {
        if (not command.isNull())
        {
            delete command;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableApply(bool enable)
{
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(enable);
    applyAllowed = enable;

    uiTabPaths->tabSeamAllowance->setEnabled(applyAllowed);
    uiTabPaths->tabInternalPaths->setEnabled(applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Pins, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Labels, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Grainline, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::Passmarks, applyAllowed);
    m_ftb->SetTabEnabled(TabOrder::PlaceLabels, applyAllowed);

    if (not applyAllowed && vis.isNull())
    {
        vis = new VisToolPiece(data);
    }
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
    QVector<CustomSARecord> records = piece.GetCustomSARecords();
    for (auto record : records)
    {
        if (record.path > NULL_ID)
        {
            const QString name = GetPathName(record.path, record.reverse);

            QListWidgetItem *item = new QListWidgetItem(name);
            item->setFont(NodeFont(item->font()));
            item->setData(Qt::UserRole, QVariant::fromValue(record));
            uiTabPaths->listWidgetCustomSA->addItem(item);
            uiTabPaths->listWidgetCustomSA->setCurrentRow(uiTabPaths->listWidgetCustomSA->count()-1);
        }
    }
    uiTabPaths->listWidgetCustomSA->blockSignals(false);

    uiTabPaths->listWidgetInternalPaths->clear();
    const QVector<quint32> paths = piece.GetInternalPaths();
    for (auto path : paths)
    {
        if (path > NULL_ID)
        {
            const QString name = GetPathName(path);

            QListWidgetItem *item = new QListWidgetItem(name);
            item->setFont(NodeFont(item->font()));
            item->setData(Qt::UserRole, QVariant::fromValue(path));
            uiTabPaths->listWidgetInternalPaths->addItem(item);
            uiTabPaths->listWidgetInternalPaths->setCurrentRow(uiTabPaths->listWidgetInternalPaths->count()-1);
        }
    }

    auto NewSpecialPoint = [this](QListWidget *listWidget, quint32 point)
    {
        if (point > NULL_ID)
        {
            try
            {
                const QSharedPointer<VGObject> p = data->GetGObject(point);

                QListWidgetItem *item = new QListWidgetItem(p->name());
                item->setFont(NodeFont(item->font()));
                item->setData(Qt::UserRole, QVariant::fromValue(point));
                listWidget->addItem(item);
                listWidget->setCurrentRow(uiTabPins->listWidgetPins->count()-1);
            }
            catch (const VExceptionBadId &e)
            {
                qWarning() << qUtf8Printable(e.ErrorMessage());
            }
        }
    };

    uiTabPins->listWidgetPins->clear();
    const QVector<quint32> pins = piece.GetPins();
    for (auto pin : pins)
    {
        NewSpecialPoint(uiTabPins->listWidgetPins, pin);
    }

    InitAllPinComboboxes();

    uiTabPlaceLabels->listWidgetPlaceLabels->clear();
    const QVector<quint32> labels = piece.GetPlaceLabels();
    for (auto label : labels)
    {
        NewSpecialPoint(uiTabPlaceLabels->listWidgetPlaceLabels, label);
    }

    if (piece.GetPlaceLabels().size() > 0)
    {
        uiTabPlaceLabels->listWidgetPlaceLabels->setCurrentRow(0);
    }

    uiTabPaths->comboBoxStartPoint->blockSignals(true);
    uiTabPaths->comboBoxStartPoint->clear();
    uiTabPaths->comboBoxStartPoint->blockSignals(false);

    uiTabPaths->comboBoxEndPoint->blockSignals(true);
    uiTabPaths->comboBoxEndPoint->clear();
    uiTabPaths->comboBoxEndPoint->blockSignals(false);

    CustomSAChanged(0);

    uiTabPaths->checkBoxForbidFlipping->setChecked(piece.IsForbidFlipping());
    uiTabPaths->checkBoxForceFlipping->setChecked(piece.IsForceFlipping());
    uiTabPaths->checkBoxSeams->setChecked(piece.IsSeamAllowance());
    uiTabPaths->checkBoxBuiltIn->setChecked(piece.IsSeamAllowanceBuiltIn());
    uiTabPaths->lineEditName->setText(piece.GetName());
    uiTabPaths->spinBoxPriority->setValue(static_cast<int>(piece.GetPriority()));

    uiTabPaths->plainTextEditFormulaWidth->setPlainText(
                qApp->TrVars()->FormulaToUser(piece.GetFormulaSAWidth(), qApp->Settings()->GetOsSeparator()));
    m_saWidth = piece.GetSAWidth();

    const VPieceLabelData &ppData = piece.GetPatternPieceData();
    uiTabLabels->lineEditLetter->setText(ppData.GetLetter());
    uiTabLabels->lineEditAnnotation->setText(ppData.GetAnnotation());
    uiTabLabels->lineEditOrientation->setText(ppData.GetOrientation());
    uiTabLabels->lineEditRotation->setText(ppData.GetRotationWay());
    uiTabLabels->lineEditTilt->setText(ppData.GetTilt());
    uiTabLabels->lineEditFoldPosition->setText(ppData.GetFoldPosition());
    uiTabLabels->spinBoxQuantity->setValue(ppData.GetQuantity());
    uiTabLabels->checkBoxFold->setChecked(ppData.IsOnFold());
    m_templateLines = ppData.GetLabelTemplate();

    uiTabLabels->groupBoxDetailLabel->setEnabled(not m_templateLines.isEmpty());

    uiTabGrainline->comboBoxArrow->setCurrentIndex(int(piece.GetGrainlineGeometry().GetArrowType()));

    uiTabLabels->groupBoxDetailLabel->setChecked(ppData.IsVisible());
    ChangeCurrentData(uiTabLabels->comboBoxDLCenterPin, ppData.CenterPin());
    ChangeCurrentData(uiTabLabels->comboBoxDLTopLeftPin, ppData.TopLeftPin());
    ChangeCurrentData(uiTabLabels->comboBoxDLBottomRightPin, ppData.BottomRightPin());
    SetDLWidth(ppData.GetLabelWidth());
    SetDLHeight(ppData.GetLabelHeight());
    SetDLAngle(ppData.GetRotation());

    const VPatternLabelData &patternInfo = piece.GetPatternInfo();
    uiTabLabels->groupBoxPatternLabel->setChecked(patternInfo.IsVisible());
    ChangeCurrentData(uiTabLabels->comboBoxPLCenterPin, patternInfo.CenterPin());
    ChangeCurrentData(uiTabLabels->comboBoxPLTopLeftPin, patternInfo.TopLeftPin());
    ChangeCurrentData(uiTabLabels->comboBoxPLBottomRightPin, patternInfo.BottomRightPin());
    SetPLWidth(patternInfo.GetLabelWidth());
    SetPLHeight(patternInfo.GetLabelHeight());
    SetPLAngle(patternInfo.GetRotation());

    const VGrainlineData &grainlineGeometry = piece.GetGrainlineGeometry();
    uiTabGrainline->groupBoxGrainline->setChecked(grainlineGeometry.IsVisible());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineCenterPin, grainlineGeometry.CenterPin());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineTopPin, grainlineGeometry.TopPin());
    ChangeCurrentData(uiTabGrainline->comboBoxGrainlineBottomPin, grainlineGeometry.BottomPin());
    SetGrainlineAngle(grainlineGeometry.GetRotation());
    SetGrainlineLength(grainlineGeometry.GetLength());

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
                emit ToolTip(tr("Select main path objects clockwise, <b>%1</b> - reverse direction curve, "
                                "<b>%2</b> - finish creation")
                             .arg(VModifierKey::Shift(), VModifierKey::EnterKey()));

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
    if (click == false && uiTabPaths->listWidgetMainPath->count() > 0)
    {
        emit ToolTip(QString());
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
    bOk->setEnabled(IsValid());
    // In case dialog hasn't apply button
    if ( bApply != nullptr && applyAllowed)
    {
        bApply->setEnabled(bOk->isEnabled());
    }

    if (flagFormula && flagFormulaBefore && flagFormulaAfter)
    {
        if (flagMainPathIsValid && flagName)
        {
            m_ftb->SetTabText(TabOrder::Paths, tr("Paths"));
        }

        uiTabPaths->tabWidget->setTabIcon(uiTabPaths->tabWidget->indexOf(uiTabPaths->tabSeamAllowance), QIcon());
    }
    else
    {
        m_ftb->SetTabText(TabOrder::Paths, tr("Paths") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        uiTabPaths->tabWidget->setTabIcon(uiTabPaths->tabWidget->indexOf(uiTabPaths->tabSeamAllowance), icon);
    }

    if (flagMainPathIsValid && flagName)
    {
        if (flagFormula && flagFormulaBefore && flagFormulaAfter)
        {
            m_ftb->SetTabText(TabOrder::Paths, tr("Paths"));
        }
        QString tooltip = tr("Ready!");
        if (not applyAllowed)
        {
            tooltip = tooltip + QStringLiteral("  <b>") +
                    tr("To open all detail's features complete creating the main path. Please, press OK.")
                    + QStringLiteral("</b>");
        }
        uiTabPaths->helpLabel->setText(tooltip);
        uiTabPaths->tabWidget->setTabIcon(uiTabPaths->tabWidget->indexOf(uiTabPaths->tabMainPath), QIcon());
    }
    else
    {
        m_ftb->SetTabText(TabOrder::Paths, tr("Paths") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        uiTabPaths->tabWidget->setTabIcon(uiTabPaths->tabWidget->indexOf(uiTabPaths->tabMainPath), icon);
    }

    uiTabPaths->comboBoxNodes->setEnabled(flagFormulaBefore && flagFormulaAfter);

    if (uiTabPassmarks->comboBoxPassmarks->count() == 0)
    {
        flagFormulaPassmarkLength = true;
    }

    if (flagFormulaPassmarkLength)
    {
        m_ftb->SetTabText(TabOrder::Passmarks, tr("Passmarks"));
    }
    else
    {
        m_ftb->SetTabText(TabOrder::Passmarks, tr("Passmarks") + '*');
    }

    uiTabPassmarks->comboBoxPassmarks->setEnabled(flagFormulaPassmarkLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::closeEvent(QCloseEvent *event)
{
    uiTabPaths->plainTextEditFormulaWidth->blockSignals(true);
    uiTabPaths->plainTextEditFormulaWidthBefore->blockSignals(true);
    uiTabPaths->plainTextEditFormulaWidthAfter->blockSignals(true);
    uiTabGrainline->lineEditRotFormula->blockSignals(true);
    uiTabGrainline->lineEditLenFormula->blockSignals(true);
    uiTabPassmarks->plainTextEditPassmarkLength->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event ); // clazy:exclude=skipped-base-method
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    const QSize sz = qApp->Settings()->GetToolSeamAllowanceDialogSize();
    if (not sz.isEmpty())
    {
        resize(sz);
    }

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::resizeEvent(QResizeEvent *event)
{
    // remember the size for the next time this dialog is opened, but only
    // if widget was already initialized, which rules out the resize at
    // dialog creating, which would
    if (isInitialized)
    {
        qApp->Settings()->SetToolSeamAllowanceDialogSize(size());
    }
    DialogTool::resizeEvent(event);
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
            ChangeColor(uiTabPaths->labelEditName, errorColor);
        }
        else
        {
            flagName = true;
            ChangeColor(uiTabPaths->labelEditName, OkColor(this));
        }
    }
    CheckState();
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
        if (applyAllowed)
        {
            actionPassmark = menu->addAction(tr("Passmark"));
            actionPassmark->setCheckable(true);
            actionPassmark->setChecked(rowNode.IsPassmark());
        }

        actionUniqueness = menu->addAction(tr("Check uniqueness"));
        actionUniqueness->setCheckable(true);
        actionUniqueness->setChecked(rowNode.IsCheckUniqueness());
    }

    QAction *actionExcluded = menu->addAction(tr("Excluded"));
    actionExcluded->setCheckable(true);
    actionExcluded->setChecked(rowNode.IsExcluded());

    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));

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
        rowItem->setFont(NodeFont(rowItem->font(), rowNode.IsExcluded()));
    }
    else if (applyAllowed && selectedAction == actionPassmark)
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
    QAction *actionOption = menu->addAction(QIcon::fromTheme(preferencesOtherIcon), tr("Options"));

    QListWidgetItem *rowItem = uiTabPaths->listWidgetCustomSA->item(row);
    SCASSERT(rowItem != nullptr);
    CustomSARecord record = qvariant_cast<CustomSARecord>(rowItem->data(Qt::UserRole));

    QAction *actionReverse = menu->addAction(tr("Reverse"));
    actionReverse->setCheckable(true);
    actionReverse->setChecked(record.reverse);

    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));

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
        dialog->SetPiecePath(CurrentPath(record.path));
        dialog->SetPieceId(toolId);
        if (record.includeType == PiecePathIncludeType::AsMainPath)
        {
            dialog->SetFormulaSAWidth(GetFormulaSAWidth());
        }
        dialog->HideVisibilityTrigger();
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
    QAction *actionOption = menu->addAction(QIcon::fromTheme(preferencesOtherIcon), tr("Options"));
    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));

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
        dialog->SetPiecePath(CurrentPath(pathId));
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
    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPins->listWidgetPins->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPins->listWidgetPins->item(row);
        FancyTabChanged(m_ftb->CurrentIndex());
        InitAllPinComboboxes();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowPlaceLabelsContextMenu(const QPoint &pos)
{
    const int row = uiTabPlaceLabels->listWidgetPlaceLabels->currentRow();
    if (uiTabPlaceLabels->listWidgetPlaceLabels->count() == 0
            || row == -1
            || row >= uiTabPlaceLabels->listWidgetPlaceLabels->count())
    {
        return;
    }

    QListWidgetItem *rowItem = uiTabPlaceLabels->listWidgetPlaceLabels->item(row);
    SCASSERT(rowItem != nullptr);
    const quint32 labelId = qvariant_cast<quint32>(rowItem->data(Qt::UserRole));
    VPlaceLabelItem currentLabel = CurrentPlaceLabel(labelId);

    QScopedPointer<QMenu> menu(new QMenu());

    auto InitAction = [currentLabel, &menu](const QString &text, PlaceLabelType type)
    {
        QAction *action = menu->addAction(text);
        action->setCheckable(true);
        action->setChecked(currentLabel.GetLabelType() == type);
        return action;
    };

    auto SaveType = [this, currentLabel, labelId](PlaceLabelType type)
    {
        VPlaceLabelItem newLabel = VPlaceLabelItem(currentLabel);
        newLabel.SetLabelType(type);
        m_newPlaceLabels.insert(labelId, newLabel);

        QPointer<VUndoCommand> saveCommand =
                new SavePlaceLabelOptions(toolId, currentLabel, newLabel, qApp->getCurrentDocument(),
                                          const_cast<VContainer *>(data), labelId);
        m_undoStack.append(saveCommand);
        UpdateCurrentPlaceLabelRecords();
    };

    QAction *actionOption = menu->addAction(QIcon::fromTheme(preferencesOtherIcon), tr("Options"));
    menu->addSeparator();
    QAction *actionSegment = InitAction(tr("Segment"), PlaceLabelType::Segment);
    QAction *actionRectangle = InitAction(tr("Rectangle"), PlaceLabelType::Rectangle);
    QAction *actionCross = InitAction(tr("Cross"), PlaceLabelType::Cross);
    QAction *actionTshaped = InitAction(tr("T-shaped"), PlaceLabelType::Tshaped);
    QAction *actionDoubletree = InitAction(tr("Doubletree"), PlaceLabelType::Doubletree);
    QAction *actionCorner = InitAction(tr("Corner"), PlaceLabelType::Corner);
    QAction *actionTriangle = InitAction(tr("Triangle"), PlaceLabelType::Triangle);
    QAction *actionHshaped = InitAction(tr("H-shaped"), PlaceLabelType::Hshaped);
    QAction *actionButton = InitAction(tr("Button"), PlaceLabelType::Button);
    QAction *actionCircle = InitAction(tr("Circle"), PlaceLabelType::Circle);
    menu->addSeparator();
    QAction *actionDelete = menu->addAction(QIcon::fromTheme(editDeleteIcon), tr("Delete"));

    QAction *selectedAction = menu->exec(uiTabPlaceLabels->listWidgetPlaceLabels->viewport()->mapToGlobal(pos));
    if (selectedAction == actionDelete)
    {
        delete uiTabPlaceLabels->listWidgetPlaceLabels->item(row);
        FancyTabChanged(m_ftb->CurrentIndex());
    }
    else if (selectedAction == actionOption)
    {
        auto *dialog = new DialogPlaceLabel(data, labelId, this);
        dialog->SetCenterPoint(labelId);
        dialog->SetLabelType(currentLabel.GetLabelType());
        dialog->SetWidth(currentLabel.GetWidthFormula());
        dialog->SetHeight(currentLabel.GetHeightFormula());
        dialog->SetAngle(currentLabel.GetAngleFormula());
        dialog->SetFormulaVisible(currentLabel.GetVisibilityTrigger());
        dialog->SetPieceId(toolId);
        dialog->EnbleShowMode(true);
        m_dialog = dialog;
        m_dialog->setModal(true);
        connect(m_dialog.data(), &DialogTool::DialogClosed, this, &DialogSeamAllowance::PlaceLabelDialogClosed);
        m_dialog->show();
    }
    else if (selectedAction == actionSegment)
    {
        SaveType(PlaceLabelType::Segment);
    }
    else if (selectedAction == actionRectangle)
    {
        SaveType(PlaceLabelType::Rectangle);
    }
    else if (selectedAction == actionCross)
    {
        SaveType(PlaceLabelType::Cross);
    }
    else if (selectedAction == actionTshaped)
    {
        SaveType(PlaceLabelType::Tshaped);
    }
    else if (selectedAction == actionDoubletree)
    {
        SaveType(PlaceLabelType::Doubletree);
    }
    else if (selectedAction == actionCorner)
    {
        SaveType(PlaceLabelType::Corner);
    }
    else if (selectedAction == actionTriangle)
    {
        SaveType(PlaceLabelType::Triangle);
    }
    else if (selectedAction == actionHshaped)
    {
        SaveType(PlaceLabelType::Hshaped);
    }
    else if (selectedAction == actionButton)
    {
        SaveType(PlaceLabelType::Button);
    }
    else if (selectedAction == actionCircle)
    {
        SaveType(PlaceLabelType::Circle);
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
    SetMoveControls();
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
        const int nodeIndex = piece.GetPath().indexOfNode(uiTabPaths->comboBoxNodes->currentData().toUInt());
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
        uiTabPaths->plainTextEditFormulaWidthBefore->setPlainText(currentSeamAllowance);
        uiTabPaths->plainTextEditFormulaWidthAfter->setPlainText(currentSeamAllowance);
        uiTabPaths->comboBoxAngle->setCurrentIndex(-1);
    }
    uiTabPaths->comboBoxAngle->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PassmarkChanged(int index)
{
    uiTabPassmarks->groupBoxMarkType->setDisabled(true);
    uiTabPassmarks->groupBoxAngleType->setDisabled(true);
    uiTabPassmarks->groupBoxManualLength->setDisabled(true);

    uiTabPassmarks->checkBoxShowSecondPassmark->setDisabled(true);
    uiTabPassmarks->checkBoxShowSecondPassmark->blockSignals(true);

    uiTabPassmarks->groupBoxManualLength->blockSignals(true);
    uiTabPassmarks->groupBoxMarkType->blockSignals(true);
    uiTabPassmarks->groupBoxAngleType->blockSignals(true);

    uiTabPassmarks->groupBoxManualLength->setChecked(false);

    if (index != -1)
    {
        const VPiece piece = CreatePiece();
        const int nodeIndex = piece.GetPath().indexOfNode(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
        if (nodeIndex != -1)
        {
            const VPieceNode &node = piece.GetPath().at(nodeIndex);

            // Passmark length
            uiTabPassmarks->groupBoxManualLength->setEnabled(true);

            if (node.IsManualPassmarkLength())
            {
                uiTabPassmarks->groupBoxManualLength->setChecked(true);

                QString passmarkLength = node.GetFormulaPassmarkLength();
                passmarkLength = qApp->TrVars()->FormulaToUser(passmarkLength, qApp->Settings()->GetOsSeparator());
                if (passmarkLength.length() > 80)// increase height if needed.
                {
                    this->DeployPassmarkLength();
                }

                if (passmarkLength.isEmpty())
                {
                    qreal length = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
                    uiTabPassmarks->plainTextEditPassmarkLength->setPlainText(qApp->LocaleToString(length));
                }
                else
                {
                    uiTabPassmarks->plainTextEditPassmarkLength->setPlainText(passmarkLength);
                }
            }
            else
            {
                qreal length = UnitConvertor(1, Unit::Cm, qApp->patternUnit());
                uiTabPassmarks->plainTextEditPassmarkLength->setPlainText(qApp->LocaleToString(length));
            }

            MoveCursorToEnd(uiTabPassmarks->plainTextEditPassmarkLength);

            // Line type
            uiTabPassmarks->groupBoxMarkType->setEnabled(true);

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
                case PassmarkLineType::VMark2:
                    uiTabPassmarks->radioButtonVMark2->setChecked(true);
                    break;
                case PassmarkLineType::UMark:
                    uiTabPassmarks->radioButtonUMark->setChecked(true);
                    break;
                case PassmarkLineType::BoxMark:
                    uiTabPassmarks->radioButtonBoxMark->setChecked(true);
                    break;
                default:
                    break;
            }

            // Angle type
            uiTabPassmarks->groupBoxAngleType->setEnabled(true);

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
                case PassmarkAngleType::IntersectionOnlyLeft:
                    uiTabPassmarks->radioButtonIntersectionOnlyLeft->setChecked(true);
                    break;
                case PassmarkAngleType::IntersectionOnlyRight:
                    uiTabPassmarks->radioButtonIntersectionOnlyRight->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2:
                    uiTabPassmarks->radioButtonIntersection2->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2OnlyLeft:
                    uiTabPassmarks->radioButtonIntersection2OnlyLeft->setChecked(true);
                    break;
                case PassmarkAngleType::Intersection2OnlyRight:
                    uiTabPassmarks->radioButtonIntersection2OnlyRight->setChecked(true);
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
    uiTabPassmarks->groupBoxManualLength->blockSignals(false);
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
    record.startPoint = uiTabPaths->comboBoxStartPoint->currentData().toUInt();
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
    record.endPoint = uiTabPaths->comboBoxEndPoint->currentData().toUInt();
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
    record.includeType = static_cast<PiecePathIncludeType>(uiTabPaths->comboBoxIncludeType->currentData().toUInt());
    rowItem->setData(Qt::UserRole, QVariant::fromValue(record));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NodeAngleChanged(int index)
{
    const int i = uiTabPaths->comboBoxNodes->currentIndex();
    if (i != -1 && index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(uiTabPaths->comboBoxNodes->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetAngleType(static_cast<PieceNodeAngle>(uiTabPaths->comboBoxAngle->currentData().toUInt()));
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));

            ListChanged();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefBefore()
{
    const QString def = qApp->TrVars()->FormulaToUser(currentSeamAllowance, qApp->Settings()->GetOsSeparator());
    uiTabPaths->plainTextEditFormulaWidthBefore->setPlainText(def);
    if (QPushButton* button = qobject_cast<QPushButton*>(sender()))
    {
        button->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ReturnDefAfter()
{
    const QString def = qApp->TrVars()->FormulaToUser(currentSeamAllowance, qApp->Settings()->GetOsSeparator());
    uiTabPaths->plainTextEditFormulaWidthAfter->setPlainText(def);
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
            VPiecePath currentPath = CurrentPath(dialogTool->GetToolId());
            VPiecePath newPath = dialogTool->GetPiecePath();
            m_newPaths.insert(dialogTool->GetToolId(), newPath);

            QPointer<VUndoCommand> saveCommand =
                        new SavePiecePathOptions(toolId, currentPath, newPath, qApp->getCurrentDocument(),
                                                 const_cast<VContainer *>(data), dialogTool->GetToolId());
            m_undoStack.append(saveCommand);
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
void DialogSeamAllowance::PlaceLabelDialogClosed(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(not m_dialog.isNull());
        DialogPlaceLabel *dialogTool = qobject_cast<DialogPlaceLabel*>(m_dialog.data());
        SCASSERT(dialogTool != nullptr);
        try
        {
            VPlaceLabelItem currentLabel = CurrentPlaceLabel(dialogTool->GetToolId());

            const QHash<QString, QSharedPointer<VInternalVariable> > *vars = data->DataVariables();

            const qreal w = qAbs(Visualization::FindLengthFromUser(dialogTool->GetWidth(), vars, false));
            const qreal h = qAbs(Visualization::FindLengthFromUser(dialogTool->GetHeight(), vars, false));
            const qreal a = Visualization::FindValFromUser(dialogTool->GetAngle(), vars, false);
            const qreal v = Visualization::FindValFromUser(dialogTool->GetFormulaVisible(), vars, false);
            qDebug() << w << h << a << v;

            VPlaceLabelItem newLabel =  VPlaceLabelItem();
            newLabel.setName(currentLabel.name());
            newLabel.setX(currentLabel.x());
            newLabel.setY(currentLabel.y());
            newLabel.setMx(currentLabel.mx());
            newLabel.setMy(currentLabel.my());
            newLabel.SetWidth(w, dialogTool->GetWidth());
            newLabel.SetHeight(h, dialogTool->GetHeight());
            newLabel.SetAngle(a, dialogTool->GetAngle());
            newLabel.SetVisibilityTrigger(v, dialogTool->GetFormulaVisible());
            newLabel.SetLabelType(dialogTool->GetLabelType());
            newLabel.SetCenterPoint(currentLabel.GetCenterPoint());
            newLabel.SetCorrectionAngle(currentLabel.GetCorrectionAngle());

            m_newPlaceLabels.insert(dialogTool->GetToolId(), newLabel);

            QPointer<VUndoCommand> saveCommand =
                        new SavePlaceLabelOptions(toolId, currentLabel, newLabel, qApp->getCurrentDocument(),
                                                  const_cast<VContainer *>(data), dialogTool->GetToolId());
            m_undoStack.append(saveCommand);
            UpdateCurrentPlaceLabelRecords();
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
    m_tabPlaceLabels->hide();

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
    switch(index)
    {
        case TabOrder::Paths:
            m_tabPaths->show();
            break;
        case TabOrder::Pins:
            m_tabPins->show();
            break;
        case TabOrder::Labels:
            m_tabLabels->show();
            break;
        case TabOrder::Grainline:
            m_tabGrainline->show();
            break;
        case TabOrder::Passmarks:
            m_tabPassmarks->show();
            break;
        case TabOrder::PlaceLabels:
            m_tabPlaceLabels->show();
            break;
    }
QT_WARNING_POP

    if (index == TabOrder::Pins
            || index == TabOrder::Grainline
            || (index == TabOrder::Labels &&
                uiTabLabels->tabWidget->currentIndex() == uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels)))
    {
        ShowPieceSpecialPointsWithRect(uiTabPins->listWidgetPins, false);
    }
    else if (index == TabOrder::PlaceLabels)
    {
        ShowPieceSpecialPointsWithRect(uiTabPlaceLabels->listWidgetPlaceLabels, true);
    }
    else
    {
        if (not m_visSpecialPoints.isNull())
        {
            delete m_visSpecialPoints;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::TabChanged(int index)
{
    if (index == uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels))
    {
        ShowPieceSpecialPointsWithRect(uiTabPins->listWidgetPins, false);
    }
    else
    {
        if (not m_visSpecialPoints.isNull())
        {
            delete m_visSpecialPoints;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PassmarkLineTypeChanged(int id)
{
    const int i = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (i != -1)
    {
        QListWidgetItem *rowItem = GetItemById(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
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
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonVMark2))
            {
                lineType = PassmarkLineType::VMark2;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonUMark))
            {
                lineType = PassmarkLineType::UMark;
            }
            else if (id == uiTabPassmarks->buttonGroupLineType->id(uiTabPassmarks->radioButtonBoxMark))
            {
                lineType = PassmarkLineType::BoxMark;
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
        QListWidgetItem *rowItem = GetItemById(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
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
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersectionOnlyLeft))
            {
                angleType = PassmarkAngleType::IntersectionOnlyLeft;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersectionOnlyRight))
            {
                angleType = PassmarkAngleType::IntersectionOnlyRight;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersection2))
            {
                angleType = PassmarkAngleType::Intersection2;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersection2OnlyLeft))
            {
                angleType = PassmarkAngleType::Intersection2OnlyLeft;
            }
            else if (id == uiTabPassmarks->buttonGroupAngleType->id(uiTabPassmarks->radioButtonIntersection2OnlyRight))
            {
                angleType = PassmarkAngleType::Intersection2OnlyRight;
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
        QListWidgetItem *rowItem = GetItemById(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
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
            qsUnit = QChar(QChar::Space) + UnitsToStr(qApp->patternUnit());
        }

        plbVal->setToolTip(tr("Value"));

        QString qsFormula = apleSender[i]->toPlainText().simplified();
        QString qsVal;
        try
        {
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->DataVariables(), qsFormula);
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
                ChangeColor(plbText, OkColor(this));
            }
        }
        catch (qmu::QmuParserError &e)
        {
            qsVal = tr("Error");
            not flagGPin ? ChangeColor(plbText, errorColor) : ChangeColor(plbText, OkColor(this));
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
        m_ftb->SetTabText(TabOrder::Grainline, tr("Grainline") + '*');
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
            qsUnit = QChar(QChar::Space) + UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = uiTabLabels->labelDLHeight;
            plbText = uiTabLabels->labelEditDLHeight;
            qsUnit = QChar(QChar::Space) + UnitsToStr(qApp->patternUnit());
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

            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->DataVariables(), qsFormula);
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
                ChangeColor(plbText, OkColor(this));
            }
        }
        catch (qmu::QmuParserError &e)
        {
            qsVal = tr("Error");
            not flagDPin ? ChangeColor(plbText, errorColor) : ChangeColor(plbText, OkColor(this));
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
        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
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
            qsUnit = QChar(QChar::Space) + UnitsToStr(qApp->patternUnit());
        }
        else if (i == 1)
        {
            plbVal = uiTabLabels->labelPLHeight;
            plbText = uiTabLabels->labelEditPLHeight;
            qsUnit = QChar(QChar::Space) + UnitsToStr(qApp->patternUnit());
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
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal;
            qreal dVal = cal.EvalFormula(data->DataVariables(), qsFormula);
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
                ChangeColor(plbText, OkColor(this));
            }
        }
        catch (qmu::QmuParserError &e)
        {
            qsVal = tr("Error");
            not flagPPin ? ChangeColor(plbText, errorColor) : ChangeColor(plbText, OkColor(this));
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
        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
    }
    else
    {
        ResetLabelsWarning();
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnabledGrainline()
{
    if (uiTabGrainline->groupBoxGrainline->isChecked() == true)
    {
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
void DialogSeamAllowance::EnabledManualPassmarkLength()
{
    const int index = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetManualPassmarkLength(uiTabPassmarks->groupBoxManualLength->isChecked());
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
            EvalPassmarkLength();
        }
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
    DeployFormula(this, uiTabGrainline->lineEditRotFormula, uiTabGrainline->pushButtonShowRot, m_iRotBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployGrainlineLength()
{
    DeployFormula(this,uiTabGrainline->lineEditLenFormula, uiTabGrainline->pushButtonShowLen, m_iLenBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLWidth()
{
    DeployFormula(this, uiTabLabels->lineEditDLWidthFormula, uiTabLabels->pushButtonShowDLWidth, m_DLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLHeight()
{
    DeployFormula(this, uiTabLabels->lineEditDLHeightFormula, uiTabLabels->pushButtonShowDLHeight,
                  m_DLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployDLAngle()
{
    DeployFormula(this, uiTabLabels->lineEditDLAngleFormula, uiTabLabels->pushButtonShowDLAngle, m_DLAngleBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLWidth()
{
    DeployFormula(this, uiTabLabels->lineEditPLWidthFormula, uiTabLabels->pushButtonShowPLWidth, m_PLWidthBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLHeight()
{
    DeployFormula(this, uiTabLabels->lineEditPLHeightFormula, uiTabLabels->pushButtonShowPLHeight,
                  m_PLHeightBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPLAngle()
{
    DeployFormula(this, uiTabLabels->lineEditPLAngleFormula, uiTabLabels->pushButtonShowPLAngle, m_PLAngleBaseHeight);
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
    FormulaData formulaData;
    formulaData.formula = uiTabPaths->plainTextEditFormulaWidth->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = uiTabPaths->labelEditWidth;
    formulaData.labelResult = uiTabPaths->labelResultWidth;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
    formulaData.checkZero = false;
    formulaData.checkLessThanZero = true;

    m_saWidth = Eval(formulaData, flagFormula);

    if (m_saWidth >= 0)
    {
        VContainer *locData = const_cast<VContainer *> (data);

        auto currentSA = new VIncrement(locData, currentSeamAllowance);
        currentSA->SetFormula(m_saWidth, QString().setNum(m_saWidth), true);
        currentSA->SetDescription(tr("Current seam allowance"));

        locData->AddVariable(currentSeamAllowance, currentSA);

        EvalWidthBefore();
        EvalWidthAfter();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthBefore()
{
    if (uiTabPaths->checkBoxSeams->isChecked())
    {
        if (uiTabPaths->comboBoxNodes->count() > 0)
        {
            FormulaData formulaData;
            formulaData.formula = uiTabPaths->plainTextEditFormulaWidthBefore->toPlainText();
            formulaData.variables = data->DataVariables();
            formulaData.labelEditFormula = uiTabPaths->labelEditBefore;
            formulaData.labelResult = uiTabPaths->labelResultBefore;
            formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
            formulaData.checkZero = false;
            formulaData.checkLessThanZero = true;

            Eval(formulaData, flagFormulaBefore);

            const QString formulaSABefore = GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthBefore);
            UpdateNodeSABefore(formulaSABefore);
            EnableDefButton(uiTabPaths->pushButtonDefBefore, formulaSABefore);
        }
        else
        {
            ChangeColor(uiTabPaths->labelEditBefore, OkColor(this));
            uiTabPaths->labelResultBefore->setText(tr("<Empty>"));
            flagFormulaBefore = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalWidthAfter()
{
    if (uiTabPaths->checkBoxSeams->isChecked())
    {
        if (uiTabPaths->comboBoxNodes->count() > 0)
        {
            FormulaData formulaData;
            formulaData.formula = uiTabPaths->plainTextEditFormulaWidthAfter->toPlainText();
            formulaData.variables = data->DataVariables();
            formulaData.labelEditFormula = uiTabPaths->labelEditAfter;
            formulaData.labelResult = uiTabPaths->labelResultAfter;
            formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
            formulaData.checkZero = false;
            formulaData.checkLessThanZero = true;

            Eval(formulaData, flagFormulaAfter);

            const QString formulaSAAfter = GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthAfter);
            UpdateNodeSAAfter(formulaSAAfter);
            EnableDefButton(uiTabPaths->pushButtonDefAfter, formulaSAAfter);
        }
        else
        {
            ChangeColor(uiTabPaths->labelEditAfter, OkColor(this));
            uiTabPaths->labelResultAfter->setText(tr("<Empty>"));
            flagFormulaAfter = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EvalPassmarkLength()
{
    if (uiTabPassmarks->groupBoxManualLength->isChecked())
    {
        if (uiTabPassmarks->comboBoxPassmarks->count() > 0)
        {
            FormulaData formulaData;
            formulaData.formula = uiTabPassmarks->plainTextEditPassmarkLength->toPlainText();
            formulaData.variables = data->DataVariables();
            formulaData.labelEditFormula = uiTabPassmarks->labelEditPassmarkLength;
            formulaData.labelResult = uiTabPassmarks->labelResultPassmarkLength;
            formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
            formulaData.checkZero = false;
            formulaData.checkLessThanZero = false;

            Eval(formulaData, flagFormulaPassmarkLength);

            UpdateNodePassmarkLength(GetFormulaFromUser(uiTabPassmarks->plainTextEditPassmarkLength));
        }
        else
        {
            ChangeColor(uiTabPassmarks->labelEditPassmarkLength, OkColor(this));
            uiTabPassmarks->labelResultPassmarkLength->setText(tr("<Empty>"));
            flagFormulaPassmarkLength = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXWidth()
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
void DialogSeamAllowance::FXWidthBefore()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit seam allowance width before"));
    dialog->SetFormula(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthBefore));
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSABefore(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXWidthAfter()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit seam allowance width after"));
    dialog->SetFormula(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidthAfter));
    dialog->setCheckLessThanZero(true);
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCurrentSAAfter(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::FXPassmarkLength()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit passmark length"));
    dialog->SetFormula(GetFormulaFromUser(uiTabPassmarks->plainTextEditPassmarkLength));
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormularPassmarkLength(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthFormulaTextEdit()
{
    DeployFormula(this, uiTabPaths->plainTextEditFormulaWidth, uiTabPaths->pushButtonGrowWidth, m_formulaBaseWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthBeforeFormulaTextEdit()
{
    DeployFormula(this, uiTabPaths->plainTextEditFormulaWidthBefore, uiTabPaths->pushButtonGrowWidthBefore,
                  m_formulaBaseWidthBefore);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployWidthAfterFormulaTextEdit()
{
    DeployFormula(this, uiTabPaths->plainTextEditFormulaWidthAfter, uiTabPaths->pushButtonGrowWidthAfter,
                  m_formulaBaseWidthAfter);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DeployPassmarkLength()
{
    DeployFormula(this, uiTabPassmarks->plainTextEditPassmarkLength, uiTabPassmarks->pushButtonGrowPassmarkLength,
                  m_formulaBasePassmarkLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::GrainlinePinPointChanged()
{
    QColor color;
    const quint32 topPinId = getCurrentObjectId(uiTabGrainline->comboBoxGrainlineTopPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabGrainline->comboBoxGrainlineBottomPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagGPin = true;
        color = OkColor(this);

        ResetGrainlineWarning();
    }
    else
    {
        flagGPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = OkColor(this) : color = errorColor;

        if (not flagGFormulas && not flagGPin)
        {
            m_ftb->SetTabText(TabOrder::Grainline, tr("Grainline"));
        }
    }
    EnableGrainlineFormulaControls(not flagGPin);
    UpdateGrainlineValues();
    ChangeColor(uiTabGrainline->labelGrainlineTopPin, color);
    ChangeColor(uiTabGrainline->labelGrainlineBottomPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::DetailPinPointChanged()
{
    QColor color;
    const quint32 topPinId = getCurrentObjectId(uiTabLabels->comboBoxDLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabLabels->comboBoxDLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagDPin = true;
        color = OkColor(this);

        if (flagPPin)
        {
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), QIcon());
        }
    }
    else
    {
        flagDPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = OkColor(this) : color = errorColor;

        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
    }
    EnableDetailLabelFormulaControls(not flagDPin);
    UpdateDetailLabelValues();
    ChangeColor(uiTabLabels->labelDLTopLeftPin, color);
    ChangeColor(uiTabLabels->labelDLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::PatternPinPointChanged()
{
    QColor color;
    const quint32 topPinId = getCurrentObjectId(uiTabLabels->comboBoxPLTopLeftPin);
    const quint32 bottomPinId = getCurrentObjectId(uiTabLabels->comboBoxPLBottomRightPin);
    if (topPinId != NULL_ID && bottomPinId != NULL_ID && topPinId != bottomPinId)
    {
        flagPPin = true;
        color = OkColor(this);

        if (flagDPin)
        {
            m_ftb->SetTabText(TabOrder::Labels, tr("Labels"));
            uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), QIcon());
        }
    }
    else
    {
        flagPPin = false;
        topPinId == NULL_ID && bottomPinId == NULL_ID ? color = OkColor(this) : color = errorColor;

        m_ftb->SetTabText(TabOrder::Labels, tr("Labels") + '*');
        const QIcon icon = QIcon::fromTheme("dialog-warning",
                                            QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));
        uiTabLabels->tabWidget->setTabIcon(uiTabLabels->tabWidget->indexOf(uiTabLabels->tabLabels), icon);
    }
    EnablePatternLabelFormulaControls(not flagPPin);
    UpdatePatternLabelValues();
    ChangeColor(uiTabLabels->labelPLTopLeftPin, color);
    ChangeColor(uiTabLabels->labelPLBottomRightPin, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EditLabel()
{
    DialogEditLabel editor(qApp->getCurrentDocument());
    editor.SetTemplate(m_templateLines);
    editor.SetPiece(GetPiece());

    if (QDialog::Accepted == editor.exec())
    {
        m_templateLines = editor.GetTemplate();
        uiTabLabels->groupBoxDetailLabel->setEnabled(not m_templateLines.isEmpty());
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiece DialogSeamAllowance::CreatePiece() const
{
    VPiece piece = data->DataPieces()->value(toolId); // Get options we do not control with the dialog
    piece.GetPath().SetNodes(GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath));
    piece.SetCustomSARecords(GetListInternals<CustomSARecord>(uiTabPaths->listWidgetCustomSA));
    piece.SetInternalPaths(GetListInternals<quint32>(uiTabPaths->listWidgetInternalPaths));
    piece.SetPins(GetListInternals<quint32>(uiTabPins->listWidgetPins));
    piece.SetPlaceLabels(GetListInternals<quint32>(uiTabPlaceLabels->listWidgetPlaceLabels));
    piece.SetForbidFlipping(uiTabPaths->checkBoxForbidFlipping->isChecked());
    piece.SetForceFlipping(uiTabPaths->checkBoxForceFlipping->isChecked());
    piece.SetSeamAllowance(uiTabPaths->checkBoxSeams->isChecked());
    piece.SetSeamAllowanceBuiltIn(uiTabPaths->checkBoxBuiltIn->isChecked());
    piece.SetHideMainPath(uiTabPaths->checkBoxHideMainPath->isChecked());
    piece.SetName(uiTabPaths->lineEditName->text());
    piece.SetPriority(static_cast<uint>(uiTabPaths->spinBoxPriority->value()));
    piece.SetFormulaSAWidth(GetFormulaFromUser(uiTabPaths->plainTextEditFormulaWidth), m_saWidth);
    piece.GetPatternPieceData().SetLetter(uiTabLabels->lineEditLetter->text());
    piece.GetPatternPieceData().SetAnnotation(uiTabLabels->lineEditAnnotation->text());
    piece.GetPatternPieceData().SetOrientation(uiTabLabels->lineEditOrientation->text());
    piece.GetPatternPieceData().SetRotationWay(uiTabLabels->lineEditRotation->text());
    piece.GetPatternPieceData().SetTilt(uiTabLabels->lineEditTilt->text());
    piece.GetPatternPieceData().SetFoldPosition(uiTabLabels->lineEditFoldPosition->text());
    piece.GetPatternPieceData().SetQuantity(static_cast<quint16>(uiTabLabels->spinBoxQuantity->value()));
    piece.GetPatternPieceData().SetOnFold(uiTabLabels->checkBoxFold->isChecked());
    piece.GetPatternPieceData().SetLabelTemplate(m_templateLines);
    piece.GetPatternPieceData().SetRotation(GetFormulaFromUser(uiTabLabels->lineEditDLAngleFormula));
    piece.GetPatternPieceData().SetVisible(uiTabLabels->groupBoxDetailLabel->isChecked());

    if (not flagDPin)
    {
        piece.GetPatternPieceData().SetLabelWidth(GetFormulaFromUser(uiTabLabels->lineEditDLWidthFormula));
        piece.GetPatternPieceData().SetLabelHeight(GetFormulaFromUser(uiTabLabels->lineEditDLHeightFormula));
        piece.GetPatternPieceData().SetCenterPin(getCurrentObjectId(uiTabLabels->comboBoxDLCenterPin));
        piece.GetPatternPieceData().SetTopLeftPin(NULL_ID);
        piece.GetPatternPieceData().SetBottomRightPin(NULL_ID);
    }
    else
    {
        piece.GetPatternPieceData().SetLabelWidth(QString::number(1));
        piece.GetPatternPieceData().SetLabelHeight(QString::number(1));
        piece.GetPatternPieceData().SetCenterPin(NULL_ID);
        piece.GetPatternPieceData().SetTopLeftPin(getCurrentObjectId(uiTabLabels->comboBoxDLTopLeftPin));
        piece.GetPatternPieceData().SetBottomRightPin(getCurrentObjectId(uiTabLabels->comboBoxDLBottomRightPin));
    }

    piece.GetPatternInfo().SetVisible(uiTabLabels->groupBoxPatternLabel->isChecked());
    piece.GetPatternInfo().SetRotation(GetFormulaFromUser(uiTabLabels->lineEditPLAngleFormula));

    if (not flagPPin)
    {
        piece.GetPatternInfo().SetCenterPin(getCurrentObjectId(uiTabLabels->comboBoxPLCenterPin));
        piece.GetPatternInfo().SetTopLeftPin(NULL_ID);
        piece.GetPatternInfo().SetBottomRightPin(NULL_ID);
        piece.GetPatternInfo().SetLabelWidth(GetFormulaFromUser(uiTabLabels->lineEditPLWidthFormula));
        piece.GetPatternInfo().SetLabelHeight(GetFormulaFromUser(uiTabLabels->lineEditPLHeightFormula));
    }
    else
    {
        piece.GetPatternInfo().SetCenterPin(NULL_ID);
        piece.GetPatternInfo().SetTopLeftPin(getCurrentObjectId(uiTabLabels->comboBoxPLTopLeftPin));
        piece.GetPatternInfo().SetBottomRightPin(getCurrentObjectId(uiTabLabels->comboBoxPLBottomRightPin));
        piece.GetPatternInfo().SetLabelWidth(QString::number(1));
        piece.GetPatternInfo().SetLabelHeight(QString::number(1));
    }

    piece.GetGrainlineGeometry().SetVisible(uiTabGrainline->groupBoxGrainline->isChecked());
    piece.GetGrainlineGeometry().SetArrowType(static_cast<ArrowType>(uiTabGrainline->comboBoxArrow->currentIndex()));

    if (not flagGPin)
    {
        piece.GetGrainlineGeometry().SetRotation(GetFormulaFromUser(uiTabGrainline->lineEditRotFormula));
        piece.GetGrainlineGeometry().SetLength(GetFormulaFromUser(uiTabGrainline->lineEditLenFormula));
        piece.GetGrainlineGeometry().SetCenterPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineCenterPin));
        piece.GetGrainlineGeometry().SetTopPin(NULL_ID);
        piece.GetGrainlineGeometry().SetBottomPin(NULL_ID);
    }
    else
    {
        piece.GetGrainlineGeometry().SetRotation(QString::number(90));
        piece.GetGrainlineGeometry().SetLength(QChar('1'));
        piece.GetGrainlineGeometry().SetCenterPin(NULL_ID);
        piece.GetGrainlineGeometry().SetTopPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineTopPin));
        piece.GetGrainlineGeometry().SetBottomPin(getCurrentObjectId(uiTabGrainline->comboBoxGrainlineBottomPin));
    }

    return piece;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::NewMainPathItem(const VPieceNode &node)
{
    NewNodeItem(uiTabPaths->listWidgetMainPath, node);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetPathName(quint32 path, bool reverse) const
{
    QString name;

    if (path > NULL_ID)
    {
        name = CurrentPath(path).GetName();

        if (reverse)
        {
            name = QStringLiteral("- ") + name;
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
        if (FirstPointEqualLast(uiTabPaths->listWidgetMainPath, data))
        {
            url += tr("First point cannot be equal to the last point!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
        else if (DoublePoints(uiTabPaths->listWidgetMainPath, data))
        {
            url += tr("You have double points!");
            uiTabPaths->helpLabel->setText(url);
            valid = false;
        }
        else if (DoubleCurves(uiTabPaths->listWidgetMainPath))
        {
            url += tr("The same curve repeats twice!");
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

    return valid;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ValidObjects(bool value)
{
    flagMainPathIsValid = value;
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
    const quint32 id = uiTabPaths->comboBoxNodes->currentData().toUInt();

    uiTabPaths->comboBoxNodes->blockSignals(true);
    uiTabPaths->comboBoxNodes->clear();

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath);

    for (auto &node : nodes)
    {
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
    const quint32 id = uiTabPassmarks->comboBoxPassmarks->currentData().toUInt();

    uiTabPassmarks->comboBoxPassmarks->blockSignals(true);
    uiTabPassmarks->comboBoxPassmarks->clear();

    const QVector<VPieceNode> nodes = GetListInternals<VPieceNode>(uiTabPaths->listWidgetMainPath);

    for (auto &node : nodes)
    {
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
        QListWidgetItem *rowItem = GetItemById(uiTabPaths->comboBoxNodes->currentData().toUInt());
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
        QListWidgetItem *rowItem = GetItemById(uiTabPaths->comboBoxNodes->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetFormulaSAAfter(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::UpdateNodePassmarkLength(const QString &formula)
{
    const int index = uiTabPassmarks->comboBoxPassmarks->currentIndex();
    if (index != -1)
    {
        QListWidgetItem *rowItem = GetItemById(uiTabPassmarks->comboBoxPassmarks->currentData().toUInt());
        if (rowItem)
        {
            VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
            rowNode.SetFormulaPassmarkLength(formula);
            rowItem->setData(Qt::UserRole, QVariant::fromValue(rowNode));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitFancyTabBar()
{
    m_ftb->InsertTab(TabOrder::Paths, QIcon("://icon/32x32/paths.png"), tr("Paths"));
    m_ftb->InsertTab(TabOrder::Pins, QIcon("://icon/32x32/pins.png"), tr("Pins"));
    m_ftb->InsertTab(TabOrder::Labels, QIcon("://icon/32x32/labels.png"), tr("Labels"));
    m_ftb->InsertTab(TabOrder::Grainline, QIcon("://icon/32x32/grainline.png"), tr("Grainline"));
    m_ftb->InsertTab(TabOrder::Passmarks, QIcon("://icon/32x32/passmark.png"), tr("Passmarks"));
    m_ftb->InsertTab(TabOrder::PlaceLabels, QIcon("://icon/32x32/button.png"), tr("Place label"));

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

    m_tabPlaceLabels->hide();
    uiTabPlaceLabels->setupUi(m_tabPlaceLabels);
    ui->horizontalLayout->addWidget(m_tabPlaceLabels, 1);

    connect(m_ftb, &FancyTabBar::CurrentChanged, this, &DialogSeamAllowance::FancyTabChanged);
    connect(uiTabLabels->tabWidget, &QTabWidget::currentChanged, this, &DialogSeamAllowance::TabChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitMainPathTab()
{
    connect(uiTabPaths->lineEditName, &QLineEdit::textChanged, this, &DialogSeamAllowance::NameDetailChanged);

    uiTabPaths->lineEditName->setClearButtonEnabled(true);
    uiTabPaths->lineEditName->setText(GetDefaultPieceName());

    connect(uiTabPaths->checkBoxForbidFlipping, &QCheckBox::stateChanged, this, [this](int state)
    {
        if (state == Qt::Checked)
        {
            uiTabPaths->checkBoxForceFlipping->setChecked(false);
        }
    });

    connect(uiTabPaths->checkBoxForceFlipping, &QCheckBox::stateChanged, this, [this](int state)
    {
        if (state == Qt::Checked)
        {
            uiTabPaths->checkBoxForbidFlipping->setChecked(false);
        }
    });

    uiTabPaths->checkBoxForbidFlipping->setChecked(qApp->Settings()->GetForbidWorkpieceFlipping());
    uiTabPaths->checkBoxForceFlipping->setChecked(qApp->Settings()->GetForceWorkpieceFlipping());
    uiTabPaths->checkBoxHideMainPath->setChecked(qApp->Settings()->IsHideMainPath());

    uiTabPaths->listWidgetMainPath->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPaths->listWidgetMainPath, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowMainPathContextMenu);
    connect(uiTabPaths->listWidgetMainPath->model(), &QAbstractItemModel::rowsMoved, this,
            &DialogSeamAllowance::ListChanged);
    connect(uiTabPaths->listWidgetMainPath, &QListWidget::itemSelectionChanged, this,
            &DialogSeamAllowance::SetMoveControls);

    connect(uiTabPaths->toolButtonTop, &QToolButton::clicked, this,
            [this](){MoveListRowTop(uiTabPaths->listWidgetMainPath);});
    connect(uiTabPaths->toolButtonUp, &QToolButton::clicked, this,
            [this](){MoveListRowUp(uiTabPaths->listWidgetMainPath);});
    connect(uiTabPaths->toolButtonDown, &QToolButton::clicked, this,
            [this](){MoveListRowDown(uiTabPaths->listWidgetMainPath);});
    connect(uiTabPaths->toolButtonBottom, &QToolButton::clicked, this,
            [this](){MoveListRowBottom(uiTabPaths->listWidgetMainPath);});
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitSeamAllowanceTab()
{
    this->m_formulaBaseWidth = uiTabPaths->plainTextEditFormulaWidth->height();
    this->m_formulaBaseWidthBefore = uiTabPaths->plainTextEditFormulaWidthBefore->height();
    this->m_formulaBaseWidthAfter = uiTabPaths->plainTextEditFormulaWidthAfter->height();

    uiTabPaths->plainTextEditFormulaWidth->installEventFilter(this);
    uiTabPaths->plainTextEditFormulaWidthBefore->installEventFilter(this);
    uiTabPaths->plainTextEditFormulaWidthAfter->installEventFilter(this);

    m_timerWidth->setSingleShot(true);
    connect(m_timerWidth, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidth);

    m_timerWidthBefore->setSingleShot(true);
    connect(m_timerWidthBefore, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthBefore);

    m_timerWidthAfter->setSingleShot(true);
    connect(m_timerWidthAfter, &QTimer::timeout, this, &DialogSeamAllowance::EvalWidthAfter);

    connect(uiTabPaths->checkBoxSeams, &QCheckBox::toggled, this, [this](bool enable)
    {
        uiTabPaths->checkBoxBuiltIn->setEnabled(enable);

        if (not enable)
        {
            uiTabPaths->groupBoxAutomatic->setEnabled(enable);
            uiTabPaths->groupBoxCustom->setEnabled(enable);
        }
        else
        {
            uiTabPaths->checkBoxBuiltIn->toggled(uiTabPaths->checkBoxBuiltIn->isChecked());
        }
    });

    connect(uiTabPaths->checkBoxBuiltIn, &QCheckBox::toggled, this, [this](bool enable)
    {
        uiTabPaths->groupBoxAutomatic->setEnabled(not enable);
        uiTabPaths->groupBoxCustom->setEnabled(not enable);

        if (not enable)
        {
            InitNodesList();
        }
    });

    // init the default seam allowance, convert the value if app unit is different than pattern unit
    m_saWidth = UnitConvertor(qApp->Settings()->GetDefaultSeamAllowance(),
                              StrToUnits(qApp->Settings()->GetUnit()), qApp->patternUnit());

    uiTabPaths->plainTextEditFormulaWidth->setPlainText(qApp->LocaleToString(m_saWidth));

    InitNodesList();
    connect(uiTabPaths->comboBoxNodes, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeChanged);

    connect(uiTabPaths->pushButtonDefBefore, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefBefore);
    connect(uiTabPaths->pushButtonDefAfter, &QPushButton::clicked, this, &DialogSeamAllowance::ReturnDefAfter);

    InitNodeAngles(uiTabPaths->comboBoxAngle);
    connect(uiTabPaths->comboBoxAngle, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::NodeAngleChanged);

    uiTabPaths->listWidgetCustomSA->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPaths->listWidgetCustomSA, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowCustomSAContextMenu);
    connect(uiTabPaths->listWidgetCustomSA, &QListWidget::currentRowChanged, this,
            &DialogSeamAllowance::CustomSAChanged);
    connect(uiTabPaths->comboBoxStartPoint, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::CSAStartPointChanged);
    connect(uiTabPaths->comboBoxEndPoint, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogSeamAllowance::CSAEndPointChanged);
    connect(uiTabPaths->comboBoxIncludeType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::CSAIncludeTypeChanged);

    connect(uiTabPaths->toolButtonExprWidth, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidth);
    connect(uiTabPaths->toolButtonExprBefore, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthBefore);
    connect(uiTabPaths->toolButtonExprAfter, &QPushButton::clicked, this, &DialogSeamAllowance::FXWidthAfter);

    connect(uiTabPaths->plainTextEditFormulaWidth, &QPlainTextEdit::textChanged, this, [this]()
    {
        m_timerWidth->start(formulaTimerTimeout);
    });

    connect(uiTabPaths->plainTextEditFormulaWidthBefore, &QPlainTextEdit::textChanged, this, [this]()
    {
        m_timerWidthBefore->start(formulaTimerTimeout);
    });

    connect(uiTabPaths->plainTextEditFormulaWidthAfter, &QPlainTextEdit::textChanged, this, [this]()
    {
        m_timerWidthAfter->start(formulaTimerTimeout);
    });

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

    for (auto &node : nodes)
    {
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
        currentId = box->currentData().toUInt();
    }

    box->clear();
    box->addItem('<' + tr("no pin") + '>', NULL_ID);

    const QVector<quint32> pins = GetListInternals<quint32>(uiTabPins->listWidgetPins);

    for (auto pin : pins)
    {
        box->addItem(data->GetGObject(pin)->name(), pin);
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
    uiTabLabels->lineEditLetter->setClearButtonEnabled(true);
    uiTabLabels->lineEditAnnotation->setClearButtonEnabled(true);
    uiTabLabels->lineEditOrientation->setClearButtonEnabled(true);
    uiTabLabels->lineEditRotation->setClearButtonEnabled(true);
    uiTabLabels->lineEditTilt->setClearButtonEnabled(true);
    uiTabLabels->lineEditFoldPosition->setClearButtonEnabled(true);

    connect(uiTabLabels->pushButtonEditPieceLabel, &QPushButton::clicked, this, &DialogSeamAllowance::EditLabel);
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

    connect(uiTabLabels->comboBoxDLTopLeftPin, &QComboBox::currentTextChanged,
            this, &DialogSeamAllowance::DetailPinPointChanged);
    connect(uiTabLabels->comboBoxDLBottomRightPin, &QComboBox::currentTextChanged,
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

    connect(uiTabLabels->comboBoxPLTopLeftPin, &QComboBox::currentTextChanged,
            this, &DialogSeamAllowance::PatternPinPointChanged);
    connect(uiTabLabels->comboBoxPLBottomRightPin, &QComboBox::currentTextChanged,
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

    connect(uiTabGrainline->comboBoxGrainlineTopPin, &QComboBox::currentTextChanged,
            this, &DialogSeamAllowance::GrainlinePinPointChanged);
    connect(uiTabGrainline->comboBoxGrainlineBottomPin, &QComboBox::currentTextChanged,
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
    this->m_formulaBasePassmarkLength = uiTabPassmarks->plainTextEditPassmarkLength->height();
    uiTabPassmarks->plainTextEditPassmarkLength->installEventFilter(this);

    m_timerPassmarkLength->setSingleShot(true);
    connect(m_timerPassmarkLength, &QTimer::timeout, this, &DialogSeamAllowance::EvalPassmarkLength);

    connect(uiTabPassmarks->groupBoxManualLength, &QGroupBox::toggled, this,
            &DialogSeamAllowance::EnabledManualPassmarkLength);

    InitPassmarksList();
    connect(uiTabPassmarks->comboBoxPassmarks, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogSeamAllowance::PassmarkChanged);

    connect(uiTabPassmarks->buttonGroupLineType, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &DialogSeamAllowance::PassmarkLineTypeChanged);
    connect(uiTabPassmarks->buttonGroupAngleType, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &DialogSeamAllowance::PassmarkAngleTypeChanged);
    connect(uiTabPassmarks->checkBoxShowSecondPassmark, &QCheckBox::stateChanged, this,
            &DialogSeamAllowance::PassmarkShowSecondChanged);
    connect(uiTabPassmarks->toolButtonExprLength, &QPushButton::clicked, this, &DialogSeamAllowance::FXPassmarkLength);

    connect(uiTabPassmarks->plainTextEditPassmarkLength, &QPlainTextEdit::textChanged, this, [this]()
    {
        m_timerPassmarkLength->start(formulaTimerTimeout);
    });

    connect(uiTabPassmarks->pushButtonGrowPassmarkLength, &QPushButton::clicked, this,
            &DialogSeamAllowance::DeployPassmarkLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::InitPlaceLabelsTab()
{
    uiTabPlaceLabels->listWidgetPlaceLabels->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(uiTabPlaceLabels->listWidgetPlaceLabels, &QListWidget::currentRowChanged, this, [this]()
    {
        if (not m_visSpecialPoints.isNull())
        {
            m_visSpecialPoints->SetShowRect(true);
            m_visSpecialPoints->SetRect(CurrentRect());
            m_visSpecialPoints->RefreshGeometry();
        }
    });
    connect(uiTabPlaceLabels->listWidgetPlaceLabels, &QListWidget::customContextMenuRequested, this,
            &DialogSeamAllowance::ShowPlaceLabelsContextMenu);
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
    return qApp->TrVars()->TryFormulaFromUser(width, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointer<VUndoCommand>> &DialogSeamAllowance::UndoStack()
{
    return m_undoStack;
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

    if (not applyAllowed)
    {
        VisToolPiece *path = qobject_cast<VisToolPiece *>(vis);
        SCASSERT(path != nullptr)
        const VPiece p = CreatePiece();
        path->SetPiece(p);
    }

    MoveCursorToEnd(uiTabPaths->plainTextEditFormulaWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetFormularPassmarkLength(const QString &formula)
{
    const QString width = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (width.length() > 80)
    {
        this->DeployPassmarkLength();
    }
    uiTabPassmarks->plainTextEditPassmarkLength->setPlainText(width);

    MoveCursorToEnd(uiTabPassmarks->plainTextEditPassmarkLength);
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
void DialogSeamAllowance::UpdateCurrentPlaceLabelRecords()
{
    const int row = uiTabPlaceLabels->listWidgetPlaceLabels->currentRow();
    if (uiTabPlaceLabels->listWidgetPlaceLabels->count() == 0 || row == -1)
    {
        return;
    }

    QListWidgetItem *item = uiTabPlaceLabels->listWidgetPlaceLabels->item(row);
    SCASSERT(item != nullptr);
    const quint32 labelId = qvariant_cast<quint32>(item->data(Qt::UserRole));
    item->setText(CurrentPlaceLabel(labelId).name());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetGrainlineAngle(QString angleFormula)
{
    if (angleFormula.isEmpty())
    {
        angleFormula = '0';
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
        angleFormula = '0';
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
        angleFormula = '0';
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
QRectF DialogSeamAllowance::CurrentRect() const
{
    QRectF rect;
    if (QListWidgetItem *item = uiTabPlaceLabels->listWidgetPlaceLabels->currentItem())
    {
        VPlaceLabelItem label = CurrentPlaceLabel(qvariant_cast<quint32>(item->data(Qt::UserRole)));
        rect = QRectF(QPointF(label.x() - label.GetWidth()/2.0, label.y() - label.GetHeight()/2.0),
                      QPointF(label.x() + label.GetWidth()/2.0, label.y() + label.GetHeight()/2.0));
    }
    return rect;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::ShowPieceSpecialPointsWithRect(const QListWidget *list, bool showRect)
{
    SCASSERT(list != nullptr)
    if (m_visSpecialPoints.isNull())
    {
        m_visSpecialPoints = new VisPieceSpecialPoints(data);
    }

    m_visSpecialPoints->SetPoints(GetListInternals<quint32>(list));
    m_visSpecialPoints->SetShowRect(showRect);
    m_visSpecialPoints->SetRect(CurrentRect());

    if (not qApp->getCurrentScene()->items().contains(m_visSpecialPoints))
    {
        m_visSpecialPoints->VisualMode(NULL_ID);
        m_visSpecialPoints->setZValue(10); // pins should be on top
        VToolSeamAllowance *tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(toolId));
        SCASSERT(tool != nullptr);
        m_visSpecialPoints->setParentItem(tool);
    }
    else
    {
        m_visSpecialPoints->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath DialogSeamAllowance::CurrentPath(quint32 id) const
{
    return m_newPaths.contains(id) ? m_newPaths.value(id) : data->GetPiecePath(id);
}

//---------------------------------------------------------------------------------------------------------------------
VPlaceLabelItem DialogSeamAllowance::CurrentPlaceLabel(quint32 id) const
{
    return m_newPlaceLabels.contains(id) ? m_newPlaceLabels.value(id) : *data->GeometricObject<VPlaceLabelItem>(id);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSeamAllowance::GetDefaultPieceName() const
{
    QList<VPiece> pieces = data->DataPieces()->values();
    QSet<QString> names;

    for (auto &piece : pieces)
    {
        names.insert(piece.GetName());
    }

    const QString defName = tr("Detail");
    QString name = defName;
    int i = 0;

    while(names.contains(name))
    {
        name = defName + QStringLiteral("_%1").arg(++i);
    }
    return name;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableGrainlineFormulaControls(bool enable)
{
    uiTabGrainline->pushButtonRot->setEnabled(enable);
    uiTabGrainline->lineEditRotFormula->setEnabled(enable);
    uiTabGrainline->pushButtonShowRot->setEnabled(enable);

    uiTabGrainline->pushButtonLen->setEnabled(enable);
    uiTabGrainline->lineEditLenFormula->setEnabled(enable);
    uiTabGrainline->pushButtonShowLen->setEnabled(enable);

    uiTabGrainline->comboBoxGrainlineCenterPin->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnableDetailLabelFormulaControls(bool enable)
{
    uiTabLabels->pushButtonDLWidth->setEnabled(enable);
    uiTabLabels->lineEditDLWidthFormula->setEnabled(enable);
    uiTabLabels->pushButtonShowDLWidth->setEnabled(enable);

    uiTabLabels->pushButtonDLHeight->setEnabled(enable);
    uiTabLabels->lineEditDLHeightFormula->setEnabled(enable);
    uiTabLabels->pushButtonShowDLHeight->setEnabled(enable);

    uiTabLabels->comboBoxDLCenterPin->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::EnablePatternLabelFormulaControls(bool enable)
{
    uiTabLabels->pushButtonPLWidth->setEnabled(enable);
    uiTabLabels->lineEditPLWidthFormula->setEnabled(enable);
    uiTabLabels->pushButtonShowPLWidth->setEnabled(enable);

    uiTabLabels->pushButtonPLHeight->setEnabled(enable);
    uiTabLabels->lineEditPLHeightFormula->setEnabled(enable);
    uiTabLabels->pushButtonShowPLHeight->setEnabled(enable);

    uiTabLabels->comboBoxPLCenterPin->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSeamAllowance::SetMoveControls()
{
    uiTabPaths->toolButtonTop->setEnabled(false);
    uiTabPaths->toolButtonUp->setEnabled(false);
    uiTabPaths->toolButtonDown->setEnabled(false);
    uiTabPaths->toolButtonBottom->setEnabled(false);

    if (uiTabPaths->listWidgetMainPath->count() >= 2)
    {
        if (uiTabPaths->listWidgetMainPath->currentRow() == 0)
        {
            uiTabPaths->toolButtonDown->setEnabled(true);
            uiTabPaths->toolButtonBottom->setEnabled(true);
        }
        else if (uiTabPaths->listWidgetMainPath->currentRow() == uiTabPaths->listWidgetMainPath->count()-1)
        {
            uiTabPaths->toolButtonTop->setEnabled(true);
            uiTabPaths->toolButtonUp->setEnabled(true);
        }
        else
        {
            uiTabPaths->toolButtonTop->setEnabled(true);
            uiTabPaths->toolButtonUp->setEnabled(true);
            uiTabPaths->toolButtonDown->setEnabled(true);
            uiTabPaths->toolButtonBottom->setEnabled(true);
        }
    }
}
