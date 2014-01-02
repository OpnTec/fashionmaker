/************************************************************************
 **
 **  @file   vstandarttablecell.h
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

#ifndef VSTANDARTTABLEROW_H
#define VSTANDARTTABLEROW_H

#include <QString>

/**
 * @brief The VStandartTableRow class keep data row of standart table
 */
class VStandartTableRow
{
public:
                   /**
                    * @brief VStandartTableRow create empty row
                    */
                   VStandartTableRow();
                   /**
                    * @brief VStandartTableRow create row
                    * @param base value in base size and growth
                    * @param ksize increment in sizes
                    * @param kgrowth increment in growths
                    * @param description description of increment
                    */
                   VStandartTableRow(const qint32 &base, const qreal &ksize, const qreal &kgrowth,
                                     const QString &description = QString());
                   ~VStandartTableRow(){}
    /**
     * @brief GetBase return value in base size and growth
     * @return value
     */
    inline qint32  GetBase() const {return base;}
    /**
     * @brief GetKsize return increment in sizes
     * @return increment
     */
    inline qreal   GetKsize() const {return ksize;}
    /**
     * @brief GetKgrowth return increment in growths
     * @return increment
     */
    inline qreal   GetKgrowth() const {return kgrowth;}
    /**
     * @brief GetDescription return description
     * @return description
     */
    inline QString GetDescription() const {return description;}
private:
    /**
     * @brief base value in base size and growth
     */
    qint32         base;
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
    QString        description;
};

#endif // VSTANDARTTABLEROW_H
