/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include "../vabstracttool.h"

class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
                 VAbstractNode(VDomDocument *doc, VContainer *data, qint64 id, qint64 idNode,
                               Draw::Draws typeobject, QObject *parent = 0 );
    virtual      ~VAbstractNode();
protected:
    qint64       idNode;
    Draw::Draws   typeobject;
    void         AddToModeling(const QDomElement &domElement);
};

#endif // VABSTRACTNODE_H
