/************************************************************************
 **
 **  @file   vtoolshoulderpoint.h
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

#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

#include "vtoollinepoint.h"
#include "../../dialogs/dialogshoulderpoint.h"

/**
 * @brief The VToolShoulderPoint class
 */
class VToolShoulderPoint : public VToolLinePoint
{
public:
                   /**
                    * @brief VToolShoulderPoint
                    * @param doc dom document container
                    * @param data container with variables
                    * @param id object id in container
                    * @param typeLine line type.
                    * @param formula
                    * @param p1Line
                    * @param p2Line
                    * @param pShoulder
                    * @param typeCreation way we create this tool.
                    * @param parent parent object
                    */
                   VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                                      const QString &formula, const qint64 &p1Line, const qint64 &p2Line,
                                      const qint64 &pShoulder, const Tool::Sources &typeCreation,
                                      QGraphicsItem * parent = 0);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void   setDialog();
    /**
     * @brief FindPoint
     * @param p1Line
     * @param p2Line
     * @param pShoulder
     * @param length
     * @return
     */
    static QPointF FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                             const qreal &length);
    /**
     * @brief Create help create tool
     * @param dialog
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     */
    static void    Create(QSharedPointer<DialogShoulderPoint> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data);
    /**
     * @brief Create help create tool
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param formula
     * @param p1Line
     * @param p2Line
     * @param pShoulder
     * @param typeLine line type.
     * @param pointName
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void    Create(const qint64 _id, const QString &formula, const qint64 &p1Line, const qint64 &p2Line,
                          const qint64 &pShoulder, const QString &typeLine, const QString &pointName, const qreal &mx,
                          const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                          const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void   FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui  refresh tool data from change options.
     * @param result
     */
    virtual void   FullUpdateFromGui(int result);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void   SetFactor(qreal factor);
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
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
private:
    /**
     * @brief p2Line
     */
    qint64         p2Line;
    /**
     * @brief pShoulder
     */
    qint64         pShoulder;
    /**
     * @brief dialogShoulderPoint
     */
    QSharedPointer<DialogShoulderPoint> dialogShoulderPoint;
};

#endif // VTOOLSHOULDERPOINT_H
