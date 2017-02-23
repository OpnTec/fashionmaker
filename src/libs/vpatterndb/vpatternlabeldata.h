/************************************************************************
 **
 **  @file   vpatternlabeldata.h
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

#ifndef VPATTERNINFOGEOMETRY_H
#define VPATTERNINFOGEOMETRY_H

#include <QPointF>
#include <QtGlobal>

/**
 * @brief The VPatternLabelData class holds the information about pattern info label geometry
 */
class VPatternLabelData
{
public:
    VPatternLabelData();

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
     * @brief m_iFontSize label text base font size
     */
    int                 m_iFontSize;
    /**
     * @brief m_dRotation Label rotation
     */
    qreal               m_dRotation;
    /**
     * @brief m_bVisible visibility flag
     */
    bool                m_bVisible;

};

#endif // VPATTERNINFOGEOMETRY_H
