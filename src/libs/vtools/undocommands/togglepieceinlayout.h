/************************************************************************
 **
 **  @file   toggledetailinlayout.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2016
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

#ifndef TOGGLEDETAILINLAYOUT_H
#define TOGGLEDETAILINLAYOUT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class QUndoCommand;
class VAbstractPattern;
class VContainer;

class TogglePieceInLayout : public VUndoCommand
{
    Q_OBJECT
public:
    TogglePieceInLayout(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                        QUndoCommand *parent = nullptr);
    virtual ~TogglePieceInLayout();
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual int  id() const Q_DECL_OVERRIDE;
    quint32      getDetId() const;
    bool         getNewState() const;

signals:
    void UpdateList();
private:
    Q_DISABLE_COPY(TogglePieceInLayout)
    quint32     m_id;
    VContainer *m_data;
    bool        m_oldState;
    bool        m_newState;

    void Do(bool state);
};

#endif // TOGGLEDETAILINLAYOUT_H
