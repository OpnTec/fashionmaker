/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
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

#include "vabstractflipping.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractFlipping::~VAbstractFlipping()
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractFlipping::VAbstractFlipping(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &suffix,
                                     const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                     QGraphicsItem *parent)
    : VAbstractOperation(doc, data, id, suffix, source, destination, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractFlipping::CreateDestination(Source typeCreation, quint32 &id, QVector<DestinationItem> &dest,
                                          const QVector<quint32> &source, const QPointF &fPoint, const QPointF &sPoint,
                                          const QString &suffix, VAbstractPattern *doc, VContainer *data,
                                          const Document &parse)
{
    if (typeCreation == Source::FromGui)
    {
        dest.clear();// Try to avoid mistake, value must be empty

        id = data->getNextId();//Just reserve id for tool

        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    dest.append(CreatePoint(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::Arc:
                    dest.append(CreateArc<VArc>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::EllipticalArc:
                    dest.append(CreateArc<VEllipticalArc>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::Spline:
                    dest.append(CreateCurve<VSpline>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::SplinePath:
                    dest.append(CreateCurveWithSegments<VSplinePath>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::CubicBezier:
                    dest.append(CreateCurve<VCubicBezier>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::CubicBezierPath:
                    dest.append(CreateCurveWithSegments<VCubicBezierPath>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
    }
    else
    {
        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    UpdatePoint(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id, dest.at(i).mx,
                                dest.at(i).my);
                    break;
                case GOType::Arc:
                    UpdateArc<VArc>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(id, idObject, fPoint, sPoint, suffix, data,
                                                              dest.at(i).id);
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VAbstractFlipping::CreatePoint(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                               const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);

    DestinationItem item;
    item.mx = rotated.mx();
    item.my = rotated.my();
    item.id = data->AddGObject(new VPointF(rotated));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VAbstractFlipping::CreateArc(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                             const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data);
    data->AddArc(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractFlipping::UpdatePoint(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                    const QPointF &secondPoint, const QString &suffix, VContainer *data, quint32 id,
                                    qreal mx, qreal my)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);
    rotated.setMx(mx);
    rotated.setMy(my);
    data->UpdateGObject(id, new VPointF(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VAbstractFlipping::UpdateArc(quint32 idTool, quint32 idItem, const QPointF &firstPoint, const QPointF &secondPoint,
                                  const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data, id);
    data->AddArc(data->GeometricObject<Item>(id), id);
}
