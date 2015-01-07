/************************************************************************
 **
 **  @file   vlayoutdetail_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VLAYOUTDETAIL_P_H
#define VLAYOUTDETAIL_P_H

#include <QSharedData>
#include <QPointF>
#include <QVector>
#include <QMatrix>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VLayoutDetailData : public QSharedData
{
public:
    VLayoutDetailData()
        :contour(QVector<QPointF>()), seamAllowence(QVector<QPointF>()), layoutAllowence(QVector<QPointF>()),
          matrix(QMatrix()), layoutWidth(0)
    {}

    VLayoutDetailData(const VLayoutDetailData &detail)
        :QSharedData(detail), contour(detail.contour), seamAllowence(detail.seamAllowence),
          layoutAllowence(detail.layoutAllowence), matrix(detail.matrix), layoutWidth(detail.layoutWidth)
    {}

    ~VLayoutDetailData() {}

    /** @brief contour list of contour points. */
    QVector<QPointF> contour;

    /** @brief seamAllowence list of seam allowence points. */
    QVector<QPointF> seamAllowence;

    /** @brief layoutAllowence list of layout allowence points. */
    QVector<QPointF> layoutAllowence;

    /** @brief matrix transformation matrix*/
    QMatrix matrix;

    /** @brief layoutWidth value layout allowence width in pixels. */
    qreal layoutWidth;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VLAYOUTDETAIL_P_H
