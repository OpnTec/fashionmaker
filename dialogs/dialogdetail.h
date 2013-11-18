/************************************************************************
 **
 **  @file   dialogdetail.h
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

#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "ui_dialogdetail.h"
#include "dialogtool.h"

class DialogDetail : public DialogTool
{
    Q_OBJECT
public:
                     DialogDetail(const VContainer *data, Draw::Draws mode, QWidget *parent = 0);
    inline VDetail   getDetails() const {return details;}
    void             setDetails(const VDetail &value);
public slots:
    virtual void     ChoosedObject(qint64 id, const Scene::Scenes &type);
    virtual void     DialogAccepted();
    void             BiasXChanged(qreal d);
    void             BiasYChanged(qreal d);
    void             ClickedSeams(bool checked);
    void             ClickedClosed(bool checked);
    void             ObjectChanged(int row);
private:
    Ui::DialogDetail ui;
    VDetail          details;
    bool             supplement;
    bool             closed;
    void             NewItem(qint64 id, const Tool::Tools &typeTool, const Draw::Draws &mode,
                             const NodeDetail::NodeDetails &typeNode, qreal mx = 0, qreal my = 0);
};

#endif // DIALOGDETAIL_H
