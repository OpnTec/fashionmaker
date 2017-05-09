/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VPIECELABELDATA_P_H
#define VPIECELABELDATA_P_H

#include <QSharedData>
#include <QString>

#include "../vmisc/diagnostic.h"
#include "floatitemdef.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VPieceLabelDataPrivate : public QSharedData
{
public:
    VPieceLabelDataPrivate()
        : m_qsLetter(),
          m_conMCP()
    {}

    VPieceLabelDataPrivate(const VPieceLabelDataPrivate &data)
        : QSharedData(data),
          m_qsLetter(data.m_qsLetter),
          m_conMCP(data.m_conMCP)
    {}

    ~VPieceLabelDataPrivate() Q_DECL_EQ_DEFAULT;

    /** @brief m_qsLetter Detail letter (should be no more than 3 characters) */
    QString      m_qsLetter;
    /** @brief m_conMCP List of material, cut, placement tuples */
    MCPContainer m_conMCP;

private:
    VPieceLabelDataPrivate &operator=(const VPieceLabelDataPrivate &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VPIECELABELDATA_P_H

