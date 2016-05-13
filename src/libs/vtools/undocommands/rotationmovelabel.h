/************************************************************************
 **
 **  @file   moverotationlabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 5, 2016
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

#ifndef ROTATIONMOVELABEL_H
#define ROTATIONMOVELABEL_H

#include "vundocommand.h"

class QGraphicsScene;

class RotationMoveLabel : public VUndoCommand
{
    Q_OBJECT
public:
    RotationMoveLabel(quint32 idTool, VAbstractPattern *doc, double x, double y, quint32 idPoint, QGraphicsScene *scene,
                      QUndoCommand *parent = nullptr);
    virtual ~RotationMoveLabel();
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      GetPointId() const;
    quint32      GetToolId() const;
    double       getNewMx() const;
    double       getNewMy() const;
signals:
    void ChangePosition(quint32 id, qreal mx, qreal my);
private:
    Q_DISABLE_COPY(RotationMoveLabel)
    double m_oldMx;
    double m_oldMy;
    double m_newMx;
    double m_newMy;
    bool   m_isRedo;
    QGraphicsScene *m_scene;
    quint32 m_idTool;

    void        Do(double mx, double my);
    QDomElement GetDestinationObject(quint32 idTool, quint32 idPoint) const;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 RotationMoveLabel::GetPointId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline double RotationMoveLabel::getNewMx() const
{
    return m_newMx;
}

//---------------------------------------------------------------------------------------------------------------------
inline double RotationMoveLabel::getNewMy() const
{
    return m_newMy;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 RotationMoveLabel::GetToolId() const
{
    return m_idTool;
}

#endif // ROTATIONMOVELABEL_H
