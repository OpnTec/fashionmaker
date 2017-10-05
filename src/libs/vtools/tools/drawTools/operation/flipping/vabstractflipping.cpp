/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
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
VAbstractFlipping::VAbstractFlipping(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &suffix,
                                     const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                     QGraphicsItem *parent)
    : VAbstractOperation(doc, data, id, suffix, source, destination, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractFlipping::CreateDestination(VAbstractOperationInitData &initData, const QPointF &fPoint,
                                          const QPointF &sPoint)
{
    if (initData.typeCreation == Source::FromGui)
    {
        initData.destination.clear();// Try to avoid mistake, value must be empty

        initData.id = VContainer::getNextId();//Just reserve id for tool

        for (int i = 0; i < initData.source.size(); ++i)
        {
            const quint32 idObject = initData.source.at(i);
            const QSharedPointer<VGObject> obj = initData.data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    initData.destination.append(CreatePoint(initData.id, idObject, fPoint, sPoint, initData.suffix,
                                                            initData.data));
                    break;
                case GOType::Arc:
                    initData.destination.append(CreateArc<VArc>(initData.id, idObject, fPoint, sPoint, initData.suffix,
                                                                initData.data));
                    break;
                case GOType::EllipticalArc:
                    initData.destination.append(CreateArc<VEllipticalArc>(initData.id, idObject, fPoint, sPoint,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::Spline:
                    initData.destination.append(CreateCurve<VSpline>(initData.id, idObject, fPoint, sPoint,
                                                                     initData.suffix, initData.data));
                    break;
                case GOType::SplinePath:
                    initData.destination.append(CreateCurveWithSegments<VSplinePath>(initData.id, idObject, fPoint,
                                                                                     sPoint, initData.suffix,
                                                                                     initData.data));
                    break;
                case GOType::CubicBezier:
                    initData.destination.append(CreateCurve<VCubicBezier>(initData.id, idObject, fPoint, sPoint,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::CubicBezierPath:
                    initData.destination.append(CreateCurveWithSegments<VCubicBezierPath>(initData.id, idObject, fPoint,
                                                                                          sPoint, initData.suffix,
                                                                                          initData.data));
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
    }
    else
    {
        for (int i = 0; i < initData.source.size(); ++i)
        {
            const quint32 idObject = initData.source.at(i);
            const QSharedPointer<VGObject> obj = initData.data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                {
                    const DestinationItem &item = initData.destination.at(i);
                    UpdatePoint(initData.id, idObject, fPoint, sPoint, initData.suffix, initData.data, item);
                    break;
                }
                case GOType::Arc:
                    UpdateArc<VArc>(initData.id, idObject, fPoint, sPoint, initData.suffix, initData.data,
                                    initData.destination.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(initData.id, idObject, fPoint, sPoint, initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(initData.id, idObject, fPoint, sPoint, initData.suffix, initData.data,
                                         initData.destination.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(initData.id, idObject, fPoint, sPoint, initData.suffix,
                                                         initData.data, initData.destination.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(initData.id, idObject, fPoint, sPoint, initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(initData.id, idObject, fPoint, sPoint, initData.suffix,
                                                              initData.data, initData.destination.at(i).id);
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
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
    item.showLabel = rotated.IsShowLabel();
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
                                    const QPointF &secondPoint, const QString &suffix, VContainer *data,
                                    const DestinationItem &item)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);
    rotated.setMx(item.mx);
    rotated.setMy(item.my);
    rotated.SetShowLabel(item.showLabel);
    data->UpdateGObject(item.id, new VPointF(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VAbstractFlipping::UpdateArc(quint32 idTool, quint32 idItem, const QPointF &firstPoint, const QPointF &secondPoint,
                                  const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data, id);
    data->AddArc(data->GeometricObject<Item>(id), id);
}
