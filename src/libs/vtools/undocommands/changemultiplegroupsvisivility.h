/************************************************************************
 **
 **  @file   changemultiplegroupsvisivility.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2019
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
#ifndef CHANGEMULTIPLEGROUPSVISIVILITY_H
#define CHANGEMULTIPLEGROUPSVISIVILITY_H

#include "vundocommand.h"

class ChangeMultipleGroupsVisivility  : public VUndoCommand
{
    Q_OBJECT
public:
    ChangeMultipleGroupsVisivility(VAbstractPattern *doc, const QVector<vidtype> &groups, bool visible,
                                   QUndoCommand *parent = nullptr);
    virtual ~ChangeMultipleGroupsVisivility() =default;
    virtual void undo() override;
    virtual void redo() override;

signals:
    void UpdateMultipleGroups(const QMap<vidtype, bool> &groups);

private:
    Q_DISABLE_COPY(ChangeMultipleGroupsVisivility)

    QVector<vidtype> m_groups;
    bool m_newVisibility{true};
    QMap<vidtype, bool> m_oldVisibility{};
    const QString m_nameActivDraw{};
};

#endif // CHANGEMULTIPLEGROUPSVISIVILITY_H
