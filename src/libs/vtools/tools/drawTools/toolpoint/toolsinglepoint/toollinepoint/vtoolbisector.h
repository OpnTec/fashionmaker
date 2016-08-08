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

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vwidgets/../vmisc/def.h"
#include "vtoollinepoint.h"

class DialogTool;
class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QPointF;
class VContainer;
class VGObject;
class VMainGraphicsScene;
template <class T> class QSharedPointer;

/**
 * @brief The VToolBisector class tool for creation bisector point.
 */
class VToolBisector : public VToolLinePoint
{
    Q_OBJECT
public:
    static qreal   BisectorAngle(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint);
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint,
                             const qreal& length);
    virtual void   setDialog() Q_DECL_OVERRIDE;
    static VToolBisector* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                 VContainer *data);
    static VToolBisector* Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                                 const quint32 &secondPointId, const quint32 &thirdPointId, const QString &typeLine,
                                 const QString &lineColor, const QString &pointName, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene  *scene, VAbstractPattern *doc, VContainer *data,
                                 const Document &parse,
                                 const Source &typeCreation);
    static const QString ToolType;
    virtual int    type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Bisector)};

    quint32 GetFirstPointId() const;
    void    SetFirstPointId(const quint32 &value);

    quint32 GetThirdPointId() const;
    void    SetThirdPointId(const quint32 &value);

    virtual void   ShowVisualization(bool show) Q_DECL_OVERRIDE;
public slots:
    virtual void   SetFactor(qreal factor) Q_DECL_OVERRIDE;
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void   RemoveReferens() Q_DECL_OVERRIDE;
    virtual void   SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void   ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void   SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolBisector)

    /** @brief firstPointId id first point of angle. */
    quint32         firstPointId;

    /** @brief thirdPointId id third point of angle. */
    quint32         thirdPointId;

    VToolBisector(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                  const QString &lineColor,
                  const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                  const quint32 &thirdPointId, const Source &typeCreation, QGraphicsItem * parent = nullptr);
};

#endif // VTOOLBISECTOR_H
