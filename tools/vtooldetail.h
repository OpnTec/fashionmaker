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
#include "../dialogs/dialogdetail.h"

/**
 * @brief The VToolDetail class
 */
class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                   /**
                                    * @brief VToolDetail
                                    * @param doc dom document container
                                    * @param data
                                    * @param id
                                    * @param typeCreation
                                    * @param scene
                                    * @param parent
                                    */
                                   VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id,
                                               const Tool::Sources &typeCreation, VMainGraphicsScene *scene,
                                               QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void                   setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data
     */
    static void                    Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene,
                                          VDomDocument *doc, VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param newDetail
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void                    Create(const qint64 _id, VDetail &newDetail, VMainGraphicsScene  *scene,
                                          VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                          const Tool::Sources &typeCreation);
    template <typename T>
    /**
     * @brief AddTool
     * @param tool
     * @param id
     * @param typeTool
     */
    void AddTool(T *tool, const qint64 &id, Tool::Tools typeTool)
    {
        tool->setParentItem(this);
        connect(tool, &T::ChoosedTool, sceneDetails, &VMainGraphicsScene::ChoosedItem);
        VNodeDetail node(id, typeTool, Draw::Modeling, NodeDetail::Modeling);
        VDetail det = VAbstractTool::data.GetDetail(this->id);
        det.append(node);
        VAbstractTool::data.UpdateDetail(this->id, det);
        QDomElement domElement = doc->elementById(QString().setNum(this->id));
        if (domElement.isElement())
        {
            AddNode(domElement, node);
        }
    }
    /**
     * @brief TagName
     */
    static const QString           TagName;
    /**
     * @brief TagNode
     */
    static const QString           TagNode;
    /**
     * @brief AttrSupplement
     */
    static const QString           AttrSupplement;
    /**
     * @brief AttrClosed
     */
    static const QString           AttrClosed;
    /**
     * @brief AttrWidth
     */
    static const QString           AttrWidth;
    /**
     * @brief AttrIdObject
     */
    static const QString           AttrIdObject;
    /**
     * @brief AttrNodeType
     */
    static const QString           AttrNodeType;
    /**
     * @brief NodeTypeContour
     */
    static const QString           NodeTypeContour;
    /**
     * @brief NodeTypeModeling
     */
    static const QString           NodeTypeModeling;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void                   FullUpdateFromFile ();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void                   FullUpdateFromGui(int result);
signals:
    /**
     * @brief RemoveTool
     * @param tool
     */
    void                           RemoveTool(QGraphicsItem *tool);
protected:
    /**
     * @brief AddToFile
     */
    virtual void                   AddToFile ();
    /**
     * @brief itemChange
     * @param change
     * @param value
     * @return
     */
    QVariant                       itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void                   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief RemoveReferens
     */
    virtual void                   RemoveReferens();
private:
    Q_DISABLE_COPY(VToolDetail)
    /**
     * @brief dialogDetail
     */
    QSharedPointer<DialogDetail>   dialogDetail;
    /**
     * @brief sceneDetails
     */
    VMainGraphicsScene             *sceneDetails;
    /**
     * @brief RefreshGeometry
     */
    void                           RefreshGeometry ();
    /**
     * @brief AddNode
     * @param domElement
     * @param node
     */
    void                           AddNode(QDomElement &domElement, VNodeDetail &node);
    template <typename Tool>
    /**
     * @brief InitTool
     * @param scene
     * @param node
     */
    void                           InitTool(VMainGraphicsScene *scene, const VNodeDetail &node);
};

#endif // VTOOLDETAIL_H
