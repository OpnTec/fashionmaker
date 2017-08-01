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

#include "global.h"
#include "../vmisc/def.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

const qreal defPointRadiusPixel = (2./*mm*/ / 25.4) * PrintDPI;
const qreal widthMainLine = (1.2/*mm*/ / 25.4) * PrintDPI;
const qreal widthHairLine = widthMainLine/3.0;

qreal SceneScale(QGraphicsScene *scene)
{
    qreal scale = 1;

    if (scene)
    {
        const QList<QGraphicsView *> views = scene->views();
        if (not views.isEmpty())
        {
            scale = views.first()->transform().m11();
        }
    }

    return scale;
}

//---------------------------------------------------------------------------------------------------------------------
QColor CorrectColor(const QGraphicsItem *item, const QColor &color)
{
    SCASSERT(item != nullptr)

    if (item->isEnabled())
    {
        return color;
    }
    else
    {
        return Qt::gray;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QRectF PointRect(qreal radius)
{
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    return rec;
}

//---------------------------------------------------------------------------------------------------------------------
qreal ScaledRadius(qreal scale)
{
    qreal scaledRadius = defPointRadiusPixel;
    if (scale > 1)
    {
        scaledRadius = qMax(defPointRadiusPixel/12, defPointRadiusPixel/scale);
    }
    return scaledRadius;
}

//---------------------------------------------------------------------------------------------------------------------
void ScaleCircleSize(QGraphicsEllipseItem *item, qreal scale)
{
    SCASSERT(item != nullptr)

    item->setRect(PointRect(ScaledRadius(scale)));
}

//---------------------------------------------------------------------------------------------------------------------
qreal ScaleWidth(qreal width, qreal scale)
{
    if (scale > 1)
    {
        width = qMax(0.1, width/scale);
    }
    return width;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath ItemShapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
    {
        return path;
    }
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
    {
        ps.setWidth(penWidthZero);
    }
    else
    {
        ps.setWidth(pen.widthF());
    }
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}
