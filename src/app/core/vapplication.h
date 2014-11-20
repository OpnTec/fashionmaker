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
#include "vtranslation.h"
#include <QSettings>
#include "../widgets/vmaingraphicsview.h"

class VApplication;// used in define
class QUndoStack;
class VMainGraphicsView;
class VMainGraphicsScene;
class VPattern;
class QFile;

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
    void               setPatternUnit(const Unit &patternUnit);
    MeasurementsType   patternType() const;
    void               setPatternType(const MeasurementsType &patternType);

    double             toPixel(double val, const Unit &unit) const;
    double             toPixel(double val) const;
    double             fromPixel(double pix, const Unit &unit) const;
    double             fromPixel(double pix) const;

    static const qreal PrintDPI;
    QString            translationsPath() const;
    QString            pathToTables() const;
    qreal              widthMainLine() const;
    qreal              widthHairLine() const;
    QString            VarToUser(const QString &var) const;
    QString            VarFromUser(const QString &var) const;
    QString            GuiText(const QString &measurement) const;
    QString            Description(const QString &measurement) const;
    QString            PostfixOperator(const QString &name) const;
    QString            FormulaFromUser(const QString &formula);
    QString            FormulaToUser(const QString &formula);
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
    QSettings          *getSettings();
    VMainGraphicsScene *getCurrentScene() const;
    void               setCurrentScene(VMainGraphicsScene *value);

    void               setCurrentDocument(VPattern *doc);
    VPattern           *getCurrentDocument()const;

    static void        setOverrideCursor(const QString & pixmapPath, int hotX = -1, int hotY = -1);
    static void        restoreOverrideCursor(const QString & pixmapPath);

    static QStringList LabelLanguages();
    QString            STDescription(const QString &id)const;
    static bool        SafeCopy(const QString &source, const QString &destination, QString &error);
    void               StartLogging();
    QTextStream       *LogFile();

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
    qreal              _widthMainLine;
    qreal              _widthHairLine;
    QMap<QString, VTranslation> measurements;
    QMap<QString, VTranslation> guiTexts;
    QMap<QString, VTranslation> descriptions;
    QMap<QString, VTranslation> variables;
    QMap<QString, VTranslation> functions;
    QMap<QString, VTranslation> postfixOperators;
    QMap<QString, VTranslation> stDescriptions;
    QUndoStack         *undoStack;
    VMainGraphicsView  *sceneView;
    VMainGraphicsScene *currentScene;
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
    QSettings          *settings;

    VPattern           *doc;
    QFile              *log;
    QTextStream        *out;
    void               InitLineWidth();
    void               InitMeasurements();
    void               InitVariables();
    void               InitFunctions();
    void               InitPostfixOperators();
    void               InitSTDescriptions();
    bool               MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token,
                                                int &bias) const;
    bool               FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const;
    void               CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens,
                                            QMap<int, QString> &numbers);
    void               BiasTokens(int position, int bias, QMap<int, QString> &tokens) const;
    void               InitMeasurement(const QString &name, const VTranslation &m, const VTranslation &g,
                                       const VTranslation &d);

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    static const QString GistFileName;

    void               CollectReport(const QString &reportName) const;
    void               SendReport(const QString &reportName) const;
    QString            ReadFileForSending(QFile &file)const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

    QString            LogDirPath()const;
    QString            LogPath()const;
};

//---------------------------------------------------------------------------------------------------------------------
inline Unit VApplication::patternUnit() const
{
    return _patternUnit;
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
inline qreal VApplication::widthMainLine() const
{
    return _widthMainLine;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VApplication::widthHairLine() const
{
    return _widthHairLine;
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
