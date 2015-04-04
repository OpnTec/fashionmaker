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

#ifndef QABSTRACTFILEENGINE_P_H
#define QABSTRACTFILEENGINE_P_H

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
#include <QDir>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifdef open
#error qabstractfileengine_p.h must be included before any header file that defines open
#endif

class QVariant;
class QAbstractFileEngineIterator;
class QAbstractFileEnginePrivate;

class QAbstractFileEngine
{
public:
    enum FileFlag {
        //perms (overlaps the QFile::Permission)
        ReadOwnerPerm = 0x4000, WriteOwnerPerm = 0x2000, ExeOwnerPerm = 0x1000,
        ReadUserPerm  = 0x0400, WriteUserPerm  = 0x0200, ExeUserPerm  = 0x0100,
        ReadGroupPerm = 0x0040, WriteGroupPerm = 0x0020, ExeGroupPerm = 0x0010,
        ReadOtherPerm = 0x0004, WriteOtherPerm = 0x0002, ExeOtherPerm = 0x0001,

        //types
        LinkType      = 0x10000,
        FileType      = 0x20000,
        DirectoryType = 0x40000,
        BundleType    = 0x80000,

        //flags
        HiddenFlag     = 0x0100000,
        LocalDiskFlag  = 0x0200000,
        ExistsFlag     = 0x0400000,
        RootFlag       = 0x0800000,
        Refresh        = 0x1000000,

        //masks
        PermsMask  = 0x0000FFFF,
        TypesMask  = 0x000F0000,
        FlagsMask  = 0x0FF00000,
        FileInfoAll = FlagsMask | PermsMask | TypesMask
    };
    Q_DECLARE_FLAGS(FileFlags, FileFlag)

    enum FileName {
        DefaultName,
        BaseName,
        PathName,
        AbsoluteName,
        AbsolutePathName,
        LinkName,
        CanonicalName,
        CanonicalPathName,
        BundleName,
        NFileNames = 9
    };
    enum FileOwner {
        OwnerUser,
        OwnerGroup
    };
    enum FileTime {
        CreationTime,
        ModificationTime,
        AccessTime
    };

    virtual ~QAbstractFileEngine();

    virtual bool open(QIODevice::OpenMode openMode);
    virtual bool close();
    virtual bool flush();
    virtual bool syncToDisk();
    virtual qint64 size() const;
    virtual qint64 pos() const;
    virtual bool seek(qint64 pos);
    virtual bool isSequential() const;
    virtual bool remove();
    virtual bool copy(const QString &newName);
    virtual bool rename(const QString &newName);
    virtual bool renameOverwrite(const QString &newName);
    virtual bool link(const QString &newName);
    virtual bool mkdir(const QString &dirName, bool createParentDirectories) const;
    virtual bool rmdir(const QString &dirName, bool recurseParentDirectories) const;
    virtual bool setSize(qint64 size);
    virtual bool caseSensitive() const;
    virtual bool isRelativePath() const;
    virtual QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const;
    virtual FileFlags fileFlags(FileFlags type=FileInfoAll) const;
    virtual bool setPermissions(uint perms);
    virtual QString fileName(FileName file=DefaultName) const;
    virtual uint ownerId(FileOwner) const;
    virtual QString owner(FileOwner) const;
    virtual QDateTime fileTime(FileTime time) const;
    virtual void setFileName(const QString &file);
    virtual int handle() const;
    bool atEnd() const;
    uchar *map(qint64 offset, qint64 size, QFile::MemoryMapFlags flags);
    bool unmap(uchar *ptr);

    typedef QAbstractFileEngineIterator Iterator;
    virtual Iterator *beginEntryList(QDir::Filters filters, const QStringList &filterNames);
    virtual Iterator *endEntryList();

    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 readLine(char *data, qint64 maxlen);
    virtual qint64 write(const char *data, qint64 len);

    QFile::FileError error() const;
    QString errorString() const;

    enum Extension {
        AtEndExtension,
        FastReadLineExtension,
        MapExtension,
        UnMapExtension
    };
    class ExtensionOption
    {};
    class ExtensionReturn
    {};

    class MapExtensionOption : public ExtensionOption {
    public:
        qint64 offset;
        qint64 size;
        QFile::MemoryMapFlags flags;
    };
    class MapExtensionReturn : public ExtensionReturn {
    public:
        uchar *address;
    };

    class UnMapExtensionOption : public ExtensionOption {
    public:
        uchar *address;
    };

    virtual bool extension(Extension extension, const ExtensionOption *option = 0, ExtensionReturn *output = 0);
    virtual bool supportsExtension(Extension extension) const;

    // Factory
    static QAbstractFileEngine *create(const QString &fileName);

protected:
    void setError(QFile::FileError error, const QString &str);

    QAbstractFileEngine();
    QAbstractFileEngine(QAbstractFileEnginePrivate &);

    QScopedPointer<QAbstractFileEnginePrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(QAbstractFileEngine)
    Q_DISABLE_COPY(QAbstractFileEngine)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAbstractFileEngine::FileFlags)

class QAbstractFileEngineHandler
{
public:
    QAbstractFileEngineHandler();
    virtual ~QAbstractFileEngineHandler();
    virtual QAbstractFileEngine *create(const QString &fileName) const = 0;
};

class QAbstractFileEngineIteratorPrivate;
class QAbstractFileEngineIterator
{
public:
    QAbstractFileEngineIterator(QDir::Filters filters, const QStringList &nameFilters);
    virtual ~QAbstractFileEngineIterator();

    virtual QString next() = 0;
    virtual bool hasNext() const = 0;

    QString path() const;
    QStringList nameFilters() const;
    QDir::Filters filters() const;

    virtual QString currentFileName() const = 0;
    virtual QFileInfo currentFileInfo() const;
    QString currentFilePath() const;

protected:
    enum EntryInfoType {
    };
    virtual QVariant entryInfo(EntryInfoType type) const;

private:
    Q_DISABLE_COPY(QAbstractFileEngineIterator)
    friend class QDirIterator;
    friend class QDirIteratorPrivate;
    void setPath(const QString &path);
    QScopedPointer<QAbstractFileEngineIteratorPrivate> d;
};

class QAbstractFileEnginePrivate
{
public:
    inline QAbstractFileEnginePrivate()
        : fileError(QFile::UnspecifiedError)
    {
    }
    inline virtual ~QAbstractFileEnginePrivate() { }

    QFile::FileError fileError;
    QString errorString;

    QAbstractFileEngine *q_ptr;
    Q_DECLARE_PUBLIC(QAbstractFileEngine)
};

QAbstractFileEngine *qt_custom_file_engine_handler_create(const QString &path);

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QABSTRACTFILEENGINE_P_H
