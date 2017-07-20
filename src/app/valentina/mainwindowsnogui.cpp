/************************************************************************
 **
 **  @file   mainwindowsnogui.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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
#include "../vlayout/vlayoutgenerator.h"
#include "dialogs/dialoglayoutprogress.h"
#include "dialogs/dialogsavelayout.h"
#include "../vlayout/vposter.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "../vtools/tools/vabstracttool.h"
#include "../vtools/tools/vtoolseamallowance.h"

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

#ifdef Q_OS_WIN
#   define PDFTOPS "pdftops.exe"
#else
#   define PDFTOPS "pdftops"
#endif

namespace
{
bool CreateLayoutPath(const QString &path)
{
    bool usedNotExistedDir = true;
    QDir dir(path);
    dir.setPath(path);
    if (not dir.exists(path))
    {
        usedNotExistedDir = dir.mkpath(".");
    }
    return usedNotExistedDir;
}

void RemoveLayoutPath(const QString &path, bool usedNotExistedDir)
{
    if (usedNotExistedDir)
    {
        QDir dir(path);
        dir.rmpath(".");
    }
}
}

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::MainWindowsNoGUI(QWidget *parent)
    : VAbstractMainWindow(parent),
      listDetails(),
      currentScene(nullptr),
      tempSceneLayout(nullptr),
      pattern(new VContainer(qApp->TrVars(), qApp->patternUnitP())),
      doc(nullptr),
      papers(),
      shadows(),
      scenes(),
      details(),
      detailsOnLayout(),
      undoAction(nullptr),
      redoAction(nullptr),
      actionDockWidgetToolOptions(nullptr),
      actionDockWidgetGroups(nullptr),
      curFile(),
      isNoScaling(false),
      isLayoutStale(true),
      ignorePrinterFields(false),
      margins(),
      paperSize(),
      isTiled(false),
      isAutoCrop(false),
      isUnitePages(false),
      layoutPrinterName()

{
    InitTempLayoutScene();
}

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::~MainWindowsNoGUI()
{
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
        LayoutSettings(lGenerator);
        tButton->setChecked(false);
    }
    else
    {
        tButton->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindowsNoGUI::LayoutSettings(VLayoutGenerator& lGenerator)
{
    lGenerator.SetDetails(listDetails);
    DialogLayoutProgress progress(listDetails.count(), this);
    if (VApplication::IsGUIMode())
    {
        connect(&lGenerator, &VLayoutGenerator::Start, &progress, &DialogLayoutProgress::Start);
        connect(&lGenerator, &VLayoutGenerator::Arranged, &progress, &DialogLayoutProgress::Arranged);
        connect(&lGenerator, &VLayoutGenerator::Error, &progress, &DialogLayoutProgress::Error);
        connect(&lGenerator, &VLayoutGenerator::Finished, &progress, &DialogLayoutProgress::Finished);
        connect(&progress, &DialogLayoutProgress::Abort, &lGenerator, &VLayoutGenerator::Abort);
    }
    else
    {
        connect(&lGenerator, &VLayoutGenerator::Error, this, &MainWindowsNoGUI::ErrorConsoleMode);
    }
    lGenerator.Generate();

    switch (lGenerator.State())
    {
        case LayoutErrors::NoError:
            CleanLayout();
            papers = lGenerator.GetPapersItems();// Blank sheets
            details = lGenerator.GetAllDetailsItems();// All details items
            detailsOnLayout = lGenerator.GetAllDetails();// All details items
            shadows = CreateShadows(papers);
            scenes = CreateScenes(papers, shadows, details);
            PrepareSceneList();
            ignorePrinterFields = not lGenerator.IsUsePrinterFields();
            margins = lGenerator.GetPrinterFields();
            paperSize = QSizeF(lGenerator.GetPaperWidth(), lGenerator.GetPaperHeight());
            isAutoCrop = lGenerator.GetAutoCrop();
            isUnitePages = lGenerator.IsUnitePages();
            isLayoutStale = false;
            if (VApplication::IsGUIMode())
            {
                QApplication::alert(this);
            }
            break;
        case LayoutErrors::ProcessStoped:
        case LayoutErrors::PrepareLayoutError:
        case LayoutErrors::EmptyPaperError:
            if (VApplication::IsGUIMode())
            {
                QApplication::alert(this);
            }
            return false;
        default:
            break;

    }
    return true;
}
//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ErrorConsoleMode(const LayoutErrors &state)
{
    switch (state)
    {
        case LayoutErrors::NoError:
            return;
        case LayoutErrors::PrepareLayoutError:
            qCritical() << tr("Couldn't prepare data for creation layout");
            break;
        case LayoutErrors::EmptyPaperError:
            qCritical() << tr("Several workpieces left not arranged, but none of them match for paper");
            break;
        case LayoutErrors::ProcessStoped:
        default:
            break;
    }

    qApp->exit(V_EX_DATAERR);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportData(const QVector<VLayoutPiece> &listDetails, const DialogSaveLayout &dialog)
{
    const LayoutExportFormats format = dialog.Format();

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
        if (dialog.Mode() == Draw::Layout)
        {
            for (int i = 0; i < detailsOnLayout.size(); ++i)
            {
                const QString name = dialog.Path() + QLatin1String("/") + dialog.FileName() + QString::number(i+1)
                        + DialogSaveLayout::ExportFromatSuffix(dialog.Format());

                QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
                SCASSERT(paper != nullptr)

                ExportApparelLayout(dialog, detailsOnLayout.at(i), name, paper->rect().size().toSize());
            }
        }
        else
        {
            ExportDetailsAsApparelLayout(dialog, listDetails);
        }
    }
    else
    {
        if (dialog.Mode() == Draw::Layout)
        {
            ExportFlatLayout(dialog, scenes, papers, shadows, details, ignorePrinterFields, margins);
        }
        else
        {
            ExportDetailsAsFlatLayout(dialog, listDetails);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportFlatLayout(const DialogSaveLayout &dialog, const QList<QGraphicsScene *> &scenes,
                                        const QList<QGraphicsItem *> &papers, const QList<QGraphicsItem *> &shadows,
                                        const QList<QList<QGraphicsItem *> > &details, bool ignorePrinterFields,
                                        const QMarginsF &margins)
{
    const QString path = dialog.Path();
    bool usedNotExistedDir = CreateLayoutPath(path);
    if (not usedNotExistedDir)
    {
        qCritical() << tr("Can't create a path");
        return;
    }

    qApp->ValentinaSettings()->SetPathLayout(path);
    const LayoutExportFormats format = dialog.Format();

    if (format == LayoutExportFormats::PDFTiled && dialog.Mode() == Draw::Layout)
    {
        const QString name = path + QLatin1String("/") + dialog.FileName() + QString::number(1)
                + DialogSaveLayout::ExportFromatSuffix(dialog.Format());
        PdfTiledFile(name);
    }
    else
    {
        ExportScene(dialog, scenes, papers, shadows, details, ignorePrinterFields, margins);
    }

    RemoveLayoutPath(path, usedNotExistedDir);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportDetailsAsFlatLayout(const DialogSaveLayout &dialog,
                                                 const QVector<VLayoutPiece> &listDetails)
{
    if (listDetails.isEmpty())
    {
        return;
    }

    QScopedPointer<QGraphicsScene> scene(new QGraphicsScene());

    QList<QGraphicsItem *> list;
    for (int i=0; i < listDetails.count(); ++i)
    {
        QGraphicsItem *item = listDetails.at(i).GetItem(dialog.IsTextAsPaths());
        item->setPos(listDetails.at(i).GetMx(), listDetails.at(i).GetMy());
        list.append(item);
    }

    for (int i=0; i < list.size(); ++i)
    {
        scene->addItem(list.at(i));
    }

    QList<QGraphicsItem *> papers;// Blank sheets
    QRect rect = scene->itemsBoundingRect().toRect();

    const int mx = rect.x();
    const int my = rect.y();

    QTransform matrix;
    matrix = matrix.translate(-mx, -my);

    for (int i=0; i < list.size(); ++i)
    {
        list.at(i)->setTransform(matrix);
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
    ExportFlatLayout(dialog, scenes, papers, shadows, details, ignorePrinterFields,
                     QMarginsF(margin, margin, margin, margin));

    qDeleteAll(scenes);//Scene will clear all other items
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportApparelLayout(const DialogSaveLayout &dialog, const QVector<VLayoutPiece> &details,
                                           const QString &name, const QSize &size) const
{
    const QString path = dialog.Path();
    bool usedNotExistedDir = CreateLayoutPath(path);
    if (not usedNotExistedDir)
    {
        qCritical() << tr("Can't create a path");
        return;
    }

    qApp->ValentinaSettings()->SetPathLayout(path);
    const LayoutExportFormats format = dialog.Format();

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
            AAMADxfFile(name, DRW::AC1006, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1009_AAMA:
            AAMADxfFile(name, DRW::AC1009, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1012_AAMA:
            AAMADxfFile(name, DRW::AC1012, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1014_AAMA:
            AAMADxfFile(name, DRW::AC1014, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1015_AAMA:
            AAMADxfFile(name, DRW::AC1015, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1018_AAMA:
            AAMADxfFile(name, DRW::AC1018, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1021_AAMA:
            AAMADxfFile(name, DRW::AC1021, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1024_AAMA:
            AAMADxfFile(name, DRW::AC1024, dialog.IsBinaryDXFFormat(), size, details);
            break;
        case LayoutExportFormats::DXF_AC1027_AAMA:
            AAMADxfFile(name, DRW::AC1027, dialog.IsBinaryDXFFormat(), size, details);
            break;
        default:
            qDebug() << "Can't recognize file type." << Q_FUNC_INFO;
            break;
    }

    RemoveLayoutPath(path, usedNotExistedDir);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportDetailsAsApparelLayout(const DialogSaveLayout &dialog,
                                                    QVector<VLayoutPiece> listDetails)
{
    if (listDetails.isEmpty())
    {
        return;
    }

    QScopedPointer<QGraphicsScene> scene(new QGraphicsScene());

    QList<QGraphicsItem *> list;
    for (int i=0; i < listDetails.count(); ++i)
    {
        QGraphicsItem *item = listDetails.at(i).GetItem(dialog.IsTextAsPaths());
        item->setPos(listDetails.at(i).GetMx(), listDetails.at(i).GetMy());
        list.append(item);
    }

    for (int i=0; i < list.size(); ++i)
    {
        scene->addItem(list.at(i));
    }

    QRect rect = scene->itemsBoundingRect().toRect();

    const int mx = rect.x();
    const int my = rect.y();

    QTransform matrix;
    matrix = matrix.translate(-mx, -my);

    for (int i=0; i < list.size(); ++i)
    {
        list.at(i)->setTransform(matrix);
    }

    rect = scene->itemsBoundingRect().toRect();

    for (int i=0; i < listDetails.count(); ++i)
    {
        QTransform moveMatrix;
        matrix = matrix.translate(listDetails.at(i).GetMx(), listDetails.at(i).GetMy());
        matrix = matrix.translate(-mx, -my);
        moveMatrix *= listDetails.at(i).GetMatrix();
        listDetails[i].SetMatrix(moveMatrix);
        listDetails[i].SetMx(listDetails.at(i).GetMx()-mx);
        listDetails[i].SetMy(listDetails.at(i).GetMy()-my);
    }

    const QString name = dialog.Path() + QLatin1String("/") + dialog.FileName() + QString::number(1)
            + DialogSaveLayout::ExportFromatSuffix(dialog.Format());

    ExportApparelLayout(dialog, listDetails, name, rect.size());
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

    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );

    int count = 0;
    QSharedPointer<QVector<PosterData>> poster;
    QSharedPointer<VPoster> posterazor;

    if (isTiled)
    {
        poster = QSharedPointer<QVector<PosterData>>(new QVector<PosterData>());
        posterazor = QSharedPointer<VPoster>(new VPoster(printer));

        for (int i=0; i < scenes.size(); ++i)
        {
            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
            if (paper)
            {
                *poster += posterazor->Calc(paper->rect().toRect(), i);
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
                    #if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
                        QMarginsF printerMargins = printer->pageLayout().margins();
                        x = qFloor(ToPixel(printerMargins.left(),Unit::Mm));
                        y = qFloor(ToPixel(printerMargins.top(),Unit::Mm));
                    #else
                        qreal left = 0, top = 0, right = 0, bottom = 0;
                        printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
                        x = qFloor(ToPixel(left,Unit::Mm));
                        y = qFloor(ToPixel(top,Unit::Mm));
                    #endif
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
/**
 * @brief RefreshDetailsLabel call to ecalculate piece labels. For example after changing a font.
 */
void MainWindowsNoGUI::RefreshDetailsLabel()
{
    const QHash<quint32, VPiece> *list = pattern->DataPieces();
    QHash<quint32, VPiece>::const_iterator i = list->constBegin();
    while (i != list->constEnd())
    {
        if (VToolSeamAllowance *tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(i.key())))
        {
            tool->UpdatePatternInfo();
            tool->UpdateDetailLabel();
        }
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiece> MainWindowsNoGUI::PrepareDetailsForLayout(const QHash<quint32, VPiece> &details)
{
    QVector<VLayoutPiece> listDetails;
    if (not details.isEmpty())
    {
        QHash<quint32, VPiece>::const_iterator i = details.constBegin();
        while (i != details.constEnd())
        {
            VAbstractTool *tool = qobject_cast<VAbstractTool*>(VAbstractPattern::getTool(i.key()));
            SCASSERT(tool != nullptr)
            listDetails.append(VLayoutPiece::Create(i.value(), tool->getData()));
            ++i;
        }
    }

    return listDetails;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::InitTempLayoutScene()
{
    tempSceneLayout = new VMainGraphicsScene();
    tempSceneLayout->setBackgroundBrush( QBrush(QColor(Qt::gray), Qt::SolidPattern) );
}

//---------------------------------------------------------------------------------------------------------------------
QIcon MainWindowsNoGUI::ScenePreview(int i) const
{
    QImage image;
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        const QRectF r = paper->rect();
        // Create the image with the exact size of the shrunk scene
        image = QImage(QSize(static_cast<qint32>(r.width()), static_cast<qint32>(r.height())), QImage::Format_RGB32);

        if (not image.isNull())
        {
            image.fill(Qt::white);
            QPainter painter(&image);
            painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush ( QBrush ( Qt::NoBrush ) );
            scenes.at(i)->render(&painter, r, r, Qt::IgnoreAspectRatio);
            painter.end();
        }
        else
        {
            qWarning()<<"Cannot create image. Size too big";
        }
    }
    else
    {
        image = QImage(QSize(101, 146), QImage::Format_RGB32);
        image.fill(Qt::white);
    }
    return QIcon(QBitmap::fromImage(image));
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> MainWindowsNoGUI::CreateShadows(const QList<QGraphicsItem *> &papers)
{
    QList<QGraphicsItem *> shadows;

    for (int i=0; i< papers.size(); ++i)
    {
        qreal x1=0, y1=0, x2=0, y2=0;
        QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (item)
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
    for (int i=0; i<papers.size(); ++i)
    {
        QGraphicsScene *scene = new VMainGraphicsScene();
        scene->setBackgroundBrush(QBrush(QColor(Qt::gray), Qt::SolidPattern));
        scene->addItem(shadows.at(i));
        scene->addItem(papers.at(i));

        QList<QGraphicsItem *> paperDetails = details.at(i);
        for (int i=0; i < paperDetails.size(); ++i)
        {
            scene->addItem(paperDetails.at(i));
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
void MainWindowsNoGUI::SvgFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene) const
{
    QSvgGenerator generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    generator.setViewBox(paper->rect());
    generator.setTitle(tr("Pattern"));
    generator.setDescription(doc->GetDescription());
    generator.setResolution(static_cast<int>(PrintDPI));
    QPainter painter;
    painter.begin(&generator);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthHairLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    scene->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PngFile save layout to png file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PngFile(const QString &name, QGraphicsRectItem *paper, QGraphicsScene *scene) const
{
    const QRectF r = paper->rect();
    // Create the image with the exact size of the shrunk scene
    QImage image(r.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);                                              // Start all pixels transparent
    QPainter painter(&image);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    printer.setCreator(QGuiApplication::applicationDisplayName()+QLatin1String(" ")+
                       QCoreApplication::applicationVersion());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(name);
    printer.setDocName(FileName());
    const QRectF r = paper->rect();
    printer.setResolution(static_cast<int>(PrintDPI));
    printer.setOrientation(QPrinter::Portrait);
    printer.setFullPage(ignorePrinterFields);
    printer.setPaperSize ( QSizeF(FromPixel(r.width() + margins.left() + margins.right(), Unit::Mm),
                                  FromPixel(r.height() + margins.top() + margins.bottom(), Unit::Mm)),
                           QPrinter::Millimeter );

    const qreal left = FromPixel(margins.left(), Unit::Mm);
    const qreal top = FromPixel(margins.top(), Unit::Mm);
    const qreal right = FromPixel(margins.right(), Unit::Mm);
    const qreal bottom = FromPixel(margins.bottom(), Unit::Mm);
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
    const bool success = printer.setPageMargins(QMarginsF(left, top, right, bottom), QPageLayout::Millimeter);
    if (not success)
    {
        qWarning() << tr("Cannot set printer margins");
    }
#else
    printer.setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)

    QPainter painter;
    if (painter.begin( &printer ) == false)
    { // failed to open file
        qCritical("%s", qUtf8Printable(tr("Can't open printer %1").arg(name)));
        return;
    }
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    printer.setPageSize(QPrinter::A4);// Want to be sure that page size is correct.

    // Call IsPagesFit after setting a printer settings and check if pages is not bigger than printer's paper size
    if (not isTiled && not IsPagesFit(printer.paperRect().size()))
    {
        qWarning()<<tr("Pages will be cropped because they do not fit printer paper size.");
    }

    printer.setOutputFileName(name);
    printer.setResolution(static_cast<int>(PrintDPI));
    PrintPages( &printer );
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
        QStringList params = QStringList() << "-eps" << tmp.fileName() << name;
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
        const QString msg = tr("Creating file '%1' failed! %2").arg(params.last()).arg(proc.errorString());
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

    switch (*pattern->GetPatternUnit())
    {
        case Unit::Cm:
            generator.setInsunits(VarInsunits::Centimeters);
            break;
        case Unit::Mm:
            generator.setInsunits(VarInsunits::Millimeters);
            break;
        case Unit::Inch:
            generator.setInsunits(VarInsunits::Inches);
            break;
        case Unit::Px:
        case Unit::LAST_UNIT_DO_NOT_USE:
            Q_UNREACHABLE();
            break;
    }

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

    switch (*pattern->GetPatternUnit())
    {
        case Unit::Cm:
            generator.setInsunits(VarInsunits::Centimeters);
            break;
        case Unit::Mm:
            generator.setInsunits(VarInsunits::Millimeters);
            break;
        case Unit::Inch:
            generator.setInsunits(VarInsunits::Inches);
            break;
        case Unit::Px:
        case Unit::LAST_UNIT_DO_NOT_USE:
            Q_UNREACHABLE();
            break;
    }

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
        paper->setPen(QPen(Qt::black, widthMainLine));
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
    for (int i = 0; i < details.size(); ++i)
    {
        const QList<QGraphicsItem *> &paperItems = details.at(i);
        for (int j = 0; j < paperItems.size(); ++j)
        {
            QList<QGraphicsItem *> pieceChildren = paperItems.at(j)->childItems();
            for (int k = 0; k < pieceChildren.size(); ++k)
            {
                QGraphicsItem *item = pieceChildren.at(k);
                if (item->type() == QGraphicsSimpleTextItem::Type)
                {
                    if(QGraphicsSimpleTextItem *textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(item))
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
    for (int i = 0; i < details.size(); ++i)
    {
        const QList<QGraphicsItem *> &paperItems = details.at(i);
        for (int j = 0; j < paperItems.size(); ++j)
        {
            QList<QGraphicsItem *> pieceChildren = paperItems.at(i)->childItems();
            for (int k = 0; k < pieceChildren.size(); ++k)
            {
                QGraphicsItem *item = pieceChildren.at(k);
                if (item->type() == QGraphicsSimpleTextItem::Type)
                {
                    if(QGraphicsSimpleTextItem *textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(item))
                    {
                        QString text = textItem->text();
                        text.replace(placeholder, "");
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
    printer->setCreator(QGuiApplication::applicationDisplayName()+" "+QCoreApplication::applicationVersion());
    printer->setOrientation(QPrinter::Portrait);

    if (not isTiled)
    {
        QSizeF size = QSizeF(FromPixel(paperSize.width(), Unit::Mm), FromPixel(paperSize.height(), Unit::Mm));
        if (isAutoCrop || isUnitePages)
        {
            auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
            if (paper)
            {
                size = QSizeF(FromPixel(paperSize.width(), Unit::Mm),
                              FromPixel(paper->rect().height() + margins.top() + margins.bottom(), Unit::Mm));
            }
        }

        const QPrinter::PageSize pSZ = FindTemplate(size);
        if (pSZ == QPrinter::Custom)
        {
            printer->setPaperSize (size, QPrinter::Millimeter );
        }
        else
        {
            printer->setPaperSize (pSZ);
        }
    }
    else
    {
        printer->setPaperSize(QPrinter::A4);
    }

    printer->setFullPage(ignorePrinterFields);

    const qreal left = FromPixel(margins.left(), Unit::Mm);
    const qreal top = FromPixel(margins.top(), Unit::Mm);
    const qreal right = FromPixel(margins.right(), Unit::Mm);
    const qreal bottom = FromPixel(margins.bottom(), Unit::Mm);
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
    const bool success = printer->setPageMargins(QMarginsF(left, top, right, bottom), QPageLayout::Millimeter);

    if (not success)
    {
        qWarning() << tr("Cannot set printer margins");
    }
#else
    printer->setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)

    switch(printType)
    {
        case PrintType::PrintPDF:
        {
            const QString outputFileName = QDir::homePath() + QDir::separator() + FileName();
            #ifdef Q_OS_WIN
            printer->setOutputFileName(outputFileName);
            #else
            printer->setOutputFileName(outputFileName + QLatin1String(".pdf"));
            #endif

            #ifdef Q_OS_MAC
            printer->setOutputFormat(QPrinter::NativeFormat);
            #else
            printer->setOutputFormat(QPrinter::PdfFormat);
            #endif
            break;
        }
        case PrintType::PrintNative:
            printer->setOutputFileName("");//Disable printing to file if was enabled.
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
        auto *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            // Hide shadow and paper border
            PreparePaper(i);

            // Render png
            QImage image(target.size(), QImage::Format_RGB32);
            image.fill(Qt::white);
            QPainter painter(&image);
            painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
QPrinter::PaperSize MainWindowsNoGUI::FindTemplate(const QSizeF &size) const
{
    if (size == QSizeF(841, 1189))
    {
        return QPrinter::A0;
    }

    if (size == QSizeF(594, 841))
    {
        return QPrinter::A1;
    }

    if (size == QSizeF(420, 594))
    {
        return QPrinter::A2;
    }

    if (size == QSizeF(297, 420))
    {
        return QPrinter::A3;
    }

    if (size == QSizeF(210, 297))
    {
        return QPrinter::A4;
    }

    if (size == QSizeF(215.9, 355.6))
    {
        return QPrinter::Legal;
    }

    if (size == QSizeF(215.9, 279.4))
    {
        return QPrinter::Letter;
    }

    return QPrinter::Custom;
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
        for (int i=1; i < papers.size(); ++i)
        {
            auto *p = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
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
void MainWindowsNoGUI::ExportScene(const DialogSaveLayout &dialog, const QList<QGraphicsScene *> &scenes,
                                   const QList<QGraphicsItem *> &papers, const QList<QGraphicsItem *> &shadows,
                                   const QList<QList<QGraphicsItem *> > &details, bool ignorePrinterFields,
                                   const QMarginsF &margins) const
{
    for (int i=0; i < scenes.size(); ++i)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            const QString name = dialog.Path() + QLatin1String("/") + dialog.FileName() + QString::number(i+1)
                    + DialogSaveLayout::ExportFromatSuffix(dialog.Format());
            QBrush *brush = new QBrush();
            brush->setColor( QColor( Qt::white ) );
            QGraphicsScene *scene = scenes.at(i);
            scene->setBackgroundBrush( *brush );
            shadows[i]->setVisible(false);
            paper->setPen(QPen(QBrush(Qt::white, Qt::NoBrush), 0.1, Qt::NoPen));

            switch (dialog.Format())
            {
                case LayoutExportFormats::SVG:
                    paper->setVisible(false);
                    SvgFile(name, paper, scene);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::PDF:
                    PdfFile(name, paper, scene, ignorePrinterFields, margins);
                    break;
                case LayoutExportFormats::PNG:
                    PngFile(name, paper, scene);
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
                    FlatDxfFile(name, DRW::AC1006, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1009_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1009, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1012_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1012, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1014_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1014, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1015_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1015, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1018_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1018, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1021_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1021, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1024_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1024, dialog.IsBinaryDXFFormat(), paper, scene, details);
                    paper->setVisible(true);
                    break;
                case LayoutExportFormats::DXF_AC1027_Flat:
                    paper->setVisible(false);
                    FlatDxfFile(name, DRW::AC1027, dialog.IsBinaryDXFFormat(), paper, scene, details);
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
    curFile.isEmpty() ? fileName = tr("unnamed") : fileName = curFile;
    return QFileInfo(fileName).baseName();
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
