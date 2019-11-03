/************************************************************************
 **
 **  @file   vrawsapoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 10, 2019
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
#ifndef VRAWSAPOINT_H
#define VRAWSAPOINT_H

#include <QPointF>

#include "../vmisc/diagnostic.h"
#include "../vmisc/def.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VRawSAPoint : public QPointF
{
public:
    Q_DECL_CONSTEXPR VRawSAPoint();
    Q_DECL_CONSTEXPR VRawSAPoint(qreal xpos, qreal ypos);
    // cppcheck-suppress noExplicitConstructor
    Q_DECL_CONSTEXPR VRawSAPoint(QPointF p);

    Q_DECL_CONSTEXPR bool LoopPoint() const;
    Q_DECL_RELAXED_CONSTEXPR void SetLoopPoint(bool loopPoint);

    QJsonObject toJson() const;

private:
    bool m_loopPoint{false};
};

Q_DECLARE_METATYPE(VRawSAPoint)
Q_DECLARE_TYPEINFO(VRawSAPoint, Q_MOVABLE_TYPE);

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VRawSAPoint::VRawSAPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VRawSAPoint::VRawSAPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos)
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress passedByValue
Q_DECL_CONSTEXPR inline VRawSAPoint::VRawSAPoint(QPointF p)
    : QPointF(p)
{

}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline bool VRawSAPoint::LoopPoint() const
{
    return m_loopPoint;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_RELAXED_CONSTEXPR inline void VRawSAPoint::SetLoopPoint(bool loopPoint)
{
    m_loopPoint = loopPoint;
}

QT_WARNING_POP

#endif // VRAWSAPOINT_H
