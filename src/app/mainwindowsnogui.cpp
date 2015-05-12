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
#include "../dialogs/app/dialoglayoutsettings.h"
#include "../../libs/vlayout/vlayoutgenerator.h"
#include "../dialogs/app/dialoglayoutprogress.h"
#include "../dialogs/app/dialogsavelayout.h"
#include "../../libs/vlayout/vposter.h"

#include <QGraphicsScene>
#include <QToolButton>

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::MainWindowsNoGUI(QWidget *parent)
    : QMainWindow(parent), listDetails(QVector<VLayoutDetail>()), currentScene(nullptr), tempSceneLayout(nullptr),
      pattern(new VContainer()), papers(QList<QGraphicsItem *>()), shadows(QList<QGraphicsItem *>()),
      scenes(QList<QGraphicsScene *>()), details(QList<QList<QGraphicsItem *> >())
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
        tButton->setChecked(false);
    }
    else
    {
        tButton->setChecked(true);
    }
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
