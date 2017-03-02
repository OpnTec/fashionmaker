/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef FLOATITEMDEF_H
#define FLOATITEMDEF_H

#include <QList>
#include <QString>

// denotes the type of arrow for the grainline
enum class ArrowType : char
{
    atBoth,
    atFront,
    atRear
};

enum class MaterialType : char
{
    mtFabric = 0,
    mtLining = 1,
    mtInterfacing = 2,
    mtInterlining = 3,
    mtUserDefined = 4
};

enum class PlacementType : char
{
    ptNone = 0,
    ptCutOnFold = 1
};

/**
 * @brief The MaterialCutPlacement struct used to hold a material, cut number and placement 3-tuple
 */
struct MaterialCutPlacement
{
    MaterialType  m_eMaterial;
    QString       m_qsMaterialUserDef;
    int           m_iCutNumber;
    PlacementType m_ePlacement;

    MaterialCutPlacement()
        : m_eMaterial(MaterialType::mtFabric),
          m_qsMaterialUserDef(),
          m_iCutNumber(0),
          m_ePlacement(PlacementType::ptNone)
    {}
};

typedef QList<MaterialCutPlacement> MCPContainer;

#endif // FLOATITEMDEF_H

