/************************************************************************
 **
 **  @file   vtoolcutspline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   15 12, 2013
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

#ifndef VTOOLCUTSPLINE_H
#define VTOOLCUTSPLINE_H

#include "vtoolpoint.h"
#include "../../dialogs/dialogcutspline.h"
#include "../../widgets/vsimplespline.h"

/**
 * @brief The VToolCutSpline class for tool CutSpline. This tool find point on spline and cut spline on two.
 */
class VToolCutSpline : public VToolPoint
{
    Q_OBJECT
public:
    /**
     * @brief VToolCutSpline
     * @param doc
     * @param data container with variables
     * @param id object id in container
     * @param formula
     * @param splineId
     * @param typeCreation way we create this tool.
     * @param parent parent object
     */
    VToolCutSpline(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &formula,
                   const qint64 &splineId, const qint64 &spl1id, const qint64 &spl2id,
                   const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    /**
     * @brief Create help create tool
     * @param dialog
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     */
    static void  Create(QSharedPointer<DialogCutSpline> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create help create tool
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName
     * @param formula
     * @param splineId
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const qint64 _id, const QString &pointName,
                        const QString &formula, const qint64 &splineId, const qreal &mx, const qreal &my,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
    static const QString AttrSpline;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui  refresh tool data from change options.
     * @param result
     */
    virtual void FullUpdateFromGui(int result);
    /**
     * @brief SplineChoosed
     * @param id object id in container
     */
    void SplineChoosed(qint64 id);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName
     */
    virtual void      ChangedActivDraw(const QString &newName);
    virtual void      ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief RefreshGeometry
     */
    void         RefreshGeometry();
    virtual void RemoveReferens();
private:
    Q_DISABLE_COPY(VToolCutSpline)
    /**
     * @brief formula keep formula of length
     */
    QString           formula;
    /**
     * @brief splineId keep id of spline
     */
    qint64            splineId;
    /**
     * @brief DialogCutSpline pointer to the tool's dialog
     */
    QSharedPointer<DialogCutSpline> dialogCutSpline;
    /**
     * @brief firstSpline
     */
    VSimpleSpline *firstSpline;
    /**
     * @brief secondSpline
     */
    VSimpleSpline *secondSpline;
    const qint64 spl1id;
    const qint64 spl2id;
    void RefreshSpline(VSimpleSpline *spline, qint64 splid, SimpleSpline::Translation tr);
};

#endif // VTOOLCUTSPLINE_H
