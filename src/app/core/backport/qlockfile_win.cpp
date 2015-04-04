/****************************************************************************
**
** Copyright (C) 2013 David Faure <faure+bluesystems@kde.org>
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qlockfile_p.h"
#include "qfilesystementry_p.h"
#include <qt_windows.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

QLockFile::LockError QLockFilePrivate::tryLock_sys()
{
    SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };
    const QFileSystemEntry fileEntry(fileName);
    // When writing, allow others to read.
    // When reading, QFile will allow others to read and write, all good.
    // Adding FILE_SHARE_DELETE would allow forceful deletion of stale files,
    // but Windows doesn't allow recreating it while this handle is open anyway,
    // so this would only create confusion (can't lock, but no lock file to read from).
    const DWORD dwShareMode = FILE_SHARE_READ;
    HANDLE fh = CreateFile((const wchar_t*)fileEntry.nativeFilePath().utf16(),
                           GENERIC_WRITE,
                           dwShareMode,
                           &securityAtts,
                           CREATE_NEW, // error if already exists
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
    if (fh == INVALID_HANDLE_VALUE) {
        const DWORD lastError = GetLastError();
        switch (lastError) {
        case ERROR_SHARING_VIOLATION:
        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS:
        case ERROR_ACCESS_DENIED: // readonly file, or file still in use by another process. Assume the latter, since we don't create it readonly.
            return QLockFile::LockFailedError;
        default:
            qWarning() << "Got unexpected locking error" << lastError;
            return QLockFile::UnknownError;
        }
    }

    // We hold the lock, continue.
    fileHandle = fh;
    // Assemble data, to write in a single call to write
    // (otherwise we'd have to check every write call)
    QByteArray fileData;
    fileData += QByteArray::number(QCoreApplication::applicationPid());
    fileData += '\n';
    fileData += qAppName().toUtf8();
    fileData += '\n';
    //fileData += localHostname(); // gethostname requires winsock init, see QHostInfo...
    fileData += '\n';
    DWORD bytesWritten = 0;
    QLockFile::LockError error = QLockFile::NoError;
    if (!WriteFile(fh, fileData.constData(), fileData.size(), &bytesWritten, NULL) || !FlushFileBuffers(fh))
        error = QLockFile::UnknownError; // partition full
    return error;
}

bool QLockFilePrivate::removeStaleLock()
{
    // QFile::remove fails on Windows if the other process is still using the file, so it's not stale.
    return QFile::remove(fileName);
}

bool QLockFilePrivate::isApparentlyStale() const
{
    qint64 pid;
    QString hostname, appname;
    if (!getLockInfo(&pid, &hostname, &appname))
        return false;

    HANDLE procHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!procHandle)
        return true;
    // We got a handle but check if process is still alive
    DWORD dwR = ::WaitForSingleObject(procHandle, 0);
    ::CloseHandle(procHandle);
    if (dwR == WAIT_TIMEOUT)
        return true;
    const qint64 age = QFileInfo(fileName).lastModified().msecsTo(QDateTime::currentDateTime());
    return staleLockTime > 0 && age > staleLockTime;
}

void QLockFile::unlock()
{
    Q_D(QLockFile);
     if (!d->isLocked)
        return;
     CloseHandle(d->fileHandle);
     QFile::remove(d->fileName);
     d->lockError = QLockFile::NoError;
     d->isLocked = false;
}

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
