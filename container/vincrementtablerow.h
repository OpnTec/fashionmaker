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

class VIncrementTableRow
{
public:
                   VIncrementTableRow();
                   VIncrementTableRow(qint64 id, qreal base, qreal ksize, qreal kgrowth,
                                      QString description = QString());
    inline qint64  getId() const {return id;}
    inline void    setId(const qint64 &value) {id = value;}
    inline qreal   getBase() const {return base;}
    inline void    setBase(const qreal &value) {base = value;}
    inline qreal   getKsize() const {return ksize;}
    inline void    setKsize(const qreal &value) {ksize = value;}
    inline qreal   getKgrowth() const {return kgrowth;}
    inline void    setKgrowth(const qreal &value) {kgrowth = value;}
    inline QString getDescription() const {return description;}
    inline void    setDescription(const QString &value) {description = value;}
private:
    qint64         id;
    qreal          base;
    qreal          ksize;
    qreal          kgrowth;
    QString        description;
};

#endif // VINCREMENTTABLEROW_H
