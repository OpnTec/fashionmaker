/****************************************************************************
**
** Copyright (C) 2013 David Faure <faure+bluesystems@kde.org>
** Contact: http://www.qt-project.org/legal
**
** $QT_BEGIN_LICENSE:LGPL21$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QLOCKFILE_P_H
#define QLOCKFILE_P_H

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

#include "qlockfile.h"
#include <QFile>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QLockFilePrivate
{
public:
    QLockFilePrivate(const QString &fn)
        : fileName(fn),
#ifdef Q_OS_WIN
          fileHandle(INVALID_HANDLE_VALUE),
#else
          fileHandle(-1),
#endif
          staleLockTime(30 * 1000), // 30 seconds
          lockError(QLockFile::NoError),
          isLocked(false)
    {
    }
    QLockFile::LockError tryLock_sys();
    bool removeStaleLock();
    bool getLockInfo(qint64 *pid, QString *hostname, QString *appname) const;
    // Returns \c true if the lock belongs to dead PID, or is old.
    // The attempt to delete it will tell us if it was really stale or not, though.
    bool isApparentlyStale() const;

#ifdef Q_OS_UNIX
    static int checkFcntlWorksAfterFlock();
#endif

    QString fileName;
#ifdef Q_OS_WIN
    Qt::HANDLE fileHandle;
#else
    int fileHandle;
#endif
    int staleLockTime; // "int milliseconds" is big enough for 24 days
    QLockFile::LockError lockError;
    bool isLocked;
};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif /* QLOCKFILE_P_H */
