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

#ifndef QLOCKFILE_H
#define QLOCKFILE_H

#include <QString>
#include <QScopedPointer>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QLockFilePrivate;

class QLockFile
{
public:
    QLockFile(const QString &fileName);
    ~QLockFile();

    bool lock();
    bool tryLock(int timeout = 0);
    void unlock();

    void setStaleLockTime(int);
    int staleLockTime() const;

    bool isLocked() const;
    bool getLockInfo(qint64 *pid, QString *hostname, QString *appname) const;
    bool removeStaleLockFile();

    enum LockError {
        NoError = 0,
        LockFailedError = 1,
        PermissionError = 2,
        UnknownError = 3
    };
    LockError error() const;

protected:
    QScopedPointer<QLockFilePrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QLockFile)
    Q_DISABLE_COPY(QLockFile)
};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QLOCKFILE_H
