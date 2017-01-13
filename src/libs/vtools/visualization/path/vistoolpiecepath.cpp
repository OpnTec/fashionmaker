/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vistoolpiecepath.h"
#include "../vwidgets/vsimplepoint.h"
#include "../vgeometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPiecePath::VisToolPiecePath(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      m_points(),
      m_line(nullptr),
      m_path()
{
    m_line = InitItem<QGraphicsLineItem>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPiecePath::~VisToolPiecePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiecePath::RefreshGeometry()
{
    HideAllItems();

    if (m_path.CountNodes() > 0)
    {
        DrawPath(this, m_path.PainterPath(Visualization::data), mainColor, Qt::SolidLine, Qt::RoundCap);

        const QVector<VPointF> nodes = m_path.PathNodePoints(Visualization::data);

        for (int i = 0; i < nodes.size(); ++i)
        {
            VSimplePoint *point = GetPoint(static_cast<quint32>(i), supportColor);
            point->SetOnlyPoint(mode == Mode::Creation);
            point->RefreshGeometry(nodes.at(i));
            point->setVisible(true);
        }

        if (mode == Mode::Creation)
        {
            const QVector<QPointF> points = m_path.PathPoints(Visualization::data);
            if (points.size() > 0)
            {
                DrawLine(m_line, QLineF(points.last(), Visualization::scenePos), supportColor, Qt::DashLine);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiecePath::SetPath(const VPiecePath &path)
{
    m_path = path;
}

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint *VisToolPiecePath::GetPoint(quint32 i, const QColor &color)
{
    if (not m_points.isEmpty() && static_cast<quint32>(m_points.size() - 1) >= i)
    {
        return m_points.at(static_cast<int>(i));
    }
    else
    {
        VSimplePoint *point = new VSimplePoint(NULL_ID, color, *Visualization::data->GetPatternUnit(), &factor);
        point->SetPointHighlight(true);
        point->setParentItem(this);
        point->SetVisualizationMode(true);
        m_points.append(point);

        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiecePath::HideAllItems()
{
    if (m_line)
    {
        m_line->setVisible(false);
    }

    for (int i=0; i < m_points.size(); ++i)
    {
        if (QGraphicsEllipseItem *item = m_points.at(i))
        {
            item->setVisible(false);
        }
    }
}
