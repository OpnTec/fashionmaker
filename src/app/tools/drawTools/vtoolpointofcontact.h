/************************************************************************
 **
 **  @file   vtoolpointofcontact.h
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

#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

#include "vtoolpoint.h"

class VFormula;

/**
 * @brief The VToolPointOfContact class tool for creation point intersection line and arc.
 */
class VToolPointOfContact : public VToolPoint
{
    Q_OBJECT
public:
    VToolPointOfContact(VPattern *doc, VContainer *data, const quint32 &id, const QString &arcRadius,
                        const quint32 &center, const quint32 &firstPointId, const quint32 &secondPointId,
                        const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void   setDialog();
    static QPointF FindPoint(const qreal &arcRadius, const QPointF &center, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    static VToolPointOfContact*    Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc,
                                          VContainer *data);
    static VToolPointOfContact*    Create(const quint32 _id, QString &arcRadius, const quint32 &center,
                          const quint32 &firstPointId, const quint32 &secondPointId, const QString &pointName,
                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                          VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int    type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfContact) };

    VFormula getArcRadius() const;
    void     setArcRadius(const VFormula &value);

    quint32 getCenter() const;
    void    setCenter(const quint32 &value);

    quint32 GetFirstPointId() const;
    void    SetFirstPointId(const quint32 &value);

    quint32 GetSecondPointId() const;
    void    SetSecondPointId(const quint32 &value);

    virtual void   ShowVisualization(bool show);
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   SetFactor(qreal factor);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   RemoveReferens();
    virtual void   SaveDialog(QDomElement &domElement);
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void   ReadToolAttributes(const QDomElement &domElement);
    virtual void   SetVisualization();
private:
    /** @brief radius string with formula radius arc. */
    QString        arcRadius;

    /** @brief center id center arc point. */
    quint32        center;

    /** @brief firstPointId id first line point. */
    quint32        firstPointId;

    /** @brief secondPointId id second line point. */
    quint32        secondPointId;
};

#endif // VTOOLPOINTOFCONTACT_H
