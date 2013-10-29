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

#ifndef DIALOGTRIANGLE_H
#define DIALOGTRIANGLE_H

#include "dialogtool.h"

namespace Ui {
class DialogTriangle;
}

class DialogTriangle : public DialogTool{
    Q_OBJECT
public:
                   DialogTriangle(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                  QWidget *parent = 0);
                   ~DialogTriangle();
    inline qint64  getAxisP1Id() const {return axisP1Id;}
    void           setAxisP1Id(const qint64 &value, const qint64 &id);
    inline qint64  getAxisP2Id() const {return axisP2Id;}
    void           setAxisP2Id(const qint64 &value, const qint64 &id);
    inline qint64  getFirstPointId() const {return firstPointId;}
    void           setFirstPointId(const qint64 &value, const qint64 &id);
    inline qint64  getSecondPointId() const {return secondPointId;}
    void           setSecondPointId(const qint64 &value, const qint64 &id);
    inline QString getPointName() const {return pointName;}
    void           setPointName(const QString &value);
public slots:
    virtual void   ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogTriangle)
    Ui::DialogTriangle *ui;
    qint32         number;
    QString        pointName;
    qint64         axisP1Id;
    qint64         axisP2Id;
    qint64         firstPointId;
    qint64         secondPointId;
};

#endif // DIALOGTRIANGLE_H
