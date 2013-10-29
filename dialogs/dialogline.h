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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include "dialogtool.h"

namespace Ui {
class DialogLine;
}

class DialogLine : public DialogTool{
    Q_OBJECT
public:
    explicit         DialogLine(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                QWidget *parent = 0);
                     ~DialogLine();   
    inline qint64    getFirstPoint() const {return firstPoint;}
    void             setFirstPoint(const qint64 &value);
    inline qint64    getSecondPoint() const {return secondPoint;}
    void             setSecondPoint(const qint64 &value);
public slots:
    void             ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogLine)
    Ui::DialogLine   *ui;
    qint32           number;
    qint64           firstPoint;
    qint64           secondPoint;
};

#endif // DIALOGLINE_H
