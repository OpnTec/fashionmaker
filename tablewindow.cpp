/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "tablewindow.h"
#include "ui_tablewindow.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "widgets/vtablegraphicsview.h"
#include <QFileDialog>
#include "options.h"
#include <QtSvg/QtSvg>

TableWindow::TableWindow(QWidget *parent) :
    QMainWindow(parent), numberDetal(0), colission(0), ui(new Ui::TableWindow),
    listDetails(QVector<VItem*>()), outItems(false), collidingItems(false), currentScene(0),
    paper(0), shadowPaper(0), listOutItems(0), listCollidingItems(QList<QGraphicsItem*>()),
    indexDetail(0), sceneRect(QRectF()){
    ui->setupUi(this);
    numberDetal = new QLabel("Залишилось 0 деталей.", this);
    colission = new QLabel("Колізій не знайдено.", this);
    ui->statusBar->addWidget(numberDetal);
    ui->statusBar->addWidget(colission);
    outItems = collidingItems = false;
    //sceneRect = QRectF(0, 0, 203*PrintDPI/25.4, 287*PrintDPI/25.4);
    sceneRect = QRectF(0, 0, toPixel(823), toPixel(1171));
    currentScene = new QGraphicsScene(sceneRect);
    QBrush *brush = new QBrush();
    brush->setStyle( Qt::SolidPattern );
    brush->setColor( QColor( Qt::gray ) );
    currentScene->setBackgroundBrush( *brush );
    VTableGraphicsView* view = new VTableGraphicsView(currentScene);
    view->fitInView(view->scene()->sceneRect(),Qt::KeepAspectRatio);
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

TableWindow::~TableWindow(){
    delete ui;
}

void TableWindow::AddPaper(){
    qreal x1, y1, x2, y2;
    sceneRect.getCoords(&x1, &y1, &x2, &y2);
    shadowPaper = new QGraphicsRectItem(QRectF(x1+4,y1+4,x2+4, y2+4));
    shadowPaper->setBrush(QBrush(Qt::black));
    currentScene->addItem(shadowPaper);
    paper = new QGraphicsRectItem(QRectF(x1,y1,x2, y2));
    paper->setPen(QPen(Qt::black, toPixel(widthMainLine)));
    paper->setBrush(QBrush(Qt::white));
    currentScene->addItem(paper);
    qDebug()<<paper->rect().size().toSize();
}

void TableWindow::AddDetail(){
    if(indexDetail<listDetails.count()){
        currentScene->clearSelection();
        VItem* Detail = listDetails[indexDetail];
        QObject::connect(Detail, SIGNAL(itemOut(int,bool)), this, SLOT(itemOut(int,bool)));
        QObject::connect(Detail, SIGNAL(itemColliding(QList<QGraphicsItem*>,int)), this,
                         SLOT(itemColliding(QList<QGraphicsItem*>,int)));
        QObject::connect(this, SIGNAL(LengthChanged()), Detail, SLOT(LengthChanged()));
        Detail->setPen(QPen(Qt::black, toPixel(widthMainLine)));
        Detail->setBrush(QBrush(Qt::white));
        Detail->setPos(paper->boundingRect().center());
        Detail->setFlag(QGraphicsItem::ItemIsMovable, true);
        Detail->setFlag(QGraphicsItem::ItemIsSelectable, true);
        Detail->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        Detail->setParentItem(paper);
        Detail->setSelected(true);
        indexDetail++;
        if(indexDetail==listDetails.count()){
            ui->actionSave->setEnabled(true);
        }
    }
    numberDetal->setText(QString("Залишилось %1 деталей.").arg(listDetails.count()-indexDetail));
}

/*
 * Отримуємо деталі розрахованої моделі для подальшого укладання.
 */
void TableWindow::ModelChosen(QVector<VItem*> listDetails){
    this->listDetails = listDetails;
    listOutItems = new QBitArray(this->listDetails.count());
    AddPaper();
    indexDetail = 0;
    AddDetail();
    show();
}

void TableWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    StopTable();
}

void TableWindow::moveToCenter(){
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

void TableWindow::showEvent ( QShowEvent * event ){
    QMainWindow::showEvent(event);
    moveToCenter();
}

void TableWindow::StopTable(){
    hide();
    currentScene->clear();
    delete listOutItems;
    listDetails.clear();
    //sceneRect = QRectF(0, 0, 230*resol/25.9, 327*resol/25.9);
    sceneRect = QRectF(0, 0, toPixel(823), toPixel(1171));
    emit closed();
}

void TableWindow::saveScene(){
    QString name = QFileDialog::getSaveFileName(0, "Зберегти розкладку", "", "Images (*.png);;Svg files (*.svg)");
    if(name.isNull()){
        return;
    }

    QBrush *brush = new QBrush();
    brush->setColor( QColor( Qt::white ) );
    currentScene->setBackgroundBrush( *brush );
    currentScene->clearSelection();                                                  // Selections would also render to the file
    shadowPaper->setBrush(QBrush(Qt::white));
    shadowPaper->setPen(QPen(Qt::white, 0.1));
    paper->setPen(QPen(Qt::white, 0.1));
    paper->setBrush(QBrush(Qt::white));
    currentScene->setSceneRect(QRectF(10,10,590,590));
    currentScene->setSceneRect(currentScene->itemsBoundingRect());

    QFileInfo fi(name);
    if(fi.suffix() == "svg"){
        SvgFile(name);
    } else if(fi.suffix() == "png"){
        PngFile(name);
    }
//    if(name.indexOf(".svg",name.size()-4)<0){
//        name.append(".svg");
//    }

    brush->setColor( QColor( Qt::gray ) );
    brush->setStyle( Qt::SolidPattern );
    currentScene->setBackgroundBrush( *brush );
    paper->setPen(QPen(Qt::black, widthMainLine));
    shadowPaper->setBrush(QBrush(Qt::black));
}

void TableWindow::itemChect(bool flag){
    ui->actionTurn->setDisabled(flag);
    ui->actionMirror->setDisabled(flag);
}

void TableWindow::checkNext(){
    if(outItems == true && collidingItems == true){
        colission->setText("Колізій не знайдено.");
        if(indexDetail==listDetails.count()){
            ui->actionSave->setEnabled(true);
            ui->actionNext->setDisabled(true);
        } else {
            ui->actionNext->setDisabled(false);
            ui->actionSave->setEnabled(false);
        }
    } else {
        colission->setText("Знайдено колізії.");
        ui->actionNext->setDisabled(true);
        ui->actionSave->setEnabled(false);
    }
}

void TableWindow::itemOut(int number, bool flag){
    listOutItems->setBit(number,flag);
    for( int i = 0; i < listOutItems->count(); ++i ){
        if(listOutItems->at(i)==true){
            outItems=false;
            qDebug()<<"itemOut::outItems="<<outItems<<"&& collidingItems"<<collidingItems;
            checkNext();
            return;
        }
    }
    outItems=true;
    checkNext();
}

void TableWindow::itemColliding(QList<QGraphicsItem *> list, int number){
    //qDebug()<<"number="<<number;
    if(number==0){
        if(listCollidingItems.isEmpty()==false){
            if(listCollidingItems.contains(list.at(0))==true){
                listCollidingItems.removeAt(listCollidingItems.indexOf(list.at(0)));
                if(listCollidingItems.size()>1){
                    for( int i = 0; i < listCollidingItems.count(); ++i ){
                        QList<QGraphicsItem *> l = listCollidingItems.at(i)->collidingItems();
                        if(l.size()-2 <= 0){
                            VItem * bitem = qgraphicsitem_cast<VItem *> ( listCollidingItems.at(i) );
                            if (bitem == 0){
                                qDebug()<<"Не можу привести тип об'єкту";
                            } else {
                                bitem->setPen(QPen(Qt::black, toPixel(widthMainLine)));
                            }
                            listCollidingItems.removeAt(i);
                        }
                    }
                } else if(listCollidingItems.size()==1){
                    VItem * bitem = qgraphicsitem_cast<VItem *> ( listCollidingItems.at(0) );
                    if (bitem == 0){
                        qDebug()<<"Не можу привести тип об'єкту";
                    } else {
                        bitem->setPen(QPen(Qt::black, toPixel(widthMainLine)));
                    }
                    listCollidingItems.clear();
                    collidingItems = true;
                }
            } else {
                collidingItems = true;
            }
        } else {
            collidingItems = true;
        }
    } else if(number==1){
        if(list.contains(paper)==true){
            list.removeAt(list.indexOf(paper));
        }
        if(list.contains(shadowPaper)==true){
            list.removeAt(list.indexOf(shadowPaper));
        }
        for( int i = 0; i < list.count(); ++i ){
            if(listCollidingItems.contains(list.at(i))==false){
                listCollidingItems.append(list.at(i));
            }
        }
        collidingItems = false;
    }
    qDebug()<<"itemColliding::outItems="<<outItems<<"&& collidingItems"<<collidingItems;
    checkNext();
}

void TableWindow::GetNextDetail(){
    AddDetail();
}

void TableWindow::AddLength(){
    QRectF rect = currentScene->sceneRect();
    rect.setHeight(rect.height()+toPixel(279));
    currentScene->setSceneRect(rect);
    rect = shadowPaper->rect();
    rect.setHeight(rect.height()+toPixel(279));
    shadowPaper->setRect(rect);
    rect = paper->rect();
    rect.setHeight(rect.height()+toPixel(279));
    paper->setRect(rect);
    ui->actionRemove->setEnabled(true);
    emit LengthChanged();
}

void TableWindow::RemoveLength(){
    if(sceneRect.height()<=currentScene->sceneRect().height()-100){
        QRectF rect = currentScene->sceneRect();
        rect.setHeight(rect.height()-toPixel(279));
        currentScene->setSceneRect(rect);
        rect = shadowPaper->rect();
        rect.setHeight(rect.height()-toPixel(279));
        shadowPaper->setRect(rect);
        rect = paper->rect();
        rect.setHeight(rect.height()-toPixel(279));
        paper->setRect(rect);
        if(sceneRect.height()==currentScene->sceneRect().height()){
            ui->actionRemove->setDisabled(true);
        }
        emit LengthChanged();
    } else {
        ui->actionRemove->setDisabled(true);
    }
}

void TableWindow::keyPressEvent ( QKeyEvent * event ){
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ){
        if(ui->actionNext->isEnabled() == true ){
            AddDetail();
            qDebug()<<"Додали деталь.";
        }
    }
    QMainWindow::keyPressEvent ( event );
}


void TableWindow::SvgFile(const QString &name) const{
    QSvgGenerator generator;
    generator.setFileName(name);
    generator.setSize(paper->rect().size().toSize());
    //generator.setViewBox(QRect(0, 0, 200, 200));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                               "Example provided with Qt."));
    QPainter painter;
    painter.begin(&generator);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, widthMainLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    currentScene->render(&painter);
    painter.end();
}

void TableWindow::PngFile(const QString &name) const{
    QRectF r = paper->rect();
    qreal x=0, y=0, w=0, h=0;
    r.getRect(&x,&y,&w,&h);// Re-shrink the scene to it's bounding contents
    QImage image(QSize(static_cast<qint32>(w), static_cast<qint32>(h)), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);                                              // Start all pixels transparent
    QPainter painter(&image);
    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, toPixel(widthMainLine), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush ( QBrush ( Qt::NoBrush ) );
    currentScene->render(&painter);
    image.save(name);
}
