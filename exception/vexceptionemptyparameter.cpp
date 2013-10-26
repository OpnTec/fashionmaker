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

#include "vexceptionemptyparameter.h"

VExceptionEmptyParameter::VExceptionEmptyParameter(const QString &what, const QString &name,
                                                   const QDomElement &domElement): VException(what),
    name(name), tagText(QString()), tagName(QString()), lineNumber(-1){
    Q_ASSERT_X(!domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    Q_ASSERT_X(!name.isEmpty(), Q_FUNC_INFO, "Parameter name is empty");
    QTextStream stream(&tagText);
    domElement.save(stream, 4);
    tagName = domElement.tagName();
    lineNumber = domElement.lineNumber();
}

QString VExceptionEmptyParameter::ErrorMessage() const{
    QString error = QString("ExceptionEmptyParameter: %1 %2").arg(what, name);
    return error;
}

QString VExceptionEmptyParameter::DetailedInformation() const{
    QString detail = QString("tag: %1 in line %2\nFull tag:\n%3").arg(tagName).arg(lineNumber).arg(tagText);
    return detail;
}
