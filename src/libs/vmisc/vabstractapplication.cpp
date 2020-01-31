/************************************************************************
 **
 **  @file   vabstractapplication.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include <QDir>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QTranslator>
#include <QUndoStack>
#include <Qt>
#include <QtDebug>
#include <QWidget>
#include <QStandardPaths>

#include "../vmisc/def.h"
#include "../vmisc/customevents.h"

#ifdef Q_OS_UNIX
#  include <unistd.h>
#endif

const QString VAbstractApplication::patternMessageSignature = QStringLiteral("[PATTERN MESSAGE]");

//---------------------------------------------------------------------------------------------------------------------
VAbstractApplication::VAbstractApplication(int &argc, char **argv)
    :QApplication(argc, argv),
      undoStack(new QUndoStack(this)),
      mainWindow(nullptr),
      settings(nullptr),
      qtTranslator(nullptr),
      qtxmlTranslator(nullptr),
      qtBaseTranslator(nullptr),
      appTranslator(nullptr),
      pmsTranslator(nullptr),
      _patternUnit(Unit::Cm),
      _patternType(MeasurementsType::Unknown),
      patternFilePath(),
      currentScene(nullptr),
      sceneView(nullptr),
      doc(nullptr),
      m_customerName(),
      m_userMaterials(),
      openingPattern(false),
      mode(Draw::Calculation)
{
    QString rules;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 1)
#if defined(V_NO_ASSERT)
    // Ignore SSL-related warnings
    // See issue #528: Error: QSslSocket: cannot resolve SSLv2_client_method.
    rules += QLatin1String("qt.network.ssl.warning=false\n");
    // See issue #568: Certificate checking on Mac OS X.
    rules += QLatin1String("qt.network.ssl.critical=false\n"
                           "qt.network.ssl.fatal=false\n");
#endif //defined(V_NO_ASSERT)
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 4, 1)

#if defined(V_NO_ASSERT)
    // See issue #992: QXcbConnection: XCB Error.
    rules += QLatin1String("qt.qpa.*=false\n");
#endif

    // cppcheck-suppress reademptycontainer
    if (not rules.isEmpty())
    {
        QLoggingCategory::setFilterRules(rules);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    // Enable support for HiDPI bitmap resources
    // The attribute is available since Qt 5.1, but by default disabled.
    // Because on Windows and Mac OS X we always use last version
    // and Linux users send bug reports probably they related to this attribute
    // better not enable it before Qt 5.6.
    //
    // Related issues:
    // Issue #584. frequent xcb errors and hangs
    // https://bitbucket.org/dismine/valentina/issues/584/frequent-xcb-errors-and-hangs
    // Issue #527. Error: Pasting a wrong formula : every dialog box is "glued" to the screen and can't close file
    // or Valentina.
    // https://bitbucket.org/dismine/valentina/issues/527/error-pasting-a-wrong-formula-every-dialog

    setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    connect(this, &QApplication::aboutToQuit, this, &VAbstractApplication::AboutToQuit);
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
    return QCoreApplication::applicationDirPath() + trPath;
#elif defined(Q_OS_MAC)
    QString mainPath;
    if (locale.isEmpty())
    {
        mainPath = QCoreApplication::applicationDirPath() + QLatin1String("/../Resources") + trPath;
    }
    else
    {
        mainPath = QCoreApplication::applicationDirPath() + QLatin1String("/../Resources") + trPath + QLatin1String("/")
                + locale + QLatin1String(".lproj");
    }
    QDir dirBundle(mainPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir appDir = QDir(qApp->applicationDirPath());
        appDir.cdUp();
        appDir.cdUp();
        appDir.cdUp();
        QDir dir(appDir.absolutePath() + trPath);
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
    QDir dir(QCoreApplication::applicationDirPath() + trPath);
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
bool VAbstractApplication::IsPedantic() const
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearMessage helps to clear a message string from standard Qt function.
 * @param msg the message that contains '"' at the start and at the end
 * @return cleared string
 */
QString VAbstractApplication::ClearMessage(QString msg)
{
    if (msg.startsWith('"') && msg.endsWith('"'))
    {
        msg.remove(0, 1);
        msg.chop(1);
    }

    return msg;
}

//---------------------------------------------------------------------------------------------------------------------
const Draw &VAbstractApplication::GetDrawMode() const
{
    return mode;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::SetDrawMode(const Draw &value)
{
    mode = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::PostPatternMessage(const QString &message, QtMsgType severity) const
{
    QApplication::postEvent(mainWindow, new PatternMessageEvent(VAbstractApplication::ClearMessage(message), severity));
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractApplication::IsPatternMessage(const QString &message) const
{
    return VAbstractApplication::ClearMessage(message).startsWith(patternMessageSignature);
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_WIN)
void VAbstractApplication::WinAttachConsole()
{
    /* Windows does not really support dual mode applications.
     * To see console output we need to attach console.
     * For case of using pipeline we check std output handler.
     * Original idea: https://stackoverflow.com/a/41701133/3045403
     */
    auto stdout_type = GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
    if (stdout_type == FILE_TYPE_UNKNOWN && AttachConsole(ATTACH_PARENT_PROCESS))
    {
        // cppcheck-suppress ignoredReturnValue
        freopen("CONOUT$", "w", stdout);
        // cppcheck-suppress ignoredReturnValue
        freopen("CONOUT$", "w", stderr);
    }
}
#endif

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
    SCASSERT(settings != nullptr)
    return settings;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsScene *VAbstractApplication::getCurrentScene() const
{
    SCASSERT(*currentScene != nullptr)
    return *currentScene;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractApplication::setCurrentScene(QGraphicsScene **value)
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
