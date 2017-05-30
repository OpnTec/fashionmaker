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

#ifndef QCOMMANDLINEPARSER_H
#define QCOMMANDLINEPARSER_H

#include <QStringList>
#include <QCoreApplication>
#include "qcommandlineoption.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

class QCommandLineParserPrivate;
class QCoreApplication;

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class QCommandLineParser
{
    Q_DECLARE_TR_FUNCTIONS(QCommandLineParser)
public:
    QCommandLineParser();
    ~QCommandLineParser();

    enum SingleDashWordOptionMode {
        ParseAsCompactedShortOptions,
        ParseAsLongOptions
    };
    void setSingleDashWordOptionMode(SingleDashWordOptionMode parsingMode);

    bool addOption(const QCommandLineOption &commandLineOption);

    QCommandLineOption addVersionOption();
    QCommandLineOption addHelpOption();
    void setApplicationDescription(const QString &description);
    QString applicationDescription() const;
    void addPositionalArgument(const QString &name, const QString &description, const QString &syntax = QString());
    void clearPositionalArguments();

    void process(const QStringList &arguments);
    void process(const QCoreApplication &app);

    bool parse(const QStringList &arguments);
    QString errorText() const;

    bool isSet(const QString &name) const;
    QString value(const QString &name) const;
    QStringList values(const QString &name) const;

    bool isSet(const QCommandLineOption &option) const;
    QString value(const QCommandLineOption &option) const;
    QStringList values(const QCommandLineOption &option) const;

    QStringList positionalArguments() const;
    QStringList optionNames() const;
    QStringList unknownOptionNames() const;

    Q_NORETURN void showHelp(int exitCode = 0);
    QString helpText() const;

private:
    Q_DISABLE_COPY(QCommandLineParser)

    QCommandLineParserPrivate * const d;
};

QT_WARNING_POP

#endif //QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

#endif // QCOMMANDLINEPARSER_H
