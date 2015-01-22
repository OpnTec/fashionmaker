/************************************************************************
 **
 **  @file   tablewindow.h
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

#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QLabel>
#include <QMainWindow>

#include "../../libs/vlayout/vlayoutdetail.h"
#include "../../libs/vlayout/vbank.h"

namespace Ui
{
    class TableWindow;
}

class QGraphicsScene;

/**
 * @brief TableWindow class layout window.
 */
class TableWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TableWindow(QWidget *parent = nullptr);
    ~TableWindow();

public slots:
    void                  ModelChosen(QVector<VLayoutDetail> listDetails, const QString &fileName,
                                      const QString &description);
    void                  Layout();
    void                  StopTable();
    void                  saveScene();
    void                  ShowPaper(int index);

signals:
    /** @brief closed emit if window is closing. */
    void                  closed();

protected:
    void                  closeEvent(QCloseEvent *event);
    void                  moveToCenter();
    void                  showEvent ( QShowEvent * event );

private:
    Q_DISABLE_COPY(TableWindow)
    /** @brief ui keeps information about user interface */
    Ui::TableWindow*      ui;

    /** @brief listDetails list of details. */
    QVector<VLayoutDetail> listDetails;

    QList<QGraphicsItem *> papers;
    QList<QGraphicsItem *> shadows;
    QList<QGraphicsScene *> scenes;

    /** @brief fileName keep name of pattern file. */
    QString               fileName;

    /** @brief description pattern description */
    QString               description;

    QGraphicsScene* tempScene;

    void                  SvgFile(const QString &name)const;
    void                  PngFile(const QString &name)const;
    void                  PdfFile(const QString &name)const;
    void                  EpsFile(const QString &name)const;
    void                  PsFile(const QString &name)const;
    void                  PdfToPs(const QStringList &params)const;
    void                  ObjFile(const QString &name)const;

    void ClearLayout();
    void CreateShadows();
    void CreateScenes();
    void PrepareSceneList();
    QIcon ScenePreview(int i) const;
};

#endif // TABLEWINDOW_H
