/************************************************************************
 **
 **  @file   movespoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2014
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

#ifndef MOVESPOINT_H
#define MOVESPOINT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class QGraphicsScene;

class MoveSPoint : public VUndoCommand
{
    Q_OBJECT
public:
    MoveSPoint(VAbstractPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
               QUndoCommand *parent = 0);
    virtual ~MoveSPoint() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      getSPointId() const;
    double       getNewX() const;
    double       getNewY() const;
    void         Do(double x, double y);
private:
    Q_DISABLE_COPY(MoveSPoint)
    double   oldX;
    double   oldY;
    double   newX;
    double   newY;
    QGraphicsScene *scene;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveSPoint::getSPointId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveSPoint::getNewX() const
{
    return newX;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveSPoint::getNewY() const
{
    return newY;
}

#endif // MOVESPOINT_H
