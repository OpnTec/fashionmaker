/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 11, 2016
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

#include "vistoolpiece.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPiece::VisToolPiece(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      m_points(),
      m_line1(nullptr),
      m_line2(nullptr),
      m_piece()
{
    m_line1 = InitItem<QGraphicsLineItem>(supportColor, this);
    m_line2 = InitItem<QGraphicsLineItem>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::RefreshGeometry()
{
    HideAllItems();

    if (m_piece.GetPath().CountNodes() > 0)
    {
        DrawPath(this, m_piece.MainPathPath(Visualization::data), mainColor, Qt::SolidLine, Qt::RoundCap);

        const QVector<VPointF> nodes = m_piece.MainPathNodePoints(Visualization::data);

        for (int i = 0; i < nodes.size(); ++i)
        {
            QGraphicsEllipseItem *point = GetPoint(static_cast<quint32>(i), supportColor);
            DrawPoint(point, nodes.at(i).toQPointF(), supportColor);
        }

        if (mode == Mode::Creation)
        {
            const QVector<QPointF> points = m_piece.MainPathPoints(Visualization::data);
            DrawLine(m_line1, QLineF(points.first(), Visualization::scenePos), supportColor, Qt::DashLine);

            if (points.size() > 1)
            {
                DrawLine(m_line2, QLineF(points.last(), Visualization::scenePos), supportColor, Qt::DashLine);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::SetPiece(const VPiece &piece)
{
    m_piece = piece;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    for (int i=0; i < m_points.size(); ++i)
    {
        ScalePoint(m_points[i], scale);
    }

    ScalePenWidth(m_line1, scale);
    ScalePenWidth(m_line2, scale);

    VisPath::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisToolPiece::boundingRect() const
{
    QRectF rect = VisPath::boundingRect();

    for (int i=0; i < m_points.size(); ++i)
    {
        rect = rect.united(m_points.at(i)->boundingRect());
    }

    rect = rect.united(m_line1->boundingRect());
    rect = rect.united(m_line2->boundingRect());
    return rect;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisToolPiece::GetPoint(quint32 i, const QColor &color)
{
    return GetPointItem(m_points, i, color, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::HideAllItems()
{
    if (m_line1)
    {
        m_line1->setVisible(false);
    }

    if (m_line2)
    {
        m_line2->setVisible(false);
    }

    for (int i=0; i < m_points.size(); ++i)
    {
        if (QGraphicsEllipseItem *item = m_points.at(i))
        {
            item->setVisible(false);
        }
    }
}
