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

#ifndef VABSTRACTFLOATITEMDATA_H
#define VABSTRACTFLOATITEMDATA_H

#include <QPointF>

class VAbstractFloatItemData
{
public:
    VAbstractFloatItemData();
    virtual ~VAbstractFloatItemData();

    // methods, which set and return values of different parameters
    QPointF GetPos() const;
    void    SetPos(const QPointF& ptPos);

    bool IsVisible() const;
    void SetVisible(bool bVisible);

protected:
    /** @brief m_ptPos position of label's top left corner */
    QPointF m_ptPos;
    /** @brief m_bVisible visibility flag */
    bool    m_bVisible;
};

#endif // VABSTRACTFLOATITEMDATA_H
