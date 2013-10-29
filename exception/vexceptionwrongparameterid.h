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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VEXCEPTIONWRONGPARAMETERID_H
#define VEXCEPTIONWRONGPARAMETERID_H

#include "vexception.h"

class VExceptionWrongParameterId : public VException{
public:
                    VExceptionWrongParameterId(const QString &what, const QDomElement &domElement);
                    VExceptionWrongParameterId(const VExceptionWrongParameterId &e):VException(e),tagText(e.TagText()),
                        tagName(e.TagName()), lineNumber(e.LineNumber()){}
    virtual         ~VExceptionWrongParameterId() noexcept(true){}
    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    inline QString  TagText() const {return tagText;}
    inline QString  TagName() const {return tagName;}
    inline qint32   LineNumber() const {return lineNumber;}
protected:
    QString         tagText;
    QString         tagName;
    qint32          lineNumber;
};

#endif // VEXCEPTIONWRONGPARAMETERID_H
