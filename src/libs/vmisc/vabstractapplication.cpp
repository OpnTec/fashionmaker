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
#include "../vmisc/logging.h"

#include <QDir>
#include <QLibraryInfo>
#include <QTranslator>
#include <QtDebug>

//---------------------------------------------------------------------------------------------------------------------
VAbstractApplication::VAbstractApplication(int &argc, char **argv)
    :QApplication(argc, argv),
      undoStack(nullptr),
      mainWindow(nullptr),
      settings(nullptr),
      qtTranslator(nullptr),
      qtxmlTranslator(nullptr),
      qtBaseTranslator(nullptr),
      appTranslator(nullptr),
      pmsTranslator(nullptr),
      _patternUnit(Unit::Cm),
      _patternType(MeasurementsType::Unknown),
      currentScene(nullptr),
      sceneView(nullptr),
      doc(nullptr),
      openingPattern(false)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    // Qt < 5.2 didn't feature categorized logging
    // Do nothing
#else
    // In Qt 5.2 need manualy enable debug information for categories. This work
    // because Qt doesn't provide debug information for categories itself. And in this
    // case will show our messages. Another situation with Qt 5.3 that has many debug
    // messages itself. We don't need this information and can turn on later if need.
    // But here Qt already show our debug messages without enabling.
    QLoggingCategory::setFilterRules("*.debug=true\n");
#endif // QT_VERSION < QT_VERSION_CHECK(5, 2, 0)

#endif // QT_VERSION < QT_VERSION_CHECK(5, 3, 0)

    // Enable support for HiDPI bitmap resources
    setAttribute(Qt::AA_UseHighDpiPixmaps);

    connect(this, &QApplication::aboutToQuit, [this]()
    {
        // If try to use the method QApplication::exit program can't sync settings and show warning about QApplication
        // instance. Solution is to call sync() before quit.
        // Connect this slot with VApplication::aboutToQuit.
        Settings()->sync();
    });

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 1)
#if defined(V_NO_ASSERT)
    // Ignore SSL-related warnings
    // See issue #528: Error: QSslSocket: cannot resolve SSLv2_client_method.
    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");
#endif //defined(V_NO_ASSERT)
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 4, 1)
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractApplication::~VAbstractApplication()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief translationsPath return path to the root directory that contain QM files.
 * @param locale used only in Mac OS. If empty return path to the root directory. If not - return path to locale
 * subdirectory inside an app bundle.
 * @return path to a directory that contain QM files.
 */
QString VAbstractApplication::translationsPath(const QString &locale) const
{
    const QString trPath = QStringLiteral("/translations");
#ifdef Q_OS_WIN
    Q_UNUSED(locale)
    return QApplication::applicationDirPath() + trPath;
#elif defined(Q_OS_MAC)
    QString mainPath;
    if (locale.isEmpty())
    {
        mainPath = QApplication::applicationDirPath() + QLatin1String("/../Resources") + trPath;
    }
    else
    {
        mainPath = QApplication::applicationDirPath() + QLatin1String("/../Resources") + trPath + QLatin1String("/")
                + locale + QLatin1String(".lproj");
    }
    QDir dirBundle(mainPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir(QApplication::applicationDirPath() + trPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/translations");
        }
    }
#else // Unix
    Q_UNUSED(locale)
    QDir dir(QApplication::applicationDirPath() + trPath);
    if (dir.exists())
    {
        return dir.absolutePath();
    }
    else
    {
        return QStringLiteral("/usr/share/valentina/translations");
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType VAbstractApplication::patternType() const
{
    return _patternType;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setPatternType(const MeasurementsType &patternType)
{
    _patternType = patternType;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setCurrentDocument(VAbstractPattern *doc)
{
    this->doc = doc;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPattern *VAbstractApplication::getCurrentDocument() const
{
    SCASSERT(doc != nullptr)
    return doc;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractApplication::getOpeningPattern() const
{
    return openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setOpeningPattern()
{
    openingPattern = !openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
QWidget *VAbstractApplication::getMainWindow() const
{
    return mainWindow;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setMainWindow(QWidget *value)
{
    SCASSERT(value != nullptr)
    mainWindow = value;
}

//---------------------------------------------------------------------------------------------------------------------
QUndoStack *VAbstractApplication::getUndoStack() const
{
    return undoStack;
}

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

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::LoadTranslation(const QString &locale)
{
    if (locale.isEmpty())
    {
        qDebug()<<"Locale is empty.";
        return;
    }
    qDebug()<<"Checked locale:"<<locale;

    ClearTranslation();

    qtTranslator = new QTranslator(this);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    qtTranslator->load("qt_" + locale, translationsPath(locale));
#else
    qtTranslator->load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    installTranslator(qtTranslator);

    qtxmlTranslator = new QTranslator(this);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    qtxmlTranslator->load("qtxmlpatterns_" + locale, translationsPath(locale));
#else
    qtxmlTranslator->load("qtxmlpatterns_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    installTranslator(qtxmlTranslator);

    qtBaseTranslator = new QTranslator(this);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    qtBaseTranslator->load("qtbase_" + locale, translationsPath(locale));
#else
    qtBaseTranslator->load("qtbase_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    installTranslator(qtBaseTranslator);

    appTranslator = new QTranslator(this);
    appTranslator->load("valentina_" + locale, translationsPath(locale));
    installTranslator(appTranslator);

    const QString system = Settings()->GetPMSystemCode();

    pmsTranslator = new QTranslator(this);
    pmsTranslator->load("measurements_" + system + "_" + locale, translationsPath(locale));
    installTranslator(pmsTranslator);

    InitTrVars();//Very important do it after load QM files.
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::ClearTranslation()
{
    if (not qtTranslator.isNull())
    {
        removeTranslator(qtTranslator);
        delete qtTranslator;
    }

    if (not qtxmlTranslator.isNull())
    {
        removeTranslator(qtxmlTranslator);
        delete qtxmlTranslator;
    }

    if (not qtBaseTranslator.isNull())
    {
        removeTranslator(qtBaseTranslator);
        delete qtBaseTranslator;
    }

    if (not appTranslator.isNull())
    {
        removeTranslator(appTranslator);
        delete appTranslator;
    }

    if (not pmsTranslator.isNull())
    {
        removeTranslator(pmsTranslator);
        delete pmsTranslator;
    }
}
