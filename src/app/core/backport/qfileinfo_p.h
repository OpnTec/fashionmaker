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

#ifndef QFILEINFO_P_H
#define QFILEINFO_P_H

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

#include <QFileInfo>
#include <QDateTime>
#include "qatomic.h"
#include <QSharedData>
#include "qfilesystemengine_p.h"
#include <QVector>

#include "qabstractfileengine_p.h"
#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QFileInfoPrivate : public QSharedData
{
public:
    enum { CachedFileFlags=0x01, CachedLinkTypeFlag=0x02, CachedBundleTypeFlag=0x04,
           CachedMTime=0x10, CachedCTime=0x20, CachedATime=0x40,
           CachedSize =0x08, CachedPerms=0x80 };

    inline QFileInfoPrivate()
        : QSharedData(), fileEngine(0),
        cachedFlags(0),
        isDefaultConstructed(true),
        cache_enabled(true), fileFlags(0), fileSize(0)
    {}
    inline QFileInfoPrivate(const QFileInfoPrivate &copy)
        : QSharedData(copy),
        fileEntry(copy.fileEntry),
        metaData(copy.metaData),
        fileEngine(QFileSystemEngine::resolveEntryAndCreateLegacyEngine(fileEntry, metaData)),
        cachedFlags(0),
#ifndef QT_NO_FSFILEENGINE
        isDefaultConstructed(false),
#else
        isDefaultConstructed(!fileEngine),
#endif
        cache_enabled(copy.cache_enabled), fileFlags(0), fileSize(0)
    {}
    inline QFileInfoPrivate(const QString &file)
        : fileEntry(QDir::fromNativeSeparators(file)),
        fileEngine(QFileSystemEngine::resolveEntryAndCreateLegacyEngine(fileEntry, metaData)),
        cachedFlags(0),
#ifndef QT_NO_FSFILEENGINE
        isDefaultConstructed(false),
#else
        isDefaultConstructed(!fileEngine),
#endif
        cache_enabled(true), fileFlags(0), fileSize(0)
    {
    }

    inline QFileInfoPrivate(const QFileSystemEntry &file, const QFileSystemMetaData &data)
        : QSharedData(),
        fileEntry(file),
        metaData(data),
        fileEngine(QFileSystemEngine::resolveEntryAndCreateLegacyEngine(fileEntry, metaData)),
        cachedFlags(0),
        isDefaultConstructed(false),
        cache_enabled(true), fileFlags(0), fileSize(0)
    {
        //If the file engine is not null, this maybe a "mount point" for a custom file engine
        //in which case we can't trust the metadata
        if (fileEngine)
            metaData = QFileSystemMetaData();
    }

    inline QFileInfoPrivate(const QFileSystemEntry &file, const QFileSystemMetaData &data, QAbstractFileEngine *engine)
        : fileEntry(file),
        metaData(data),
        fileEngine(engine),
        cachedFlags(0),
#ifndef QT_NO_FSFILEENGINE
        isDefaultConstructed(false),
#else
        isDefaultConstructed(!fileEngine),
#endif
        cache_enabled(true), fileFlags(0), fileSize(0)
    {
    }

    inline void clearFlags() const {
        fileFlags = 0;
        cachedFlags = 0;
        if (fileEngine)
            (void)fileEngine->fileFlags(QAbstractFileEngine::Refresh);
    }
    inline void clear() {
        metaData.clear();
        clearFlags();
        for (int i = QAbstractFileEngine::NFileNames - 1 ; i >= 0 ; --i)
            fileNames[i].clear();
        fileOwners[1].clear();
        fileOwners[0].clear();
    }

    uint getFileFlags(QAbstractFileEngine::FileFlags) const;
    QDateTime &getFileTime(QAbstractFileEngine::FileTime) const;
    QString getFileName(QAbstractFileEngine::FileName) const;
    QString getFileOwner(QAbstractFileEngine::FileOwner own) const;

    QFileSystemEntry fileEntry;
    mutable QFileSystemMetaData metaData;

    QScopedPointer<QAbstractFileEngine> const fileEngine;

    mutable QString fileNames[QAbstractFileEngine::NFileNames];
    mutable QString fileOwners[2];

    mutable uint cachedFlags : 30;
    bool const isDefaultConstructed : 1; // QFileInfo is a default constructed instance
    bool cache_enabled : 1;
    mutable uint fileFlags;
    mutable qint64 fileSize;
    // ### Qt6: FIXME: This vector is essentially a plain array
    // mutable QDateTime fileTimes[3], but the array is slower
    // to initialize than the QVector as QDateTime has a pimpl.
    // In Qt 6, QDateTime should inline its data members,
    // and this here can be an array again.
    mutable QVector<QDateTime> fileTimes;
    inline bool getCachedFlag(uint c) const
    { return cache_enabled ? (cachedFlags & c) : 0; }
    inline void setCachedFlag(uint c) const
    { if (cache_enabled) cachedFlags |= c; }

};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QFILEINFO_P_H
