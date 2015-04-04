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

#ifndef QMUTEXPOOL_P_H
#define QMUTEXPOOL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of QSettings. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qatomic.h"
#include <QMutex>
#include <QVarLengthArray>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifndef QT_NO_THREAD

class QMutexPool
{
public:
    explicit QMutexPool(QMutex::RecursionMode recursionMode = QMutex::NonRecursive, int size = 131);
    ~QMutexPool();

    inline QMutex *get(const void *address) {
        int index = uint(quintptr(address)) % mutexes.count();
        QMutex *m = mutexes[index].load();
        if (m)
            return m;
        else
            return createMutex(index);
    }
    static QMutexPool *instance();
    static QMutex *globalInstanceGet(const void *address);

private:
    QMutex *createMutex(int index);
    QVarLengthArray<QAtomicPointer<QMutex>, 131> mutexes;
    QMutex::RecursionMode recursionMode;
};

#endif // QT_NO_THREAD

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif // QMUTEXPOOL_P_H
