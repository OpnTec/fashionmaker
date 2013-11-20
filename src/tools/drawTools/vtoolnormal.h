/************************************************************************
 **
 **  @file   vtoolnormal.h
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

#ifndef VTOOLNORMAL_H
#define VTOOLNORMAL_H

#include "vtoollinepoint.h"
#include "../../dialogs/dialognormal.h"

/**
 * @brief The VToolNormal class
 */
class VToolNormal : public VToolLinePoint
{
    Q_OBJECT
public:
                   /**
                    * @brief VToolNormal
                    * @param doc dom document container
                    * @param data
                    * @param id
                    * @param typeLine
                    * @param formula
                    * @param angle
                    * @param firstPointId
                    * @param secondPointId
                    * @param typeCreation
                    * @param parent
                    */
                   VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                               const QString &formula, const qreal &angle, const qint64 &firstPointId,
                               const qint64 &secondPointId, const Tool::Sources &typeCreation,
                               QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void   setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data
     */
    static void    Create(QSharedPointer<DialogNormal> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param formula
     * @param firstPointId
     * @param secondPointId
     * @param typeLine
     * @param pointName
     * @param angle
     * @param mx
     * @param my
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void    Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                          const qint64 &secondPointId, const QString &typeLine, const QString &pointName,
                          const qreal angle, const qreal &mx, const qreal &my, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                          const Tool::Sources &typeCreation);
    /**
     * @brief FindPoint
     * @param firstPoint
     * @param secondPoint
     * @param length
     * @param angle
     * @return
     */
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                             const qreal &angle = 0);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
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
     * @brief RemoveReferens
     */
    virtual void   RemoveReferens();
private:
    /**
     * @brief secondPointId
     */
    qint64         secondPointId;
    /**
     * @brief dialogNormal
     */
    QSharedPointer<DialogNormal> dialogNormal;
};

#endif // VTOOLNORMAL_H
