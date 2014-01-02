/************************************************************************
 **
 **  @file   vabstractnode.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

/**
 * @brief The VAbstractNode class
 */
class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
                 /**
                  * @brief VAbstractNode
                  * @param doc dom document container
                  * @param data
                  * @param id
                  * @param idNode
                  * @param parent
                  */
                 VAbstractNode(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &idNode,
                               const qint64 &idTool);
    virtual      ~VAbstractNode() {}
    /**
     * @brief AttrIdObject
     */
    static const QString AttrIdObject;
    static const QString AttrIdTool;
protected:
    /**
     * @brief idNode
     */
    qint64       idNode;
    qint64       idTool;
    /**
     * @brief AddToModeling
     * @param domElement
     */
    void         AddToModeling(const QDomElement &domElement);
    /**
     * @brief decrementReferens
     */
    virtual void decrementReferens();
};

#endif // VABSTRACTNODE_H
