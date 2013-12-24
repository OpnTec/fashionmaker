/************************************************************************
 **
 **  @file   vtoollineintersect.h
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

#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#include "vtoolpoint.h"
#include "../../dialogs/dialoglineintersect.h"

/**
 * @brief The VToolLineIntersect class
 */
class VToolLineIntersect:public VToolPoint
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolLineIntersect
                  * @param doc dom document container
                  * @param data
                  * @param id
                  * @param p1Line1
                  * @param p2Line1
                  * @param p1Line2
                  * @param p2Line2
                  * @param typeCreation
                  * @param parent
                  */
                 VToolLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &p1Line1,
                                    const qint64 &p2Line1, const qint64 &p1Line2, const qint64 &p2Line2,
                                    const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data
     */
    static void  Create(QSharedPointer<DialogLineIntersect> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param p1Line1Id
     * @param p2Line1Id
     * @param p1Line2Id
     * @param p2Line2Id
     * @param pointName
     * @param mx
     * @param my
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void  Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id, const qint64 &p1Line2Id,
                        const qint64 &p2Line2Id, const QString &pointName, const qreal &mx, const qreal &my,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
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
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void SetFactor(qreal factor);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief RemoveReferens
     */
    virtual void RemoveReferens();
private:
    /**
     * @brief p1Line1
     */
    qint64       p1Line1;
    /**
     * @brief p2Line1
     */
    qint64       p2Line1;
    /**
     * @brief p1Line2
     */
    qint64       p1Line2;
    /**
     * @brief p2Line2
     */
    qint64       p2Line2;
    /**
     * @brief dialogLineIntersect
     */
    QSharedPointer<DialogLineIntersect> dialogLineIntersect;
};

#endif // VTOOLLINEINTERSECT_H
