/************************************************************************
 **
 **  @file   VArcRadiusData.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VARCRADIUSDATA_H
#define VARCRADIUSDATA_H

#include <QSharedData>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VArcRadiusData : public QSharedData
{
public:

    VArcRadiusData()
        :arcId(NULL_ID)
    {}

    VArcRadiusData(const quint32 &arcId)
        :arcId(arcId)
    {}

    VArcRadiusData(const VArcRadiusData &var)
        :QSharedData(var), arcId(var.arcId)
    {}

    virtual  ~VArcRadiusData() Q_DECL_OVERRIDE;

    quint32 arcId;

private:
    VArcRadiusData &operator=(const VArcRadiusData &) Q_DECL_EQ_DELETE;
};

VArcRadiusData::~VArcRadiusData()
{}

QT_WARNING_POP

#endif // VARCRADIUSDATA_H
