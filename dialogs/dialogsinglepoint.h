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

#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogSinglePoint;
}

class DialogSinglePoint : public DialogTool
{
    Q_OBJECT
public:
                   DialogSinglePoint(const VContainer *data, QWidget *parent = 0);
    void           setData(const QString &name, const QPointF &point);
    inline QString getName()const {return name;}
    inline QPointF getPoint()const {return point;}
                   ~DialogSinglePoint();
public slots:
    void           mousePress(const QPointF &scenePos);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogSinglePoint)
    Ui::DialogSinglePoint *ui;
    QString        name;
    QPointF        point;
};

#endif // DIALOGSINGLEPOINT_H
