/************************************************************************
 **
 **  @file   vmodelingpointofcontact.h
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

#ifndef VMODELINGPOINTOFCONTACT_H
#define VMODELINGPOINTOFCONTACT_H

#include "vmodelingpoint.h"
#include "../../dialogs/dialogpointofcontact.h"

/**
 * @brief The VModelingPointOfContact class
 */
class VModelingPointOfContact : public VModelingPoint
{
    Q_OBJECT
public:
                                    /**
                                     * @brief VModelingPointOfContact
                                     * @param doc dom document container
                                     * @param data
                                     * @param id
                                     * @param radius
                                     * @param center
                                     * @param firstPointId
                                     * @param secondPointId
                                     * @param typeCreation
                                     * @param parent
                                     */
                                    VModelingPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                            const QString &radius, const qint64 &center,
                                                            const qint64 &firstPointId, const qint64 &secondPointId,
                                                            const Tool::Sources &typeCreation,
                                                            QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void                    setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param doc dom document container
     * @param data
     * @return
     */
    static VModelingPointOfContact* Create(QSharedPointer<DialogPointOfContact> &dialog, VDomDocument *doc,
                                           VContainer *data);
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
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     * @return
     */
    static VModelingPointOfContact* Create(const qint64 _id, const QString &radius, const qint64 &center,
                                           const qint64 &firstPointId, const qint64 &secondPointId,
                                           const QString &pointName, const qreal &mx, const qreal &my,
                                           VDomDocument *doc, VContainer *data,
                                           const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void                    FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void                    FullUpdateFromGui(int result);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void                    contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void                    AddToFile();
    /**
     * @brief RemoveReferens
     */
    virtual void                    RemoveReferens();
private:
    /**
     * @brief radius
     */
    QString                         radius;
    /**
     * @brief center
     */
    qint64                          center;
    /**
     * @brief firstPointId
     */
    qint64                          firstPointId;
    /**
     * @brief secondPointId
     */
    qint64                          secondPointId;
    /**
     * @brief dialogPointOfContact
     */
    QSharedPointer<DialogPointOfContact> dialogPointOfContact;
};

#endif // VMODELINGPOINTOFCONTACT_H
