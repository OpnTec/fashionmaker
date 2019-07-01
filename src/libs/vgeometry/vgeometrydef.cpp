/************************************************************************
 **
 **  @file   vgeometrydef.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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

#include "vgeometrydef.h"

#include "../vmisc/vdatastreamenum.h"
#include "../ifc/exception/vexception.h"

#include <QCoreApplication>

const quint32 VLayoutPassmark::streamHeader = 0x943E2759; // CRC-32Q string "VLayoutPassmark"
const quint16 VLayoutPassmark::classVersion = 1;

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &dataStream, const VLayoutPassmark &data)
{
    dataStream << VLayoutPassmark::streamHeader << VLayoutPassmark::classVersion;

    // Added in classVersion = 1
    dataStream << data.lines;
    dataStream << data.type;
    dataStream << data.baseLine;
    dataStream << data.isBuiltIn;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &dataStream, VLayoutPassmark &data)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VLayoutPassmark::streamHeader)
    {
        QString message = QCoreApplication::tr("VLayoutPassmark prefix mismatch error: actualStreamHeader = 0x%1 and "
                                               "streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VLayoutPassmark::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VLayoutPassmark::classVersion)
    {
        QString message = QCoreApplication::tr("VLayoutPassmark compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VLayoutPassmark::classVersion);
        throw VException(message);
    }

    dataStream >> data.lines;
    dataStream >> data.type;
    dataStream >> data.baseLine;
    dataStream >> data.isBuiltIn;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}

const quint32 VLayoutPlaceLabel::streamHeader = 0xB282E284; // CRC-32Q string "VLayoutPlaceLabel"
const quint16 VLayoutPlaceLabel::classVersion = 1;

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream &dataStream, const VLayoutPlaceLabel &data)
{
    dataStream << VLayoutPlaceLabel::streamHeader << VLayoutPlaceLabel::classVersion;

    // Added in classVersion = 1
    dataStream << data.center;
    dataStream << data.type;
    dataStream << data.shape;
    dataStream << data.rotationMatrix;
    dataStream << data.box;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator>>(QDataStream &dataStream, VLayoutPlaceLabel &data)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VLayoutPlaceLabel::streamHeader)
    {
        QString message = QCoreApplication::tr("VLayoutPlaceLabel prefix mismatch error: actualStreamHeader = 0x%1 and "
                                               "streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VLayoutPlaceLabel::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VLayoutPlaceLabel::classVersion)
    {
        QString message = QCoreApplication::tr("VLayoutPlaceLabel compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VLayoutPlaceLabel::classVersion);
        throw VException(message);
    }

    dataStream >> data.center;
    dataStream >> data.type;
    dataStream >> data.shape;
    dataStream >> data.rotationMatrix;
    dataStream >> data.box;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}
