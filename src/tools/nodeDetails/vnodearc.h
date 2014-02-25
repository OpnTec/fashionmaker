/************************************************************************
 **
 **  @file   vnodearc.h
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

#ifndef VNODEARC_H
#define VNODEARC_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

/**
 * @brief The VNodeArc class arc detail node.
 */
class VNodeArc :public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VNodeArc constructor.
                  * @param doc dom document container.
                  * @param data container with variables.
                  * @param id object id in container.
                  * @param id object id in containerArc.
                  * @param typeCreation way we create this tool.
                  * @param parent parent object.
                  */
                 VNodeArc(VPattern *doc, VContainer *data, qint64 id, qint64 idArc,
                          const Tool::Sources &typeCreation, const qint64 &idTool = 0, QObject *qoParent = 0,
                          QGraphicsItem * parent = 0);
    /**
     * @brief Create help create tool.
     * @param doc dom document container.
     * @param data container with variables.
     * @param id object id in container.
     * @param id object id in containerArc.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(VPattern *doc, VContainer *data, qint64 id, qint64 idArc, const Document::Documents &parse,
                        const Tool::Sources &typeCreation, const qint64 &idTool = 0, QObject *parent = 0);
    static const QString TagName;
    static const QString ToolType;
    /**
     * @brief DeleteNode delete node from detail.
     */
    virtual void DeleteNode();
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile();
protected:
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    /**
     * @brief RefreshGeometry refresh item on scene.
     */
    void         RefreshGeometry();
};

#endif // VNODEARC_H4
