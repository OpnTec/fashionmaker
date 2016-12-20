/************************************************************************
 **
 **  @file   vtoolrotation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VTOOLROTATION_H
#define VTOOLROTATION_H

#include <qcompilerdetection.h>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "vabstractoperation.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "../ifc/xml/vabstractpattern.h"

class DialogTool;
class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QPointF;
class VContainer;
class VGObject;
class VMainGraphicsScene;
template <class T> class QSharedPointer;
class VFormula;

class VToolRotation : public VAbstractOperation
{
    Q_OBJECT
public:
    virtual ~VToolRotation();
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolRotation* Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data);
    static VToolRotation* Create(const quint32 _id, const quint32 &origin, QString &formulaAngle, const QString &suffix,
                                 const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                 VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                 const Document &parse, const Source &typeCreation);

    static const QString ToolType;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Rotation)};

    VFormula GetFormulaAngle() const;
    void     SetFormulaAngle(const VFormula &value);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;

protected:
    virtual void SetVisualization() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolRotation)
    quint32 origPointId;
    QString formulaAngle;

    VToolRotation(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 origPointId,
                  const QString &formulaAngle, const QString &suffix, const QVector<quint32> &source,
                  const QVector<DestinationItem> &destination, const Source &typeCreation,
                  QGraphicsItem *parent = nullptr);

    static DestinationItem CreatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                                       const QString &suffix, VContainer *data);

    template <class Item>
    static DestinationItem CreateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                                                   const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                                     const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                                       const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin,
                                                   qreal formulaAngle, const QString &suffix, VContainer *data);

    static void UpdatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                            const QString &suffix, VContainer *data, quint32 id, qreal mx, qreal my);
    template <class Item>
    static void UpdateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                           const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                          const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                            const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin, qreal formulaAngle,
                                        const QString &suffix, VContainer *data, quint32 id);
};

#endif // VTOOLROTATION_H
