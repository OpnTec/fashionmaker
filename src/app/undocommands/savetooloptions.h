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

#ifndef SAVETOOLOPTIONS_H
#define SAVETOOLOPTIONS_H

#include <QUndoCommand>
#include <QDomElement>

class VPattern;

class SaveToolOptions : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    SaveToolOptions(const QDomElement &oldXml, const QDomElement &newXml, VPattern *doc, const quint32 &id, QUndoCommand *parent = 0);
    virtual ~SaveToolOptions();
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *command);
    virtual int  id() const;
    QDomElement  getNewXml() const;
    quint32 getToolId() const;
signals:
    void NeedLiteParsing();
private:
    Q_DISABLE_COPY(SaveToolOptions)
    enum { Id = 3 };
    const QDomElement oldXml;
    QDomElement       newXml;
    VPattern          *doc;
    const quint32     toolId;
};

//---------------------------------------------------------------------------------------------------------------------
inline QDomElement SaveToolOptions::getNewXml() const
{
    return newXml;
}

//---------------------------------------------------------------------------------------------------------------------
inline int SaveToolOptions::id() const
{
    return Id;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 SaveToolOptions::getToolId() const
{
    return toolId;
}

#endif // SAVETOOLOPTIONS_H
