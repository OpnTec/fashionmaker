/************************************************************************
 **
 **  @file   vstandardtablecell.h
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

#ifndef VSTANDARDTABLEROW_H
#define VSTANDARDTABLEROW_H

#include <QString>

/**
 * @brief The VStandardTableRow class keep data row of standard table
 */
class VStandardTableRow
{
public:
                   /**
                    * @brief VStandardTableRow create empty row
                    */
                   VStandardTableRow();
                   /**
                    * @brief VStandardTableRow create row
                    * @param base value in base size and growth
                    * @param ksize increment in sizes
                    * @param kgrowth increment in growths
                    * @param description description of increment
                    */
                   VStandardTableRow(const qreal &base, const qreal &ksize, const qreal &kgrowth,
                                     const QString &gui_text = QString(), const QString &number = QString());
                   ~VStandardTableRow(){}
    /**
     * @brief GetBase return value in base size and growth
     * @return value
     */
    qreal   GetBase() const;
    /**
     * @brief GetKsize return increment in sizes
     * @return increment
     */
    qreal   GetKsize() const;
    /**
     * @brief GetKgrowth return increment in growths
     * @return increment
     */
    qreal   GetKgrowth() const;
    /**
     * @brief GetDescription return description
     * @return description
     */
    QString GetDescription() const;
    QString GetNumber() const;
private:
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
     * @brief description description measurement
     */
    QString        gui_text;
    QString        number;
};

inline qreal VStandardTableRow::GetBase() const
{
    return base;
}

inline qreal VStandardTableRow::GetKsize() const
{
    return ksize;
}

inline qreal VStandardTableRow::GetKgrowth() const
{
    return kgrowth;
}

inline QString VStandardTableRow::GetDescription() const
{
    return gui_text;
}

inline QString VStandardTableRow::GetNumber() const
{
    return number;
}

#endif // VSTANDARDTABLEROW_H
