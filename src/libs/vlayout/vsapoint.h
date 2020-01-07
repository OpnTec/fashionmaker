/************************************************************************
 **
 **  @file   vsapoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 9, 2019
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
#ifndef VSAPOINT_H
#define VSAPOINT_H

#include "../vmisc/diagnostic.h"
#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"

#include <QPointF>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

/**
 * @brief The VSAPoint class seam allowance point
 */
class VSAPoint : public QPointF
{
public:
    Q_DECL_CONSTEXPR VSAPoint() V_NOEXCEPT_EXPR (true);
    Q_DECL_CONSTEXPR VSAPoint(qreal xpos, qreal ypos);
    Q_DECL_CONSTEXPR explicit VSAPoint(QPointF p);

    Q_DECL_CONSTEXPR qreal GetSABefore() const;
                     qreal GetSABefore(qreal width) const;
                     void  SetSABefore(qreal value);

    Q_DECL_CONSTEXPR qreal GetSAAfter() const;
                     qreal GetSAAfter(qreal width) const;
                     void  SetSAAfter(qreal value);

    Q_DECL_CONSTEXPR PieceNodeAngle GetAngleType() const;
                     void           SetAngleType(PieceNodeAngle value);

    Q_DECL_CONSTEXPR bool IsManualPasskmarkLength() const;
    Q_DECL_RELAXED_CONSTEXPR void SetManualPasskmarkLength(bool value);

    Q_DECL_CONSTEXPR qreal GetPasskmarkLength() const;
    Q_DECL_RELAXED_CONSTEXPR void  SetPasskmarkLength(qreal value);

    qreal MaxLocalSA(qreal width) const;
    qreal PassmarkLength(qreal width) const;

    QJsonObject toJson() const;

    static const qreal passmarkFactor;
    static const qreal maxPassmarkLength;
    static const qreal minSAWidth;

private:
    qreal          m_before{-1};
    qreal          m_after{-1};
    PieceNodeAngle m_angle{PieceNodeAngle::ByLength};
    bool           m_manualPassmarkLength{false};
    qreal          m_passmarkLength{0};
};

Q_DECLARE_METATYPE(VSAPoint)
Q_DECLARE_TYPEINFO(VSAPoint, Q_MOVABLE_TYPE);

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint() V_NOEXCEPT_EXPR (true)
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos)
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress passedByValue
Q_DECL_CONSTEXPR inline VSAPoint::VSAPoint(QPointF p)
    : QPointF(p)
{}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetSABefore() const
{
    return m_before;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSABefore(qreal value)
{
    value < 0 ? m_before = -1 : m_before = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetSAAfter() const
{
    return m_after;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSAAfter(qreal value)
{
    value < 0 ? m_after = -1 : m_after = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline PieceNodeAngle VSAPoint::GetAngleType() const
{
    return m_angle;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetAngleType(PieceNodeAngle value)
{
    m_angle = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline bool VSAPoint::IsManualPasskmarkLength() const
{
    return m_manualPassmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_RELAXED_CONSTEXPR inline void VSAPoint::SetManualPasskmarkLength(bool value)
{
    m_manualPassmarkLength = value;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qreal VSAPoint::GetPasskmarkLength() const
{
    return m_passmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_RELAXED_CONSTEXPR inline void VSAPoint::SetPasskmarkLength(qreal value)
{
    m_passmarkLength = value;
}

QT_WARNING_POP

#endif // VSAPOINT_H
