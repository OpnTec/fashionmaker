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

#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "ui_dialogdetail.h"
#include "dialogtool.h"
#include "container/vcontainer.h"
#include "geometry/vdetail.h"

class DialogDetail : public DialogTool
{
    Q_OBJECT  
public:
    explicit DialogDetail(const VContainer *data, Draw::Draws mode, QWidget *parent = 0);
    VDetail getDetails() const;
    void setDetails(const VDetail &value);

public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void      DialogAccepted();
private:
    Ui::DialogDetail ui;
    VDetail details;
    void             NewItem(qint64 id, Tool::Tools typeTool, Draw::Draws mode, NodeDetail::NodeDetails typeNode);
};

#endif // DIALOGDETAIL_H
