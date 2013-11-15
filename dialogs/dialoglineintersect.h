/************************************************************************
 **
 **  @file dialoglineintersect.h
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

#ifndef DIALOGLINEINTERSECT_H
#define DIALOGLINEINTERSECT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogLineIntersect;
}

class DialogLineIntersect : public DialogTool
{
    Q_OBJECT
public:
                            DialogLineIntersect(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                                QWidget *parent = 0);
                            ~DialogLineIntersect();
    inline qint64           getP1Line1() const {return p1Line1;}
    void                    setP1Line1(const qint64 &value);
    inline qint64           getP2Line1() const {return p2Line1;}
    void                    setP2Line1(const qint64 &value);
    inline qint64           getP1Line2() const {return p1Line2;}
    void                    setP1Line2(const qint64 &value);
    inline qint64           getP2Line2() const {return p2Line2;}
    void                    setP2Line2(const qint64 &value);
    inline QString          getPointName() const {return pointName;}
    void                    setPointName(const QString &value);
public slots:
    virtual void            ChoosedObject(qint64 id, const Scene::Scenes &type);
    virtual void            DialogAccepted();
    void                    P1Line1Changed( int index);
    void                    P2Line1Changed( int index);
    void                    P1Line2Changed( int index);
    void                    P2Line2Changed( int index);
private:
    Q_DISABLE_COPY(DialogLineIntersect)
    Ui::DialogLineIntersect *ui;
    qint32                  number;
    QString                 pointName;
    qint64                  p1Line1;
    qint64                  p2Line1;
    qint64                  p1Line2;
    qint64                  p2Line2;
    bool                    flagPoint;
    virtual void            CheckState();
    bool                    CheckIntersecion();
};

#endif // DIALOGLINEINTERSECT_H
