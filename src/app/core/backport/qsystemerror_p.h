/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** $QT_BEGIN_LICENSE:GPL$
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSYSTEMERROR_P_H
#define QSYSTEMERROR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QString>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QSystemError
{
public:
    enum ErrorScope
    {
        NoError,
        StandardLibraryError,
        NativeError
    };

    inline QSystemError(int error, ErrorScope scope);
    inline QSystemError();

    QString toString();
    inline ErrorScope scope();
    inline int error();

    //data members
    int errorCode;
    ErrorScope errorScope;
};

QSystemError::QSystemError(int error, QSystemError::ErrorScope scope)
: errorCode(error), errorScope(scope)
{

}

QSystemError::QSystemError()
: errorCode(0), errorScope(NoError)
{

}

QSystemError::ErrorScope QSystemError::scope()
{
    return errorScope;
}

int QSystemError::error()
{
    return errorCode;
}

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QSYSTEMERROR_P_H
