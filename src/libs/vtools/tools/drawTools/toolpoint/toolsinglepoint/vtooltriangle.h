/************************************************************************
 **
 **  @file   vtooltriangle.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

#include "vtoolsinglepoint.h"

/**
 * @brief The VToolTriangle class for tool that find point intersection two foots right triangle
 * (triangle with 90 degree).
 */
class VToolTriangle : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() Q_DECL_OVERRIDE;
    static VToolTriangle *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                 VContainer *data);
    static VToolTriangle *Create(const quint32 _id, const QString &pointName, const quint32 &axisP1Id,
                                 const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                                 const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data, const Document &parse, const Source &typeCreation);
    static QPointF FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    static const QString ToolType;
    virtual int    type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Triangle)};

    quint32 GetAxisP1Id() const;
    void    SetAxisP1Id(const quint32 &value);

    quint32 GetAxisP2Id() const;
    void    SetAxisP2Id(const quint32 &value);

    quint32 GetFirstPointId() const;
    void    SetFirstPointId(const quint32 &value);

    quint32 GetSecondPointId() const;
    void    SetSecondPointId(const quint32 &value);

    virtual void   ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void   RemoveReferens() Q_DECL_OVERRIDE;
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void   SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void   ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void   SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolTriangle)
    /** @brief axisP1Id id first axis point. */
    quint32        axisP1Id;

    /** @brief axisP2Id id second axis point. */
    quint32        axisP2Id;

    /** @brief firstPointId id first triangle point, what lies on the hypotenuse. */
    quint32        firstPointId;

    /** @brief secondPointId id second triangle point, what lies on the hypotenuse. */
    quint32        secondPointId;

    VToolTriangle(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &axisP1Id,
                  const quint32 &axisP2Id,
                  const quint32 &firstPointId, const quint32 &secondPointId, const Source &typeCreation,
                  QGraphicsItem * parent = nullptr);
};

#endif // VTOOLTRIANGLE_H
