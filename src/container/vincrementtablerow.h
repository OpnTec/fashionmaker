/************************************************************************
 **
 **  @file   vincrementtablerow.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
 * @brief The VIncrementTableRow class keep data row of increment table
 */
class VIncrementTableRow
{
public:
                   /**
                    * @brief VIncrementTableRow create enpty row
                    */
                   VIncrementTableRow();
                   /**
                    * @brief VIncrementTableRow create row
                    * @param id id
                    * @param base value in base size and growth
                    * @param ksize increment in sizes
                    * @param kgrowth increment in growths
                    * @param description description of increment
                    */
                   VIncrementTableRow(quint32 id, qreal base, qreal ksize, qreal kgrowth,
                                      QString description = QString());
    /**
     * @brief getId return id of row
     * @return id
     */
    quint32 getId() const;
    /**
     * @brief setId set id of row
     * @param value id
     */
    void    setId(const quint32 &value);
    /**
     * @brief getBase return value in base size and growth
     * @return value
     */
    qreal   getBase() const;
    /**
     * @brief setBase set value in base size and growth
     * @param value base value
     */
    void    setBase(const qreal &value);
    /**
     * @brief getKsize return increment in sizes
     * @return increment
     */
    qreal   getKsize() const;
    /**
     * @brief setKsize set increment in sizes
     * @param value value of increment
     */
    void    setKsize(const qreal &value);
    /**
     * @brief getKgrowth return increment in growths
     * @return increment
     */
    qreal   getKgrowth() const;
    /**
     * @brief setKgrowth set increment in growths
     * @param value value of increment
     */
    void    setKgrowth(const qreal &value);
    /**
     * @brief getDescription return description
     * @return description
     */
    QString getDescription() const;
    /**
     * @brief setDescription set description for row
     * @param value description
     */
    void    setDescription(const QString &value);
private:
    /**
     * @brief id identificator
     */
    quint32         id;
    /**
     * @brief base value in base size and growth
     */
    qreal          base;
    /**
     * @brief ksize increment in sizes
     */
    qreal          ksize;
    /**
     * @brief kgrowth increment in growths
     */
    qreal          kgrowth;
    /**
     * @brief description description of increment
     */
    QString        description;
};

inline quint32 VIncrementTableRow::getId() const
{
    return id;
}

inline void VIncrementTableRow::setId(const quint32 &value)
{
    id = value;
}

inline qreal VIncrementTableRow::getBase() const
{
    return base;
}

inline void VIncrementTableRow::setBase(const qreal &value)
{
    base = value;
}

inline qreal VIncrementTableRow::getKsize() const
{
    return ksize;
}

inline void VIncrementTableRow::setKsize(const qreal &value)
{
    ksize = value;
}

inline qreal VIncrementTableRow::getKgrowth() const
{
    return kgrowth;
}

inline void VIncrementTableRow::setKgrowth(const qreal &value)
{
    kgrowth = value;
}

inline QString VIncrementTableRow::getDescription() const
{
    return description;
}

inline void VIncrementTableRow::setDescription(const QString &value)
{
    description = value;
}

#endif // VINCREMENTTABLEROW_H
