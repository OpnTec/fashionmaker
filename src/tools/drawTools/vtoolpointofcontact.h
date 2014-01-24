/************************************************************************
 **
 **  @file   vtoolpointofcontact.h
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

#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

#include "vtoolpoint.h"
#include "../../dialogs/dialogpointofcontact.h"

/**
 * @brief The VToolPointOfContact class
 */
class VToolPointOfContact : public VToolPoint
{
public:
                   /**
                    * @brief VToolPointOfContact
                    * @param doc dom document container
                    * @param data container with variables
                    * @param id object id in container
                    * @param radius
                    * @param center
                    * @param firstPointId
                    * @param secondPointId
                    * @param typeCreation
                    * @param parent parent object
                    */
                   VToolPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &radius, const qint64 &center,
                                       const qint64 &firstPointId, const qint64 &secondPointId,
                                       const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void   setDialog();
    /**
     * @brief FindPoint
     * @param radius
     * @param center
     * @param firstPoint
     * @param secondPoint
     * @return
     */
    static QPointF FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data container with variables
     */
    static void    Create(QSharedPointer<DialogPointOfContact> &dialog, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param radius
     * @param center
     * @param firstPointId
     * @param secondPointId
     * @param pointName
     * @param mx
     * @param my
     * @param scene
     * @param doc dom document container
     * @param data container with variables
     * @param parse
     * @param typeCreation
     */
    static void    Create(const qint64 _id, const QString &radius, const qint64 &center,
                          const qint64 &firstPointId, const qint64 &secondPointId, const QString &pointName,
                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation);
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
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void   FullUpdateFromGui(int result);
    /**
     * @brief SetFactor
     * @param factor
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
     * @brief AddToFile
     */
    virtual void   AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief RemoveReferens
     */
    virtual void   RemoveReferens();
private:
    /**
     * @brief radius
     */
    QString        radius;
    /**
     * @brief center
     */
    qint64         center;
    /**
     * @brief firstPointId
     */
    qint64         firstPointId;
    /**
     * @brief secondPointId
     */
    qint64         secondPointId;
    /**
     * @brief dialogPointOfContact
     */
    QSharedPointer<DialogPointOfContact> dialogPointOfContact;
};

#endif // VTOOLPOINTOFCONTACT_H
