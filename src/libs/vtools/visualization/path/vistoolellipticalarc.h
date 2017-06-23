/************************************************************************
 **
 **  @file   vistoolellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   24 10, 2016
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
#ifndef VISTOOLELLIPTICALARC_H
#define VISTOOLELLIPTICALARC_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "vispath.h"

class VisToolEllipticalArc : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolEllipticalArc(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolEllipticalArc() Q_DECL_EQ_DEFAULT;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void setRadius1(const QString &expression);
    void setRadius2(const QString &expression);
    void setF1(const QString &expression);
    void setF2(const QString &expression);
    void setRotationAngle(const QString &expression);

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolEllipticalArc)};
private:
    Q_DISABLE_COPY(VisToolEllipticalArc)
    VScaledEllipse *arcCenter;
    qreal           radius1;
    qreal           radius2;
    qreal           f1;
    qreal           f2;
    qreal           rotationAngle;
};

#endif // VISTOOLELLIPTICALARC_H
