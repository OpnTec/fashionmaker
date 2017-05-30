/****************************************************************************
**
** Copyright (C) 2013 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:LGPL21$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCOMMANDLINEOPTION_H
#define QCOMMANDLINEOPTION_H

#include <QStringList>
#include <QSharedData>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class QCommandLineOptionPrivate;

class QCommandLineOption
{
public:
    explicit QCommandLineOption(const QString &name, const QString &description = QString(),
                                const QString &valueName = QString(),
                                const QString &defaultValue = QString());
    explicit QCommandLineOption(const QStringList &names, const QString &description = QString(),
                                const QString &valueName = QString(),
                                const QString &defaultValue = QString());
    QCommandLineOption(const QCommandLineOption &other);

    ~QCommandLineOption();

    QCommandLineOption &operator=(const QCommandLineOption &other);
#ifdef Q_COMPILER_RVALUE_REFS
    inline QCommandLineOption &operator=(QCommandLineOption &&other)
    { qSwap(d, other.d); return *this; }
#endif

    inline void swap(QCommandLineOption &other)
    { qSwap(d, other.d); }

    QStringList names() const;

    void setValueName(const QString &name);
    QString valueName() const;

    void setDescription(const QString &description);
    QString description() const;

    void setDefaultValue(const QString &defaultValue);
    void setDefaultValues(const QStringList &defaultValues);
    QStringList defaultValues() const;

private:
    QSharedDataPointer<QCommandLineOptionPrivate> d;
};

QT_WARNING_POP

Q_DECLARE_SHARED(QCommandLineOption)

#endif //QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

#endif // QCOMMANDLINEOPTION_H
