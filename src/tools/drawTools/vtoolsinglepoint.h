/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include "vtoolpoint.h"

/**
 * @brief The VToolSinglePoint class tool for creation pattern base point. Obly base point can move. All object
 * pattern peace depend on base point.
 */
class VToolSinglePoint : public VToolPoint
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolSinglePoint constructor.
                  * @param doc dom document container.
                  * @param data container with variables.
                  * @param id object id in container.
                  * @param typeCreation way we create this tool.
                  * @param parent parent object.
                  */
                 VToolSinglePoint (VPattern *doc, VContainer *data, quint32 id, const Valentina::Sources &typeCreation,
                                   QGraphicsItem * parent = nullptr );
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void ChangedActivDraw(const QString &newName);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void SetFactor(qreal factor);
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
signals:
    /**
     * @brief FullUpdateTree handle if need update pattern file.
     */
    void         FullUpdateTree();
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief itemChange handle tool change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief decrementReferens decrement referens parents objects.
     */
    virtual void decrementReferens();
    /**
     * @brief DeleteTool delete tool from file and scene. This tool can't be deleted by now.
     * @param tool tool what me delete.
     */
    virtual void DeleteTool(QGraphicsItem *tool){Q_UNUSED(tool)}
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void SaveDialog(QDomElement &domElement);
private:
    /**
     * @brief setColorLabel change color for label and label line.
     * @param color new color.
     */
    void         setColorLabel(const Qt::GlobalColor & color);
};

#endif // VTOOLSINGLEPOINT_H
