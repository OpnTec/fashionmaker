/************************************************************************
 **
 **  @file   vtoolcutarc.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   7 1, 2014
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

#ifndef VTOOLCUTARC_H
#define VTOOLCUTARC_H

#include "vtoolpoint.h"
#include "../../widgets/vsimplearc.h"

/**
 * @brief The VToolCutArc class tool for cutting arc.
 */
class VToolCutArc : public VToolPoint
{
    Q_OBJECT
public:
    /**
     * @brief VToolCutArc constructor.
     * @param doc dom document container.
     * @param data container with variables.
     * @param id object id in container.
     * @param formula string with formula length first arc.
     * @param arcId id arc in data container.
     * @param arc1id id first cutting arc.
     * @param arc2id id second cutting arc.
     * @param typeCreation way we create this tool.
     * @param parent parent object.
     */
    VToolCutArc(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                const quint32 &arcId, const quint32 &arc1id, const quint32 &arc2id,
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
    static void  Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName point name.
     * @param formula string with formula length first arc.
     * @param arcId id arc in data container.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const quint32 _id, const QString &pointName, const QString &formula, const quint32 &arcId,
                        const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                        VContainer *data, const Document::Documents &parse, const Valentina::Sources &typeCreation);
    static const QString ToolType;
    static const QString AttrArc;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief ArcChoosed send signal about selection from cutted arc.
     * @param id object id in container.
     */
    void         ArcChoosed(quint32 id);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void ChangedActivDraw(const QString &newName);
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
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
     * @brief RefreshGeometry  refresh item on scene.
     */
    void         RefreshGeometry();
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void SaveDialog(QDomElement &domElement);
private:
    Q_DISABLE_COPY(VToolCutArc)
    /**
     * @brief formula keep formula of length
     */
    QString      formula;
    /**
     * @brief arcId keep id of arc
     */
    quint32       arcId;
    /**
     * @brief firstArc first arc after cutting.
     */
    VSimpleArc   *firstArc;
    /**
     * @brief secondArc second arc after cutting.
     */
    VSimpleArc   *secondArc;
    /**
     * @brief arc1id id first arc after cutting.
     */
    const quint32 arc1id;
    /**
     * @brief arc2id id second arc after cutting.
     */
    const quint32 arc2id;
    /**
     * @brief RefreshArc refresh arc on scene.
     * @param sArc arc.
     * @param arcid arc id.
     * @param tr arc type.
     */
    void         RefreshArc(VSimpleArc *sArc, quint32 arcid, SimpleArc::Translation tr);
};

#endif // VTOOLCUTARC_H
