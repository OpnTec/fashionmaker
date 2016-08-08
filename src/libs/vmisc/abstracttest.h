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

class QPointF;
class QStringList;
template <class T> class QVector;

#ifdef Q_CC_MSVC
#include <ciso646>
#endif /* Q_CC_MSVC */

// Return codes for testing run application
static const auto TST_EX_BIN = -1;      // Can't find binary.
static const auto TST_EX_TIME_OUT = -2; // The operation timed out or an error occurred.
static const auto TST_EX_CRASH = -3;    // Program crashed.

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

    bool Run(bool showWarn, int exit, int &exitCode, const QString &program, const QStringList &arguments,
             int msecs = 30000);
    bool CopyRecursively(const QString &srcFilePath, const QString &tgtFilePath) const;
};

#endif // ABSTRACTTEST_H
