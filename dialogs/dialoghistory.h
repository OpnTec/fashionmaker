/************************************************************************
 **
 **  @file dialoghistory.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#include "dialogtool.h"
#include "../xml/vdomdocument.h"

namespace Ui
{
    class DialogHistory;
}

class DialogHistory : public DialogTool
{
    Q_OBJECT
public:
                      DialogHistory(VContainer *data, VDomDocument *doc, QWidget *parent = 0);
    virtual           ~DialogHistory();
public slots:
    virtual void      DialogAccepted();
    void              cellClicked(int row, int column);
    void              ChangedCursor(qint64 id);
    void              UpdateHistory();
signals:
    void              ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    virtual void      closeEvent ( QCloseEvent * event );
private:
    Q_DISABLE_COPY(DialogHistory)
    Ui::DialogHistory *ui;
    VDomDocument      *doc;
    qint32            cursorRow;
    qint32            cursorToolRecordRow;
    void              FillTable();
    QString           Record(const VToolRecord &tool);
    void              InitialTable();
    void              ShowPoint();
};

#endif // DIALOGHISTORY_H
