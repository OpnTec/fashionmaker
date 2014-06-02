/************************************************************************
 **
 **  @file   vtoolcutsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef VTOOLCUTSPLINEPATH_H
#define VTOOLCUTSPLINEPATH_H

#include "vtoolpoint.h"
#include "../../widgets/vsimplespline.h"

/**
 * @brief The VToolCutSplinePath class for tool CutSplinePath. This tool find point on splinePath and cut splinePath on
 * two.
 */
class VToolCutSplinePath : public VToolPoint
{
    Q_OBJECT
public:
    /**
     * @brief VToolCutSplinePath constructor.
     * @param doc dom document container.
     * @param data container with variables.
     * @param id object id in container.
     * @param formula string with formula length first splinePath.
     * @param splinePathId id splinePath (we cut this splinePath) in data container.
     * @param splPath1id id first splinePath after cutting.
     * @param splPath2id id second splinePath after cutting.
     * @param typeCreation way we create this tool.
     * @param parent parent object.
     */
    VToolCutSplinePath(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                       const quint32 &splinePathId, const quint32 &splPath1id, const quint32 &splPath2id,
                       const Valentina::Sources &typeCreation, QGraphicsItem * parent = nullptr);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    /**
     * @brief Create help create tool form GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void  Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName point name.
     * @param formula string with formula length first splinePath.
     * @param splinePathId id of splinePath in data container.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &splinePathId,
                        const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                        const Document::Documents &parse, const Valentina::Sources &typeCreation);
    static const QString ToolType;
    static const QString AttrSplinePath;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void  FullUpdateFromFile();
    /**
     * @brief SplineChoosed send signal about selection splinePath.
     * @param id object id in container.
     */
    void          SplineChoosed(quint32 id);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void  ChangedActivDraw(const QString &newName);
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void  ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void  AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void  RefreshDataInFile();
    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    void          RefreshGeometry();
    /**
     * @brief RemoveReferens decrement referens value for used objects.
     */
    virtual void  RemoveReferens();
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void  SaveDialog(QDomElement &domElement);
private:
    Q_DISABLE_COPY(VToolCutSplinePath)
    /**
     * @brief formula keep formula of length
     */
    QString       formula;
    /**
     * @brief splineId keep id of spline.
     */
    quint32        splinePathId;
    /**
     * @brief firstSpline first splinePath after cutting.
     */
    VSimpleSpline *firstSpline;
    /**
     * @brief secondSpline second splinePath after cutting.
     */
    VSimpleSpline *secondSpline;
    /**
     * @brief splPath1id id first splinePath after cutting.
     */
    const quint32  splPath1id;
    /**
     * @brief splPath2id id second splinePath after cutting.
     */
    const quint32  splPath2id;
    /**
     * @brief RefreshSpline refresh splinePath on scene.
     * @param spline splinePath.
     * @param splPathid splinePath id.
     * @param tr splineType type.
     */
    void          RefreshSpline(VSimpleSpline *spline, quint32 splPathid, SimpleSpline::Translation tr);
};

#endif // VTOOLCUTSPLINEPATH_H
