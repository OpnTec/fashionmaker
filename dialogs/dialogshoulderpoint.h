/************************************************************************
 **
 **  @file   dialogshoulderpoint.h
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

#ifndef DIALOGSHOULDERPOINT_H
#define DIALOGSHOULDERPOINT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogShoulderPoint;
}

class DialogShoulderPoint : public DialogTool
{
    Q_OBJECT
public:
                   DialogShoulderPoint(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                       QWidget *parent = 0);
                   ~DialogShoulderPoint();
    inline QString getPointName() const {return pointName;}
    void           setPointName(const QString &value);
    inline QString getTypeLine() const {return typeLine;}
    void           setTypeLine(const QString &value);
    inline QString getFormula() const {return formula;}
    void           setFormula(const QString &value);
    inline qint64  getP1Line() const {return p1Line;}
    void           setP1Line(const qint64 &value, const qint64 &id);
    inline qint64  getP2Line() const {return p2Line;}
    void           setP2Line(const qint64 &value, const qint64 &id);
    inline qint64  getPShoulder() const {return pShoulder;}
    void           setPShoulder(const qint64 &value, const qint64 &id);
public slots:
    virtual void   ChoosedObject(qint64 id, const Scene::Scenes &type);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogShoulderPoint)
    Ui::DialogShoulderPoint *ui;
    qint32         number;
    QString        pointName;
    QString        typeLine;
    QString        formula;
    qint64         p1Line;
    qint64         p2Line;
    qint64         pShoulder;
};

#endif // DIALOGSHOULDERPOINT_H
