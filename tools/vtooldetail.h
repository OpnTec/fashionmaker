/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#ifndef VTOOLDETAIL_H
#define VTOOLDETAIL_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>
#include "dialogs/dialogdetail.h"

class VToolDetail: public VAbstractTool, public QGraphicsPathItem{
    Q_OBJECT
public:
                                   VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id,
                                               Tool::Sources typeCreation, VMainGraphicsScene *scene,
                                               QGraphicsItem * parent = 0);
    virtual void                   setDialog();
    static void                    Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene,
                                          VDomDocument *doc, VContainer *data);
    static void                    Create(const qint64 _id, VDetail &newDetail, VMainGraphicsScene  *scene,
                                          VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                          Tool::Sources typeCreation);
    template <typename T>
    void AddTool(T *tool, const qint64 &id, Tool::Tools typeTool){
        tool->setParentItem(this);
        connect(tool, &T::ChoosedTool, sceneDetails, &VMainGraphicsScene::ChoosedItem);
        VNodeDetail node(id, typeTool, Draw::Modeling, NodeDetail::Modeling);
        VDetail det = VAbstractTool::data.GetDetail(this->id);
        det.append(node);
        VAbstractTool::data.UpdateDetail(this->id, det);
        QDomElement domElement = doc->elementById(QString().setNum(this->id));
        if(domElement.isElement()){
            AddNode(domElement, node);
        }
    }
    static const QString           TagName;
    static const QString           TagNode;
    static const QString           AttrSupplement;
    static const QString           AttrClosed;
    static const QString           AttrWidth;
    static const QString           AttrIdObject;
    static const QString           AttrNodeType;
    static const QString           NodeTypeContour;
    static const QString           NodeTypeModeling;
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   FullUpdateFromGui(int result);
signals:
    void                           RemoveTool(QGraphicsItem *tool);
protected:
    virtual void                   AddToFile ();
    QVariant                       itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                   RemoveReferens();
private:
    Q_DISABLE_COPY(VToolDetail)
    QSharedPointer<DialogDetail>   dialogDetail;
    VMainGraphicsScene             *sceneDetails;
    void                           RefreshGeometry ();
    void                           AddNode(QDomElement &domElement, VNodeDetail &node);
    template <typename Tool>
    void                           InitTool(VMainGraphicsScene *scene, const VNodeDetail &node);
};

#endif // VTOOLDETAIL_H
