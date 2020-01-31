/************************************************************************
 **
 **  @file   addgroup.h
 **  @author Ronan Le Tiec
 **  @date   31 3, 2018
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef ADDITEMTOGROUP_H
#define ADDITEMTOGROUP_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class AddItemToGroup : public VUndoCommand
{
    Q_OBJECT
public:
    AddItemToGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId, QUndoCommand *parent = nullptr);
    virtual ~AddItemToGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
protected:
    void performUndoRedo(bool isUndo);
private:
    Q_DISABLE_COPY(AddItemToGroup)
    const QString nameActivDraw;
};

#endif // ADDITEMTOGROUP_H
