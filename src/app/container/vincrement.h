/************************************************************************
 **
 **  @file   vincrementtablerow.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VINCREMENTTABLEROW_H
#define VINCREMENTTABLEROW_H

#include <QString>

/**
 * @brief The VIncrement class keep data row of increment table
 */
class VIncrement
{
public:
    VIncrement();
    VIncrement(quint32 id, qreal base, qreal ksize, qreal kheight, QString description = QString());
    VIncrement(const VIncrement &incr);
    VIncrement &operator=(const VIncrement &incr);
    quint32 getId() const;
    void    setId(const quint32 &value);
    qreal   getBase() const;
    void    setBase(const qreal &value);
    qreal   getKsize() const;
    void    setKsize(const qreal &value);
    qreal   getKheight() const;
    void    setKheight(const qreal &value);
    QString getDescription() const;
    void    setDescription(const QString &value);
    qreal   GetValue() const;
    qreal   GetValue(const qreal &size, const qreal &height) const;
private:
    /** @brief id identificator */
    quint32         id;

    /** @brief base value in base size and height */
    qreal          base;

    /** @brief ksize increment in sizes */
    qreal          ksize;

    /** @brief kgrowth increment in growths */
    qreal          kheight;

    /** @brief description description of increment */
    QString        description;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getId return id of row
 * @return id
 */
inline quint32 VIncrement::getId() const
{
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId set id of row
 * @param value id
 */
inline void VIncrement::setId(const quint32 &value)
{
    id = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getBase return value in base size and growth
 * @return value
 */
inline qreal VIncrement::getBase() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setBase set value in base size and growth
 * @param value base value
 */
inline void VIncrement::setBase(const qreal &value)
{
    base = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getKsize return increment in sizes
 * @return increment
 */
inline qreal VIncrement::getKsize() const
{
    return ksize;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setKsize set increment in sizes
 * @param value value of increment
 */
inline void VIncrement::setKsize(const qreal &value)
{
    ksize = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getKheight return increment in growths
 * @return increment
 */
inline qreal VIncrement::getKheight() const
{
    return kheight;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setKheight set increment in growths
 * @param value value of increment
 */
inline void VIncrement::setKheight(const qreal &value)
{
    kheight = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getDescription return description
 * @return description
 */
inline QString VIncrement::getDescription() const
{
    return description;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDescription set description for row
 * @param value description
 */
inline void VIncrement::setDescription(const QString &value)
{
    description = value;
}

#endif // VINCREMENTTABLEROW_H
