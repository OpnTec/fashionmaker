/************************************************************************
 **
 **  @file   vgobject_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VGOBJECT_P_H
#define VGOBJECT_P_H

#include <QSharedData>
#include "../options.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VGObjectData : public QSharedData
{
public:
    VGObjectData()
        :_id(NULL_ID), type(GOType::Unknown), idObject(NULL_ID), _name(QString()), mode(Draw::Calculation)
    {}

    VGObjectData(const GOType &type, const quint32 &idObject, const Draw &mode)
        :_id(NULL_ID), type(type), idObject(idObject), _name(QString()), mode(mode)
    {}

    VGObjectData(const VGObjectData &obj)
        :QSharedData(obj), _id(obj._id), type(obj.type), idObject(obj.idObject), _name(obj._name), mode(obj.mode)
    {}

    virtual ~VGObjectData();

    /** @brief _id id in container. Ned for arcs, spline and spline paths. */
    quint32 _id;

    /** @brief type type of graphical object */
    GOType  type;

    /** @brief idObject id of parent object. Only for modeling. All another return 0. */
    quint32 idObject;

    /** @brief _name object name */
    QString _name;

    /** @brief mode object created in calculation or drawing mode */
    Draw    mode;
};

VGObjectData::~VGObjectData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VGOBJECT_P_H
