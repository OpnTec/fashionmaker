/************************************************************************
 **
 **  @file   movedoublelabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef MOVEDOUBLELABEL_H
#define MOVEDOUBLELABEL_H

#include "vundocommand.h"

class QGraphicsScene;

enum class DoublePoint: char { FirstPoint, SecondPoint };

class MoveDoubleLabel : public VUndoCommand
{
    Q_OBJECT
public:
    MoveDoubleLabel(VAbstractPattern *doc, const double &x, const double &y, DoublePoint type,
                    quint32 tooId, quint32 pointId, QGraphicsScene *scene, QUndoCommand *parent = 0);
    virtual ~MoveDoubleLabel() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      getPointId() const;
    quint32      getLabelId() const;
    double       getNewMx() const;
    double       getNewMy() const;
    DoublePoint  getPointType() const;
    void         Do(double mx, double my);
signals:
    void ChangePosition(quint32 id, qreal mx, qreal my);
private:
    Q_DISABLE_COPY(MoveDoubleLabel)
    double oldMx;
    double oldMy;
    double newMx;
    double newMy;
    QGraphicsScene *scene;
    DoublePoint type;
    quint32 pointId;
    bool isRedo;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveDoubleLabel::getPointId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline DoublePoint MoveDoubleLabel::getPointType() const
{
    return type;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveDoubleLabel::getNewMx() const
{
    return newMx;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveDoubleLabel::getNewMy() const
{
    return newMy;
}

#endif // MOVEDOUBLELABEL_H
