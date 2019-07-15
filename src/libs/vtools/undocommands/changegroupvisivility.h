/************************************************************************
 **
 **  @file   changegroupvisivility.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef CHANGEGROUPVISIVILITY_H
#define CHANGEGROUPVISIVILITY_H

#include "vundocommand.h"

class ChangeGroupVisivility  : public VUndoCommand
{
    Q_OBJECT
public:
    ChangeGroupVisivility(VAbstractPattern *doc, vidtype id, bool visible, QUndoCommand *parent = nullptr);
    virtual ~ChangeGroupVisivility();
    virtual void undo() override;
    virtual void redo() override;

signals:
    void UpdateGroup(vidtype id, bool visible);

private:
    Q_DISABLE_COPY(ChangeGroupVisivility)

    bool m_oldVisibility{true};
    bool m_newVisibility{true};
    const QString m_nameActivDraw{};

    void Do(bool visible);
};

#endif // CHANGEGROUPVISIVILITY_H
