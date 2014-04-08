/************************************************************************
 **
 **  @file   dialoghistory.h
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

#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#include "../tools/dialogtool.h"
#include "../../xml/vpattern.h"

namespace Ui
{
    class DialogHistory;
}

/**
 * @brief The DialogHistory class show dialog history.
 */
class DialogHistory : public DialogTool
{
    Q_OBJECT
public:
                      /**
                       * @brief DialogHistory create dialog
                       * @param data container with data
                       * @param doc dom document container
                       * @param parent parent widget
                       */
                      DialogHistory(VContainer *data, VPattern *doc, QWidget *parent = nullptr);
    virtual           ~DialogHistory();
public slots:
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void      DialogAccepted();
    /**
     * @brief cellClicked changed history record
     * @param row number row in table
     * @param column number column in table
     */
    void              cellClicked(int row, int column);
    /**
     * @brief ChangedCursor changed cursor of input. Cursor show after what record we will insert new object
     * @param id id of object
     */
    void              ChangedCursor(quint32 id);
    /**
     * @brief UpdateHistory update history table
     */
    void              UpdateHistory();
signals:
    /**
     * @brief ShowHistoryTool signal change color of selected in records tool
     * @param id id of tool
     * @param color new color of tool
     * @param enable true enable selection, false disable selection
     */
    void              ShowHistoryTool(quint32 id, Qt::GlobalColor color, bool enable);
protected:
    /**
     * @brief closeEvent handle when windows is closing
     * @param event event
     */
    virtual void      closeEvent ( QCloseEvent * event );
private:
    Q_DISABLE_COPY(DialogHistory)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogHistory *ui;
    /**
     * @brief doc dom document container
     */
    VPattern          *doc;
    /**
     * @brief cursorRow save number of row where is cursor
     */
    qint32            cursorRow;
    /**
     * @brief cursorToolRecordRow save number of row selected record
     */
    qint32            cursorToolRecordRow;
    /**
     * @brief FillTable fill table
     */
    void              FillTable();
    /**
     * @brief Record return description for record
     * @param tool record data
     * @return description
     */
    QString           Record(const VToolRecord &tool);
    /**
     * @brief InitialTable set initial option of table
     */
    void              InitialTable();
    /**
     * @brief ShowPoint show selected point
     */
    void              ShowPoint();
};

#endif // DIALOGHISTORY_H
