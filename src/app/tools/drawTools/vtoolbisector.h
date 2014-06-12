/************************************************************************
 **
 **  @file   vtoolbisector.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef VTOOLBISECTOR_H
#define VTOOLBISECTOR_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolBisector class tool for creation bisector point.
 */
class VToolBisector : public VToolLinePoint
{
public:
                   /**
                    * @brief VToolBisector constructor.
                    * @param doc dom document container.
                    * @param data container with variables.
                    * @param id object id in container.
                    * @param typeLine line type.
                    * @param formula string with formula length of bisector.
                    * @param firstPointId id first point of angle.
                    * @param secondPointId id second point of angle.
                    * @param thirdPointId id third point of angle.
                    * @param typeCreation way we create this tool.
                    * @param parent parent object.
                    */
                   VToolBisector(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                                 const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                                 const quint32 &thirdPointId, const Source &typeCreation,
                                 QGraphicsItem * parent = nullptr);
    /**
     * @brief FindPoint find bisector point.
     * @param firstPoint first point of angle.
     * @param secondPoint second point of angle.
     * @param thirdPoint third point of angle.
     * @param length bisector length.
     * @return bisector point.
     */
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint,
                             const qreal& length);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void   setDialog();
    /**
     * @brief Create help create tool form GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static VToolBisector* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param formula string with formula.
     * @param firstPointId id first point of angle.
     * @param secondPointId id second point of angle.
     * @param thirdPointId id third point of angle.
     * @param typeLine line type.
     * @param pointName point name.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static VToolBisector* Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                          const quint32 &secondPointId, const quint32 &thirdPointId, const QString &typeLine,
                          const QString &pointName, const qreal &mx, const qreal &my, VMainGraphicsScene  *scene,
                          VPattern *doc, VContainer *data, const Document &parse,
                          const Source &typeCreation);
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void   FullUpdateFromFile();
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void   SetFactor(qreal factor);
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
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
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void   RemoveReferens();
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void   SaveDialog(QDomElement &domElement);
private:
    /**
     * @brief firstPointId id first point of angle.
     */
    quint32         firstPointId;
    /**
     * @brief thirdPointId id third point of angle.
     */
    quint32         thirdPointId;
};

#endif // VTOOLBISECTOR_H
