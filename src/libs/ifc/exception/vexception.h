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

#ifndef VEXCEPTION_H
#define VEXCEPTION_H

#include <QException>
#include <QString>
#include "ifcdef.h"

class QWidget;

/**
 * @brief The VException class parent for all exception. Could be use for abstract exception
 */
class VException : public QException
{
    Q_DECLARE_TR_FUNCTIONS(VException)
public:
    VException(const QString &what);
    VException(const VException &e);
    VException &operator=(const VException &e);
    virtual ~VException() V_NOEXCEPT_EXPR (true){}

    Q_NORETURN virtual void raise() const;
    // cppcheck-suppress unusedFunction
    virtual VException *clone() const;
    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    QString         What() const;
    virtual void    CriticalMessageBox(const QString &situation, QWidget *parent = nullptr) const;
    void            AddMoreInformation(const QString &info);
    QString         MoreInformation() const;
protected:
    /** @brief what string with error */
    QString         what;

    /** @brief moreInfo more information about error */
    QString         moreInfo;

    QString         MoreInfo(const QString &detInfo) const;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief What return string with error
 * @return string with error
 */
inline QString VException::What() const
{
    return what;
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

#endif // VEXCEPTION_H
