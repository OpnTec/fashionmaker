/************************************************************************
 **
 **  @file   vundocommand.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 7, 2014
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

#ifndef VUNDOCOMMAND_H
#define VUNDOCOMMAND_H

#include <QUndoCommand>
#include <QDomElement>

enum class UndoCommand: char { AddPatternPiece,
                               AddToCalc,
                               MoveSpline,
                               MoveSplinePath,
                               MoveSPoint,
                               SaveToolOptions,
                               SaveDetailOptions,
                               MoveDetail,
                               DeleteTool,
                               DeletePatternPiece
                             };

class VPattern;

class VUndoCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    VUndoCommand(const QDomElement &xml, VPattern *doc, QUndoCommand *parent = 0);
    virtual ~VUndoCommand();
signals:
    void ClearScene();
    void NeedFullParsing();
    void NeedLiteParsing();
protected:
    QDomElement xml;
    VPattern    *doc;
    quint32     nodeId;
    bool        redoFlag;
    void        RedoFullParsing();
private:
    Q_DISABLE_COPY(VUndoCommand)
};

#endif // VUNDOCOMMAND_H
