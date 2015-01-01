/************************************************************************
 **
 **  @file   movelabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef MOVELABEL_H
#define MOVELABEL_H

#include "vundocommand.h"

class QGraphicsScene;

class MoveLabel : public VUndoCommand
{
    Q_OBJECT
public:
    MoveLabel(VPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
               QUndoCommand *parent = 0);
    virtual ~MoveLabel();
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *command);
    virtual int  id() const;
    quint32      getPointId() const;
    double       getNewMx() const;
    double       getNewMy() const;
    void         Do(double mx, double my);
private:
    Q_DISABLE_COPY(MoveLabel)
    double   oldMx;
    double   oldMy;
    double   newMx;
    double   newMy;
    QGraphicsScene *scene;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveLabel::getPointId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveLabel::getNewMx() const
{
    return newMx;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveLabel::getNewMy() const
{
    return newMy;
}

#endif // MOVELABEL_H
