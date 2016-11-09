/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2016
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

#ifndef DELETEPIECE_H
#define DELETEPIECE_H

#include <QtGlobal>

#include "../tools/vtoolseamallowance.h"
#include "vpiece.h"
#include "vundocommand.h"

class QGraphicsItem;
class QUndoCommand;
class VAbstractPattern;

class DeletePiece : public VUndoCommand
{
    Q_OBJECT
public:
    DeletePiece(VAbstractPattern *doc, quint32 id, const VPiece &m_detail, QUndoCommand *parent = nullptr);
    virtual ~DeletePiece();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DeletePiece)

    QDomNode m_parentNode;
    quint32  m_siblingId;
    VPiece   m_detail;
};

#endif // DELETEPIECE_H
