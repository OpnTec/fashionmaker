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

#ifndef VLAYOUTDETAIL_P_H
#define VLAYOUTDETAIL_P_H

#include <QSharedData>
#include <QPointF>
#include <QVector>
#include <QTransform>

#include "../vpatterndb/vpiecelabeldata.h"
#include "../vpatterndb/vpatternlabeldata.h"
#include "../vpatterndb/vgrainlinedata.h"
#include "../vmisc/diagnostic.h"
#include "vlayoutpiecepath.h"

#include "vtextmanager.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VLayoutPieceData : public QSharedData
{
public:
    VLayoutPieceData()
        : contour(),
          seamAllowance(),
          layoutAllowance(),
          m_internalPaths(),
          matrix(),
          layoutWidth(0),
          mirror(false),
          detailLabel(),
          patternInfo(),
          grainlinePoints(),
          detailData(),
          patternGeom(),
          grainlineGeom(),
          m_tmDetail(),
          m_tmPattern(),
          m_liPP()
    {}

    VLayoutPieceData(const VLayoutPieceData &detail)
        : QSharedData(detail),
          contour(detail.contour),
          seamAllowance(detail.seamAllowance),
          layoutAllowance(detail.layoutAllowance),
          m_internalPaths(detail.m_internalPaths),
          matrix(detail.matrix),
          layoutWidth(detail.layoutWidth),
          mirror(detail.mirror),
          detailLabel(detail.detailLabel),
          patternInfo(detail.patternInfo),
          grainlinePoints(detail.grainlinePoints),
          detailData(detail.detailData),
          patternGeom(detail.patternGeom),
          grainlineGeom(detail.grainlineGeom),
          m_tmDetail(detail.m_tmDetail),
          m_tmPattern(detail.m_tmPattern),
          m_liPP(detail.m_liPP)
    {}

    ~VLayoutPieceData() {}

    /** @brief contour list of contour points. */
    QVector<QPointF>          contour;

    /** @brief seamAllowance list of seam allowance points. */
    QVector<QPointF>          seamAllowance;

    /** @brief layoutAllowance list of layout allowance points. */
    QVector<QPointF>          layoutAllowance;

    /** @brief m_internalPaths list of internal paths. */
    QVector<VLayoutPiecePath> m_internalPaths;

    /** @brief matrix transformation matrix*/
    QTransform                matrix;

    /** @brief layoutWidth value layout allowance width in pixels. */
    qreal                     layoutWidth;

    bool                      mirror;

    /** @brief detailLabel detail label rectangle */
    QVector<QPointF>          detailLabel;
    /** @brief patternInfo pattern info rectangle */
    QVector<QPointF>          patternInfo;
    /** @brief grainlineInfo line */
    QVector<QPointF>          grainlinePoints;
    /** @brief detailData detail data */
    VPieceLabelData         detailData;
    /** @brief patternGeom pattern geometry */
    VPatternLabelData      patternGeom;
    /** @brief grainlineGeom grainline geometry */
    VGrainlineData        grainlineGeom;
    /** @brief m_tmDetail text manager for laying out detail info */
    VTextManager              m_tmDetail;
    /** @brief m_tmPattern text manager for laying out pattern info */
    VTextManager              m_tmPattern;
    /** @bried m_liPP list of generated text painter paths */
    QList<QPainterPath>       m_liPP;

private:
    VLayoutPieceData &operator=(const VLayoutPieceData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VLAYOUTDETAIL_P_H
