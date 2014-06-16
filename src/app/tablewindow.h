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
#include "widgets/vitem.h"

namespace Ui
{
    class TableWindow;
}

/**
 * @brief TableWindow class layout window.
 */
class TableWindow : public QMainWindow
{
    Q_OBJECT
public:
    /** @brief numberDetal show count details, what need placed. */
    QLabel*               numberDetal;

    /** @brief colission show if exist colissions. */
    QLabel*               colission;

    explicit TableWindow(QWidget *parent = nullptr);
    ~TableWindow();
public slots:

    void                  ModelChosen(QVector<VItem*> listDetails, const QString &fileName, const QString &description);

    void                  StopTable();

    void                  saveScene();

    void                  GetNextDetail();

    void                  itemChect(bool flag);

    void                  itemOut(int number, bool flag);

    void                  itemColliding(QList<QGraphicsItem *> list, int number);

    void                  AddLength();

    void                  RemoveLength();
signals:
    /**
     * @brief closed emit if window is closing.
     */
    void                  closed();
    /**
     * @brief LengthChanged emit if changing length of paper sheet.
     */
    void                  LengthChanged();
protected:

    void                  closeEvent(QCloseEvent *event);

    void                  moveToCenter();

    void                  showEvent ( QShowEvent * event );

    void                  keyPressEvent ( QKeyEvent * event );
private:
    Q_DISABLE_COPY(TableWindow)
    /** @brief ui keeps information about user interface */
    Ui::TableWindow*      ui;

    /** @brief listDetails list of details. */
    QVector<VItem*>       listDetails;

    /** @brief outItems true if we have details out paper sheet. */
    bool                  outItems;

    /** @brief collidingItems true if we have colission details. */
    bool                  collidingItems;

    /** @brief currentScene pointer to scene. */
    QGraphicsScene*       tableScene;

    /** @brief paper paper sheet. */
    QGraphicsRectItem*    paper;

    /** @brief shadowPaper paper sheet shadow. */
    QGraphicsRectItem*    shadowPaper;

    /** @brief listOutItems list state out each detail. */
    QBitArray*            listOutItems;

    /** @brief listCollidingItems list colissed details. */
    QList<QGraphicsItem*> listCollidingItems;

    /** @brief indexDetail index next detail in list what will be shown. */
    qint32                indexDetail;

    /** @brief sceneRect minimal size of a paper. */
    QRectF                sceneRect;

    /** @brief fileName keep name of pattern file. */
    QString               fileName;

    /** @brief description pattern description */
    QString               description;

    void                  checkNext();
    void                  AddPaper();
    void                  AddDetail();
    void                  SvgFile(const QString &name)const;
    void                  PngFile(const QString &name)const;
    void                  PdfFile(const QString &name)const;
    void                  EpsFile(const QString &name)const;
    void                  PsFile(const QString &name)const;
    void                  PdfToPs(const QStringList &params)const;
};

#endif // TABLEWINDOW_H
