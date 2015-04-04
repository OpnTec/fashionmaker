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

#ifndef QFSFILEENGINE_ITERATOR_P_H
#define QFSFILEENGINE_ITERATOR_P_H

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
#include "qfilesystemiterator_p.h"
#include <QDir>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifndef QT_NO_FILESYSTEMITERATOR

class QFSFileEngineIteratorPrivate;
class QFSFileEngineIteratorPlatformSpecificData;

class QFSFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    QFSFileEngineIterator(QDir::Filters filters, const QStringList &filterNames);
    ~QFSFileEngineIterator();

    QString next();
    bool hasNext() const;

    QString currentFileName() const;
    QFileInfo currentFileInfo() const;

private:
    void advance() const;
    mutable QScopedPointer<QFileSystemIterator> nativeIterator;
    mutable QFileInfo currentInfo;
    mutable QFileInfo nextInfo;
    mutable bool done;
};

#endif // QT_NO_FILESYSTEMITERATOR

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QFSFILEENGINE_ITERATOR_P_H
