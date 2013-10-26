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

#ifndef DIALOGALONGLINE_H
#define DIALOGALONGLINE_H

#include "dialogtool.h"

namespace Ui {
class DialogAlongLine;
}

class DialogAlongLine : public DialogTool{
    Q_OBJECT    
public:
                        DialogAlongLine(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                        QWidget *parent = 0);
                        ~DialogAlongLine();
    inline QString      getPointName() const {return pointName;}
    void                setPointName(const QString &value);
    inline QString      getTypeLine() const {return typeLine;}
    void                setTypeLine(const QString &value);
    inline QString      getFormula() const {return formula;}
    void                setFormula(const QString &value);
    inline qint64       getFirstPointId() const {return firstPointId;}
    void                setFirstPointId(const qint64 &value, const qint64 &id);
    inline qint64       getSecondPointId() const {return secondPointId;}
    void                setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void        ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void        DialogAccepted();   
private:
    Q_DISABLE_COPY(DialogAlongLine)
    Ui::DialogAlongLine *ui;
    qint32              number;
    QString             pointName;
    QString             typeLine;
    QString             formula;
    qint64              firstPointId;
    qint64              secondPointId;
};

#endif // DIALOGALONGLINE_H
