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
#include <QVector>

#include "../vmisc/diagnostic.h"
#include "floatitemdef.h"
#include "../ifc/ifcdef.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VPieceLabelDataPrivate : public QSharedData
{
public:
    VPieceLabelDataPrivate()
        : m_qsLetter(),
          m_annotation(),
          m_orientation(),
          m_rotation(),
          m_tilt(),
          m_foldPosition(),
          m_quantity(1),
          m_onFold(false),
          m_lines()
    {}

    VPieceLabelDataPrivate(const VPieceLabelDataPrivate &data)
        : QSharedData(data),
          m_qsLetter(data.m_qsLetter),
          m_annotation(data.m_annotation),
          m_orientation(data.m_orientation),
          m_rotation(data.m_rotation),
          m_tilt(data.m_tilt),
          m_foldPosition(data.m_foldPosition),
          m_quantity(data.m_quantity),
          m_onFold(data.m_onFold),
          m_lines(data.m_lines)
    {}

    ~VPieceLabelDataPrivate() Q_DECL_EQ_DEFAULT;

    /** @brief m_qsLetter Detail letter (should be no more than 3 characters) */
    QString m_qsLetter;
    QString m_annotation;
    QString m_orientation;
    QString m_rotation;
    QString m_tilt;
    QString m_foldPosition;

    int  m_quantity;
    bool m_onFold;

    QVector<VLabelTemplateLine> m_lines;

private:
    VPieceLabelDataPrivate &operator=(const VPieceLabelDataPrivate &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VPIECELABELDATA_P_H

