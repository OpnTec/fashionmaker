/************************************************************************
 **
 **  @file   vapplication.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include <QApplication>
#include "../options.h"

class VApplication;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<VApplication*>(QCoreApplication::instance()))

/**
 * @brief The VApplication class reimplamentation QApplication class.
 */
class VApplication : public QApplication
{
    Q_OBJECT
public:
    /**
     * @brief VApplication constructor.
     * @param argc number arguments.
     * @param argv command line.
     */
    VApplication(int &argc, char ** argv);
    virtual ~VApplication() {}
    /**
     * @brief notify Reimplemented from QApplication::notify().
     * @param receiver receiver.
     * @param event event.
     * @return value that is returned from the receiver's event handler.
     */
    virtual bool          notify(QObject * receiver, QEvent * event);
    Valentina::Units      patternUnit() const;
    void                  setPatternUnit(const Valentina::Units &patternUnit);
    Pattern::Measurements patternType() const;
    void                  setPatternType(const Pattern::Measurements &patternType);
    double                toPixel(double unit) const;
    double                fromPixel(double pix) const;
    static const qreal    PrintDPI;
    QString               translationsPath() const;
    QString               pathToTables() const;
    qreal                 widthMainLine() const;
    qreal                 widthHairLine() const;
private:
    Valentina::Units      _patternUnit;
    Pattern::Measurements _patternType;
    qreal                 _widthMainLine;
    qreal                 _widthHairLine;
    void                  InitLineWidth();
};

inline Valentina::Units VApplication::patternUnit() const
{
    return _patternUnit;
}

inline Pattern::Measurements VApplication::patternType() const
{
    return _patternType;
}

inline void VApplication::setPatternType(const Pattern::Measurements &patternType)
{
    _patternType = patternType;
}

inline qreal VApplication::widthMainLine() const
{
    return _widthMainLine;
}

inline qreal VApplication::widthHairLine() const
{
    return _widthHairLine;
}

#endif // VAPPLICATION_H
