/************************************************************************
 **
 **  @file   vtapesettings.h
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

#ifndef VTAPESETTINGS_H
#define VTAPESETTINGS_H

#include "vcommonsettings.h"

class VTapeSettings : public VCommonSettings
{
    Q_OBJECT
public:
    VTapeSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
                  QObject *parent = 0);

    virtual QString StandardTablesPath()const Q_DECL_OVERRIDE;
    QString TemplatesPath() const;

    QString GetPathTemplate() const;
    void SetPathTemplate(const QString &value);

    QByteArray GetDataBaseGeometry() const;
    void SetDataBaseGeometry(const QByteArray &value);

private:
    Q_DISABLE_COPY(VTapeSettings)

    static const QString SettingPathsTemplates;
    static const QString SettingDataBaseGeometry;
};

#endif // VTAPESETTINGS_H
