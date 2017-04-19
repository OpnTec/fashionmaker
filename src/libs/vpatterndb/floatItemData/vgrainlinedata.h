/************************************************************************
 **
 **  @file   vgrainlinedata.h
 **  @author Bojan Kverh
 **  @date   September 06, 2016
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

#ifndef VGRAINLINEGEOMETRY_H
#define VGRAINLINEGEOMETRY_H

#include <QString>
#include <QPointF>

#include "vabstractfloatitemdata.h"
#include "floatitemdef.h"

class VGrainlineDataPrivate;

/**
 * @brief The VGrainlineData class holds information about a grainline like
 * position, size, rotation and visibility
 */
class VGrainlineData : public VAbstractFloatItemData
{
public:
    VGrainlineData();
    VGrainlineData(const VGrainlineData &data);

    virtual ~VGrainlineData();

    VGrainlineData &operator=(const VGrainlineData &data);
#ifdef Q_COMPILER_RVALUE_REFS
    VGrainlineData &operator=(VGrainlineData &&data) Q_DECL_NOTHROW { Swap(data); return *this; }
#endif

    void Swap(VGrainlineData &data) Q_DECL_NOTHROW
    { std::swap(d, data.d); }

    // methods, which set and return values of different parameters
    QString GetLength() const;
    void    SetLength(const QString& qsLen);

    QString GetRotation() const;
    void    SetRotation(const QString& qsRot);

    ArrowType GetArrowType() const;
    void      SetArrowType(ArrowType eAT);

    quint32 CenterPin() const;
    void    SetCenterPin(quint32 centerPin);

    quint32 TopPin() const;
    void    SetTopPin(quint32 topPin);

    quint32 BottomPin() const;
    void    SetBottomPin(quint32 bottomPin);

private:
    QSharedDataPointer<VGrainlineDataPrivate> d;
};

#endif // VGRAINLINEGEOMETRY_H

