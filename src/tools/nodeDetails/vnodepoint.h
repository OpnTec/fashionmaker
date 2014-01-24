/************************************************************************
 **
 **  @file   vnodepoint.h
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

#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include "vabstractnode.h"
#include "../../widgets/vgraphicssimpletextitem.h"

/**
 * @brief The VNodePoint class
 */
class VNodePoint: public VAbstractNode, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VNodePoint
                  * @param doc dom document container
                  * @param data container with variables
                  * @param id object id in container
                  * @param id object id in containerPoint
                  * @param typeCreation
                  * @param parent parent object
                  */
                 VNodePoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                            const Tool::Sources &typeCreation, const qint64 &idTool = 0, QObject *qoParent = 0,
                            QGraphicsItem * parent = 0 );
    /**
     * @brief Create
     * @param doc dom document container
     * @param data container with variables
     * @param id object id in container
     * @param id object id in containerPoint
     * @param parse
     * @param typeCreation
     */
    static void  Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                        const Document::Documents &parse, const Tool::Sources &typeCreation, const qint64 &idTool = 0,
                        QObject *parent = 0);
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
    virtual void FullUpdateFromFile();
    /**
     * @brief NameChangePosition
     * @param pos
     */
    void         NameChangePosition(const QPointF &pos);
protected:
    /**
     * @brief radius
     */
    qreal        radius;
    /**
     * @brief namePoint
     */
    VGraphicsSimpleTextItem *namePoint;
    /**
     * @brief lineName
     */
    QGraphicsLineItem       *lineName;
    /**
     * @brief AddToFile
     */
    virtual void AddToFile();
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
    /**
     * @brief UpdateNamePosition
     * @param mx
     * @param my
     */
    virtual void UpdateNamePosition(qreal mx, qreal my);
    /**
     * @brief RefreshPointGeometry
     * @param point
     */
    virtual void RefreshPointGeometry(const VPointF &point);
    /**
     * @brief RefreshLine
     */
    void         RefreshLine();
private:
    Q_DISABLE_COPY(VNodePoint)
};

#endif // VNODEPOINT_H
