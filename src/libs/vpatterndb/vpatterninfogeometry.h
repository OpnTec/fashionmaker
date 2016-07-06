/************************************************************************
 **
 **  @file   vpatterninfogeometry.h
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

#define MIN_FONT_SIZE               12

#include <QPointF>

class VPatternInfoGeometry
{
public:
    VPatternInfoGeometry();
    ~VPatternInfoGeometry();

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
    QPointF             m_ptPos;
    /** @brief Label width
     */
    qreal               m_dLabelWidth;
    /** @brief Label height
     */
    qreal               m_dLabelHeight;
    /** @brief Label font size
     */
    int                 m_iFontSize;
    /** @brief Label rotation
     */
    qreal               m_dRotation;
    /** @brief Visibility flag
     */
    bool                m_bVisible;

};

#endif // VPATTERNINFOGEOMETRY_H
