/************************************************************************
 **
 **  @file   vistoolheight.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VISTOOLHEIGHT_H
#define VISTOOLHEIGHT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "visline.h"

class QLineF;
class VContainer;

class VisToolHeight : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolHeight(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolHeight() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void         setLineP1Id(const quint32 &value);
    void         setLineP2Id(const quint32 &value);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolHeight)};
private:
    Q_DISABLE_COPY(VisToolHeight)
    //base point in parent class
    quint32              lineP1Id;//first point of line
    quint32              lineP2Id;//second point of line
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *base_point;
    QGraphicsEllipseItem *lineP1;
    QGraphicsEllipseItem *lineP2;
    QGraphicsLineItem    *line;
    QGraphicsLineItem    *line_intersection;
    void         ShowIntersection(const QLineF &height_line, const QLineF &base_line);
};

#endif // VISTOOLHEIGHT_H
