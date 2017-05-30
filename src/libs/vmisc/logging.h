/****************************************************************************
**
** Copyright (C) 2014 Digia Plc.
** Contact: http://www.qt-project.org/legal
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
****************************************************************************/

#ifndef LOGGING_H
#define LOGGING_H

#include <QtGlobal>

// Backport useful macros
#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
#   ifndef qUtf8Printable
#       define qUtf8Printable(string) QString(string).toUtf8().constData()
#   endif
#endif

#include <QLoggingCategory>

#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)

// printf style for qCDebug was only added in Qt 5.3

#undef qCDebug
#undef qCWarning
#undef qCCritical

#define qCDebug(category, ...) \
    for (bool q_category_enabled = category().isDebugEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).debug(__VA_ARGS__)
#define qCWarning(category, ...) \
    for (bool q_category_enabled = category().isWarningEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).warning(__VA_ARGS__)
#define qCCritical(category, ...) \
    for (bool q_category_enabled = category().isCriticalEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).critical(__VA_ARGS__)

#endif // QT_VERSION < QT_VERSION_CHECK(5, 3, 0)

#endif // LOGGING_H
