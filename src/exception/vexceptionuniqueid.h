/************************************************************************
 **
 **  @file   vexceptionuniqueid.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#ifndef VEXCEPTIONUNIQUEID_H
#define VEXCEPTIONUNIQUEID_H

#include "vexception.h"

#include <QDomElement>

/**
 * @brief The VExceptionUniqueId class for exception unique id
 */
class VExceptionUniqueId : public VException
{
public:
                    /**
                     * @brief VExceptionUniqueId exception unique id
                     * @param what string with error
                     * @param domElement dom element
                     */
                    VExceptionUniqueId(const QString &what, const QDomElement &domElement);
                    /**
                     * @brief VExceptionUniqueId copy constructor
                     * @param e exception
                     */
                    VExceptionUniqueId(const VExceptionUniqueId &e)
                        :VException(e), tagText(e.TagText()), tagName(e.TagName()), lineNumber(e.LineNumber()){}
    virtual         ~VExceptionUniqueId() noexcept (true){}
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
     * @brief TagText return tag text
     * @return tag text
     */
    inline QString  TagText() const {return tagText;}
    /**
     * @brief TagName return tag name
     * @return tag name
     */
    inline QString  TagName() const {return tagName;}
    /**
     * @brief LineNumber return line number in file
     * @return line number
     */
    inline qint32   LineNumber() const {return lineNumber;}
protected:
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

#endif // VEXCEPTIONUNIQUEID_H
