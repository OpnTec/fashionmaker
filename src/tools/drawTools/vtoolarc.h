/************************************************************************
 **
 **  @file   vtoolarc.h
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

#ifndef VTOOLARC_H
#define VTOOLARC_H

#include "vdrawtool.h"
#include <QGraphicsPathItem>
#include "../../widgets/vcontrolpointspline.h"

/**
 * @brief The VToolArc class tool for creation arc.
 */
class VToolArc :public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolArc constuctor.
                  * @param doc dom document container
                  * @param data container with variables
                  * @param id object id in container
                  * @param typeCreation way we create this tool.
                  * @param parent parent object
                  */
                 VToolArc(VPattern *doc, VContainer *data, quint32 id, const Tool::Sources &typeCreation,
                          QGraphicsItem * parent = 0);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    /**
     * @brief Create help create tool
     * @param dialog dialog options.
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     */
    static void  Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool form GUI.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param center id arc center point.
     * @param radius arc radius.
     * @param f1 start angle of arc.
     * @param f2 end angle of arc.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const quint32 _id, const quint32 &center, const QString &radius, const QString &f1,
                        const QString &f2, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString TagName;
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void     FullUpdateFromFile();
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void     ChangedActivDraw(const QString &newName);
    /**
     * @brief ShowTool highlight tool.
     * @param id object id in container
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    virtual void     ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void     SetFactor(qreal factor);
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void     AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void     RefreshDataInFile();
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void     RemoveReferens();
    /**
     * @brief itemChange handle tool change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief keyReleaseEvent handle key release events.
     * @param event key release event.
     */
    virtual void     keyReleaseEvent(QKeyEvent * event);
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void     SaveDialog(QDomElement &domElement);
private:
    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    void             RefreshGeometry();
};

#endif // VTOOLARC_H
