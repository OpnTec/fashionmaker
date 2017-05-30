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

#ifndef VLAYOUTPAPER_P_H
#define VLAYOUTPAPER_P_H

#include <QSharedData>
#include <QVector>
#include <QPointF>

#include "vlayoutpiece.h"
#include "vcontour.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VLayoutPaperData : public QSharedData
{
public:
    VLayoutPaperData()
        : details(QVector<VLayoutPiece>()),
          globalContour(VContour()),
          paperIndex(0),
          frame(0),
          layoutWidth(0),
          globalRotate(true),
          localRotate(true),
          globalRotationIncrease(180),
          localRotationIncrease(180),
          saveLength(false)
    {}

    VLayoutPaperData(int height,
                     int width)
        : details(QVector<VLayoutPiece>()),
          globalContour(VContour(height, width)),
          paperIndex(0),
          frame(0),
          layoutWidth(0),
          globalRotate(true),
          localRotate(true),
          globalRotationIncrease(180),
          localRotationIncrease(180),
          saveLength(false)
    {}

    VLayoutPaperData(const VLayoutPaperData &paper)
        : QSharedData(paper),
          details(paper.details),
          globalContour(paper.globalContour),
          paperIndex(paper.paperIndex),
          frame(paper.frame),
          layoutWidth(paper.layoutWidth),
          globalRotate(paper.globalRotate),
          localRotate(paper.localRotate),
          globalRotationIncrease(paper.globalRotationIncrease),
          localRotationIncrease(paper.localRotationIncrease),
          saveLength(paper.saveLength)
    {}

    ~VLayoutPaperData() {}

    /** @brief details list of arranged details. */
    QVector<VLayoutPiece> details;

    /** @brief globalContour list of global points contour. */
    VContour globalContour;

    quint32 paperIndex;
    quint32 frame;
    qreal layoutWidth;
    bool globalRotate;
    bool localRotate;
    int globalRotationIncrease;
    int localRotationIncrease;
    bool saveLength;

private:
    VLayoutPaperData& operator=(const VLayoutPaperData&) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VLAYOUTPAPER_P_H
