/************************************************************************
 **
 **  @file   vexceptionemptyparameter.h
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

#ifndef VEXCEPTIONEMPTYPARAMETER_H
#define VEXCEPTIONEMPTYPARAMETER_H

#include "vexception.h"

#include <QDomElement>

/**
 * @brief The VExceptionEmptyParameter class for exception empty parameter
 */
class VExceptionEmptyParameter : public VException
{
public:
                    /**
                     * @brief VExceptionEmptyParameter exception empty parameter
                     * @param what string with error
                     * @param name name of attribute where error
                     * @param domElement dom element
                     */
                    VExceptionEmptyParameter(const QString &what, const QString &name, const QDomElement &domElement);
                    /**
                     * @brief VExceptionEmptyParameter copy constructor
                     * @param e exception
                     */
                    VExceptionEmptyParameter(const VExceptionEmptyParameter &e);
    virtual         ~VExceptionEmptyParameter() noexcept (true) {}
    /**
     * @brief ErrorMessage return main error message
     * @return main error message
     */
    virtual QString ErrorMessage() const;
    /**
     * @brief DetailedInformation return detailed information about error
     * @return detailed information
     */
    virtual QString DetailedInformation() const;
    /**
     * @brief Name return name of attribute where error
     * @return name
     */
    QString         Name() const;
    /**
     * @brief TagText return tag text
     * @return tag text
     */
    QString         TagText() const;
    /**
     * @brief TagName return tag name
     * @return tag name
     */
    QString         TagName() const;
    /**
     * @brief LineNumber return line number of tag
     * @return line number
     */
    qint32          LineNumber() const;
protected:
    /**
     * @brief name name attribute
     */
    QString         name;
    /**
     * @brief tagText tag text
     */
    QString         tagText;
    /**
     * @brief tagName tag name
     */
    QString         tagName;
    /**
     * @brief lineNumber line number
     */
    qint32          lineNumber;
};

inline QString VExceptionEmptyParameter::Name() const
{
    return name;
}

inline QString VExceptionEmptyParameter::TagText() const
{
    return tagText;
}

inline QString VExceptionEmptyParameter::TagName() const
{
    return tagName;
}

inline qint32 VExceptionEmptyParameter::LineNumber() const
{
    return lineNumber;
}

#endif // VEXCEPTIONEMPTYPARAMETER_H
