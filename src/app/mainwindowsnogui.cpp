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
#include "../core/vapplication.h"
#include "../container/vcontainer.h"
#include "../../libs/vobj/vobjpaintdevice.h"
#include "../dialogs/app/dialoglayoutsettings.h"
#include "../../libs/vlayout/vlayoutgenerator.h"
#include "../dialogs/app/dialoglayoutprogress.h"
#include "../dialogs/app/dialogsavelayout.h"
#include "../../libs/vlayout/vposter.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QProcess>
#include <QToolButton>
#include <QtSvg>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>

#ifdef Q_OS_WIN
#   define PDFTOPS "pdftops.exe"
#else
#   define PDFTOPS "pdftops"
#endif

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::MainWindowsNoGUI(QWidget *parent)
    : QMainWindow(parent), listDetails(QVector<VLayoutDetail>()), currentScene(nullptr), tempSceneLayout(nullptr),
      pattern(new VContainer()), doc(nullptr), papers(QList<QGraphicsItem *>()), shadows(QList<QGraphicsItem *>()),
      scenes(QList<QGraphicsScene *>()), details(QList<QList<QGraphicsItem *> >()), undoAction(nullptr),
      redoAction(nullptr), actionDockWidgetToolOptions(nullptr), curFile(QString()), isLayoutStale(true), isTiled(false)
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
        VLayoutGenerator lGenerator(this);
        lGenerator.SetDetails(listDetails);

        DialogLayoutSettings layout(&lGenerator, this);
        if (layout.exec() == QDialog::Rejected)
        {
            tButton->setChecked(false);
            return;
        }

        DialogLayoutProgress progress(listDetails.count(), this);

        connect(&lGenerator, &VLayoutGenerator::Start, &progress, &DialogLayoutProgress::Start);
        connect(&lGenerator, &VLayoutGenerator::Arranged, &progress, &DialogLayoutProgress::Arranged);
        connect(&lGenerator, &VLayoutGenerator::Error, &progress, &DialogLayoutProgress::Error);
        connect(&lGenerator, &VLayoutGenerator::Finished, &progress, &DialogLayoutProgress::Finished);
        connect(&progress, &DialogLayoutProgress::Abort, &lGenerator, &VLayoutGenerator::Abort);

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
                isLayoutStale = false;
                break;
            case LayoutErrors::ProcessStoped:
                break;
            case LayoutErrors::PrepareLayoutError:
            case LayoutErrors::PaperSizeError:
            case LayoutErrors::EmptyPaperError:
                break;
            default:
                break;
        }
        tButton->setChecked(false);
    }
    else
    {
        tButton->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::ExportLayoutAs()
{
    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            return;
        }
    }
    QMap<QString, QString> extByMessage = InitFormates();
    DialogSaveLayout dialog(extByMessage, scenes.size(), FileName(), this);

    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }

    QString suf = dialog.Formate();
    suf.replace(".", "");

    const QString path = dialog.Path();
    qApp->getSettings()->SetPathLayout(path);
    const QString mask = dialog.FileName();

    for (int i=0; i < scenes.size(); ++i)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            const QString name = path + "/" + mask+QString::number(i+1) + dialog.Formate();
            QBrush *brush = new QBrush();
            brush->setColor( QColor( Qt::white ) );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(false);
            paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
            const QStringList suffix = QStringList() << "svg" << "png" << "pdf" << "eps" << "ps" << "obj";
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
                default:
                    qDebug() << "Can't recognize file suffix." << Q_FUNC_INFO;
                    break;
            }
            paper->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
            brush->setColor( QColor( Qt::gray ) );
            brush->setStyle( Qt::SolidPattern );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(true);
            delete brush;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::SaveAsPDF()
{
    if (not isPagesUniform())
    {
        qCritical()<<tr("For saving multypage document all sheet should have the same size. Use export "
                        "function instead.");
        return;
    }
    isTiled = false;
    SaveLayoutAs();
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
    if (printer == nullptr)
    {
        return;
    }

    const QVector<QImage> images = AllSheets();

    QVector<QImage> poster;
    if (isTiled)
    {
        VPoster posterazor(printer);
        for (int i=0; i < images.size(); i++)
        {
            poster += posterazor.Generate(images.at(i), i+1, images.size());
        }
    }
    else
    {
        poster = images;
    }

    QPainter painter;
    if (not painter.begin(printer))
    { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    // Handle the fromPage(), toPage(), supportsMultipleCopies(), and numCopies() values from QPrinter.
    int firstPage = printer->fromPage() - 1;
    if (firstPage >= poster.size())
    {
        return;
    }
    if (firstPage == -1)
    {
        firstPage = 0;
    }

    int lastPage = printer->toPage() - 1;
    if (lastPage == -1 || lastPage >= poster.size())
    {
        lastPage = poster.size() - 1;
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
            painter.drawImage(QPointF(), poster.at(index));
        }
    }

    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::PrintPreviewOrigin()
{
    if (not isPagesUniform())
    {
        qCritical()<<tr("For previewing multypage document all sheet should have the same size.");
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
        qCritical()<<tr("For printing multypages document all sheet should have the same size.");
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
void MainWindowsNoGUI::PrepareDetailsForLayout(const QHash<quint32, VDetail> *details)
{
    SCASSERT(details != nullptr)
    if (details->count() == 0)
    {
        listDetails.clear();
        return;
    }

    listDetails.clear();
    QHashIterator<quint32, VDetail> idetail(*details);
    while (idetail.hasNext())
    {
        idetail.next();
        VLayoutDetail det = VLayoutDetail();
        det.SetCountourPoints(idetail.value().ContourPoints(pattern));
        det.SetSeamAllowencePoints(idetail.value().SeamAllowancePoints(pattern),
                                   idetail.value().getSeamAllowance());
        det.setName(idetail.value().getName());
        det.setWidth(qApp->toPixel(idetail.value().getWidth()));

        listDetails.append(det);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::InitTempLayoutScene()
{
    tempSceneLayout = new QGraphicsScene();
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
            painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.setBrush ( QBrush ( Qt::NoBrush ) );
            scenes.at(i)->render(&painter);
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
        QGraphicsScene *scene = new QGraphicsScene();
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
QMap<QString, QString> MainWindowsNoGUI::InitFormates() const
{
    QMap<QString, QString> extByMessage;
    extByMessage[ tr("Svg files (*.svg)") ] = ".svg";
    extByMessage[ tr("PDF files (*.pdf)") ] = ".pdf";
    extByMessage[ tr("Images (*.png)") ] = ".png";
    extByMessage[ tr("Wavefront OBJ (*.obj)") ] = ".obj";

    QProcess proc;
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    proc.start(qApp->applicationDirPath()+"/"+PDFTOPS); // Seek pdftops in app bundle or near valentin.exe
#else
    proc.start(PDFTOPS); // Seek pdftops in standard path
#endif
    if (proc.waitForFinished(15000))
    {
        extByMessage[ tr("PS files (*.ps)") ] = ".ps";
        extByMessage[ tr("EPS files (*.eps)") ] = ".eps";
    }
    else
    {
        qDebug()<<PDFTOPS<<"error"<<proc.error()<<proc.errorString();
    }
    return extByMessage;
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
        generator.setResolution(static_cast<int>(qApp->PrintDPI));
        QPainter painter;
        painter.begin(&generator);
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine()), Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter);
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
        QImage image(QSize(static_cast<qint32>(r.width()), static_cast<qint32>(r.height())), QImage::Format_ARGB32);
        image.fill(Qt::transparent);                                              // Start all pixels transparent
        QPainter painter(&image);
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter, r, r);
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
        printer.setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(name);
        printer.setDocName(FileName());
        const QRectF r = paper->rect();
        printer.setResolution(static_cast<int>(qApp->PrintDPI));
        // Set orientation
        if (paper->rect().height()>= paper->rect().width())
        {
            printer.setOrientation(QPrinter::Portrait);
        }
        else
        {
            printer.setOrientation(QPrinter::Landscape);
        }
        printer.setPaperSize ( QSizeF(qApp->fromPixel(r.width(), Unit::Mm), qApp->fromPixel(r.height(), Unit::Mm)),
                               QPrinter::Millimeter );
        QPainter painter;
        if (painter.begin( &printer ) == false)
        { // failed to open file
            qCritical("Can't open printer %s", qPrintable(name));
            return;
        }
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter);
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
    proc.start(PDFTOPS, params);
    proc.waitForFinished(15000);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    QFile f(params.last());
    if (f.exists() == false)
    {
        QString msg = QString(tr("Creating file '%1' failed! %2")).arg(params.last()).arg(proc.errorString());
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
        generator.setResolution(static_cast<int>(qApp->PrintDPI));
        QPainter painter;
        painter.begin(&generator);
        scenes.at(i)->render(&painter);
        painter.end();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QImage> MainWindowsNoGUI::AllSheets()
{
    QVector<QImage> images;
    for (int i=0; i < scenes.size(); ++i)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (paper)
        {
            // Hide shadow and paper border
            QBrush *brush = new QBrush();
            brush->setColor( QColor( Qt::white ) );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(false);
            paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));// border

            // Render png
            const QRectF r = paper->rect();
            // Create the image with the exact size of the shrunk scene
            QImage image(QSize(static_cast<qint32>(r.width()), static_cast<qint32>(r.height())), QImage::Format_RGB32);
            image.fill(Qt::white);
            QPainter painter(&image);
            painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.setBrush ( QBrush ( Qt::NoBrush ) );
            scenes.at(i)->render(&painter);
            painter.end();
            images.append(image);

            // Resore
            paper->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
            brush->setColor( QColor( Qt::gray ) );
            brush->setStyle( Qt::SolidPattern );
            scenes[i]->setBackgroundBrush( *brush );
            shadows[i]->setVisible(true);
            delete brush;
        }
    }
    return images;
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
    SetPrinterSettings(&printer);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Print to pdf"),
                                                    qApp->getSettings()->GetPathLayout()+"/"+FileName()+".pdf",
                                                    tr("PDF file (*.pdf)"));
    if (not fileName.isEmpty())
    {
        QFileInfo f( fileName );
        if(f.suffix().isEmpty())
        {
            fileName.append(".pdf");
        }
        qApp->getSettings()->SetPathLayout(f.absolutePath());

        printer.setOutputFileName(fileName);
        printer.setResolution(static_cast<int>(VApplication::PrintDPI));
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
    QPrinterInfo def = QPrinterInfo::defaultPrinter();

    //if there is no default printer set the print preview won't show
    if(def.isNull() || def.printerName().isEmpty())
    {
        if(QPrinterInfo::availablePrinters().isEmpty())
        {
            QMessageBox::critical(this, tr("Print error"),
                                  tr("Cannot proceed because there are no available printers in your system."),
                                  QMessageBox::Ok);
            return;
        }
        else
        {
            def = QPrinterInfo::availablePrinters().first();
        }
    }

    QPrinter printer(def, QPrinter::ScreenResolution);
    printer.setResolution(static_cast<int>(VApplication::PrintDPI));
    SetPrinterSettings(&printer);
    // display print preview dialog
    QPrintPreviewDialog  preview(&printer);
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
    QPrinter printer(QPrinter::HighResolution);
    SetPrinterSettings(&printer);
    QPrintDialog dialog( &printer, this );
    // If only user couldn't change page margins we could use method setMinMax();
    dialog.setOption(QPrintDialog::PrintCurrentPage, false);
    if ( dialog.exec() == QDialog::Accepted )
    {
        printer.setResolution(static_cast<int>(VApplication::PrintDPI));
        PrintPages( &printer );
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindowsNoGUI::SetPrinterSettings(QPrinter *printer)
{
    SCASSERT(printer != nullptr)
    printer->setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());

    // Set orientation
    if (papers.size() > 0)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
        SCASSERT(paper != nullptr)
        if (paper->rect().height()>= paper->rect().width())
        {
            printer->setOrientation(QPrinter::Portrait);
        }
        else
        {
            printer->setOrientation(QPrinter::Landscape);
        }
    }

    if (not isTiled && papers.size() > 0)
    {
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
        SCASSERT(paper != nullptr)
        printer->setPaperSize ( QSizeF(qApp->fromPixel(paper->rect().width(), Unit::Mm),
                                       qApp->fromPixel(paper->rect().height(), Unit::Mm)), QPrinter::Millimeter );
    }

    printer->setDocName(FileName());
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
        QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(0));
        SCASSERT(paper != nullptr)
        for (int i=1; i < papers.size(); ++i)
        {
            QGraphicsRectItem *p = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
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
    SCASSERT(layout != nullptr);
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msgBox.exec();
    return msgBox.result();
}
