/************************************************************************
 **
 **  @file   mainwindowsnogui.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "mainwindowsnogui.h"
#include "core/vapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vobj/vobjpaintdevice.h"
#include "../vdxf/vdxfpaintdevice.h"
#include "dialogs/dialoglayoutsettings.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vmisc/dialogs/dialogexporttocsv.h"
#include "../vmisc/qxtcsvmodel.h"
#include "../vformat/vmeasurements.h"
#include "../vlayout/vlayoutgenerator.h"
#include "dialogs/dialoglayoutprogress.h"
#include "dialogs/dialogsavelayout.h"
#include "../vlayout/vposter.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "../vpatterndb/measurements.h"
#include "../vpatterndb/calculator.h"
#include "../vtools/tools/vabstracttool.h"
#include "../vtools/tools/vtoolseamallowance.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QProcess>
#include <QToolButton>
#include <QtSvg>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QtConcurrent>
#include <functional>
#include <QPageSize>

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vMainNoGUIWindow, "v.mainnoguiwindow")

QT_WARNING_POP

#ifndef PDFTOPS
#ifdef Q_OS_WIN
#   define PDFTOPS QStringLiteral("pdftops.exe")
#else
#   define PDFTOPS QStringLiteral("pdftops")
#endif
#endif

namespace
{
//---------------------------------------------------------------------------------------------------------------------
bool CreateLayoutPath(const QString &path)
{
    bool usedNotExistedDir = true;
    QDir dir(path);
    dir.setPath(path);
    if (not dir.exists(path))
    {
        usedNotExistedDir = dir.mkpath(QChar('.'));
    }
    return usedNotExistedDir;
}

//---------------------------------------------------------------------------------------------------------------------
void RemoveLayoutPath(const QString &path, bool usedNotExistedDir)
{
    if (usedNotExistedDir)
    {
        QDir dir(path);
        dir.rmpath(QChar('.'));
    }
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_UNUSED void InsertGlobalContours(const QList<QGraphicsScene *> &scenes, const QList<QGraphicsItem *> &gcontours);
void InsertGlobalContours(const QList<QGraphicsScene *> &scenes, const QList<QGraphicsItem *> &gcontours)
{
    if (scenes.size() != gcontours.size())
    {
        return;
    }

    for(int i = 0; i < scenes.size(); ++i)
    {
        scenes.at(i)->addItem(gcontours.at(i));
    }
}
}

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::MainWindowsNoGUI(QWidget *parent)
    : VAbstractMainWindow(parent),
      listDetails(),
      currentScene(nullptr),
      tempSceneLayout(nullptr),
      pattern(new VContainer(qApp->TrVars(), qApp->patternUnitP(), valentinaNamespace)),
      doc(nullptr),
      papers(),
      shadows(),
      scenes(),
      details(),
      gcontours(),
      detailsOnLayout(),
      undoAction(nullptr),
      redoAction(nullptr),
      actionDockWidgetToolOptions(nullptr),
      actionDockWidgetGroups(nullptr),
      isNoScaling(false),
      isLayoutStale(true),
      isNeedAutosave(false),
      ignorePrinterFields(false),
      margins(),
      paperSize(),
      m_dialogSaveLayout(),
      m_mouseCoordinate(),
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
      m_taskbarButton(new QWinTaskbarButton(this)),
      m_taskbarProgress(nullptr),
#endif
      isTiled(false),
      isAutoCropLength(false),
      isAutoCropWidth(false),
      isUnitePages(false),
      layoutPrinterName()
{
    InitTempLayoutScene();

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    m_taskbarButton->setWindow(this->windowHandle());
    m_taskbarProgress = m_taskbarButton->progress();
    m_taskbarProgress->setMinimum(0);
#endif
}

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::~MainWindowsNoGUI()
{
    delete m_unreadPatternMessage;
    delete m_mouseCoordinate;
    delete tempSceneLayout;
    delete pattern;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ToolLayoutSettings(bool checked)
{
    QToolButton *tButton = qobject_cast< QToolButton * >(this->sender());
    SCASSERT(tButton != nullptr)

    if (checked)
    {
        VLayoutGenerator lGenerator;

        DialogLayoutSettings layout(&lGenerator, this);
        if (layout.exec() == QDialog::Rejected)
        {
            tButton->setChecked(false);
            return;
        }
        layoutPrinterName = layout.SelectedPrinter();
        GenerateLayout(lGenerator);
        tButton->setChecked(false);
    }
    else
    {
        tButton->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::GenerateLayout(VLayoutGenerator& lGenerator)
{
    lGenerator.SetDetails(listDetails);

    QElapsedTimer timer;
    timer.start();

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QTimer *progressTimer = nullptr;
#endif

    QSharedPointer<DialogLayoutProgress> progress;
    if (VApplication::IsGUIMode())
    {
#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
        m_taskbarProgress->setVisible(true);
        m_taskbarProgress->setValue(0);
        m_taskbarProgress->setMaximum(lGenerator.GetNestingTime()*60);
        progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, this, [this, timer]()
        {
            m_taskbarProgress->setValue(static_cast<int>(timer.elapsed()/1000));
        });
        progressTimer->start(1000);
#endif

        progress = QSharedPointer<DialogLayoutProgress>(
                    new DialogLayoutProgress(timer, lGenerator.GetNestingTimeMSecs(), this));

        connect(progress.data(), &DialogLayoutProgress::Abort, &lGenerator, &VLayoutGenerator::Abort);
        connect(progress.data(), &DialogLayoutProgress::Timeout, &lGenerator, &VLayoutGenerator::Timeout);

        progress->Start();
    }
    else
    {
        // Because the progress bar dialog will not terminate nesting we must create separate timer for this
        auto *progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, this, [timer, &lGenerator, progressTimer]()
        {
            const int timeout = static_cast<int>(lGenerator.GetNestingTimeMSecs() - timer.elapsed());

            if (timeout <= 1000)
            {
                lGenerator.Timeout();
                progressTimer->stop();
                progressTimer->deleteLater();
            }
        });
        progressTimer->start(1000);
    }

    LayoutErrors nestingState = LayoutErrors::NoError;

    auto IsTimeout = [&progress, &lGenerator, timer, &nestingState]()
    {
        if (timer.hasExpired(lGenerator.GetNestingTimeMSecs()))
        {
            if (nestingState != LayoutErrors::EmptyPaperError)
            {
                nestingState = LayoutErrors::Timeout;
            }

            if (VApplication::IsGUIMode())
            {
                progress->Finished();
            }
            return true;
        }
        return false;
    };

    bool rotationUsed = false;
    int rotatate = 1;
    lGenerator.SetShift(-1); // Trigger first shift calulation
    lGenerator.SetRotate(false);
    int papersCount = INT_MAX;
    qreal efficiency = 0;
    bool hasResult = false;

    QCoreApplication::processEvents();

    forever
    {
        if (IsTimeout())
        {
            break;
        }

        lGenerator.Generate(timer, lGenerator.GetNestingTimeMSecs(), nestingState);

        if (IsTimeout())
        {
            break;
        }

        switch (lGenerator.State())
        {
            case LayoutErrors::NoError:
                if (lGenerator.PapersCount() <= papersCount)
                {
                    const qreal layoutEfficiency = lGenerator.LayoutEfficiency();
                    if (efficiency < layoutEfficiency)
                    {
                        efficiency = layoutEfficiency;
                        if (VApplication::IsGUIMode())
                        {
                            progress->Efficiency(efficiency);
                        }

                        CleanLayout();
                        papers = lGenerator.GetPapersItems();// Blank sheets
                        details = lGenerator.GetAllDetailsItems();// All details items
                        detailsOnLayout = lGenerator.GetAllDetails();// All details items
                        shadows = CreateShadows(papers);
                        isLayoutPortrait = lGenerator.IsPortrait();
                        scenes = CreateScenes(papers, shadows, details);
#if !defined(V_NO_ASSERT)
                       //Uncomment to debug, shows global contour
//                        gcontours = lGenerator.GetGlobalContours(); // uncomment for debugging
//                        InsertGlobalContours(scenes, gcontours); // uncomment for debugging
#endif
                        if (VApplication::IsGUIMode())
                        {
                            PrepareSceneList(PreviewQuatilty::Fast);
                        }
                        ignorePrinterFields = not lGenerator.IsUsePrinterFields();
                        margins = lGenerator.GetPrinterFields();
                        paperSize = QSizeF(lGenerator.GetPaperWidth(), lGenerator.GetPaperHeight());
                        isAutoCropLength = lGenerator.GetAutoCropLength();
                        isAutoCropWidth = lGenerator.GetAutoCropWidth();
                        isUnitePages = lGenerator.IsUnitePages();
                        isLayoutStale = false;
                        papersCount = lGenerator.PapersCount();
                        hasResult = true;
                        qDebug() << "Layout efficiency: " << efficiency;
                    }
                    else
                    {
                        if (lGenerator.IsRotationNeeded())
                        {
                            lGenerator.SetRotate(true);
                            lGenerator.SetRotationNumber(++rotatate);
                            rotationUsed = true;
                        }
                    }
                }
                else
                {
                    if (lGenerator.IsRotationNeeded())
                    {
                        lGenerator.SetRotate(true);
                        lGenerator.SetRotationNumber(++rotatate);
                        rotationUsed = true;
                    }
                }
                lGenerator.SetShift(lGenerator.GetShift()/2.0);
                break;
            case LayoutErrors::EmptyPaperError:
                if (lGenerator.IsRotationNeeded())
                {
                    if (not rotationUsed)
                    {
                        lGenerator.SetRotate(true);
                        lGenerator.SetRotationNumber(++rotatate);
                        rotationUsed = true;
                    }
                    else
                    {
                        lGenerator.SetShift(lGenerator.GetShift()/2.0);
                        rotationUsed = false;
                    }
                }
                else
                {
                    lGenerator.SetShift(lGenerator.GetShift()/2.0);
                }
                break;
            case LayoutErrors::Timeout:
            case LayoutErrors::PrepareLayoutError:
            case LayoutErrors::ProcessStoped:
            case LayoutErrors::TerminatedByException:
            default:
                break;
        }

        nestingState = lGenerator.State();

        if (nestingState == LayoutErrors::PrepareLayoutError || nestingState == LayoutErrors::ProcessStoped
                || nestingState == LayoutErrors::TerminatedByException
                || (nestingState == LayoutErrors::NoError && not qFuzzyIsNull(lGenerator.GetEfficiencyCoefficient())
                    && efficiency >= lGenerator.GetEfficiencyCoefficient()))
        {
            break;
        }

        if (IsTimeout())
        {
            break;
        }
    }

    if (VApplication::IsGUIMode())
    {
        PrepareSceneList(PreviewQuatilty::Slow);
        progress->Finished();
    }

#if defined(Q_OS_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    if (VApplication::IsGUIMode())
    {
        progressTimer->stop();
        m_taskbarProgress->setVisible(false);
    }
#endif

    if (VApplication::IsGUIMode())
    {
        QApplication::alert(this);
    }

    if (hasResult && nestingState != LayoutErrors::ProcessStoped)
    {
        return true;
    }
    else
    {
        ShowLayoutError(nestingState);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ShowLayoutError(const LayoutErrors &state)
{
    switch (state)
    {
        case LayoutErrors::NoError:
            return;
        case LayoutErrors::PrepareLayoutError:
            qCritical() << tr("Couldn't prepare data for creation layout");
            break;
        case LayoutErrors::EmptyPaperError:
            qCritical() << tr("One or more pattern pieces are bigger than the paper format you selected. Please, "
                              "select a bigger paper format.");
            break;
        case LayoutErrors::Timeout:
            qCritical() << tr("Timeout.");
            break;
        case LayoutErrors::TerminatedByException:
            qCritical() << tr("Process has been stoped because of exception.");
            break;
        case LayoutErrors::ProcessStoped:
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportFMeasurementsToCSV()
{
    QString fileName = CSVFilePath();

    if (fileName.isEmpty())
    {
        return;
    }

    DialogExportToCSV dialog(this);
    dialog.SetWithHeader(qApp->Settings()->GetCSVWithHeader());
    dialog.SetSelectedMib(qApp->Settings()->GetCSVCodec());
    dialog.SetSeparator(qApp->Settings()->GetCSVSeparator());

    if (dialog.exec() == QDialog::Accepted)
    {
        ExportFMeasurementsToCSVData(fileName, dialog.IsWithHeader(), dialog.GetSelectedMib(), dialog.GetSeparator());

        qApp->Settings()->SetCSVSeparator(dialog.GetSeparator());
        qApp->Settings()->SetCSVCodec(dialog.GetSelectedMib());
        qApp->Settings()->SetCSVWithHeader(dialog.IsWithHeader());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportData(const QVector<VLayoutPiece> &listDetails)
{
    const LayoutExportFormats format = m_dialogSaveLayout->Format();

    if (format == LayoutExportFormats::DXF_AC1006_AAMA ||
        format == LayoutExportFormats::DXF_AC1009_AAMA ||
        format == LayoutExportFormats::DXF_AC1012_AAMA ||
        format == LayoutExportFormats::DXF_AC1014_AAMA ||
        format == LayoutExportFormats::DXF_AC1015_AAMA ||
        format == LayoutExportFormats::DXF_AC1018_AAMA ||
        format == LayoutExportFormats::DXF_AC1021_AAMA ||
        format == LayoutExportFormats::DXF_AC1024_AAMA ||
        format == LayoutExportFormats::DXF_AC1027_AAMA)
    {
        if (m_dialogSaveLayout->Mode() == Draw::Layout)
        {
            for (int i = 0; i < detailsOnLayout.size(); ++i)
            {
                const QString name = m_dialogSaveLayout->Path() + '/' + m_dialogSaveLayout->FileName() +
                        QString::number(i+1) + DialogSaveLayout::ExportFormatSuffix(m_dialogSaveLayout->Format());

                QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
                SCASSERT(paper != nullptr)

                ExportApparelLayout(detailsOnLayout.at(i), name, paper->rect().size().toSize());
            }
        }
        else
        {
            ExportDetailsAsApparelLayout(listDetails);
        }
    }
    else
    {
        if (m_dialogSaveLayout->Mode() == Draw::Layout)
        {
            ExportFlatLayout(scenes, papers, shadows, details, ignorePrinterFields, margins);
        }
        else
        {
            ExportDetailsAsFlatLayout(listDetails);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportFlatLayout(const QList<QGraphicsScene *> &scenes,
                                        const QList<QGraphicsItem *> &papers, const QList<QGraphicsItem *> &shadows,
                                        const QList<QList<QGraphicsItem *> > &details, bool ignorePrinterFields,
                                        const QMarginsF &margins)
{
    const QString path = m_dialogSaveLayout->Path();
    bool usedNotExistedDir = CreateLayoutPath(path);
    if (not usedNotExistedDir)
    {
        qCritical() << tr("Can't create a path");
        return;
    }

    qApp->ValentinaSettings()->SetPathLayout(path);
    const LayoutExportFormats format = m_dialogSaveLayout->Format();

    if (format == LayoutExportFormats::PDFTiled && m_dialogSaveLayout->Mode() == Draw::Layout)
    {
        const QString name = path + '/' + m_dialogSaveLayout->FileName() + QString::number(1)
                + DialogSaveLayout::ExportFormatSuffix(m_dialogSaveLayout->Format());
        PdfTiledFile(name);
    }
    else
    {
        ExportScene(scenes, papers, shadows, details, ignorePrinterFields, margins);
    }

    RemoveLayoutPath(path, usedNotExistedDir);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportDetailsAsFlatLayout(const QVector<VLayoutPiece> &listDetails)
{
    if (listDetails.isEmpty())
    {
        return;
    }

    QScopedPointer<QGraphicsScene> scene(new QGraphicsScene());

    QList<QGraphicsItem *> list;
    for (auto piece : listDetails)
    {
        QGraphicsItem *item = piece.GetItem(m_dialogSaveLayout->IsTextAsPaths());
        qreal diff = 0;
        if (piece.IsForceFlipping())
        {
            const qreal x = item->boundingRect().x();
            piece.Mirror();
            item = piece.GetItem(m_dialogSaveLayout->IsTextAsPaths());
            diff = item->boundingRect().x() - x;
        }

        if (piece.IsForceFlipping())
        {
            item->setPos(piece.GetMx()-diff, piece.GetMy());
        }
        else
        {
            item->setPos(piece.GetMx(), piece.GetMy());
        }
        list.append(item);
    }

    for (auto item : list)
    {
        scene->addItem(item);
    }

    QList<QGraphicsItem *> papers;// Blank sheets
    QRect rect = scene->itemsBoundingRect().toRect();

    const int mx = rect.x();
    const int my = rect.y();

    QTransform matrix;
    matrix = matrix.translate(-mx, -my);

    for (auto item : list)
    {
        item->setTransform(matrix);
    }

    rect = scene->itemsBoundingRect().toRect();

    QGraphicsRectItem *paper = new QGraphicsRectItem(rect);
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    papers.append(paper);

    QList<QList<QGraphicsItem *> > details;// All details
    details.append(list);

    QList<QGraphicsItem *> shadows = CreateShadows(papers);
    QList<QGraphicsScene *> scenes = CreateScenes(papers, shadows, details);

    const bool ignorePrinterFields = false;
    const qreal margin = ToPixel(1, Unit::Cm);
    ExportFlatLayout(scenes, papers, shadows, details, ignorePrinterFields, QMarginsF(margin, margin, margin, margin));

    qDeleteAll(scenes);//Scene will clear all other items
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportApparelLayout(const QVector<VLayoutPiece> &details, const QString &name,
                                           const QSize &size) const
{
    const QString path = m_dialogSaveLayout->Path();
    bool usedNotExistedDir = CreateLayoutPath(path);
    if (not usedNotExistedDir)
    {
        qCritical() << tr("Can't create a path");
        return;
    }

    qApp->ValentinaSettings()->SetPathLayout(path);
    const LayoutExportFormats format = m_dialogSaveLayout->Format();

    switch (format)
    {
        case LayoutExportFormats::DXF_AC1006_ASTM:
        case LayoutExportFormats::DXF_AC1009_ASTM:
        case LayoutExportFormats::DXF_AC1012_ASTM:
        case LayoutExportFormats::DXF_AC1014_ASTM:
        case LayoutExportFormats::DXF_AC1015_ASTM:
        case LayoutExportFormats::DXF_AC1018_ASTM:
        case LayoutExportFormats::DXF_AC1021_ASTM:
        case LayoutExportFormats::DXF_AC1024_ASTM:
        case LayoutExportFormats::DXF_AC1027_ASTM:
            Q_UNREACHABLE(); // For now not supported
            break;
        case LayoutExportFormats::DXF_AC1006_AAMA:
            AAMADxfFile(name, DRW::AC1006, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1009_AAMA:
            AAMADxfFile(name, DRW::AC1009, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1012_AAMA:
            AAMADxfFile(name, DRW::AC1012, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1014_AAMA:
            AAMADxfFile(name, DRW::AC1014, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1015_AAMA:
            AAMADxfFile(name, DRW::AC1015, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1018_AAMA:
            AAMADxfFile(name, DRW::AC1018, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1021_AAMA:
            AAMADxfFile(name, DRW::AC1021, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1024_AAMA:
            AAMADxfFile(name, DRW::AC1024, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1027_AAMA:
            AAMADxfFile(name, DRW::AC1027, m_dialogSaveLayout->IsBinaryDXFFormat(), size, details);
            break;
        default:
            qDebug() << "Can't recognize file type." << Q_FUNC_INFO;
            break;
    }

    RemoveLayoutPath(path, usedNotExistedDir);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportDetailsAsApparelLayout(QVector<VLayoutPiece> listDetails)
{
    if (listDetails.isEmpty())
    {
        return;
    }

    QScopedPointer<QGraphicsScene> scene(new QGraphicsScene());

    QList<QGraphicsItem *> list;
    for (int i=0; i < listDetails.count(); ++i)
    {
        VLayoutPiece piece = listDetails.at(i);
        QGraphicsItem *item = piece.GetItem(m_dialogSaveLayout->IsTextAsPaths());
        qreal diff = 0;
        if (piece.IsForceFlipping())
        {
            const qreal x = item->boundingRect().x();
            piece.Mirror();
            item = piece.GetItem(m_dialogSaveLayout->IsTextAsPaths());
            diff = item->boundingRect().x() - x;
        }

        QTransform moveMatrix = piece.GetMatrix();
        if (piece.IsForceFlipping())
        {
            item->setPos(piece.GetMx()-diff, piece.GetMy());
            moveMatrix = moveMatrix.translate(-piece.GetMx()+diff, piece.GetMy());
        }
        else
        {
            item->setPos(piece.GetMx(), piece.GetMy());
            moveMatrix = moveMatrix.translate(piece.GetMx(), piece.GetMy());
        }
        listDetails[i].SetMatrix(moveMatrix);
        list.append(item);
    }

    for (auto item : list)
    {
        scene->addItem(item);
    }

    QRect rect = scene->itemsBoundingRect().toRect();

    const int mx = rect.x();
    const int my = rect.y();

    QTransform matrix;
    matrix = matrix.translate(-mx, -my);

    for (auto item : list)
    {
        item->setTransform(matrix);
    }

    rect = scene->itemsBoundingRect().toRect();

    for (int i=0; i < listDetails.count(); ++i)
    {
        QTransform moveMatrix = listDetails.at(i).GetMatrix();
        if (listDetails.at(i).IsForceFlipping())
        {
            moveMatrix = moveMatrix.translate(mx, -my);
        }
        else
        {
            moveMatrix = moveMatrix.translate(-mx, -my);
        }
        listDetails[i].SetMatrix(moveMatrix);
    }

    const QString name = m_dialogSaveLayout->Path() + '/' + m_dialogSaveLayout->FileName() +
            QString::number(1) + DialogSaveLayout::ExportFormatSuffix(m_dialogSaveLayout->Format());

    ExportApparelLayout(listDetails, name, rect.size());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintPages(QPrinter *printer)
{
    // Here we try understand difference between printer's dpi and our.
    // Get printer rect acording to our dpi.
    const QRectF printerPageRect(0, 0, ToPixel(printer->pageRect(QPrinter::Millimeter).width(), Unit::Mm),
                                 ToPixel(printer->pageRect(QPrinter::Millimeter).height(), Unit::Mm));
    const double xscale = printer->pageRect().width() / printerPageRect.width();
    const double yscale = printer->pageRect().height() / printerPageRect.height();
    const double scale = qMin(xscale, yscale);

    QPainter painter;
    if (not painter.begin(printer))
    { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    painter.setFont( QFont( QStringLiteral("Arial"), 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->Settings()->WidthMainLine(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );

    int count = 0;
    QSharedPointer<QVector<PosterData>> poster;
    QSharedPointer<VPoster> posterazor;

    if (isTiled)
    {
        PageOrientation orientation;
        if (not m_dialogSaveLayout.isNull())
        {
            orientation = m_dialogSaveLayout->GetTiledPageOrientation();
        }
        else
        {
            orientation = qApp->ValentinaSettings()->GetTiledPDFOrientation();
        }

        // when isTiled, the landscape tiles have to be rotated, because the pages
        // stay portrait in the pdf
        if(orientation == PageOrientation::Landscape)
        {
            painter.rotate(-90);
            painter.translate(-ToPixel(printer->pageRect(QPrinter::Millimeter).height(), Unit::Mm), 0);
        }

        poster = QSharedPointer<QVector<PosterData>>(new QVector<PosterData>());
        posterazor = QSharedPointer<VPoster>(new VPoster(printer));

        for (int i=0; i < scenes.size(); ++i)
        {
            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
            if (paper)
            {
                *poster += posterazor->Calc(paper->rect().toRect(), i, orientation);
            }
        }

        count = poster->size();
    }
    else
    {
        count = scenes.size();
    }

    // Handle the fromPage(), toPage(), supportsMultipleCopies(), and numCopies() values from QPrinter.
    int firstPage = printer->fromPage() - 1;
    if (firstPage >= count)
    {
        return;
    }
    if (firstPage == -1)
    {
        firstPage = 0;
    }

    int lastPage = printer->toPage() - 1;
    if (lastPage == -1 || lastPage >= count)
    {
        lastPage = count - 1;
    }

    const int numPages = lastPage - firstPage + 1;
    int copyCount = 1;
    if (not printer->supportsMultipleCopies())
    {
        copyCount = printer->copyCount();
    }

    for (int i = 0; i < copyCount; ++i)
    {
        for (int j = 0; j < numPages; ++j)
        {
            if (i != 0 || j != 0)
            {
                if (not printer->newPage())
                {
                    qWarning("failed in flushing page to disk, disk full?");
                    return;
                }
            }
            int index;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
            {
                index = firstPage + j;
            }
            else
            {
                index = lastPage - j;
            }

            int paperIndex = -1;
            isTiled ? paperIndex = static_cast<int>(poster->at(index).index) : paperIndex = index;

            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(paperIndex));
            if (paper)
            {
                QVector<QGraphicsItem *> posterData;
                if (isTiled)
                {
                    // Draw borders
                    posterData = posterazor->Borders(paper, poster->at(index), scenes.size());
                }

                PreparePaper(paperIndex);

                // Render
                QRectF source;
                isTiled ? source = poster->at(index).rect : source = paper->rect();

                qreal x,y;
                if(printer->fullPage())
                {
                    QMarginsF printerMargins = printer->pageLayout().margins();
                    x = qFloor(ToPixel(printerMargins.left(),Unit::Mm));
                    y = qFloor(ToPixel(printerMargins.top(),Unit::Mm));
                }
                else
                {
                    x = 0; y = 0;
                }

                QRectF target(x * scale, y * scale, source.width() * scale, source.height() * scale);

                scenes.at(paperIndex)->render(&painter, target, source, Qt::IgnoreAspectRatio);

                if (isTiled)
                {
                    // Remove borders
                    qDeleteAll(posterData);
                }

                // Restore
                RestorePaper(paperIndex);
            }
        }
    }

    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintPreviewOrigin()
{
    if (not isPagesUniform())
    {
        qCritical()<<tr("For previewing multipage document all sheet should have the same size.");
        return;
    }

    isTiled = false;
    PrintPreview();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintPreviewTiled()
{
    isTiled = true;
    PrintPreview();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintOrigin()
{
    if (not isPagesUniform())
    {
        qCritical()<<tr("For printing multipages document all sheet should have the same size.");
        return;
    }

    isTiled = false;
    LayoutPrint();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintTiled()
{
    isTiled = true;
    LayoutPrint();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiece> MainWindowsNoGUI::PrepareDetailsForLayout(const QVector<DetailForLayout> &details)
{
    if (details.isEmpty())
    {
        return QVector<VLayoutPiece>();
    }

    std::function<VLayoutPiece (const DetailForLayout &data)> PrepareDetail = [](const DetailForLayout &data)
    {
        VAbstractTool *tool = qobject_cast<VAbstractTool*>(VAbstractPattern::getTool(data.id));
        SCASSERT(tool != nullptr)
        return VLayoutPiece::Create(data.piece, data.id, tool->getData());
    };

    QProgressDialog progress(tr("Preparing details for layout"), QString(), 0, details.size());
    progress.setWindowModality(Qt::ApplicationModal);

    QFutureWatcher<VLayoutPiece> futureWatcher;
    QObject::connect(&futureWatcher, &QFutureWatcher<VLayoutPiece>::finished, &progress, &QProgressDialog::reset);
    QObject::connect(&futureWatcher,  &QFutureWatcher<VLayoutPiece>::progressRangeChanged, &progress,
                     &QProgressDialog::setRange);
    QObject::connect(&futureWatcher, &QFutureWatcher<VLayoutPiece>::progressValueChanged, &progress,
                     &QProgressDialog::setValue);

    futureWatcher.setFuture(QtConcurrent::mapped(details, PrepareDetail));

    if (qApp->IsGUIMode())
    {
        progress.exec();
    }

    futureWatcher.waitForFinished();

    QVector<VLayoutPiece> layoutDetails;
    layoutDetails.reserve(details.size());
    const QFuture<VLayoutPiece> future = futureWatcher.future();

    QFuture<VLayoutPiece>::const_iterator i;
    for (i = future.constBegin(); i != future.constEnd(); ++i)
    {
        layoutDetails.append(*i);
    }

    return layoutDetails;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::InitTempLayoutScene()
{
    tempSceneLayout = new VMainGraphicsScene();
    tempSceneLayout->setBackgroundBrush( QBrush(QColor(Qt::gray), Qt::SolidPattern) );
}

//---------------------------------------------------------------------------------------------------------------------
QStringList MainWindowsNoGUI::RecentFileList() const
{
    return qApp->ValentinaSettings()->GetRecentFileList();
}

//---------------------------------------------------------------------------------------------------------------------
QIcon MainWindowsNoGUI::ScenePreview(int i, QSize iconSize, PreviewQuatilty quality) const
{
    QImage image;
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        if (quality == PreviewQuatilty::Fast)
        {
            image = QImage(iconSize, QImage::Format_RGB32);
            image.fill(Qt::white);
        }
        else
        {
            const QRectF r = paper->rect();
            // Create the image with the exact size of the shrunk scene
            image = QImage(QSize(static_cast<qint32>(r.width()), static_cast<qint32>(r.height())),
                           QImage::Format_RGB32);

            if (not image.isNull())
            {
                image.fill(Qt::white);
                QPainter painter(&image);
                painter.setFont( QFont( QStringLiteral("Arial"), 8, QFont::Normal ) );
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setPen(QPen(Qt::black, qApp->Settings()->WidthMainLine(), Qt::SolidLine, Qt::RoundCap,
                                    Qt::RoundJoin));
                painter.setBrush ( QBrush ( Qt::NoBrush ) );
                scenes.at(i)->render(&painter, r, r, Qt::IgnoreAspectRatio);
                painter.end();
            }
            else
            {
                qDebug()<<"Cannot create image. Size " << r.size() << "too big";

                image = QImage(iconSize, QImage::Format_RGB32);
                image.fill(Qt::white);
            }
        }
    }
    else
    {
        image = QImage(iconSize, QImage::Format_RGB32);
        image.fill(Qt::white);
    }
    return QIcon(QBitmap::fromImage(image));
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> MainWindowsNoGUI::CreateShadows(const QList<QGraphicsItem *> &papers)
{
    QList<QGraphicsItem *> shadows;

    for (auto paper : papers)
    {
        qreal x1=0, y1=0, x2=0, y2=0;
        if (QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem *>(paper))
        {
            item->rect().getCoords(&x1, &y1, &x2, &y2);
            QGraphicsRectItem *shadowPaper = new QGraphicsRectItem(QRectF(x1+4, y1+4, x2+4, y2+4));
            shadowPaper->setBrush(QBrush(Qt::black));
            shadows.append(shadowPaper);
        }
        else
        {
            shadows.append(nullptr);
        }
    }

    return shadows;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsScene *> MainWindowsNoGUI::CreateScenes(const QList<QGraphicsItem *> &papers,
                                                       const QList<QGraphicsItem *> &shadows,
                                                       const QList<QList<QGraphicsItem *> > &details)
{
    QList<QGraphicsScene *> scenes;
    scenes.reserve(papers.size());
    for (int i=0; i<papers.size(); ++i)
    {
        auto *scene = new VMainGraphicsScene();
        scene->SetNonInteractive(true);
        scene->setBackgroundBrush(QBrush(QColor(Qt::gray), Qt::SolidPattern));
        scene->addItem(shadows.at(i));
        scene->addItem(papers.at(i));

        const QList<QGraphicsItem *> paperDetails = details.at(i);
        for (auto &detail : paperDetails)
        {
            scene->addItem(detail);
        }

        scenes.append(scene);
    }

    return scenes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SvgFile save layout to svg file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::SvgFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene,
                               const QMarginsF &margins) const
{
    const QRectF r = paper->rect();
    QSvgGenerator generator;
    generator.setFileName(name);
    generator.setSize(QSize(qFloor(r.width() + margins.left() + margins.right()),
                            qFloor(r.height() + margins.top() + margins.bottom())));
    generator.setViewBox(QRectF(0, 0, r.width() + margins.left() + margins.right(),
                                r.height() + margins.top() + margins.bottom()));
    generator.setTitle(tr("Pattern"));
    generator.setDescription(doc->GetDescription().toHtmlEscaped());
    generator.setResolution(static_cast<int>(PrintDPI));
    QPainter painter;
    painter.begin(&generator);
    painter.translate(margins.left(), margins.top());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->Settings()->WidthHairLine(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    scene->render(&painter, r, r, Qt::IgnoreAspectRatio);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PngFile save layout to png file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PngFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene,
                               const QMarginsF &margins) const
{
    const QRectF r = paper->rect();
    // Create the image with the exact size of the shrunk scene
    QImage image(QSize(qFloor(r.width() + margins.left() + margins.right()),
                       qFloor(r.height() + margins.top() + margins.bottom())),
                 QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.translate(margins.left(), margins.top());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->Settings()->WidthMainLine(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    scene->render(&painter, r, r, Qt::IgnoreAspectRatio);
    image.save(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfFile save layout to pdf file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PdfFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene,
                               bool ignorePrinterFields, const QMarginsF &margins) const
{
    QPrinter printer;
    printer.setCreator(QGuiApplication::applicationDisplayName()+QChar(QChar::Space)+
                       QCoreApplication::applicationVersion());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(name);
    printer.setDocName(FileName());
    const QRectF r = paper->rect();
    printer.setResolution(static_cast<int>(PrintDPI));
    printer.setOrientation(QPrinter::Portrait);
    printer.setFullPage(ignorePrinterFields);
    if (not printer.setPageSize(QPageSize(QSizeF(FromPixel(r.width() + margins.left() + margins.right(), Unit::Mm),
                                                 FromPixel(r.height() + margins.top() + margins.bottom(), Unit::Mm)),
                                          QPageSize::Millimeter)))
    {
        qWarning() << tr("Cannot set printer page size");
    }

    const qreal left = FromPixel(margins.left(), Unit::Mm);
    const qreal top = FromPixel(margins.top(), Unit::Mm);
    const qreal right = FromPixel(margins.right(), Unit::Mm);
    const qreal bottom = FromPixel(margins.bottom(), Unit::Mm);

    if (not printer.setPageMargins(QMarginsF(left, top, right, bottom), QPageLayout::Millimeter))
    {
        qWarning() << tr("Cannot set printer margins");
    }

    QPainter painter;
    if (painter.begin( &printer ) == false)
    { // failed to open file
        qCritical("%s", qUtf8Printable(tr("Can't open printer %1").arg(name)));
        return;
    }
    painter.setFont( QFont( QStringLiteral("Arial"), 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->Settings()->WidthMainLine(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    scene->render(&painter, r, r, Qt::IgnoreAspectRatio);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PdfTiledFile(const QString &name)
{
    isTiled = true;

    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            return;
        }
    }
    QPrinter printer;
    SetPrinterSettings(&printer, PrintType::PrintPDF);

    // Call IsPagesFit after setting a printer settings and check if pages is not bigger than printer's paper size
    if (not isTiled && not IsPagesFit(printer.paperRect().size()))
    {
        qWarning()<<tr("Pages will be cropped because they do not fit printer paper size.");
    }

    printer.setOutputFileName(name);
    printer.setResolution(static_cast<int>(PrintDPI));
    PrintPages(&printer);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EpsFile save layout to eps file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::EpsFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene,
                               bool ignorePrinterFields, const QMarginsF &margins) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName(), paper, scene, ignorePrinterFields, margins);
        QStringList params = QStringList() << QStringLiteral("-eps") << tmp.fileName() << name;
        PdfToPs(params);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PsFile save layout to ps file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PsFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene, bool
                              ignorePrinterFields, const QMarginsF &margins) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName(), paper, scene, ignorePrinterFields, margins);
        QStringList params = QStringList() << tmp.fileName() << name;
        PdfToPs(params);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfToPs use external tool "pdftops" for converting pdf too eps or ps format.
 * @param params string with parameter for tool. Parameters have format: "-eps input_file out_file". Use -eps when
 * need create eps file.
 */
void MainWindowsNoGUI::PdfToPs(const QStringList &params) const
{
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QProcess proc;
#if defined(Q_OS_MAC)
    // Fix issue #594. Broken export on Mac.
    proc.setWorkingDirectory(qApp->applicationDirPath());
    proc.start(QLatin1String("./") + PDFTOPS, params);
#else
    proc.start(PDFTOPS, params);
#endif
    if (proc.waitForStarted(15000))
    {
        proc.waitForFinished(15000);
    }
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    QFile f(params.last());
    if (f.exists() == false)
    {
        const QString msg = tr("Creating file '%1' failed! %2").arg(params.last(), proc.errorString());
        QMessageBox msgBox(QMessageBox::Critical, tr("Critical error!"), msg, QMessageBox::Ok | QMessageBox::Default);
        msgBox.exec();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ObjFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene) const
{
    VObjPaintDevice generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    generator.setResolution(static_cast<int>(PrintDPI));
    QPainter painter;
    painter.begin(&generator);
    scene->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

void MainWindowsNoGUI::FlatDxfFile(const QString &name, int version, bool binary, QGraphicsRectItem *paper,
                               QGraphicsScene *scene, const QList<QList<QGraphicsItem *> > &details) const
{
    PrepareTextForDXF(endStringPlaceholder, details);
    VDxfPaintDevice generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    generator.setResolution(PrintDPI);
    generator.SetVersion(static_cast<DRW::Version>(version));
    generator.SetBinaryFormat(binary);
    generator.setInsunits(VarInsunits::Millimeters);// Decided to always use mm. See issue #745

    QPainter painter;
    if (painter.begin(&generator))
    {
        scene->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
        painter.end();
    }
    RestoreTextAfterDXF(endStringPlaceholder, details);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::AAMADxfFile(const QString &name, int version, bool binary, const QSize &size,
                                   const QVector<VLayoutPiece> &details) const
{
    VDxfPaintDevice generator;
    generator.setFileName(name);
    generator.setSize(size);
    generator.setResolution(PrintDPI);
    generator.SetVersion(static_cast<DRW::Version>(version));
    generator.SetBinaryFormat(binary);
    generator.setInsunits(VarInsunits::Millimeters);// Decided to always use mm. See issue #745
    generator.ExportToAAMA(details);
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PreparePaper(int index) const
{
    auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(index));
    if (paper)
    {
        QBrush brush(Qt::white);
        scenes.at(index)->setBackgroundBrush(brush);
        shadows.at(index)->setVisible(false);
        paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));// border
    }

}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::RestorePaper(int index) const
{
    auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(index));
    if (paper)
    {
        // Restore
        paper->setPen(QPen(Qt::black, 1));
        QBrush brush(Qt::gray);
        scenes.at(index)->setBackgroundBrush(brush);
        shadows.at(index)->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PrepareTextForDXF prepare QGraphicsSimpleTextItem items for export to flat dxf.
 *
 * Because QPaintEngine::drawTextItem doesn't pass whole string per time we mark end of each string by adding special
 * placholder. This method append it.
 *
 * @param placeholder placeholder that will be appended to each QGraphicsSimpleTextItem item's text string.
 */
void MainWindowsNoGUI::PrepareTextForDXF(const QString &placeholder,
                                         const QList<QList<QGraphicsItem *> > &details) const
{
    for (auto &paperItems : details)
    {
        for (auto item : paperItems)
        {
            QList<QGraphicsItem *> pieceChildren = item->childItems();
            for (auto child : pieceChildren)
            {
                if (child->type() == QGraphicsSimpleTextItem::Type)
                {
                    if(QGraphicsSimpleTextItem *textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(child))
                    {
                        textItem->setText(textItem->text() + placeholder);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindowsNoGUI::RestoreTextAfterDXF restore QGraphicsSimpleTextItem items after export to flat dxf.
 *
 * Because QPaintEngine::drawTextItem doesn't pass whole string per time we mark end of each string by adding special
 * placholder. This method remove it.
 *
 * @param placeholder placeholder that will be removed from each QGraphicsSimpleTextItem item's text string.
 */
void MainWindowsNoGUI::RestoreTextAfterDXF(const QString &placeholder,
                                           const QList<QList<QGraphicsItem *> > &details) const
{
    for (auto &paperItems : details)
    {
        for (auto item : paperItems)
        {
            QList<QGraphicsItem *> pieceChildren = item->childItems();
            for (auto child : pieceChildren)
            {
                if (child->type() == QGraphicsSimpleTextItem::Type)
                {
                    if(QGraphicsSimpleTextItem *textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(child))
                    {
                        QString text = textItem->text();
                        text.replace(placeholder, QString());
                        textItem->setText(text);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintPreview()
{
    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            return;
        }
    }

    QPrinterInfo info = QPrinterInfo::printerInfo(layoutPrinterName);
    if(info.isNull() || info.printerName().isEmpty())
    {
        info = QPrinterInfo::defaultPrinter();
    }
    QSharedPointer<QPrinter> printer = PreparePrinter(info);
    if (printer.isNull())
    {
        qCritical("%s\n\n%s", qUtf8Printable(tr("Print error")),
                  qUtf8Printable(tr("Cannot proceed because there are no available printers in your system.")));
        return;
    }

    SetPrinterSettings(printer.data(), PrintType::PrintPreview);
    printer->setResolution(static_cast<int>(PrintDPI));
    // display print preview dialog
    QPrintPreviewDialog preview(printer.data());
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindowsNoGUI::PrintPages);
    preview.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::LayoutPrint()
{
    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            return;
        }
    }
    // display print dialog and if accepted print
    QPrinterInfo info = QPrinterInfo::printerInfo(layoutPrinterName);
    if(info.isNull() || info.printerName().isEmpty())
    {
        info = QPrinterInfo::defaultPrinter();
    }
    QSharedPointer<QPrinter> printer = PreparePrinter(info, QPrinter::HighResolution);
    if (printer.isNull())
    {
        qCritical("%s\n\n%s", qUtf8Printable(tr("Print error")),
                  qUtf8Printable(tr("Cannot proceed because there are no available printers in your system.")));
        return;
    }

    SetPrinterSettings(printer.data(), PrintType::PrintNative);
    QPrintDialog dialog(printer.data(), this );
    // If only user couldn't change page margins we could use method setMinMax();
    dialog.setOption(QPrintDialog::PrintCurrentPage, false);
    if ( dialog.exec() == QDialog::Accepted )
    {
        printer->setResolution(static_cast<int>(PrintDPI));
        PrintPages(printer.data());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::SetPrinterSettings(QPrinter *printer, const PrintType &printType)
{
    SCASSERT(printer != nullptr)
    printer->setCreator(QGuiApplication::applicationDisplayName()+QChar(QChar::Space)+
                        QCoreApplication::applicationVersion());

    printer->setOrientation(isLayoutPortrait ? QPrinter::Portrait : QPrinter::Landscape);

    if (not isTiled)
    {
        qreal width = FromPixel(paperSize.width(), Unit::Mm);
        qreal height = FromPixel(paperSize.height(), Unit::Mm);

        if (isAutoCropLength || isUnitePages)
        {
            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
            if (paper)
            {
                if (isLayoutPortrait)
                {
                    height = FromPixel(paper->rect().height() + margins.top() + margins.bottom(), Unit::Mm);
                }
                else
                {
                    width = FromPixel(paper->rect().width() + margins.left() + margins.right(), Unit::Mm);
                }
            }
        }

        if (isAutoCropWidth)
        {
            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
            if (paper)
            {
                if (isLayoutPortrait)
                {
                    width = FromPixel(paper->rect().width() + margins.left() + margins.right(), Unit::Mm);
                }
                else
                {
                    height = FromPixel(paper->rect().height() + margins.top() + margins.bottom(), Unit::Mm);
                }
            }
        }

        QSizeF size = QSizeF(width, height);

        if (not isLayoutPortrait)
        {
            size.transpose(); // QPrinter reverse this for landscape orientation
        }

        const QPageSize::PageSizeId pSZ = FindPageSizeId(size);
        if (pSZ == QPageSize::Custom)
        {
            if (not printer->setPageSize(QPageSize(size, QPageSize::Millimeter)))
            {
                qWarning() << tr("Cannot set custom printer page size");
            }
        }
        else
        {
            if (not printer->setPageSize(QPageSize(pSZ)))
            {
                qWarning() << tr("Cannot set printer page size");
            }
        }
    }
    else
    {
        if (not m_dialogSaveLayout.isNull())
        {
            VAbstractLayoutDialog::PaperSizeTemplate tiledFormat = m_dialogSaveLayout->GetTiledPageFormat();
            if (not printer->setPageSize(QPageSize(m_dialogSaveLayout->GetTemplateSize(tiledFormat, Unit::Mm),
                                                   QPageSize::Millimeter)))
            {
                qWarning() << tr("Cannot set printer page size");
            }
        }
    }

    printer->setFullPage(ignorePrinterFields);

    qreal left, top, right, bottom;

    if (not isTiled)
    {
        QMarginsF pageMargin = QMarginsF(UnitConvertor(margins, Unit::Px, Unit::Mm));
        left = pageMargin.left();
        top = pageMargin.top();
        right = pageMargin.right();
        bottom = pageMargin.bottom();
    }
    else
    {
        QMarginsF pageMargin;
        PageOrientation orientation;
        if (not m_dialogSaveLayout.isNull())
        {
            pageMargin = m_dialogSaveLayout->GetTiledMargins();
            orientation = m_dialogSaveLayout->GetTiledPageOrientation();
        }
        else
        {
            VSettings *settings = qApp->ValentinaSettings();
            pageMargin = QMarginsF(settings->GetTiledPDFMargins(Unit::Mm));
            orientation = settings->GetTiledPDFOrientation();
        }

        if(orientation == PageOrientation::Landscape)
        {
            // because when painting we have a -90rotation in landscape modus,
            // see function PrintPages.
            left = pageMargin.bottom();
            top = pageMargin.left();
            right = pageMargin.top();
            bottom = pageMargin.right();
        }
        else
        {
            left = pageMargin.left();
            top = pageMargin.top();
            right = pageMargin.right();
            bottom = pageMargin.bottom();
        }
    }

    const bool success = printer->setPageMargins(QMarginsF(left, top, right, bottom), QPageLayout::Millimeter);
    if (not success)
    {
        qWarning() << tr("Cannot set printer margins");
    }

    switch(printType)
    {
        case PrintType::PrintPDF:
        {
            const QString outputFileName = QDir::homePath() + QDir::separator() + FileName();
            #ifdef Q_OS_WIN
            printer->setOutputFileName(outputFileName);
            #else
            printer->setOutputFileName(outputFileName + QStringLiteral(".pdf"));
            #endif

            #ifdef Q_OS_MAC
            printer->setOutputFormat(QPrinter::NativeFormat);
            #else
            printer->setOutputFormat(QPrinter::PdfFormat);
            #endif
            break;
        }
        case PrintType::PrintNative:
            printer->setOutputFileName(QString());//Disable printing to file if was enabled.
            printer->setOutputFormat(QPrinter::NativeFormat);
            break;
        case PrintType::PrintPreview: /*do nothing*/
        default:
            break;
    }

    printer->setDocName(FileName());

    IsLayoutGrayscale() ? printer->setColorMode(QPrinter::GrayScale) : printer->setColorMode(QPrinter::Color);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::IsLayoutGrayscale() const
{
    const QRect target = QRect(0, 0, 100, 100);//Small image less memory need

    for (int i=0; i < scenes.size(); ++i)
    {
        if (auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i)))
        {
            // Hide shadow and paper border
            PreparePaper(i);

            // Render png
            QImage image(target.size(), QImage::Format_RGB32);
            image.fill(Qt::white);
            QPainter painter(&image);
            painter.setPen(QPen(Qt::black, qApp->Settings()->WidthMainLine(), Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.setBrush ( QBrush ( Qt::NoBrush ) );
            scenes.at(i)->render(&painter, target, paper->rect(), Qt::KeepAspectRatio);
            painter.end();

            // Restore
            RestorePaper(i);

            if (not image.isGrayscale())
            {
                return false;
            }
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QPageSize::PageSizeId MainWindowsNoGUI::FindPageSizeId(const QSizeF &size) const
{
    if (size == QSizeF(841, 1189) || size == QSizeF(1189, 841))
    {
        return QPageSize::A0;
    }

    if (size == QSizeF(594, 841) || size == QSizeF(841, 594))
    {
        return QPageSize::A1;
    }

    if (size == QSizeF(420, 594) || size == QSizeF(594, 420))
    {
        return QPageSize::A2;
    }

    if (size == QSizeF(297, 420) || size == QSizeF(420, 297))
    {
        return QPageSize::A3;
    }

    if (size == QSizeF(210, 297) || size == QSizeF(297, 210))
    {
        return QPageSize::A4;
    }

    if (size == QSizeF(215.9, 355.6) || size == QSizeF(355.6, 215.9))
    {
        return QPageSize::Legal;
    }

    if (size == QSizeF(215.9, 279.4) || size == QSizeF(279.4, 215.9))
    {
        return QPageSize::Letter;
    }

    return QPageSize::Custom;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::isPagesUniform() const
{
    if (papers.size() < 2)
    {
        return true;
    }
    else
    {
        auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
        SCASSERT(paper != nullptr)
        for (auto paperItem : papers)
        {
            auto *p = qgraphicsitem_cast<QGraphicsRectItem *>(paperItem);
            SCASSERT(p != nullptr)
            if (paper->rect() != p->rect())
            {
                return false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::IsPagesFit(const QSizeF &printPaper) const
{
    // On previous stage already was checked if pages have uniform size
    // Enough will be to check only one page
    QGraphicsRectItem *p = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
    SCASSERT(p != nullptr)
    const QSizeF pSize = p->rect().size();
    if (pSize.height() <= printPaper.height() && pSize.width() <= printPaper.width())
    {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportScene(const QList<QGraphicsScene *> &scenes,
                                   const QList<QGraphicsItem *> &papers, const QList<QGraphicsItem *> &shadows,
                                   const QList<QList<QGraphicsItem *> > &details, bool ignorePrinterFields,
                                   const QMarginsF &margins) const
{
    for (int i=0; i < scenes.size(); ++i)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            const QString name = m_dialogSaveLayout->Path() + '/' + m_dialogSaveLayout->FileName() +
                    QString::number(i+1) + DialogSaveLayout::ExportFormatSuffix(m_dialogSaveLayout->Format());
            QBrush *brush = new QBrush();
            brush->setColor( QColor( Qt::white ) );
            QGraphicsScene *scene = scenes.at(i);
            scene->setBackgroundBrush( *brush );
            shadows[i]->setVisible(false);
            paper->setPen(QPen(QBrush(Qt::white, Qt::NoBrush), 0.1, Qt::NoPen));

            switch (m_dialogSaveLayout->Format())
            {
                case LayoutExportFormats::SVG:
                    paper->setVisible(false);
                    SvgFile(name, paper, scene, margins);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::PDF:
                    PdfFile(name, paper, scene, ignorePrinterFields, margins);
                    break;
                case LayoutExportFormats::PNG:
                    PngFile(name, paper, scene, margins);
                    break;
                case LayoutExportFormats::OBJ:
                    paper->setVisible(false);
                    ObjFile(name, paper, scene);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::PS:
                    PsFile(name, paper, scene, ignorePrinterFields, margins);
                    break;
                case LayoutExportFormats::EPS:
                    EpsFile(name, paper, scene, ignorePrinterFields, margins);
                    break;
                case LayoutExportFormats::DXF_AC1006_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1006, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1009_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1009, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1012_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1012, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1014_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1014, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1015_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1015, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1018_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1018, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1021_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1021, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1024_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1024, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1027_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1027, m_dialogSaveLayout->IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                default:
                    qDebug() << "Can't recognize file type." << Q_FUNC_INFO;
                    break;
            }
            paper->setPen(QPen(Qt::black, 1));
            brush->setColor( QColor( Qt::gray ) );
            brush->setStyle( Qt::SolidPattern );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(true);
            delete brush;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindowsNoGUI::FileName() const
{
    QString fileName;
    qApp->GetPatternPath().isEmpty() ? fileName = tr("unnamed") : fileName = qApp->GetPatternPath();
    return QFileInfo(fileName).baseName();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::SetSizeHeightForIndividualM() const
{
    const QHash<QString, QSharedPointer<VInternalVariable> > * vars = pattern->DataVariables();

    if (vars->contains(size_M))
    {
        pattern->SetSize(*vars->value(size_M)->GetValue());
    }
    else
    {
        pattern->SetSize(0);
    }

    if (vars->contains(height_M))
    {
        pattern->SetHeight(*vars->value(height_M)->GetValue());
    }
    else
    {
        pattern->SetHeight(0);
    }

    doc->SetPatternWasChanged(true);
    emit doc->UpdatePatternLabel();
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::ExportFMeasurementsToCSVData(const QString &fileName, bool withHeader, int mib,
                                                    const QChar &separator) const
{
    QxtCsvModel csv;

    csv.insertColumn(0);
    csv.insertColumn(1);
    csv.insertColumn(2);

    if (withHeader)
    {
        csv.setHeaderText(0, tr("Name"));
        csv.setHeaderText(1, tr("Value"));
        csv.setHeaderText(2, tr("Description"));
    }

    const QVector<VFinalMeasurement> measurements = doc->GetFinalMeasurements();
    const VContainer completeData = doc->GetCompleteData();

    for (int i=0; i < measurements.size(); ++i)
    {
        const VFinalMeasurement &m = measurements.at(i);

        csv.insertRow(i);
        csv.setText(i, 0, m.name); // name

        if (not m.formula.isEmpty())
        {
            try
            {
                QScopedPointer<Calculator> cal(new Calculator());
                const qreal result = cal->EvalFormula(completeData.DataVariables(), m.formula);

                csv.setText(i, 1, qApp->LocaleToString(result)); // value

                if (qIsInf(result) || qIsNaN(result))
                {
                    qCritical("%s\n\n%s", qUtf8Printable(tr("Export final measurements error.")),
                              qUtf8Printable(tr("Value in line %1 is infinite or NaN. Please, check your calculations.")
                                             .arg(i+1)));
                    if (not VApplication::IsGUIMode())
                    {
                        qApp->exit(V_EX_DATAERR);
                    }
                    return false;
                }
            }
            catch (qmu::QmuParserError &e)
            {
                qCritical("%s\n\n%s", qUtf8Printable(tr("Export final measurements error.")),
                          qUtf8Printable(tr("Parser error at line %1: %2.").arg(i+1).arg(e.GetMsg())));
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_DATAERR);
                }
                return false;
            }
        }

        csv.setText(i, 2, m.description); // description
    }

    QString error;
    const bool success = csv.toCSV(fileName, error, withHeader, separator, QTextCodec::codecForMib(mib));

    if (not success)
    {
        qCritical("%s\n\n%s", qUtf8Printable(tr("Export final measurements error.")),
                  qUtf8Printable(tr("File error %1.").arg(error)));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_CANTCREAT);
        }
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------
QSharedPointer<VMeasurements> MainWindowsNoGUI::OpenMeasurementFile(const QString &path) const
{
    QSharedPointer<VMeasurements> m;
    if (path.isEmpty())
    {
        return m;
    }

    try
    {
        m = QSharedPointer<VMeasurements>(new VMeasurements(pattern));
        m->setXMLContent(path);

        if (m->Type() == MeasurementsType::Unknown)
        {
            VException e(tr("Measurement file has unknown format."));
            throw e;
        }

        if (m->Type() == MeasurementsType::Multisize)
        {
            VVSTConverter converter(path);
            m->setXMLContent(converter.Convert());// Read again after conversion
        }
        else
        {
            VVITConverter converter(path);
            m->setXMLContent(converter.Convert());// Read again after conversion
        }

        if (not m->IsDefinedKnownNamesValid())
        {
            VException e(tr("Measurement file contains invalid known measurement(s)."));
            throw e;
        }

        CheckRequiredMeasurements(m.data());

        if (m->Type() == MeasurementsType::Multisize)
        {
            if (m->MUnit() == Unit::Inch)
            {
                qCCritical(vMainNoGUIWindow, "%s\n\n%s", qUtf8Printable(tr("Wrong units.")),
                          qUtf8Printable(tr("Application doesn't support multisize table with inches.")));
                m->clear();
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_DATAERR);
                }
                return m;
            }
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainNoGUIWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        m->clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return m;
    }
    return m;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::CheckRequiredMeasurements(const VMeasurements *m) const
{
    const QSet<QString> match = doc->ListMeasurements().toSet().subtract(m->ListAll().toSet());
    if (not match.isEmpty())
    {
        QList<QString> list = match.toList();
        for (int i = 0; i < list.size(); ++i)
        {
            list[i] = qApp->TrVars()->MToUser(list.at(i));
        }

        VException e(tr("Measurement file doesn't include all required measurements."));
        e.AddMoreInformation(tr("Please, additionally provide: %1").arg(QStringList(list).join(QStringLiteral(", "))));
        throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int MainWindowsNoGUI::ContinueIfLayoutStale()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("The layout is stale."));
    msgBox.setText(tr("The layout was not updated since last pattern modification. Do you want to continue?"));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout*>(msgBox.layout());
    SCASSERT(layout != nullptr)
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
