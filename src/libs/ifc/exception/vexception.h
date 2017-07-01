/************************************************************************
 **
 **  @file   vexception.h
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

#ifndef VEXCEPTION_H
#define VEXCEPTION_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QException>
#include <QString>

#include "../ifcdef.h"

/**
 * @brief The VException class parent for all exception. Could be use for abstract exception
 */
class VException : public QException
{
    Q_DECLARE_TR_FUNCTIONS(VException)
public:
    explicit VException(const QString &error);
    VException(const VException &e);
    VException &operator=(const VException &e);
    virtual ~VException() V_NOEXCEPT_EXPR (true) Q_DECL_EQ_DEFAULT;

    Q_NORETURN virtual void raise() const Q_DECL_OVERRIDE;

    // cppcheck-suppress unusedFunction
    Q_REQUIRED_RESULT virtual VException *clone() const Q_DECL_OVERRIDE;

    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    QString         WhatUtf8() const V_NOEXCEPT_EXPR (true);
    void            AddMoreInformation(const QString &info);
    QString         MoreInformation() const;
    virtual const char* what() const V_NOEXCEPT_EXPR (true) Q_DECL_OVERRIDE;

protected:
    /** @brief error string with error */
    QString         error;

    /** @brief moreInfo more information about error */
    QString         moreInfo;

    QString         MoreInfo(const QString &detInfo) const;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief What return string with error
 * @return string with error
 */
inline QString VException::WhatUtf8() const V_NOEXCEPT_EXPR (true)
{
    return error;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MoreInformation return more information for error
 * @return information
 */
inline QString VException::MoreInformation() const
{
    return moreInfo;
}

// Want have special exception for catching unhadled deleting a tool
class VExceptionToolWasDeleted : public VException
{
    Q_DECLARE_TR_FUNCTIONS(VExceptionToolDeleted)
public:
    explicit VExceptionToolWasDeleted(const QString &error);
    VExceptionToolWasDeleted(const VExceptionToolWasDeleted &e);
    VExceptionToolWasDeleted &operator=(const VExceptionToolWasDeleted &e);
    virtual ~VExceptionToolWasDeleted() V_NOEXCEPT_EXPR (true) Q_DECL_EQ_DEFAULT;

    Q_NORETURN virtual void raise() const Q_DECL_OVERRIDE;
    // cppcheck-suppress unusedFunction
    virtual VExceptionToolWasDeleted *clone() const Q_DECL_OVERRIDE;
};

#endif // VEXCEPTION_H
