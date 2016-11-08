/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
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

#ifndef ADDPIECE_H
#define ADDPIECE_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../tools/vtoolseamallowance.h"
#include "vpiece.h"
#include "vundocommand.h"

class QDomElement;
class QUndoCommand;
class VAbstractPattern;


class AddPiece : public VUndoCommand
{
    Q_OBJECT
public:
    AddPiece(const QDomElement &xml, VAbstractPattern *doc, const VPiece &detail, const QString &drawName = QString(),
             QUndoCommand *parent = nullptr);
    virtual ~AddPiece();

    // cppcheck-suppress unusedFunction
    virtual void undo() Q_DECL_OVERRIDE;
    // cppcheck-suppress unusedFunction
    virtual void redo() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(AddPiece)

    VPiece m_detail;
    QString m_drawName;

    QDomElement GetDetailsSection() const;
};

#endif // ADDPIECE_H
