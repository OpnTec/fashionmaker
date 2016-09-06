/****************************************************************************
**
** Copyright (C) 2013 Digia Plc.
** Contact: http://www.qt-project.org/legal
**
** $QT_BEGIN_LICENSE:GPL$
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef VMATH_H
#define VMATH_H

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
#include <QtMath>
#else
#include "qmath.h"

Q_DECL_CONSTEXPR inline float qDegreesToRadians(float degrees)
{
    return degrees * float(M_PI/180);
}

Q_DECL_CONSTEXPR inline double qDegreesToRadians(double degrees)
{
    return degrees * (M_PI / 180);
}

Q_DECL_CONSTEXPR inline float qRadiansToDegrees(float radians)
{
    return radians * float(180/M_PI);
}

Q_DECL_CONSTEXPR inline double qRadiansToDegrees(double radians)
{
    return radians * (180 / M_PI);
}

#endif // QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifndef M_2PI
#define M_2PI (6.28318530717958647692) /* 2*pi */
#endif

#ifndef M_3PI_4
#define M_3PI_4 (2.35619449019234492884) /* 3*pi/4 */
#endif

#ifndef M_3PI_8
#define M_3PI_8 (1.17809724509617246442) /* 3*pi/8 */
#endif

#ifndef M_PI_8
#define M_PI_8 (0.39269908169872415480) /* pi/8 */
#endif

#ifndef M_4_PI
#define M_4_PI (1.27323954473516268615) /* 4/pi */
#endif

#ifndef M_1_SQRT2
#define M_1_SQRT2 (0.70710678118654752440) /* 1/sqrt(2) */
#endif

#ifndef M_EULER
#define M_EULER (0.57721566490153286060)
#endif

#endif // VMATH_H

