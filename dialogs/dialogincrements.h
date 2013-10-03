/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#include "dialogtool.h"
#include "xml/vdomdocument.h"

namespace Ui {
class DialogIncrements;
}

class DialogIncrements : public DialogTool
{
    Q_OBJECT   
public:
    explicit DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent = 0);
    ~DialogIncrements();
public slots:
    void clickedToolButtonAdd();
    void clickedToolButtonRemove();
    void cellChanged ( qint32 row, qint32 column );
    void FullUpdateFromFile();
    virtual void DialogAccepted();
signals:
    void FullUpdateTree();
    void haveLiteChange();
private:
    Q_DISABLE_COPY(DialogIncrements)
    Ui::DialogIncrements *ui;
    VContainer *data; // need because we must change data
    VDomDocument *doc;
    void InitialStandartTable();
    void InitialIncrementTable();
    void InitialLinesTable();
    void InitialSplinesTable();
    void InitialArcsTable();
    void FillStandartTable();
    void FillIncrementTable();
    void FillLengthLines();
    void FillLengthSplines();
    void FillLengthArcs();
    void AddIncrementToFile(qint64 id, QString name, qreal base, qreal ksize, qreal kgrowth,
                            QString description);
};

#endif // DIALOGINCREMENTS_H
