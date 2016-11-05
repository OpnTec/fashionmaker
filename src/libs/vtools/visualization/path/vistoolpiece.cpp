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
VisToolPiece::~VisToolPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::RefreshGeometry()
{
    if (m_piece.CountNode() > 0)
    {
        DrawPath(this, m_piece.MainPathPath(Visualization::data), mainColor, Qt::SolidLine, Qt::RoundCap);

        const QVector<QPointF> nodes = m_piece.MainPathNodePoints(Visualization::data);

        for (int i = 0; i < nodes.size(); ++i)
        {
            QGraphicsEllipseItem *point = GetPoint(static_cast<quint32>(i), supportColor);
            DrawPoint(point, nodes.at(i), supportColor);
        }

        const QVector<QPointF> points = m_piece.MainPathPoints(Visualization::data);
        DrawLine(m_line1, QLineF(points.first(), Visualization::scenePos), supportColor, Qt::DashLine);

        if (points.size() > 1)
        {
            DrawLine(m_line2, QLineF(points.last(), Visualization::scenePos), supportColor, Qt::DashLine);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::SetPiece(const VPiece &piece)
{
    m_piece = piece;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisToolPiece::GetPoint(quint32 i, const QColor &color)
{
    return GetPointItem(Visualization::data, factor, m_points, i, color, this);
}
