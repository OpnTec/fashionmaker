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

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

//
// Qt < 5.2 didn't feature categorized logging:
// Just enable qCWarning, qCCritical, but disable qCDebug
//
class QLoggingCategory
{
    Q_DISABLE_COPY(QLoggingCategory)
public:
    explicit QLoggingCategory(const char *category) : name(category) {}
    ~QLoggingCategory();

    bool isDebugEnabled() const { return false; }
    bool isWarningEnabled() const { return true; }
    bool isCriticalEnabled() const { return true; }

    const char *categoryName() const { return name; }

    QLoggingCategory &operator()() { return *this; }

private:
    const char *name;
};

#define Q_DECLARE_LOGGING_CATEGORY(name) \
    extern QLoggingCategory &name();

// relies on QLoggingCategory(QString) being thread safe!
#define Q_LOGGING_CATEGORY(name, string) \
    QLoggingCategory &name() \
    { \
        static QLoggingCategory category(string); \
        return category; \
    }

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)

//
// Qt 5.0, 5.1 do have already support for categories in QMessageLogger
//

#define qCDebug(category, ...) \
    for (bool q_category_enabled = category().isDebugEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).debug(__VA_ARGS__)
#define qCWarning(category, ...) \
    for (bool q_category_enabled = category().isWarningEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).warning(__VA_ARGS__)
#define qCCritical(category, ...) \
    for (bool q_category_enabled = category().isCriticalEnabled(); q_category_enabled; q_category_enabled = false)  \
        QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, category().categoryName()).critical(__VA_ARGS__)
# endif // QT_VERSION > QT_VERSION_CHECK(5, 0, 0)

#else // QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

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

#endif // QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

#endif // LOGGING_H
