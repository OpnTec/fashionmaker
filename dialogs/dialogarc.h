/************************************************************************
 **
 **  @file dialogarc.h
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

#ifndef DIALOGARC_H
#define DIALOGARC_H

#include "dialogtool.h"

namespace Ui
{
    class DialogArc;
}

class DialogArc : public DialogTool
{
    Q_OBJECT
public:
                     DialogArc(const VContainer *data, Draw::Draws mode = Draw::Calculation, QWidget *parent = 0);
                     ~DialogArc();
    inline qint64    GetCenter() const {return center;}
    void             SetCenter(const qint64 &value);
    inline QString   GetRadius() const {return radius;}
    void             SetRadius(const QString &value);
    inline QString   GetF1() const {return f1;}
    void             SetF1(const QString &value);
    inline QString   GetF2() const {return f2;}
    void             SetF2(const QString &value);
public slots:
    virtual void     ChoosedObject(qint64 id, const Scene::Scenes &type);
    virtual void     DialogAccepted();
    virtual void     ValChenged(int row);
    void             PutRadius();
    void             PutF1();
    void             PutF2();
    void             LineAngles();
    void             RadiusChanged();
    void             F1Changed();
    void             F2Changed();
protected:
    virtual void     CheckState();
private:
    Q_DISABLE_COPY(DialogArc)
    Ui::DialogArc    *ui;
    bool             flagRadius;
    bool             flagF1;
    bool             flagF2;
    QTimer           *timerRadius;
    QTimer           *timerF1;
    QTimer           *timerF2;
    qint64           center;
    QString          radius;
    QString          f1;
    QString          f2;
    void             EvalRadius();
    void             EvalF1();
    void             EvalF2();
    void             ShowLineAngles();
};

#endif // DIALOGARC_H
