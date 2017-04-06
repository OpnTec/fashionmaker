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

#ifndef SAVEPIECEOPTIONS_H
#define SAVEPIECEOPTIONS_H

#include <QtGlobal>

#include "vpiece.h"
#include "vundocommand.h"

class SavePieceOptions : public VUndoCommand
{
public:
    SavePieceOptions(const VPiece &oldDet, const VPiece &newDet, VAbstractPattern *doc, quint32 id,
                     QUndoCommand *parent = nullptr);
    virtual ~SavePieceOptions();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      DetId() const;
    VPiece       NewDet() const;
private:
    Q_DISABLE_COPY(SavePieceOptions)

    const VPiece    m_oldDet;
    VPiece          m_newDet;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 SavePieceOptions::DetId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline VPiece SavePieceOptions::NewDet() const
{
    return m_newDet;
}

#endif // SAVEPIECEOPTIONS_H
