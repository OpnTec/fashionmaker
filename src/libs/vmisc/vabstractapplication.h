/************************************************************************
 **
 **  @file   vabstractapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 6, 2015
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

#ifndef VABSTRACTAPPLICATION_H
#define VABSTRACTAPPLICATION_H

#include <QApplication>
#include "def.h"
#include "vsettings.h"

class VAbstractApplication;// use in define
class VTranslateVars;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<VAbstractApplication*>(QCoreApplication::instance()))

class VAbstractApplication : public QApplication
{
public:
    VAbstractApplication(int &argc, char ** argv);
    virtual ~VAbstractApplication();

    virtual const VTranslateVars *TrVars()=0;

    MeasurementsType patternType() const;
    void             setPatternType(const MeasurementsType &patternType);

    void             OpenSettings();
    VSettings        *getSettings();

    template <typename T>
    QString          LocaleToString(const T &value);


private:
    Q_DISABLE_COPY(VAbstractApplication)
    MeasurementsType   _patternType;
    /**
     * @brief settings pointer to settings. Help hide constructor creation settings. Make make code more readable.
     */
    VSettings          *settings;
};

//---------------------------------------------------------------------------------------------------------------------
inline MeasurementsType VAbstractApplication::patternType() const
{
    return _patternType;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::setPatternType(const MeasurementsType &patternType)
{
    _patternType = patternType;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QString VAbstractApplication::LocaleToString(const T &value)
{
    QLocale loc;
    qApp->getSettings()->GetOsSeparator() ? loc = QLocale::system() : loc = QLocale(QLocale::C);
    return loc.toString(value);
}

#endif // VABSTRACTAPPLICATION_H
