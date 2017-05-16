/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#ifndef VISOPERATION_H
#define VISOPERATION_H

#include <QtGlobal>

#include "../visline.h"

class VisOperation : public VisLine
{
    Q_OBJECT
public:
    explicit VisOperation(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisOperation();

    void SetObjects(QVector<quint32> objects);

    virtual void VisualMode(const quint32 &pointId = NULL_ID) Q_DECL_OVERRIDE;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolRotation)};
protected:
    QVector<quint32> objects;
    QColor           supportColor2;

    QVector<QGraphicsEllipseItem *> points;
    QVector<VCurvePathItem *>       curves;

    QGraphicsEllipseItem * GetPoint(quint32 i, const QColor &color);
    VCurvePathItem       * GetCurve(quint32 i, const QColor &color);

    template <class Item>
    int AddFlippedCurve(const QPointF &firstPoint, const QPointF &secondPoint, quint32 id, int i);

    void RefreshFlippedObjects(const QPointF &firstPoint, const QPointF &secondPoint);
private:
    Q_DISABLE_COPY(VisOperation)
};

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisOperation::AddFlippedCurve(const QPointF &firstPoint, const QPointF &secondPoint, quint32 id, int i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor2);
    DrawPath(path, curve->GetPath(PathDirection::Show), supportColor2, Qt::SolidLine, Qt::RoundCap);

    ++i;
    path = GetCurve(static_cast<quint32>(i), supportColor);
    if (object1Id != NULL_ID)
    {
        const Item flipped = curve->Flip(QLineF(firstPoint, secondPoint));
        DrawPath(path, flipped.GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);
    }

    return i;
}

#endif // VISOPERATION_H
