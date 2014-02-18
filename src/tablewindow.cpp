/************************************************************************
 **
 **  @file   tablewindow.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
#include "options.h"

TableWindow::TableWindow(QWidget *parent)
    :QMainWindow(parent), numberDetal(0), colission(0), ui(new Ui::TableWindow),
    listDetails(QVector<VItem*>()), outItems(false), collidingItems(false), tableScene(0),
    paper(0), shadowPaper(0), listOutItems(0), listCollidingItems(QList<QGraphicsItem*>()),
    indexDetail(0), sceneRect(QRectF()), fileName(QString())
{
    ui->setupUi(this);
    numberDetal = new QLabel(tr("0 details left."), this);
    colission = new QLabel(tr("Collisions not found."), this);
    ui->statusBar->addWidget(numberDetal);
    ui->statusBar->addWidget(colission);
    outItems = collidingItems = false;
    //sceneRect = QRectF(0, 0, toPixel(203), toPixel(287));
    sceneRect = QRectF(0, 0, toPixel(823), toPixel(1171));
    tableScene = new QGraphicsScene(sceneRect);
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );
    tableScene->setBackgroundBrush( brush );
    VTableGraphicsView* view = new VTableGraphicsView(tableScene);
    view->fitInView(view->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->horizontalLayout->addWidget(view);
    connect(ui->actionTurn, &QAction::triggered, view, &VTableGraphicsView::rotateItems);
    connect(ui->actionMirror, &QAction::triggered, view, &VTableGraphicsView::MirrorItem);
    connect(ui->actionZoomIn, &QAction::triggered, view, &VTableGraphicsView::ZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, view, &VTableGraphicsView::ZoomOut);
    connect(ui->actionStop, &QAction::triggered, this, &TableWindow::StopTable);
    connect(ui->actionSave, &QAction::triggered, this, &TableWindow::saveScene);
    connect(ui->actionNext, &QAction::triggered, this, &TableWindow::GetNextDetail);
    connect(ui->actionAdd, &QAction::triggered, this, &TableWindow::AddLength);
    connect(ui->actionRemove, &QAction::triggered, this, &TableWindow::RemoveLength);
    connect(view, &VTableGraphicsView::itemChect, this, &TableWindow::itemChect);
}

TableWindow::~TableWindow()
{
    delete tableScene;
    delete ui;
}

void TableWindow::AddPaper()
{
    qreal x1, y1, x2, y2;
    sceneRect.getCoords(&x1, &y1, &x2, &y2);
    shadowPaper = new QGraphicsRectItem(QRectF(x1+4, y1+4, x2+4, y2+4));
    shadowPaper->setBrush(QBrush(Qt::black));
    tableScene->addItem(shadowPaper);
    paper = new QGraphicsRectItem(QRectF(x1, y1, x2, y2));
    paper->setPen(QPen(Qt::black, widthMainLine));
    paper->setBrush(QBrush(Qt::white));
    tableScene->addItem(paper);
    qDebug()<<paper->rect().size().toSize();
}

void TableWindow::AddDetail()
{
    if (indexDetail<listDetails.count())
    {
        tableScene->clearSelection();
        VItem* Detail = listDetails[indexDetail];
        QObject::connect(Detail, SIGNAL(itemOut(int, bool)), this, SLOT(itemOut(int, bool)));
        QObject::connect(Detail, SIGNAL(itemColliding(QList<QGraphicsItem*>, int)), this,
                         SLOT(itemColliding(QList<QGraphicsItem*>, int)));
        QObject::connect(this, SIGNAL(LengthChanged()), Detail, SLOT(LengthChanged()));
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

/*
 * Get details for creation layout.
 */
void TableWindow::ModelChosen(QVector<VItem*> listDetails, const QString &fileName)
{
    this->fileName = fileName;
    this->fileName.remove(this->fileName.size()-4, 4);
    this->listDetails = listDetails;
    listOutItems = new QBitArray(this->listDetails.count());
    AddPaper();
    indexDetail = 0;
    AddDetail();
    show();
}

void TableWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    StopTable();
}

void TableWindow::moveToCenter()
{
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

void TableWindow::showEvent ( QShowEvent * event )
{
    QMainWindow::showEvent(event);
    moveToCenter();
}

void TableWindow::StopTable()
{
    hide();
    tableScene->clear();
    delete listOutItems;
    listDetails.clear();
    //sceneRect = QRectF(0, 0, 230*resol/25.9, 327*resol/25.9);
    sceneRect = QRectF(0, 0, toPixel(823), toPixel(1171));
    emit closed();
}

void TableWindow::saveScene()
{
    QMap<QString, QString> extByMessage;
    extByMessage[ tr("Svg files (*.svg)") ] = ".svg";
    extByMessage[ tr("PDF files (*.pdf)") ] = ".pdf";
    extByMessage[ tr("Images (*.png)") ] = ".png";
    extByMessage[ tr("PS files (*.ps)") ] = ".ps";
    extByMessage[ tr("EPS files (*.eps)") ] = ".eps";

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
    QString dir = QDir::homePath()+fileName;
    QString name = QFileDialog::getSaveFileName(this, tr("Save layout"), dir, saveMessage, &sf);

    if (name.isEmpty())
    {
        return;
    }

    // what if the users did not specify a suffix...?
    QFileInfo f( name );
    if (f.suffix().isEmpty() && f.suffix() != "svg" && f.suffix() != "png" && f.suffix() != "pdf"
        && f.suffix() != "eps" && f.suffix() != "ps")
    {
        name += extByMessage[sf];
    }

    QBrush *brush = new QBrush();
    brush->setColor( QColor( Qt::white ) );
    tableScene->setBackgroundBrush( *brush );
    tableScene->clearSelection(); // Selections would also render to the file, so need delete them
    shadowPaper->setVisible(false);
    QFileInfo fi( name );
    QStringList suffix;
    suffix << "svg" << "png" << "pdf" << "eps" << "ps";
    switch (suffix.indexOf(fi.suffix())) {
    case 0:
        paper->setVisible(false);
        SvgFile(name);
        paper->setVisible(true);
        break;
    case 1:
        paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
        PngFile(name);
        paper->setPen(QPen(Qt::black, widthMainLine));
        break;
    case 2:
        paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
        PdfFile(name);
        paper->setPen(QPen(Qt::black, widthMainLine));
        break;
    case 3:
        paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
        EpsFile(name);
        paper->setPen(QPen(Qt::black, widthMainLine));
        break;
    case 4:
        paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
        PsFile(name);
        paper->setPen(QPen(Qt::black, widthMainLine));
        break;
    default:
        qWarning() << "Bad file suffix in TableWindow::saveScene().";
        break;
    }
    brush->setColor( QColor( Qt::gray ) );
    brush->setStyle( Qt::SolidPattern );
    tableScene->setBackgroundBrush( *brush );
    shadowPaper->setVisible(true);
    delete brush;
}

void TableWindow::itemChect(bool flag)
{
    ui->actionTurn->setDisabled(flag);
    ui->actionMirror->setDisabled(flag);
}

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
                            Q_CHECK_PTR(bitem);
                            bitem->setPen(QPen(Qt::black, widthMainLine));
                            listCollidingItems.removeAt(i);
                        }
                    }
                }
                else if (listCollidingItems.size()==1)
                {
                    VItem * bitem = qgraphicsitem_cast<VItem *> ( listCollidingItems.at(0) );
                    Q_CHECK_PTR(bitem);
                    bitem->setPen(QPen(Qt::black, widthMainLine));
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

void TableWindow::GetNextDetail()
{
    AddDetail();
}

void TableWindow::AddLength()
{
    QRectF rect = tableScene->sceneRect();
    rect.setHeight(rect.height()+toPixel(279));
    tableScene->setSceneRect(rect);
    rect = shadowPaper->rect();
    rect.setHeight(rect.height()+toPixel(279));
    shadowPaper->setRect(rect);
    rect = paper->rect();
    rect.setHeight(rect.height()+toPixel(279));
    paper->setRect(rect);
    ui->actionRemove->setEnabled(true);
    emit LengthChanged();
}

void TableWindow::RemoveLength()
{
    if (sceneRect.height() <= tableScene->sceneRect().height() - 100)
    {
        QRectF rect = tableScene->sceneRect();
        rect.setHeight(rect.height()-toPixel(279));
        tableScene->setSceneRect(rect);
        rect = shadowPaper->rect();
        rect.setHeight(rect.height()-toPixel(279));
        shadowPaper->setRect(rect);
        rect = paper->rect();
        rect.setHeight(rect.height()-toPixel(279));
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


void TableWindow::SvgFile(const QString &name) const
{
    QSvgGenerator generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                               "Example provided with Qt."));
    generator.setResolution(PrintDPI);
    qDebug()<<"resolution is" << generator.resolution();
    QPainter painter;
    painter.begin(&generator);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    painter.end();
}

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
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    image.save(name);
}

void TableWindow::PdfFile(const QString &name) const
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(name);
    QRectF r = paper->rect();
    qreal x=0, y=0, w=0, h=0;
    r.getRect(&x, &y, &w, &h);// Re-shrink the scene to it's bounding contents
    printer.setResolution(PrintDPI);
    printer.setPaperSize ( QSizeF(toMM(w), toMM(h)), QPrinter::Millimeter );
    QPainter painter;
    if (painter.begin( &printer ) == false)
    { // failed to open file
        qCritical("Can't open printer %s", qPrintable(name));
        return;
    }
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    tableScene->render(&painter);
    painter.end();
}

void TableWindow::EpsFile(const QString &name) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName());

        QStringList params;
        params << "-eps" << tmp.fileName() << name;

        PdfToPs(name, params);
    } 
}

void TableWindow::PsFile(const QString &name) const
{
    QTemporaryFile tmp;
    if (tmp.open())
    {
        PdfFile(tmp.fileName());

        QStringList params;
        params << tmp.fileName() << name;

        PdfToPs(name, params);
    }
}

void TableWindow::PdfToPs(const QString &name, const QStringList &params) const
{
    QProcess proc;
    QString program;

#ifdef Q_OS_WIN32
    program = "pdftops.exe";
#else
    program = "pdftops";
#endif

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    proc.start(program, params);
    proc.waitForFinished(15000);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    qDebug() << proc.errorString();

    QFile f(name);
    if (!f.exists())
    {
        QMessageBox msgBox(QMessageBox::Critical, "Critical error!", "Creating file '"+name+"' failed!",
                    QMessageBox::Ok | QMessageBox::Default);
        msgBox.exec();
    }
}
