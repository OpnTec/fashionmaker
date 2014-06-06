/************************************************************************
 **
 **  @file   vundocommands.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 6, 2014
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

#ifndef VUNDOCOMMANDS_H
#define VUNDOCOMMANDS_H

#include "vpattern.h"

#include <QDomElement>
#include <QUndoCommand>

class AddToCal : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    AddToCal(const QDomElement &xml, VPattern *doc, QUndoCommand *parent = 0);
    virtual ~AddToCal();
    virtual void undo();
    virtual void redo();
signals:
    void UnsavedChange();
    void NeedFullParsing();
private:
    Q_DISABLE_COPY(AddToCal)
    const QDomElement xml;
    VPattern *doc;
    const QString nameActivDraw;
    quint32 cursor;
};

class AddPatternPiece : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    AddPatternPiece(const QDomElement &xml, VPattern *doc, QUndoCommand *parent = 0);
    virtual ~AddPatternPiece();
    virtual void undo();
    virtual void redo();
signals:
    void UnsavedChange();
    void NeedFullParsing();
private:
    Q_DISABLE_COPY(AddPatternPiece)
    const QDomElement xml;
    VPattern *doc;
    static int countPP;
};

#endif // VUNDOCOMMANDS_H
