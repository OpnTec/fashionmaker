/************************************************************************
 **
 **  @file   moveabstractlabel.h
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

#ifndef MOVEABSTRACTLABEL_H
#define MOVEABSTRACTLABEL_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vundocommand.h"

class QGraphicsScene;
class QGraphicsView;
class QUndoCommand;
class VAbstractPattern;

class MoveAbstractLabel : public VUndoCommand
{
    Q_OBJECT
public:
    MoveAbstractLabel(VAbstractPattern *doc, quint32 pointId, double x, double y, QUndoCommand *parent = nullptr);
    virtual ~MoveAbstractLabel();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;

    quint32 GetPointId() const;
    double  GetNewMx() const;
    double  GetNewMy() const;

signals:
    void ChangePosition(quint32 id, qreal mx, qreal my);
protected:
    double m_oldMx;
    double m_oldMy;
    double m_newMx;
    double m_newMy;
    bool   m_isRedo;
    //Need for resizing scene rect
    QGraphicsScene *m_scene;

    virtual void Do(double mx, double my)=0;
private:
    Q_DISABLE_COPY(MoveAbstractLabel)
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveAbstractLabel::GetPointId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveAbstractLabel::GetNewMx() const
{
    return m_newMx;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MoveAbstractLabel::GetNewMy() const
{
    return m_newMy;
}

#endif // MOVEABSTRACTLABEL_H
