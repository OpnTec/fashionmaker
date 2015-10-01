/************************************************************************
 **
 **  @file   vexception.cpp
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

#include "vexception.h"
#include "../vmisc/logging.h"
#include "../vmisc/def.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QSpacerItem>
#include <QApplication>

Q_LOGGING_CATEGORY(vExcep, "v.excep")

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VException constructor exception
 * @param what string with error
 */
VException::VException(const QString &what):QException(), what(what), moreInfo(QString())
{
    Q_ASSERT_X(what.isEmpty() == false, Q_FUNC_INFO, "Error message is empty");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VException copy constructor
 * @param e exception
 */
VException::VException(const VException &e):what(e.What()), moreInfo(e.MoreInformation())
{}

//---------------------------------------------------------------------------------------------------------------------
VException &VException::operator=(const VException &e)
{
    if ( &e == this )
    {
        return *this;
    }
    this->what = e.What();
    this->moreInfo = e.MoreInformation();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return error message
 */
QString VException::ErrorMessage() const
{
    QString error = QString("Exception: %1").arg(what);
    return error;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddMoreInformation add more information for error
 * @param info information
 */
void VException::AddMoreInformation(const QString &info)
{
    if (info.isEmpty())
    {
        return;
    }
    moreInfo = QString("%1\n%2").arg(moreInfo, info);
}

//---------------------------------------------------------------------------------------------------------------------
QString VException::MoreInfo(const QString &detInfo) const
{
    if (moreInfo.isEmpty() == false)
    {
        return QString("%1\n%2").arg(moreInfo, detInfo);
    }
    else
    {
        return detInfo;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DetailedInformation return detailed information about error
 * @return detailed information
 */
QString VException::DetailedInformation() const
{
    return moreInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clone clone exception
 * @return new exception
 */
// cppcheck-suppress unusedFunction
VException *VException::clone() const
{
    return new VException(*this);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief raise method raise for exception
 */
// cppcheck-suppress unusedFunction
Q_NORETURN void VException::raise() const
{
    throw *this;
}
