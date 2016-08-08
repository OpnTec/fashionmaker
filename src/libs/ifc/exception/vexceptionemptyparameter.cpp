/************************************************************************
 **
 **  @file   vexceptionemptyparameter.cpp
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

#include "vexceptionemptyparameter.h"

#include <QDomElement>
#include <QTextStream>

#include "exception/vexception.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionEmptyParameter exception empty parameter
 * @param what string with error
 * @param name name of attribute where error
 * @param domElement dom element
 */
VExceptionEmptyParameter::VExceptionEmptyParameter(const QString &what, const QString &name,
                                                   const QDomElement &domElement)
    : VException(what), name(name), tagText(QString()), tagName(QString()), lineNumber(-1)
{
    Q_ASSERT_X(not domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(not name.isEmpty(), Q_FUNC_INFO, "Parameter name is empty");
    QTextStream stream(&tagText);
    domElement.save(stream, 4);
    tagName = domElement.tagName();
    lineNumber = domElement.lineNumber();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VExceptionEmptyParameter copy constructor
 * @param e exception
 */
VExceptionEmptyParameter::VExceptionEmptyParameter(const VExceptionEmptyParameter &e)
    :VException(e), name(e.Name()), tagText(e.TagText()), tagName(e.TagName()), lineNumber(e.LineNumber())
{}

//---------------------------------------------------------------------------------------------------------------------
VExceptionEmptyParameter::~VExceptionEmptyParameter() V_NOEXCEPT_EXPR (true)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return main error message
 */
QString VExceptionEmptyParameter::ErrorMessage() const
{
    return QString("ExceptionEmptyParameter: %1 %2").arg(error, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DetailedInformation return detailed information about error
 * @return detailed information
 */
QString VExceptionEmptyParameter::DetailedInformation() const
{
    return MoreInfo(QString("tag: %1 in line %2\nFull tag:\n%3").arg(tagName).arg(lineNumber).arg(tagText));
}
