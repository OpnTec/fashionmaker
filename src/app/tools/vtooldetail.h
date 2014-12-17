/************************************************************************
 **
 **  @file   vtooldetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../xml/vpattern.h"

class VMainGraphicsScene;
class DialogTool;

/**
 * @brief The VToolDetail class for working with detail.
 */
class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDetail(VPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                VMainGraphicsScene *scene, QGraphicsItem * parent = nullptr);
    ~VToolDetail();

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
        T *node = new T(*data->GeometricObject<T>(id).data());
        node->setMode(Draw::Modeling);
        return data->AddGObject(node);
    }

    static void        Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    static void        Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene  *scene,
                              VPattern *doc, VContainer *data, const Document &parse,
                              const Source &typeCreation);
    static const QString TagName;
    static const QString TagNode;
    static const QString AttrSupplement;
    static const QString AttrClosed;
    static const QString AttrWidth;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString AttrReverse;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;
    static const QString NodeArc;
    static const QString NodePoint;
    static const QString NodeSpline;
    static const QString NodeSplinePath;
    void               Remove(bool ask);
    static void        AddNode(VPattern *doc, QDomElement &domElement, const VNodeDetail &node);
    virtual int        type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Detail)};
    virtual QString    getTagName() const;
    virtual void       ShowVisualization(bool show);
public slots:
    virtual void       FullUpdateFromFile ();
    virtual void       FullUpdateFromGuiOk(int result);
protected:
    virtual void       AddToFile ();
    virtual void       RefreshDataInFile();
    virtual QVariant   itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void       mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void       contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void       RemoveReferens();
    virtual void       keyReleaseEvent(QKeyEvent * event);
    virtual void       SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    Q_DISABLE_COPY(VToolDetail)
    /** @brief dialog dialog options. */
    DialogTool         *dialog;

    /** @brief sceneDetails pointer to the scene. */
    VMainGraphicsScene *sceneDetails;

    void               RefreshGeometry ();
    template <typename Tool>
    /**
     * @brief InitTool initial node item on scene
     * @param scene pointer to scene.
     * @param node node of detail.
     */
    void               InitTool(VMainGraphicsScene *scene, const VNodeDetail &node);
    virtual void       DeleteTool(bool ask = true);
};

#endif // VTOOLDETAIL_H
