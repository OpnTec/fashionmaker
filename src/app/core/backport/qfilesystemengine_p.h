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

#ifndef QFILESYSTEMENGINE_P_H
#define QFILESYSTEMENGINE_P_H

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

#include <QFile>
#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"
#include "qsystemerror_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QFileSystemEngine
{
public:
    static bool isCaseSensitive()
    {
#ifndef Q_OS_WIN
        return true;
#else
        return false;
#endif
    }

    static QFileSystemEntry getLinkTarget(const QFileSystemEntry &link, QFileSystemMetaData &data);
    static QFileSystemEntry canonicalName(const QFileSystemEntry &entry, QFileSystemMetaData &data);
    static QFileSystemEntry absoluteName(const QFileSystemEntry &entry);
    static QByteArray id(const QFileSystemEntry &entry);
    static QString resolveUserName(const QFileSystemEntry &entry, QFileSystemMetaData &data);
    static QString resolveGroupName(const QFileSystemEntry &entry, QFileSystemMetaData &data);

#if defined(Q_OS_UNIX)
    static QString resolveUserName(uint userId);
    static QString resolveGroupName(uint groupId);
#endif

#if defined(Q_OS_MACX)
    static QString bundleName(const QFileSystemEntry &entry);
#else
    static QString bundleName(const QFileSystemEntry &entry) { Q_UNUSED(entry) return QString(); }
#endif

    static bool fillMetaData(const QFileSystemEntry &entry, QFileSystemMetaData &data,
                             QFileSystemMetaData::MetaDataFlags what);
#if defined(Q_OS_UNIX)
    static bool fillMetaData(int fd, QFileSystemMetaData &data); // what = PosixStatFlags
#endif
#if defined(Q_OS_WIN)

    static bool uncListSharesOnServer(const QString &server, QStringList *list); //Used also by QFSFileEngineIterator::hasNext()
    static bool fillMetaData(int fd, QFileSystemMetaData &data,
                             QFileSystemMetaData::MetaDataFlags what);
    static bool fillMetaData(HANDLE fHandle, QFileSystemMetaData &data,
                             QFileSystemMetaData::MetaDataFlags what);
    static bool fillPermissions(const QFileSystemEntry &entry, QFileSystemMetaData &data,
                                QFileSystemMetaData::MetaDataFlags what);
    static QString owner(const QFileSystemEntry &entry, QAbstractFileEngine::FileOwner own);
    static QString nativeAbsoluteFilePath(const QString &path);
#endif
    //homePath, rootPath and tempPath shall return clean paths
    static QString homePath();
    static QString rootPath();
    static QString tempPath();

    static bool createDirectory(const QFileSystemEntry &entry, bool createParents);
    static bool removeDirectory(const QFileSystemEntry &entry, bool removeEmptyParents);

    static bool createLink(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);

    static bool copyFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);
    static bool renameFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);
    static bool removeFile(const QFileSystemEntry &entry, QSystemError &error);

    static bool setPermissions(const QFileSystemEntry &entry, QFile::Permissions permissions, QSystemError &error,
                               QFileSystemMetaData *data = 0);

    static bool setCurrentPath(const QFileSystemEntry &entry);
    static QFileSystemEntry currentPath();

    static QAbstractFileEngine *resolveEntryAndCreateLegacyEngine(QFileSystemEntry &entry,
                                                                  QFileSystemMetaData &data);
private:
    static QString slowCanonicalized(const QString &path);
#if defined(Q_OS_WIN)
    static void clearWinStatData(QFileSystemMetaData &data);
#endif
};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // include guard
