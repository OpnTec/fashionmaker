/************************************************************************
 **
 **  @file   vispath.h
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

#ifndef VISPATH_H
#define VISPATH_H

#include "visualization.h"
#include <QGraphicsPathItem>

class VisPath : public Visualization, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VisPath(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisPath();

protected:
    virtual void InitPen();
    virtual void AddOnScene();

    void         DrawPath(QGraphicsPathItem *pathItem, const QPainterPath &path, const QColor &color,
                          Qt::PenStyle style = Qt::SolidLine);
private:
    Q_DISABLE_COPY(VisPath)
};

#endif // VISPATH_H
