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

class VApplication;
class QUndoStack;
class VMainGraphicsView;

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
    virtual ~VApplication() {}
    virtual bool       notify(QObject * receiver, QEvent * event);
    Unit               patternUnit() const;
    void               setPatternUnit(const Unit &patternUnit);
    MeasurementsType   patternType() const;
    void               setPatternType(const MeasurementsType &patternType);
    double             toPixel(double unit) const;
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
    QString            Variable(const QString &name) const;
    QString            Function(const QString &name) const;
    QString            PostfixOperator(const QString &name) const;
    QString            FormulaFromUser(const QString &formula);
    QString            FormulaToUser(const QString &formula);
    QUndoStack         *getUndoStack() const;
    VMainGraphicsView  *getSceneView() const;
    void               setSceneView(VMainGraphicsView *value);
    QTimer             *getAutoSaveTimer() const;
    void               setAutoSaveTimer(QTimer *value);
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
    QUndoStack         *undoStack;
    VMainGraphicsView  *sceneView;
    QTimer             *autoSaveTimer;
    void               InitLineWidth();
    void               InitMeasurements();
    void               InitVariables();
    void               InitFunctions();
    void               InitPostfixOperators();
    bool               MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token,
                                                int &bias) const;
    bool               FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;
    bool               VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const;
    void               CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens,
                                            QMap<int, QString> &numbers);
    void               BiasTokens(int position, int bias, QMap<int, QString> &tokens) const;
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

#endif // VAPPLICATION_H
