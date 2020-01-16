/************************************************************************
 **
 **  @file   compatibility.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 1, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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
#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#include <QtGlobal>
#include <QStringList>
#include <QSet>

class QPointF;

// Contains helpful methods to hide version dependent code. It can be deprecation of method or change in API

//---------------------------------------------------------------------------------------------------------------------
template <typename T, template <typename> class Cont>
inline const T& ConstFirst (const Cont<T> &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    return container.constFirst();
#else
    return container.first(); // clazy:exclude=detaching-temporary
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T, typename C>
inline const T& ConstFirst (const C &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    return container.constFirst();
#else
    return container.first(); // clazy:exclude=detaching-temporary
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline typename T::IntersectType Intersects(const T &l1, const T &l2, QPointF *intersectionPoint)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return l1.intersects(l2, intersectionPoint);
#else
    return l1.intersect(l2, intersectionPoint);
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T, template <typename> class C>
inline QList<T> ConvertToList(const C<T> &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return T(container.begin(), container.end());
#else
    return container.toList();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T, template <typename> class C>
inline QSet<T> ConvertToSet(const C<T> &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return T(container.begin(), container.end());
#else
    return container.toSet();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T, typename C>
inline QSet<T> ConvertToSet(const C &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return T(container.begin(), container.end());
#else
    return container.toSet();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T, template <typename> class C>
inline QVector<T> ConvertToVector(const C<T> &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return QVector<T>(container.begin(), container.end());
#else
    return container.toVector();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline QVector<T> ConvertToVector(const QSet<T> &container)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return QVector<T>(container.begin(), container.end());
#else
    return container.toList().toVector();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void SwapItemsAt(T &container, int i, int j)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    container.swapItemsAt(i, j);
#else
    container.swap(i, j);
#endif
}

#endif // COMPATIBILITY_H
