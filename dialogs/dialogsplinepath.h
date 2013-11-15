/************************************************************************
 **
 **  @file dialogsplinepath.h
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

#ifndef DIALOGSPLINEPATH_H
#define DIALOGSPLINEPATH_H

#include "dialogtool.h"
#include "../geometry/vsplinepath.h"

namespace Ui
{
    class DialogSplinePath;
}

class DialogSplinePath : public DialogTool
{
    Q_OBJECT
public:
                       DialogSplinePath(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                        QWidget *parent = 0);
                       ~DialogSplinePath();
    inline VSplinePath GetPath() const {return path;}
    void               SetPath(const VSplinePath &value);
public slots:
    virtual void       ChoosedObject(qint64 id, const Scene::Scenes &type);
    virtual void       DialogAccepted();
    void               PointChenged(int row);
    void               currentPointChanged( int index );
    void               Angle1Changed( int index );
    void               Angle2Changed( int index );
    void               KAsm1Changed(qreal d);
    void               KAsm2Changed(qreal d);
private:
    Q_DISABLE_COPY(DialogSplinePath)
    Ui::DialogSplinePath *ui;
    VSplinePath        path;
    void               NewItem(qint64 id, qreal kAsm1, qreal angle, qreal kAsm2);
    void               DataPoint(qint64 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    void               EnableFields();
    void               SetAngle(qint32 angle);
};

#endif // DIALOGSPLINEPATH_H
