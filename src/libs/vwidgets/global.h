/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>

extern const qreal defPointRadiusPixel;
extern const qreal widthMainLine;
extern const qreal widthHairLine;

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsLineItem;
class QColor;
class QRectF;

qreal SceneScale(QGraphicsScene *scene);

QColor CorrectColor(const QGraphicsItem *item, const QColor &color);

QRectF PointRect(qreal radius);
qreal  ScaledRadius(qreal scale);
void   ScaleCircleSize(QGraphicsEllipseItem *item, qreal scale);
qreal  ScaleWidth(qreal width, qreal scale);

#endif // GLOBAL_H
