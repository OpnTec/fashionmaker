/************************************************************************
 **
 **  @file   vtoolbisector.h
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

#ifndef VTOOLBISECTOR_H
#define VTOOLBISECTOR_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolBisector class tool for creation bisector point.
 */
class VToolBisector : public VToolLinePoint
{
    Q_OBJECT
public:

    VToolBisector(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine, const QString &lineColor,
                  const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                  const quint32 &thirdPointId, const Source &typeCreation, QGraphicsItem * parent = nullptr);
    static qreal   BisectorAngle(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint);
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint,
                             const qreal& length);
    virtual void   setDialog();
    static VToolBisector* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolBisector* Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                                 const quint32 &secondPointId, const quint32 &thirdPointId, const QString &typeLine,
                                 const QString &lineColor, const QString &pointName, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene  *scene, VPattern *doc, VContainer *data, const Document &parse,
                                 const Source &typeCreation);
    static const QString ToolType;
    virtual int    type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Bisector)};

    quint32 GetFirstPointId() const;
    void    SetFirstPointId(const quint32 &value);

    quint32 GetThirdPointId() const;
    void    SetThirdPointId(const quint32 &value);

    virtual void   ShowVisualization(bool show);
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   SetFactor(qreal factor);
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   RemoveReferens();
    virtual void   SaveDialog(QDomElement &domElement);
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void   ReadToolAttributes(const QDomElement &domElement);
private:
    /** @brief firstPointId id first point of angle. */
    quint32         firstPointId;

    /** @brief thirdPointId id third point of angle. */
    quint32         thirdPointId;
};

#endif // VTOOLBISECTOR_H
