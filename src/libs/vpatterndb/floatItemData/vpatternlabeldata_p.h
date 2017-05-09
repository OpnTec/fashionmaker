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

#ifndef VPATTERNLABELDATA_P_H
#define VPATTERNLABELDATA_P_H

#include <QPointF>
#include <QSharedData>

#include "../vmisc/diagnostic.h"
#include "../ifc/ifcdef.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VPatternLabelDataPrivate : public QSharedData
{
public:
    VPatternLabelDataPrivate()
        : m_dLabelWidth(),
          m_dLabelHeight(),
          m_dLabelAngle(),
          m_iFontSize(0),
          m_centerPin(NULL_ID),
          m_topLeftPin(NULL_ID),
          m_bottomRightPin(NULL_ID)
    {}

    VPatternLabelDataPrivate(const VPatternLabelDataPrivate &data)
        : QSharedData(data),
          m_dLabelWidth(data.m_dLabelWidth),
          m_dLabelHeight(data.m_dLabelHeight),
          m_dLabelAngle(data.m_dLabelAngle),
          m_iFontSize(data.m_iFontSize),
          m_centerPin(data.m_centerPin),
          m_topLeftPin(data.m_topLeftPin),
          m_bottomRightPin(data.m_bottomRightPin)
    {}

    ~VPatternLabelDataPrivate() Q_DECL_EQ_DEFAULT;

    /** @brief m_dLabelWidth formula to calculate the width of label */
    QString m_dLabelWidth;
    /** @brief m_dLabelHeight formula to calculate the height of label */
    QString m_dLabelHeight;
    /** @brief m_dLabelAngle formula to calculate the rotation angle of label */
    QString m_dLabelAngle;
    /** @brief m_iFontSize label text base font size */
    int     m_iFontSize;
    /** @brief m_centerPin center pin id */
    quint32 m_centerPin;
    /** @brief m_topLeftPin top left corner pin id */
    quint32 m_topLeftPin;
    /** @brief m_bottomRightPin bottom right corner pin id */
    quint32 m_bottomRightPin;

private:
    VPatternLabelDataPrivate &operator=(const VPatternLabelDataPrivate &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VPATTERNLABELDATA_P_H

