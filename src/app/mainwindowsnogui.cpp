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

#include <QGraphicsScene>

//---------------------------------------------------------------------------------------------------------------------
MainWindowsNoGUI::MainWindowsNoGUI(QWidget *parent)
    : QMainWindow(parent), listDetails(QVector<VLayoutDetail>()), currentScene(nullptr), tempSceneLayout(nullptr),
      pattern(new VContainer())
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
    tempSceneLayout = new VMainGraphicsScene();
    tempSceneLayout->setBackgroundBrush( QBrush(QColor(Qt::gray), Qt::SolidPattern) );
}
