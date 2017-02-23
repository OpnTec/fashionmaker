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

#ifndef VABSTRACTFLOATITEMDATA_P_H
#define VABSTRACTFLOATITEMDATA_P_H

#include <QPointF>
#include <QSharedData>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VAbstractFloatItemDataPrivate : public QSharedData
{
public:
    VAbstractFloatItemDataPrivate()
        : m_ptPos(),
          m_bVisible(false)
    {}

    VAbstractFloatItemDataPrivate(const VAbstractFloatItemDataPrivate &data)
        : QSharedData(data),
          m_ptPos(data.m_ptPos),
          m_bVisible(data.m_bVisible)
    {}

    ~VAbstractFloatItemDataPrivate();

    /** @brief m_ptPos position of label's top left corner */
    QPointF m_ptPos;
    /** @brief m_bVisible visibility flag */
    bool    m_bVisible;

private:
    VAbstractFloatItemDataPrivate &operator=(const VAbstractFloatItemDataPrivate &) Q_DECL_EQ_DELETE;
};

VAbstractFloatItemDataPrivate::~VAbstractFloatItemDataPrivate()
{}

QT_WARNING_POP

#endif // VABSTRACTFLOATITEMDATA_P_H

