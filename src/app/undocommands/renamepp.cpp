/************************************************************************
 **
 **  @file   renamepp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "renamepp.h"

#include <QComboBox>
#include "../options.h"
#include "../xml/vpattern.h"


//---------------------------------------------------------------------------------------------------------------------
RenamePP::RenamePP(VPattern *doc, const QString &newPPname, QComboBox *combo, QUndoCommand *parent)
    :VUndoCommand(QDomElement(), doc, parent), combo(combo), newPPname(newPPname), oldPPname(QString())
{
    setText(tr("Rename pattern piece"));
    SCASSERT(combo != nullptr);
    oldPPname = doc->GetNameActivPP();
}

//---------------------------------------------------------------------------------------------------------------------
RenamePP::~RenamePP()
{}

void RenamePP::undo()
{
    ChangeName(newPPname, oldPPname);
}

void RenamePP::redo()
{
    ChangeName(oldPPname, newPPname);
}

bool RenamePP::mergeWith(const QUndoCommand *command)
{
    const RenamePP *renameCommand = static_cast<const RenamePP *>(command);
    SCASSERT(renameCommand != nullptr);

    const QString oldName = renameCommand->getOldPPname();
    if (newPPname != oldName)
    {
        return false;
    }

    newPPname = renameCommand->getNewPPname();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int RenamePP::id() const
{
    return static_cast<int>(UndoCommand::RenamePP);
}

//---------------------------------------------------------------------------------------------------------------------
void RenamePP::ChangeName(const QString &oldName, const QString &newName)
{
    if (doc->ChangeNamePP(oldName, newName))
    {
        combo->setItemText(combo->findText(oldName), newName);
    }
    else
    {
        qWarning()<<"Can't change pattern piece name";
    }
}
