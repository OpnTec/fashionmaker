/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VABSTRACTPIECE_P_H
#define VABSTRACTPIECE_P_H

#include <QSharedData>
#include <QString>
#include <QCoreApplication>

#include "../vmisc/diagnostic.h"
#include "../vmisc/vdatastreamenum.h"
#include "../vmisc/defglobal.h"
#include "../ifc/exception/vexception.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VAbstractPieceData : public QSharedData
{
    Q_DECLARE_TR_FUNCTIONS(VAbstractPieceData)
public:
    VAbstractPieceData()
        : m_name(tr("Detail")),
          m_forbidFlipping(false),
          m_forceFlipping(false),
          m_seamAllowance(false),
          m_seamAllowanceBuiltIn(false),
          m_hideMainPath(false),
          m_width(0),
          m_mx(0),
          m_my(0)
    {}

    VAbstractPieceData(const VAbstractPieceData &piece)
        : QSharedData(piece),
          m_name(piece.m_name),
          m_forbidFlipping(piece.m_forbidFlipping),
          m_forceFlipping(piece.m_forceFlipping),
          m_seamAllowance(piece.m_seamAllowance),
          m_seamAllowanceBuiltIn(piece.m_seamAllowanceBuiltIn),
          m_hideMainPath(piece.m_hideMainPath),
          m_width(piece.m_width),
          m_mx(piece.m_mx),
          m_my(piece.m_my)
    {}

    ~VAbstractPieceData() Q_DECL_EQ_DEFAULT;

    friend QDataStream& operator<<(QDataStream& dataStream, const VAbstractPieceData& piece);
    friend QDataStream& operator>>(QDataStream& dataStream, VAbstractPieceData& piece);

    QString m_name;
    /** @brief forbidFlipping forbid piece be mirrored in a layout. */
    bool    m_forbidFlipping;
    bool    m_forceFlipping;
    bool    m_seamAllowance;
    bool    m_seamAllowanceBuiltIn;
    bool    m_hideMainPath;
    qreal   m_width;
    qreal   m_mx;
    qreal   m_my;

private:
    Q_DISABLE_ASSIGN(VAbstractPieceData)

    static const quint32 streamHeader;
    static const quint16 classVersion;
};

QT_WARNING_POP

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
inline QDataStream &operator<<(QDataStream &dataStream, const VAbstractPieceData &piece)
{
    dataStream << VAbstractPieceData::streamHeader << VAbstractPieceData::classVersion;

    // Added in classVersion = 1
    dataStream << piece.m_name;
    dataStream << piece.m_forbidFlipping;
    dataStream << piece.m_forceFlipping;
    dataStream << piece.m_seamAllowance;
    dataStream << piece.m_seamAllowanceBuiltIn;
    dataStream << piece.m_hideMainPath;
    dataStream << piece.m_width;
    dataStream << piece.m_mx;
    dataStream << piece.m_my;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
inline QDataStream &operator>>(QDataStream &dataStream, VAbstractPieceData &piece)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VAbstractPieceData::streamHeader)
    {
        QString message = QCoreApplication::tr("VAbstractPieceData prefix mismatch error: actualStreamHeader = 0x%1 "
                                               "and streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VAbstractPieceData::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VAbstractPieceData::classVersion)
    {
        QString message = QCoreApplication::tr("VAbstractPieceData compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VAbstractPieceData::classVersion);
        throw VException(message);
    }

    dataStream >> piece.m_name;
    dataStream >> piece.m_forbidFlipping;
    dataStream >> piece.m_forceFlipping;
    dataStream >> piece.m_seamAllowance;
    dataStream >> piece.m_seamAllowanceBuiltIn;
    dataStream >> piece.m_hideMainPath;
    dataStream >> piece.m_width;
    dataStream >> piece.m_mx;
    dataStream >> piece.m_my;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}

#endif // VABSTRACTPIECE_P_H

