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

#ifndef VCURVELENGTH_P_H
#define VCURVELENGTH_P_H

#include <QSharedData>

#include "../ifc/ifcdef.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VCurveVariableData : public QSharedData
{
public:

    VCurveVariableData()
        :id(NULL_ID), parentId(NULL_ID)
    {}

    VCurveVariableData(const quint32 &id, const quint32 &parentId)
        :id(id), parentId(parentId)
    {}

    VCurveVariableData(const VCurveVariableData &var)
        :QSharedData(var), id(var.id), parentId(var.parentId)
    {}

    virtual  ~VCurveVariableData();

    quint32 id;
    quint32 parentId;

private:
    VCurveVariableData &operator=(const VCurveVariableData &) Q_DECL_EQ_DELETE;
};

VCurveVariableData::~VCurveVariableData()
{}

QT_WARNING_POP

#endif // VCURVELENGTH_P_H
