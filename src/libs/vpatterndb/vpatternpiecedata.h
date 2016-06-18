/************************************************************************
 **
 **  @file   vpatternpiecedata.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
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

#ifndef VPATTERNPIECEDATA_H
#define VPATTERNPIECEDATA_H

#include <QList>
#include <QString>

enum class MaterialType : char
{
    mtFabric,
    mtLining,
    mtInterfacing,
    mtInterlining,
    mtUserDefined
};

enum class PlacementType : char { ptNone, ptCutOnFold };

/**
 * @brief This structure holds a material, cut number and placement 3-tuple
 */
struct MaterialCutPlacement
{
    MaterialType                m_eMaterial;
    QString                     m_qsMaterialUserDef;
    int                         m_iCutNumber;
    PlacementType               m_ePlacement;
};

typedef QList<MaterialCutPlacement> MCPContainer;

/**
 * @brief The VPatternPieceData class holds some information about a single
 * piece like letter, name, material type, cut number and placement.
 */
class VPatternPieceData
{
public:
    VPatternPieceData();
    ~VPatternPieceData();

    // methods, which operate on MaterialCutPlacement container
    void                        Append(const MaterialCutPlacement& rMCP);
    void                        Insert(int i, const MaterialCutPlacement& rMCP);
    void                        Set(int i, const MaterialCutPlacement& rMCP);
    int                         GetMCPCount() const;
    const MaterialCutPlacement& GetMCP(int i) const;
    void                        RemoveMCP(int i);

    // methods, which operate on other members
    const QString&              GetLetter() const;
    void                        SetLetter(QString qsLetter);
    const QString&              GetName() const;
    void                        SetName(QString qsName);

private:
    /** @brief Pattern piece name
     */
    QString             m_qsName;
    /** @brief Pattern piece letter (should be no more than 3 letters)
     */
    QString             m_qsLetter;
    /** @brief List of material, cut number and placement 3-tuples
     */
    MCPContainer        m_conMCP;
};

#endif // VPATTERNPIECEDATA_H
