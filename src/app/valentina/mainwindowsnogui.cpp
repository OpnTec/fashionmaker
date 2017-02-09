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
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vpatterndb/vgrainlinegeometry.h"
#include "../vtools/tools/vabstracttool.h"

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
            details = lGenerator.GetAllDetails();// All details
            CreateShadows();
            CreateScenes();
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
void MainWindowsNoGUI::ExportLayout(const DialogSaveLayout &dialog)
{

    const QString suf = dialog.Format().replace(".", "");
    const QString path = dialog.Path();
    QDir dir(path);
    dir.setPath(path);
    if (not dir.exists(path))
    {
        if (not dir.mkpath(path))
        {
            qCritical() << tr("Can't create path");
            return;
        }
    }
    qApp->ValentinaSettings()->SetPathLayout(path);
    const QString mask = dialog.FileName();

    for (int i=0; i < scenes.size(); ++i)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            const QString name = path + QLatin1String("/") + mask+QString::number(i+1) + dialog.Format();
            QBrush *brush = new QBrush();
            brush->setColor( QColor( Qt::white ) );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(false);
            paper->setPen(QPen(QBrush(Qt::white, Qt::NoBrush), 0.1, Qt::NoPen));
            const QStringList suffix = QStringList() << QLatin1String("svg")
                                                     << QLatin1String("png")
                                                     << QLatin1String("pdf")
                                                     << QLatin1String("eps")
                                                     << QLatin1String("ps")
                                                     << QLatin1String("obj")
                                                     << QLatin1String("dxf");
            switch (suffix.indexOf(suf))
            {
                case 0: //svg
                    paper->setVisible(false);
                    SvgFile(name, i);
                    paper->setVisible(true);
                    break;
                case 1: //png
                    PngFile(name, i);
                    break;
                case 2: //pdf
                    PdfFile(name, i);
                    break;
                case 3: //eps
                    EpsFile(name, i);
                    break;
                case 4: //ps
                    PsFile(name, i);
                    break;
                case 5: //obj
                    paper->setVisible(false);
                    ObjFile(name, i);
                    paper->setVisible(true);
                    break;
                case 6: //dxf
                    DxfFile(name, i);
                    break;
                default:
                    qDebug() << "Can't recognize file suffix." << Q_FUNC_INFO;
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
void MainWindowsNoGUI::SaveAsTiledPDF()
{
    isTiled = true;
    SaveLayoutAs();
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
    painter.setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
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
    isTiled = false;
    LayoutPrint();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrepareDetailsForLayout(const QHash<quint32, VPiece> *details)
{
    listDetails.clear();
    SCASSERT(details != nullptr)
    if (details->count() == 0)
    {
        return;
    }

    QHash<quint32, VPiece>::const_iterator i = details->constBegin();
    while (i != details->constEnd())
    {
        VAbstractTool *tool = qobject_cast<VAbstractTool*>(doc->getTool(i.key()));
        SCASSERT(tool != nullptr)
        listDetails.append(VLayoutPiece::Create(i.value(), tool->getData()));
        ++i;
    }
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
            painter.setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                                Qt::RoundCap, Qt::RoundJoin));
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
void MainWindowsNoGUI::CreateShadows()
{
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
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::CreateScenes()
{
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
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SvgFile save layout to svg file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::SvgFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        QSvgGenerator generator;
        generator.setFileName(name);
        generator.setSize(paper->rect().size().toSize());
        generator.setViewBox(paper->rect());
        generator.setTitle("Valentina. Pattern layout");
        generator.setDescription(doc->GetDescription());
        generator.setResolution(static_cast<int>(PrintDPI));
        QPainter painter;
        painter.begin(&generator);
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
        painter.end();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PngFile save layout to png file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PngFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        const QRectF r = paper->rect();
        // Create the image with the exact size of the shrunk scene
        QImage image(r.size().toSize(), QImage::Format_ARGB32);
        image.fill(Qt::transparent);                                              // Start all pixels transparent
        QPainter painter(&image);
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter, r, r, Qt::IgnoreAspectRatio);
        image.save(name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfFile save layout to pdf file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PdfFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        QPrinter printer;
        printer.setCreator(qApp->applicationDisplayName()+QLatin1String(" ")+qApp->applicationVersion());
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(name);
        printer.setDocName(FileName());
        const QRectF r = paper->rect();
        printer.setResolution(static_cast<int>(PrintDPI));
        // Set orientation
        if (paper->rect().height()>= paper->rect().width())
        {
            printer.setOrientation(QPrinter::Portrait);
        }
        else
        {
            printer.setOrientation(QPrinter::Landscape);
        }
        printer.setFullPage(ignorePrinterFields);
        printer.setPaperSize ( QSizeF(FromPixel(r.width() + margins.left() + margins.right(), Unit::Mm),
                                      FromPixel(r.height() + margins.top() + margins.bottom(), Unit::Mm)),
                               QPrinter::Millimeter );
        QPainter painter;
        if (painter.begin( &printer ) == false)
        { // failed to open file
            qCritical("%s", qUtf8Printable(tr("Can't open printer %1").arg(name)));
            return;
        }
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter, r, r, Qt::IgnoreAspectRatio);
        painter.end();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EpsFile save layout to eps file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::EpsFile(const QString &name, int i) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName(), i);
        QStringList params = QStringList() << "-eps" << tmp.fileName() << name;
        PdfToPs(params);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PsFile save layout to ps file.
 * @param name name layout file.
 */
void MainWindowsNoGUI::PsFile(const QString &name, int i) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName(), i);
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
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
    QApplication::restoreOverrideCursor();
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
void MainWindowsNoGUI::ObjFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        VObjPaintDevice generator;
        generator.setFileName(name);
        generator.setSize(paper->rect().size().toSize());
        generator.setResolution(static_cast<int>(PrintDPI));
        QPainter painter;
        painter.begin(&generator);
        scenes.at(i)->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
        painter.end();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

void MainWindowsNoGUI::DxfFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        VDxfPaintDevice generator;
        generator.setFileName(name);
        generator.setSize(paper->rect().size().toSize());
        generator.setResolution(PrintDPI);

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
        painter.begin(&generator);
        scenes.at(i)->render(&painter, paper->rect(), paper->rect(), Qt::IgnoreAspectRatio);
        painter.end();
    }
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
        paper->setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit()))));
        QBrush brush(Qt::gray);
        scenes.at(index)->setBackgroundBrush(brush);
        shadows.at(index)->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::SaveLayoutAs()
{
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

    QString fileName = QFileDialog::getSaveFileName(this, tr("Print to pdf"),
                                                    qApp->ValentinaSettings()->GetPathLayout()+"/"+FileName()+".pdf",
                                                    tr("PDF file (*.pdf)"));
    if (not fileName.isEmpty())
    {
        QFileInfo f( fileName );
        if(f.suffix().isEmpty())
        {
            fileName.append(".pdf");
        }
        qApp->ValentinaSettings()->SetPathLayout(f.absolutePath());

        printer.setOutputFileName(fileName);
        printer.setResolution(static_cast<int>(PrintDPI));
        PrintPages( &printer );
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
    printer->setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());

    // Set orientation
    if (paperSize.height() >= paperSize.width())
    {
        printer->setOrientation(QPrinter::Portrait);
    }
    else
    {
        printer->setOrientation(QPrinter::Landscape);
    }

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
            painter.setPen(QPen(Qt::black, qApp->toPixel(WidthMainLine(*pattern->GetPatternUnit())), Qt::SolidLine,
                                Qt::RoundCap, Qt::RoundJoin));
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
    msgBox.exec();
    return msgBox.result();
}
