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

#ifndef VEXCEPTIONBADID_H
#define VEXCEPTIONBADID_H

#include "vexception.h"

class VExceptionBadId : public VException{
public:
                    VExceptionBadId(const QString &what, const qint64 &id):VException(what), id(id),
                        key(QString()){}
                    VExceptionBadId(const QString &what, const QString &key):VException(what), id(0), key(key){}
                    VExceptionBadId(const VExceptionBadId &e):VException(e), id(e.BadId()), key(e.BadKey()){}
    virtual         ~VExceptionBadId() noexcept(true){}
    virtual QString ErrorMessage() const;
    inline qint64   BadId() const {return id; }
    inline QString  BadKey() const {return key; }
protected:
    qint64          id;
    QString         key;
};

#endif // VEXCEPTIONBADID_H
