/************************************************************************
 **
 **  @file   vapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../widgets/vmaingraphicsview.h"
#include "../libs/vpatterndb/vtranslatevars.h"
#include "vsettings.h"

class VApplication;// used in define
class QUndoStack;
class VMainGraphicsView;
class VPattern;
class QFile;
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
class QLockFile;
#endif

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
    VApplication(int &argc, char ** argv);
    virtual ~VApplication();
    static void        NewValentina(const QString &fileName = QString());
    static void        CheckFactor(qreal &oldFactor, const qreal &Newfactor);
    virtual bool       notify(QObject * receiver, QEvent * event);
    Unit               patternUnit() const;
    const Unit        *patternUnitP() const;
    void               setPatternUnit(const Unit &patternUnit);
    MeasurementsType   patternType() const;
    void               setPatternType(const MeasurementsType &patternType);

    void               InitOptions();

    double             toPixel(double val) const;
    double             fromPixel(double pix) const;

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    static bool        TryLock(QLockFile *lock);
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)

    QString            translationsPath() const;

    template <typename T>
    QString            LocaleToString(const T &value)
    {
        QLocale loc;
        qApp->getSettings()->GetOsSeparator() ? loc = QLocale::system() : loc = QLocale(QLocale::C);
        return loc.toString(value);
    }

    QUndoStack         *getUndoStack() const;
    VMainGraphicsView  *getSceneView() const;
    void               setSceneView(VMainGraphicsView *value);
    QTimer             *getAutoSaveTimer() const;
    void               setAutoSaveTimer(QTimer *value);
    QWidget            *getMainWindow() const;
    void               setMainWindow(QWidget *value);
    bool               getOpeningPattern() const;
    void               setOpeningPattern();

    void               OpenSettings();
    VSettings          *getSettings();

    QGraphicsScene    *getCurrentScene() const;
    void               setCurrentScene(QGraphicsScene *value);

    void               setCurrentDocument(VPattern *doc);
    VPattern           *getCurrentDocument()const;

    static void        setOverrideCursor(const QString & pixmapPath, int hotX = -1, int hotY = -1);
    static void        restoreOverrideCursor(const QString & pixmapPath);

    static QStringList LabelLanguages();

    void               StartLogging();
    QTextStream       *LogFile();

    const VTranslateVars *TrVars();
    void               InitTrVars();


#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    static void        DrMingw();
    void               CollectReports() const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

private slots:
#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    void               CleanGist() const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

private:
    Q_DISABLE_COPY(VApplication)
    Unit               _patternUnit;
    MeasurementsType   _patternType;
    VTranslateVars     *trVars;
    QUndoStack         *undoStack;
    VMainGraphicsView  *sceneView;
    QGraphicsScene     *currentScene;
    QTimer             *autoSaveTimer;
    /**
     * @brief mainWindow pointer to main window. Usefull if need create modal dialog. Without pointer to main window
     * modality doesn't work.
     */
    QWidget            *mainWindow;
    /**
     * @brief openingPattern true when we opening pattern. If something will be wrong in formula this help understand if
     * we can allow user use Undo option.
     */
    bool               openingPattern;
    /**
     * @brief settings pointer to settings. Help hide constructor creation settings. Make make code more readable.
     */
    VSettings          *settings;

    VPattern           *doc;
    QFile              *log;
    QTextStream        *out;
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    QLockFile          *logLock;
#endif
    void               InitLineWidth();

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    static const QString GistFileName;

    void               CollectReport(const QString &reportName) const;
    void               SendReport(const QString &reportName) const;
    QString            ReadFileForSending(QFile &file)const;
    void               ClearOldReports()const;
    void               GatherLogs()const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

    QString            LogDirPath()const;
    QString            LogPath()const;
    void               CreateLogDir()const;
    void               BeginLogging();
    void               ClearOldLogs()const;
};

//---------------------------------------------------------------------------------------------------------------------
inline Unit VApplication::patternUnit() const
{
    return _patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
inline const Unit *VApplication::patternUnitP() const
{
    return &_patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
inline MeasurementsType VApplication::patternType() const
{
    return _patternType;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VApplication::setPatternType(const MeasurementsType &patternType)
{
    _patternType = patternType;
}

//---------------------------------------------------------------------------------------------------------------------
inline QUndoStack *VApplication::getUndoStack() const
{
    return undoStack;
}

//---------------------------------------------------------------------------------------------------------------------
inline VMainGraphicsView *VApplication::getSceneView() const
{
    return sceneView;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VApplication::setSceneView(VMainGraphicsView *value)
{
    sceneView = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline QTimer *VApplication::getAutoSaveTimer() const
{
    return autoSaveTimer;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VApplication::setAutoSaveTimer(QTimer *value)
{
    autoSaveTimer = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VApplication::setCurrentDocument(VPattern *doc)
{
    this->doc = doc;
}

//---------------------------------------------------------------------------------------------------------------------
inline VPattern *VApplication::getCurrentDocument() const
{
    SCASSERT(doc != nullptr)
    return doc;
}

#endif // VAPPLICATION_H
