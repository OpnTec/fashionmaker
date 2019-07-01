/************************************************************************
 **
 **  @file   vdatastreamenum.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 12, 2018
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef DATASTREAM_ENUM_H
#define DATASTREAM_ENUM_H

#include <QDataStream>
#include <type_traits>

// (de)serialize enums into QDataStream

//a function that can serialize any enum into QDataStream
//it stores the enum in a qint64
template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator<<(QDataStream& stream, const Enum& e)
{
    stream << static_cast<qint64>(e);
    return stream;
}

//a function that can deserialize any enum from QDataStream
//it reads the enum as if it was stored in qint64
template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator>>(QDataStream& stream, Enum& e)
{
    qint64 v;
    stream >> v;
    e = static_cast<Enum>(v);
    return stream;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
template <typename Enum>
inline QDataStream &operator<<(QDataStream &s, QFlags<Enum> e)
{
    return s << static_cast<int>(e);
}

template <typename Enum>
inline QDataStream &operator>>(QDataStream &s, QFlags<Enum> &e)
{
    int v;
    s >> v;
    e = static_cast<QFlags<Enum>>(v);
}
#endif

#endif // DATASTREAM_ENUM_H
