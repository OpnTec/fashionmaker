/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VINCREMENTTABLEROW_H
#define VINCREMENTTABLEROW_H

#include <QString>

class VIncrementTableRow
{
public:
    VIncrementTableRow();
    VIncrementTableRow(qint64 id, qreal base, qreal ksize, qreal kgrowth,
                        QString description = QString());
    qint64 getId() const;
    void setId(const qint64 &value);

    qreal getBase() const;
    void setBase(const qreal &value);

    qreal getKsize() const;
    void setKsize(const qreal &value);

    qreal getKgrowth() const;
    void setKgrowth(const qreal &value);

    QString getDescription() const;
    void setDescription(const QString &value);

private:
    qint64 id;
    qreal base;
    qreal ksize;
    qreal kgrowth;
    QString description;
};

#endif // VINCREMENTTABLEROW_H
