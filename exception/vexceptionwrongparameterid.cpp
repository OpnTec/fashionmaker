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

#include "vexceptionwrongparameterid.h"
#include <QDebug>

VExceptionWrongParameterId::VExceptionWrongParameterId(const QString &what, const QDomElement &domElement)
    :VException(what), tagText(QString()), tagName(QString()), lineNumber(-1)
{
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    QTextStream stream(&tagText);
    domElement.save(stream, 4);
    tagName = domElement.tagName();
    lineNumber = domElement.lineNumber();
}

QString VExceptionWrongParameterId::ErrorMessage() const
{
    QString error = QString("ExceptionWrongParameterId: %1").arg(what);
    return error;
}

QString VExceptionWrongParameterId::DetailedInformation() const
{
    QString detail = QString("tag: %1 in line %2\n%3").arg(tagName).arg(lineNumber).arg(tagText);
    return detail;
}
