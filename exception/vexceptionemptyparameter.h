/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VEXCEPTIONEMPTYPARAMETER_H
#define VEXCEPTIONEMPTYPARAMETER_H

#include "vexception.h"

class VExceptionEmptyParameter : public VException{
public:
                    VExceptionEmptyParameter(const QString &what, const QString &name,
                                             const QDomElement &domElement);
                    VExceptionEmptyParameter(const VExceptionEmptyParameter &e):VException(e), name(e.Name()),
                        tagText(e.TagText()), tagName(e.TagName()), lineNumber(e.LineNumber()){}
    virtual         ~VExceptionEmptyParameter() noexcept(true) {}
    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    inline QString  Name() const {return name;}
    inline QString  TagText() const {return tagText;}
    inline QString  TagName() const {return tagName;}
    inline qint32   LineNumber() const {return lineNumber;}
protected:
    QString         name;
    QString         tagText;
    QString         tagName;
    qint32          lineNumber;
};

#endif // VEXCEPTIONEMPTYPARAMETER_H
