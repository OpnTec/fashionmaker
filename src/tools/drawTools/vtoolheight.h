/************************************************************************
 **
 **  @file   vtoolheight.h
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

#ifndef VTOOLHEIGHT_H
#define VTOOLHEIGHT_H

#include "vtoollinepoint.h"
#include "../../dialogs/dialogheight.h"

/**
 * @brief The VToolHeight class
 */
class VToolHeight: public VToolLinePoint
{
    Q_OBJECT
public:
                   /**
                    * @brief VToolHeight
                    * @param doc dom document container
                    * @param data container with variables
                    * @param id object id in container
                    * @param typeLine line type.
                    * @param basePointId
                    * @param p1LineId
                    * @param p2LineId
                    * @param typeCreation way we create this tool.
                    * @param parent parent object
                    */
                   VToolHeight(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                               const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                               const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void   setDialog();
    /**
     * @brief Create help create tool
     * @param dialog
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     */
    static void    Create(QSharedPointer<DialogHeight> &dialog, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data);
    /**
     * @brief Create help create tool
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName point name.
     * @param typeLine line type.
     * @param basePointId
     * @param p1LineId
     * @param p2LineId
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void    Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                          const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief FindPoint
     * @param line
     * @param point
     * @return
     */
    static QPointF FindPoint(const QLineF &line, const QPointF &point);
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
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
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
private:
    /**
     * @brief dialogHeight
     */
    QSharedPointer<DialogHeight> dialogHeight;
    /**
     * @brief p1LineId
     */
    qint64         p1LineId;
    /**
     * @brief p2LineId
     */
    qint64         p2LineId;
};

#endif // VTOOLHEIGHT_H
