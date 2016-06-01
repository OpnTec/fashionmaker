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

    QByteArray GetDataBaseGeometry() const;
    void SetDataBaseGeometry(const QByteArray &value);

    void SetDefHeight(int value);
    int  GetDefHeight() const;

    void SetDefSize(int value);
    int  GetDefSize() const;

    void SetCSVWithHeader(bool withHeader);
    bool GetCSVWithHeader() const;
    bool GetDefCSVWithHeader() const;

    void SetCSVCodec(int mib);
    int  GetCSVCodec() const;
    int  GetDefCSVCodec() const;

    void  SetCSVSeparator(const QChar &separator);
    QChar GetCSVSeparator() const;
    QChar GetDefCSVSeparator() const;

private:
    Q_DISABLE_COPY(VTapeSettings)

    static const QString SettingDataBaseGeometry;
    static const QString SettingDefHeight;
    static const QString SettingDefSize;
    static const QString SettingCSVWithHeader;
    static const QString SettingCSVCodec;
    static const QString SettingCSVSeparator;
};

#endif // VTAPESETTINGS_H
