/************************************************************************
 **
 **  @file   vcurvelength_p.h
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

#ifndef VCURVELENGTH_P_H
#define VCURVELENGTH_P_H

#include <QSharedData>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VCurveLengthData : public QSharedData
{
public:

    VCurveLengthData()
        :id(NULL_ID), parentId(NULL_ID)
    {}

    VCurveLengthData(const quint32 &id, const quint32 &parentId)
        :id(id), parentId(parentId)
    {}

    VCurveLengthData(const VCurveLengthData &var)
        :QSharedData(var), id(var.id), parentId(var.parentId)
    {}

    virtual  ~VCurveLengthData();

    quint32 id;
    quint32 parentId;
};

VCurveLengthData::~VCurveLengthData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VCURVELENGTH_P_H
