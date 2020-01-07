/************************************************************************
 **
 **  @file   vlayoutdetail_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 1, 2015
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

#ifndef VLAYOUTDETAIL_P_H
#define VLAYOUTDETAIL_P_H

#include <QSharedData>
#include <QPointF>
#include <QVector>
#include <QTransform>

#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "../vmisc/diagnostic.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#   include "../vmisc/vdatastreamenum.h"
#endif
#include "vlayoutpiecepath.h"
#include "../vgeometry/vgeometrydef.h"
#include "vtextmanager.h"
#include "../ifc/exception/vexception.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VLayoutPieceData : public QSharedData
{
public:
    VLayoutPieceData()
    {}

    VLayoutPieceData(const VLayoutPieceData &detail)
        : QSharedData(detail),
          contour(detail.contour),
          seamAllowance(detail.seamAllowance),
          layoutAllowance(detail.layoutAllowance),
          passmarks(detail.passmarks),
          m_internalPaths(detail.m_internalPaths),
          matrix(detail.matrix),
          layoutWidth(detail.layoutWidth),
          mirror(detail.mirror),
          detailLabel(detail.detailLabel),
          patternInfo(detail.patternInfo),
          grainlinePoints(detail.grainlinePoints),
          grainlineArrowType(detail.grainlineArrowType),
          grainlineAngle(detail.grainlineAngle),
          grainlineEnabled(detail.grainlineEnabled),
          m_tmDetail(detail.m_tmDetail),
          m_tmPattern(detail.m_tmPattern),
          m_placeLabels(detail.m_placeLabels),
          m_square(detail.m_square),
          m_quantity(detail.m_quantity),
          m_id(detail.m_id)
    {}

    ~VLayoutPieceData() Q_DECL_EQ_DEFAULT;

    friend QDataStream& operator<<(QDataStream& dataStream, const VLayoutPieceData& piece);
    friend QDataStream& operator>>(QDataStream& dataStream, VLayoutPieceData& piece);

    /** @brief contour list of contour points. */
    QVector<QPointF>          contour{};

    /** @brief seamAllowance list of seam allowance points. */
    QVector<QPointF>          seamAllowance{};

    /** @brief layoutAllowance list of layout allowance points. */
    QVector<QPointF>          layoutAllowance{};

    /** @brief passmarks list of passmakrs. */
    QVector<VLayoutPassmark>  passmarks{};

    /** @brief m_internalPaths list of internal paths. */
    QVector<VLayoutPiecePath> m_internalPaths{};

    /** @brief matrix transformation matrix*/
    QTransform                matrix{};

    /** @brief layoutWidth value layout allowance width in pixels. */
    qreal                     layoutWidth{0};

    bool                      mirror{false};

    /** @brief detailLabel detail label rectangle */
    QVector<QPointF>          detailLabel{};

    /** @brief patternInfo pattern info rectangle */
    QVector<QPointF>          patternInfo{};

    /** @brief grainlineInfo line */
    QVector<QPointF>          grainlinePoints{};

    GrainlineArrowDirection                 grainlineArrowType{GrainlineArrowDirection::atFront};
    qreal                     grainlineAngle{0};
    bool                      grainlineEnabled{false};

    /** @brief m_tmDetail text manager for laying out detail info */
    VTextManager              m_tmDetail{};

    /** @brief m_tmPattern text manager for laying out pattern info */
    VTextManager              m_tmPattern{};

    /** @brief m_placeLabels list of place labels. */
    QVector<VLayoutPlaceLabel> m_placeLabels{};

    qint64 m_square{0};

    quint16 m_quantity{1};

    /** @brief m_id keep id of original piece. */
    vidtype                   m_id;

private:
    Q_DISABLE_ASSIGN(VLayoutPieceData)

    static const quint32 streamHeader;
    static const quint16 classVersion;
};

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
inline QDataStream &operator<<(QDataStream &dataStream, const VLayoutPieceData &piece)
{
    dataStream << VLayoutPieceData::streamHeader << VLayoutPieceData::classVersion;

    // Added in classVersion = 1
    dataStream << piece.contour;
    dataStream << piece.seamAllowance;
    dataStream << piece.layoutAllowance;
    dataStream << piece.passmarks;
    dataStream << piece.m_internalPaths;
    dataStream << piece.matrix;
    dataStream << piece.layoutWidth;
    dataStream << piece.mirror;
    dataStream << piece.detailLabel;
    dataStream << piece.patternInfo;
    dataStream << piece.grainlinePoints;
    dataStream << piece.grainlineArrowType;
    dataStream << piece.grainlineAngle;
    dataStream << piece.grainlineEnabled;
    dataStream << piece.m_placeLabels;
    dataStream << piece.m_square;

    // Added in classVersion = 2
    dataStream << piece.m_quantity;
    dataStream << piece.m_id;

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
inline QDataStream &operator>>(QDataStream &dataStream, VLayoutPieceData &piece)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VLayoutPieceData::streamHeader)
    {
        QString message = QCoreApplication::tr("VRawLayoutData prefix mismatch error: actualStreamHeader = 0x%1 and "
                                               "streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VLayoutPieceData::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VLayoutPieceData::classVersion)
    {
        QString message = QCoreApplication::tr("VRawLayoutData compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VLayoutPieceData::classVersion);
        throw VException(message);
    }

    dataStream >> piece.contour;
    dataStream >> piece.seamAllowance;
    dataStream >> piece.layoutAllowance;
    dataStream >> piece.passmarks;
    dataStream >> piece.m_internalPaths;
    dataStream >> piece.matrix;
    dataStream >> piece.layoutWidth;
    dataStream >> piece.mirror;
    dataStream >> piece.detailLabel;
    dataStream >> piece.patternInfo;
    dataStream >> piece.grainlinePoints;
    dataStream >> piece.grainlineArrowType;
    dataStream >> piece.grainlineAngle;
    dataStream >> piece.grainlineEnabled;
    dataStream >> piece.m_placeLabels;
    dataStream >> piece.m_square;

    if (actualClassVersion >= 2)
    {
        dataStream >> piece.m_quantity;
        dataStream >> piece.m_id;
    }

    return dataStream;
}

QT_WARNING_POP

#endif // VLAYOUTDETAIL_P_H
