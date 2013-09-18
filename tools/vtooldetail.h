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
 **  Tox is distributed in the hope that it will be useful,
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

class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, VDetail &oldDetail,
                Tool::Enum typeCreation, Document::Enum parse, VMainGraphicsScene *scene,
                QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                       VContainer *data);
    static void Create(const qint64 _id, VDetail &newDetail, VDetail &oldDetail,
                       VMainGraphicsScene  *scene,
                       VDomDocument *doc, VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
    template <typename Tool>
    void AddTool(Tool *tool, const qint64 &id, Tools::Enum typeTool){
        tool->setParentItem(this);
        connect(tool, &Tool::ChoosedTool, sceneDetails, &VMainGraphicsScene::ChoosedItem);
        VNodeDetail node(id, typeTool, Draw::Modeling, NodeDetail::Modeling);
        VDetail det = VAbstractTool::data.GetDetail(this->id);
        det.append(node);
        VAbstractTool::data.UpdateDetail(this->id, det);
        QDomElement domElement = doc->elementById(QString().setNum(this->id));
        if(domElement.isElement()){
            AddNode(domElement, node);
        }
    }
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   FullUpdateFromGui(int result);
protected:
    virtual void                   AddToFile ();
    QVariant                       itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
private:
    QSharedPointer<DialogDetail>   dialogDetail;
    VMainGraphicsScene             *sceneDetails;
    void                           RefreshGeometry ();
    void AddNode(QDomElement &domElement, VNodeDetail &node);
                                   VToolDetail(const VToolDetail &tool);
    VToolDetail                    &operator=(const VToolDetail &tool);
};

#endif // VTOOLDETAIL_H
