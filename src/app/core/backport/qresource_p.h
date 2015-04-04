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

#ifndef QRESOURCE_P_H
#define QRESOURCE_P_H

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

#include "qabstractfileengine_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

class QResourceFileEnginePrivate;
class QResourceFileEngine : public QAbstractFileEngine
{
private:
    Q_DECLARE_PRIVATE(QResourceFileEngine)
public:
    explicit QResourceFileEngine(const QString &path);
    ~QResourceFileEngine();

    virtual void setFileName(const QString &file);

    virtual bool open(QIODevice::OpenMode flags) ;
    virtual bool close();
    virtual bool flush();
    virtual qint64 size() const;
    virtual qint64 pos() const;
    virtual bool atEnd() const;
    virtual bool seek(qint64);
    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 write(const char *data, qint64 len);

    virtual bool remove();
    virtual bool copy(const QString &newName);
    virtual bool rename(const QString &newName);
    virtual bool link(const QString &newName);

    virtual bool isSequential() const;

    virtual bool isRelativePath() const;

    virtual bool mkdir(const QString &dirName, bool createParentDirectories) const;
    virtual bool rmdir(const QString &dirName, bool recurseParentDirectories) const;

    virtual bool setSize(qint64 size);

    virtual QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const;

    virtual bool caseSensitive() const;

    virtual FileFlags fileFlags(FileFlags type) const;

    virtual bool setPermissions(uint perms);

    virtual QString fileName(QAbstractFileEngine::FileName file) const;

    virtual uint ownerId(FileOwner) const;
    virtual QString owner(FileOwner) const;

    virtual QDateTime fileTime(FileTime time) const;

    virtual Iterator *beginEntryList(QDir::Filters filters, const QStringList &filterNames);
    virtual Iterator *endEntryList();

    bool extension(Extension extension, const ExtensionOption *option = 0, ExtensionReturn *output = 0);
    bool supportsExtension(Extension extension) const;
};

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QRESOURCE_P_H
