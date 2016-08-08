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

#ifndef VEXCEPTIONEMPTYPARAMETER_H
#define VEXCEPTIONEMPTYPARAMETER_H

#include <qcompilerdetection.h>
#include <QString>
#include <QtGlobal>

#include "../ifcdef.h"
#include "vexception.h"

class QDomElement;

/**
 * @brief The VExceptionEmptyParameter class for exception empty parameter
 */
class VExceptionEmptyParameter : public VException
{
public:
    VExceptionEmptyParameter(const QString &error, const QString &name,
                             const QDomElement &domElement);
    VExceptionEmptyParameter(const VExceptionEmptyParameter &e);
    virtual         ~VExceptionEmptyParameter() V_NOEXCEPT_EXPR (true) Q_DECL_OVERRIDE;
    virtual QString ErrorMessage() const Q_DECL_OVERRIDE;
    virtual QString DetailedInformation() const Q_DECL_OVERRIDE;
    QString         Name() const;
    QString         TagText() const;
    QString         TagName() const;
    qint32          LineNumber() const;
protected:
    /** @brief name name attribute */
    QString         name;

    /** @brief tagText tag text */
    QString         tagText;

    /** @brief tagName tag name */
    QString         tagName;

    /** @brief lineNumber line number */
    qint32          lineNumber;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Name return name of attribute where error
 * @return name
 */
inline QString VExceptionEmptyParameter::Name() const
{
    return name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TagText return tag text
 * @return tag text
 */
inline QString VExceptionEmptyParameter::TagText() const
{
    return tagText;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TagName return tag name
 * @return tag name
 */
inline QString VExceptionEmptyParameter::TagName() const
{
    return tagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineNumber return line number of tag
 * @return line number
 */
inline qint32 VExceptionEmptyParameter::LineNumber() const
{
    return lineNumber;
}

#endif // VEXCEPTIONEMPTYPARAMETER_H
