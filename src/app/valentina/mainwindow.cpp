/************************************************************************
 **
 **  @file   mainwindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../vgeometry/vspline.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../ifc/exception/vexceptionundo.h"
#include "version.h"
#include "core/vapplication.h"
#include "../vmisc/customevents.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/def.h"
#include "../vmisc/qxtcsvmodel.h"
#include "../vmisc/vmodifierkey.h"
#include "undocommands/renamepp.h"
#include "core/vtooloptionspropertybrowser.h"
#include "options.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vformat/vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"
#include "../vwidgets/vwidgetpopup.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "tools/drawTools/drawtools.h"
#include "../vtools/dialogs/tooldialogs.h"
#include "tools/vtoolseamallowance.h"
#include "tools/nodeDetails/vtoolpiecepath.h"
#include "tools/nodeDetails/vtoolpin.h"
#include "tools/nodeDetails/vtoolplacelabel.h"
#include "tools/vtooluniondetails.h"
#include "dialogs/dialogs.h"
#include "dialogs/vwidgetgroups.h"
#include "../vtools/undocommands/addgroup.h"
#include "dialogs/vwidgetdetails.h"
#include "../vpatterndb/vpiecepath.h"
#include "../qmuparser/qmuparsererror.h"
#include "../vtools/dialogs/support/dialogeditlabel.h"
#include "../vformat/vpatternrecipe.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#include "../vmisc/backport/qscopeguard.h"
#else
#include <QScopeGuard>
#endif

#include <QInputDialog>
#include <QtDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QSourceLocation>
#include <QUndoStack>
#include <QAction>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QtGlobal>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <chrono>
#include <thread>
#include <QFileSystemWatcher>
#include <QComboBox>
#include <QTextCodec>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QGlobalStatic>
#include <QFuture>
#include <QtConcurrent>

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

#if defined(Q_OS_MAC)
#include <QMimeData>
#include <QDrag>
#endif //defined(Q_OS_MAC)

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vMainWindow, "v.mainwindow")

QT_WARNING_POP

namespace
{
Q_GLOBAL_STATIC_WITH_ARGS(const QString, autosavePrefix, (QLatin1String(".autosave")))

//---------------------------------------------------------------------------------------------------------------------
QVector<DetailForLayout> SortDetailsForLayout(const QHash<quint32, VPiece> *allDetails,
                                              const QString &nameRegex = QString())
{
    QVector<DetailForLayout> details;
    QHash<quint32, VPiece>::const_iterator i = allDetails->constBegin();

    if (nameRegex.isEmpty())
    {
        while (i != allDetails->constEnd())
        {
            if (i.value().IsInLayout())
            {
                details.append(DetailForLayout(i.key(), i.value()));
            }

            ++i;
        }
    }
    else
    {
        QRegularExpression nameRe(nameRegex);
        while (i != allDetails->constEnd())
        {
            if (nameRe.match(i.value().GetName()).hasMatch())
            {
                details.append(DetailForLayout(i.key(), i.value()));
            }

            ++i;
        }
    }

    return details;
}

} // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindow constructor.
 * @param parent parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    :MainWindowsNoGUI(parent), ui(new Ui::MainWindow), watcher(new QFileSystemWatcher(this)), currentTool(Tool::Arrow),
      lastUsedTool(Tool::Arrow), sceneDraw(nullptr), sceneDetails(nullptr),
      isInitialized(false), mChanges(false), mChangesAsked(true),
      patternReadOnly(false),
      dialogTable(nullptr),
      dialogTool(),
      dialogHistory(nullptr),
      dialogFMeasurements(nullptr),
      comboBoxDraws(nullptr), patternPieceLabel(nullptr),
      currentDrawIndex(0), currentToolBoxIndex(0),
      isDockToolOptionsVisible(true),
      isDockGroupsVisible(true),
      drawMode(true),
      leftGoToStage(nullptr), rightGoToStage(nullptr), autoSaveTimer(nullptr), guiEnabled(true),
      gradationHeights(nullptr),
      gradationSizes(nullptr),
      gradationHeightsLabel(nullptr),
      gradationSizesLabel(nullptr),
      zoomScale(nullptr),
      doubleSpinBoxScale(nullptr),
      toolOptions(nullptr),
      groupsWidget(nullptr),
      detailsWidget(nullptr),
      lock(nullptr),
      toolButtonPointerList(),
      m_progressBar(new QProgressBar(this)),
      m_statusLabel(new QLabel(this))
{
    CreateActions();
    InitScenes();

    doc = new VPattern(pattern, sceneDraw, sceneDetails);
    connect(doc, &VPattern::ClearMainWindow, this, &MainWindow::Clear);
    connect(doc, &VPattern::patternChanged, this, &MainWindow::PatternChangesWereSaved);
    connect(doc, &VPattern::UndoCommand, this, &MainWindow::FullParseFile);
    connect(doc, &VPattern::SetEnabledGUI, this, &MainWindow::SetEnabledGUI);
    connect(doc, &VPattern::CheckLayout, this, [this]()
    {
        if (pattern->DataPieces()->count() == 0)
        {
            if(not ui->actionDraw->isChecked())
            {
                ActionDraw(true);
            }
        }
    });
    connect(doc, &VPattern::SetCurrentPP, this, &MainWindow::GlobalChangePP);
    connect(doc, &VPattern::MadeProgress, this, &MainWindow::ShowProgress);
    qApp->setCurrentDocument(doc);

    InitDocksContain();
    CreateMenus();
    ToolBarDraws();
    ToolBarStages();
    InitToolButtons();

    m_progressBar->setVisible(false);
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarProgress->setVisible(false);
#endif
    m_statusLabel->setText(tr("Create new pattern piece to start working."));
    statusBar()->addPermanentWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_progressBar, 1);

    ToolBarTools();

    connect(qApp->getUndoStack(), &QUndoStack::cleanChanged, this, &MainWindow::PatternChangesWereSaved);

    InitAutoSave();

    ui->toolBox->setCurrentIndex(0);

    ReadSettings();

    setCurrentFile(QString());
    WindowsLocale();

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::ShowPaper);
    ui->dockWidgetLayoutPages->setVisible(false);

    connect(watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::MeasurementsChanged);
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget *old, QWidget *now)
    {
        if (old == nullptr && isAncestorOf(now) == true)
        {// focus IN
            static bool asking = false;
            if (not asking && mChanges && not mChangesAsked)
            {
                asking = true;
                mChangesAsked = true;
                const auto answer = QMessageBox::question(this, tr("Measurements"),
                                                 tr("Measurements were changed. Do you want to sync measurements now?"),
                                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
                if (answer == QMessageBox::Yes)
                {
                    SyncMeasurements();
                }
                asking = false;
            }
        }

        // In case we will need it
        // else if (isAncestorOf(old) == true && now == nullptr)
        // focus OUT
    });

#if defined(Q_OS_MAC)
    // On Mac deafault icon size is 32x32.
    ui->toolBarDraws->setIconSize(QSize(24, 24));
    ui->toolBarOption->setIconSize(QSize(24, 24));
    ui->toolBarStages->setIconSize(QSize(24, 24));
    ui->toolBarTools->setIconSize(QSize(24, 24));

    setUnifiedTitleAndToolBarOnMac(true);

    // Mac OS Dock Menu
    QMenu *menu = new QMenu(this);

    QAction *actionNewPattern = menu->addAction(tr("New pattern"));
    actionNewPattern->setMenuRole(QAction::NoRole);
    connect(actionNewPattern, &QAction::triggered, this, &MainWindow::on_actionNew_triggered);

    QAction *actionOpenPattern = menu->addAction(tr("Open pattern"));
    actionOpenPattern->setMenuRole(QAction::NoRole);
    connect(actionOpenPattern, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);

    QAction *actionOpenTape = menu->addAction(tr("Create/Edit measurements"));
    actionOpenTape->setMenuRole(QAction::NoRole);
    connect(actionOpenTape, &QAction::triggered, this, &MainWindow::CreateMeasurements);

    QAction *actionPreferences = menu->addAction(tr("Preferences"));
    actionPreferences->setMenuRole(QAction::NoRole);
    connect(actionPreferences, &QAction::triggered, this, &MainWindow::Preferences);

    menu->setAsDockMenu();
#endif //defined(Q_OS_MAC)
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddPP(const QString &PPName)
{
    if (doc->appendPP(PPName) == false)
    {
        qCDebug(vMainWindow, "Error creating pattern piece with the name %s.", qUtf8Printable(PPName));
        return;
    }

    if (comboBoxDraws->count() == 0)
    {
        sceneDraw->InitOrigins();
        sceneDetails->InitOrigins();
    }

    comboBoxDraws->blockSignals(true);
    comboBoxDraws->addItem(PPName);

    pattern->ClearCalculationGObjects();
    //Create single point
    emit ui->view->itemClicked(nullptr);//hide options previous tool
    const QString label = doc->GenerateLabel(LabelType::NewPatternPiece);
    const QPointF startPosition = StartPositionNewPP();

    VToolBasePointInitData initData;
    initData.scene = sceneDraw;
    initData.doc = doc;
    initData.data = pattern;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;
    initData.x = startPosition.x();
    initData.y = startPosition.y();
    initData.name = label;
    initData.nameActivPP = PPName;

    auto spoint = VToolBasePoint::Create(initData);
    emit ui->view->itemClicked(spoint);

    SetEnableTool(true);
    SetEnableWidgets(true);

    const qint32 index = comboBoxDraws->findText(PPName);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
    }
    else
    {
        comboBoxDraws->setCurrentIndex(0);
    }
    comboBoxDraws->blockSignals(false);

    // Show best for new PP
    VMainGraphicsView::NewSceneRect(ui->view->scene(), ui->view, spoint);
    //ZoomFitBestCurrent();
    QApplication::postEvent(this, new FitBestCurrentEvent());

    ui->actionNewDraw->setEnabled(true);
    m_statusLabel->setText(QString());
}

//---------------------------------------------------------------------------------------------------------------------
QPointF MainWindow::StartPositionNewPP() const
{
    const qreal originX = 30.0;
    const qreal originY = 40.0;
    const qreal margin = 40.0;
    if (comboBoxDraws->count() > 1)
    {
        const QRectF rect = sceneDraw->VisibleItemsBoundingRect();
        if (rect.width() <= rect.height())
        {
            return QPointF(rect.width()+margin, originY);
        }
        else
        {
            return QPointF(originX, rect.height()+margin);
        }
    }
    else
    {
        return QPointF(originX, originY);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitScenes()
{
    sceneDraw = new VMainGraphicsScene(this);
    currentScene = sceneDraw;
    qApp->setCurrentScene(&currentScene);
    connect(this, &MainWindow::EnableItemMove, sceneDraw, &VMainGraphicsScene::EnableItemMove);
    connect(this, &MainWindow::ItemsSelection, sceneDraw, &VMainGraphicsScene::ItemsSelection);

    connect(this, &MainWindow::EnableLabelSelection, sceneDraw, &VMainGraphicsScene::ToggleLabelSelection);
    connect(this, &MainWindow::EnablePointSelection, sceneDraw, &VMainGraphicsScene::TogglePointSelection);
    connect(this, &MainWindow::EnableLineSelection, sceneDraw, &VMainGraphicsScene::ToggleLineSelection);
    connect(this, &MainWindow::EnableArcSelection, sceneDraw, &VMainGraphicsScene::ToggleArcSelection);
    connect(this, &MainWindow::EnableElArcSelection, sceneDraw, &VMainGraphicsScene::ToggleElArcSelection);
    connect(this, &MainWindow::EnableSplineSelection, sceneDraw, &VMainGraphicsScene::ToggleSplineSelection);
    connect(this, &MainWindow::EnableSplinePathSelection, sceneDraw, &VMainGraphicsScene::ToggleSplinePathSelection);

    connect(this, &MainWindow::EnableLabelHover, sceneDraw, &VMainGraphicsScene::ToggleLabelHover);
    connect(this, &MainWindow::EnablePointHover, sceneDraw, &VMainGraphicsScene::TogglePointHover);
    connect(this, &MainWindow::EnableLineHover, sceneDraw, &VMainGraphicsScene::ToggleLineHover);
    connect(this, &MainWindow::EnableArcHover, sceneDraw, &VMainGraphicsScene::ToggleArcHover);
    connect(this, &MainWindow::EnableElArcHover, sceneDraw, &VMainGraphicsScene::ToggleElArcHover);
    connect(this, &MainWindow::EnableSplineHover, sceneDraw, &VMainGraphicsScene::ToggleSplineHover);
    connect(this, &MainWindow::EnableSplinePathHover, sceneDraw, &VMainGraphicsScene::ToggleSplinePathHover);

    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

    sceneDetails = new VMainGraphicsScene(this);
    connect(this, &MainWindow::EnableItemMove, sceneDetails, &VMainGraphicsScene::EnableItemMove);

    connect(this, &MainWindow::EnableNodeLabelSelection, sceneDetails, &VMainGraphicsScene::ToggleNodeLabelSelection);
    connect(this, &MainWindow::EnableNodePointSelection, sceneDetails, &VMainGraphicsScene::ToggleNodePointSelection);
    connect(this, &MainWindow::EnableDetailSelection, sceneDetails, &VMainGraphicsScene::ToggleDetailSelection);

    connect(this, &MainWindow::EnableNodeLabelHover, sceneDetails, &VMainGraphicsScene::ToggleNodeLabelHover);
    connect(this, &MainWindow::EnableNodePointHover, sceneDetails, &VMainGraphicsScene::ToggleNodePointHover);
    connect(this, &MainWindow::EnableDetailHover, sceneDetails, &VMainGraphicsScene::ToggleDetailHover);

    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

    ui->view->setScene(currentScene);

    sceneDraw->setTransform(ui->view->transform());
    sceneDetails->setTransform(ui->view->transform());

    connect(ui->view, &VMainGraphicsView::MouseRelease, this, [this](){EndVisualization(true);});
    connect(ui->view, &VMainGraphicsView::ScaleChanged, this, &MainWindow::ScaleChanged);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    ui->view->setSizePolicy(policy);
    qApp->setSceneView(ui->view);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::LoadMeasurements(const QString &path)
{
    QSharedPointer<VMeasurements> m = OpenMeasurementFile(path);

    if (m->isNull())
    {
        return false;
    }

    if (qApp->patternUnit() == Unit::Inch && m->Type() == MeasurementsType::Multisize)
    {
        qWarning()<<tr("Gradation doesn't support inches");
        return false;
    }

    const qreal size = UnitConvertor(m->BaseSize(), m->MUnit(), *m->GetData()->GetPatternUnit());
    const qreal height = UnitConvertor(m->BaseHeight(), m->MUnit(), *m->GetData()->GetPatternUnit());

    try
    {
        qApp->setPatternType(m->Type());
        if (m->Type() == MeasurementsType::Individual)
        {
            qApp->SetCustomerName(m->Customer());
        }
        ToolBarOption();
        pattern->ClearVariables(VarType::Measurement);
        m->ReadMeasurements(height, size);
    }
    catch (VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    if (m->Type() == MeasurementsType::Multisize)
    {
        pattern->SetSize(size);
        pattern->SetHeight(height);

        doc->SetPatternWasChanged(true);
        emit doc->UpdatePatternLabel();
    }
    else if (m->Type() == MeasurementsType::Individual)
    {
        SetSizeHeightForIndividualM();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::UpdateMeasurements(const QString &path, int size, int height)
{
    QSharedPointer<VMeasurements> m = OpenMeasurementFile(path);

    if (m->isNull())
    {
        return false;
    }

    if (qApp->patternType() != m->Type())
    {
        qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Measurement files types have not match.")));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return false;
    }

    try
    {
        pattern->ClearVariables(VarType::Measurement);
        m->ReadMeasurements(height, size);
        if (m->Type() == MeasurementsType::Individual)
        {
            qApp->SetCustomerName(m->Customer());
        }
    }
    catch (VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    if (m->Type() == MeasurementsType::Multisize)
    {
        pattern->SetSize(size);
        pattern->SetHeight(height);

        doc->SetPatternWasChanged(true);
        emit doc->UpdatePatternLabel();
    }
    else if (m->Type() == MeasurementsType::Individual)
    {
        SetSizeHeightForIndividualM();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetToolButton set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function what handle after close dialog.
 */
template <typename Dialog, typename Func>
void MainWindow::SetToolButton(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                               Func closeDialogSlot)
{
    if (checked)
    {
        CancelTool();
        emit EnableItemMove(false);
        currentTool = lastUsedTool = t;
        auto cursorResource = cursor;
        if (qApp->devicePixelRatio() >= 2)
        {
            // Try to load HiDPI versions of the cursors if availible
            auto cursorHidpiResource = QString(cursor).replace(QLatin1String(".png"), QLatin1String("@2x.png"));
            if (QFileInfo::exists(cursorResource))
            {
                cursorResource = cursorHidpiResource;
            }
        }
        QPixmap pixmap(cursorResource);
        QCursor cur(pixmap, 2, 2);
        ui->view->viewport()->setCursor(cur);
        ui->view->setCurrentCursorShape(); // Hack to fix problem with a cursor
        m_statusLabel->setText(toolTip);
        ui->view->setShowToolOptions(false);
        dialogTool = new Dialog(pattern, 0, this);

        // This check helps to find missed tools in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Check if need to extend.");

        switch(t)
        {
            case Tool::Midpoint:
                dialogTool->Build(t);
                break;
            case Tool::PiecePath:
            case Tool::Pin:
            case Tool::InsertNode:
            case Tool::PlaceLabel:
                dialogTool->SetPiecesList(doc->GetActivePPPieces());
                break;
            default:
                break;
        }

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr)

        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogTool.data(), &DialogTool::ChosenObject);
        connect(scene, &VMainGraphicsScene::SelectedObject, dialogTool.data(), &DialogTool::SelectedObject);
        connect(dialogTool.data(), &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool.data(), &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        emit ui->view->itemClicked(nullptr);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            tButton->setChecked(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Dialog, typename Func, typename Func2>
/**
 * @brief SetToolButtonWithApply set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function to handle close of dialog.
 * @param applyDialogSlot function to handle apply in dialog.
 */
void MainWindow::SetToolButtonWithApply(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                                        Func closeDialogSlot, Func2 applyDialogSlot)
{
    if (checked)
    {
        SetToolButton<Dialog>(checked, t, cursor, toolTip, closeDialogSlot);

        connect(dialogTool.data(), &DialogTool::DialogApplied, this, applyDialogSlot);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            tButton->setChecked(true);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialog handle close dialog
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialog(int result)
{
    SCASSERT(not dialogTool.isNull())
    if (result == QDialog::Accepted)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr)

        QGraphicsItem *tool = dynamic_cast<QGraphicsItem *>(DrawTool::Create(dialogTool, scene, doc, pattern));
        // Do not check for nullptr! See issue #719.
        emit ui->view->itemClicked(tool);
    }
    ArrowTool(true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogWithApply handle close dialog that has apply button
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialogWithApply(int result, VMainGraphicsScene *scene)
{
    SCASSERT(not dialogTool.isNull())
    if (result == QDialog::Accepted)
    {
        ApplyDialog<DrawTool>(scene);
    }
    // If before Cancel was used Apply we have an item
    DrawTool *vtool = qobject_cast<DrawTool *>(dialogTool->GetAssociatedTool());// Don't check for nullptr here
    if (dialogTool->GetAssociatedTool() != nullptr)
    {
        SCASSERT(vtool != nullptr)
        vtool->DialogLinkDestroy();
        connect(vtool, &DrawTool::ToolTip, this, &MainWindow::ShowToolTip);
    }
    ArrowTool(true);
    ui->view->itemClicked(vtool);// Don't check for nullptr here
    if (vtool)
    {
       vtool->setFocus();
    }
    // If insert not to the end of file call lite parse
    if (doc->getCursor() > 0)
    {
        doc->LiteParseTree(Document::LiteParse);
        if (dialogHistory)
        {
            dialogHistory->UpdateHistory();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ApplyDialog handle apply in dialog
 */
template <typename DrawTool>
void MainWindow::ApplyDialog(VMainGraphicsScene *scene)
{
    SCASSERT(not dialogTool.isNull())

    // Only create tool if not already created with apply
    if (dialogTool->GetAssociatedTool() == nullptr)
    {
        SCASSERT(scene != nullptr)

        dialogTool->SetAssociatedTool(DrawTool::Create(dialogTool, scene, doc, pattern));
    }
    else
    { // Or update associated tool with data
        DrawTool * vtool = qobject_cast<DrawTool *>(dialogTool->GetAssociatedTool());
        SCASSERT(vtool != nullptr)
        vtool->FullUpdateFromGuiApply();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ClosedDrawDialogWithApply(int result)
{
    ClosedDialogWithApply<DrawTool>(result, sceneDraw);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ApplyDrawDialog()
{
    ApplyDialog<DrawTool>(sceneDraw);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ClosedDetailsDialogWithApply(int result)
{
    ClosedDialogWithApply<DrawTool>(result, sceneDetails);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ApplyDetailsDialog()
{
    ApplyDialog<DrawTool>(sceneDetails);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolEndLine handler tool endLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolEndLine(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogEndLine>(checked, Tool::EndLine, QStringLiteral(":/cursor/endline_cursor.png"),
                                          tr("Select point"),
                                          &MainWindow::ClosedDrawDialogWithApply<VToolEndLine>,
                                          &MainWindow::ApplyDrawDialog<VToolEndLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLine handler tool line.
 * @param checked true - button checked.
 */
void MainWindow::ToolLine(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLine>(checked, Tool::Line, QStringLiteral(":/cursor/line_cursor.png"),
                                       tr("Select first point"),
                                       &MainWindow::ClosedDrawDialogWithApply<VToolLine>,
                                       &MainWindow::ApplyDrawDialog<VToolLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolAlongLine handler tool alongLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolAlongLine(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogAlongLine>(checked, Tool::AlongLine, QStringLiteral(":/cursor/alongline_cursor.png"),
                                            tr("Select point"), &MainWindow::ClosedDrawDialogWithApply<VToolAlongLine>,
                                            &MainWindow::ApplyDrawDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolMidpoint(bool checked)
{
    ToolSelectPointByRelease();
    // Reuse DialogAlongLine and VToolAlongLine but with different cursor
    SetToolButtonWithApply<DialogAlongLine>(checked, Tool::Midpoint, QStringLiteral(":/cursor/midpoint_cursor.png"),
                                            tr("Select point"), &MainWindow::ClosedDrawDialogWithApply<VToolAlongLine>,
                                            &MainWindow::ApplyDrawDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolShoulderPoint handler tool shoulderPoint.
 * @param checked true - button checked.
 */
void MainWindow::ToolShoulderPoint(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogShoulderPoint>(checked, Tool::ShoulderPoint,
                                                QStringLiteral(":/cursor/shoulder_cursor.png"),
                                                tr("Select point"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolShoulderPoint>,
                                                &MainWindow::ApplyDrawDialog<VToolShoulderPoint>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolNormal handler tool normal.
 * @param checked true - button checked.
 */
void MainWindow::ToolNormal(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogNormal>(checked, Tool::Normal, QStringLiteral(":/cursor/normal_cursor.png"),
                                         tr("Select first point of line"),
                                         &MainWindow::ClosedDrawDialogWithApply<VToolNormal>,
                                         &MainWindow::ApplyDrawDialog<VToolNormal>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBisector handler tool bisector.
 * @param checked true - button checked.
 */
void MainWindow::ToolBisector(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogBisector>(checked, Tool::Bisector, QStringLiteral(":/cursor/bisector_cursor.png"),
                                           tr("Select first point of angle"),
                                           &MainWindow::ClosedDrawDialogWithApply<VToolBisector>,
                                           &MainWindow::ApplyDrawDialog<VToolBisector>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLineIntersect handler tool lineIntersect.
 * @param checked true - button checked.
 */
void MainWindow::ToolLineIntersect(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLineIntersect>(checked, Tool::LineIntersect,
                                                QStringLiteral(":/cursor/intersect_cursor.png"),
                                                tr("Select first point of first line"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolLineIntersect>,
                                                &MainWindow::ApplyDrawDialog<VToolLineIntersect>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSpline handler tool spline.
 * @param checked true - button checked.
 */
void MainWindow::ToolSpline(bool checked)
{
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSpline>(checked, Tool::Spline, QStringLiteral(":/cursor/spline_cursor.png"),
                                         tr("Select first point curve"),
                                         &MainWindow::ClosedDrawDialogWithApply<VToolSpline>,
                                         &MainWindow::ApplyDrawDialog<VToolSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCubicBezier(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezier>(checked, Tool::CubicBezier,
                                              QStringLiteral(":/cursor/cubic_bezier_cursor.png"),
                                              tr("Select first curve point"),
                                              &MainWindow::ClosedDrawDialogWithApply<VToolCubicBezier>,
                                              &MainWindow::ApplyDrawDialog<VToolCubicBezier>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSpline handler tool CutSpline.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSpline(bool checked)
{
    ToolSelectSpline();
    SetToolButtonWithApply<DialogCutSpline>(checked, Tool::CutSpline,
                                            QStringLiteral(":/cursor/spline_cut_point_cursor.png"),
                                            tr("Select simple curve"),
                                            &MainWindow::ClosedDrawDialogWithApply<VToolCutSpline>,
                                            &MainWindow::ApplyDrawDialog<VToolCutSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolArc handler tool arc.
 * @param checked true - button checked.
 */
void MainWindow::ToolArc(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogArc>(checked, Tool::Arc, QStringLiteral(":/cursor/arc_cursor.png"),
                                      tr("Select point of center of arc"),
                                      &MainWindow::ClosedDrawDialogWithApply<VToolArc>,
                                      &MainWindow::ApplyDrawDialog<VToolArc>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolEllipticalArc handler tool arc.
 * @param checked true - button checked.
 */
void MainWindow::ToolEllipticalArc(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogEllipticalArc>(checked, Tool::EllipticalArc,
                                                QStringLiteral(":/cursor/el_arc_cursor.png"),
                                                tr("Select point of center of elliptical arc"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolEllipticalArc>,
                                                &MainWindow::ApplyDrawDialog<VToolEllipticalArc>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSplinePath handler tool splinePath.
 * @param checked true - button checked.
 */
void MainWindow::ToolSplinePath(bool checked)
{
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSplinePath>(checked, Tool::SplinePath,
                                             QStringLiteral(":/cursor/splinepath_cursor.png"),
                                             tr("Select point of curve path"),
                                             &MainWindow::ClosedDrawDialogWithApply<VToolSplinePath>,
                                             &MainWindow::ApplyDrawDialog<VToolSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCubicBezierPath(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezierPath>(checked, Tool::CubicBezierPath,
                                                  QStringLiteral(":/cursor/cubic_bezier_path_cursor.png"),
                                                  tr("Select point of cubic bezier path"),
                                                  &MainWindow::ClosedDrawDialogWithApply<VToolCubicBezierPath>,
                                                  &MainWindow::ApplyDrawDialog<VToolCubicBezierPath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSplinePath handler tool CutSplinePath.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSplinePath(bool checked)
{
    ToolSelectSplinePath();
    SetToolButtonWithApply<DialogCutSplinePath>(checked, Tool::CutSplinePath,
                                                QStringLiteral(":/cursor/splinepath_cut_point_cursor.png"),
                                                tr("Select curve path"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolCutSplinePath>,
                                                &MainWindow::ApplyDrawDialog<VToolCutSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfContact handler tool pointOfContact.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfContact(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointOfContact>(checked, Tool::PointOfContact,
                                                 QStringLiteral(":/cursor/pointcontact_cursor.png"),
                                                 tr("Select first point of line"),
                                                 &MainWindow::ClosedDrawDialogWithApply<VToolPointOfContact>,
                                                 &MainWindow::ApplyDrawDialog<VToolPointOfContact>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolDetail handler tool detail.
 * @param checked true - button checked.
 */
void MainWindow::ToolDetail(bool checked)
{
    ToolSelectAllDrawObjects();
    SetToolButtonWithApply<DialogSeamAllowance>(checked, Tool::Piece,
                                                QStringLiteral("://cursor/new_detail_cursor.png"),
                                                tr("Select main path objects clockwise."),
                                                &MainWindow::ClosedDetailsDialogWithApply<VToolSeamAllowance>,
                                                &MainWindow::ApplyDetailsDialog<VToolSeamAllowance>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPiecePath(bool checked)
{
    ToolSelectAllDrawObjects();
    SetToolButton<DialogPiecePath>(checked, Tool::PiecePath, QStringLiteral("://cursor/path_cursor.png"),
                                   tr("Select path objects, <b>%1</b> - reverse direction curve")
                                   .arg(VModifierKey::Shift()),
                                   &MainWindow::ClosedDialogPiecePath);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPin(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButton<DialogPin>(checked, Tool::Pin, QStringLiteral("://cursor/pin_cursor.png"), tr("Select pin point"),
                             &MainWindow::ClosedDialogPin);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPlaceLabel(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButton<DialogPlaceLabel>(checked, Tool::PlaceLabel, QStringLiteral("://cursor/place_label_cursor.png"),
                                    tr("Select placelabel center point"), &MainWindow::ClosedDialogPlaceLabel);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolHeight handler tool height.
 * @param checked true - button checked.
 */
void MainWindow::ToolHeight(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogHeight>(checked, Tool::Height, QStringLiteral(":/cursor/height_cursor.png"),
                                         tr("Select base point"),
                                         &MainWindow::ClosedDrawDialogWithApply<VToolHeight>,
                                         &MainWindow::ApplyDrawDialog<VToolHeight>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolTriangle handler tool triangle.
 * @param checked true - button checked.
 */
void MainWindow::ToolTriangle(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogTriangle>(checked, Tool::Triangle, QStringLiteral(":/cursor/triangle_cursor.png"),
                                           tr("Select first point of axis"),
                                           &MainWindow::ClosedDrawDialogWithApply<VToolTriangle>,
                                           &MainWindow::ApplyDrawDialog<VToolTriangle>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfIntersection handler tool pointOfIntersection.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfIntersection(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointOfIntersection>(checked, Tool::PointOfIntersection,
                                                      QStringLiteral(":/cursor/pointofintersect_cursor.png"),
                                                      tr("Select point for X value (vertical)"),
                                                      &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersection>,
                                                      &MainWindow::ApplyDrawDialog<VToolPointOfIntersection>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolUnionDetails handler tool unionDetails.
 * @param checked true - button checked.
 */
void MainWindow::ToolUnionDetails(bool checked)
{
    ToolSelectDetail();
    SetToolButton<DialogUnionDetails>(checked, Tool::UnionDetails, QStringLiteral(":/cursor/union_cursor.png"),
                                      tr("Select detail"), &MainWindow::ClosedDialogUnionDetails);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogUnionDetails actions after closing DialogUnionDetails.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(result);// Avoid error: Template function as signal or slot
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolDuplicateDetail(bool checked)
{
    ToolSelectDetail();
    SetToolButton<DialogDuplicateDetail>(checked, Tool::DuplicateDetail,
                                         QStringLiteral(":/cursor/duplicate_detail_cursor.png"),
                                         tr("Select detail"), &MainWindow::ClosedDialogDuplicateDetail);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogDuplicateDetail(int result)
{
    SCASSERT(not dialogTool.isNull())
    if (result == QDialog::Accepted)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr)

        QGraphicsItem *tool = dynamic_cast<QGraphicsItem *>(VToolSeamAllowance::Duplicate(dialogTool, scene, doc));
        // Do not check for nullptr! See issue #719.
        emit ui->view->itemClicked(tool);
    }
    ArrowTool(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolGroup(bool checked)
{
    ToolSelectGroupObjects();
    const QString tooltip = tr("Select one or more objects, hold <b>%1</b> - for multiple selection, "
                               "<b>%2</b> - finish creation")
            .arg(VModifierKey::Control(), VModifierKey::EnterKey());
    SetToolButton<DialogGroup>(checked, Tool::Group, QStringLiteral(":/cursor/group_plus_cursor.png"), tooltip,
                               &MainWindow::ClosedDialogGroup);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolRotation(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip = tr("Select one or more objects, hold <b>%1</b> - for multiple selection, "
                               "<b>%2</b> - confirm selection")
            .arg(VModifierKey::Control(), VModifierKey::EnterKey());
    SetToolButtonWithApply<DialogRotation>(checked, Tool::Rotation, QStringLiteral(":/cursor/rotation_cursor.png"),
                                           tooltip, &MainWindow::ClosedDrawDialogWithApply<VToolRotation>,
                                           &MainWindow::ApplyDrawDialog<VToolRotation>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolFlippingByLine(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip = tr("Select one or more objects, hold <b>%1</b> - for multiple selection, "
                               "<b>%2</b> - confirm selection")
            .arg(VModifierKey::Control(), VModifierKey::EnterKey());
    SetToolButtonWithApply<DialogFlippingByLine>(checked, Tool::FlippingByLine, ":/cursor/flipping_line_cursor.png",
                                                 tooltip, &MainWindow::ClosedDrawDialogWithApply<VToolFlippingByLine>,
                                                 &MainWindow::ApplyDrawDialog<VToolFlippingByLine>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolFlippingByAxis(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip = tr("Select one or more objects, hold <b>%1</b> - for multiple selection, "
                               "<b>%2</b> - confirm selection")
            .arg(VModifierKey::Control(), VModifierKey::EnterKey());
    SetToolButtonWithApply<DialogFlippingByAxis>(checked, Tool::FlippingByAxis, ":/cursor/flipping_axis_cursor.png",
                                                 tooltip, &MainWindow::ClosedDrawDialogWithApply<VToolFlippingByAxis>,
                                                 &MainWindow::ApplyDrawDialog<VToolFlippingByAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolMove(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip = tr("Select one or more objects, hold <b>%1</b> - for multiple selection, "
                               "<b>%2</b> - confirm selection")
            .arg(VModifierKey::Control(), VModifierKey::EnterKey());
    SetToolButtonWithApply<DialogMove>(checked, Tool::Move, ":/cursor/move_cursor.png", tooltip,
                                       &MainWindow::ClosedDrawDialogWithApply<VToolMove>,
                                       &MainWindow::ApplyDrawDialog<VToolMove>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogGroup(int result)
{
    SCASSERT(dialogTool != nullptr)
    if (result == QDialog::Accepted)
    {
        const QPointer<DialogGroup> dialog = qobject_cast<DialogGroup *>(dialogTool);
        SCASSERT(not dialog.isNull())
        const QDomElement group = doc->CreateGroup(pattern->getNextId(), dialog->GetName(), dialog->GetGroup());
        if (not group.isNull())
        {
            AddGroup *addGroup = new AddGroup(group, doc);
            connect(addGroup, &AddGroup::UpdateGroups, groupsWidget, &VWidgetGroups::UpdateGroups);
            qApp->getUndoStack()->push(addGroup);
        }
    }
    ArrowTool(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogPiecePath(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        VToolPiecePath::Create(dialogTool, sceneDetails, doc, pattern);
    }
    ArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogPin(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        VToolPin::Create(dialogTool, doc, pattern);
    }
    ArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogPlaceLabel(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        VToolPlaceLabel::Create(dialogTool, doc, pattern);
    }
    ArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogInsertNode(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        const QPointer<DialogInsertNode> dTool = qobject_cast<DialogInsertNode *>(dialogTool);
        SCASSERT(not dTool.isNull())
        VToolSeamAllowance::InsertNode(dTool->GetNode(), dTool->GetPieceId(), sceneDetails, pattern, doc);
    }
    ArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ZoomFitBestCurrent()
{
    if(drawMode)
    {
        const QRectF rect = doc->ActiveDrawBoundingRect();
        if (rect.isEmpty())
        {
            return;
        }

        ui->view->fitInView(rect, Qt::KeepAspectRatio);
        QTransform transform = ui->view->transform();

        qreal factor = transform.m11();
        factor = qMax(factor, VMainGraphicsView::MinScale());
        factor = qMin(factor, VMainGraphicsView::MaxScale());

        transform.setMatrix(factor, transform.m12(), transform.m13(), transform.m21(), factor, transform.m23(),
                            transform.m31(), transform.m32(), transform.m33());
        ui->view->setTransform(transform);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutArc handler tool cutArc.
 * @param checked true - button checked.
 */
void MainWindow::ToolCutArc(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogCutArc>(checked, Tool::CutArc, ":/cursor/arc_cut_cursor.png",
                                         tr("Select arc"), &MainWindow::ClosedDrawDialogWithApply<VToolCutArc>,
                                         &MainWindow::ApplyDrawDialog<VToolCutArc>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolLineIntersectAxis(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLineIntersectAxis>(checked, Tool::LineIntersectAxis,
                                                    ":/cursor/line_intersect_axis_cursor.png",
                                                    tr("Select first point of line"),
                                                    &MainWindow::ClosedDrawDialogWithApply<VToolLineIntersectAxis>,
                                                    &MainWindow::ApplyDrawDialog<VToolLineIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCurveIntersectAxis(bool checked)
{
    ToolSelectAllDrawObjects();
    SetToolButtonWithApply<DialogCurveIntersectAxis>(checked, Tool::CurveIntersectAxis,
                                                     ":/cursor/curve_intersect_axis_cursor.png",
                                                     tr("Select curve"),
                                                     &MainWindow::ClosedDrawDialogWithApply<VToolCurveIntersectAxis>,
                                                     &MainWindow::ApplyDrawDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolArcIntersectAxis(bool checked)
{
    ToolSelectAllDrawObjects();
    // Reuse ToolCurveIntersectAxis but with different cursor and tool tip
    SetToolButtonWithApply<DialogCurveIntersectAxis>(checked, Tool::ArcIntersectAxis,
                                                     ":/cursor/arc_intersect_axis_cursor.png",
                                                     tr("Select arc"),
                                                     &MainWindow::ClosedDrawDialogWithApply<VToolCurveIntersectAxis>,
                                                     &MainWindow::ApplyDrawDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionArcs(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogPointOfIntersectionArcs>(checked, Tool::PointOfIntersectionArcs,
                                                          "://cursor/point_of_intersection_arcs.png",
                                                          tr("Select first an arc"),
                                                   &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersectionArcs>,
                                                          &MainWindow::ApplyDrawDialog<VToolPointOfIntersectionArcs>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionCircles(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointOfIntersectionCircles>(checked, Tool::PointOfIntersectionCircles,
                                                             "://cursor/point_of_intersection_circles.png",
                                                             tr("Select first circle center"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersectionCircles>,
                                                         &MainWindow::ApplyDrawDialog<VToolPointOfIntersectionCircles>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionCurves(bool checked)
{
    ToolSelectCurve();
    SetToolButtonWithApply<DialogPointOfIntersectionCurves>(checked, Tool::PointOfIntersectionCurves,
                                                             "://cursor/intersection_curves_cursor.png",
                                                             tr("Select first curve"),
                                                 &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersectionCurves>,
                                                          &MainWindow::ApplyDrawDialog<VToolPointOfIntersectionCurves>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromCircleAndTangent(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointFromCircleAndTangent>(checked, Tool::PointFromCircleAndTangent,
                                                            "://cursor/point_from_circle_and_tangent_cursor.png",
                                                            tr("Select point on tangent"),
                                                 &MainWindow::ClosedDrawDialogWithApply<VToolPointFromCircleAndTangent>,
                                                          &MainWindow::ApplyDrawDialog<VToolPointFromCircleAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromArcAndTangent(bool checked)
{
    ToolSelectPointArc();
    SetToolButtonWithApply<DialogPointFromArcAndTangent>(checked, Tool::PointFromArcAndTangent,
                                                         "://cursor/point_from_arc_and_tangent_cursor.png",
                                                         tr("Select point on tangent"),
                                                    &MainWindow::ClosedDrawDialogWithApply<VToolPointFromArcAndTangent>,
                                                         &MainWindow::ApplyDrawDialog<VToolPointFromArcAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolArcWithLength(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogArcWithLength>(checked, Tool::ArcWithLength,
                                                "://cursor/arc_with_length_cursor.png",
                                                tr("Select point of the center of the arc"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolArcWithLength>,
                                                &MainWindow::ApplyDrawDialog<VToolArcWithLength>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolTrueDarts(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogTrueDarts>(checked, Tool::TrueDarts,
                                                "://cursor/true_darts_cursor.png",
                                                tr("Select the first base line point"),
                                                &MainWindow::ClosedDrawDialogWithApply<VToolTrueDarts>,
                                            &MainWindow::ApplyDrawDialog<VToolTrueDarts>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolInsertNode(bool checked)
{
    ToolSelectAllDrawObjects();
    SetToolButton<DialogInsertNode>(checked, Tool::InsertNode, "://cursor/insert_node_cursor.png",
                                    tr("Select an item to insert"), &MainWindow::ClosedDialogInsertNode);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.Tip show tools tooltip.
 * @param toolTip tooltip text.
 */
void MainWindow::ShowToolTip(const QString &toolTip)
{
    m_statusLabel->setText(toolTip);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief triggers the update of the visibility groups
 */
void MainWindow::UpdateVisibilityGroups()
{
    groupsWidget->UpdateGroups();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief triggers the update of the details list
 */
void MainWindow::UpdateDetailsList()
{
    detailsWidget->UpdateList();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle after show window.
 * @param event show event.
 */
void MainWindow::showEvent( QShowEvent *event )
{
    QMainWindow::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarButton->setWindow(windowHandle());
#endif

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    MinimumScrollBar();

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        undoAction->setText(tr("&Undo"));
        redoAction->setText(tr("&Redo"));
        statusBar()->showMessage(tr("Changes applied."), 5000);
        patternPieceLabel->setText(tr("Pattern Piece:"));

        if (qApp->GetDrawMode() == Draw::Calculation)
        {
            ui->dockWidgetGroups->setWindowTitle(tr("Groups of visibility"));
            ui->dockWidgetGroups->setToolTip(tr("Contains all visibility groups"));
        }
        else
        {
            ui->dockWidgetGroups->setWindowTitle(tr("Details"));
            ui->dockWidgetGroups->setToolTip(tr("Show which details will go in layout"));
        }

        UpdateWindowTitle();
        emit sceneDetails->LanguageChanged();
    }
    // remember to call base class implementation
    QMainWindow::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle after close window.
 * @param event close event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
#if defined(Q_OS_MAC) && QT_VERSION < QT_VERSION_CHECK(5, 11, 1)
    // Workaround for Qt bug https://bugreports.qt.io/browse/QTBUG-43344
    static int numCalled = 0;
    if (numCalled++ >= 1)
    {
        return;
    }
#endif

    qCDebug(vMainWindow, "Closing main window");
    if (MaybeSave())
    {
        FileClosedCorrect();

        event->accept();
        QApplication::closeAllWindows();
    }
    else
    {
        qCDebug(vMainWindow, "Closing canceled.");
        event->ignore();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::customEvent(QEvent *event)
{
    if (event->type() == UNDO_EVENT)
    {
        qApp->getUndoStack()->undo();
    }
    else if (event->type() == FIT_BEST_CURRENT_EVENT)
    {
        ZoomFitBestCurrent();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CleanLayout()
{
    qDeleteAll (scenes);
    scenes.clear();
    shadows.clear();
    papers.clear();
    gcontours.clear();
    ui->listWidget->clear();
    SetLayoutModeActions();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PrepareSceneList(PreviewQuatilty quality)
{
    ui->listWidget->clear();
    for (int i=1; i<=scenes.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(ScenePreview(i-1, ui->listWidget->iconSize(), quality),
                                                    QString::number(i));
        ui->listWidget->addItem(item);
    }

    if (not scenes.isEmpty())
    {
        ui->listWidget->setCurrentRow(0);
        SetLayoutModeActions();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ExportToCSVData(const QString &fileName, bool withHeader, int mib, const QChar &separator)
{
    QxtCsvModel csv;

    csv.insertColumn(0);
    csv.insertColumn(1);
    csv.insertColumn(2);

    if (withHeader)
    {
        csv.setHeaderText(0, tr("Name"));
        csv.setHeaderText(1, tr("The calculated value"));
        csv.setHeaderText(2, tr("Formula"));
    }

    const QMap<QString, QSharedPointer<VIncrement> > increments = pattern->DataIncrements();

    qint32 currentRow = -1;

    auto SavePreviewCalculation = [&currentRow, &csv, increments](bool save)
    {
        QMap<QString, QSharedPointer<VIncrement> >::const_iterator i;
        QMap<quint32, QString> map;
        //Sorting QHash by id
        for (i = increments.constBegin(); i != increments.constEnd(); ++i)
        {
            const QSharedPointer<VIncrement> incr = i.value();
            if (incr->IsPreviewCalculation() == save)
            {
                map.insert(incr->GetIndex(), i.key());
            }
        }

        QMapIterator<quint32, QString> iMap(map);
        while (iMap.hasNext())
        {
            iMap.next();
            QSharedPointer<VIncrement> incr = increments.value(iMap.value());
            currentRow++;

            csv.insertRow(currentRow);
            csv.setText(currentRow, 0, incr->GetName()); // name
            csv.setText(currentRow, 1, qApp->LocaleToString(*incr->GetValue())); // calculated value

            QString formula = VTranslateVars::TryFormulaToUser(incr->GetFormula(),
                                                               qApp->Settings()->GetOsSeparator());
            csv.setText(currentRow, 2, formula); // formula
        }
    };

    SavePreviewCalculation(false);
    SavePreviewCalculation(true);

    QString error;
    csv.toCSV(fileName, error, withHeader, separator, QTextCodec::codecForMib(mib));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ScaleChanged(qreal scale)
{
    if (not doubleSpinBoxScale.isNull())
    {
        doubleSpinBoxScale->blockSignals(true);
        doubleSpinBoxScale->setMaximum(qFloor(VMainGraphicsView::MaxScale()*1000)/10.0);
        doubleSpinBoxScale->setMinimum(qFloor(VMainGraphicsView::MinScale()*1000)/10.0);
        doubleSpinBoxScale->setValue(qFloor(scale*1000)/10.0);
        doubleSpinBoxScale->setSingleStep(1);
        doubleSpinBoxScale->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadIndividual()
{
    const QString filter = tr("Individual measurements") + QLatin1String(" (*.vit);;") + tr("Multisize measurements") +
                           QLatin1String(" (*.vst)");
    //Use standard path to individual measurements
    const QString path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();

    bool usedNotExistedDir = false;
    QDir directory(path);
    if (not directory.exists())
    {
        usedNotExistedDir = directory.mkpath(QChar('.'));
    }

    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter, nullptr
#ifdef Q_OS_LINUX
                                                       , QFileDialog::DontUseNativeDialog
#endif
                                                       );

    if (not mPath.isEmpty())
    {
        if (LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()));
            }
            ui->actionUnloadMeasurements->setEnabled(true);
            doc->SetMPath(RelativeMPath(qApp->GetPatternPath(), mPath));
            watcher->addPath(mPath);
            PatternChangesWereSaved(false);
            ui->actionEditCurrent->setEnabled(true);
            statusBar()->showMessage(tr("Measurements loaded"), 5000);
            doc->LiteParseTree(Document::FullLiteParse);

            UpdateWindowTitle();
        }
    }

    if (usedNotExistedDir)
    {
        QDir directory(path);
        directory.rmpath(QChar('.'));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadMultisize()
{
    const QString filter = tr("Multisize measurements") + QLatin1String(" (*.vst);;") + tr("Individual measurements") +
                           QLatin1String("(*.vit)");
    //Use standard path to multisize measurements
    QString path = qApp->ValentinaSettings()->GetPathMultisizeMeasurements();
    path = VCommonSettings::PrepareMultisizeTables(path);
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter, nullptr
#ifdef Q_OS_LINUX
                                                       , QFileDialog::DontUseNativeDialog
#endif
                                                       );

    if (not mPath.isEmpty())
    {
        QString hText;
        if (not gradationHeights.isNull())
        {
            hText = gradationHeights->currentText();
        }
        QString sText;
        if (not gradationSizes.isNull())
        {
            sText = gradationSizes->currentText();
        }

        if(LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()));
            }
            ui->actionUnloadMeasurements->setEnabled(true);
            doc->SetMPath(RelativeMPath(qApp->GetPatternPath(), mPath));
            watcher->addPath(mPath);
            PatternChangesWereSaved(false);
            ui->actionEditCurrent->setEnabled(true);
            statusBar()->showMessage(tr("Measurements loaded"), 5000);
            doc->LiteParseTree(Document::FullLiteParse);

            UpdateWindowTitle();

            if (qApp->patternType() == MeasurementsType::Multisize)
            {
                if (not hText.isEmpty() && not gradationHeights.isNull())
                {
                    gradationHeights->setCurrentText(hText);
                }

                if (not sText.isEmpty() && not gradationSizes.isNull())
                {
                    gradationSizes->setCurrentText(sText);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UnloadMeasurements()
{
    if (doc->MPath().isEmpty())
    {
        ui->actionUnloadMeasurements->setDisabled(true);
        return;
    }

    if (doc->ListMeasurements().isEmpty())
    {
        watcher->removePath(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()));
        if (qApp->patternType() == MeasurementsType::Multisize)
        {
            ToolBarOption();
        }
        qApp->setPatternType(MeasurementsType::Unknown);
        doc->SetMPath(QString());
        emit doc->UpdatePatternLabel();
        PatternChangesWereSaved(false);
        ui->actionEditCurrent->setEnabled(false);
        ui->actionUnloadMeasurements->setDisabled(true);
        statusBar()->showMessage(tr("Measurements unloaded"), 5000);

        UpdateWindowTitle();
    }
    else
    {
        qCWarning(vMainWindow, "%s",
                  qUtf8Printable(tr("Couldn't unload measurements. Some of them are used in the pattern.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowMeasurements()
{
    if (not doc->MPath().isEmpty())
    {
        const QString absoluteMPath = AbsoluteMPath(qApp->GetPatternPath(), doc->MPath());

        QStringList arguments;
        if (qApp->patternType() == MeasurementsType::Multisize)
        {
            arguments = QStringList()
                    << absoluteMPath
                    << "-u"
                    << UnitsToStr(qApp->patternUnit())
                    << "-e"
                    << QString().setNum(static_cast<int>(UnitConvertor(pattern->height(), doc->MUnit(), Unit::Cm)))
                    << "-s"
                    << QString().setNum(static_cast<int>(UnitConvertor(pattern->size(), doc->MUnit(), Unit::Cm)));
        }
        else
        {
            arguments = QStringList() << absoluteMPath
                                      << "-u"
                                      << UnitsToStr(qApp->patternUnit());
        }

        if (isNoScaling)
        {
            arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
        }

        const QString tape = qApp->TapeFilePath();
        const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
        QProcess::startDetached(tape, arguments, workingDirectory);
    }
    else
    {
        ui->actionEditCurrent->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::MeasurementsChanged(const QString &path)
{
    mChanges = false;
    QFileInfo checkFile(path);
    if (checkFile.exists())
    {
        mChanges = true;
        mChangesAsked = false;
    }
    else
    {
        for(int i=0; i<=1000; i=i+10)
        {
            if (checkFile.exists())
            {
                mChanges = true;
                mChangesAsked = false;
                break;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    UpdateWindowTitle();
    ui->actionSyncMeasurements->setEnabled(mChanges);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SyncMeasurements()
{
    if (mChanges)
    {
        const QString path = AbsoluteMPath(qApp->GetPatternPath(), doc->MPath());
        if(UpdateMeasurements(path, static_cast<int>(pattern->size()), static_cast<int>(pattern->height())))
        {
            if (not watcher->files().contains(path))
            {
                watcher->addPath(path);
            }
            const QString msg = tr("Measurements have been synced");
            qCDebug(vMainWindow, "%s", qUtf8Printable(msg));
            statusBar()->showMessage(msg, 5000);
            VWidgetPopup::PopupMessage(this, msg);
            doc->LiteParseTree(Document::FullLiteParse);
            mChanges = false;
            mChangesAsked = true;
            UpdateWindowTitle();
            ui->actionSyncMeasurements->setEnabled(mChanges);
        }
        else
        {
            qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't sync measurements.")));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_MAC)
void MainWindow::OpenAt(QAction *where)
{
    const QString path = qApp->GetPatternPath().left(qApp->GetPatternPath().indexOf(where->text())) + where->text();
    if (path == qApp->GetPatternPath())
    {
        return;
    }
    QProcess process;
    process.start(QStringLiteral("/usr/bin/open"), QStringList() << path, QIODevice::ReadOnly);
    process.waitForFinished();
}
#endif //defined(Q_OS_MAC)

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarOption enable option toolbar.
 */
void MainWindow::ToolBarOption()
{
    ui->toolBarOption->clear();
    if (not gradationHeights.isNull())
    {
        delete gradationHeights;
    }
    if (not gradationSizes.isNull())
    {
        delete gradationSizes;
    }
    if (not gradationHeightsLabel.isNull())
    {
        delete gradationHeightsLabel;
    }
    if (not gradationSizesLabel.isNull())
    {
        delete gradationSizesLabel;
    }
    if (not zoomScale.isNull())
    {
        delete zoomScale;
    }
    if (not doubleSpinBoxScale.isNull())
    {
        delete doubleSpinBoxScale;
    }

    if (qApp->patternType() == MeasurementsType::Multisize)
    {
        const QStringList listHeights = VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit());
        const QStringList listSizes = VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit());

        gradationHeightsLabel = new QLabel(tr("Height:"), this);
        gradationHeights = SetGradationList(gradationHeightsLabel, listHeights);

        // set default height
        SetDefaultHeight();

        connect(gradationHeights.data(), &QComboBox::currentTextChanged,
                this, &MainWindow::ChangedHeight);

        gradationSizesLabel = new QLabel(tr("Size:"), this);
        gradationSizes = SetGradationList(gradationSizesLabel, listSizes);

        // set default size
        SetDefaultSize();

        connect(gradationSizes.data(), &QComboBox::currentTextChanged,
                this, &MainWindow::ChangedSize);

        ui->toolBarOption->addSeparator();
    }

    zoomScale = new QLabel(tr("Scale:"), this);
    ui->toolBarOption->addWidget(zoomScale);

    doubleSpinBoxScale = new QDoubleSpinBox(this);
    doubleSpinBoxScale->setDecimals(1);
    doubleSpinBoxScale->setSuffix("%");
    ScaleChanged(ui->view->transform().m11());
    connect(doubleSpinBoxScale.data(), QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double d){ui->view->Zoom(d/100.0);});
    ui->toolBarOption->addWidget(doubleSpinBoxScale);

    ui->toolBarOption->addSeparator();

    m_mouseCoordinate = new QLabel(QString("0, 0 (%1)").arg(UnitsToStr(qApp->patternUnit(), true)));
    ui->toolBarOption->addWidget(m_mouseCoordinate);
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox *MainWindow::SetGradationList(QLabel *label, const QStringList &list)
{
    ui->toolBarOption->addWidget(label);

    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems(list);
    ui->toolBarOption->addWidget(comboBox);

    return comboBox;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStages()
{
    leftGoToStage = new QLabel(this);
    leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
    ui->toolBarStages->insertWidget(ui->actionDetails, leftGoToStage);

    rightGoToStage = new QLabel(this);
    rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));
    ui->toolBarStages->insertWidget(ui->actionLayout, rightGoToStage);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarDraws enable draw toolbar.
 */
void MainWindow::ToolBarDraws()
{
    patternPieceLabel = new QLabel(tr("Pattern Piece:"));
    ui->toolBarDraws->addWidget(patternPieceLabel);

    // By using Qt UI Designer we can't add QComboBox to toolbar
    comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);
    comboBoxDraws->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboBoxDraws->setEnabled(false);
    connect(comboBoxDraws, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index){ChangePP(index);});

    connect(ui->actionOptionDraw, &QAction::triggered, this, [this]()
    {
        const QString activDraw = doc->GetNameActivPP();
        const QString nameDraw = PatternPieceName(activDraw);
        if (nameDraw.isEmpty() || activDraw == nameDraw)
        {
            return;
        }
        qApp->getUndoStack()->push(new RenamePP(doc, nameDraw, comboBoxDraws));
    });
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarTools()
{
    /*First we will try use Standard Shortcuts from Qt, but because keypad "-" and "+" not the same keys like in main
    keypad, shortcut Ctrl+"-" or "+" from keypad will not working with standard shortcut (QKeySequence::ZoomIn or
    QKeySequence::ZoomOut). For examle "+" is Qt::Key_Plus + Qt::KeypadModifier for keypad.
    Also for me don't work Qt:CTRL and work Qt::ControlModifier.*/

    QList<QKeySequence> zoomInShortcuts;
    zoomInShortcuts.append(QKeySequence(QKeySequence::ZoomIn));
    zoomInShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Plus + Qt::KeypadModifier));
    ui->actionZoomIn->setShortcuts(zoomInShortcuts);
    connect(ui->actionZoomIn, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomIn);

    QList<QKeySequence> zoomOutShortcuts;
    zoomOutShortcuts.append(QKeySequence(QKeySequence::ZoomOut));
    zoomOutShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Minus + Qt::KeypadModifier));
    ui->actionZoomOut->setShortcuts(zoomOutShortcuts);
    connect(ui->actionZoomOut, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomOut);

    QList<QKeySequence> zoomOriginalShortcuts;
    zoomOriginalShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_0));
    zoomOriginalShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_0 + Qt::KeypadModifier));
    ui->actionZoomOriginal->setShortcuts(zoomOriginalShortcuts);
    connect(ui->actionZoomOriginal, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomOriginal);

    QList<QKeySequence> zoomFitBestShortcuts;
    zoomFitBestShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Equal));
    ui->actionZoomFitBest->setShortcuts(zoomFitBestShortcuts);
    connect(ui->actionZoomFitBest, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomFitBest);

    QList<QKeySequence> zoomFitBestCurrentShortcuts;
    zoomFitBestCurrentShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_M));
    ui->actionZoomFitBestCurrent->setShortcuts(zoomFitBestCurrentShortcuts);
    connect(ui->actionZoomFitBestCurrent, &QAction::triggered, this, &MainWindow::ZoomFitBestCurrent);

    connect(ui->actionPreviousPatternPiece, &QAction::triggered, this, &MainWindow::PreviousPatternPiece);
    connect(ui->actionNextPatternPiece, &QAction::triggered, this, &MainWindow::NextPatternPiece);

    ui->actionIncreaseLabelFont->setShortcut(QKeySequence(Qt::ShiftModifier + Qt::Key_Plus));
    connect(ui->actionIncreaseLabelFont, &QAction::triggered, this, [this]()
    {
        VSettings *settings = qApp->ValentinaSettings();
        settings->SetLabelFontSize(settings->GetLabelFontSize() + 1);
        if (sceneDraw)
        {
            sceneDraw->update();
        }

        if (sceneDetails)
        {
            sceneDetails->update();
        }
    });

    ui->actionDecreaseLabelFont->setShortcut(QKeySequence(Qt::ShiftModifier + Qt::Key_Minus));
    connect(ui->actionDecreaseLabelFont, &QAction::triggered, this, [this]()
    {
        VSettings *settings = qApp->ValentinaSettings();
        settings->SetLabelFontSize(settings->GetLabelFontSize() - 1);
        if (sceneDraw)
        {
            sceneDraw->update();
        }

        if (sceneDetails)
        {
            sceneDetails->update();
        }
    });

    ui->actionOriginalLabelFont->setShortcut(QKeySequence(Qt::ShiftModifier + Qt::Key_0));
    connect(ui->actionOriginalLabelFont, &QAction::triggered, this, [this]()
    {
        VSettings *settings = qApp->ValentinaSettings();
        settings->SetLabelFontSize(settings->GetDefLabelFontSize());
        if (sceneDraw)
        {
            sceneDraw->update();
        }

        if (sceneDetails)
        {
            sceneDetails->update();
        }
    });

    ui->actionHideLabels->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_L));
    connect(ui->actionHideLabels, &QAction::triggered, this, [this](bool checked)
    {
        qApp->ValentinaSettings()->SetHideLabels(checked);
        if (sceneDraw)
        {
            sceneDraw->update();
        }

        if (sceneDetails)
        {
            sceneDetails->update();
        }
    });
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitToolButtons()
{
    toolButtonPointerList.append(ui->toolButtonPointerPoint);
    toolButtonPointerList.append(ui->toolButtonPointerLine);
    toolButtonPointerList.append(ui->toolButtonPointerCurve);
    toolButtonPointerList.append(ui->toolButtonPointerArc);
    toolButtonPointerList.append(ui->toolButtonPointerDetail);
    toolButtonPointerList.append(ui->toolButtonPointerOperations);
    toolButtonPointerList.append(ui->toolButtonPointerEllipticalArc);

    for (auto pointer : qAsConst(toolButtonPointerList))
    {
        connect(pointer, &QToolButton::clicked, this, &MainWindow::ArrowTool);
    }

    // This check helps to find missed tools
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Check if all tools were connected.");

    connect(ui->toolButtonEndLine, &QToolButton::clicked, this, &MainWindow::ToolEndLine);
    connect(ui->toolButtonLine, &QToolButton::clicked, this, &MainWindow::ToolLine);
    connect(ui->toolButtonAlongLine, &QToolButton::clicked, this, &MainWindow::ToolAlongLine);
    connect(ui->toolButtonShoulderPoint, &QToolButton::clicked, this, &MainWindow::ToolShoulderPoint);
    connect(ui->toolButtonNormal, &QToolButton::clicked, this, &MainWindow::ToolNormal);
    connect(ui->toolButtonBisector, &QToolButton::clicked, this, &MainWindow::ToolBisector);
    connect(ui->toolButtonLineIntersect, &QToolButton::clicked, this, &MainWindow::ToolLineIntersect);
    connect(ui->toolButtonSpline, &QToolButton::clicked, this, &MainWindow::ToolSpline);
    connect(ui->toolButtonCubicBezier, &QToolButton::clicked, this, &MainWindow::ToolCubicBezier);
    connect(ui->toolButtonArc, &QToolButton::clicked, this, &MainWindow::ToolArc);
    connect(ui->toolButtonSplinePath, &QToolButton::clicked, this, &MainWindow::ToolSplinePath);
    connect(ui->toolButtonCubicBezierPath, &QToolButton::clicked, this, &MainWindow::ToolCubicBezierPath);
    connect(ui->toolButtonPointOfContact, &QToolButton::clicked, this, &MainWindow::ToolPointOfContact);
    connect(ui->toolButtonNewDetail, &QToolButton::clicked, this, &MainWindow::ToolDetail);
    connect(ui->toolButtonInternalPath, &QToolButton::clicked, this, &MainWindow::ToolPiecePath);
    connect(ui->toolButtonHeight, &QToolButton::clicked, this, &MainWindow::ToolHeight);
    connect(ui->toolButtonTriangle, &QToolButton::clicked, this, &MainWindow::ToolTriangle);
    connect(ui->toolButtonPointOfIntersection, &QToolButton::clicked, this, &MainWindow::ToolPointOfIntersection);
    connect(ui->toolButtonSplineCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSpline);
    connect(ui->toolButtonSplinePathCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSplinePath);
    connect(ui->toolButtonUnionDetails, &QToolButton::clicked, this, &MainWindow::ToolUnionDetails);
    connect(ui->toolButtonDuplicateDetail, &QToolButton::clicked, this, &MainWindow::ToolDuplicateDetail);
    connect(ui->toolButtonArcCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutArc);
    connect(ui->toolButtonLineIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolLineIntersectAxis);
    connect(ui->toolButtonCurveIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolCurveIntersectAxis);
    connect(ui->toolButtonArcIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolArcIntersectAxis);
    connect(ui->toolButtonLayoutSettings, &QToolButton::clicked, this, &MainWindow::ToolLayoutSettings);
    connect(ui->toolButtonPointOfIntersectionArcs, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionArcs);
    connect(ui->toolButtonPointOfIntersectionCircles, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionCircles);
    connect(ui->toolButtonIntersectionCurves, &QToolButton::clicked, this, &MainWindow::ToolPointOfIntersectionCurves);
    connect(ui->toolButtonPointFromCircleAndTangent, &QToolButton::clicked, this,
            &MainWindow::ToolPointFromCircleAndTangent);
    connect(ui->toolButtonPointFromArcAndTangent, &QToolButton::clicked, this, &MainWindow::ToolPointFromArcAndTangent);
    connect(ui->toolButtonArcWithLength, &QToolButton::clicked, this, &MainWindow::ToolArcWithLength);
    connect(ui->toolButtonTrueDarts, &QToolButton::clicked, this, &MainWindow::ToolTrueDarts);
    connect(ui->toolButtonGroup, &QToolButton::clicked, this, &MainWindow::ToolGroup);
    connect(ui->toolButtonRotation, &QToolButton::clicked, this, &MainWindow::ToolRotation);
    connect(ui->toolButtonFlippingByLine, &QToolButton::clicked, this, &MainWindow::ToolFlippingByLine);
    connect(ui->toolButtonFlippingByAxis, &QToolButton::clicked, this, &MainWindow::ToolFlippingByAxis);
    connect(ui->toolButtonMove, &QToolButton::clicked, this, &MainWindow::ToolMove);
    connect(ui->toolButtonMidpoint, &QToolButton::clicked, this, &MainWindow::ToolMidpoint);
    connect(ui->toolButtonLayoutExportAs, &QToolButton::clicked, this, &MainWindow::ExportLayoutAs);
    connect(ui->toolButtonDetailExportAs, &QToolButton::clicked, this, &MainWindow::ExportDetailsAs);
    connect(ui->toolButtonEllipticalArc, &QToolButton::clicked, this, &MainWindow::ToolEllipticalArc);
    connect(ui->toolButtonPin, &QToolButton::clicked, this, &MainWindow::ToolPin);
    connect(ui->toolButtonInsertNode, &QToolButton::clicked, this, &MainWindow::ToolInsertNode);
    connect(ui->toolButtonPlaceLabel, &QToolButton::clicked, this, &MainWindow::ToolPlaceLabel);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMove save mouse position and show user.
 * @param scenePos position mouse.
 */
void MainWindow::MouseMove(const QPointF &scenePos)
{
    if (not m_mouseCoordinate.isNull())
    {
        //: Coords in status line: "X, Y (units)"
        m_mouseCoordinate->setText(QString("%1, %2 (%3)").arg(static_cast<qint32>(qApp->fromPixel(scenePos.x())))
                                                         .arg(static_cast<qint32>(qApp->fromPixel(scenePos.y())))
                                                         .arg(UnitsToStr(qApp->patternUnit(), true)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
/**
 * @brief CancelTool cancel tool.
 */
void MainWindow::CancelTool()
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Not all tools were handled.");

    qCDebug(vMainWindow, "Canceling tool.");
    if(not dialogTool.isNull())
    {
        dialogTool->hide();
        dialogTool->deleteLater();
    }
    qCDebug(vMainWindow, "Dialog closed.");

    currentScene->setFocus(Qt::OtherFocusReason);
    currentScene->clearSelection();
    emit ui->view->itemClicked(nullptr); // Hide visualization to avoid a crash

    switch ( currentTool )
    {
        case Tool::Arrow:
            for (auto pointer : qAsConst(toolButtonPointerList))
            {
                pointer->setChecked(false);
            }
            m_statusLabel->setText(QString());

            // Crash: using CRTL+Z while using line tool.
            // related bug report:
            // https://bitbucket.org/dismine/valentina/issues/454/crash-using-crtl-z-while-using-line-tool
            undoAction->setEnabled(false);
            redoAction->setEnabled(false);
            VAbstractTool::m_suppressContextMenu = true;
            return;
        case Tool::BasePoint:
        case Tool::SinglePoint:
        case Tool::DoublePoint:
        case Tool::LinePoint:
        case Tool::AbstractSpline:
        case Tool::Cut:
        case Tool::LAST_ONE_DO_NOT_USE:
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeElArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
            Q_UNREACHABLE(); //-V501
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLine:
            ui->toolButtonEndLine->setChecked(false);
            break;
        case Tool::Line:
            ui->toolButtonLine->setChecked(false);
            break;
        case Tool::AlongLine:
            ui->toolButtonAlongLine->setChecked(false);
            break;
        case Tool::Midpoint:
            ui->toolButtonMidpoint->setChecked(false);
            break;
        case Tool::ShoulderPoint:
            ui->toolButtonShoulderPoint->setChecked(false);
            break;
        case Tool::Normal:
            ui->toolButtonNormal->setChecked(false);
            break;
        case Tool::Bisector:
            ui->toolButtonBisector->setChecked(false);
            break;
        case Tool::LineIntersect:
            ui->toolButtonLineIntersect->setChecked(false);
            break;
        case Tool::Spline:
            ui->toolButtonSpline->setChecked(false);
            break;
        case Tool::CubicBezier:
            ui->toolButtonCubicBezier->setChecked(false);
            break;
        case Tool::Arc:
            ui->toolButtonArc->setChecked(false);
            break;
        case Tool::ArcWithLength:
            ui->toolButtonArcWithLength->setChecked(false);
            break;
        case Tool::SplinePath:
            ui->toolButtonSplinePath->setChecked(false);
            break;
        case Tool::CubicBezierPath:
            ui->toolButtonCubicBezierPath->setChecked(false);
            break;
        case Tool::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(false);
            break;
        case Tool::Piece:
            ui->toolButtonNewDetail->setChecked(false);
            break;
        case Tool::PiecePath:
            ui->toolButtonInternalPath->setChecked(false);
            break;
        case Tool::Height:
            ui->toolButtonHeight->setChecked(false);
            break;
        case Tool::Triangle:
            ui->toolButtonTriangle->setChecked(false);
            break;
        case Tool::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(false);
            break;
        case Tool::CutSpline:
            ui->toolButtonSplineCutPoint->setChecked(false);
            break;
        case Tool::CutSplinePath:
            ui->toolButtonSplinePathCutPoint->setChecked(false);
            break;
        case Tool::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(false);
            break;
        case Tool::DuplicateDetail:
            ui->toolButtonDuplicateDetail->setChecked(false);
            break;
        case Tool::CutArc:
            ui->toolButtonArcCutPoint->setChecked(false);
            break;
        case Tool::LineIntersectAxis:
            ui->toolButtonLineIntersectAxis->setChecked(false);
            break;
        case Tool::CurveIntersectAxis:
            ui->toolButtonCurveIntersectAxis->setChecked(false);
            break;
        case Tool::ArcIntersectAxis:
            ui->toolButtonArcIntersectAxis->setChecked(false);
            break;
        case Tool::PointOfIntersectionArcs:
            ui->toolButtonPointOfIntersectionArcs->setChecked(false);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(false);
            break;
        case Tool::PointOfIntersectionCurves:
            ui->toolButtonIntersectionCurves->setChecked(false);
            break;
        case Tool::PointFromCircleAndTangent:
            ui->toolButtonPointFromCircleAndTangent->setChecked(false);
            break;
        case Tool::PointFromArcAndTangent:
            ui->toolButtonPointFromArcAndTangent->setChecked(false);
            break;
        case Tool::TrueDarts:
            ui->toolButtonTrueDarts->setChecked(false);
            break;
        case Tool::Group:
            ui->toolButtonGroup->setChecked(false);
            break;
        case Tool::Rotation:
            ui->toolButtonRotation->setChecked(false);
            break;
        case Tool::FlippingByLine:
            ui->toolButtonFlippingByLine->setChecked(false);
            break;
        case Tool::FlippingByAxis:
            ui->toolButtonFlippingByAxis->setChecked(false);
            break;
        case Tool::Move:
            ui->toolButtonMove->setChecked(false);
            break;
        case Tool::EllipticalArc:
            ui->toolButtonEllipticalArc->setChecked(false);
            break;
        case Tool::Pin:
            ui->toolButtonPin->setChecked(false);
            break;
        case Tool::InsertNode:
            ui->toolButtonInsertNode->setChecked(false);
            break;
        case Tool::PlaceLabel:
            ui->toolButtonPlaceLabel->setChecked(false);
            break;
    }

    // Crash: using CRTL+Z while using line tool.
    // related bug report:
    // https://bitbucket.org/dismine/valentina/issues/454/crash-using-crtl-z-while-using-line-tool
    undoAction->setEnabled(qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(qApp->getUndoStack()->canRedo());
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowTool enable arrow tool.
 */
void  MainWindow::ArrowTool(bool checked)
{
    if (checked && currentTool != Tool::Arrow)
    {
        qCDebug(vMainWindow, "Arrow tool.");
        CancelTool();
        for (auto pointer : qAsConst(toolButtonPointerList))
        {
            pointer->setChecked(true);
        }
        currentTool = Tool::Arrow;
        emit EnableItemMove(true);
        emit ItemsSelection(SelectionType::ByMouseRelease);
        VAbstractTool::m_suppressContextMenu = false;

        // Only true for rubber band selection
        emit EnableLabelSelection(true);
        emit EnablePointSelection(false);
        emit EnableLineSelection(false);
        emit EnableArcSelection(false);
        emit EnableElArcSelection(false);
        emit EnableSplineSelection(false);
        emit EnableSplinePathSelection(false);
        emit EnableNodeLabelSelection(true);
        emit EnableNodePointSelection(true);
        emit EnableDetailSelection(true);// Disable when done visualization details

        // Hovering
        emit EnableLabelHover(true);
        emit EnablePointHover(true);
        emit EnableLineHover(true);
        emit EnableArcHover(true);
        emit EnableElArcHover(true);
        emit EnableSplineHover(true);
        emit EnableSplinePathHover(true);
        emit EnableNodeLabelHover(true);
        emit EnableNodePointHover(true);
        emit EnableDetailHover(true);

        ui->view->AllowRubberBand(true);
        ui->view->viewport()->unsetCursor();
        ui->view->viewport()->setCursor(QCursor(Qt::ArrowCursor));
        ui->view->setCurrentCursorShape(); // Hack to fix problem with a cursor
        m_statusLabel->setText(QString());
        ui->view->setShowToolOptions(true);
        qCDebug(vMainWindow, "Enabled arrow tool.");
    }
    else
    {
        for (auto pointer : qAsConst(toolButtonPointerList))
        {
            pointer->setChecked(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyPressEvent handle key press events.
 * @param event key event.
 */
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            ArrowTool(true);
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            EndVisualization();
            break;
        default:
            break;
    }
    QMainWindow::keyPressEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveCurrentScene save scene options before set another.
 */
void MainWindow::SaveCurrentScene()
{
    if (qApp->GetDrawMode() == Draw::Calculation || qApp->GetDrawMode() == Draw::Modeling)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr)

        /*Save transform*/
        scene->setTransform(ui->view->transform());
        /*Save scroll bars value for previous scene.*/
        QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
        scene->setHorScrollBar(horScrollBar->value());
        QScrollBar *verScrollBar = ui->view->verticalScrollBar();
        scene->setVerScrollBar(verScrollBar->value());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RestoreCurrentScene restore scene options after change.
 */
void MainWindow::RestoreCurrentScene()
{
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
    SCASSERT(scene != nullptr)

    /*Set transform for current scene*/
    ui->view->setTransform(scene->transform());
    /*Set value for current scene scroll bar.*/
    QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
    horScrollBar->setValue(scene->getHorScrollBar());
    QScrollBar *verScrollBar = ui->view->verticalScrollBar();
    verScrollBar->setValue(scene->getVerScrollBar());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionDraw show draw scene.
 * @param checked true - button checked.
 */
void MainWindow::ActionDraw(bool checked)
{
    if (checked)
    {
        qCDebug(vMainWindow, "Show draw scene");
        ArrowTool(true);

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDraw->setChecked(true);
        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDraw;
        ui->view->setScene(currentScene);
        RestoreCurrentScene();

        qApp->SetDrawMode(Draw::Calculation);
        comboBoxDraws->setCurrentIndex(currentDrawIndex);//restore current pattern peace
        drawMode = true;

        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(currentToolBoxIndex);

        if (qApp->patternType() == MeasurementsType::Multisize)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(isDockToolOptionsVisible);

        ui->dockWidgetGroups->setWidget(groupsWidget);
        ui->dockWidgetGroups->setWindowTitle(tr("Groups of visibility"));
        ui->dockWidgetGroups->setVisible(isDockGroupsVisible);
        ui->dockWidgetGroups->setToolTip(tr("Contains all visibility groups"));
    }
    else
    {
        ui->actionDraw->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionDetails show details scene.
 * @param checked true - button checked.
 */
void MainWindow::ActionDetails(bool checked)
{
    if (checked)
    {
        ArrowTool(true);

        if(drawMode)
        {
            currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern piece
            drawMode = false;
        }
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);// Need to get data about all details

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionDetails->setChecked(true);
        ui->actionLayout->setChecked(false);

        if(not qApp->getOpeningPattern())
        {
            if (pattern->DataPieces()->count() == 0)
            {
                QMessageBox::information(this, tr("Detail mode"), tr("You can't use Detail mode yet. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                ActionDraw(true);
                return;
            }
        }

        detailsWidget->UpdateList();

        qCDebug(vMainWindow, "Show details scene");
        SaveCurrentScene();

        currentScene = sceneDetails;
        emit ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);
        RestoreCurrentScene();

        if (qApp->GetDrawMode() == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        qApp->SetDrawMode(Draw::Modeling);
        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(ui->toolBox->indexOf(ui->detailPage));

        if (qApp->patternType() == MeasurementsType::Multisize)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(isDockToolOptionsVisible);

        ui->dockWidgetGroups->setWidget(detailsWidget);
        ui->dockWidgetGroups->setWindowTitle(tr("Details"));
        ui->dockWidgetGroups->setVisible(isDockGroupsVisible);
        ui->dockWidgetGroups->setToolTip(tr("Show which details will go in layout"));

        m_statusLabel->setText(QString());
    }
    else
    {
        ui->actionDetails->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionLayout begin creation layout.
 * @param checked true - button checked.
 */
void MainWindow::ActionLayout(bool checked)
{
    if (checked)
    {
        ArrowTool(true);

        if(drawMode)
        {
            currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern piece
            drawMode = false;
        }
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);// Need to get data about all details

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_right_to_left_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(true);

        QVector<DetailForLayout> details;
        if(not qApp->getOpeningPattern())
        {
            const QHash<quint32, VPiece> *allDetails = pattern->DataPieces();
            if (allDetails->count() == 0)
            {
                QMessageBox::information(this, tr("Layout mode"), tr("You can't use Layout mode yet. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                ActionDraw(true);
                return;
            }
            else
            {
                details = SortDetailsForLayout(allDetails);

                if (details.count() == 0)
                {
                    QMessageBox::information(this, tr("Layout mode"),  tr("You can't use Layout mode yet. Please, "
                                                                          "include at least one detail in layout."),
                                             QMessageBox::Ok, QMessageBox::Ok);
                    qApp->GetDrawMode() == Draw::Calculation ? ActionDraw(true) : ActionDetails(true);
                    return;
                }
            }
        }

        comboBoxDraws->setCurrentIndex(-1);// Hide pattern pieces

        qCDebug(vMainWindow, "Show layout scene");

        SaveCurrentScene();

        try
        {
            listDetails = PrepareDetailsForLayout(details);
        }
        catch (VException &e)
        {
            listDetails.clear();
            QMessageBox::warning(this, tr("Layout mode"),
                                 tr("You can't use Layout mode yet.") + QLatin1String(" \n") + e.ErrorMessage(),
                                 QMessageBox::Ok, QMessageBox::Ok);
            qApp->GetDrawMode() == Draw::Calculation ? ActionDraw(true) : ActionDetails(true);
            return;
        }

        currentScene = tempSceneLayout;
        emit ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);

        if (qApp->GetDrawMode() == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        qApp->SetDrawMode(Draw::Layout);
        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(ui->toolBox->indexOf(ui->layoutPage));

        if (not m_mouseCoordinate.isNull())
        {
            m_mouseCoordinate->setText(QString());
        }

        if (qApp->patternType() == MeasurementsType::Multisize)
        {
            ui->toolBarOption->setVisible(false);
        }

        ui->dockWidgetLayoutPages->setVisible(true);

        ui->dockWidgetToolOptions->blockSignals(true);
        ui->dockWidgetToolOptions->setVisible(false);
        ui->dockWidgetToolOptions->blockSignals(false);

        ui->dockWidgetGroups->blockSignals(true);
        ui->dockWidgetGroups->setVisible(false);
        ui->dockWidgetGroups->blockSignals(false);

        ShowPaper(ui->listWidget->currentRow());

        if (scenes.isEmpty() || isLayoutStale)
        {
            ui->toolButtonLayoutSettings->click();
        }

        m_statusLabel->setText(QString());
    }
    else
    {
        ui->actionLayout->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief on_actionSaveAs_triggered save as pattern file.
 * @return true for successes saving.
 */
bool MainWindow::on_actionSaveAs_triggered()
{
    const QString oldFilePath = qApp->GetPatternPath();
    QString filters(tr("Pattern files") + QLatin1String("(*.val)"));
    QString dir;
    if (qApp->GetPatternPath().isEmpty())
    {
        dir = qApp->ValentinaSettings()->GetPathPattern();
    }
    else
    {
        dir = QFileInfo(qApp->GetPatternPath()).absolutePath();
    }

    bool usedNotExistedDir = false;
    QDir directory(dir);
    if (not directory.exists())
    {
        usedNotExistedDir = directory.mkpath(QChar('.'));
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"),
                                                    dir + QLatin1String("/") + tr("pattern") + QLatin1String(".val"),
                                                    filters, nullptr
#ifdef Q_OS_LINUX
                                                    , QFileDialog::DontUseNativeDialog
#endif
                                                    );

    auto RemoveTempDir = qScopeGuard([usedNotExistedDir, dir]()
    {
        if (usedNotExistedDir)
        {
            QDir directory(dir);
            directory.rmpath(QChar('.'));
        }
    });

    if (fileName.isEmpty())
    {
        return false;
    }

    QFileInfo f( fileName );
    if (f.suffix().isEmpty() && f.suffix() != QLatin1String("val"))
    {
        fileName += QLatin1String(".val");
    }

    if (f.exists() && qApp->GetPatternPath() != fileName)
    {
        // Temporary try to lock the file before saving
        // Also help to rewite current read-only pattern
        VLockGuard<char> tmp(fileName);
        if (not tmp.IsLocked())
        {
            qCCritical(vMainWindow, "%s",
                       qUtf8Printable(tr("Failed to lock. This file already opened in another window.")));
            return false;
        }
    }

    // Need for restoring previous state in case of failure
    const bool wasModified = doc->IsModified(); // Need because SetReadOnly() will change internal state
    const bool readOnly = doc->IsReadOnly();

    doc->SetReadOnly(false);// Save as... disable read only state
    QString error;
    const bool result = SavePattern(fileName, error);
    if (result == false)
    {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();

        // Restoring previous state
        doc->SetReadOnly(readOnly);
        doc->SetModified(wasModified);

        return result;
    }
    else if (not oldFilePath.isEmpty())
    {
        qCDebug(vMainWindow, "Updating restore file list.");
        QStringList restoreFiles = qApp->ValentinaSettings()->GetRestoreFileList();
        restoreFiles.removeAll(oldFilePath);
        qApp->ValentinaSettings()->SetRestoreFileList(restoreFiles);
        QFile::remove(oldFilePath + *autosavePrefix);
    }

    patternReadOnly = false;

    qCDebug(vMainWindow, "Locking file");
    if (qApp->GetPatternPath() == fileName && not lock.isNull())
    {
        lock->Unlock();
    }
    VlpCreateLock(lock, fileName);

    if (lock->IsLocked())
    {
        qCDebug(vMainWindow, "Pattern file %s was locked.", qUtf8Printable(fileName));
    }
    else
    {
        qCDebug(vMainWindow, "Failed to lock %s", qUtf8Printable(fileName));
        qCDebug(vMainWindow, "Error type: %d", lock->GetLockError());
        qCCritical(vMainWindow, "%s",
                   qUtf8Printable(tr("Failed to lock. This file already opened in another window. Expect "
                                     "collissions when run 2 copies of the program.")));
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief on_actionSave_triggered save pattern file.
 * @return true for successes saving.
 */
bool MainWindow::on_actionSave_triggered()
{
    if (qApp->GetPatternPath().isEmpty() || patternReadOnly)
    {
        return on_actionSaveAs_triggered();
    }
    else
    {
        if (m_curFileFormatVersion < VPatternConverter::PatternMaxVer
                && not ContinueFormatRewrite(m_curFileFormatVersionStr, VPatternConverter::PatternMaxVerStr))
        {
            return false;
        }
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/
        const bool isFileWritable = QFileInfo(qApp->GetPatternPath()).isWritable();
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

        if (not isFileWritable)
        {
            QMessageBox messageBox(this);
            messageBox.setIcon(QMessageBox::Question);
            messageBox.setText(tr("The document has no write permissions."));
            messageBox.setInformativeText("Do you want to change the premissions?");
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            messageBox.setDefaultButton(QMessageBox::Yes);

            if (messageBox.exec() == QMessageBox::Yes)
            {
#ifdef Q_OS_WIN32
                qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/
                bool changed = QFile::setPermissions(qApp->GetPatternPath(),
                                                    QFileInfo(qApp->GetPatternPath()).permissions() | QFileDevice::WriteUser);
#ifdef Q_OS_WIN32
                qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

                if (not changed)
                {
                    QMessageBox messageBox(this);
                    messageBox.setIcon(QMessageBox::Warning);
                    messageBox.setText(tr("Cannot set permissions for %1 to writable.").arg(qApp->GetPatternPath()));
                    messageBox.setInformativeText(tr("Could not save the file."));
                    messageBox.setDefaultButton(QMessageBox::Ok);
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.exec();
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        QString error;
        bool result = SavePattern(qApp->GetPatternPath(), error);
        if (result)
        {
            QFile::remove(qApp->GetPatternPath() + *autosavePrefix);
            m_curFileFormatVersion = VPatternConverter::PatternMaxVer;
            m_curFileFormatVersionStr = VPatternConverter::PatternMaxVerStr;
        }
        else
        {
            QMessageBox messageBox(this);
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setText(tr("Could not save the file"));
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.setDetailedText(error);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();
        }
        return result;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief on_actionOpen_triggered ask user select pattern file.
 */
void MainWindow::on_actionOpen_triggered()
{
    qCDebug(vMainWindow, "Openning new file.");
    const QString filter(tr("Pattern files") + QLatin1String(" (*.val)"));
    //Get list last open files
    const QStringList files = qApp->ValentinaSettings()->GetRecentFileList();
    QString dir;
    if (files.isEmpty())
    {
        dir = QDir::homePath();
    }
    else
    {
        //Absolute path to last open file
        dir = QFileInfo(files.first()).absolutePath();
    }
    qCDebug(vMainWindow, "Run QFileDialog::getOpenFileName: dir = %s.", qUtf8Printable(dir));
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), dir, filter, nullptr
#ifdef Q_OS_LINUX
                                                          , QFileDialog::DontUseNativeDialog
#endif
                                                          );
    if (filePath.isEmpty())
    {
        return;
    }
    LoadPattern(filePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear reset to default window.
 */
void MainWindow::Clear()
{
    qCDebug(vMainWindow, "Reseting main window.");
    lock.reset();
    qCDebug(vMainWindow, "Unlocked pattern file.");
    ActionDraw(true);
    qCDebug(vMainWindow, "Returned to Draw mode.");
    setCurrentFile(QString());// Keep before cleaning a pattern data to prevent a crash
    pattern->Clear();
    qCDebug(vMainWindow, "Clearing pattern.");
    if (not qApp->GetPatternPath().isEmpty() && not doc->MPath().isEmpty())
    {
        watcher->removePath(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()));
    }
    doc->clear();
    UpdateVisibilityGroups();
    detailsWidget->UpdateList();
    qCDebug(vMainWindow, "Clearing scenes.");
    sceneDraw->clear();
    sceneDetails->clear();
    ArrowTool(true);
    comboBoxDraws->clear();
    ui->actionDraw->setEnabled(false);
    ui->actionDetails->setEnabled(false);
    ui->actionLayout->setEnabled(false);
    ui->actionNewDraw->setEnabled(false);
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->actionPattern_properties->setEnabled(false);
    ui->actionZoomIn->setEnabled(false);
    ui->actionZoomOut->setEnabled(false);
    ui->actionZoomFitBest->setEnabled(false);
    ui->actionZoomFitBestCurrent->setEnabled(false);
    ui->actionZoomOriginal->setEnabled(false);
    ui->actionHistory->setEnabled(false);
    ui->actionExportRecipe->setEnabled(false);
    ui->actionTable->setEnabled(false);
    ui->actionExportIncrementsToCSV->setEnabled(false);
    ui->actionExportFinalMeasurementsToCSV->setEnabled(false);
    ui->actionFinalMeasurements->setEnabled(false);
    ui->actionLast_tool->setEnabled(false);
    ui->actionShowCurveDetails->setEnabled(false);
    ui->actionShowMainPath->setEnabled(false);
    ui->actionLoadIndividual->setEnabled(false);
    ui->actionLoadMultisize->setEnabled(false);
    ui->actionUnloadMeasurements->setEnabled(false);
    ui->actionEditCurrent->setEnabled(false);
    ui->actionPreviousPatternPiece->setEnabled(false);
    ui->actionNextPatternPiece->setEnabled(false);
    SetEnableTool(false);
    qApp->setPatternUnit(Unit::Cm);
    qApp->setPatternType(MeasurementsType::Unknown);
    ui->toolBarOption->clear();
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    CleanLayout();
    listDetails.clear(); // don't move to CleanLayout()
    qApp->getUndoStack()->clear();
    toolOptions->ClearPropertyBrowser();
    toolOptions->itemClicked(nullptr);
    m_progressBar->setVisible(false);
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarProgress->setVisible(false);
#endif
    m_statusLabel->setVisible(true);
    ui->actionIncreaseLabelFont->setEnabled(false);
    ui->actionDecreaseLabelFont->setEnabled(false);
    ui->actionOriginalLabelFont->setEnabled(false);
    ui->actionHideLabels->setEnabled(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::FileClosedCorrect()
{
    WriteSettings();

    //File was closed correct.
    QStringList restoreFiles = qApp->ValentinaSettings()->GetRestoreFileList();
    restoreFiles.removeAll(qApp->GetPatternPath());
    qApp->ValentinaSettings()->SetRestoreFileList(restoreFiles);

    // Remove autosave file
    QFile autofile(qApp->GetPatternPath() + *autosavePrefix);
    if (autofile.exists())
    {
        autofile.remove();
    }
    qCDebug(vMainWindow, "File %s closed correct.", qUtf8Printable(qApp->GetPatternPath()));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::FullParseFile()
{
    qCDebug(vMainWindow, "Full parsing file");

    toolOptions->ClearPropertyBrowser();
    QFuture<void> futureTestUniqueId;

    auto WaitForFutureFinish = [](QFuture<void> &futureTestUniqueId)
    {
        try
        {
            futureTestUniqueId.waitForFinished();
        }
        catch (...)
        {
            // ignore
        }
    };

    try
    {
        if (qApp->getOpeningPattern())
        {
            futureTestUniqueId = QtConcurrent::run(static_cast<VDomDocument *>(doc), &VDomDocument::TestUniqueId);
        }

        SetEnabledGUI(true);
        doc->Parse(Document::FullParse);

        if (qApp->getOpeningPattern())
        {
            futureTestUniqueId.waitForFinished();
        }
    }
    catch (const VExceptionUndo &e)
    {
        Q_UNUSED(e)
        /* If user want undo last operation before undo we need finish broken redo operation. For those we post event
         * myself. Later in method customEvent call undo.*/
        if (qApp->getOpeningPattern())
        {
            try
            {
                futureTestUniqueId.waitForFinished();
            }
            catch (const VExceptionWrongId &e)
            {
                qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id.")),
                                       qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
                SetEnabledGUI(false);
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_DATAERR);
                }
                return;
            }
        }
        QApplication::postEvent(this, new UndoEvent());
        return;
    }
    catch (const VExceptionObjectError &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")), //-V807
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }
    catch (const std::bad_alloc &)
    {
        qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Error parsing file (std::bad_alloc).")));
        SetEnabledGUI(false);
        if (qApp->getOpeningPattern())
        {
            WaitForFutureFinish(futureTestUniqueId);
        }
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return;
    }

    QString patternPiece;
    if (comboBoxDraws->currentIndex() != -1)
    {
        patternPiece = comboBoxDraws->itemText(comboBoxDraws->currentIndex());
    }
    comboBoxDraws->blockSignals(true);
    comboBoxDraws->clear();

    QStringList patternPieceNames = doc->getPatternPieces();
    patternPieceNames.sort();
    comboBoxDraws->addItems(patternPieceNames);

    if (not drawMode)
    {
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);
    }
    else
    {
        const qint32 index = comboBoxDraws->findText(patternPiece);
        if ( index != -1 )
        {
            comboBoxDraws->setCurrentIndex(index);
        }
    }
    comboBoxDraws->blockSignals(false);
    ui->actionPattern_properties->setEnabled(true);

    GlobalChangePP(patternPiece);

    SetEnableTool(comboBoxDraws->count() > 0);
    detailsWidget->UpdateList();

    VMainGraphicsView::NewSceneRect(sceneDraw, qApp->getSceneView());
    VMainGraphicsView::NewSceneRect(sceneDetails, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::GlobalChangePP(const QString &patternPiece)
{
    const qint32 index = comboBoxDraws->findText(patternPiece);
    try
    {
        if ( index != -1 )
        { // -1 for not found
            ChangePP(index, false);
            comboBoxDraws->blockSignals(true);
            comboBoxDraws->setCurrentIndex(index);
            comboBoxDraws->blockSignals(false);
        }
        else
        {
            ChangePP(0, false);
        }
    }
    catch (VExceptionBadId &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Bad id.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PreviousPatternPiece()
{
    int index = comboBoxDraws->currentIndex();

    if (index == -1 || comboBoxDraws->count() <= 1)
    {
        return;
    }

    if (index == 0)
    {
        index = comboBoxDraws->count() - 1;
    }
    else
    {
        --index;
    }

    comboBoxDraws->setCurrentIndex(index);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::NextPatternPiece()
{
    int index = comboBoxDraws->currentIndex();

    if (index == -1 || comboBoxDraws->count() <= 1)
    {
        return;
    }

    if (index == comboBoxDraws->count()-1)
    {
        index = 0;
    }
    else
    {
        ++index;
    }

    comboBoxDraws->setCurrentIndex(index);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetEnabledGUI(bool enabled)
{
    if (guiEnabled != enabled)
    {
        if (enabled == false)
        {
            ArrowTool(true);
            qApp->getUndoStack()->clear();
        }
        SetEnableWidgets(enabled);

        guiEnabled = enabled;

        SetEnableTool(enabled);
        ui->toolBarOption->setEnabled(enabled);
    #ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
    #endif
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetEnableWidgets enable action button.
 * @param enable enable value.
 */
void MainWindow::SetEnableWidgets(bool enable)
{
    const bool drawStage = (qApp->GetDrawMode() == Draw::Calculation);
    const bool detailsStage = (qApp->GetDrawMode() == Draw::Modeling);
    const bool designStage = (drawStage || detailsStage);

    comboBoxDraws->setEnabled(enable && drawStage);
    ui->actionOptionDraw->setEnabled(enable && drawStage);
    ui->actionSave->setEnabled(isWindowModified() && enable && not patternReadOnly);
    ui->actionSaveAs->setEnabled(enable);
    ui->actionPattern_properties->setEnabled(enable && designStage);
    ui->actionZoomIn->setEnabled(enable);
    ui->actionZoomOut->setEnabled(enable);
    ui->actionArrowTool->setEnabled(enable && designStage);
    ui->actionHistory->setEnabled(enable && drawStage);
    ui->actionExportRecipe->setEnabled(enable && drawStage);
    ui->actionNewDraw->setEnabled(enable && drawStage);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionLayout->setEnabled(enable);
    ui->actionTable->setEnabled(enable && drawStage);
    ui->actionExportIncrementsToCSV->setEnabled(enable);
    ui->actionExportFinalMeasurementsToCSV->setEnabled(enable);
    ui->actionFinalMeasurements->setEnabled(enable);
    ui->actionZoomFitBest->setEnabled(enable);
    ui->actionZoomFitBestCurrent->setEnabled(enable && drawStage);
    ui->actionZoomOriginal->setEnabled(enable);
    ui->actionShowCurveDetails->setEnabled(enable && drawStage);
    ui->actionShowMainPath->setEnabled(enable && detailsStage);
    ui->actionLoadIndividual->setEnabled(enable && designStage);
    ui->actionLoadMultisize->setEnabled(enable && designStage);
    ui->actionUnloadMeasurements->setEnabled(enable && designStage);
    ui->actionPreviousPatternPiece->setEnabled(enable && drawStage);
    ui->actionNextPatternPiece->setEnabled(enable && drawStage);
    ui->actionIncreaseLabelFont->setEnabled(enable);
    ui->actionDecreaseLabelFont->setEnabled(enable);
    ui->actionOriginalLabelFont->setEnabled(enable);
    ui->actionHideLabels->setEnabled(enable);

    actionDockWidgetToolOptions->setEnabled(enable && designStage);
    actionDockWidgetGroups->setEnabled(enable && designStage);

    undoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canRedo());

    //Now we don't want allow user call context menu
    sceneDraw->SetDisableTools(!enable, doc->GetNameActivPP());
    ui->view->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateHeightsList(const QStringList &list)
{
    QString val;
    if (gradationHeights->currentIndex() != -1)
    {
        val = gradationHeights->currentText();
    }

    gradationHeights->blockSignals(true);
    gradationHeights->clear();
    gradationHeights->addItems(list);
    gradationHeights->blockSignals(false);

    int index = gradationHeights->findText(val);
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        ChangedHeight(list.at(0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateSizesList(const QStringList &list)
{
    QString val;
    if (gradationSizes->currentIndex() != -1)
    {
        val = gradationSizes->currentText();
    }

    gradationSizes->blockSignals(true);
    gradationSizes->clear();
    gradationSizes->addItems(list);
    gradationSizes->blockSignals(false);

    int index = gradationSizes->findText(val);
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        ChangedSize(list.at(0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief on_actionNew_triggered create new empty pattern.
 */
void MainWindow::on_actionNew_triggered()
{
    if (comboBoxDraws->count() == 0)
    {
        qCDebug(vMainWindow, "New PP.");
        QString patternPieceName = tr("Pattern piece %1").arg(comboBoxDraws->count()+1);
        qCDebug(vMainWindow, "Generated PP name: %s", qUtf8Printable(patternPieceName));

        qCDebug(vMainWindow, "First PP");
        DialogNewPattern newPattern(pattern, patternPieceName, this);
        if (newPattern.exec() == QDialog::Accepted)
        {
            patternPieceName = newPattern.name();
            qApp->setPatternUnit(newPattern.PatternUnit());
            qCDebug(vMainWindow, "PP name: %s", qUtf8Printable(patternPieceName));
        }
        else
        {
            qCDebug(vMainWindow, "Creation a new pattern was canceled.");
            return;
        }

        //Set scene size to size scene view
        VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
        VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

        AddPP(patternPieceName);

        m_mouseCoordinate = new QLabel(QString("0, 0 (%1)").arg(UnitsToStr(qApp->patternUnit(), true)));
        ui->toolBarOption->addWidget(m_mouseCoordinate);

        m_curFileFormatVersion = VPatternConverter::PatternMaxVer;
        m_curFileFormatVersionStr = VPatternConverter::PatternMaxVerStr;

        ToolBarOption();
    }
    else
    {
        OpenNewValentina();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveChange enable action save if we have unsaved change.
 */
void MainWindow::PatternChangesWereSaved(bool saved)
{
    if (guiEnabled)
    {
        const bool state = doc->IsModified() || !saved;
        setWindowModified(state);
        not patternReadOnly ? ui->actionSave->setEnabled(state): ui->actionSave->setEnabled(false);
        isLayoutStale = true;
        isNeedAutosave = not saved;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedSize change new size value.
 * @param text value size.
 */
void MainWindow::ChangedSize(const QString & text)
{
    const int size = static_cast<int>(pattern->size());
    if (UpdateMeasurements(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()), text.toInt(),
                           static_cast<int>(pattern->height())))
    {
        doc->LiteParseTree(Document::FullLiteParse);
        emit sceneDetails->DimensionsChanged();
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = gradationSizes->findText(QString().setNum(size));
        if (index != -1)
        {
            gradationSizes->setCurrentIndex(index);
        }
        else
        {
            qCDebug(vMainWindow, "Couldn't restore size value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedGrowth change new height value.
 * @param text value height.
 */
void MainWindow::ChangedHeight(const QString &text)
{
    const int height = static_cast<int>(pattern->height());
    if (UpdateMeasurements(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()), static_cast<int>(pattern->size()),
                           text.toInt()))
    {
        doc->LiteParseTree(Document::FullLiteParse);
        emit sceneDetails->DimensionsChanged();
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = gradationHeights->findText(QString().setNum(height));
        if (index != -1)
        {
            gradationHeights->setCurrentIndex(index);
        }
        else
        {
            qCDebug(vMainWindow, "Couldn't restore height value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowProgress()
{
    if (m_progressBar->isVisible() && m_progressBar->value() + 1 <= m_progressBar->maximum())
    {
        const int newValue = m_progressBar->value() + 1;
        m_progressBar->setValue(newValue);
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
        m_taskbarProgress->setValue(newValue);
#endif
        qApp->processEvents();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultHeight()
{
    const QString defHeight = QString().setNum(doc->GetDefCustomHeight());
    int index = gradationHeights->findText(defHeight);
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        index = gradationHeights->findText(QString().setNum(pattern->height()));
        if (index != -1)
        {
            gradationHeights->setCurrentIndex(index);
        }
    }
    pattern->SetHeight(gradationHeights->currentText().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultSize()
{
    const QString defSize = QString().setNum(doc->GetDefCustomSize());
    int index = gradationSizes->findText(defSize);
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        index = gradationSizes->findText(QString().setNum(pattern->size()));
        if (index != -1)
        {
            gradationSizes->setCurrentIndex(index);
        }
    }
    pattern->SetSize(gradationSizes->currentText().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetEnableTool enable button.
 * @param enable enable value.
 */
void MainWindow::SetEnableTool(bool enable)
{
    bool drawTools = false;
    bool modelingTools = false;
    bool layoutTools = false;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
    switch (qApp->GetDrawMode())
    {
        case Draw::Calculation:
            drawTools = enable;
            break;
        case Draw::Modeling:
            modelingTools = enable;
            break;
        case Draw::Layout:
            layoutTools = enable;
            break;
    }
QT_WARNING_POP

    // This check helps to find missed tools
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Not all tools were handled.");

    //Drawing Tools
    ui->toolButtonEndLine->setEnabled(drawTools);
    ui->toolButtonLine->setEnabled(drawTools);
    ui->toolButtonAlongLine->setEnabled(drawTools);
    ui->toolButtonShoulderPoint->setEnabled(drawTools);
    ui->toolButtonNormal->setEnabled(drawTools);
    ui->toolButtonBisector->setEnabled(drawTools);
    ui->toolButtonLineIntersect->setEnabled(drawTools);
    ui->toolButtonSpline->setEnabled(drawTools);
    ui->toolButtonCubicBezier->setEnabled(drawTools);
    ui->toolButtonArc->setEnabled(drawTools);
    ui->toolButtonSplinePath->setEnabled(drawTools);
    ui->toolButtonCubicBezierPath->setEnabled(drawTools);
    ui->toolButtonPointOfContact->setEnabled(drawTools);
    ui->toolButtonNewDetail->setEnabled(drawTools);
    ui->toolButtonInternalPath->setEnabled(drawTools);
    ui->toolButtonHeight->setEnabled(drawTools);
    ui->toolButtonTriangle->setEnabled(drawTools);
    ui->toolButtonPointOfIntersection->setEnabled(drawTools);
    ui->toolButtonSplineCutPoint->setEnabled(drawTools);
    ui->toolButtonSplinePathCutPoint->setEnabled(drawTools);
    ui->toolButtonArcCutPoint->setEnabled(drawTools);
    ui->toolButtonLineIntersectAxis->setEnabled(drawTools);
    ui->toolButtonCurveIntersectAxis->setEnabled(drawTools);
    ui->toolButtonArcIntersectAxis->setEnabled(drawTools);
    ui->toolButtonPointOfIntersectionArcs->setEnabled(drawTools);
    ui->toolButtonIntersectionCurves->setEnabled(drawTools);
    ui->toolButtonPointOfIntersectionCircles->setEnabled(drawTools);
    ui->toolButtonPointFromCircleAndTangent->setEnabled(drawTools);
    ui->toolButtonPointFromArcAndTangent->setEnabled(drawTools);
    ui->toolButtonArcWithLength->setEnabled(drawTools);
    ui->toolButtonTrueDarts->setEnabled(drawTools);
    ui->toolButtonGroup->setEnabled(drawTools);
    ui->toolButtonRotation->setEnabled(drawTools);
    ui->toolButtonFlippingByLine->setEnabled(drawTools);
    ui->toolButtonFlippingByAxis->setEnabled(drawTools);
    ui->toolButtonMove->setEnabled(drawTools);
    ui->toolButtonMidpoint->setEnabled(drawTools);
    ui->toolButtonEllipticalArc->setEnabled(drawTools);
    ui->toolButtonPin->setEnabled(drawTools);
    ui->toolButtonInsertNode->setEnabled(drawTools);
    ui->toolButtonPlaceLabel->setEnabled(drawTools);

    ui->actionLast_tool->setEnabled(drawTools);

    for (auto pointer : qAsConst(toolButtonPointerList))
    {
        pointer->setEnabled(drawTools || modelingTools);
        pointer->setChecked(drawTools || modelingTools);
    }

    //Modeling Tools
    ui->toolButtonUnionDetails->setEnabled(modelingTools);
    ui->toolButtonDetailExportAs->setEnabled(modelingTools);
    ui->toolButtonDuplicateDetail->setEnabled(modelingTools);

    //Layout tools
    ui->toolButtonLayoutSettings->setEnabled(layoutTools);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetLayoutModeActions()
{
    const bool enabled = not scenes.isEmpty();

    ui->toolButtonLayoutExportAs->setEnabled(enabled);
    ui->actionExportAs->setEnabled(enabled);
    ui->actionPrintPreview->setEnabled(enabled);
    ui->actionPrintPreviewTiled->setEnabled(enabled);
    ui->actionPrint->setEnabled(enabled);
    ui->actionPrintTiled->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MinimumScrollBar set scroll bar to minimum.
 */
void MainWindow::MinimumScrollBar()
{
    QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = ui->view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SavePattern save pattern file.
 * @param fileName pattern file name.
 * @return true if all is good.
 */
bool MainWindow::SavePattern(const QString &fileName, QString &error)
{
    qCDebug(vMainWindow, "Saving pattern file %s.", qUtf8Printable(fileName));
    QFileInfo tempInfo(fileName);

    const QString mPath = AbsoluteMPath(qApp->GetPatternPath(), doc->MPath());
    if (not mPath.isEmpty() && qApp->GetPatternPath() != fileName)
    {
        doc->SetMPath(RelativeMPath(fileName, mPath));
    }

    const bool result = doc->SaveDocument(fileName, error);
    if (result)
    {
        if (tempInfo.suffix() != QLatin1String("autosave"))
        {
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("File saved"), 5000);
            qCDebug(vMainWindow, "File %s saved.", qUtf8Printable(fileName));
            PatternChangesWereSaved(result);
        }
    }
    else
    {
        doc->SetMPath(mPath);
        emit doc->UpdatePatternLabel();
        qCDebug(vMainWindow, "Could not save file %s. %s.", qUtf8Printable(fileName), qUtf8Printable(error));
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AutoSavePattern start safe saving.
 */
void MainWindow::AutoSavePattern()
{
    if (not qApp->GetPatternPath().isEmpty() && isWindowModified() && isNeedAutosave)
    {
        qCDebug(vMainWindow, "Autosaving pattern.");
        QString error;
        if (SavePattern(qApp->GetPatternPath() + *autosavePrefix, error))
        {
            isNeedAutosave = false;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentFile the function is called to reset the state of a few variables when a file
 * is loaded or saved, or when the user starts editing a new file (in which case fileName is empty).
 * @param fileName file name.
 */
void MainWindow::setCurrentFile(const QString &fileName)
{
    qCDebug(vMainWindow, "Set current name to \"%s\"", qUtf8Printable(fileName));
    qApp->SetPatternPath(fileName);
    doc->SetPatternWasChanged(true);
    emit doc->UpdatePatternLabel();
    qApp->getUndoStack()->setClean();

    if (not qApp->GetPatternPath().isEmpty() && VApplication::IsGUIMode())
    {
        qCDebug(vMainWindow, "Updating recent file list.");
        VSettings *settings = qApp->ValentinaSettings();
        QStringList files = settings->GetRecentFileList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
        {
            files.removeLast();
        }

        settings->SetRecentFileList(files);
        UpdateRecentFileActions();

        qCDebug(vMainWindow, "Updating restore file list.");
        QStringList restoreFiles = settings->GetRestoreFileList();
        restoreFiles.removeAll(fileName);
        restoreFiles.prepend(fileName);
        settings->SetRestoreFileList(restoreFiles);
    }

    UpdateWindowTitle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ReadSettings read setting for app.
 */
void MainWindow::ReadSettings()
{
    qCDebug(vMainWindow, "Reading settings.");
    const VSettings *settings = qApp->ValentinaSettings();

    if (settings->status() == QSettings::NoError)
    {
        restoreGeometry(settings->GetGeometry());
        restoreState(settings->GetWindowState());
        restoreState(settings->GetToolbarsState(), APP_VERSION);

        // Scene antialiasing
        ui->view->SetAntialiasing(settings->GetGraphicalOutput());

        // Stack limit
        qApp->getUndoStack()->setUndoLimit(settings->GetUndoCount());

        // Text under tool buton icon
        ToolBarStyles();

        isDockToolOptionsVisible = ui->dockWidgetToolOptions->isVisible();
        isDockGroupsVisible = ui->dockWidgetGroups->isVisible();
    }
    else
    {
        qWarning() << tr("Cannot read settings from a malformed .INI file.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief WriteSettings save setting for app.
 */
void MainWindow::WriteSettings()
{
    ActionDraw(true);

    VSettings *settings = qApp->ValentinaSettings();
    settings->SetGeometry(saveGeometry());
    settings->SetWindowState(saveState());
    settings->SetToolbarsState(saveState(APP_VERSION));

    settings->sync();
    if (settings->status() == QSettings::AccessError)
    {
        qWarning() << tr("Cannot save settings. Access denied.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MaybeSave The function is called to save pending changes.
 * @return returns true in all cases, except when the user clicks Cancel.
 */
bool MainWindow::MaybeSave()
{
    if (this->isWindowModified() && guiEnabled)
    {
        QScopedPointer<QMessageBox> messageBox(new QMessageBox(tr("Unsaved changes"),
                                                               tr("The pattern has been modified.\n"
                                                                  "Do you want to save your changes?"),
                                                               QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No,
                                                               QMessageBox::Cancel, this, Qt::Sheet));

        messageBox->setDefaultButton(QMessageBox::Yes);
        messageBox->setEscapeButton(QMessageBox::Cancel);

        messageBox->setButtonText(QMessageBox::Yes,
                                  qApp->GetPatternPath().isEmpty() || patternReadOnly ? tr("Save…") : tr("Save"));
        messageBox->setButtonText(QMessageBox::No, tr("Don't Save"));

        messageBox->setWindowModality(Qt::ApplicationModal);
        const auto ret = static_cast<QMessageBox::StandardButton>(messageBox->exec());

        switch (ret)
        {
            case QMessageBox::Yes:
                if (patternReadOnly)
                {
                    return on_actionSaveAs_triggered();
                }
                else
                {
                    return on_actionSave_triggered();
                }
            case QMessageBox::No:
                return true;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateMenus()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        ui->menuFile->insertAction(ui->actionPreferences, m_recentFileActs.at(i));
    }
    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);
    ui->menuFile->insertAction(ui->actionPreferences, m_separatorAct);
    UpdateRecentFileActions();

    //Add Undo/Redo actions.
    undoAction = qApp->getUndoStack()->createUndoAction(this, tr("&Undo"));
    connect(undoAction, &QAction::triggered, toolOptions, &VToolOptionsPropertyBrowser::RefreshOptions);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, undoAction);
    ui->toolBarTools->addAction(undoAction);

    redoAction = qApp->getUndoStack()->createRedoAction(this, tr("&Redo"));
    connect(redoAction, &QAction::triggered, toolOptions, &VToolOptionsPropertyBrowser::RefreshOptions);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, redoAction);
    ui->toolBarTools->addAction(redoAction);

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, m_separatorAct);

    AddDocks();
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
void MainWindow::LastUsedTool()
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Not all tools were handled.");

    if (currentTool == lastUsedTool)
    {
        return;
    }

    switch ( lastUsedTool )
    {
        case Tool::Arrow:
            for (auto pointer : qAsConst(toolButtonPointerList))
            {
                pointer->setChecked(true);
            }
            ArrowTool(true);
            break;
        case Tool::BasePoint:
        case Tool::SinglePoint:
        case Tool::DoublePoint:
        case Tool::LinePoint:
        case Tool::AbstractSpline:
        case Tool::Cut:
        case Tool::LAST_ONE_DO_NOT_USE:
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeElArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
            Q_UNREACHABLE(); //-V501
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLine:
            ui->toolButtonEndLine->setChecked(true);
            ToolEndLine(true);
            break;
        case Tool::Line:
            ui->toolButtonLine->setChecked(true);
            ToolLine(true);
            break;
        case Tool::AlongLine:
            ui->toolButtonAlongLine->setChecked(true);
            ToolAlongLine(true);
            break;
        case Tool::Midpoint:
            ui->toolButtonMidpoint->setChecked(true);
            ToolMidpoint(true);
            break;
        case Tool::ShoulderPoint:
            ui->toolButtonShoulderPoint->setChecked(true);
            ToolShoulderPoint(true);
            break;
        case Tool::Normal:
            ui->toolButtonNormal->setChecked(true);
            ToolNormal(true);
            break;
        case Tool::Bisector:
            ui->toolButtonBisector->setChecked(true);
            ToolBisector(true);
            break;
        case Tool::LineIntersect:
            ui->toolButtonLineIntersect->setChecked(true);
            ToolLineIntersect(true);
            break;
        case Tool::Spline:
            ui->toolButtonSpline->setChecked(true);
            ToolSpline(true);
            break;
        case Tool::CubicBezier:
            ui->toolButtonCubicBezier->setChecked(true);
            ToolCubicBezier(true);
            break;
        case Tool::Arc:
            ui->toolButtonArc->setChecked(true);
            ToolArc(true);
            break;
        case Tool::SplinePath:
            ui->toolButtonSplinePath->setChecked(true);
            ToolSplinePath(true);
            break;
        case Tool::CubicBezierPath:
            ui->toolButtonCubicBezierPath->setChecked(true);
            ToolCubicBezierPath(true);
            break;
        case Tool::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(true);
            ToolPointOfContact(true);
            break;
        case Tool::Piece:
            ui->toolButtonNewDetail->setChecked(true);
            ToolDetail(true);
            break;
        case Tool::PiecePath:
            ui->toolButtonInternalPath->setChecked(true);
            ToolPiecePath(true);
            break;
        case Tool::Height:
            ui->toolButtonHeight->setChecked(true);
            ToolHeight(true);
            break;
        case Tool::Triangle:
            ui->toolButtonTriangle->setChecked(true);
            ToolTriangle(true);
            break;
        case Tool::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(true);
            ToolPointOfIntersection(true);
            break;
        case Tool::PointOfIntersectionArcs:
            ui->toolButtonPointOfIntersectionArcs->setChecked(true);
            ToolPointOfIntersectionArcs(true);
            break;
        case Tool::CutSpline:
            ui->toolButtonSplineCutPoint->setChecked(true);
            ToolCutSpline(true);
            break;
        case Tool::CutSplinePath:
            ui->toolButtonSplinePathCutPoint->setChecked(true);
            ToolCutSplinePath(true);
            break;
        case Tool::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(true);
            ToolUnionDetails(true);
            break;
        case Tool::DuplicateDetail:
            ui->toolButtonDuplicateDetail->setChecked(true);
            ToolDuplicateDetail(true);
            break;
        case Tool::CutArc:
            ui->toolButtonArcCutPoint->setChecked(true);
            ToolCutArc(true);
            break;
        case Tool::LineIntersectAxis:
            ui->toolButtonLineIntersectAxis->setChecked(true);
            ToolLineIntersectAxis(true);
            break;
        case Tool::CurveIntersectAxis:
            ui->toolButtonCurveIntersectAxis->setChecked(true);
            ToolCurveIntersectAxis(true);
            break;
        case Tool::ArcIntersectAxis:
            ui->toolButtonArcIntersectAxis->setChecked(true);
            ToolArcIntersectAxis(true);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(true);
            ToolPointOfIntersectionCircles(true);
            break;
        case Tool::PointOfIntersectionCurves:
            ui->toolButtonIntersectionCurves->setChecked(true);
            ToolPointOfIntersectionCurves(true);
            break;
        case Tool::PointFromCircleAndTangent:
            ui->toolButtonPointFromCircleAndTangent->setChecked(true);
            ToolPointFromCircleAndTangent(true);
            break;
        case Tool::PointFromArcAndTangent:
            ui->toolButtonPointFromArcAndTangent->setChecked(true);
            ToolPointFromArcAndTangent(true);
            break;
        case Tool::ArcWithLength:
            ui->toolButtonArcWithLength->setChecked(true);
            ToolArcWithLength(true);
            break;
        case Tool::TrueDarts:
            ui->toolButtonTrueDarts->setChecked(true);
            ToolTrueDarts(true);
            break;
        case Tool::Group:
            ui->toolButtonGroup->setChecked(true);
            ToolGroup(true);
            break;
        case Tool::Rotation:
            ui->toolButtonRotation->setChecked(true);
            ToolRotation(true);
            break;
        case Tool::FlippingByLine:
            ui->toolButtonFlippingByLine->setChecked(true);
            ToolFlippingByLine(true);
            break;
        case Tool::FlippingByAxis:
            ui->toolButtonFlippingByAxis->setChecked(true);
            ToolFlippingByAxis(true);
            break;
        case Tool::Move:
            ui->toolButtonMove->setChecked(true);
            ToolMove(true);
            break;
        case Tool::EllipticalArc:
            ui->toolButtonEllipticalArc->setChecked(true);
            ToolEllipticalArc(true);
            break;
        case Tool::Pin:
            ui->toolButtonPin->setChecked(true);
            ToolPin(true);
            break;
        case Tool::InsertNode:
            ui->toolButtonInsertNode->setChecked(true);
            ToolInsertNode(true);
            break;
        case Tool::PlaceLabel:
            ui->toolButtonPlaceLabel->setChecked(true);
            ToolPlaceLabel(true);
            break;
    }
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddDocks()
{
    ui->menuWindow->addSeparator();

    //Add dock
    actionDockWidgetToolOptions = ui->dockWidgetToolOptions->toggleViewAction();
    ui->menuWindow->addAction(actionDockWidgetToolOptions);
    connect(ui->dockWidgetToolOptions, &QDockWidget::visibilityChanged, this, [this](bool visible)
    {
        isDockToolOptionsVisible = visible;
    });

    actionDockWidgetGroups = ui->dockWidgetGroups->toggleViewAction();
    ui->menuWindow->addAction(actionDockWidgetGroups);
    connect(ui->dockWidgetGroups, &QDockWidget::visibilityChanged, this, [this](bool visible)
    {
        isDockGroupsVisible = visible;
    });
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitDocksContain()
{
    qCDebug(vMainWindow, "Initialization property browser.");
    toolOptions = new VToolOptionsPropertyBrowser(ui->dockWidgetToolOptions);

    connect(ui->view, &VMainGraphicsView::itemClicked, toolOptions, &VToolOptionsPropertyBrowser::itemClicked);
    connect(doc, &VPattern::FullUpdateFromFile, toolOptions, &VToolOptionsPropertyBrowser::UpdateOptions);

    qCDebug(vMainWindow, "Initialization groups dock.");
    groupsWidget = new VWidgetGroups(doc, this);
    ui->dockWidgetGroups->setWidget(groupsWidget);
    connect(doc,&VAbstractPattern::UpdateGroups , this, &MainWindow::UpdateVisibilityGroups);

    detailsWidget = new VWidgetDetails(pattern, doc, this);
    connect(doc, &VPattern::FullUpdateFromFile, detailsWidget, &VWidgetDetails::UpdateList);
    connect(doc, &VPattern::UpdateInLayoutList, detailsWidget, &VWidgetDetails::UpdateList);
    connect(doc, &VPattern::ShowDetail, detailsWidget, &VWidgetDetails::SelectDetail);
    connect(detailsWidget, &VWidgetDetails::Highlight, sceneDetails, &VMainGraphicsScene::HighlightItem);
    detailsWidget->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenNewValentina(const QString &fileName) const
{
    if (this->isWindowModified() || qApp->GetPatternPath().isEmpty() == false)
    {
        VApplication::NewValentina(fileName);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateActions()
{
    ui->setupUi(this);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ArrowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionLayout, &QAction::triggered, this, &MainWindow::ActionLayout);

    connect(ui->actionHistory, &QAction::triggered, this, [this](bool checked)
    {
        if (checked)
        {
            dialogHistory = new DialogHistory(pattern, doc, this);
            dialogHistory->setWindowFlags(Qt::Window);
            connect(this, &MainWindow::RefreshHistory, dialogHistory.data(), &DialogHistory::UpdateHistory);
            connect(dialogHistory.data(), &DialogHistory::DialogClosed, this, [this]()
            {
                ui->actionHistory->setChecked(false);
                delete dialogHistory;
            });
            // Fix issue #526. Dialog Detail is not on top after selection second object on Mac.
            dialogHistory->setWindowFlags(dialogHistory->windowFlags() | Qt::WindowStaysOnTopHint);
            dialogHistory->show();
        }
        else
        {
            ui->actionHistory->setChecked(true);
            dialogHistory->activateWindow();
        }
    });

    connect(ui->actionExportRecipe, &QAction::triggered, this, [this]()
    {
        QString filters(tr("Recipe files") + QStringLiteral("(*.vpr)"));
        QString fileName =
                QFileDialog::getSaveFileName(this, tr("Export recipe"),
                                             QDir::homePath() + '/' + tr("recipe") + QStringLiteral(".vpr"),
                                             filters, nullptr);
        if (fileName.isEmpty())
        {
            return;
        }

        VPatternRecipe recipe(pattern, doc);
        QString error;
        if (not recipe.SaveDocument(fileName, error))
        {
            qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Could not save recipe. %1").arg(error)));
        }
    });

    connect(ui->actionNewDraw, &QAction::triggered, this, [this]()
    {
        qCDebug(vMainWindow, "New PP.");
        QString patternPieceName = tr("Pattern piece %1").arg(comboBoxDraws->count()+1);
        qCDebug(vMainWindow, "Generated PP name: %s", qUtf8Printable(patternPieceName));

        qCDebug(vMainWindow, "PP count %d", comboBoxDraws->count());
        patternPieceName = PatternPieceName(patternPieceName);
        qCDebug(vMainWindow, "PP name: %s", qUtf8Printable(patternPieceName));
        if (patternPieceName.isEmpty())
        {
            qCDebug(vMainWindow, "Name empty.");
            return;
        }

        AddPP(patternPieceName);
    });

    connect(ui->actionExportIncrementsToCSV, &QAction::triggered, this, &MainWindow::ExportDataToCSV);
    connect(ui->actionExportFinalMeasurementsToCSV, &QAction::triggered, this, &MainWindow::ExportFMeasurementsToCSV);

    connect(ui->actionTable, &QAction::triggered, this, [this]()
    {
        // Because of bug on Mac with Qt 5.11 closing this dialog causes a crash. Instead of closing we will keep
        // dialog in memory.
        if (dialogTable.isNull())
        {
            dialogTable = new DialogIncrements(pattern, doc, this);
            connect(dialogTable.data(), &DialogIncrements::UpdateProperties, toolOptions,
                    &VToolOptionsPropertyBrowser::RefreshOptions);
            dialogTable->show();
        }
        else
        {
            dialogTable->FullUpdateFromFile();
            dialogTable->RestoreAfterClose(); // Redo some moves after close
            dialogTable->isVisible() ? dialogTable->activateWindow() : dialogTable->show();
        }
    });

    connect(ui->actionFinalMeasurements, &QAction::triggered, this, [this]()
    {
        if (dialogFMeasurements.isNull())
        {
            dialogFMeasurements = new DialogFinalMeasurements(doc, this);
            dialogFMeasurements->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialogFMeasurements.data(), &DialogFinalMeasurements::finished, this, [this](int result)
            {
                if (result == QDialog::Accepted)
                {
                    doc->SetFinalMeasurements(dialogFMeasurements->FinalMeasurements());
                }
                dialogFMeasurements->close();
            });
            dialogFMeasurements->show();
        }
        else
        {
            dialogFMeasurements->activateWindow();
        }
    });

    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]()
    {
        QMessageBox::aboutQt(this, tr("About Qt"));
    });

    connect(ui->actionAbout_Valentina, &QAction::triggered, this, [this]()
    {
        DialogAboutApp *aboutDialog = new DialogAboutApp(this);
        aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        aboutDialog->show();
    });

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::Preferences);
    connect(ui->actionReportBug, &QAction::triggered, this, []()
    {
        qCDebug(vMainWindow, "Reporting bug");
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://bitbucket.org/dismine/valentina/issues/new")));
    });

    connect(ui->actionForum, &QAction::triggered, this, []()
    {
        qCDebug(vMainWindow, "Opening forum");
        QDesktopServices::openUrl(QUrl(QStringLiteral("http://valentinaproject.forumotion.me/")));
    });

    connect(ui->actionLast_tool, &QAction::triggered, this, &MainWindow::LastUsedTool);

    connect(ui->actionPattern_properties, &QAction::triggered, this, [this]()
    {
        DialogPatternProperties proper(doc, pattern, this);
        connect(&proper, &DialogPatternProperties::UpdateGradation, this, [this]()
        {
            UpdateHeightsList(VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit()));
            UpdateSizesList(VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit()));
        });
        proper.exec();
    });

    ui->actionPattern_properties->setEnabled(false);
    connect(ui->actionClosePattern, &QAction::triggered, this, [this]()
    {
        if (MaybeSave())
        {
            FileClosedCorrect();
            Clear();
        }
    });

    ui->actionShowCurveDetails->setChecked(qApp->ValentinaSettings()->IsShowCurveDetails());
    connect(ui->actionShowCurveDetails, &QAction::triggered, this, [this](bool checked)
    {
        emit ui->view->itemClicked(nullptr);
        sceneDraw->EnableDetailsMode(checked);
        qApp->ValentinaSettings()->SetShowCurveDetails(checked);
    });

    ui->actionShowMainPath->setChecked(qApp->ValentinaSettings()->IsPieceShowMainPath());
    connect(ui->actionShowMainPath, &QAction::triggered, this, [this](bool checked)
    {
        qApp->ValentinaSettings()->SetPieceShowMainPath(checked);
        const QList<quint32> ids = pattern->DataPieces()->keys();
        const bool updateChildren = false;
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        for(auto &id : ids)
        {
            try
            {
                if (VToolSeamAllowance *tool = qobject_cast<VToolSeamAllowance *>(VAbstractPattern::getTool(id)))
                {
                    tool->RefreshGeometry(updateChildren);
                }
            }
            catch(VExceptionBadId &)
            {}
        }
        QGuiApplication::restoreOverrideCursor();
    });

    connect(ui->actionLoadIndividual, &QAction::triggered, this, &MainWindow::LoadIndividual);
    connect(ui->actionLoadMultisize, &QAction::triggered, this, &MainWindow::LoadMultisize);

    connect(ui->actionOpenTape, &QAction::triggered, this, [this]()
    {
        const QString tape = qApp->TapeFilePath();
        const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();

        QStringList arguments;
        if (isNoScaling)
        {
            arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
        }

        QProcess::startDetached(tape, arguments, workingDirectory);
    });

    connect(ui->actionEditCurrent, &QAction::triggered, this, &MainWindow::ShowMeasurements);
    connect(ui->actionExportAs, &QAction::triggered, this, &MainWindow::ExportLayoutAs);
    connect(ui->actionPrintPreview, &QAction::triggered, this, &MainWindow::PrintPreviewOrigin);
    connect(ui->actionPrintPreviewTiled, &QAction::triggered, this, &MainWindow::PrintPreviewTiled);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::PrintOrigin);
    connect(ui->actionPrintTiled, &QAction::triggered, this, &MainWindow::PrintTiled);

    //Actions for recent files loaded by a main window application.
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        QAction *action = new QAction(this);
        action->setVisible(false);
        m_recentFileActs[i] = action;
        connect(m_recentFileActs[i], &QAction::triggered, this, [this]()
        {
            if (QAction *action = qobject_cast<QAction*>(sender()))
            {
                const QString filePath = action->data().toString();
                if (not filePath.isEmpty())
                {
                    LoadPattern(filePath);
                }
            }
        });
    }

    connect(ui->actionSyncMeasurements, &QAction::triggered, this, &MainWindow::SyncMeasurements);
    connect(ui->actionUnloadMeasurements, &QAction::triggered, this, &MainWindow::UnloadMeasurements);

    connect(ui->actionLabelTemplateEditor, &QAction::triggered, this, [this]()
    {
        DialogEditLabel editor(doc);
        editor.exec();
    });
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitAutoSave()
{
    //Autosaving file each 1 minutes
    delete autoSaveTimer;
    autoSaveTimer = nullptr;

    autoSaveTimer = new QTimer(this);
    autoSaveTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
    autoSaveTimer->stop();

    if (qApp->ValentinaSettings()->GetAutosaveState())
    {
        const qint32 autoTime = qApp->ValentinaSettings()->GetAutosaveTime();
        autoSaveTimer->start(autoTime*60000);
        qCDebug(vMainWindow, "Autosaving each %d minutes.", autoTime);
    }
    qApp->setAutoSaveTimer(autoSaveTimer);
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::PatternPieceName(const QString &text)
{
    QScopedPointer<QInputDialog> dlg(new QInputDialog(this));
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a new label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(text);
    QString nameDraw;
    while (1)
    {
        const bool bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty() || text == nameDraw)
        {
            return text;
        }

        if (comboBoxDraws->findText(nameDraw) == -1)
        {
            break;//repeate show dialog
        }
    }
    return nameDraw;
}

//---------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    CancelTool();
    qDeleteAll (scenes);

    delete doc;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LoadPattern open pattern file.
 * @param fileName name of file.
 */
bool MainWindow::LoadPattern(QString fileName, const QString& customMeasureFile)
{
    qCDebug(vMainWindow, "Loading new file %s.", qUtf8Printable(fileName));

    { // Convert to absolute path if need
        QFileInfo info(fileName);
        if (info.exists() && info.isRelative())
        {
            fileName = QFileInfo(QDir::currentPath() + QLatin1Char('/') + fileName).canonicalFilePath();
        }
    }

    QFuture<VPatternConverter *> futureConverter = QtConcurrent::run([fileName]()
    {
        QScopedPointer<VPatternConverter> converter(new VPatternConverter(fileName));
        return converter.take();
    });

    //We have unsaved changes or load more then one file per time
    if (OpenNewValentina(fileName))
    {
        return false;
    }

    if (fileName.isEmpty())
    {
        qCDebug(vMainWindow, "Got empty file.");
        Clear();
        return false;
    }

    try
    {
        // Here comes undocumented Valentina's feature.
        // Because app bundle in Mac OS X doesn't allow setup assosiation for Tape we must do this through Valentina
        VMeasurements m(pattern);
        m.setXMLContent(fileName);

        if (m.Type() == MeasurementsType::Multisize || m.Type() == MeasurementsType::Individual)
        {
            const QString tape = qApp->TapeFilePath();
            const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();

            QStringList arguments = QStringList() << fileName;
            if (isNoScaling)
            {
                arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
            }

            QProcess::startDetached(tape, arguments, workingDirectory);
            qApp->exit(V_EX_OK);
            return false; // stop continue processing
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        Clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    qCDebug(vMainWindow, "Loking file");
    VlpCreateLock(lock, fileName);

    if (lock->IsLocked())
    {
        qCDebug(vMainWindow, "Pattern file %s was locked.", qUtf8Printable(fileName));
    }
    else
    {
        if (not IgnoreLocking(lock->GetLockError(), fileName))
        {
            return false;
        }
    }

    // On this stage scene empty. Fit scene size to view size
    VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
    VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

    qApp->setOpeningPattern();//Begin opening file
    try
    {
        // Quick reading measurements
        doc->setXMLContent(fileName);
        const int currentFormatVersion = doc->GetFormatVersion(doc->GetFormatVersionStr());
        if (currentFormatVersion != VPatternConverter::PatternMaxVer)
        { // Because we rely on the fact that we know where is path to measurements optimization available only for
          // the latest format version
            QScopedPointer<VPatternConverter> converter(futureConverter.result());
            m_curFileFormatVersion = converter->GetCurrentFormatVersion();
            m_curFileFormatVersionStr = converter->GetFormatVersionStr();
            doc->setXMLContent(converter->Convert());
        }

        if (!customMeasureFile.isEmpty())
        {
            doc->SetMPath(RelativeMPath(fileName, customMeasureFile));
        }
        qApp->setPatternUnit(doc->MUnit());

        QString path = AbsoluteMPath(fileName, doc->MPath());
        QString fixedMPath;

        // See issue #976. Pattern can lost link to measurements
        if (path.isEmpty() && doc->RequiresMeasurements())
        {
            const QString fakeName = QStringLiteral("unknown_measurements.vit");
            // Check if exist
            fixedMPath = CheckPathToMeasurements(fileName, fakeName);
            if (fixedMPath.isEmpty())
            {
                qApp->setOpeningPattern();// End opening file
                Clear();
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("The measurements file '%1' could not be found.")
                                                             .arg(fakeName)));
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }
            else
            {
                path = AbsoluteMPath(fileName, doc->MPath());
            }
        }

        if (not path.isEmpty())
        {
            // Check if exist
            fixedMPath = CheckPathToMeasurements(fileName, path);
            if (fixedMPath.isEmpty())
            {
                qApp->setOpeningPattern();// End opening file
                Clear();
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("The measurements file '%1' could not be found.")
                                                             .arg(path)));
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }

            if (not LoadMeasurements(fixedMPath))
            {
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("The measurements file '%1' could not be found or "
                                                                "provides not enough information.")
                                                             .arg(fixedMPath)));
                qApp->setOpeningPattern();// End opening file
                Clear();
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }
            else
            {
                ui->actionUnloadMeasurements->setEnabled(true);
                watcher->addPath(fixedMPath);
                ui->actionEditCurrent->setEnabled(true);
            }
        }

        if (qApp->patternType() == MeasurementsType::Unknown)
        {// Show toolbar only if was not uploaded any measurements.
            ToolBarOption();
        }

        if (currentFormatVersion == VPatternConverter::PatternMaxVer)
        {
            // Real read
            QScopedPointer<VPatternConverter> converter(futureConverter.result());
            m_curFileFormatVersion = converter->GetCurrentFormatVersion();
            m_curFileFormatVersionStr = converter->GetFormatVersionStr();
            doc->setXMLContent(converter->Convert());
            if (!customMeasureFile.isEmpty())
            {
                doc->SetMPath(RelativeMPath(fileName, customMeasureFile));
            }
            else if (not path.isEmpty() && fixedMPath != path)
            {
                doc->SetMPath(RelativeMPath(fileName, fixedMPath));
            }
            qApp->setPatternUnit(doc->MUnit());
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        qApp->setOpeningPattern();// End opening file
        Clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    m_progressBar->setVisible(true);
    m_statusLabel->setVisible(false);
    const int elements = doc->ElementsToParse();
    m_progressBar->setMaximum(elements);
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarProgress->setVisible(true);
    m_taskbarProgress->setMaximum(elements);
#endif
    
    FullParseFile();

    m_progressBar->setVisible(false);
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarProgress->setVisible(false);
#endif
    m_statusLabel->setVisible(true);

    if (guiEnabled)
    { // No errors occurred
        if (qApp->IsGUIMode())
        {
            /* Collect garbage only after successfully parse. This way wrongly accused items have one more time to restore
             * a reference. */
            QTimer::singleShot(100, Qt::CoarseTimer, this, [this](){doc->GarbageCollector(true);});
        }

        patternReadOnly = doc->IsReadOnly();
        SetEnableWidgets(true);
        setCurrentFile(fileName);
        qCDebug(vMainWindow, "File loaded.");

        //Fit scene size to best size for first show
        ZoomFirstShow();

        ActionDraw(true);

        qApp->setOpeningPattern();// End opening file
        return true;
    }
    else
    {
        qApp->setOpeningPattern();// End opening file
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList MainWindow::GetUnlokedRestoreFileList() const
{
    QStringList restoreFiles;
    //Take all files that need to be restored
    QStringList files = qApp->ValentinaSettings()->GetRestoreFileList();
    if (files.size() > 0)
    {
        for (auto &file : files)
        {
            // Seeking file that realy need reopen
            VLockGuard<char> tmp(file);
            if (tmp.IsLocked())
            {
                restoreFiles.append(file);
            }
        }

        // Clearing list after filtering
        for (auto &file : restoreFiles)
        {
            files.removeAll(file);
        }

        // Clear all files that do not exist.
        QStringList filtered;
        for (auto &file : files)
        {
            if (QFileInfo::exists(file))
            {
                filtered.append(file);
            }
        }

        qApp->ValentinaSettings()->SetRestoreFileList(filtered);
    }
    return restoreFiles;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStyles()
{
    ToolBarStyle(ui->toolBarDraws);
    ToolBarStyle(ui->toolBarOption);
    ToolBarStyle(ui->toolBarStages);
    ToolBarStyle(ui->toolBarTools);
    ToolBarStyle(ui->mainToolBar);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowPaper(int index)
{
    if (index < 0 || index >= scenes.size())
    {
        ui->view->setScene(tempSceneLayout);
    }
    else
    {
        ui->view->setScene(scenes.at(index));
    }

    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::Preferences()
{
    // Calling constructor of the dialog take some time. Because of this user have time to call the dialog twice.
    static QPointer<DialogPreferences> guard;// Prevent any second run
    if (guard.isNull())
    {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        DialogPreferences *preferences = new DialogPreferences(this);
        // QScopedPointer needs to be sure any exception will never block guard
        QScopedPointer<DialogPreferences> dlg(preferences);
        guard = preferences;
        connect(dlg.data(), &DialogPreferences::UpdateProperties, this, &MainWindow::WindowsLocale); // Must be first
        connect(dlg.data(), &DialogPreferences::UpdateProperties, toolOptions,
                &VToolOptionsPropertyBrowser::RefreshOptions);
        connect(dlg.data(), &DialogPreferences::UpdateProperties, this, &MainWindow::ToolBarStyles);
        connect(dlg.data(), &DialogPreferences::UpdateProperties, this, [this](){emit doc->FullUpdateFromFile();});
        connect(dlg.data(), &DialogPreferences::UpdateProperties, ui->view,
                &VMainGraphicsView::ResetScrollingAnimation);
        QGuiApplication::restoreOverrideCursor();

        if (guard->exec() == QDialog::Accepted)
        {
            InitAutoSave();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_MAC)
void MainWindow::CreateMeasurements()
{
    const QString tape = qApp->TapeFilePath();
    const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();

    QStringList arguments;
    if (isNoScaling)
    {
        arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
    }

    QProcess::startDetached(tape, arguments, workingDirectory);
}
#endif

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ExportLayoutAs()
{
    auto Uncheck = qScopeGuard([this] {ui->toolButtonLayoutExportAs->setChecked(false);});

    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            return;
        }
    }

    try
    {
        m_dialogSaveLayout = QSharedPointer<DialogSaveLayout>(new DialogSaveLayout(scenes.size(), Draw::Layout,
                                                                                   FileName(), this));

        if (m_dialogSaveLayout->exec() == QDialog::Rejected)
        {
            m_dialogSaveLayout.clear();
            return;
        }

        ExportData(QVector<VLayoutPiece>());
        m_dialogSaveLayout.clear();
    }
    catch (const VException &e)
    {
        m_dialogSaveLayout.clear();
        qCritical("%s\n\n%s\n\n%s", qUtf8Printable(tr("Export error.")),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ExportDetailsAs()
{
    auto Uncheck = qScopeGuard([this] {ui->toolButtonDetailExportAs->setChecked(false);});

    QVector<DetailForLayout> detailsInLayout = SortDetailsForLayout(pattern->DataPieces());

    if (detailsInLayout.count() == 0)
    {
        QMessageBox::information(this, tr("Layout mode"),  tr("You don't have enough details to export. Please, "
                                                              "include at least one detail in layout."),
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QVector<VLayoutPiece> listDetails;
    try
    {
        listDetails = PrepareDetailsForLayout(detailsInLayout);
    }
    catch (VException &e)
    {
        QMessageBox::warning(this, tr("Export details"),
                             tr("Can't export details.") + QLatin1String(" \n") + e.ErrorMessage(),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    try
    {
        m_dialogSaveLayout = QSharedPointer<DialogSaveLayout>(new DialogSaveLayout(1, Draw::Modeling, FileName(),
                                                                                   this));

        if (m_dialogSaveLayout->exec() == QDialog::Rejected)
        {
            m_dialogSaveLayout.clear();
            return;
        }

        ExportData(listDetails);
        m_dialogSaveLayout.clear();
    }
    catch (const VException &e)
    {
        m_dialogSaveLayout.clear();
        qCritical("%s\n\n%s\n\n%s", qUtf8Printable(tr("Export error.")),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ReopenFilesAfterCrash(QStringList &args)
{
    const QStringList files = GetUnlokedRestoreFileList();
    if (files.size() > 0)
    {
        qCDebug(vMainWindow, "Reopen files after crash.");

        QStringList restoreFiles;
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(files.at(i) + *autosavePrefix);
            if (file.exists())
            {
                restoreFiles.append(files.at(i));
            }
        }

        if (restoreFiles.size() > 0)
        {
            QMessageBox::StandardButton reply;
            const QString mes = tr("Valentina didn't shut down correctly. Do you want reopen files (%1) you had open?")
                    .arg(restoreFiles.size());
            reply = QMessageBox::question(this, tr("Reopen files."), mes, QMessageBox::Yes|QMessageBox::No,
                                          QMessageBox::Yes);
            if (reply == QMessageBox::Yes)
            {
                qCDebug(vMainWindow, "User said Yes.");

                for (auto &file : restoreFiles)
                {
                    QString error;
                    if (VDomDocument::SafeCopy(file + *autosavePrefix, file, error))
                    {
                        QFile autoFile(file + *autosavePrefix);
                        autoFile.remove();
                        LoadPattern(file);
                        args.removeAll(file);// Do not open file twice after we restore him.
                    }
                    else
                    {
                        qCDebug(vMainWindow, "Could not copy %s%s to %s %s",
                                qUtf8Printable(file), qUtf8Printable(*autosavePrefix),
                                qUtf8Printable(file), qUtf8Printable(error));
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::CheckPathToMeasurements(const QString &patternPath, const QString &path)
{
    if (path.isEmpty())
    {
        return path;
    }

    auto FindLocation = [this](const QString &filter, const QString &dirPath, const QString &selectedName)
    {
        VCommonSettings::PrepareMultisizeTables(VCommonSettings::GetDefPathMultisizeMeasurements());

        bool usedNotExistedDir = false;
        QDir directory(dirPath);
        if (not directory.exists())
        {
            usedNotExistedDir = directory.mkpath(QChar('.'));
        }

        QString mPath;

        QFileDialog dialog(this, tr("Open file"), dirPath, filter);
        dialog.selectFile(selectedName);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec() == QDialog::Accepted)
        {
            mPath = dialog.selectedFiles().value(0);
        }

        if (usedNotExistedDir)
        {
            QDir(dirPath).rmpath(QChar('.'));
        }

        return mPath;
    };

    QFileInfo table(path);
    if (table.exists() == false)
    {
        if (!VApplication::IsGUIMode())
        {
            return QString();// console mode doesn't support fixing path to a measurement file
        }
        else
        {
            const QString text = tr("The measurements file <br/><br/> <b>%1</b> <br/><br/> could not be found. Do you "
                                    "want to update the file location?").arg(path);
            QMessageBox::StandardButton res = QMessageBox::question(this, tr("Loading measurements file"), text,
                                                                    QMessageBox::Yes | QMessageBox::No,
                                                                    QMessageBox::Yes);
            if (res == QMessageBox::No)
            {
                return QString();
            }
            else
            {
                MeasurementsType patternType;
                if (table.suffix() == QLatin1String("vst"))
                {
                    patternType = MeasurementsType::Multisize;
                }
                else
                {
                    patternType = MeasurementsType::Individual; // or Unknown
                }

                auto DirPath = [patternPath, table](const QString &defPath, QString &selectedName)
                {
                    QString dirPath;
                    const QDir patternDir = QFileInfo(patternPath).absoluteDir();
                    if (patternDir.exists(table.fileName()))
                    {
                        selectedName = table.fileName();
                        dirPath = patternDir.absolutePath();
                    }
                    else
                    {
                        dirPath = defPath;
                    }
                    return dirPath;
                };


                QString mPath;
                if (patternType == MeasurementsType::Multisize)
                {
                    const QString filter = tr("Multisize measurements") + QLatin1String(" (*.vst);;") +
                                           tr("Individual measurements") + QLatin1String(" (*.vit)");
                    //Use standard path to multisize measurements
                    QString selectedName;
                    const QString dirPath = DirPath(qApp->ValentinaSettings()->GetPathMultisizeMeasurements(),
                                                    selectedName);
                    mPath = FindLocation(filter, dirPath, selectedName);
                }
                else
                {
                    const QString filter = tr("Individual measurements") + QLatin1String(" (*.vit);;") +
                                           tr("Multisize measurements") + QLatin1String(" (*.vst)");
                    //Use standard path to individual measurements
                    QString selectedName;
                    const QString dirPath = DirPath(qApp->ValentinaSettings()->GetPathIndividualMeasurements(),
                                                    selectedName);
                    mPath = FindLocation(filter, dirPath, selectedName);
                }

                if (mPath.isEmpty())
                {
                    return mPath;
                }
                else
                {
                    QScopedPointer<VMeasurements> m(new VMeasurements(pattern));
                    m->setXMLContent(mPath);

                    patternType = m->Type();

                    if (patternType == MeasurementsType::Unknown)
                    {
                        VException e(tr("Measurement file has unknown format."));
                        throw e;
                    }

                    if (patternType == MeasurementsType::Multisize)
                    {
                        VVSTConverter converter(mPath);
                        m->setXMLContent(converter.Convert());// Read again after conversion
                    }
                    else
                    {
                        VVITConverter converter(mPath);
                        m->setXMLContent(converter.Convert());// Read again after conversion
                    }

                    if (not m->IsDefinedKnownNamesValid())
                    {
                        VException e(tr("Measurement file contains invalid known measurement(s)."));
                        throw e;
                    }

                    CheckRequiredMeasurements(m.data());

                    qApp->setPatternType(patternType);
                    doc->SetMPath(RelativeMPath(patternPath, mPath));
                    return mPath;
                }
            }
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ChangePP(int index, bool zoomBestFit)
{
    if (index != -1)
    {
        doc->ChangeActivPP(comboBoxDraws->itemText(index));
        doc->setCurrentData();
        emit RefreshHistory();
        if (drawMode)
        {
            ArrowTool(true);
            if (zoomBestFit)
            {
                ZoomFitBestCurrent();
            }
        }
        toolOptions->itemClicked(nullptr);//hide options for tool in previous pattern piece
        groupsWidget->UpdateGroups();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::EndVisualization(bool click)
{
    if (not dialogTool.isNull())
    {
        dialogTool->ShowDialog(click);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ZoomFirstShow Fit scene size to best size for first show
 */
void MainWindow::ZoomFirstShow()
{
    /* If don't call ZoomFitBest() twice, after first scaling or moving pattern piece, scene change coordinate and whole
     * pattern will be moved. Looks very ugly. It is best solution that i have now.
     */

    if (pattern->DataPieces()->size() > 0)
    {
        ActionDetails(true);
        ui->view->ZoomFitBest();
    }

    if (not ui->actionDraw->isChecked())
    {
        ActionDraw(true);
    }
    ZoomFitBestCurrent();

    VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
    VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

    if (pattern->DataPieces()->size() > 0)
    {
        ActionDetails(true);
        ui->view->ZoomFitBest();
    }

    if (not ui->actionDraw->isChecked())
    {
        ActionDraw(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::DoExport(const VCommandLinePtr &expParams)
{
    QVector<DetailForLayout> details;
    if(not qApp->getOpeningPattern())
    {
        const QHash<quint32, VPiece> *allDetails = pattern->DataPieces();
        if (allDetails->count() == 0)
        {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("You can't export empty scene.")));
            qApp->exit(V_EX_DATAERR);
            return false;
        }
        else
        {
            details = SortDetailsForLayout(allDetails, expParams->OptExportSuchDetails());

            if (details.count() == 0)
            {
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("You can't export empty scene. Please, "
                                                                "include at least one detail in layout.")));
                qApp->exit(V_EX_DATAERR);
                return false;
            }
        }
    }
    listDetails = PrepareDetailsForLayout(details);

    const bool exportOnlyDetails = expParams->IsExportOnlyDetails();
    if (exportOnlyDetails)
    {
        try
        {
            m_dialogSaveLayout = QSharedPointer<DialogSaveLayout>(new DialogSaveLayout(1, Draw::Modeling,
                                                                                       expParams->OptBaseName(), this));
            m_dialogSaveLayout->SetDestinationPath(expParams->OptDestinationPath());
            m_dialogSaveLayout->SelectFormat(static_cast<LayoutExportFormats>(expParams->OptExportType()));
            m_dialogSaveLayout->SetBinaryDXFFormat(expParams->IsBinaryDXF());
            m_dialogSaveLayout->SetTextAsPaths(expParams->IsTextAsPaths());

            if (static_cast<LayoutExportFormats>(expParams->OptExportType()) == LayoutExportFormats::PDFTiled)
            {
                m_dialogSaveLayout->SetTiledExportMode(true);
                m_dialogSaveLayout->SetTiledMargins(expParams->TiledPageMargins());
                m_dialogSaveLayout->SetTiledPageFormat(expParams->OptTiledPaperSize());
                m_dialogSaveLayout->SetTiledPageOrientation(expParams->OptTiledPageOrientation());
            }

            ExportData(listDetails);
            m_dialogSaveLayout.clear();
        }
        catch (const VException &e)
        {
            m_dialogSaveLayout.clear();
            qCCritical(vMainWindow, "%s\n\n%s", qUtf8Printable(tr("Export error.")), qUtf8Printable(e.ErrorMessage()));
            qApp->exit(V_EX_DATAERR);
            return false;
        }
    }
    else
    {
        auto settings = expParams->DefaultGenerator();
        settings->SetTextAsPaths(expParams->IsTextAsPaths());

        if (GenerateLayout(*settings.get()))
        {
            try
            {
                m_dialogSaveLayout = QSharedPointer<DialogSaveLayout>(new DialogSaveLayout(scenes.size(), Draw::Layout,
                                                                                           expParams->OptBaseName(),
                                                                                           this));
                m_dialogSaveLayout->SetDestinationPath(expParams->OptDestinationPath());
                m_dialogSaveLayout->SelectFormat(static_cast<LayoutExportFormats>(expParams->OptExportType()));
                m_dialogSaveLayout->SetBinaryDXFFormat(expParams->IsBinaryDXF());

                if (static_cast<LayoutExportFormats>(expParams->OptExportType()) == LayoutExportFormats::PDFTiled)
                {
                    m_dialogSaveLayout->SetTiledExportMode(true);
                    m_dialogSaveLayout->SetTiledMargins(expParams->TiledPageMargins());
                    m_dialogSaveLayout->SetTiledPageFormat(expParams->OptTiledPaperSize());
                    m_dialogSaveLayout->SetTiledPageOrientation(expParams->OptTiledPageOrientation());
                }

                ExportData(listDetails);
                m_dialogSaveLayout.clear();
            }
            catch (const VException &e)
            {
                m_dialogSaveLayout.clear();
                qCCritical(vMainWindow, "%s\n\n%s", qUtf8Printable(tr("Export error.")),
                           qUtf8Printable(e.ErrorMessage()));
                qApp->exit(V_EX_DATAERR);
                return false;
            }
        }
        else
        {
            qApp->exit(V_EX_DATAERR);
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DoFMExport process export final measurements
 * @param expParams command line options
 * @return true if succesfull
 */
bool MainWindow::DoFMExport(const VCommandLinePtr &expParams)
{
    QString filePath = expParams->OptExportFMTo();

    if (filePath.isEmpty())
    {
        qCCritical(vMainWindow, "%s\n\n%s", qUtf8Printable(tr("Export final measurements error.")),
                   qUtf8Printable(tr("Destination path is empty.")));
        qApp->exit(V_EX_DATAERR);
        return false;
    }

    QFileInfo info(filePath);
    if (info.isRelative())
    {
        filePath = QDir::currentPath() + QLatin1Char('/') + filePath;
    }

    const QString codecName = expParams->OptCSVCodecName();
    int mib = QTextCodec::codecForLocale()->mibEnum();
    if (not codecName.isEmpty())
    {
        if (QTextCodec *codec = QTextCodec::codecForName(codecName.toLatin1()))
        {
            mib = codec->mibEnum();
        }
    }

    QChar separator = expParams->OptCSVSeparator();
    if (separator.isNull())
    {
        separator = VCommonSettings::GetDefCSVSeparator();
    }

    return ExportFMeasurementsToCSVData(filePath, expParams->IsCSVWithHeader(), mib, separator);

}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::SetSize(const QString &text)
{
    if (not VApplication::IsGUIMode())
    {
        if (this->isWindowModified() || not qApp->GetPatternPath().isEmpty())
        {
            if (qApp->patternType() == MeasurementsType::Multisize)
            {
                const int size = static_cast<int>(UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = gradationSizes->findText(QString().setNum(size));
                if (index != -1)
                {
                    gradationSizes->setCurrentIndex(index);
                }
                else
                {
                    qCCritical(vMainWindow, "%s",
                              qUtf8Printable(tr("Not supported size value '%1' for this pattern file.").arg(text)));
                    return false;
                }
            }
            else
            {
                qCCritical(vMainWindow, "%s",
                          qUtf8Printable(tr("Couldn't set size. Need a file with multisize measurements.")));
                return false;
            }
        }
        else
        {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Couldn't set size. File wasn't opened.")));
            return false;
        }
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::SetHeight(const QString &text)
{
    if (not VApplication::IsGUIMode())
    {
        if (this->isWindowModified() || not qApp->GetPatternPath().isEmpty())
        {
            if (qApp->patternType() == MeasurementsType::Multisize)
            {
                const int height = static_cast<int>(UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = gradationHeights->findText(QString().setNum(height));
                if (index != -1)
                {
                    gradationHeights->setCurrentIndex(index);
                }
                else
                {
                    qCCritical(vMainWindow, "%s",
                              qUtf8Printable(tr("Not supported height value '%1' for this pattern file.").arg(text)));
                    return false;
                }
            }
            else
            {
                qCCritical(vMainWindow, "%s",
                          qUtf8Printable(tr("Couldn't set height. Need a file with multisize measurements.")));
                return false;
            }
        }
        else
        {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Couldn't set height. File wasn't opened.")));
            return false;
        }
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ProcessCMD()
{
    const VCommandLinePtr cmd = qApp->CommandLine();
    auto args = cmd->OptInputFileNames();

    isNoScaling = cmd->IsNoScalingEnabled();

    if (VApplication::IsGUIMode())
    {
        ReopenFilesAfterCrash(args);

        for (auto &arg : args)
        {
            LoadPattern(arg);
        }
    }
    else
    {
        if (args.size() != 1)
        {
            qCritical() << tr("Please, provide one input file.");
            qApp->exit(V_EX_NOINPUT);
            return;
        }

        qApp->SetUserMaterials(cmd->OptUserMaterials());

        const bool loaded = LoadPattern(args.first(), cmd->OptMeasurePath());

        if (not loaded)
        {
            return; // process only one input file
        }

        bool hSetted = true;
        bool sSetted = true;
        if (cmd->IsSetGradationSize())
        {
            sSetted = SetSize(cmd->OptGradationSize());
        }

        if (cmd->IsSetGradationHeight())
        {
            hSetted = SetHeight(cmd->OptGradationHeight());
        }

        if (not (hSetted && sSetted))
        {
            qApp->exit(V_EX_DATAERR);
            return;
        }

        if (not cmd->IsTestModeEnabled())
        {
            if (cmd->IsExportEnabled() && not DoExport(cmd))
            {
                return;
            }

            if (cmd->IsExportFMEnabled() && not DoFMExport(cmd))
            {
                return;
            }
        }

        qApp->exit(V_EX_OK);// close program after processing in console mode
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetPatternFileName()
{
    QString shownName = tr("untitled.val");
    if(not qApp->GetPatternPath().isEmpty())
    {
        shownName = StrippedName(qApp->GetPatternPath());
    }
    shownName += QLatin1String("[*]");
    return shownName;
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetMeasurementFileName()
{
    if(doc->MPath().isEmpty())
    {
        return QString();
    }
    else
    {
        QString shownName(" [");
        shownName += StrippedName(AbsoluteMPath(qApp->GetPatternPath(), doc->MPath()));

        if(mChanges)
        {
            shownName += QLatin1String("*");
        }

        shownName += QLatin1String("]");
        return shownName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateWindowTitle()
{
    bool isFileWritable = true;
    if (not qApp->GetPatternPath().isEmpty())
    {
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/
        isFileWritable = QFileInfo(qApp->GetPatternPath()).isWritable();
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/
    }

    if (not patternReadOnly && isFileWritable)
    {
        setWindowTitle(GetPatternFileName()+GetMeasurementFileName());
    }
    else
    {
        setWindowTitle(GetPatternFileName()+GetMeasurementFileName() + QLatin1String(" (") + tr("read only") +
                       QLatin1String(")"));
    }
    setWindowFilePath(qApp->GetPatternPath());

#if defined(Q_OS_MAC)
    static QIcon fileIcon = QIcon(QCoreApplication::applicationDirPath() +
                                  QLatin1String("/../Resources/Valentina.icns"));
    QIcon icon;
    if (not qApp->GetPatternPath().isEmpty())
    {
        if (not isWindowModified())
        {
            icon = fileIcon;
        }
        else
        {
            static QIcon darkIcon;

            if (darkIcon.isNull())
            {
                darkIcon = QIcon(darkenPixmap(fileIcon.pixmap(16, 16)));
            }
            icon = darkIcon;
        }
    }
    setWindowIcon(icon);
#endif //defined(Q_OS_MAC)
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::IgnoreLocking(int error, const QString &path)
{
    QMessageBox::StandardButton answer = QMessageBox::Abort;
    if (VApplication::IsGUIMode())
    {
        switch(error)
        {
            case QLockFile::LockFailedError:
                answer = QMessageBox::warning(this, tr("Locking file"),
                                               tr("This file already opened in another window. Ignore if you want "
                                                  "to continue (not recommended, can cause a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            case QLockFile::PermissionError:
                answer = QMessageBox::question(this, tr("Locking file"),
                                               tr("The lock file could not be created, for lack of permissions. "
                                                  "Ignore if you want to continue (not recommended, can cause "
                                                  "a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            case QLockFile::UnknownError:
                answer = QMessageBox::question(this, tr("Locking file"),
                                               tr("Unknown error happened, for instance a full partition prevented "
                                                  "writing out the lock file. Ignore if you want to continue (not "
                                                  "recommended, can cause a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            default:
                answer = QMessageBox::Abort;
                break;
        }
    }

    if (answer == QMessageBox::Abort)
    {
        qCDebug(vMainWindow, "Failed to lock %s", qUtf8Printable(path));
        qCDebug(vMainWindow, "Error type: %d", error);
        Clear();
        if (not VApplication::IsGUIMode())
        {
            switch(error)
            {
                case QLockFile::LockFailedError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("This file already opened in another window.")));
                    break;
                case QLockFile::PermissionError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("The lock file could not be created, for lack of permissions.")));
                    break;
                case QLockFile::UnknownError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("Unknown error happened, for instance a full partition prevented "
                                                 "writing out the lock file.")));
                    break;
                default:
                    break;
            }

            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPoint()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByRelease()
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMouseRelease);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByPress()
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMousePress);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSpline()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSplinePath()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectArc()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointArc()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectCurve()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectAllDrawObjects()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectOperationObjects()
{
    // Only true for rubber band selection
    emit EnableLabelSelection(true);
    emit EnablePointSelection(true);
    emit EnableLineSelection(false);
    emit EnableArcSelection(true);
    emit EnableElArcSelection(true);
    emit EnableSplineSelection(true);
    emit EnableSplinePathSelection(true);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectGroupObjects()
{
    ToolSelectOperationObjects();
    // Only true for rubber band selection
    emit EnableLineSelection(true);

    // Hovering
    emit EnableLineHover(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectDetail()
{
    // Only true for rubber band selection
    emit EnableNodeLabelSelection(false);
    emit EnableNodePointSelection(false);
    emit EnableDetailSelection(true);// Disable when done visualization details

    // Hovering
    emit EnableNodeLabelHover(true);
    emit EnableNodePointHover(true);
    emit EnableDetailHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}
