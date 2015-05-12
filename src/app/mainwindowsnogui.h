/************************************************************************
 **
 **  @file   mainwindowsnogui.h
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

#ifndef MAINWINDOWSNOGUI_H
#define MAINWINDOWSNOGUI_H

#include <QMainWindow>

#include "../geometry/vdetail.h"
#include "../libs/vlayout/vlayoutdetail.h"

class QGraphicsScene;

class MainWindowsNoGUI : public QMainWindow
{
    Q_OBJECT
public:
    MainWindowsNoGUI(QWidget *parent = nullptr);
    virtual ~MainWindowsNoGUI();

public slots:
    void ToolLayoutSettings(bool checked);

protected:
    QVector<VLayoutDetail> listDetails;

    /** @brief currentScene pointer to current scene. */
    QGraphicsScene *currentScene;

    QGraphicsScene *tempSceneLayout;

    /** @brief pattern container with data (points, arcs, splines, spline paths, variables) */
    VContainer         *pattern;

    QList<QGraphicsItem *> papers;
    QList<QGraphicsItem *> shadows;
    QList<QGraphicsScene *> scenes;
    QList<QList<QGraphicsItem *> > details;

    void PrepareDetailsForLayout(const QHash<quint32, VDetail> *details);

    void InitTempLayoutScene();
    virtual void ClearLayout()=0;
    virtual void PrepareSceneList()=0;
    QIcon ScenePreview(int i) const;

private:
    Q_DISABLE_COPY(MainWindowsNoGUI)

    void CreateShadows();
    void CreateScenes();

};

#endif // MAINWINDOWSNOGUI_H
