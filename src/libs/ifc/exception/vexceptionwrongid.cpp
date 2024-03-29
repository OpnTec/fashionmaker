/************************************************************************
 **
 **  @file   vexceptionwrongparameterid.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vexceptionwrongid.h"

#include <QDomElement>
#include <QTextStream>

#include "vexception.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionWrongId exception wrong parameter id
 * @param what string with error
 * @param domElement som element
 */
VExceptionWrongId::VExceptionWrongId(const QString &what, const QDomElement &domElement) V_NOEXCEPT_EXPR (true)
    :VException(what), tagText(QString()), tagName(QString()), lineNumber(-1)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    QTextStream stream(&tagText);
    domElement.save(stream, 4);
    tagName = domElement.tagName();
    lineNumber = domElement.lineNumber();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionWrongId copy constructor
 * @param e exception
 */
VExceptionWrongId::VExceptionWrongId(const VExceptionWrongId &e) V_NOEXCEPT_EXPR (true)
    :VException(e), tagText(e.TagText()), tagName(e.TagName()), lineNumber(e.LineNumber())
{}

//---------------------------------------------------------------------------------------------------------------------
VExceptionWrongId &VExceptionWrongId::operator=(const VExceptionWrongId &e) V_NOEXCEPT_EXPR (true)
{
    if ( &e == this )
    {
        return *this;
    }
    VException::operator=(e);
    tagText = e.TagText();
    tagName = e.TagName();
    lineNumber = e.LineNumber();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return main error message
 */
QString VExceptionWrongId::ErrorMessage() const
{
    return QString("ExceptionWrongId: %1").arg(error);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DetailedInformation return detailed information about error
 * @return detailed information
 */
QString VExceptionWrongId::DetailedInformation() const
{
    return MoreInfo(QString("tag: %1 in line %2\nFull tag:\n%3").arg(tagName).arg(lineNumber).arg(tagText));
}
