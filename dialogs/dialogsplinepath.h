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

#ifndef DIALOGSPLINEPATH_H
#define DIALOGSPLINEPATH_H

#include "dialogtool.h"
#include "container/vcontainer.h"
#include "geometry/vsplinepath.h"

namespace Ui {
class DialogSplinePath;
}

class DialogSplinePath : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogSplinePath(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                              QWidget *parent = 0);
    ~DialogSplinePath();
    VSplinePath      GetPath() const;
    void             SetPath(const VSplinePath &value);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
    void             PointChenged(int row);
    void             currentPointChanged( int index );
    void             Angle1Changed( int index );
    void             Angle2Changed( int index );
    void             KAsm1Changed(qreal d);
    void             KAsm2Changed(qreal d);
private:
    Ui::DialogSplinePath *ui;
    VSplinePath      path;
    void             NewItem(qint64 id, qreal kAsm1, qreal angle, qreal kAsm2);
    void             DataPoint(qint64 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    void             EnableFields();
    void             SetAngle(qint32 angle);
    DialogSplinePath(const DialogSplinePath &dialog);
    const DialogSplinePath &operator=(const DialogSplinePath& dialog);
};

#endif // DIALOGSPLINEPATH_H
