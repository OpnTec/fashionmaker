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

#ifndef QSYSTEMLIBRARY_P_H
#define QSYSTEMLIBRARY_P_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#ifdef Q_OS_WIN
#  include <QString>
#  include <qt_windows.h>

class QSystemLibrary
{
public:
    explicit QSystemLibrary(const QString &libraryName)
    {
        m_libraryName = libraryName;
        m_handle = 0;
        m_didLoad = false;
    }

    explicit QSystemLibrary(const wchar_t *libraryName)
    {
        m_libraryName = QString::fromWCharArray(libraryName);
        m_handle = 0;
        m_didLoad = false;
    }

    bool load(bool onlySystemDirectory = true)
    {
        m_handle = load((const wchar_t *)m_libraryName.utf16(), onlySystemDirectory);
        m_didLoad = true;
        return (m_handle != 0);
    }

    bool isLoaded()
    {
        return (m_handle != 0);
    }

    QFunctionPointer resolve(const char *symbol)
    {
        if (!m_didLoad)
            load();
        if (!m_handle)
            return 0;
#ifdef Q_OS_WINCE
        return QFunctionPointer(GetProcAddress(m_handle, (const wchar_t*)QString::fromLatin1(symbol).utf16()));
#else
        return QFunctionPointer(GetProcAddress(m_handle, symbol));
#endif
    }

    static QFunctionPointer resolve(const QString &libraryName, const char *symbol)
    {
        return QSystemLibrary(libraryName).resolve(symbol);
    }

    static Q_CORE_EXPORT HINSTANCE load(const wchar_t *lpFileName, bool onlySystemDirectory = true);
private:
    HINSTANCE m_handle;
    QString m_libraryName;
    bool m_didLoad;
};

#endif  //Q_OS_WIN

#endif //QT_VERSION < QT_VERSION_CHECK(5, 1, 0)

#endif  //QSYSTEMLIBRARY_P_H
