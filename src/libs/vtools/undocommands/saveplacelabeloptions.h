/************************************************************************
 **
 **  @file   saveplacelabeloptions.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#ifndef SAVEPLACELABELOPTIONS_H
#define SAVEPLACELABELOPTIONS_H

#include "vundocommand.h"
#include "../vgeometry/vplacelabelitem.h"

class SavePlaceLabelOptions : public VUndoCommand
{
public:
    SavePlaceLabelOptions(const VPlaceLabelItem &oldLabel, const VPlaceLabelItem &newLabel, VAbstractPattern *doc,
                          VContainer *data, quint32 id, QUndoCommand *parent = nullptr);
    virtual ~SavePlaceLabelOptions()=default;

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;

    quint32         LabelId() const;
    VPlaceLabelItem NewLabel() const;
private:
    Q_DISABLE_COPY(SavePlaceLabelOptions)

    const VPlaceLabelItem m_oldLabel;
    VPlaceLabelItem       m_newLabel;

    VContainer *m_data;
};

#endif // SAVEPLACELABELOPTIONS_H
