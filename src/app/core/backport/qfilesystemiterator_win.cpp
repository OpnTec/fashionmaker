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

#if _WIN32_WINNT < 0x0500
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "qfilesystemiterator_p.h"
#include "qfilesystemengine_p.h"
#include "qplatformdefs.h"

#include <qt_windows.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

bool done = true;

QFileSystemIterator::QFileSystemIterator(const QFileSystemEntry &entry, QDir::Filters filters,
                                         const QStringList &nameFilters, QDirIterator::IteratorFlags flags)
    : nativePath(entry.nativeFilePath())
    , dirPath(entry.filePath())
    , findFileHandle(INVALID_HANDLE_VALUE)
    , uncFallback(false)
    , uncShareIndex(0)
    , onlyDirs(false)
{
    Q_UNUSED(nameFilters)
    Q_UNUSED(flags)
    if (nativePath.endsWith(QLatin1String(".lnk"))) {
        QFileSystemMetaData metaData;
        QFileSystemEntry link = QFileSystemEngine::getLinkTarget(entry, metaData);
        nativePath = link.nativeFilePath();
    }
    if (!nativePath.endsWith(QLatin1Char('\\')))
        nativePath.append(QLatin1Char('\\'));
    nativePath.append(QLatin1Char('*'));
    if (!dirPath.endsWith(QLatin1Char('/')))
        dirPath.append(QLatin1Char('/'));
    if ((filters & (QDir::Dirs|QDir::Drives)) && (!(filters & (QDir::Files))))
        onlyDirs = true;
}

QFileSystemIterator::~QFileSystemIterator()
{
   if (findFileHandle != INVALID_HANDLE_VALUE)
        FindClose(findFileHandle);
}

bool QFileSystemIterator::advance(QFileSystemEntry &fileEntry, QFileSystemMetaData &metaData)
{
    bool haveData = false;
    WIN32_FIND_DATA findData;

    if (findFileHandle == INVALID_HANDLE_VALUE && !uncFallback) {
        haveData = true;
        int infoLevel = 0 ;         // FindExInfoStandard;
        DWORD dwAdditionalFlags  = 0;
#ifndef Q_OS_WINCE
        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
            dwAdditionalFlags = 2;  // FIND_FIRST_EX_LARGE_FETCH
            infoLevel = 1 ;         // FindExInfoBasic;
        }
#endif
        int searchOps =  0;         // FindExSearchNameMatch
        if (onlyDirs)
            searchOps = 1 ;         // FindExSearchLimitToDirectories
        findFileHandle = FindFirstFileEx((const wchar_t *)nativePath.utf16(), FINDEX_INFO_LEVELS(infoLevel), &findData,
                                         FINDEX_SEARCH_OPS(searchOps), 0, dwAdditionalFlags);
        if (findFileHandle == INVALID_HANDLE_VALUE) {
            if (nativePath.startsWith(QLatin1String("\\\\?\\UNC\\"))) {
                QStringList parts = nativePath.split(QLatin1Char('\\'), QString::SkipEmptyParts);
                if (parts.count() == 4 && QFileSystemEngine::uncListSharesOnServer(
                        QLatin1String("\\\\") + parts.at(2), &uncShares)) {
                    if (uncShares.isEmpty())
                        return false; // No shares found in the server
                    else
                        uncFallback = true;
                }
            }
        }
    }
    if (findFileHandle == INVALID_HANDLE_VALUE && !uncFallback)
        return false;
    // Retrieve the new file information.
    if (!haveData) {
        if (uncFallback) {
            if (++uncShareIndex >= uncShares.count())
                return false;
        } else {
            if (!FindNextFile(findFileHandle, &findData))
                return false;
        }
    }
    // Create the new file system entry & meta data.
    if (uncFallback) {
        fileEntry = QFileSystemEntry(dirPath + uncShares.at(uncShareIndex));
        metaData.fillFromFileAttribute(FILE_ATTRIBUTE_DIRECTORY);
        return true;
    } else {
        QString fileName = QString::fromWCharArray(findData.cFileName);
        fileEntry = QFileSystemEntry(dirPath + fileName);
        metaData = QFileSystemMetaData();
        if (!fileName.endsWith(QLatin1String(".lnk"))) {
            metaData.fillFromFindData(findData, true);
        }
        return true;
    }
    return false;
}

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
