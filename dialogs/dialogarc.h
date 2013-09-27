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

#ifndef DIALOGARC_H
#define DIALOGARC_H

#include "dialogtool.h"
#include "container/vcontainer.h"

namespace Ui {
class DialogArc;
}

class DialogArc : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogArc(const VContainer *data, Draw::Draws mode = Draw::Calculation, QWidget *parent = 0);
    ~DialogArc();
    qint64 GetCenter() const;
    void SetCenter(const qint64 &value);

    QString GetRadius() const;
    void SetRadius(const QString &value);

    QString GetF1() const;
    void SetF1(const QString &value);

    QString GetF2() const;
    void SetF2(const QString &value);

public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void     DialogAccepted();
    virtual void     ValChenged(int row);
    void             PutRadius();
    void             PutF1();
    void             PutF2();
    void             LineArcs();
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
    void             ShowLineArcs();
};

#endif // DIALOGARC_H
