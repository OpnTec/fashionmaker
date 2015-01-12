/************************************************************************
 **
 **  @file   vlayoutpaper_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 1, 2015
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

#ifndef VLAYOUTPAPER_P_H
#define VLAYOUTPAPER_P_H

#include <QSharedData>
#include <QVector>
#include <QPointF>

#include "vlayoutdetail.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VLayoutPaperData : public QSharedData
{
public:
    VLayoutPaperData()
        :details(QVector<VLayoutDetail>()), globalContour(QVector<QPointF>()), paperHeight(0), paperWidth(0), shift(0),
          paperIndex(0), frame(0)
    {}

    VLayoutPaperData(int height, int width)
        :details(QVector<VLayoutDetail>()), globalContour(QVector<QPointF>()), paperHeight(height), paperWidth(width),
          shift(0), paperIndex(0), frame(0)
    {}

    VLayoutPaperData(const VLayoutPaperData &paper)
        :QSharedData(paper), details(paper.details), globalContour(paper.globalContour), paperHeight(paper.paperHeight),
          paperWidth(paper.paperWidth), shift(paper.shift), paperIndex(paper.paperIndex), frame(paper.frame)
    {}

    ~VLayoutPaperData() {}

    /** @brief details list of arranged details. */
    QVector<VLayoutDetail> details;

    /** @brief globalContour list of global points contour. */
    QVector<QPointF> globalContour;

    /** @brief paperHeight height of paper in pixels*/
    int paperHeight;

    /** @brief paperWidth width of paper in pixels*/
    int paperWidth;

    unsigned int shift;
    quint32 paperIndex;
    quint32 frame;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VLAYOUTPAPER_P_H
