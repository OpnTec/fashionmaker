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

#include "../libs/vpatterndb/vdetail.h"
#include "../libs/vlayout/vlayoutdetail.h"
#include "xml/vpattern.h"
#include "dialogs/dialogsavelayout.h"
#include "../libs/vlayout/vlayoutgenerator.h"


class QGraphicsScene;
class QPrinter;


class MainWindowsNoGUI : public QMainWindow
{
    Q_OBJECT
public:
    MainWindowsNoGUI(QWidget *parent = nullptr);
    virtual ~MainWindowsNoGUI() Q_DECL_OVERRIDE;

public slots:
    void ToolLayoutSettings(bool checked);
    void ExportLayoutAs();
    void SaveAsPDF();
    void SaveAsTiledPDF();
    void PrintPages (QPrinter *printer);
    void PrintPreviewOrigin();
    void PrintPreviewTiled();
    void PrintOrigin();
    void PrintTiled();
    void ErrorConsoleMode(const LayoutErrors &state);
protected:
    QVector<VLayoutDetail> listDetails;

    /** @brief currentScene pointer to current scene. */
    QGraphicsScene *currentScene;

    QGraphicsScene *tempSceneLayout;

    /** @brief pattern container with data (points, arcs, splines, spline paths, variables) */
    VContainer         *pattern;

    /** @brief doc dom document container */
    VPattern           *doc;

    QList<QGraphicsItem *> papers;
    QList<QGraphicsItem *> shadows;
    QList<QGraphicsScene *> scenes;
    QList<QList<QGraphicsItem *> > details;

    QAction *undoAction;
    QAction *redoAction;
    QAction *actionDockWidgetToolOptions;

    /** @brief fileName name current pattern file. */
    QString            curFile;

    bool isLayoutStale;

    void PrepareDetailsForLayout(const QHash<quint32, VDetail> *details);
    void ExportLayout(const DialogSaveLayout &dialog);

    void InitTempLayoutScene();
    virtual void CleanLayout()=0;
    virtual void PrepareSceneList()=0;
    QIcon ScenePreview(int i) const;
    void LayoutSettings(VLayoutGenerator& lGenerator);
private:
    Q_DISABLE_COPY(MainWindowsNoGUI)

    bool isTiled;

    void CreateShadows();
    void CreateScenes();


    void SvgFile(const QString &name, int i)const;
    void PngFile(const QString &name, int i)const;
    void PdfFile(const QString &name, int i)const;
    void EpsFile(const QString &name, int i)const;
    void PsFile(const QString &name, int i)const;
    void PdfToPs(const QStringList &params)const;
    void ObjFile(const QString &name, int i)const;

    QVector<QImage> AllSheets();

    void SaveLayoutAs();
    void PrintPreview();
    void LayoutPrint();

    void SetPrinterSettings(QPrinter *printer);

    bool isPagesUniform() const;
    QString FileName() const;

    int ContinueIfLayoutStale();

    void UnitePages();
    QList<QGraphicsItem *> MoveDetails(qreal length, const QList<QGraphicsItem *> &details);
    void UnitePapers(int j, QList<QGraphicsItem *> &nPapers, qreal width, qreal length);
    void UniteDetails(int j, QList<QList<QGraphicsItem *> > &nDetails, qreal length, int i);
};

#endif // MAINWINDOWSNOGUI_H
