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

#include "moveabstractlabel.h"

class RotationMoveLabel : public MoveAbstractLabel
{
    Q_OBJECT
public:
    RotationMoveLabel(quint32 idTool, VAbstractPattern *doc, double x, double y, quint32 idPoint,
                      QUndoCommand *parent = nullptr);
    virtual ~RotationMoveLabel();

    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;

    quint32 GetToolId() const;
protected:
    virtual void Do(double mx, double my) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(RotationMoveLabel)
    quint32 m_idTool;

    QDomElement GetDestinationObject(quint32 idTool, quint32 idPoint) const;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 RotationMoveLabel::GetToolId() const
{
    return m_idTool;
}

#endif // ROTATIONMOVELABEL_H
