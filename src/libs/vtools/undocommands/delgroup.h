/************************************************************************
 **
 **  @file   delgroup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef DELGROUP_H
#define DELGROUP_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class DelGroup : public VUndoCommand
{
    Q_OBJECT
public:
    DelGroup(VAbstractPattern *doc, quint32 id, QUndoCommand *parent = nullptr);
    virtual ~DelGroup();
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(DelGroup)
    const QString nameActivDraw;
};

#endif // DELGROUP_H
