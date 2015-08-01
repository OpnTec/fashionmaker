/************************************************************************
 **
 **  @file   vabstractapplication.cpp
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

#include "vabstractapplication.h"
#include "../vmisc/def.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
#   include <QLockFile>
#endif

//---------------------------------------------------------------------------------------------------------------------
VAbstractApplication::VAbstractApplication(int &argc, char **argv)
    :QApplication(argc, argv),
      undoStack(nullptr),
      mainWindow(nullptr),
      settings(nullptr),
      _patternUnit(Unit::Cm),
      _patternType(MeasurementsType::Individual),
      currentScene(nullptr),
      sceneView(nullptr),
      doc(nullptr),
      openingPattern(false)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractApplication::~VAbstractApplication()
{}

//---------------------------------------------------------------------------------------------------------------------
Unit VAbstractApplication::patternUnit() const
{
    return _patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
const Unit *VAbstractApplication::patternUnitP() const
{
    return &_patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setPatternUnit(const Unit &patternUnit)
{
    _patternUnit = patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSettings hide settings constructor.
 * @return pointer to class for acssesing to settings in ini file.
 */
VCommonSettings *VAbstractApplication::Settings()
{
    SCASSERT(settings != nullptr);
    return settings;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsScene *VAbstractApplication::getCurrentScene() const
{
    SCASSERT(currentScene != nullptr);
    return currentScene;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setCurrentScene(QGraphicsScene *value)
{
    currentScene = value;
}

//---------------------------------------------------------------------------------------------------------------------
VMainGraphicsView *VAbstractApplication::getSceneView() const
{
    return sceneView;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setSceneView(VMainGraphicsView *value)
{
    sceneView = value;
}

//---------------------------------------------------------------------------------------------------------------------
double VAbstractApplication::toPixel(double val) const
{
    return ToPixel(val, _patternUnit);
}

//---------------------------------------------------------------------------------------------------------------------
double VAbstractApplication::fromPixel(double pix) const
{
    return FromPixel(pix, _patternUnit);
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
//---------------------------------------------------------------------------------------------------------------------
bool VAbstractApplication::TryLock(QLockFile *lock)
{
    if (lock == nullptr)
    {
        return false;
    }

    if (lock->tryLock())
    {
        return true;
    }
    else
    {
        if (lock->error() == QLockFile::LockFailedError)
        {
            // This happens if a stale lock file exists and another process uses that PID.
            // Try removing the stale file, which will fail if a real process is holding a
            // file-level lock. A false error is more problematic than not locking properly
            // on corner-case systems.
            if (lock->removeStaleLockFile() == false || lock->tryLock() == false)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
        return false;
    }
}

#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
