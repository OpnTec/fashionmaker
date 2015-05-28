/************************************************************************
 **
 **  @file   vtoolpointofintersectionarcs.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VTOOLPOINTOFINTERSECTIONARCS_H
#define VTOOLPOINTOFINTERSECTIONARCS_H

#include "vtoolpoint.h"
#include "../../xml/vpattern.h"

class VArc;

class VToolPointOfIntersectionArcs : public VToolPoint
{
    Q_OBJECT

public:
    VToolPointOfIntersectionArcs(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &firstArcId,
                                 const quint32 &secondArcId, CrossArcsPoint crossPoint, const Source &typeCreation,
                                 QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolPointOfIntersectionArcs *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc,
                                                VContainer *data);
    static VToolPointOfIntersectionArcs *Create(const quint32 _id, const QString &pointName, const quint32 &firstArcId,
                                                const quint32 &secondArcId, CrossArcsPoint crossPoint,
                                                const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                VPattern *doc, VContainer *data, const Document &parse,
                                                const Source &typeCreation);
    static QPointF FindPoint(const VArc *arc1, const VArc *arc2, const CrossArcsPoint crossPoint);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionArcs) };

    quint32 GetFirstArcId() const;
    void    SetFirstArcId(const quint32 &value);

    quint32 GetSecondArcId() const;
    void    SetSecondArcId(const quint32 &value);

    CrossArcsPoint GetCrossArcsPoint() const;
    void           SetCrossArcsPoint(CrossArcsPoint &value);

    virtual void ShowVisualization(bool show);
public slots:
    virtual void FullUpdateFromFile();
protected:
    virtual void RemoveReferens();
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void ReadToolAttributes(const QDomElement &domElement);
    virtual void SetVisualization();
private:
    Q_DISABLE_COPY(VToolPointOfIntersectionArcs)

    /** @brief firstArcId id first arc. */
    quint32       firstArcId;

    /** @brief secondArcId id second arc. */
    quint32       secondArcId;

    CrossArcsPoint crossPoint;
};

#endif // VTOOLPOINTOFINTERSECTIONARCS_H
