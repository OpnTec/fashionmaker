/************************************************************************
 **
 **  @file   movesplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef MOVESPLINEPATH_H
#define MOVESPLINEPATH_H

#include "vundocommand.h"
#include "../libs/vgeometry/vsplinepath.h"

class QGraphicsScene;

class MoveSplinePath : public VUndoCommand
{
    Q_OBJECT
public:
    MoveSplinePath(VAbstractPattern *doc, const VSplinePath &oldSplPath, const VSplinePath &newSplPath,
                   const quint32 &id, QGraphicsScene *scene, QUndoCommand *parent = 0);
    virtual ~MoveSplinePath();
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *command);
    virtual int  id() const;
    quint32      getSplinePathId() const;
    VSplinePath  getNewSplinePath() const;
private:
    Q_DISABLE_COPY(MoveSplinePath)
    VSplinePath oldSplinePath;
    VSplinePath newSplinePath;
    QGraphicsScene *scene;
    void         Do(const VSplinePath &splPath);
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveSplinePath::getSplinePathId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline VSplinePath MoveSplinePath::getNewSplinePath() const
{
    return newSplinePath;
}

#endif // MOVESPLINEPATH_H
