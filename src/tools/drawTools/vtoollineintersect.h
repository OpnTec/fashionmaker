/************************************************************************
 **
 **  @file   vtoollineintersect.h
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

#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#include "vtoolpoint.h"

/**
 * @brief The VToolLineIntersect class help find point intersection lines.
 */
class VToolLineIntersect:public VToolPoint
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolLineIntersect constructor.
                  * @param doc dom document container.
                  * @param data container with variables.
                  * @param id object id in container.
                  * @param p1Line1 id first point first line.
                  * @param p2Line1 id second point first line.
                  * @param p1Line2 id first point second line.
                  * @param p2Line2 id second point second line.
                  * @param typeCreation way we create this tool.
                  * @param parent parent object.
                  */
                 VToolLineIntersect(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &p1Line1,
                                    const quint32 &p2Line1, const quint32 &p1Line2, const quint32 &p2Line2,
                                    const Tool::Sources &typeCreation, QGraphicsItem * parent = nullptr);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    /**
     * @brief Create help create tool from GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void  Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param p1Line1Id id first point first line.
     * @param p2Line1Id id second point first line.
     * @param p1Line2Id id first point second line.
     * @param p2Line2Id id second point second line.
     * @param pointName point name.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const quint32 _id, const quint32 &p1Line1Id, const quint32 &p2Line1Id, const quint32 &p1Line2Id,
                        const quint32 &p2Line2Id, const QString &pointName, const qreal &mx, const qreal &my,
                        VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile();
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
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void RemoveReferens();
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void SaveDialog(QDomElement &domElement);
private:
    /**
     * @brief p1Line1 id first point first line.
     */
    quint32       p1Line1;
    /**
     * @brief p2Line1 id second point first line.
     */
    quint32       p2Line1;
    /**
     * @brief p1Line2 id first point second line.
     */
    quint32       p1Line2;
    /**
     * @brief p2Line2 id second point second line.
     */
    quint32       p2Line2;
};

#endif // VTOOLLINEINTERSECT_H
