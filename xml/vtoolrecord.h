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

#ifndef VTOOLRECORD_H
#define VTOOLRECORD_H

#include <QString>
#include "options.h"

class VToolRecord
{
public:
    VToolRecord();
    VToolRecord(const qint64 &id, const Tool::Tools &typeTool, const QString &nameDraw);
    qint64 getId() const;
    void setId(const qint64 &value);

    Tool::Tools getTypeTool() const;
    void setTypeTool(const Tool::Tools &value);

    QString getNameDraw() const;
    void setNameDraw(const QString &value);

private:
    qint64 id;
    Tool::Tools typeTool;
    QString nameDraw;
};

#endif // VTOOLRECORD_H
