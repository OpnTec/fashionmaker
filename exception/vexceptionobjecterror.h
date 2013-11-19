/************************************************************************
 **
 **  @file   vexceptionobjecterror.h
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

#ifndef VEXCEPTIONOBJECTERROR_H
#define VEXCEPTIONOBJECTERROR_H

#include "vexception.h"

/**
 * @brief The VExceptionObjectError class
 */
class VExceptionObjectError : public VException
{
public:
                    /**
                     * @brief VExceptionObjectError
                     * @param what
                     * @param domElement
                     */
                    VExceptionObjectError(const QString &what, const QDomElement &domElement);
                    /**
                     * @brief VExceptionObjectError
                     * @param e
                     */
                    VExceptionObjectError(const VExceptionObjectError &e)
                        :VException(e), tagText(e.TagText()), tagName(e.TagName()), lineNumber(e.LineNumber()),
                          moreInfo(e.MoreInformation()){}
    virtual         ~VExceptionObjectError() Q_DECL_NOEXCEPT_EXPR(true) {}
    /**
     * @brief ErrorMessage
     * @return
     */
    virtual QString ErrorMessage() const;
    /**
     * @brief DetailedInformation
     * @return
     */
    virtual QString DetailedInformation() const;
    /**
     * @brief TagText
     * @return
     */
    inline QString  TagText() const {return tagText;}
    /**
     * @brief TagName
     * @return
     */
    inline QString  TagName() const {return tagName;}
    /**
     * @brief LineNumber
     * @return
     */
    inline qint32   LineNumber() const {return lineNumber;}
    /**
     * @brief AddMoreInformation
     * @param info
     */
    void            AddMoreInformation(const QString &info);
    /**
     * @brief MoreInformation
     * @return
     */
    inline QString  MoreInformation() const {return moreInfo;}
protected:
    /**
     * @brief tagText
     */
    QString         tagText;
    /**
     * @brief tagName
     */
    QString         tagName;
    /**
     * @brief lineNumber
     */
    qint32          lineNumber;
    /**
     * @brief moreInfo
     */
    QString         moreInfo;
};

#endif // VEXCEPTIONOBJECTERROR_H
