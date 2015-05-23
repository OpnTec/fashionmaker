/************************************************************************
 **
 **  @file   vtooluniondetails.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../xml/vpattern.h"

class VPointF;
class VMainGraphicsScene;
class DialogTool;

/**
 * @brief The VToolUnionDetails class tool union details.
 */
class VToolUnionDetails : public VAbstractTool
{
    Q_OBJECT
public:
    VToolUnionDetails(VPattern *doc, VContainer *data, const quint32 &id, const VDetail &d1, const VDetail &d2,
                      const quint32 &indexD1, const quint32 &indexD2, const Source &typeCreation,
                      QObject *parent = nullptr);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog() {}
    static VToolUnionDetails *Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    static VToolUnionDetails *Create(const quint32 _id, const VDetail &d1,  const VDetail &d2, const quint32 &d1id,
                                     const quint32 &d2id, const quint32 &indexD1, const quint32 &indexD2,
                                     VMainGraphicsScene *scene, VPattern *doc, VContainer *data, const Document &parse,
                                     const Source &typeCreation);
    static void  PointsOnEdge(const VDetail &d, const quint32 &index, VPointF &p1, VPointF &p2, VContainer *data);
    static void  FindIndexJ(const qint32 &pointsD2, const VDetail &d2, const quint32 &indexD2, qint32 &j);
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
    static void  AddToNewDetail(QObject *tool, VPattern *doc, VContainer *data, VDetail &newDetail,
                                const VDetail &det, const int &i, const quint32 &idTool, const qreal &dx = 0,
                                const qreal &dy = 0, const quint32 &pRotate = 0, const qreal &angle = 0);
    static void  UpdatePoints(const quint32 &idDetail, VContainer *data, const VDetail &det, const int &i,
                              quint32 &idCount, const qreal &dx = 0, const qreal &dy = 0, const quint32 &pRotate = 0,
                              const qreal &angle = 0);
    static void  BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                 const qreal &angle);
    virtual QString getTagName() const;
    virtual void ShowVisualization(bool show);
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile (){}
protected:
    virtual void AddToFile();
    virtual void RefreshDataInFile();
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void SetVisualization() {}
private:
    Q_DISABLE_COPY(VToolUnionDetails)
    /** @brief d1 first detail. */
    VDetail      d1;

    /** @brief d2 second detail. */
    VDetail      d2;

    /** @brief indexD1 index edge in first detail. */
    quint32      indexD1;

    /** @brief indexD2 index edge in second detail. */
    quint32      indexD2;

    void         AddDetail(QDomElement &domElement, VDetail &d);
    void         AddNode(QDomElement &domElement, const VNodeDetail &node);
    QDomNode     UpdateDetail(const QDomNode &domNode, const VDetail &d);
    void         AddToModeling(const QDomElement &domElement);
};

#endif // VTOOLUNIONDETAILS_H
