/************************************************************************
 **
 **  @file   vpiecelabeldata.h
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
#include <QPointF>
#include <QSet>
#include <QString>
#include <QVector>
#include <QtGlobal>

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
    MaterialType                m_eMaterial;
    QString                     m_qsMaterialUserDef;
    int                         m_iCutNumber;
    PlacementType               m_ePlacement;

    MaterialCutPlacement();
};

typedef QList<MaterialCutPlacement> MCPContainer;

/**
 * @brief The VPieceLabelData class holds some information about a single
 * piece like letter, name, material type, cut number and placement.
 */
class VPieceLabelData
{
public:
    VPieceLabelData();

    // methods, which operate on MaterialCutPlacement container
    void                        Append(const MaterialCutPlacement& rMCP);
    void                        Insert(int i, const MaterialCutPlacement& rMCP);
    void                        Set(int i, const MaterialCutPlacement& rMCP);
    int                         GetMCPCount() const;
    const MaterialCutPlacement& GetMCP(int i) const;
    void                        RemoveMCP(int i);
    void                        Clear();

    // methods, which operate on other members
    const QString&              GetLetter() const;
    void                        SetLetter(QString qsLetter);

    // methods, which set up label parameters
    QPointF                     GetPos() const;
    void                        SetPos(const QPointF& ptPos);
    qreal                       GetLabelWidth() const;
    void                        SetLabelWidth(qreal dLabelW);
    qreal                       GetLabelHeight() const;
    void                        SetLabelHeight(qreal dLabelH);
    int                         GetFontSize() const;
    void                        SetFontSize(int iSize);
    qreal                       GetRotation() const;
    void                        SetRotation(qreal dRot);
    bool                        IsVisible() const;
    void                        SetVisible(bool bVal);

private:
    /**
     * @brief m_qsLetter Detail letter (should be no more than 3 characters)
     */
    QString             m_qsLetter;
    /**
     * @brief m_conMCP List of material, cut, placement tuples
     */
    MCPContainer        m_conMCP;
    /**
     * @brief m_ptPos position of label's top left corner
     */
    QPointF             m_ptPos;
    /**
     * @brief m_dLabelWidth label width
     */
    qreal               m_dLabelWidth;
    /**
     * @brief m_dLabelHeight label height
     */
    qreal               m_dLabelHeight;
    /**
     * @brief m_iFontSize label font size
     */
    int                 m_iFontSize;
    /**
     * @brief m_dRotation label rotation
     */
    qreal               m_dRotation;
    /**
     * @brief m_bVisible visibility flag
     */
    bool                m_bVisible;

};

#endif // VPATTERNPIECEDATA_H
