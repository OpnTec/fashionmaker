/************************************************************************
 **
 **  @file   vmodelingbisector.h
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

#ifndef VMODELINGBISECTOR_H
#define VMODELINGBISECTOR_H

#include "vmodelinglinepoint.h"
#include "../../dialogs/dialogbisector.h"

/**
 * @brief The VModelingBisector class
 */
class VModelingBisector : public VModelingLinePoint
{
    Q_OBJECT
public:
                              /**
                               * @brief VModelingBisector
                               * @param doc dom document container
                               * @param data
                               * @param id
                               * @param typeLine
                               * @param formula
                               * @param firstPointId
                               * @param secondPointId
                               * @param thirdPointId
                               * @param typeCreation
                               * @param parent
                               */
                              VModelingBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                const QString &typeLine, const QString &formula,
                                                const qint64 &firstPointId, const qint64 &secondPointId,
                                                const qint64 &thirdPointId, const Tool::Sources &typeCreation,
                                                QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void              setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param doc dom document container
     * @param data
     * @return
     */
    static VModelingBisector* Create(QSharedPointer<DialogBisector> &dialog, VDomDocument *doc, VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param formula
     * @param firstPointId
     * @param secondPointId
     * @param thirdPointId
     * @param typeLine
     * @param pointName
     * @param mx
     * @param my
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     * @return
     */
    static VModelingBisector* Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                                     const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                                     const QString &pointName, const qreal &mx, const qreal &my, VDomDocument *doc,
                                     VContainer *data, const Document::Documents &parse,
                                     const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void              FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void              FullUpdateFromGui(int result);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void              contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void              AddToFile();
    /**
     * @brief RemoveReferens
     */
    virtual void              RemoveReferens();
private:
    /**
     * @brief firstPointId
     */
    qint64                    firstPointId;
    /**
     * @brief thirdPointId
     */
    qint64                    thirdPointId;
    /**
     * @brief dialogBisector
     */
    QSharedPointer<DialogBisector> dialogBisector;
};

#endif // VMODELINGBISECTOR_H
