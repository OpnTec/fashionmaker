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

#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include "dialogtool.h"
#include "container/vcontainer.h"

namespace Ui {
class DialogSpline;
}

class DialogSpline : public DialogTool
{
    Q_OBJECT  
public:
    explicit         DialogSpline(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                  QWidget *parent = 0);
                     ~DialogSpline();
    qint64           getP1() const;
    void             setP1(const qint64 &value);
    qint64           getP4() const;
    void             setP4(const qint64 &value);
    qreal            getAngle1() const;
    void             setAngle1(const qreal &value);
    qreal            getAngle2() const;
    void             setAngle2(const qreal &value);
    qreal            getKAsm1() const;
    void             setKAsm1(const qreal &value);
    qreal            getKAsm2() const;
    void             setKAsm2(const qreal &value);
    qreal            getKCurve() const;
    void             setKCurve(const qreal &value);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogSpline)
    Ui::DialogSpline *ui;
    qint32           number;
    qint64           p1;			// перша точка
    qint64           p4;			// четверта точка
    qreal            angle1;		// кут нахилу дотичної в першій точці
    qreal            angle2;		// кут нахилу дотичної в другій точці
    qreal            kAsm1;
    qreal            kAsm2;
    qreal            kCurve;
};

#endif // DIALOGSPLINE_H
