/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "../vmisc/vabstractapplication.h"

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<TestVApplication*>(QCoreApplication::instance()))

class TestVApplication : public VAbstractApplication
{
public:
    TestVApplication(int &argc, char ** argv)
        : VAbstractApplication(argc, argv),
          m_trVars(nullptr)
    {}

    virtual ~TestVApplication() Q_DECL_EQ_DEFAULT;

    virtual const VTranslateVars *TrVars() Q_DECL_OVERRIDE
    {
        return m_trVars;
    }

    virtual void OpenSettings() Q_DECL_OVERRIDE
    {}

    virtual bool IsAppInGUIMode() const Q_DECL_OVERRIDE
    {
        return false;
    }

    virtual void InitTrVars() Q_DECL_OVERRIDE
    {}

    void SetTrVars(VTranslateVars *trVars)
    {
        m_trVars = trVars;
    }
private:
    Q_DISABLE_COPY(TestVApplication)
    VTranslateVars *m_trVars;
};

#endif // TESTVAPPLICATION_H

