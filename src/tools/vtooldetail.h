/************************************************************************
 **
 **  @file   vtooldetail.h
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

#ifndef VTOOLDETAIL_H
#define VTOOLDETAIL_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>
#include "../dialogs/tools/dialogdetail.h"
#include "../dialogs/tools/dialogtool.h"
/**
 * @brief The VToolDetail class for working with detail.
 */
class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    /**
     * @brief VToolDetail constructor.
     * @param doc dom document container
     * @param data container with variables
     * @param id object id in container
     * @param typeCreation way we create this tool.
     * @param scene pointer to scene.
     * @param parent parent object
     */
    VToolDetail(VPattern *doc, VContainer *data, const quint32 &id, const Valentina::Sources &typeCreation,
                VMainGraphicsScene *scene, QGraphicsItem * parent = nullptr);
    ~VToolDetail();
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void       setDialog();
    template<typename T>
    /**
     * @brief CreateNode create new node for detail.
     * @param data container.
     * @param id id parent object.
     * @return id for new object.
     */
    static quint32 CreateNode(VContainer *data, const quint32 &id)
    {
        //We can't use exist object. Need create new.
        T *node = new T(*data->GeometricObject<const T *>(id));
        node->setMode(Valentina::Modeling);
        return data->AddGObject(node);
    }

    /**
     * @brief Create help create tool from GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void        Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param newDetail detail what we want show.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void        Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene  *scene,
                              VPattern *doc, VContainer *data, const Document::Documents &parse,
                              const Valentina::Sources &typeCreation);
    static const QString TagName;
    static const QString TagNode;
    static const QString AttrSupplement;
    static const QString AttrClosed;
    static const QString AttrWidth;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;
    static const QString NodeArc;
    static const QString NodePoint;
    static const QString NodeSpline;
    static const QString NodeSplinePath;
    /**
     * @brief Remove full delete detail.
     */
    void               Remove();
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void       FullUpdateFromFile ();
    /**
     * @brief FullUpdateFromGui refresh tool data after change in options.
     * @param result keep result working dialog.
     */
    virtual void       FullUpdateFromGui(int result);
protected:
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void       AddToFile ();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void       RefreshDataInFile();
    /**
     * @brief itemChange handle detail change.
     * @param change change
     * @param value value
     * @return new value.
     */
    virtual QVariant   itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief mouseReleaseEvent handle mouse release events.
     * @param event mouse release event.
     */
    virtual void       mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void       contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void       RemoveReferens();
    /**
     * @brief keyReleaseEvent handle key release events.
     * @param event key release event.
     */
    virtual void       keyReleaseEvent(QKeyEvent * event);
private:
    Q_DISABLE_COPY(VToolDetail)
    /**
     * @brief dialog dialog options.
     */
    DialogTool         *dialog;
    /**
     * @brief sceneDetails pointer to the scene.
     */
    VMainGraphicsScene *sceneDetails;
    /**
     * @brief RefreshGeometry refresh item on scene.
     */
    void               RefreshGeometry ();
    /**
     * @brief AddNode add node to the file.
     * @param domElement tag in xml tree.
     * @param node node of detail.
     */
    void               AddNode(QDomElement &domElement, const VNodeDetail &node);
    template <typename Tool>
    /**
     * @brief InitTool initial node item on scene
     * @param scene pointer to scene.
     * @param node node of detail.
     */
    void               InitTool(VMainGraphicsScene *scene, const VNodeDetail &node);
};

#endif // VTOOLDETAIL_H
