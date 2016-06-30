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
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog() {}
    static VToolUnionDetails *Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                     VContainer *data);
    static VToolUnionDetails *Create(const quint32 _id, const VDetail &d1,  const VDetail &d2, const quint32 &d1id,
                                     const quint32 &d2id, const quint32 &indexD1, const quint32 &indexD2,
                                     VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                     const Document &parse,
                                     const Source &typeCreation);
    static void  PointsOnEdge(const VDetail &d, const quint32 &index, VPointF &p1, VPointF &p2, VContainer *data);
    static void  FindIndexJ(const qint32 &pointsD2, const VDetail &d2, const quint32 &indexD2, qint32 &j);
    static QVector<VDetail> GetDetailFromFile(VAbstractPattern *doc, const QDomElement &domElement);

    static const QString ToolType;
    static const QString TagDetail;
    static const QString TagNode;
    static const QString TagChildren;
    static const QString TagChild;
    static const QString AttrIndexD1;
    static const QString AttrIndexD2;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;
    static void  AddToNewDetail(VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                VDetail &newDetail, const VDetail &det, const int &i, const quint32 &idTool,
                                QVector<quint32> &children, const QString &drawName, const qreal &dx = 0,
                                const qreal &dy = 0, const quint32 &pRotate = 0, const qreal &angle = 0);
    static void  UpdatePoints(VContainer *data, const VDetail &det, const int &i,
                              QVector<quint32> &children, const qreal &dx = 0, const qreal &dy = 0,
                              const quint32 &pRotate = 0, const qreal &angle = 0);
    static void  BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                 const qreal &angle);
    virtual QString getTagName() const Q_DECL_OVERRIDE;
    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
    virtual void incrementReferens() Q_DECL_OVERRIDE;
    virtual void decrementReferens() Q_DECL_OVERRIDE;
    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile () Q_DECL_OVERRIDE {}
    virtual void AllowHover(bool) Q_DECL_OVERRIDE {}
    virtual void AllowSelecting(bool) Q_DECL_OVERRIDE {}
protected:
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE {}
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

    QString      drawName;

    VToolUnionDetails(VAbstractPattern *doc, VContainer *data, const quint32 &id, const VDetail &d1, const VDetail &d2,
                      const quint32 &indexD1, const quint32 &indexD2, const Source &typeCreation,
                      const QString &drawName, QObject *parent = nullptr);

    void         AddDetail(QDomElement &domElement, VDetail &d);
    void         AddNode(QDomElement &domElement, const VNodeDetail &node);
    QDomNode     UpdateDetail(const QDomNode &domNode, const VDetail &d);
    void         AddToModeling(const QDomElement &domElement);
    void         IncrementReferences(const VDetail &d) const;
    void         DecrementReferences(const VDetail &d) const;

    static void             SaveChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children);
    static QVector<quint32> AllChildren(VAbstractPattern *doc, quint32 id);
    static quint32          TakeNextId(QVector<quint32> &children);
    static QString          DrawName(VAbstractPattern *doc, quint32 d1id, quint32 d2id);
};

#endif // VTOOLUNIONDETAILS_H
