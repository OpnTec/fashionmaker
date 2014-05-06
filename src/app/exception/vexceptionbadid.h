/************************************************************************
 **
 **  @file   vexceptionbadid.h
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

#ifndef VEXCEPTIONBADID_H
#define VEXCEPTIONBADID_H

#include "vexception.h"

/**
 * @brief The VExceptionBadId class for exception bad id
 */
class VExceptionBadId : public VException
{
public:
                    /**
                     * @brief VExceptionBadId exception bad id
                     * @param what string with error
                     * @param id id
                     */
                    VExceptionBadId(const QString &what, const quint32 &id)
                        :VException(what), id(id), key(QString()){}
                    /**
                     * @brief VExceptionBadId exception bad id
                     * @param what string with error
                     * @param key string key
                     */
                    VExceptionBadId(const QString &what, const QString &key)
                        :VException(what), id(0), key(key){}
                    /**
                     * @brief VExceptionBadId copy constructor
                     * @param e exception
                     */
                    VExceptionBadId(const VExceptionBadId &e)
                        :VException(e), id(e.BadId()), key(e.BadKey()){}
    virtual         ~VExceptionBadId() noexcept (true){}
    /**
     * @brief ErrorMessage return main error message
     * @return main error message
     */
    virtual QString ErrorMessage() const;
    /**
     * @brief BadId return bad id
     * @return id
     */
    quint32         BadId() const;
    /**
     * @brief BadKey return bad key
     * @return key
     */
    QString         BadKey() const;
protected:
    /**
     * @brief id id
     */
    quint32          id;
    /**
     * @brief key key
     */
    QString         key;
};

inline quint32 VExceptionBadId::BadId() const
{
    return id;
}

inline QString VExceptionBadId::BadKey() const
{
    return key;
}

#endif // VEXCEPTIONBADID_H
