/************************************************************************
**
**  @file   vlayoutpaper_p.h
**  @author Roman Telezhynskyi <dismine(at)gmail.com>
**  @date   8 1, 2015
**
**  @brief
**  @copyright
**  This source code is part of the Valentina project, a pattern making
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
    {}

    VLayoutPaperData(int height, int width, qreal layoutWidth)
        : globalContour(VContour(height, width, layoutWidth)),
          layoutWidth(layoutWidth)
    {}

    VLayoutPaperData(const VLayoutPaperData &paper)
        : QSharedData(paper),
          details(paper.details),
          positionsCache(paper.positionsCache),
          globalContour(paper.globalContour),
          paperIndex(paper.paperIndex),
          layoutWidth(paper.layoutWidth),
          globalRotate(paper.globalRotate),
          localRotate(paper.localRotate),
          globalRotationNumber(paper.globalRotationNumber),
          localRotationNumber(paper.localRotationNumber),
          saveLength(paper.saveLength),
          followGrainline(paper.followGrainline),
          originPaperOrientation(paper.originPaperOrientation)
    {}

    ~VLayoutPaperData() {}

    /** @brief details list of arranged details. */
    QVector<VLayoutPiece> details{};

    QVector<QPainterPath> positionsCache{};

    /** @brief globalContour list of global points contour. */
    VContour globalContour{};

    quint32 paperIndex{0};
    qreal layoutWidth{0};
    bool globalRotate{true};
    bool localRotate{true};
    int globalRotationNumber{2};
    int localRotationNumber{2};
    bool saveLength{false};
    bool followGrainline{false};
    bool originPaperOrientation{true};

private:
    Q_DISABLE_ASSIGN(VLayoutPaperData)
};

QT_WARNING_POP

#endif // VLAYOUTPAPER_P_H
