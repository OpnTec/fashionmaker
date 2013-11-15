/************************************************************************
 **
 **  @file vabstractnode.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include "../vabstracttool.h"

class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
                 VAbstractNode(VDomDocument *doc, VContainer *data, qint64 id, qint64 idNode,
                               Draw::Draws typeobject, QObject *parent = 0 );
    virtual      ~VAbstractNode() {}
    static const QString AttrIdObject;
    static const QString AttrTypeObject;
    static const QString TypeObjectCalculation;
    static const QString TypeObjectModeling;
protected:
    qint64       idNode;
    Draw::Draws  typeobject;
    void         AddToModeling(const QDomElement &domElement);
    virtual void decrementReferens();
};

#endif // VABSTRACTNODE_H
