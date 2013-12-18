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
     * @param data
     * @param id
     * @param formula
     * @param splineId
     * @param typeCreation
     * @param parent
     */
    VToolCutSpline(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &formula,
                   const qint64 &splineId, const qint64 &spl1id, const qint64 &spl2id,
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
    static void  Create(QSharedPointer<DialogCutSpline> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param pointName
     * @param formula
     * @param splineId
     * @param mx
     * @param my
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
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
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui(int result);
    /**
     * @brief SplineChoosed
     * @param id
     */
    void SplineChoosed(qint64 id);
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void      ChangedActivDraw(const QString &newName);
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
    void      RefreshGeometry();
private:
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
