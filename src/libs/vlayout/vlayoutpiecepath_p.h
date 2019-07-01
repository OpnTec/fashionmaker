/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vmisc/vdatastreamenum.h"
#include "../ifc/exception/vexception.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VLayoutPiecePathData : public QSharedData
{
public:
    VLayoutPiecePathData()
    {}

    explicit VLayoutPiecePathData(const QVector<QPointF> &points)
        : m_points(points)
    {}

    VLayoutPiecePathData(const VLayoutPiecePathData &path)
        : QSharedData(path),
          m_points(path.m_points),
          m_penStyle(path.m_penStyle),
          m_cut(path.m_cut)
    {}

    ~VLayoutPiecePathData() Q_DECL_EQ_DEFAULT;

    friend QDataStream& operator<<(QDataStream& dataStream, const VLayoutPiecePathData& path);
    friend QDataStream& operator>>(QDataStream& dataStream, VLayoutPiecePathData& path);

    /** @brief m_points list of path points. */
    QVector<QPointF> m_points{};

    /** @brief m_penStyle path pen style. */
    Qt::PenStyle     m_penStyle{Qt::SolidLine};

    bool             m_cut{false};

private:
    VLayoutPiecePathData &operator=(const VLayoutPiecePathData &) Q_DECL_EQ_DELETE;

    static const quint32 streamHeader;
    static const quint16 classVersion;
};

QT_WARNING_POP

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream &dataStream, const VLayoutPiecePathData &path)
{
    dataStream << VLayoutPiecePathData::streamHeader << VLayoutPiecePathData::classVersion;

    // Added in classVersion = 1
    dataStream << path.m_points;
    dataStream << path.m_penStyle;
    dataStream << path.m_cut;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator>>(QDataStream &dataStream, VLayoutPiecePathData &path)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VLayoutPiecePathData::streamHeader)
    {
        QString message = QCoreApplication::tr("VLayoutPiecePathData prefix mismatch error: actualStreamHeader = 0x%1 "
                                               "and streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VLayoutPiecePathData::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VLayoutPiecePathData::classVersion)
    {
        QString message = QCoreApplication::tr("VLayoutPiecePathData compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VLayoutPiecePathData::classVersion);
        throw VException(message);
    }

    dataStream >> path.m_points;
    dataStream >> path.m_penStyle;
    dataStream >> path.m_cut;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}

#endif // VLAYOUTPIECEPATH_P_H

