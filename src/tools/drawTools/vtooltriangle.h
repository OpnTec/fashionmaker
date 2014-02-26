/************************************************************************
 **
 **  @file   vtooltriangle.h
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

#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

#include "vtoolpoint.h"

/**
 * @brief The VToolTriangle class tool for what find point intersection two foots right triangle.
 */
class VToolTriangle : public VToolPoint
{
    Q_OBJECT
public:
                   /**
                    * @brief VToolTriangle constructor.
                    * @param doc dom document container.
                    * @param data container with variables.
                    * @param id object id in container.
                    * @param axisP1Id id first axis point.
                    * @param axisP2Id id second axis point.
                    * @param firstPointId id first triangle point, what lies on the hypotenuse.
                    * @param secondPointId id second triangle point, what lies on the hypotenuse.
                    * @param typeCreation way we create this tool.
                    * @param parent parent object.
                    */
                   VToolTriangle(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &axisP1Id,
                                 const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                                 const Tool::Sources &typeCreation, QGraphicsItem * parent = nullptr);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void   setDialog();
    /**
     * @brief Create help create tool from GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void    Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName point name.
     * @param axisP1Id id first axis point.
     * @param axisP2Id id second axis point.
     * @param firstPointId id first triangle point, what lies on the hypotenuse.
     * @param secondPointId id second triangle point, what lies on the hypotenuse.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void    Create(const quint32 _id, const QString &pointName, const quint32 &axisP1Id, const quint32 &axisP2Id,
                          const quint32 &firstPointId, const quint32 &secondPointId, const qreal &mx, const qreal &my,
                          VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                          const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief FindPoint find point intersection two foots right triangle.
     * @param axisP1 first axis point.
     * @param axisP2 second axis point.
     * @param firstPoint first triangle point, what lies on the hypotenuse.
     * @param secondPoint second triangle point, what lies on the hypotenuse.
     * @return point intersection two foots right triangle.
     */
    static QPointF FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void   FullUpdateFromFile();
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void   RemoveReferens();
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void   AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void SaveDialog(QDomElement &domElement);
private:
    Q_DISABLE_COPY(VToolTriangle)
    /**
     * @brief axisP1Id id first axis point.
     */
    quint32         axisP1Id;
    /**
     * @brief axisP2Id id second axis point.
     */
    quint32         axisP2Id;
    /**
     * @brief firstPointId id first triangle point, what lies on the hypotenuse.
     */
    quint32         firstPointId;
    /**
     * @brief secondPointId id second triangle point, what lies on the hypotenuse.
     */
    quint32         secondPointId;
};

#endif // VTOOLTRIANGLE_H
