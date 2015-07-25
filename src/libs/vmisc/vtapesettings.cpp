/************************************************************************
 **
 **  @file   vtapesettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
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

#include "vtapesettings.h"

#include <QApplication>
#include <QDir>

const QString VTapeSettings::SettingPathsTemplates = QStringLiteral("paths/templates");

//---------------------------------------------------------------------------------------------------------------------
VTapeSettings::VTapeSettings(Format format, Scope scope, const QString &organization, const QString &application,
                             QObject *parent)
    :VCommonSettings(format, scope, organization, application, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
QString VTapeSettings::StandardTablesPath() const
{
    const QString stPath = QStringLiteral("/tables/standard");
#ifdef Q_OS_WIN
    QDir dir(QApplication::applicationDirPath() + stPath);
    if (dir.exists())
    {
        return dir.absolutePath();
    }
    else
    {
        return QApplication::applicationDirPath() + "../../valentina/bin" + stPath;
    }
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + stPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir1(QApplication::applicationDirPath() + stPath);
        if (dir1.exists())
        {
            return dir1.absolutePath();
        }

        QDir dir2(QApplication::applicationDirPath() + "../../valentina/bin" + stPath);
        if (dir2.exists())
        {
            return dir2.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/standard");
        }
    }
#else // Unix
    #ifdef QT_DEBUG
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QApplication::applicationDirPath() + "../../valentina/bin" + stPath;
        }
    #else
        QDir dir1(QApplication::applicationDirPath() + stPath);
        if (dir1.exists())
        {
            return dir1.absolutePath();
        }

        QDir dir2(QApplication::applicationDirPath() + "../../valentina/bin" + stPath);
        if (dir2.exists())
        {
            return dir2.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/standard");
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VTapeSettings::TemplatesPath() const
{
    const QString stPath = QStringLiteral("/tables/template");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + stPath;
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + stPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/template");
        }
    }
#else // Unix
    #ifdef QT_DEBUG
        return QApplication::applicationDirPath() + stPath;
    #else
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/template");
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VTapeSettings::GetPathTemplate() const
{
    return value(SettingPathsTemplates, TemplatesPath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetPathTemplate(const QString &value)
{
    setValue(SettingPathsTemplates, value);
}
