/************************************************************************
 **
 **  @file   vabstractnode.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include "../vabstracttool.h"

#include <QColor>

/**
 * @brief The VAbstractNode class parent class for all detail node.
 */
class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
    VAbstractNode(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &idNode,
                  const quint32 &idTool, QObject *parent = nullptr);
    virtual      ~VAbstractNode() {}
    static const QString AttrIdObject;
    static const QString AttrIdTool;
    virtual void DeleteNode();
    virtual void RestoreNode();
    virtual void ShowVisualization(bool show);
protected:
    /** @brief idNodenode id. */
    quint32       idNode;

    /** @brief idTool id tool. */
    quint32       idTool;

    /** @brief currentColor current tool color. */
    QColor        currentColor;

    void         AddToModeling(const QDomElement &domElement);
    virtual void decrementReferens();
    virtual void RemoveReferens();
    virtual void RestoreReferens();
    virtual void SetVisualization() {}
};

#endif // VABSTRACTNODE_H
