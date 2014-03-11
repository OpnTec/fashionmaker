/************************************************************************
 **
 **  @file   vtooluniondetails.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   26 12, 2013
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

#ifndef VTOOLUNIONDETAILS_H
#define VTOOLUNIONDETAILS_H

#include "vabstracttool.h"
#include "../dialogs/tools/dialoguniondetails.h"

/**
 * @brief The VToolUnionDetails class tool union details.
 */
class VToolUnionDetails : public VAbstractTool
{
    Q_OBJECT
public:
    /**
     * @brief VToolUnionDetails costructor.
     * @param doc dom document container.
     * @param data dom document container.
     * @param id object id in container.
     * @param d1 first detail.
     * @param d2 second detail.
     * @param indexD1 index edge in first detail.
     * @param indexD2 index edge in second detail.
     * @param typeCreation way we create this tool.
     * @param parent parent object.
     */
    VToolUnionDetails(VPattern *doc, VContainer *data, const quint32 &id, const VDetail &d1, const VDetail &d2,
                      const quint32 &indexD1, const quint32 &indexD2, const Valentina::Sources &typeCreation,
                      QObject *parent = nullptr);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog() {}
    /**
     * @brief Create help create tool from GUI.
     * @param dialog dialog.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void  Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param d1 first detail.
     * @param d2 second detail.
     * @param d1id id first detail.
     * @param d2id id second detail.
     * @param indexD1 index edge in first detail.
     * @param indexD2 index edge in second detail.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const quint32 _id, const VDetail &d1,  const VDetail &d2, const quint32 &d1id,
                        const quint32 &d2id, const quint32 &indexD1, const quint32 &indexD2, VMainGraphicsScene *scene,
                        VPattern *doc, VContainer *data, const Document::Documents &parse,
                        const Valentina::Sources &typeCreation);
    static void  PointsOnEdge(const VDetail &d, const qint32 &index, VPointF &p1, VPointF &p2, VContainer *data);
    static void FindJ(const qint32 &pointsD2, const VDetail &d2, const qint32 &indexD2, qint32 &j);
    /**
     * @brief GetDetailFromFile parse detail from file.
     * @param doc dom document container.
     * @param domElement tag in xml tree.
     * @return detail stored in file.
     */
    static QVector<VDetail> GetDetailFromFile(VPattern *doc, const QDomElement &domElement);
    static const QString TagName;
    static const QString ToolType;
    static const QString TagDetail;
    static const QString TagNode;
    static const QString AttrIndexD1;
    static const QString AttrIndexD2;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;
    /**
     * @brief AddToNewDetail create united detail adding one node per time.
     * @param tool tool that make union.
     * @param doc dom document container.
     * @param data container with variables.
     * @param newDetail united detail.
     * @param det detail what we union.
     * @param i index node in detail.
     * @param idTool id tool union details.
     * @param dx bias node x axis.
     * @param dy bias node y axis.
     * @param pRotate point rotation.
     * @param angle angle rotation.
     */
    static void  AddToNewDetail(QObject *tool, VPattern *doc, VContainer *data, VDetail &newDetail,
                                const VDetail &det, const ptrdiff_t &i, const quint32 &idTool, const qreal &dx = 0,
                                const qreal &dy = 0, const quint32 &pRotate = 0, const qreal &angle = 0);
    /**
     * @brief UpdatePoints update data for united details.
     * @param idDetail id united detail.
     * @param data container with variables.
     * @param det detail what we union.
     * @param i index node in detail.
     * @param idCount
     * @param dx bias node x axis.
     * @param dy bias node y axis.
     * @param pRotate point rotation.
     * @param angle angle rotation.
     */
    static void  UpdatePoints(const quint32 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &i,
                              quint32 &idCount, const qreal &dx = 0, const qreal &dy = 0, const quint32 &pRotate = 0,
                              const qreal &angle = 0);
    /**
     * @brief BiasRotatePoint bias and rotate point.
     * @param point point.
     * @param dx bias x axis.
     * @param dy bias y axis.
     * @param pRotate point rotation.
     * @param angle angle rotation.
     */
    static void  BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                 const qreal angle);
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile (){}
protected:
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
private:
    Q_DISABLE_COPY(VToolUnionDetails)
    /**
     * @brief d1 first detail.
     */
    VDetail      d1;
    /**
     * @brief d2 second detail.
     */
    VDetail      d2;
    /**
     * @brief indexD1 index edge in first detail.
     */
    quint32       indexD1;
    /**
     * @brief indexD2 index edge in second detail.
     */
    quint32       indexD2;
    /**
     * @brief AddDetail add detail to xml file.
     * @param domElement tag in xml tree.
     * @param d detail.
     */
    void         AddDetail(QDomElement &domElement, VDetail &d);
    /**
     * @brief AddNode add node to xml file.
     * @param domElement tag in xml tree.
     * @param node node.
     */
    void         AddNode(QDomElement &domElement, const VNodeDetail &node);
    /**
     * @brief UpdateDetail update detail in xml tree.
     * @param domNode dom node.
     * @param d detail.
     * @return return next detail tag in xml tree if exist.
     */
    QDomNode     UpdateDetail(const QDomNode &domNode, const VDetail &d);
    /**
     * @brief AddToModeling add tool to xml tree.
     * @param domElement tag in xml tree.
     */
    void         AddToModeling(const QDomElement &domElement);
};

#endif // VTOOLUNIONDETAILS_H
