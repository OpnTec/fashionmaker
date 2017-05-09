/************************************************************************
 **
 **  @file   abstracttest.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <QMetaObject>
#include <QObject>
#include <QString>

template <class T> class QVector;

#ifdef Q_CC_MSVC
#include <ciso646>
#endif /* Q_CC_MSVC */

#ifdef __GNUC__
#define V_UNUSED __attribute__ ((unused))
#else
#define V_UNUSED
#endif

// Return codes for testing run application
static const auto V_UNUSED TST_EX_BIN = -1;      // Can't find binary.
static const auto V_UNUSED TST_EX_FINISH_TIME_OUT = -2; // The operation timed out or an error occurred.
static const auto V_UNUSED TST_EX_START_TIME_OUT = -3; // The operation timed out or an error occurred.
static const auto V_UNUSED TST_EX_CRASH = -4;    // Program crashed.

#undef V_UNUSED

enum ErrorState {ErrorLoad = 0, ErrorInstall, ErrorSize, NoError};

class AbstractTest : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTest(QObject *parent = nullptr);

protected:
    void Comparison(const QVector<QPointF> &ekv, const QVector<QPointF> &ekvOrig) const;

    QString ValentinaPath() const;
    QString TapePath() const;
    QString TranslationsPath() const;

    int Run(int exit, const QString &program, const QStringList &arguments, QString &error, int msecs = 120000);
    bool CopyRecursively(const QString &srcFilePath, const QString &tgtFilePath) const;
};

#endif // ABSTRACTTEST_H
