/************************************************************************
 **
 **  @file   vgrainlinegeometry.h
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

#include <QtGlobal>
#include <QString>

class QPointF;

/**
 * @brief The VGrainlineGeometry class holds information about a grainline like
 * position, size, rotation and visibility
 */
class VGrainlineGeometry
{
public:
    // denotes the type of arrow for the grainline
    enum ArrowType
    {
        atBoth,
        atFront,
        atRear
    };

    VGrainlineGeometry();

    // methods, which set and return values of different parameters
    QPointF                 GetPos() const;
    void                    SetPos(const QPointF& ptPos);
    QString                 GetLength() const;
    void                    SetLength(const QString& qsLen);
    QString                 GetRotation() const;
    void                    SetRotation(const QString& qsRot);
    bool                    IsVisible() const;
    void                    SetVisible(bool bVisible);
    ArrowType               GetArrowType() const;
    void                    SetArrowType(ArrowType eAT);

private:
    /**
     * @brief m_ptPos position of grainline's center
     */
    QPointF                 m_ptPos;
    /**
     * @brief m_dLength formula to calculate the length of grainline
     */
    QString                 m_qsLength;
    /**
     * @brief m_dRotation formula to calculate the rotation of grainline in [degrees]
     */
    QString                 m_qsRotation;
    /**
     * @brief m_bVisible visibility flag
     */
    bool                    m_bVisible;
    /**
     * @brief m_eArrowType type of arrow on the grainline
     */
    ArrowType                m_eArrowType;
};

#endif // VGRAINLINEGEOMETRY_H

