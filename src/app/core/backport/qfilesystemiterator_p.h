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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFILESYSTEMITERATOR_P_H
#define QFILESYSTEMITERATOR_P_H

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

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifndef QT_NO_FILESYSTEMITERATOR

#include <QDir>
#include <QDirIterator>
#include <QStringList>

#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"

// Platform-specific headers
#if !defined(Q_OS_WIN)
#include <QScopedPointer>
#endif

class QFileSystemIterator
{
public:
    QFileSystemIterator(const QFileSystemEntry &entry, QDir::Filters filters,
            const QStringList &nameFilters, QDirIterator::IteratorFlags flags
                = QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
    ~QFileSystemIterator();

    bool advance(QFileSystemEntry &fileEntry, QFileSystemMetaData &metaData);

private:
    QFileSystemEntry::NativePath nativePath;

    // Platform-specific data
#if defined(Q_OS_WIN)
    QString dirPath;
    HANDLE findFileHandle;
    QStringList uncShares;
    bool uncFallback;
    int uncShareIndex;
    bool onlyDirs;
#else
    QT_DIR *dir;
    QT_DIRENT *dirEntry;
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN)
    // for readdir_r
    QScopedPointer<QT_DIRENT, QScopedPointerPodDeleter> mt_file;
#if defined(Q_OS_QNX) && defined(__EXT_QNX__READDIR_R)
    // for _readdir_r
    size_t direntSize;
#endif
#endif
    int lastError;
#endif

    Q_DISABLE_COPY(QFileSystemIterator)
};

#endif // QT_NO_FILESYSTEMITERATOR

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // include guard
