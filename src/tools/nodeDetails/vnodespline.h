/************************************************************************
 **
 **  @file   vnodespline.h
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

#ifndef VNODESPLINE_H
#define VNODESPLINE_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

/**
 * @brief The VNodeSpline class
 */
class VNodeSpline:public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
                  /**
                   * @brief VNodeSpline
                   * @param doc dom document container
                   * @param data container with variables
                   * @param id object id in container
                   * @param id object id in containerSpline
                   * @param typeCreation way we create this tool.
                   * @param parent parent object
                   */
                  VNodeSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                              const Tool::Sources &typeCreation, const qint64 &idTool = 0, QObject *qoParent = 0,
                              QGraphicsItem * parent = 0);
    /**
     * @brief Create help create tool
     * @param doc dom document container
     * @param data container with variables
     * @param id object id in container
     * @param id object id in containerSpline
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     * @return
     */
    static VNodeSpline *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                               const Document::Documents &parse, const Tool::Sources &typeCreation,
                               const qint64 &idTool = 0, QObject *parent = 0);
    /**
     * @brief TagName
     */
    static const QString TagName;
    /**
     * @brief ToolType
     */
    static const QString ToolType;
    virtual void DeleteNode();
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile ();
protected:
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void AddToFile ();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    /**
     * @brief RefreshGeometry
     */
    void         RefreshGeometry ();
};

#endif // VNODESPLINE_H
