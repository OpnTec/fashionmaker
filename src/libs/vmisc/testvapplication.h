/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef TESTVAPPLICATION_H
#define TESTVAPPLICATION_H

#include "vabstractapplication.h"
#include "projectversion.h"

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<TestVApplication*>(QCoreApplication::instance()))

class VTestSettings;

class TestVApplication : public VAbstractApplication
{
public:
    TestVApplication(int &argc, char ** argv)
        : VAbstractApplication(argc, argv),
          m_trVars(nullptr)
    {
        setApplicationName("ValentinaTest");
        setOrganizationName(VER_COMPANYNAME_STR);

        OpenSettings();
    }

    virtual ~TestVApplication() Q_DECL_EQ_DEFAULT;

    virtual const VTranslateVars *TrVars() override
    {
        return m_trVars;
    }

    virtual void OpenSettings() override
    {
        settings = new VSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(),
                                 QCoreApplication::applicationName(), this);
    }

    virtual bool IsAppInGUIMode() const override
    {
        return false;
    }

    virtual void InitTrVars() override
    {}

    void SetTrVars(VTranslateVars *trVars)
    {
        m_trVars = trVars;
    }
private:
    Q_DISABLE_COPY(TestVApplication)
    VTranslateVars *m_trVars;
};

class VTestSettings : public VCommonSettings
{
    Q_OBJECT
public:
    VTestSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
                  QObject *parent = nullptr)
        : VCommonSettings(format, scope, organization, application, parent)
    {
        qRegisterMetaTypeStreamOperators<QMarginsF>("QMarginsF");
    }
};

#endif // TESTVAPPLICATION_H
