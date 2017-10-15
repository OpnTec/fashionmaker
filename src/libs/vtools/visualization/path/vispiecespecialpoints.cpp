/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vispiecespecialpoints.h"
#include "../vwidgets/vsimplepoint.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
VisPieceSpecialPoints::VisPieceSpecialPoints(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      m_points(),
      m_spoints()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VisPieceSpecialPoints::RefreshGeometry()
{
    HideAllItems();

    for (int i = 0; i < m_spoints.size(); ++i)
    {
        VSimplePoint *point = GetPoint(static_cast<quint32>(i), supportColor);
        point->SetOnlyPoint(false);
        const QSharedPointer<VPointF> p = Visualization::data->GeometricObject<VPointF>(m_spoints.at(i));
        point->RefreshPointGeometry(*p);
        point->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisPieceSpecialPoints::SetPoints(const QVector<quint32> &pins)
{
    m_spoints = pins;
}

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint *VisPieceSpecialPoints::GetPoint(quint32 i, const QColor &color)
{
    return VisPath::GetPoint(m_points, i, color);
}

//---------------------------------------------------------------------------------------------------------------------
void VisPieceSpecialPoints::HideAllItems()
{
    for (int i=0; i < m_points.size(); ++i)
    {
        if (QGraphicsEllipseItem *item = m_points.at(i))
        {
            item->setVisible(false);
        }
    }
}

