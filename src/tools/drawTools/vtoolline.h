/************************************************************************
 **
 **  @file   vtoolline.h
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

#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#include "vdrawtool.h"
#include <QGraphicsLineItem>
#include "../../dialogs/dialogline.h"

/**
 * @brief The VToolLine class
 */
class VToolLine: public VDrawTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolLine
                  * @param doc dom document container
                  * @param data container with variables
                  * @param id object id in container
                  * @param firstPoint
                  * @param secondPoint
                  * @param typeCreation
                  * @param parent parent object
                  */
                 VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                           qint64 secondPoint, const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data container with variables
     */
    static void  Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param firstPoint
     * @param secondPoint
     * @param scene
     * @param doc dom document container
     * @param data container with variables
     * @param parse
     * @param typeCreation
     */
    static void  Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief TagName
     */
    static const QString TagName;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void     FullUpdateFromFile();
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void     ChangedActivDraw(const QString &newName);
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void     FullUpdateFromGui(int result);
    /**
     * @brief ShowTool
     * @param id object id in container
     * @param color
     * @param enable
     */
    virtual void     ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void     SetFactor(qreal factor);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void     AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void     RefreshDataInFile();
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens
     */
    virtual void     RemoveReferens();
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void     keyReleaseEvent(QKeyEvent * event);
private:
    /**
     * @brief firstPoint
     */
    qint64           firstPoint;
    /**
     * @brief secondPoint
     */
    qint64           secondPoint;
    /**
     * @brief dialogLine
     */
    QSharedPointer<DialogLine> dialogLine;
    /**
     * @brief RefreshGeometry
     */
    void             RefreshGeometry();
};

#endif // VTOOLLINE_H
