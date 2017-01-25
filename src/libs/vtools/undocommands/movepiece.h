/************************************************************************
 **
 **  @file   movedetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 6, 2014
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

#ifndef MOVEDETAIL_H
#define MOVEDETAIL_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class QDomElement;
class QGraphicsScene;
class QUndoCommand;
class VAbstractPattern;

class MovePiece : public VUndoCommand
{
    Q_OBJECT
public:
    MovePiece(VAbstractPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
               QUndoCommand *parent = nullptr);
    virtual ~MovePiece();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    // cppcheck-suppress unusedFunction
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      getDetId() const;
    double       getNewX() const;
    double       getNewY() const;
private:
    Q_DISABLE_COPY(MovePiece)

    double          m_oldX;
    double          m_oldY;
    double          m_newX;
    double          m_newY;
    QGraphicsScene *m_scene;

    void SaveCoordinates(QDomElement &domElement, double x, double y);
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MovePiece::getDetId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MovePiece::getNewX() const
{
    return m_newX;
}

//---------------------------------------------------------------------------------------------------------------------
inline double MovePiece::getNewY() const
{
    return m_newY;
}

#endif // MOVEDETAIL_H
