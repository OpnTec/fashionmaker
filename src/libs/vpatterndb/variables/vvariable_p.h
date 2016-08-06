/************************************************************************
 **
 **  @file   vvariable_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VVARIABLE_P_H
#define VVARIABLE_P_H

#include <QSharedData>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VVariableData : public QSharedData
{
public:

    VVariableData()
        :base(0), ksize(0), kheight(0), description(QString()), baseSize(0), baseHeight(0)
    {}

    VVariableData(qreal baseSize, qreal baseHeight, const qreal &base, const qreal &ksize, const qreal &kheight,
                  const QString &description)
        :base(base), ksize(ksize), kheight(kheight), description(description), baseSize(baseSize),
          baseHeight(baseHeight)
    {}

    VVariableData(const qreal &base, const QString &description)
        :base(base), ksize(0), kheight(0), description(description), baseSize(0), baseHeight(0)
    {}

    VVariableData(const VVariableData &var)
        :QSharedData(var), base(var.base), ksize(var.ksize), kheight(var.kheight), description(var.description),
          baseSize(var.baseSize), baseHeight(var.baseHeight)
    {}

    virtual ~VVariableData();

    /** @brief base value in base size and height */
    qreal   base;

    /** @brief ksize increment in sizes */
    qreal   ksize;

    /** @brief kgrowth increment in heights */
    qreal   kheight;

    /** @brief description description of increment */
    QString description;

    qreal baseSize;
    qreal baseHeight;

private:
    VVariableData &operator=(const VVariableData &) Q_DECL_EQ_DELETE;
};

VVariableData::~VVariableData()
{}

QT_WARNING_POP

#endif // VVARIABLE_P_H
