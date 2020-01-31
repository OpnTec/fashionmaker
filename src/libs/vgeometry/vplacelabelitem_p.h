/************************************************************************
 **
 **  @file   vplacelabelitem_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef VPLACELABELITEM_P_H
#define VPLACELABELITEM_P_H

#include <QSharedData>
#include "vgeometrydef.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VPlaceLabelItemData : public QSharedData
{
public:
    VPlaceLabelItemData()
    {}

    VPlaceLabelItemData(const VPlaceLabelItemData &item)
        : QSharedData(item),
          width(item.width),
          height(item.height),
          angle(item.angle),
          visibilityTrigger(item.visibilityTrigger),
          type(item.type),
          centerPoint(item.centerPoint),
          wValue(item.wValue),
          hValue(item.hValue),
          aValue(item.aValue),
          correctionAngle(item.correctionAngle),
          isVisible(item.isVisible)
    {}

    virtual ~VPlaceLabelItemData();

    QString width{};
    QString height{};
    QString angle{'0'};
    QString visibilityTrigger{'1'};
    PlaceLabelType type{PlaceLabelType::Button};
    quint32 centerPoint{0};

    qreal wValue{0};
    qreal hValue{0};
    qreal aValue{0};
    qreal correctionAngle{0};
    qreal isVisible{1};

private:
    Q_DISABLE_ASSIGN(VPlaceLabelItemData)
};

VPlaceLabelItemData::~VPlaceLabelItemData()
{}

QT_WARNING_POP

#endif // VPLACELABELITEM_P_H
