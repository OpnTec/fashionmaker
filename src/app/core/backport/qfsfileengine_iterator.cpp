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

#include "qfsfileengine_iterator_p.h"
#include "qfileinfo_p.h"
#include <QVariant>

#ifndef QT_NO_FILESYSTEMITERATOR

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

QFSFileEngineIterator::QFSFileEngineIterator(QDir::Filters filters, const QStringList &filterNames)
    : QAbstractFileEngineIterator(filters, filterNames)
    , done(false)
{
}

QFSFileEngineIterator::~QFSFileEngineIterator()
{
}

bool QFSFileEngineIterator::hasNext() const
{
    if (!done && !nativeIterator) {
        nativeIterator.reset(new QFileSystemIterator(QFileSystemEntry(path()),
                    filters(), nameFilters()));
        advance();
    }

    return !done;
}

QString QFSFileEngineIterator::next()
{
    if (!hasNext())
        return QString();

    advance();
    return currentFilePath();
}

void QFSFileEngineIterator::advance() const
{
    currentInfo = nextInfo;

    QFileSystemEntry entry;
    QFileSystemMetaData data;
    if (nativeIterator->advance(entry, data)) {
        nextInfo = QFileInfo(new QFileInfoPrivate(entry, data));
    } else {
        done = true;
        nativeIterator.reset();
    }
}

QString QFSFileEngineIterator::currentFileName() const
{
    return currentInfo.fileName();
}

QFileInfo QFSFileEngineIterator::currentFileInfo() const
{
    return currentInfo;
}

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QT_NO_FILESYSTEMITERATOR
