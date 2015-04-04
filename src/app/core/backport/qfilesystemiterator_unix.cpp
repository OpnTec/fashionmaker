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

#include "qplatformdefs.h"
#include "qfilesystemiterator_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifndef QT_NO_FILESYSTEMITERATOR

#include <stdlib.h>
#include <errno.h>

QFileSystemIterator::QFileSystemIterator(const QFileSystemEntry &entry, QDir::Filters filters,
                                         const QStringList &nameFilters, QDirIterator::IteratorFlags flags)
    : nativePath(entry.nativeFilePath())
    , dir(0)
    , dirEntry(0)
#if defined(Q_OS_QNX) && defined(__EXT_QNX__READDIR_R)
    , direntSize(0)
#endif
    , lastError(0)
{
    Q_UNUSED(filters)
    Q_UNUSED(nameFilters)
    Q_UNUSED(flags)

    if ((dir = QT_OPENDIR(nativePath.constData())) == 0) {
        lastError = errno;
    } else {

        if (!nativePath.endsWith('/'))
            nativePath.append('/');

#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN)
        // ### Race condition; we should use fpathconf and dirfd().
        size_t maxPathName = ::pathconf(nativePath.constData(), _PC_NAME_MAX);
        if (maxPathName == size_t(-1))
            maxPathName = FILENAME_MAX;
        maxPathName += sizeof(QT_DIRENT) + 1;

        QT_DIRENT *p = reinterpret_cast<QT_DIRENT*>(::malloc(maxPathName));
        Q_CHECK_PTR(p);

        mt_file.reset(p);
#if defined(Q_OS_QNX) && defined(__EXT_QNX__READDIR_R)
        direntSize = maxPathName;

        // Include extra stat information in the readdir() call (d_stat member of dirent_extra_stat).
        // This is used in QFileSystemMetaData::fillFromDirEnt() to avoid extra stat() calls when iterating
        // over directories
        if (dircntl(dir, D_SETFLAG, D_FLAG_STAT) == -1)
            lastError = errno;
#endif
#endif
    }
}

QFileSystemIterator::~QFileSystemIterator()
{
    if (dir)
        QT_CLOSEDIR(dir);
}

bool QFileSystemIterator::advance(QFileSystemEntry &fileEntry, QFileSystemMetaData &metaData)
{
    if (!dir)
        return false;

#if defined(Q_OS_QNX) && defined(__EXT_QNX__READDIR_R)
    lastError = _readdir_r(dir, mt_file.data(), &dirEntry, direntSize);
    if (lastError)
        return false;
#elif defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN)
    lastError = QT_READDIR_R(dir, mt_file.data(), &dirEntry);
    if (lastError)
        return false;
#else
    // ### add local lock to prevent breaking reentrancy
    dirEntry = QT_READDIR(dir);
#endif // _POSIX_THREAD_SAFE_FUNCTIONS

    if (dirEntry) {
        fileEntry = QFileSystemEntry(nativePath + QByteArray(dirEntry->d_name), QFileSystemEntry::FromNativePath());
        metaData.fillFromDirEnt(*dirEntry);
        return true;
    }

    lastError = errno;
    return false;
}

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QT_NO_FILESYSTEMITERATOR
