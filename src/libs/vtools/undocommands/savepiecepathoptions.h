/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef SAVEPIECEPATHOPTIONS_H
#define SAVEPIECEPATHOPTIONS_H

#include <QtGlobal>

#include "../vpatterndb/vpiecepath.h"
#include "vundocommand.h"

class SavePiecePathOptions : public VUndoCommand
{
    Q_OBJECT
public:
    SavePiecePathOptions(quint32 pieceId, const VPiecePath &oldPath, const VPiecePath &newPath, VAbstractPattern *doc,
                         VContainer *data, quint32 id, QUndoCommand *parent = nullptr);
    virtual ~SavePiecePathOptions() = default;

    virtual void undo() override;
    virtual void redo() override;
    virtual bool mergeWith(const QUndoCommand *command) override;
    virtual int  id() const override;
    quint32      PathId() const;
    VPiecePath   NewPath() const;
private:
    Q_DISABLE_COPY(SavePiecePathOptions)

    const VPiecePath m_oldPath;
    VPiecePath       m_newPath;

    VContainer *m_data;
    quint32 m_pieceId;
};

//---------------------------------------------------------------------------------------------------------------------
inline int SavePiecePathOptions::id() const
{
    return static_cast<int>(UndoCommand::SavePiecePathOptions);
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 SavePiecePathOptions::PathId() const
{
    return nodeId;
}

//---------------------------------------------------------------------------------------------------------------------
inline VPiecePath SavePiecePathOptions::NewPath() const
{
    return m_newPath;
}

#endif // SAVEPIECEPATHOPTIONS_H
