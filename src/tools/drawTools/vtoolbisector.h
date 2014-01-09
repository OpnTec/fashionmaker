/************************************************************************
 **
 **  @file   vtoolbisector.h
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

#ifndef VTOOLBISECTOR_H
#define VTOOLBISECTOR_H

#include "vtoollinepoint.h"
#include "../../dialogs/dialogbisector.h"

/**
 * @brief The VToolBisector class
 */
class VToolBisector : public VToolLinePoint
{
public:
                   /**
                    * @brief VToolBisector
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
                   VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                                 const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                                 const qint64 &thirdPointId, const Tool::Sources &typeCreation,
                                 QGraphicsItem * parent = 0);
    /**
     * @brief FindPoint
     * @param firstPoint
     * @param secondPoint
     * @param thirdPoint
     * @param length
     * @return
     */
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint,
                             const qreal& length);
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
    static void    Create(QSharedPointer<DialogBisector> &dialog, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data);
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
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void    Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                          const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                          const QString &pointName, const qreal &mx, const qreal &my, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                          const Tool::Sources &typeCreation);
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
     * @brief firstPointId
     */
    qint64         firstPointId;
    /**
     * @brief thirdPointId
     */
    qint64         thirdPointId;
    /**
     * @brief dialogBisector
     */
    QSharedPointer<DialogBisector> dialogBisector;
};

#endif // VTOOLBISECTOR_H
