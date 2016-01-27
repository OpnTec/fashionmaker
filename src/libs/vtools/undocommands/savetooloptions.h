/************************************************************************
 **
 **  @file   savetooloptions.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
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

#ifndef SAVETOOLOPTIONS_H
#define SAVETOOLOPTIONS_H

#include "vundocommand.h"

class SaveToolOptions : public VUndoCommand
{
    Q_OBJECT
public:
    SaveToolOptions(const QDomElement &oldXml, const QDomElement &newXml, VAbstractPattern *doc, const quint32 &id,
                    QUndoCommand *parent = 0);
    virtual ~SaveToolOptions() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    QDomElement  getNewXml() const;
    quint32 getToolId() const;
private:
    Q_DISABLE_COPY(SaveToolOptions)
    const QDomElement oldXml;
    QDomElement       newXml;
};

//---------------------------------------------------------------------------------------------------------------------
inline QDomElement SaveToolOptions::getNewXml() const
{
    return newXml;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 SaveToolOptions::getToolId() const
{
    return nodeId;
}

#endif // SAVETOOLOPTIONS_H
