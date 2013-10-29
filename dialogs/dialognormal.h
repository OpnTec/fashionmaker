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

#ifndef DIALOGNORMAL_H
#define DIALOGNORMAL_H

#include "dialogtool.h"

namespace Ui {
class DialogNormal;
}

class DialogNormal : public DialogTool{
    Q_OBJECT
public:
    explicit         DialogNormal(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                  QWidget *parent = 0);
                     ~DialogNormal();
    inline QString   getPointName() const{return pointName;}
    void             setPointName(const QString &value);
    inline QString   getTypeLine() const {return typeLine;}
    void             setTypeLine(const QString &value);
    inline QString   getFormula() const {return formula;}
    void             setFormula(const QString &value);
    inline qreal     getAngle() const {return angle;}
    void             setAngle(const qreal &value);
    inline qint64    getFirstPointId() const {return firstPointId;}
    void             setFirstPointId(const qint64 &value, const qint64 &id);
    inline qint64    getSecondPointId() const {return secondPointId;}
    void             setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogNormal)
    Ui::DialogNormal *ui;
    qint32           number;
    QString          pointName;
    QString          typeLine;
    QString          formula;
    qreal            angle;
    qint64           firstPointId;
    qint64           secondPointId;
};

#endif // DIALOGNORMAL_H
