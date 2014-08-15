/************************************************************************
 **
 **  @file   vispath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vispath.h"

//---------------------------------------------------------------------------------------------------------------------
VisPath::VisPath(const VContainer *data, QGraphicsItem *parent)
    :Visualization(data), QGraphicsPathItem(parent)
{
    this->setZValue(1);// Show on top real tool
    InitPen();
}

//---------------------------------------------------------------------------------------------------------------------
VisPath::~VisPath()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisPath::InitPen()
{
    this->setPen(QPen(mainColor, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisPath::AddOnScene()
{
    AddItem(this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisPath::DrawPath(QGraphicsPathItem *pathItem, const QPainterPath &path, const QColor &color, Qt::PenStyle style)
{
    SCASSERT (pathItem != nullptr);

    pathItem->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, style));
    pathItem->setPath(path);
}
