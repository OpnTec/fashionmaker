/************************************************************************
 **
 **  @file   tablewindow.h
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
    /**
     * @brief numberDetal show count details, what need placed.
     */
    QLabel*               numberDetal;
    /**
     * @brief colission show if exist colissions.
     */
    QLabel*               colission;
    /**
     * @brief TableWindow constructor.
     * @param parent parent widget.
     */
                 explicit TableWindow(QWidget *parent = 0);
    ~TableWindow();
public slots:
    /**
     * @brief ModelChosen show window when user want create new layout.
     * @param listDetails list of details.
     */
    void                  ModelChosen(QVector<VItem*> listDetails, const QString &fileName);
    /**
     * @brief StopTable stop creation layout.
     */
    void                  StopTable();
    /**
     * @brief saveScene save created layout.
     */
    void                  saveScene();
    /**
     * @brief GetNextDetail put next detail on table.
     */
    void                  GetNextDetail();
    /**
     * @brief itemChect turn off rotation button if don't selected detail.
     * @param flag true - enable button.
     */
    void                  itemChect(bool flag);
    /**
     * @brief itemOut handled if detail moved out paper sheet.
     * @param number Number detail in list.
     * @param flag set state of detail. True if detail moved out paper sheet.
     */
    void                  itemOut(int number, bool flag);
    /**
     * @brief itemColliding handled if we have colission details.
     * @param list list of colission details.
     * @param number 0 - include to list of colission dcetails, 1 - exclude from list.
     */
    void                  itemColliding(QList<QGraphicsItem *> list, int number);
    /**
     * @brief AddLength Add length paper sheet.Збільшує довжину листа на певне значення за один раз.
     */
    void                  AddLength();
    /**
     * @brief RemoveLength reduce the length of paper sheet. You can reduce to the minimal value only.
     */
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
    /**
     * @brief closeEvent handle after close window.
     * @param event close event.
     */
    void                  closeEvent(QCloseEvent *event);
    /**
     * @brief moveToCenter move screen to the center of window.
     */
    void                  moveToCenter();
    /**
     * @brief showEvent handle after show window.
     * @param event show event.
     */
    void                  showEvent ( QShowEvent * event );
    /**
     * @brief keyPressEvent handle key press events.
     * @param event key event.
     */
    void                  keyPressEvent ( QKeyEvent * event );
private:
    Q_DISABLE_COPY(TableWindow)
    /**
     * @brief ui keeps information about user interface Змінна для доступу до об'єктів вікна.
     */
    Ui::TableWindow*      ui;
    /**
     * @brief listDetails list of details.
     */
    QVector<VItem*>       listDetails;
    /**
     * @brief outItems true if we have details out paper sheet.
     */
    bool                  outItems;
    /**
     * @brief collidingItems true if we have colission details.
     */
    bool                  collidingItems;
    /**
     * @brief currentScene pointer to scene.
     */
    QGraphicsScene*       tableScene;
    /**
     * @brief paper paper sheet.
     */
    QGraphicsRectItem*    paper;
    /**
     * @brief shadowPaper paper sheet shadow.
     */
    QGraphicsRectItem*    shadowPaper;
    /**
     * @brief checkNext disable next detail button if exist colission or out details.
     */
    void                  checkNext();
    /**
     * @brief listOutItems list state out each detail.
     */
    QBitArray*            listOutItems;
    /**
     * @brief listCollidingItems list colissed details.
     */
    QList<QGraphicsItem*> listCollidingItems;
    /**
     * @brief AddPaper add to the scene paper and shadow.
     */
    void                  AddPaper();
    /**
     * @brief AddDetail show on scene next detail.
     */
    void                  AddDetail();
    /**
     * @brief indexDetail index next detail in list what will be shown.
     */
    qint32                indexDetail;
    /**
     * @brief sceneRect minimal size of a paper.
     */
    QRectF                sceneRect;
    /**
     * @brief fileName keep name of pattern file.
     */
    QString               fileName;
    /**
     * @brief SvgFile save layout to svg file.
     * @param name name layout file.
     */
    void SvgFile(const QString &name)const;
    /**
     * @brief PngFile save layout to png file.
     * @param name name layout file.
     */
    void PngFile(const QString &name)const;
    /**
     * @brief PsFile save layout to ps file.
     * @param name name layout file.
     */
    void PdfFile(const QString &name)const;
};

#endif // TABLEWINDOW_H
