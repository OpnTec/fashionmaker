/************************************************************************
 **
 **  @file   vnobrushscalepathitem.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 1, 2016
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

#ifndef VNOBRUSHSCALEPATHITEM_H
#define VNOBRUSHSCALEPATHITEM_H

#include <qcompilerdetection.h>
#include <QGraphicsPathItem>
#include <QtGlobal>

class VNoBrushScalePathItem : public QGraphicsPathItem
{
public:
    explicit VNoBrushScalePathItem(QGraphicsItem *parent = 0);

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = nullptr) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VNoBrushScalePathItem)
};

#endif // VNOBRUSHSCALEPATHITEM_H
