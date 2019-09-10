/************************************************************************
 **
 **  @file   movedoublelabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "moveabstractlabel.h"

enum class MoveDoublePoint: char { FirstPoint, SecondPoint };

class MoveDoubleLabel : public MoveAbstractLabel
{
    Q_OBJECT
public:
    MoveDoubleLabel(VAbstractPattern *doc, const QPointF &pos, MoveDoublePoint type,
                    quint32 toolId, quint32 pointId, QUndoCommand *parent = nullptr);
    virtual ~MoveDoubleLabel()=default;

    virtual bool mergeWith(const QUndoCommand *command) override;
    virtual int  id() const override;

    quint32     GetToolId() const;
    MoveDoublePoint GetPointType() const;
protected:
    virtual void Do(const QPointF &pos) override;
private:
    Q_DISABLE_COPY(MoveDoubleLabel)
    MoveDoublePoint m_type;
    quint32 m_idTool;
    //Need for resizing scene rect
    QGraphicsScene *m_scene;
};

//---------------------------------------------------------------------------------------------------------------------
inline MoveDoublePoint MoveDoubleLabel::GetPointType() const
{
    return m_type;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveDoubleLabel::GetToolId() const
{
    return m_idTool;
}

#endif // MOVEDOUBLELABEL_H
