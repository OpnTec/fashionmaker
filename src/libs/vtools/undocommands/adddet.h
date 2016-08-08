/************************************************************************
 **
 **  @file   adddet.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2014
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

#ifndef ADDDET_H
#define ADDDET_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../tools/vtooldetail.h"
#include "vdetail.h"
#include "vundocommand.h"

class QDomElement;
class QUndoCommand;
class VAbstractPattern;

class AddDet : public VUndoCommand
{
    Q_OBJECT
public:
    AddDet(const QDomElement &xml, VAbstractPattern *doc, const VDetail &detail, const QString &drawName = QString(),
           QUndoCommand *parent = 0);
    virtual ~AddDet() Q_DECL_OVERRIDE;
    // cppcheck-suppress unusedFunction
    virtual void undo() Q_DECL_OVERRIDE;
    // cppcheck-suppress unusedFunction
    virtual void redo() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(AddDet)
    VDetail detail;
    QString drawName;

    QDomElement GetDetailsSection() const;
};

#endif // ADDDET_H
