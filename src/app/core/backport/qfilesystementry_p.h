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

#ifndef QFILESYSTEMENTRY_P_H
#define QFILESYSTEMENTRY_P_H

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
#include <QByteArray>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#if defined(Q_OS_WIN)
#define QFILESYSTEMENTRY_NATIVE_PATH_IS_UTF16
#endif

class QFileSystemEntry
{
public:

#ifndef QFILESYSTEMENTRY_NATIVE_PATH_IS_UTF16
    typedef QByteArray NativePath;
#else
    typedef QString NativePath;
#endif
    struct FromNativePath{};
    struct FromInternalPath{};

    QFileSystemEntry();
    explicit QFileSystemEntry(const QString &filePath);

    QFileSystemEntry(const QString &filePath, FromInternalPath dummy);
    QFileSystemEntry(const NativePath &nativeFilePath, FromNativePath dummy);
    QFileSystemEntry(const QString &filePath, const NativePath &nativeFilePath);

    QString filePath() const;
    QString fileName() const;
    QString path() const;
    NativePath nativeFilePath() const;
    QString baseName() const;
    QString completeBaseName() const;
    QString suffix() const;
    QString completeSuffix() const;
    bool isAbsolute() const;
    bool isRelative() const;
    bool isClean() const;

#if defined(Q_OS_WIN)
    bool isDriveRoot() const;
#endif
    bool isRoot() const;

    bool isEmpty() const;
    void clear()
    {
        *this = QFileSystemEntry();
    }

private:
    // creates the QString version out of the bytearray version
    void resolveFilePath() const;
    // creates the bytearray version out of the QString version
    void resolveNativeFilePath() const;
    // resolves the separator
    void findLastSeparator() const;
    // resolves the dots and the separator
    void findFileNameSeparators() const;

    mutable QString m_filePath; // always has slashes as separator
    mutable NativePath m_nativeFilePath; // native encoding and separators

    mutable qint16 m_lastSeparator; // index in m_filePath of last separator
    mutable qint16 m_firstDotInFileName; // index after m_filePath for first dot (.)
    mutable qint16 m_lastDotInFileName; // index after m_firstDotInFileName for last dot (.)
};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // include guard
