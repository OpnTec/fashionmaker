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

#include <QGraphicsSceneMouseEvent>

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
void VisToolPiecePath::RefreshGeometry()
{
    HideAllItems();

    if (m_path.CountNodes() > 0)
    {
        DrawPath(this, m_path.PainterPath(Visualization::data), mainColor, m_path.GetPenType(), Qt::RoundCap);

        const QVector<VPointF> nodes = m_path.PathNodePoints(Visualization::data);

        for (int i = 0; i < nodes.size(); ++i)
        {
            VSimplePoint *point = GetPoint(static_cast<quint32>(i), supportColor);
            point->SetOnlyPoint(mode == Mode::Creation);
            point->RefreshPointGeometry(nodes.at(i));
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
void VisToolPiecePath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    for (int i=0; i < m_points.size(); ++i)
    {
        ScalePoint(m_points[i], scale);
    }

    ScalePenWidth(m_line, scale);

    VisPath::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiecePath::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint *VisToolPiecePath::GetPoint(quint32 i, const QColor &color)
{
    return VisPath::GetPoint(m_points, i, color);
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
