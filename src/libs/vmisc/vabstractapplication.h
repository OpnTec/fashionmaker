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
#include <QGraphicsScene>
#include "def.h"
#include "vsettings.h"

class VAbstractApplication;// use in define
class VTranslateVars;
class VAbstractPattern;
class VMainGraphicsView;
class QUndoStack;
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
class QLockFile;
#endif

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<VAbstractApplication*>(QCoreApplication::instance()))

class VAbstractApplication : public QApplication
{
public:
    VAbstractApplication(int &argc, char ** argv);
    virtual ~VAbstractApplication() Q_DECL_OVERRIDE;

    virtual const VTranslateVars *TrVars()=0;

    Unit             patternUnit() const;
    const Unit      *patternUnitP() const;
    void             setPatternUnit(const Unit &patternUnit);

    MeasurementsType patternType() const;
    void             setPatternType(const MeasurementsType &patternType);

    virtual void     OpenSettings()=0;
    VCommonSettings *Settings();

    template <typename T>
    QString          LocaleToString(const T &value);

    QGraphicsScene  *getCurrentScene() const;
    void             setCurrentScene(QGraphicsScene *value);

    VMainGraphicsView *getSceneView() const;
    void               setSceneView(VMainGraphicsView *value);

    double           toPixel(double val) const;
    double           fromPixel(double pix) const;

    void             setCurrentDocument(VAbstractPattern *doc);
    VAbstractPattern *getCurrentDocument()const;

    bool             getOpeningPattern() const;
    void             setOpeningPattern();

    QWidget         *getMainWindow() const;
    void             setMainWindow(QWidget *value);

    QUndoStack      *getUndoStack() const;

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    static bool TryLock(QLockFile *lock);
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)

protected:
    QUndoStack         *undoStack;

    /**
     * @brief mainWindow pointer to main window. Usefull if need create modal dialog. Without pointer to main window
     * modality doesn't work.
     */
    QWidget            *mainWindow;

    /**
     * @brief settings pointer to settings. Help hide constructor creation settings. Make make code more readable.
     */
    VCommonSettings    *settings;

private:
    Q_DISABLE_COPY(VAbstractApplication)
    Unit               _patternUnit;
    MeasurementsType   _patternType;


    QGraphicsScene     *currentScene;
    VMainGraphicsView  *sceneView;

    VAbstractPattern   *doc;

    /**
     * @brief openingPattern true when we opening pattern. If something will be wrong in formula this help understand if
     * we can allow user use Undo option.
     */
    bool               openingPattern;
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
inline QString VAbstractApplication::LocaleToString(const T &value)
{
    QLocale loc;
    qApp->Settings()->GetOsSeparator() ? loc = QLocale::system() : loc = QLocale(QLocale::C);
    return loc.toString(value);
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::setCurrentDocument(VAbstractPattern *doc)
{
    this->doc = doc;
}

//---------------------------------------------------------------------------------------------------------------------
inline VAbstractPattern *VAbstractApplication::getCurrentDocument() const
{
    SCASSERT(doc != nullptr)
    return doc;
}

//---------------------------------------------------------------------------------------------------------------------
inline bool VAbstractApplication::getOpeningPattern() const
{
    return openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::setOpeningPattern()
{
    openingPattern = !openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
inline QWidget *VAbstractApplication::getMainWindow() const
{
    return mainWindow;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::setMainWindow(QWidget *value)
{
    SCASSERT(value != nullptr)
    mainWindow = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline QUndoStack *VAbstractApplication::getUndoStack() const
{
    return undoStack;
}

#endif // VABSTRACTAPPLICATION_H
