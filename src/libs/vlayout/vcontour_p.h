/************************************************************************
 **
 **  @file   vcontour_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
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

#ifndef VCONTOUR_P_H
#define VCONTOUR_P_H

#include <QSharedData>
#include <QPointF>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VContourData : public QSharedData
{
public:
    VContourData()
        :globalContour(QVector<QPointF>()), paperHeight(0), paperWidth(0), shift(0)
    {}

    VContourData(int height, int width)
        :globalContour(QVector<QPointF>()), paperHeight(height), paperWidth(width), shift(0)
    {}

    VContourData(const VContourData &contour)
        :QSharedData(contour), globalContour(contour.globalContour), paperHeight(contour.paperHeight),
          paperWidth(contour.paperWidth), shift(contour.shift)
    {}

    ~VContourData() {}

    /** @brief globalContour list of global points contour. */
    QVector<QPointF> globalContour;

    /** @brief paperHeight height of paper in pixels*/
    int paperHeight;

    /** @brief paperWidth width of paper in pixels*/
    int paperWidth;

    quint32 shift;

private:
    VContourData &operator=(const VContourData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VCONTOUR_P_H
