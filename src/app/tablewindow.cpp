/************************************************************************
 **
 **  @file   tablewindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "widgets/vtablegraphicsview.h"
#include "core/vapplication.h"
#include "core/vsettings.h"
#include "../../libs/vobj/vobjpaintdevice.h"
#include "../dialogs/app/dialoglayoutsettings.h"
#include "../../libs/vlayout/vlayoutgenerator.h"
#include "../dialogs/app/dialoglayoutprogress.h"
#include "../dialogs/app/dialogsavelayout.h"
#include "../../libs/vlayout/vposter.h"

#include <QtSvg>
#include <QPrinter>
#include <QGraphicsScene>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QtPrintSupport>
#include <QtCore/qmath.h>

#ifdef Q_OS_WIN
#   define PDFTOPS "pdftops.exe"
#else
#   define PDFTOPS "pdftops"
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TableWindow constructor.
 * @param parent parent widget.
 */
TableWindow::TableWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::TableWindow),
    listDetails(QVector<VLayoutDetail>()), papers(QList<QGraphicsItem *>()), shadows(QList<QGraphicsItem *>()),
    scenes(QList<QGraphicsScene *>()), details(QList<QList<QGraphicsItem *> >()), fileName(QString()),
    description(QString()), tempScene(nullptr)
{
    ui->setupUi(this);

    qApp->getSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    tempScene = new QGraphicsScene(QRectF(0, 0, qApp->toPixel(823, Unit::Mm), qApp->toPixel(1171, Unit::Mm)));
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );
    tempScene->setBackgroundBrush( brush );

    ui->view->setScene(tempScene);
    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->horizontalLayout->addWidget(ui->view);
    connect(ui->actionZoomIn, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomOut);
    connect(ui->actionStop, &QAction::triggered, this, &TableWindow::StopTable);
    connect(ui->actionSave, &QAction::triggered, this, &TableWindow::SaveLayout);
    connect(ui->actionLayout, &QAction::triggered, this, &TableWindow::Layout);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &TableWindow::ShowPaper);
    connect(ui->actionPrint_pre_view, &QAction::triggered, this, &TableWindow::PrintPreview);
    connect(ui->action_Print, &QAction::triggered, this, &TableWindow::LayoutPrint);
    connect(ui->actionSave_to_p_df, &QAction::triggered, this, &TableWindow::PrintToPdf);

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
TableWindow::~TableWindow()
{
    ClearLayout();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ModelChosen show window when user want create new layout.
 * @param listDetails list of details.
 * @param description pattern description.
 */
/*
 * Get details for creation layout.
 */
void TableWindow::ModelChosen(QVector<VLayoutDetail> listDetails, const QString &fileName, const QString &description)
{
    this->description = description;

    QString file;
    if (fileName.isEmpty())
    {
        file = tr("untitled");
    }
    else
    {
        file = fileName;
    }
    QFileInfo fi( file );
    this->fileName = fi.baseName();

    this->listDetails = listDetails;
    show();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle after close window.
 * @param event close event.
 */
void TableWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    StopTable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief moveToCenter move screen to the center of window.
 */
void TableWindow::moveToCenter()
{
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle after show window.
 * @param event show event.
 */
void TableWindow::showEvent ( QShowEvent * event )
{
    QMainWindow::showEvent(event);
    moveToCenter();
    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief StopTable stop creation layout.
 */
void TableWindow::StopTable()
{
    hide();
    ClearLayout();
    emit closed();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief saveScene save created layout.
 */
void TableWindow::SaveLayout()
{
    QMap<QString, QString> extByMessage = InitFormates();
    DialogSaveLayout dialog(extByMessage, scenes.size(), fileName, this);

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
            QStringList suffix = QStringList() << "svg" << "png" << "pdf" << "eps" << "ps" << "obj";
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
void TableWindow::ShowPaper(int index)
{
    if (index < 0 || index > scenes.size())
    {
        ui->view->setScene(tempScene);
        EnableActions(false);
    }
    else
    {
        ui->view->setScene(scenes.at(index));
    }

    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::PrintPreview()
{
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
    printer.setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());
    printer.setDocName(fileName);

    // display print preview dialog
    QPrintPreviewDialog  preview(&printer);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TableWindow::Print);
    preview.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::Print(QPrinter *printer)
{
    if (printer == nullptr)
    {
        return;
    }

    const QVector<QImage> images = AllSheets();

    VPoster posterazor(printer);
    QVector<QImage> poster;
    for (int i=0; i < images.size(); i++)
    {
        poster += posterazor.Generate(images.at(i), i+1, images.size());
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
void TableWindow::LayoutPrint()
{
    // display print dialog and if accepted print
    QPrinter printer(QPrinter::HighResolution);
    printer.setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());
    printer.setDocName(fileName);
    QPrintDialog dialog( &printer, this );
    // If only user couldn't change page margins we could use method setMinMax();
    dialog.setOption(QPrintDialog::PrintCurrentPage, false);
    if ( dialog.exec() == QDialog::Accepted )
    {
        printer.setResolution(static_cast<int>(VApplication::PrintDPI));
        Print( &printer );
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::PrintToPdf()
{
    QPrinter printer;
    printer.setCreator(qApp->applicationDisplayName()+" "+qApp->applicationVersion());
    printer.setDocName(fileName);

    QPageSetupDialog dialog(&printer, this);
    if ( dialog.exec() == QDialog::Accepted )
    {
        printer.setOutputFormat(QPrinter::PdfFormat);
        const QString fileName = QFileDialog::getSaveFileName(this, tr("Print to pdf"),
                                                              QDir::homePath()+"/"+this->fileName+".pdf",
                                                              tr("PDF file (*.pdf)"));
        if (not fileName.isEmpty())
        {
            if(QFileInfo(fileName).suffix().isEmpty())
            {
                fileName.append(".pdf");
            }

            printer.setOutputFileName(fileName);
            printer.setResolution(static_cast<int>(VApplication::PrintDPI));
            Print( &printer );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::Layout()
{
    DialogLayoutSettings layout(this);
    if (layout.exec() == QDialog::Rejected)
    {
        return;
    }

    VLayoutGenerator lGenerator(this);
    lGenerator.SetDetails(listDetails);
    lGenerator.SetLayoutWidth(layout.GetLayoutWidth());
    lGenerator.SetCaseType(layout.GetGroup());
    lGenerator.SetPaperHeight(layout.GetPaperHeight());
    lGenerator.SetPaperWidth(layout.GetPaperWidth());
    lGenerator.SetShift(layout.GetShift());
    lGenerator.SetRotate(layout.GetRotate());
    lGenerator.SetRotationIncrease(layout.GetIncrease());

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
            ClearLayout();
            papers = lGenerator.GetPapersItems();// Blank sheets
            details = lGenerator.GetAllDetails();// All details
            CreateShadows();
            CreateScenes();
            PrepareSceneList();
            break;
        case LayoutErrors::ProcessStoped:
            break;
        case LayoutErrors::PrepareLayoutError:
        case LayoutErrors::PaperSizeError:
        case LayoutErrors::EmptyPaperError:
            ClearLayout();
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SvgFile save layout to svg file.
 * @param name name layout file.
 */
void TableWindow::SvgFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        QSvgGenerator generator;
        generator.setFileName(name);
        generator.setSize(paper->rect().size().toSize());
        generator.setViewBox(paper->rect());
        generator.setTitle("Valentina. Pattern layout");
        generator.setDescription(description);
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
void TableWindow::PngFile(const QString &name, int i) const
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
void TableWindow::PdfFile(const QString &name, int i) const
{
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(name);
        const QRectF r = paper->rect();
        printer.setResolution(static_cast<int>(qApp->PrintDPI));
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
void TableWindow::EpsFile(const QString &name, int i) const
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
void TableWindow::PsFile(const QString &name, int i) const
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
void TableWindow::PdfToPs(const QStringList &params) const
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
void TableWindow::ObjFile(const QString &name, int i) const
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
QVector<QImage> TableWindow::AllSheets()
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
void TableWindow::ClearLayout()
{
    qDeleteAll (scenes);
    scenes.clear();
    shadows.clear();
    papers.clear();
    ui->listWidget->clear();
    EnableActions(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::CreateShadows()
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
void TableWindow::CreateScenes()
{
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );

    for (int i=0; i<papers.size(); ++i)
    {
        QGraphicsScene *scene = new QGraphicsScene();
        scene->setBackgroundBrush(brush);
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
void TableWindow::PrepareSceneList()
{
    for (int i=1; i<=scenes.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(ScenePreview(i-1), QString::number(i));
        ui->listWidget->addItem(item);
    }

    if (scenes.isEmpty() == false)
    {
        ui->listWidget->setCurrentRow(0);
        EnableActions(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QIcon TableWindow::ScenePreview(int i) const
{
    QImage image;
    QGraphicsRectItem *paper = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
    if (paper)
    {
        const QRectF r = paper->rect();
        // Create the image with the exact size of the shrunk scene
        image = QImage(QSize(static_cast<qint32>(r.width()), static_cast<qint32>(r.height())), QImage::Format_RGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.setBrush ( QBrush ( Qt::NoBrush ) );
        scenes.at(i)->render(&painter);
        image.scaled(101, 146, Qt::KeepAspectRatio);
    }
    else
    {
        image = QImage(QSize(101, 146), QImage::Format_RGB32);
        image.fill(Qt::white);
    }
    return QIcon(QBitmap::fromImage(image));
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> TableWindow::InitFormates() const
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
void TableWindow::EnableActions(bool enable)
{
    ui->actionSave->setEnabled(enable);
    ui->actionSave_to_p_df->setEnabled(enable);
    ui->actionPrint_pre_view->setEnabled(enable);
    ui->action_Print->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::ToolBarStyle(QToolBar *bar)
{
    if (qApp->getSettings()->GetToolBarStyle())
    {
        bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else
    {
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::ReadSettings()
{
    // Text under tool buton icon
    ToolBarStyles();
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::ToolBarStyles()
{
    ToolBarStyle(ui->toolBar);
    ToolBarStyle(ui->toolBar_2);
}
