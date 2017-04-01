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
 **  Copyright (C) 2013-2015 Valentina project
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

#include <qcompilerdetection.h>
#include <QString>
#include <QtGlobal>

#include "../ifcdef.h"
#include "vexception.h"

/**
 * @brief The VExceptionBadId class for exception bad id
 */
class VExceptionBadId : public VException
{
public:
    VExceptionBadId(const QString &error, const quint32 &id);
    VExceptionBadId(const QString &error, const QString &key);
    VExceptionBadId(const VExceptionBadId &e);
    VExceptionBadId &operator=(const VExceptionBadId &e);
    virtual         ~VExceptionBadId() V_NOEXCEPT_EXPR (true) Q_DECL_EQ_DEFAULT;
    virtual QString ErrorMessage() const Q_DECL_OVERRIDE;
    quint32         BadId() const;
    QString         BadKey() const;
protected:
    /** @brief id id */
    quint32          id;

    /** @brief key key */
    QString         key;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BadId return bad id
 * @return id
 */
inline quint32 VExceptionBadId::BadId() const
{
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BadKey return bad key
 * @return key
 */
inline QString VExceptionBadId::BadKey() const
{
    return key;
}

#endif // VEXCEPTIONBADID_H
