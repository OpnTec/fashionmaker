/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 2, 2017
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

#ifndef VLAYOUTPIECEPATH_P_H
#define VLAYOUTPIECEPATH_P_H

#include <QSharedData>
#include <QPointF>
#include <QVector>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VLayoutPiecePathData : public QSharedData
{
public:
    VLayoutPiecePathData()
        : m_points(),
          m_penStyle(Qt::SolidLine)
    {}

    VLayoutPiecePathData(const QVector<QPointF> points, Qt::PenStyle penStyle)
        : m_points(points),
          m_penStyle(penStyle)
    {}

    VLayoutPiecePathData(const VLayoutPiecePathData &path)
        : QSharedData(path),
          m_points(path.m_points),
          m_penStyle(path.m_penStyle)
    {}

    ~VLayoutPiecePathData();

    /** @brief m_points list of path points. */
    QVector<QPointF> m_points;

    /** @brief m_penStyle path pen style. */
    Qt::PenStyle     m_penStyle;

private:
    VLayoutPiecePathData &operator=(const VLayoutPiecePathData &) Q_DECL_EQ_DELETE;
};

VLayoutPiecePathData::~VLayoutPiecePathData()
{}

QT_WARNING_POP

#endif // VLAYOUTPIECEPATH_P_H

