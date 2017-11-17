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

#ifndef VISTOOLMOVE_H
#define VISTOOLMOVE_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "visoperation.h"

class VisToolMove : public VisOperation
{
    Q_OBJECT
public:
    explicit VisToolMove(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolMove() = default;

    virtual void   RefreshGeometry() Q_DECL_OVERRIDE;

    QString Angle() const;
    void    SetAngle(const QString &expression);

    QString RotationAngle() const;
    void    SetRotationAngle(const QString &expression);

    QString Length() const;
    qreal   LengthValue() const;
    void    SetLength(const QString &expression);

    void SetRotationOriginPointId(quint32 value);

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolMove)};
private:
    Q_DISABLE_COPY(VisToolMove)
    qreal           angle;
    qreal           rotationAngle;
    qreal           length;
    VScaledEllipse *pointOrigin;
    VScaledEllipse *pointRotationOrigin;
    VScaledEllipse *pointFinish;
    VCurvePathItem *angleArc;
    VScaledLine    *rotationLine;
    VScaledLine    *xAxis;

    template <class Item>
    QGraphicsPathItem *AddOriginCurve(quint32 id, int &i);

    template <class Item>
    int AddMovedRotatedCurve(qreal angle, qreal length, quint32 id, int i, qreal rotationAngle,
                             const QPointF &rotationOrigin);

    static QPointF GetOriginPoint(const QVector<QGraphicsItem *> &objects);

    QVector<QGraphicsItem *> CreateOriginObjects(int &iPoint, int &iCurve);
    void CreateMovedRotatedObjects(int &iPoint, int &iCurve, qreal length, qreal angle, qreal rotationAngle,
                                   const QPointF &rotationOrigin);
};

#endif // VISTOOLMOVE_H
