/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2017
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

#ifndef SCALESCENEITEMS_H
#define SCALESCENEITEMS_H

#include <QGraphicsLineItem>

#include "../vmisc/def.h"

class VScaledLine : public QGraphicsLineItem
{
public:
    explicit VScaledLine(QGraphicsItem * parent = nullptr);
    VScaledLine(const QLineF &line, QGraphicsItem * parent = nullptr);
    virtual ~VScaledLine() = default;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ScaledLine)};

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) Q_DECL_OVERRIDE;

    qreal GetBasicWidth() const;
    void  SetBasicWidth(const qreal &value);

private:
    Q_DISABLE_COPY(VScaledLine)

    qreal basicWidth;
};

class VScaledEllipse : public QGraphicsEllipseItem
{
public:
    explicit VScaledEllipse(QGraphicsItem * parent = nullptr);
    virtual ~VScaledEllipse() = default;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ScaledEllipse)};

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VScaledEllipse)
};

#endif // SCALESCENEITEMS_H
