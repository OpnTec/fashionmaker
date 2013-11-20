/************************************************************************
 **
 **  @file   vmodelingtriangle.h
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

#ifndef VMODELINGTRIANGLE_H
#define VMODELINGTRIANGLE_H

#include "vmodelingpoint.h"
#include "../drawTools/vtooltriangle.h"
#include "../../dialogs/dialogtriangle.h"

/**
 * @brief The VModelingTriangle class
 */
class VModelingTriangle : public VModelingPoint
{
    Q_OBJECT
public:
                 /**
                  * @brief VModelingTriangle
                  * @param doc dom document container
                  * @param data
                  * @param id
                  * @param axisP1Id
                  * @param axisP2Id
                  * @param firstPointId
                  * @param secondPointId
                  * @param typeCreation
                  * @param parent
                  */
                 VModelingTriangle(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &axisP1Id,
                                   const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                                   const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param doc dom document container
     * @param data
     * @return
     */
    static VModelingTriangle* Create(QSharedPointer<DialogTriangle> &dialog, VDomDocument *doc, VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param pointName
     * @param axisP1Id
     * @param axisP2Id
     * @param firstPointId
     * @param secondPointId
     * @param mx
     * @param my
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     * @return
     */
    static VModelingTriangle* Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id,
                                     const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                                     const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                     const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui(int result);
protected:
    /**
     * @brief RemoveReferens
     */
    virtual void RemoveReferens();
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void AddToFile();
private:
    Q_DISABLE_COPY(VModelingTriangle)
    /**
     * @brief axisP1Id
     */
    qint64       axisP1Id;
    /**
     * @brief axisP2Id
     */
    qint64       axisP2Id;
    /**
     * @brief firstPointId
     */
    qint64       firstPointId;
    /**
     * @brief secondPointId
     */
    qint64       secondPointId;
    /**
     * @brief dialogTriangle
     */
    QSharedPointer<DialogTriangle> dialogTriangle;
};

#endif // VMODELINGTRIANGLE_H
