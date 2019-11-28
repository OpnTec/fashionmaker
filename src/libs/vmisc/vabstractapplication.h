/************************************************************************
 **
 **  @file   vabstractapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <qcompilerdetection.h>
#include <QApplication>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QLocale>
#include <QMetaObject>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../vpatterndb/vtranslatevars.h"
#include "def.h"
#include "vcommonsettings.h"
#include "vlockguard.h"
#include "vsettings.h"

class QUndoStack;
class VAbstractApplication;// use in define
class VAbstractPattern;
class VMainGraphicsView;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<VAbstractApplication*>(QCoreApplication::instance()))

class VAbstractApplication : public QApplication
{
    Q_OBJECT
public:
    VAbstractApplication(int &argc, char ** argv);
    virtual ~VAbstractApplication() override;

    virtual const VTranslateVars *TrVars()=0;

    QString          translationsPath(const QString &locale = QString()) const;

    void             LoadTranslation(const QString &locale);

    Unit             patternUnit() const;
    const Unit      *patternUnitP() const;
    void             setPatternUnit(const Unit &patternUnit);

    MeasurementsType patternType() const;
    void             setPatternType(const MeasurementsType &patternType);

    QString GetCustomerName() const;
    void    SetCustomerName(const QString &name);

    virtual void     OpenSettings()=0;
    VCommonSettings *Settings();

    template <typename T>
    QString          LocaleToString(const T &value);

    QGraphicsScene  *getCurrentScene() const;
    void             setCurrentScene(QGraphicsScene **value);

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

    virtual bool     IsAppInGUIMode()const =0;
    virtual bool     IsPedantic() const;

    QString         GetPatternPath() const;
    void            SetPatternPath(const QString &value);

#if defined(Q_OS_WIN)
    static void WinAttachConsole();
#endif

    static QString ClearMessage(QString msg);

    QMap<int, QString> GetUserMaterials() const;
    void               SetUserMaterials(const QMap<int, QString> &userMaterials);

    const Draw &GetDrawMode() const;
    void        SetDrawMode(const Draw &value);

    void PostPatternMessage(const QString &message, QtMsgType severity) const;

    static const QString patternMessageSignature;
    bool IsPatternMessage(const QString &message) const;

#if defined(APPIMAGE) && defined(Q_OS_LINUX)
    static void SetICUData(int &argc, char ** argv);
#endif

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

    QPointer<QTranslator> qtTranslator;
    QPointer<QTranslator> qtxmlTranslator;
    QPointer<QTranslator> qtBaseTranslator;
    QPointer<QTranslator> appTranslator;
    QPointer<QTranslator> pmsTranslator;

    virtual void InitTrVars()=0;

protected slots:
    virtual void AboutToQuit()=0;

private:
    Q_DISABLE_COPY(VAbstractApplication)
    Unit               _patternUnit;
    MeasurementsType   _patternType;
    QString            patternFilePath;

    QGraphicsScene     **currentScene;
    VMainGraphicsView  *sceneView;

    VAbstractPattern   *doc;
    QString            m_customerName;

    QMap<int, QString> m_userMaterials;

    /**
     * @brief openingPattern true when we opening pattern. If something will be wrong in formula this help understand if
     * we can allow user use Undo option.
     */
    bool               openingPattern;

    /** @brief mode keep current draw mode. */
    Draw               mode;

    void ClearTranslation();
};

//---------------------------------------------------------------------------------------------------------------------
inline QString VAbstractApplication::GetCustomerName() const
{
    return m_customerName;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::SetCustomerName(const QString &name)
{
    m_customerName = name;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VAbstractApplication::GetPatternPath() const
{
    return patternFilePath;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::SetPatternPath(const QString &value)
{
    patternFilePath = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline QMap<int, QString> VAbstractApplication::GetUserMaterials() const
{
    return m_userMaterials;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VAbstractApplication::SetUserMaterials(const QMap<int, QString> &userMaterials)
{
    m_userMaterials = userMaterials;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline QString VAbstractApplication::LocaleToString(const T &value)
{
    QLocale loc;
    qApp->Settings()->GetOsSeparator() ? loc = QLocale() : loc = QLocale::c();
    return loc.toString(value);
}

#endif // VABSTRACTAPPLICATION_H
