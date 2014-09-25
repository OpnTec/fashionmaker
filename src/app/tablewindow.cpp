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
 **  Copyright (C) 2013 Valentina project
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
#include <QtSvg>
#include <QPrinter>
#include "core/vapplication.h"

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
    :QMainWindow(parent), numberDetal(nullptr), colission(nullptr), ui(new Ui::TableWindow),
    listDetails(QVector<VItem*>()), outItems(false), collidingItems(false), tableScene(nullptr),
    paper(nullptr), shadowPaper(nullptr), listOutItems(nullptr), listCollidingItems(QList<QGraphicsItem*>()),
    indexDetail(0), sceneRect(QRectF()), fileName(QString()), description(QString())
{
    ui->setupUi(this);
    numberDetal = new QLabel(tr("0 details left."), this);
    colission = new QLabel(tr("Collisions not found."), this);
    ui->statusBar->addWidget(numberDetal);
    ui->statusBar->addWidget(colission);
    outItems = collidingItems = false;
    sceneRect = QRectF(0, 0, qApp->toPixel(823, Unit::Mm), qApp->toPixel(1171, Unit::Mm));
    tableScene = new QGraphicsScene(sceneRect);
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );
    tableScene->setBackgroundBrush( brush );

    ui->view->setScene(tableScene);
    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->horizontalLayout->addWidget(ui->view);
    connect(tableScene, &QGraphicsScene::selectionChanged, ui->view, &VTableGraphicsView::selectionChanged);
    connect(ui->actionTurn, &QAction::triggered, ui->view, &VTableGraphicsView::rotateItems);
    connect(ui->actionMirror, &QAction::triggered, ui->view, &VTableGraphicsView::MirrorItem);
    connect(ui->actionZoomIn, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomOut);
    connect(ui->actionStop, &QAction::triggered, this, &TableWindow::StopTable);
    connect(ui->actionSave, &QAction::triggered, this, &TableWindow::saveScene);
    connect(ui->actionNext, &QAction::triggered, this, &TableWindow::GetNextDetail);
    connect(ui->actionAdd, &QAction::triggered, this, &TableWindow::AddLength);
    connect(ui->actionRemove, &QAction::triggered, this, &TableWindow::RemoveLength);
    connect(ui->view, &VTableGraphicsView::itemChect, this, &TableWindow::itemChect);
}

//---------------------------------------------------------------------------------------------------------------------
TableWindow::~TableWindow()
{
    delete tableScene;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddPaper add to the scene paper and shadow.
 */
void TableWindow::AddPaper()
{
    qreal x1, y1, x2, y2;
    sceneRect.getCoords(&x1, &y1, &x2, &y2);
    shadowPaper = new QGraphicsRectItem(QRectF(x1+4, y1+4, x2+4, y2+4));
    shadowPaper->setBrush(QBrush(Qt::black));
    tableScene->addItem(shadowPaper);
    paper = new QGraphicsRectItem(QRectF(x1, y1, x2, y2));
    paper->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
    paper->setBrush(QBrush(Qt::white));
    tableScene->addItem(paper);
    qDebug()<<paper->rect().size().toSize();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddDetail show on scene next detail.
 */
void TableWindow::AddDetail()
{
    if (indexDetail<listDetails.count())
    {
        tableScene->clearSelection();
        VItem* Detail = listDetails[indexDetail];
        SCASSERT(Detail != nullptr);
        connect(Detail, &VItem::itemOut, this, &TableWindow::itemOut);
        connect(Detail, &VItem::itemColliding, this, &TableWindow::itemColliding);
        connect(this, &TableWindow::LengthChanged, Detail, &VItem::LengthChanged);
        Detail->setPen(QPen(Qt::black, 1));
        Detail->setBrush(QBrush(Qt::white));
        Detail->setPos(paper->boundingRect().center());
        Detail->setFlag(QGraphicsItem::ItemIsMovable, true);
        Detail->setFlag(QGraphicsItem::ItemIsSelectable, true);
        Detail->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        Detail->setPaper(paper);
        tableScene->addItem(Detail);
        Detail->setSelected(true);
        indexDetail++;
        if (indexDetail==listDetails.count())
        {
            ui->actionSave->setEnabled(true);
        }
    }
    numberDetal->setText(QString(tr("%1 details left.")).arg(listDetails.count()-indexDetail));
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
void TableWindow::ModelChosen(QVector<VItem*> listDetails, const QString &fileName, const QString &description)
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
    listOutItems = new QBitArray(this->listDetails.count());
    AddPaper();
    indexDetail = 0;
    AddDetail();
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
    tableScene->clear();
    delete listOutItems;
    listDetails.clear();
    sceneRect = QRectF(0, 0, qApp->toPixel(823, Unit::Mm), qApp->toPixel(1171, Unit::Mm));
    tableScene->setSceneRect(sceneRect);
    emit closed();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief saveScene save created layout.
 */
void TableWindow::saveScene()
{
    QMap<QString, QString> extByMessage;
    extByMessage[ tr("Svg files (*.svg)") ] = ".svg";
    extByMessage[ tr("PDF files (*.pdf)") ] = ".pdf";
    extByMessage[ tr("Images (*.png)") ] = ".png";

    QProcess proc;
    proc.start(PDFTOPS);
    if (proc.waitForFinished(15000))
    {
        extByMessage[ tr("PS files (*.ps)") ] = ".ps";
        extByMessage[ tr("EPS files (*.eps)") ] = ".eps";
    }
    else
    {
        qWarning()<<PDFTOPS<<"error"<<proc.error()<<proc.errorString();
    }

    QString saveMessage;
    QMapIterator<QString, QString> i(extByMessage);
    while (i.hasNext())
    {
        i.next();
        saveMessage += i.key();
        if (i.hasNext())
        {
            saveMessage += ";;";
        }
    }

    QString sf;
    // the save function
    QString dir = QDir::homePath()+"/"+fileName;
    QString name = QFileDialog::getSaveFileName(this, tr("Save layout"), dir, saveMessage, &sf);

    if (name.isEmpty())
    {
        return;
    }

    // what if the user did not specify a suffix...?
    QString suf = extByMessage.value(sf);
    suf.replace(".", "");
    QFileInfo f( name );
    if (f.suffix().isEmpty() || f.suffix() != suf)
    {
        name += extByMessage.value(sf);
    }

    QBrush *brush = new QBrush();
    brush->setColor( QColor( Qt::white ) );
    tableScene->setBackgroundBrush( *brush );
    tableScene->clearSelection(); // Selections would also render to the file, so need delete them
    shadowPaper->setVisible(false);
    paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
    QFileInfo fi( name );
    QStringList suffix{"svg", "png", "pdf", "eps", "ps"};
    switch (suffix.indexOf(fi.suffix()))
    {
        case 0: //svg
            paper->setVisible(false);
            SvgFile(name);
            paper->setVisible(true);
            break;
        case 1: //png
            PngFile(name);
            break;
        case 2: //pdf
            PdfFile(name);
            break;
        case 3: //eps
            EpsFile(name);
            break;
        case 4: //ps
            PsFile(name);
            break;
        default:
            qDebug() << "Can't recognize file suffix. File file "<<name<<Q_FUNC_INFO;
            break;
    }
    paper->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
    brush->setColor( QColor( Qt::gray ) );
    brush->setStyle( Qt::SolidPattern );
    tableScene->setBackgroundBrush( *brush );
    shadowPaper->setVisible(true);
    delete brush;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChect turn off rotation button if don't selected detail.
 * @param flag true - enable button.
 */
void TableWindow::itemChect(bool flag)
{
    ui->actionTurn->setDisabled(flag);
    ui->actionMirror->setDisabled(flag);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief checkNext disable next detail button if exist colission or out details.
 */
void TableWindow::checkNext()
{
    if (outItems == true && collidingItems == true)
    {
        colission->setText(tr("Collisions not found."));
        if (indexDetail==listDetails.count())
        {
            ui->actionSave->setEnabled(true);
            ui->actionNext->setDisabled(true);
        }
        else
        {
            ui->actionNext->setDisabled(false);
            ui->actionSave->setEnabled(false);
        }
    }
    else
    {
        colission->setText(tr("Collisions found."));
        ui->actionNext->setDisabled(true);
        ui->actionSave->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemOut handled if detail moved out paper sheet.
 * @param number Number detail in list.
 * @param flag set state of detail. True if detail moved out paper sheet.
 */
void TableWindow::itemOut(int number, bool flag)
{
    listOutItems->setBit(number, flag);
    for ( int i = 0; i < listOutItems->count(); ++i )
    {
        if (listOutItems->at(i)==true)
        {
            outItems=false;
            qDebug()<<"itemOut::outItems="<<outItems<<"&& collidingItems"<<collidingItems;
            checkNext();
            return;
        }
    }
    outItems=true;
    checkNext();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemColliding handled if we have colission details.
 * @param list list of colission details.
 * @param number 0 - include to list of colission dcetails, 1 - exclude from list.
 */
void TableWindow::itemColliding(QList<QGraphicsItem *> list, int number)
{
    //qDebug()<<"number="<<number;
    if (number==0)
    {
        if (listCollidingItems.isEmpty()==false)
        {
            if (listCollidingItems.contains(list.at(0))==true)
            {
                listCollidingItems.removeAt(listCollidingItems.indexOf(list.at(0)));
                if (listCollidingItems.size()>1)
                {
                    for ( int i = 0; i < listCollidingItems.count(); ++i )
                    {
                        QList<QGraphicsItem *> lis = listCollidingItems.at(i)->collidingItems();
                        if (lis.size()-2 <= 0)
                        {
                            VItem * bitem = qgraphicsitem_cast<VItem *> ( listCollidingItems.at(i) );
                            SCASSERT(bitem != nullptr);
                            bitem->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
                            listCollidingItems.removeAt(i);
                        }
                    }
                }
                else if (listCollidingItems.size()==1)
                {
                    VItem * bitem = qgraphicsitem_cast<VItem *> ( listCollidingItems.at(0) );
                    SCASSERT(bitem != nullptr);
                    bitem->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
                    listCollidingItems.clear();
                    collidingItems = true;
                }
            }
            else
            {
                collidingItems = true;
            }
        }
        else
        {
            collidingItems = true;
        }
    }
    else if (number==1)
    {
        if (list.contains(paper)==true)
        {
            list.removeAt(list.indexOf(paper));
        }
        if (list.contains(shadowPaper)==true)
        {
            list.removeAt(list.indexOf(shadowPaper));
        }
        for ( int i = 0; i < list.count(); ++i )
        {
            if (listCollidingItems.contains(list.at(i))==false)
            {
                listCollidingItems.append(list.at(i));
            }
        }
        collidingItems = false;
    }
    qDebug()<<"itemColliding::outItems="<<outItems<<"&& collidingItems"<<collidingItems;
    checkNext();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNextDetail put next detail on table.
 */
void TableWindow::GetNextDetail()
{
    AddDetail();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLength Add length paper sheet.Збільшує довжину листа на певне значення за один раз.
 */
void TableWindow::AddLength()
{
    QRectF rect = tableScene->sceneRect();
    rect.setHeight(rect.height()+qApp->toPixel(279, Unit::Mm));
    tableScene->setSceneRect(rect);
    rect = shadowPaper->rect();
    rect.setHeight(rect.height()+qApp->toPixel(279, Unit::Mm));
    shadowPaper->setRect(rect);
    rect = paper->rect();
    rect.setHeight(rect.height()+qApp->toPixel(279, Unit::Mm));
    paper->setRect(rect);
    ui->actionRemove->setEnabled(true);
    emit LengthChanged();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveLength reduce the length of paper sheet. You can reduce to the minimal value only.
 */
void TableWindow::RemoveLength()
{
    if (sceneRect.height() <= tableScene->sceneRect().height() - 100)
    {
        QRectF rect = tableScene->sceneRect();
        rect.setHeight(rect.height()-qApp->toPixel(279, Unit::Mm));
        tableScene->setSceneRect(rect);
        rect = shadowPaper->rect();
        rect.setHeight(rect.height()-qApp->toPixel(279, Unit::Mm));
        shadowPaper->setRect(rect);
        rect = paper->rect();
        rect.setHeight(rect.height()-qApp->toPixel(279, Unit::Mm));
        paper->setRect(rect);
        if (fabs(sceneRect.height() - tableScene->sceneRect().height()) < 0.01)
        {
            ui->actionRemove->setDisabled(true);
        }
        emit LengthChanged();
    }
    else
    {
        ui->actionRemove->setDisabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyPressEvent handle key press events.
 * @param event key event.
 */
void TableWindow::keyPressEvent ( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        if (ui->actionNext->isEnabled() == true )
        {
            AddDetail();
            qDebug()<<"Added detail.";
        }
    }
    QMainWindow::keyPressEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SvgFile save layout to svg file.
 * @param name name layout file.
 */
void TableWindow::SvgFile(const QString &name) const
{
    QSvgGenerator generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    generator.setViewBox(paper->rect());
    generator.setTitle("Valentina pattern");
    generator.setDescription(description);
    generator.setResolution(static_cast<int>(qApp->PrintDPI));
    QPainter painter;
    painter.begin(&generator);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PngFile save layout to png file.
 * @param name name layout file.
 */
void TableWindow::PngFile(const QString &name) const
{
    QRectF r = paper->rect();
    qreal x=0, y=0, w=0, h=0;
    r.getRect(&x, &y, &w, &h);// Re-shrink the scene to it's bounding contents
    // Create the image with the exact size of the shrunk scene
    QImage image(QSize(static_cast<qint32>(w), static_cast<qint32>(h)), QImage::Format_ARGB32);
    image.fill(Qt::transparent);                                              // Start all pixels transparent
    QPainter painter(&image);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    image.save(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfFile save layout to pdf file.
 * @param name name layout file.
 */
void TableWindow::PdfFile(const QString &name) const
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(name);
    QRectF r = paper->rect();
    qreal x=0, y=0, w=0, h=0;
    r.getRect(&x, &y, &w, &h);// Re-shrink the scene to it's bounding contents
    printer.setResolution(static_cast<int>(qApp->PrintDPI));
    printer.setPaperSize ( QSizeF(qApp->fromPixel(w, Unit::Mm), qApp->fromPixel(h, Unit::Mm)), QPrinter::Millimeter );
    QPainter painter;
    if (painter.begin( &printer ) == false)
    { // failed to open file
        qCritical("Can't open printer %s", qPrintable(name));
        return;
    }
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EpsFile save layout to eps file.
 * @param name name layout file.
 */
void TableWindow::EpsFile(const QString &name) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName());
        QStringList params{"-eps", tmp.fileName(), name};
        PdfToPs(params);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PsFile save layout to ps file.
 * @param name name layout file.
 */
void TableWindow::PsFile(const QString &name) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName());
        QStringList params{tmp.fileName(), name};
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
