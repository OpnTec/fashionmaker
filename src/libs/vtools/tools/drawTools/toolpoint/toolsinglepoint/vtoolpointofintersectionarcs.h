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

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

class VToolPointOfIntersectionArcs : public VToolSinglePoint
{
    Q_OBJECT

public:
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolPointOfIntersectionArcs *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                                VAbstractPattern *doc, VContainer *data);
    static VToolPointOfIntersectionArcs *Create(const quint32 _id, const QString &pointName, const quint32 &firstArcId,
                                                const quint32 &secondArcId, CrossCirclesPoint pType,
                                                const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                const Source &typeCreation);
    static QPointF FindPoint(const VArc *arc1, const VArc *arc2, const CrossCirclesPoint pType);
    static const QString ToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionArcs) };

    QString FirstArcName() const;
    QString SecondArcName() const;

    quint32 GetFirstArcId() const;
    void    SetFirstArcId(const quint32 &value);

    quint32 GetSecondArcId() const;
    void    SetSecondArcId(const quint32 &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(const CrossCirclesPoint &value);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void RemoveReferens() Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolPointOfIntersectionArcs)

    /** @brief firstArcId id first arc. */
    quint32       firstArcId;

    /** @brief secondArcId id second arc. */
    quint32       secondArcId;

    CrossCirclesPoint crossPoint;

    VToolPointOfIntersectionArcs(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &firstArcId,
                                 const quint32 &secondArcId, CrossCirclesPoint pType, const Source &typeCreation,
                                 QGraphicsItem * parent = nullptr);
};

#endif // VTOOLPOINTOFINTERSECTIONARCS_H
