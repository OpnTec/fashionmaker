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
#include <QPrinter>

#include "../vpatterndb/vdetail.h"
#include "../vlayout/vlayoutdetail.h"
#include "xml/vpattern.h"
#include "dialogs/dialogsavelayout.h"
#include "../vlayout/vlayoutgenerator.h"
#include "../vwidgets/vabstractmainwindow.h"


class QGraphicsScene;
class QPrinter;
struct PosterData;

class MainWindowsNoGUI : public VAbstractMainWindow
{
    Q_OBJECT
public:
    explicit MainWindowsNoGUI(QWidget *parent = nullptr);
    virtual ~MainWindowsNoGUI() Q_DECL_OVERRIDE;

public slots:
    void ToolLayoutSettings(bool checked);
    void SaveAsTiledPDF();
    void PrintPreviewOrigin();
    void PrintPreviewTiled();
    void PrintOrigin();
    void PrintTiled();
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
    QAction *actionDockWidgetGroups;

    /** @brief fileName name current pattern file. */
    QString            curFile;

    bool isLayoutStale;
    bool ignorePrinterFields;
    QMarginsF margins;
    QSizeF paperSize;

    void PrepareDetailsForLayout(const QHash<quint32, VDetail> *details);
    void ExportLayout(const DialogSaveLayout &dialog);

    void InitTempLayoutScene();
    virtual void CleanLayout()=0;
    virtual void PrepareSceneList()=0;
    QIcon ScenePreview(int i) const;
    bool LayoutSettings(VLayoutGenerator& lGenerator);
    int ContinueIfLayoutStale();
    QString FileName() const;
private slots:
    void PrintPages (QPrinter *printer);
    void ErrorConsoleMode(const LayoutErrors &state);
private:
    Q_DISABLE_COPY(MainWindowsNoGUI)

    bool isTiled;
    bool isAutoCrop;
    bool isUnitePages;

    QString layoutPrinterName;

    void CreateShadows();
    void CreateScenes();

    void SvgFile(const QString &name, int i)const;
    void PngFile(const QString &name, int i)const;
    void PdfFile(const QString &name, int i)const;
    void EpsFile(const QString &name, int i)const;
    void PsFile(const QString &name, int i)const;
    void PdfToPs(const QStringList &params)const;
    void ObjFile(const QString &name, int i)const;
    void DxfFile(const QString &name, int i)const;

    void PreparePaper(int index) const;
    void RestorePaper(int index) const;

    void SaveLayoutAs();
    void PrintPreview();
    void LayoutPrint();

    enum class PrintType : char {PrintPDF, PrintPreview, PrintNative};

    void SetPrinterSettings(QPrinter *printer, const PrintType &printType);
    bool IsLayoutGrayscale() const;
    QPrinter::PaperSize FindTemplate(const QSizeF &size) const;

    bool isPagesUniform() const;
    bool IsPagesFit(const QSizeF &printPaper) const;
};

#endif // MAINWINDOWSNOGUI_H
