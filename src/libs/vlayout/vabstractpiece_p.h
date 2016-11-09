/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VAbstractPieceData : public QSharedData
{
public:
    VAbstractPieceData()
        : m_name()
    {}

    VAbstractPieceData(const VAbstractPieceData &piece)
        : QSharedData(piece),
          m_name(piece.m_name)
    {}

    ~VAbstractPieceData();

    QString m_name;

private:
    VAbstractPieceData &operator=(const VAbstractPieceData &) Q_DECL_EQ_DELETE;
};

VAbstractPieceData::~VAbstractPieceData()
{}

QT_WARNING_POP

#endif // VABSTRACTPIECE_P_H

