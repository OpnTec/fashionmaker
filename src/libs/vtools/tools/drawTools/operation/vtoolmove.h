/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VTOOLMOVING_H
#define VTOOLMOVING_H

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

template <class T> class QSharedPointer;
class VFormula;

struct VToolMoveInitData : VAbstractOperationInitData
{
    VToolMoveInitData()
        : VAbstractOperationInitData(),
          formulaAngle(),
          formulaRotationAngle(),
          formulaLength(),
          rotationOrigin(NULL_ID)
    {}

    QString formulaAngle;
    QString formulaRotationAngle;
    QString formulaLength;
    quint32 rotationOrigin;
};

class VToolMove : public VAbstractOperation
{
    Q_OBJECT
public:
    virtual ~VToolMove() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() override;
    static VToolMove* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                             VContainer *data);
    static VToolMove* Create(VToolMoveInitData &initData);

    static const QString ToolType;

    virtual int type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Move)};

    VFormula GetFormulaAngle() const;
    void     SetFormulaAngle(const VFormula &value);

    VFormula GetFormulaRotationAngle() const;
    void     SetFormulaRotationAngle(const VFormula &value);

    VFormula GetFormulaLength() const;
    void     SetFormulaLength(const VFormula &value);

    QString OriginPointName() const;

    virtual void ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    SetVisualization() override;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual QString MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolMove)
    QString formulaAngle;
    QString formulaRotationAngle;
    QString formulaLength;
    quint32 origPointId;

    VToolMove(const VToolMoveInitData &initData, QGraphicsItem *parent = nullptr);

    static DestinationItem CreatePoint(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                       const QPointF &rotationOrigin, const QString &suffix, VContainer *data);

    template <class Item>
    static DestinationItem CreateItem(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                      const QPointF &rotationOrigin, const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                     const QPointF &rotationOrigin, const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                       const QPointF &rotationOrigin,const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle, qreal length,
                                                   qreal rotationAngle, const QPointF &rotationOrigin,
                                                   const QString &suffix, VContainer *data);

    static void UpdatePoint(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                            const QPointF &rotationOrigin, const QString &suffix, VContainer *data,
                            const DestinationItem &item);
    template <class Item>
    static void UpdateItem(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                           const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                          const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                            const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                        const QPointF &rotationOrigin, const QString &suffix, VContainer *data,
                                        quint32 id);
};

#endif // VTOOLMOVING_H
