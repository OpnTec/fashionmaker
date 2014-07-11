/************************************************************************
 **
 **  @file   dialoghistory.h
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

#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#include "../tools/dialogtool.h"

class VPattern;
class VToolRecord;

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
    DialogHistory(VContainer *data, VPattern *doc, QWidget *parent = nullptr);
    virtual ~DialogHistory();
public slots:
    virtual void      DialogAccepted();
    /** TODO ISSUE 79 : create real function
    * @brief DialogApply apply data and emit signal about applied dialog.
    */
    virtual void      DialogApply(){}
    void              cellClicked(int row, int column);
    void              ChangedCursor(quint32 id);
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
    virtual void      closeEvent ( QCloseEvent * event );
private:
    Q_DISABLE_COPY(DialogHistory)

    /** @brief ui keeps information about user interface */
    Ui::DialogHistory *ui;

    /** @brief doc dom document container */
    VPattern          *doc;

    /** @brief cursorRow save number of row where is cursor */
    qint32            cursorRow;

    /** @brief cursorToolRecordRow save number of row selected record */
    qint32            cursorToolRecordRow;

    void              FillTable();
    QString           Record(const VToolRecord &tool);
    void              InitialTable();
    void              ShowPoint();
    QString           PointName(quint32 pointId);
    quint32           AttrUInt(const QDomElement &domElement, const QString &name);
};

#endif // DIALOGHISTORY_H
