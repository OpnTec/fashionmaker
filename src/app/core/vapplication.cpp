/************************************************************************
 **
 **  @file   vapplication.cpp
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

#include "vapplication.h"
#include "../libs/ifc/exception/vexceptionobjecterror.h"
#include "../libs/ifc/exception/vexceptionbadid.h"
#include "../libs/ifc/exception/vexceptionconversionerror.h"
#include "../libs/ifc/exception/vexceptionemptyparameter.h"
#include "../libs/ifc/exception/vexceptionwrongid.h"
#include "vmaingraphicsview.h"
#include "../container/calculator.h"
#include "../version.h"

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QTemporaryFile>
#include <QUndoStack>
#include <QtCore/qmath.h>
#include <QTemporaryFile>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QLoggingCategory>
#include <QThread>
#include <QDateTime>
#include <QLockFile>
#include <QtXmlPatterns>

using namespace qmu;

Q_LOGGING_CATEGORY(vApp, "v.application")

//---------------------------------------------------------------------------------------------------------------------
inline void noisyFailureMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Why on earth didn't Qt want to make failed signal/slot connections qWarning?
    if ((type == QtDebugMsg) && msg.contains("::connect"))
    {
        type = QtWarningMsg;
    }

    // this is another one that doesn't make sense as just a debug message.  pretty serious
    // sign of a problem
    // http://www.developer.nokia.com/Community/Wiki/QPainter::begin:Paint_device_returned_engine_%3D%3D_0_(Known_Issue)
    if ((type == QtDebugMsg) && msg.contains("QPainter::begin") && msg.contains("Paint device returned engine"))
    {
        type = QtWarningMsg;
    }

    // This qWarning about "Cowardly refusing to send clipboard message to hung application..."
    // is something that can easily happen if you are debugging and the application is paused.
    // As it is so common, not worth popping up a dialog.
    if ((type == QtWarningMsg) && QString(msg).contains("QClipboard::event")
            && QString(msg).contains("Cowardly refusing"))
    {
        type = QtDebugMsg;
    }

    // only the GUI thread should display message boxes.  If you are
    // writing a multithreaded application and the error happens on
    // a non-GUI thread, you'll have to queue the message to the GUI
    QCoreApplication *instance = QCoreApplication::instance();
    const bool isGuiThread = instance && (QThread::currentThread() == instance->thread());

    if (isGuiThread)
    {
        QString debugdate = "[" + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
        QMessageBox messageBox;
        switch (type)
        {
            case QtDebugMsg:
                debugdate += QString(":DEBUG:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                        .arg(context.function).arg(context.category).arg(msg);
                break;
            case QtWarningMsg:
                debugdate += QString(":WARNING:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                        .arg(context.function).arg(context.category).arg(msg);
                messageBox.setIcon(QMessageBox::Warning);
                break;
            case QtCriticalMsg:
                debugdate += QString(":CRITICAL:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                        .arg(context.function).arg(context.category).arg(msg);
                messageBox.setIcon(QMessageBox::Critical);
                break;
            case QtFatalMsg:
                debugdate += QString(":FATAL:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                        .arg(context.function).arg(context.category).arg(msg);
                messageBox.setIcon(QMessageBox::Critical);
                break;
            default:
                break;
        }

        (*qApp->LogFile()) << debugdate <<  endl;

        if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
        {
            messageBox.setInformativeText(msg);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setWindowModality(Qt::ApplicationModal);
            messageBox.setModal(true);
            messageBox.exec();
        }

        if (QtFatalMsg == type)
        {
            abort();
        }
    }
    else
    {
        if (type != QtDebugMsg)
        {
            abort(); // be NOISY unless overridden!
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

const qreal VApplication::PrintDPI = 96.0;

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
const QString VApplication::GistFileName = QStringLiteral("gist.json");
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

#define DefWidth 1.2//mm

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication constructor.
 * @param argc number arguments.
 * @param argv command line.
 */
VApplication::VApplication(int &argc, char **argv)
    : QApplication(argc, argv), _patternUnit(Unit::Cm), _patternType(MeasurementsType::Individual),
      _widthMainLine(DefWidth), _widthHairLine(DefWidth/3.0), measurements(QMap<QString, QmuTranslation>()),
      guiTexts(QMap<QString, QmuTranslation>()), descriptions(QMap<QString, QmuTranslation>()),
      variables(QMap<QString, QmuTranslation>()), functions(QMap<QString, QmuTranslation>()),
      postfixOperators(QMap<QString, QmuTranslation>()), stDescriptions(QMap<QString, QmuTranslation>()),
      undoStack(nullptr), sceneView(nullptr), currentScene(nullptr), autoSaveTimer(nullptr), mainWindow(nullptr),
      openingPattern(false), settings(nullptr), doc(nullptr), log(nullptr), out(nullptr), logLock(nullptr)
{
    undoStack = new QUndoStack(this);

    InitLineWidth();
    InitMeasurements();
    InitVariables();
    InitFunctions();
    InitPostfixOperators();
    InitSTDescriptions();
}

//---------------------------------------------------------------------------------------------------------------------
VApplication::~VApplication()
{
    qCDebug(vApp)<<"Application closing.";
    qInstallMessageHandler(0); // Resore the message handler
    delete out;

    if (log != nullptr)
    {
        log->close();
        delete log;
        delete logLock;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewValentina start Valentina in new process, send path to pattern file in argument.
 * @param fileName path to pattern file.
 */
void VApplication::NewValentina(const QString &fileName)
{
    qCDebug(vApp)<<"Open new detached process.";
    if (fileName.isEmpty())
    {
        qCDebug(vApp)<<"New process without arguments. program ="<<qApp->applicationFilePath();
        // Path can contain spaces.
        if (QProcess::startDetached("\""+qApp->applicationFilePath()+"\""))
        {
            qCDebug(vApp)<<"The process was started successfully.";
        }
        else
        {
            qCWarning(vApp)<<"Could not run process. The operation timed out or an error occurred.";
        }
    }
    else
    {
        const QString run = QString("\"%1\" \"%2\"").arg(qApp->applicationFilePath()).arg(fileName);
        qCDebug(vApp)<<"New process with arguments. program ="<<run;
        if(QProcess::startDetached(run))
        {
            qCDebug(vApp)<<"The process was started successfully.";
        }
        else
        {
            qCWarning(vApp)<<"Could not run process. The operation timed out or an error occurred.";
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::CheckFactor(qreal &oldFactor, const qreal &Newfactor)
{
    if (Newfactor <= 2 && Newfactor >= 0.5)
    {
        oldFactor = Newfactor;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief notify Reimplemented from QApplication::notify().
 * @param receiver receiver.
 * @param event event.
 * @return value that is returned from the receiver's event handler.
 */
// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file. Program will be terminated."), mainWindow);
        abort();
    }
    catch (const VExceptionBadId &e)
    {
        e.CriticalMessageBox(tr("Error bad id. Program will be terminated."), mainWindow);
        abort();
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value. Program will be terminated."), mainWindow);
        abort();
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter. Program will be terminated."), mainWindow);
        abort();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id. Program will be terminated."), mainWindow);
        abort();
    }
    catch (const VException &e)
    {
        e.CriticalMessageBox(tr("Something's wrong!!"), mainWindow);
        return true;
    }
    catch (std::exception& e)
    {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::toPixel(double val, const Unit &unit) const
{
    switch (unit)
    {
        case Unit::Mm:
            return (val / 25.4) * PrintDPI;
        case Unit::Cm:
            return ((val * 10.0) / 25.4) * PrintDPI;
        case Unit::Inch:
            return val * PrintDPI;
        case Unit::Px:
            return val;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::toPixel(double val) const
{
    return toPixel(val, _patternUnit);
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::fromPixel(double pix, const Unit &unit) const
{
    switch (unit)
    {
        case Unit::Mm:
            return (pix / PrintDPI) * 25.4;
        case Unit::Cm:
            return ((pix / PrintDPI) * 25.4) / 10.0;
        case Unit::Inch:
            return pix / PrintDPI;
        case Unit::Px:
            return pix;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::fromPixel(double pix) const
{
    return fromPixel(pix, _patternUnit);
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::TryLock(QLockFile *lock)
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

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::translationsPath() const
{
    const QString trPath = QStringLiteral("/translations");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + trPath;
#else
#ifdef QT_DEBUG
    return QApplication::applicationDirPath() + trPath;
    #else
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
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitLineWidth()
{
    switch (_patternUnit)
    {
        case Unit::Mm:
            _widthMainLine = DefWidth;
            break;
        case Unit::Cm:
            _widthMainLine = DefWidth/10.0;
            break;
        case Unit::Inch:
            _widthMainLine = DefWidth/25.4;
            break;
        default:
            _widthMainLine = DefWidth;
            break;
    }
    _widthHairLine = _widthMainLine/3.0;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitMeasurement(const QString &name, const QmuTranslation &m, const QmuTranslation &g,
                                   const QmuTranslation &d)
{
    measurements.insert(name, m);
    guiTexts.insert(name, g);
    descriptions.insert(name, d);
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::LogDirPath() const
{
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    const QString logDirPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(),
                                                      QStandardPaths::LocateDirectory) + "Valentina";
#else
    const QString logDirPath = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(),
                                                      QStandardPaths::LocateDirectory) + organizationName();
#endif
    return logDirPath;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::LogPath() const
{
    return QString("%1/valentina-pid%2.log").arg(LogDirPath()).arg(qApp->applicationPid());
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::CreateLogDir() const
{
    QDir logDir(LogDirPath());
    if (logDir.exists() == false)
    {
        logDir.mkpath("."); // Create directory for log if need
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::BeginLogging()
{
    log = new QFile(LogPath());
    if (log->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        out = new QTextStream(log);
        qInstallMessageHandler(noisyFailureMsgHandler);
        logLock = new QLockFile(LogPath()+".lock");
        logLock->setStaleLockTime(0);
        if (TryLock(logLock))
        {
            qCDebug(vApp) << "Log file"<<LogPath()<<"was locked.";
        }
        else
        {
            qCDebug(vApp) << "Failed to lock" << LogPath();
            qCDebug(vApp) << "Error type:"<<logLock->error();
        }
    }
    else
    {
        delete log;
        log = nullptr;
        qCDebug(vApp) << "Error opening log file '" << LogPath() << "'. All debug output redirected to console.";
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::ClearOldLogs() const
{
    QStringList filters{"*.log"};
    QDir logsDir(LogDirPath());
    logsDir.setNameFilters(filters);
    logsDir.setCurrent(LogDirPath());

    const QStringList allFiles = logsDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    if (allFiles.isEmpty() == false)
    {
        qCDebug(vApp) << "Clearing old logs";
        for (int i = 0; i < allFiles.size(); ++i)
        {
            QFileInfo info(allFiles.at(i));
            QLockFile *lock = new QLockFile(info.absoluteFilePath() + ".lock");
            if (TryLock(lock))
            {
                qCDebug(vApp) << "Locked file"<<info.absoluteFilePath();
                QFile oldLog(allFiles.at(i));
                if (oldLog.remove())
                {
                    qCDebug(vApp) << "Deleted"<<info.absoluteFilePath();
                }
                else
                {
                    qCDebug(vApp) << "Could not delete"<<info.absoluteFilePath();
                }
            }
            else
            {
                qCDebug(vApp) << "Failed to lock"<<info.absoluteFilePath();
            }
            delete lock;
            lock = nullptr;
        }
    }
    else
    {
        qCDebug(vApp) << "There are no old logs.";
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitMeasurements()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    // head_and_neck
    m = QmuTranslation::translate("Measurements", "head_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Head girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around fullest part of Head",
                                  "Full measurement description");
    InitMeasurement(headGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "mid_neck_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Mid-neck girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around middle part of Neck",
                                  "Full measurement description");
    InitMeasurement(midNeckGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_base_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck Base girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Neck at base", "Full measurement description");
    InitMeasurement(neckBaseGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_and_neck_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Head and Neck length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Vertical Distance from Crown to Nape",
                                  "Full measurement description");
    InitMeasurement(headAndNeckLength_M, m, g, d);
    //=================================================================================================================
    // torso
    m = QmuTranslation::translate("Measurements", "center_front_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Center length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Neck Center over tape at Bustline to Front Waist "
                                  "Center", "Full measurement description");
    InitMeasurement(centerFrontWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "center_back_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Center length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back Neck Center to Back Waist Center",
                                  "Full measurement description");
    InitMeasurement(centerBackWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to ShoulderTip",
                                  "Full measurement description");
    InitMeasurement(shoulderLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "side_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Side Waist length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Armpit to Waist side", "Full measurement description");
    InitMeasurement(sideWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "trunk_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Trunk length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions",
                                  "Around Body from middle of Shoulder length to BustPoint to Crotch up back to "
                                  "beginning point", "Full measurement description");
    InitMeasurement(trunkLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Arms and Torso, at bicep level parallel to "
                                  "floor, with arms hanging at the sides", "Full measurement description");
    InitMeasurement(shoulderGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "upper_chest_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Upper Chest girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Chest at Armfold level, will be parallel to "
                                  "floor across back, will not be parallel to floor across front chest",
                                  "Full measurement description");
    InitMeasurement(upperChestGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Bust girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around fullest part of Bust, parallel to floor",
                                "Full measurement description");
    InitMeasurement(bustGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "under_bust_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Under Bust girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Chest below the Bust, parallel to floor",
                                "Full measurement description");
    InitMeasurement(underBustGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Tie a string around smallest part of waist, keep string "
                                "tied while taking meaasurements. Not usually parallel to floor for front waist or "
                                "back waist.",
                                  "Full measurement description");
    InitMeasurement(waistGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "high_hip_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "HighHip girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around HighHip, parallel to floor",
                                  "Full measurement description");
    InitMeasurement(highHipGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hip girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Hip, parallel to floor",
                                  "Full measurement description");
    InitMeasurement(hipGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "upper_front_chest_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Upper Chest width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Across Front UpperChest, smallest width from armscye to "
                                  "armscye", "Full measurement description");
    InitMeasurement(upperFrontChestWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_chest_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Chest width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Across Front Chest, from armfold to armfold",
                                  "Full measurement description");
    InitMeasurement(frontChestWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_front_shoulder_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Across Shoulder width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "From ShoulderTip to ShoulderTip, across Front",
                                  "Full measurement description");
    InitMeasurement(acrossFrontShoulderWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_shoulder_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Across Shoulder width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "From ShoulderTip to ShoulderTip, across Back",
                                  "Full measurement description");
    InitMeasurement(acrossBackShoulderWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "upper_back_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Upper Chest width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Across Back UpperChest, smallest width from armscye to "
                                  "armscye", "Full measurement description");
    InitMeasurement(upperBackWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Chest width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Across Back Chest, from armfold to armfold",
                                  "Full measurement description");
    InitMeasurement(backWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "BustPoint to BustPoint", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Distance between BustPoints, across Chest",
                                  "Full measurement description");
    InitMeasurement(bustpointToBustpoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "halter_bustpoint_to_bustpoint",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Halter Bustpoint to Bustpoint", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Distance from Bustpoint, behind neck, down to Bustpoint",
                                  "Full measurement description");
    InitMeasurement(halterBustpointToBustpoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_bustpoint",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "NeckPoint to BustPoint", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "From NeckPoint to BustPoint",
                                  "Full measurement description");
    InitMeasurement(neckToBustpoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Crotch length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "From Front Waist Center, down to crotch, up to Back "
                                  "Waist Center", "Full measurement description");
    InitMeasurement(crotchLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Rise height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Sit on hard chair, measure from side waist straight "
                                  "down to chair bottom", "Full measurement description");
    InitMeasurement(riseHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_drop",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder Drop", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Vertical Distance from NeckPoint level to ShoulderTip "
                                  "level", "Full measurement description");
    InitMeasurement(shoulderDrop_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_degrees",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder Slope degrees", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Degrees of angle from NeckPoint to ShoulderTip – "
                                  "requires goniometer", "Full measurement description");
    InitMeasurement(shoulderSlopeDegrees_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_shoulder_slope_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Shoulder Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "ShoulderTip to Front Waist Center",
                                  "Full measurement description");
    InitMeasurement(frontShoulderSlopeLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_shoulder_slope_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Shoulder Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "ShoulderTip to Back Waist Center",
                                  "Full measurement description");
    InitMeasurement(backShoulderSlopeLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_shoulder_to_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Full Length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint straight down front chest to Waistline",
                                  "Full measurement description");
    InitMeasurement(frontShoulderToWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_shoulder_to_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Full Length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back NeckPoint straight down back chest to Waistline",
                                  "Full measurement description");
    InitMeasurement(backShoulderToWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_neck_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Neck arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to NeckPoint through Front Neck Center",
                                  "Full measurement description");
    InitMeasurement(frontNeckArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_neck_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Neck arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to NeckPoint across Nape",
                                  "Full measurement description");
    InitMeasurement(backNeckArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_upper_chest_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front upper-bust arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front upper-bust arc", "Full measurement description");
    InitMeasurement(frontUpperChestArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_upper_chest_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back UpperBust arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back UpperBust side to side",
                                  "Full measurement description");
    InitMeasurement(backUpperChestArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Waist arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Waist side to side",
                                  "Full measurement description");
    InitMeasurement(frontWaistArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_waist_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Waist arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back Waist side to side",
                                  "Full measurement description");
    InitMeasurement(backWaistArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_upper_hip_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front UpperHip arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front UpperHip side to side",
                                  "Full measurement description");
    InitMeasurement(frontUpperHipArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_upper_hip_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back UpperHip arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back UpperHip side to side",
                                  "Full measurement description");
    InitMeasurement(backUpperHipArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_hip_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Hip arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Hip side to side",
                                  "Full measurement description");
    InitMeasurement(frontHipArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_hip_arc",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Hip arc", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back Hip side to side", "Full measurement description");
    InitMeasurement(backHipArc_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "chest_slope",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Chest Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to Front ArmfoldPoint",
                                "Full measurement description");
    InitMeasurement(chestSlope_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_slope",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to Back ArmfoldPoint",
                                  "Full measurement description");
    InitMeasurement(backSlope_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_slope",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Waist Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint across Front Chest to Waist side",
                                  "Full measurement description");
    InitMeasurement(frontWaistSlope_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_waist_slope",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back Waist Balance", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint across Back Chest to Waist side",
                                  "Full measurement description");
    InitMeasurement(backWaistSlope_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_neck_to_upper_chest_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front UpperChest height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Neck Center straight down to UpperChest line",
                                  "Full measurement description");
    InitMeasurement(frontNeckToUpperChestHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_neck_to_bust_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Bust height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Neck Center straight down to Bust line",
                                  "Full measurement description");
    InitMeasurement(frontNeckToBustHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_to_upper_chest",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front Upper chest waist", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front Upper chest waist",
                                  "Full measurement description");
    InitMeasurement(frontWaistToUpperChest_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_to_lower_breast",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front waist to lower breast", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front waist to lower breast",
                                  "Full measurement description");
    InitMeasurement(frontWaistToLowerBreast_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "back_waist_to_upper_chest",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Back waist to upper chest", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Back waist to upper chest",
                                  "Full measurement description");
    InitMeasurement(backWaistToUpperChest_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "strap_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Strap length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Strap length",
                                  "Full measurement description");
    InitMeasurement(strapLength_M, m, g, d);
    //=================================================================================================================
    // arm
    m = QmuTranslation::translate("Measurements", "armscye_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Armscye Girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Armscye", "Full measurement description");
    InitMeasurement(armscyeGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "elbow_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Elbow Girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Elbow with elbow bent",
                                  "Full measurement description");
    InitMeasurement(elbowGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "upper_arm_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Upperarm Girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around UpperArm", "Full measurement description");
    InitMeasurement(upperArmGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "wrist_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Wrist girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Wrist", "Full measurement description");
    InitMeasurement(wristGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "scye_depth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Armscye depth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Nape straight down to UnderBust line (same as Back "
                                  "UpperBust height)", "Full measurement description");
    InitMeasurement(scyeDepth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_and_arm_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder and Arm length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "NeckPoint to ShoulderTip to Wrist, with elbow bent and "
                                  "hand on hip", "Full measurement description");
    InitMeasurement(shoulderAndArmLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "underarm_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Underarm length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Armpit to Wrist, with arm straight and hanging at side",
                                  "Full measurement description");
    InitMeasurement(underarmLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "cervicale_to_wrist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Nape to wrist length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Nape to Wrist, with elbow bent and hand on hip",
                                  "Full measurement description");
    InitMeasurement(cervicaleToWristLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_to_elbow_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Elbow length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "ShoulderTip to Elbow, with elbow bent and hand on hip",
                                  "Full measurement description");
    InitMeasurement(shoulderToElbowLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Arm length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "ShoulderTip to Wrist, with elbow bent and hand on hip",
                                  "Full measurement description");
    InitMeasurement(armLength_M, m, g, d);
    //=================================================================================================================
    // hand
    m = QmuTranslation::translate("Measurements", "hand_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hand width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Hand side to side", "Full measurement description");
    InitMeasurement(handWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hand length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Hand Middle Finger tip to wrist",
                                "Full measurement description");
    InitMeasurement(handLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hand girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Hand", "Full measurement description");
    InitMeasurement(handGirth_M, m, g, d);
    //=================================================================================================================
    // leg
    m = QmuTranslation::translate("Measurements", "thigh_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Thigh girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Thigh", "Full measurement description");
    InitMeasurement(thighGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "mid_thigh_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Midthigh girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around MidThigh", "Full measurement description");
    InitMeasurement(midThighGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "knee_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Knee girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Knee", "Full measurement description");
    InitMeasurement(kneeGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "calf_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Calf girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Calf", "Full measurement description");
    InitMeasurement(calfGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "ankle_girth",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Ankle girth", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Around Ankle", "Full measurement description");
    InitMeasurement(ankleGirth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "knee_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Knee height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Knee to Floor", "Full measurement description");
    InitMeasurement(kneeHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "ankle_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Ankle height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Ankle to Floor", "Full measurement description");
    InitMeasurement(ankleHeight_M, m, g, d);
    //=================================================================================================================
    // foot
    m = QmuTranslation::translate("Measurements", "foot_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Foot width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Widest part of Foot side to side",
                                  "Full measurement description");
    InitMeasurement(footWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Foot length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Tip of Longest Toe straight to back of heel",
                                  "Full measurement description");
    InitMeasurement(footLength_M, m, g, d);
    //=================================================================================================================
    // heights
    m = QmuTranslation::translate("Measurements", "height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Total Height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Top of head to floor", "Full measurement description");
    InitMeasurement(height_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "cervicale_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Nape height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Nape to Floor", "Full measurement description");
    InitMeasurement(cervicaleHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "cervicale_to_knee_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Nape to knee height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Nape to Knee", "Full measurement description");
    InitMeasurement(cervicaleToKneeHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Waist side to floor", "Full measurement description");
    InitMeasurement(waistHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "high_hip_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "HighHip height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "HighHip side to Floor", "Full measurement description");
    InitMeasurement(highHipHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hip height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Hip side to Floor", "Full measurement description");
    InitMeasurement(hipHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist to Hip height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Waist side to Hip", "Full measurement description");
    InitMeasurement(waistToHipHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_knee_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist to Knee height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Waist side to Knee", "Full measurement description");
    InitMeasurement(waistToKneeHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Crotch height/Inseam", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Crotch to Floor along inside leg",
                                  "Full measurement description");
    InitMeasurement(crotchHeight_M, m, g, d);
    //=================================================================================================================
    //extended
    m = QmuTranslation::translate("Measurements", "size", "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Size", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Size", "Full measurement description");
    InitMeasurement(size_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_front_neck_base_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height front neck base point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height of the point base of the neck in front",
                                  "Full measurement description");
    InitMeasurement(heightFrontNeckBasePoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_base_neck_side_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height base neck side point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height of the base of the neck side point",
                                  "Full measurement description");
    InitMeasurement(heightBaseNeckSidePoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_shoulder_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height shoulder point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The height of the shoulder point",
                                  "Full measurement description");
    InitMeasurement(heightShoulderPoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_nipple_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height nipple point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height nipple point", "Full measurement description");
    InitMeasurement(heightNipplePoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_back_angle_axilla",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height back angle axilla", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height back angle axilla",
                                  "Full measurement description");
    InitMeasurement(heightBackAngleAxilla_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_scapular_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height scapular point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height scapular point", "Full measurement description");
    InitMeasurement(heightScapularPoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_under_buttock_folds",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height under buttock folds", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height under buttock folds",
                                  "Full measurement description");
    InitMeasurement(heightUnderButtockFolds_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hips_excluding_protruding_abdomen",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hips excluding protruding abdomen",
                                  "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Hips excluding protruding abdomen",
                                  "Full measurement description");
    InitMeasurement(hipsExcludingProtrudingAbdomen_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "girth_foot_instep",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Girth foot instep", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Girth foot instep", "Full measurement description");
    InitMeasurement(girthFootInstep_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "side_waist_to_floor",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Side waist to floor", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the side waist to floor",
                                  "Full measurement description");
    InitMeasurement(sideWaistToFloor_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_to_floor",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front waist to floor", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the front waist to floor",
                                  "Full measurement description");
    InitMeasurement(frontWaistToFloor_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arc_through_groin_area",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Arc through groin area", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Arc through groin area", "Full measurement description");
    InitMeasurement(arcThroughGroinArea_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_plane_seat",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist to plane seat", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the waist to the plane seat",
                                  "Full measurement description");
    InitMeasurement(waistToPlaneSeat_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_radial_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to radial point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the base of the neck to the side of "
                                  "the radial point", "Full measurement description");
    InitMeasurement(neckToRadialPoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_third_finger",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to third finger", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Distance from the base of the neck side point to the "
                                  "end of the third finger", "Full measurement description");
    InitMeasurement(neckToThirdFinger_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_first_line_chest_circumference",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to first line chest circumference",
                                  "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the base of the neck to the side of "
                                  "the first line in front of chest circumference", "Full measurement description");
    InitMeasurement(neckToFirstLineChestCircumference_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_waist_length",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front waist length", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the base of the neck to the waist "
                                  "side front (waist length in the front)", "Full measurement description");
    InitMeasurement(frontWaistLength_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arc_through_shoulder_joint",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Arc through shoulder joint", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Arc through the highest point of the shoulder joint",
                                  "Full measurement description");
    InitMeasurement(arcThroughShoulderJoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_back_line_chest_circumference",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to back line chest circumference",
                                  "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the base of the neck to the back line "
                                  "of chest circumference of the first and the second based on ledge vanes",
                                  "Full measurement description");
    InitMeasurement(neckToBackLineChestCircumference_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_neck_side",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist to neck side", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the waist to the back base of the "
                                  "neck side point", "Full measurement description");
    InitMeasurement(waistToNeckSide_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arc_length_upper_body",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Arc length upper body", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Arc length of the upper body through the base of the "
                                  "neck side point", "Full measurement description");
    InitMeasurement(arcLengthUpperBody_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "chest_width",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Chest width", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Chest width", "Full measurement description");
    InitMeasurement(chestWidth_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "anteroposterior_diameter_hands",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Anteroposterior diameter hands", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Anteroposterior diameter of the hands",
                                  "Full measurement description");
    InitMeasurement(anteroposteriorDiameterHands_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_clavicular_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height clavicular point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Height clavicular point",
                                  "Full measurement description");
    InitMeasurement(heightClavicularPoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_armhole_slash",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Height armhole slash", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the point to the cervical level of "
                                  "the posterior angle of the front armpit (underarm height oblique)",
                                  "Full measurement description");
    InitMeasurement(heightArmholeSlash_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "slash_shoulder_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Slash shoulder height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Slash shoulder height", "Full measurement description");
    InitMeasurement(slashShoulderHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_neck",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth neck", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth neck", "Full measurement description");
    InitMeasurement(halfGirthNeck_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_neck_for_shirts",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth neck for shirts", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth neck for shirts",
                                  "Full measurement description");
    InitMeasurement(halfGirthNeckForShirts_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_chest_first",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth chest first", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth chest first", "Full measurement description");
    InitMeasurement(halfGirthChestFirst_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_chest_second",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth chest second", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth chest second",
                                  "Full measurement description");
    InitMeasurement(halfGirthChestSecond_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_chest_third",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth chest third", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth chest third", "Full measurement description");
    InitMeasurement(halfGirthChestThird_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_waist",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth waist", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth waist", "Full measurement description");
    InitMeasurement(halfGirthWaist_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_hips_considering_protruding_abdomen",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth hips considering protruding abdomen",
                                  "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth hips considering protruding abdomen",
                                  "Full measurement description");
    InitMeasurement(halfGirthHipsConsideringProtrudingAbdomen_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "half_girth_hips_excluding_protruding_abdomen",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Half girth hips excluding protruding abdomen",
                                  "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Half girth hips excluding protruding abdomen",
                                  "Full measurement description");
    InitMeasurement(halfGirthHipsExcludingProtrudingAbdomen_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "girth_knee_flexed_feet",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Girth knee flexed feet", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Girth knee flexed feet", "Full measurement description");
    InitMeasurement(girthKneeFlexedFeet_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_transverse_diameter",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck transverse diameter", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Neck transverse diameter",
                                  "Full measurement description");
    InitMeasurement(neckTransverseDiameter_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "front_slash_shoulder_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Front slash shoulder height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Front slash shoulder height",
                                  "Full measurement description");
    InitMeasurement(frontSlashShoulderHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_front_waist_line",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to front waist line", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the base of the neck to the waist "
                                  "line front", "Full measurement description");
    InitMeasurement(neckToFrontWaistLine_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_vertical_diameter",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Hand vertical diameter", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Hand vertical diameter", "Full measurement description");
    InitMeasurement(handVerticalDiameter_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_knee_point",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to knee point", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Distance from neck to knee point",
                                  "Full measurement description");
    InitMeasurement(neckToKneePoint_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_knee",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Waist to knee", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "The distance from the waist to the knee",
                                  "Full measurement description");
    InitMeasurement(waistToKnee_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Shoulder height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Shoulder height", "Full measurement description");
    InitMeasurement(shoulderHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_height",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Head height", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Head height", "Full measurement description");
    InitMeasurement(headHeight_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_position",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Body position", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Body position", "Full measurement description");
    InitMeasurement(bodyPosition_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arc_behind_shoulder_girdle",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Arc behind shoulder girdle", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Arc behind the shoulder girdle",
                                  "Full measurement description");
    InitMeasurement(arcBehindShoulderGirdle_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_to_neck_base",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Neck to neck base", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Distance from neck point to point on the base of the "
                                  "neck side neck girth measurement line", "Full measurement description");
    InitMeasurement(neckToNeckBase_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "depth_waist_first",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Depth waist first", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Depth waist first", "Full measurement description");
    InitMeasurement(depthWaistFirst_M, m, g, d);
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "depth_waist_second",
                                  "Short measurement name. Don't use math symbols in name!!!!");
    g = QmuTranslation::translate("MeasurementsFullNames", "Depth waist second", "Full measurement name");
    d = QmuTranslation::translate("MeasurementsDescriptions", "Depth waist second", "Full measurement description");
    InitMeasurement(depthWaistSecond_M, m, g, d);
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitVariables()
{
    variables.insert(line_, QmuTranslation::translate("Variables", "Line_", "Left symbol _ in name"));
    variables.insert(angleLine_, QmuTranslation::translate("Variables", "AngleLine_", "Left symbol _ in name"));
    variables.insert(arc_, QmuTranslation::translate("Variables", "Arc_", "Left symbol _ in name"));
    variables.insert(spl_, QmuTranslation::translate("Variables", "Spl_", "Left symbol _ in name"));
    variables.insert(splPath, QmuTranslation::translate("Variables", "SplPath",
                                                      "Do not add symbol _ to the end of name"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitFunctions()
{
    functions.insert(sin_F, QmuTranslation::translate("Functions", "sin", "sine function"));
    functions.insert(cos_F, QmuTranslation::translate("Functions", "cos", "cosine function"));
    functions.insert(tan_F, QmuTranslation::translate("Functions", "tan", "tangens function"));
    functions.insert(asin_F, QmuTranslation::translate("Functions", "asin", "arcus sine function"));
    functions.insert(acos_F, QmuTranslation::translate("Functions", "acos", "arcus cosine function"));
    functions.insert(atan_F, QmuTranslation::translate("Functions", "atan", "arcus tangens function"));
    functions.insert(sinh_F, QmuTranslation::translate("Functions", "sinh", "hyperbolic sine function"));
    functions.insert(cosh_F, QmuTranslation::translate("Functions", "cosh", "hyperbolic cosine"));
    functions.insert(tanh_F, QmuTranslation::translate("Functions", "tanh", "hyperbolic tangens function"));
    functions.insert(asinh_F, QmuTranslation::translate("Functions", "asinh", "hyperbolic arcus sine function"));
    functions.insert(acosh_F, QmuTranslation::translate("Functions", "acosh", "hyperbolic arcus tangens function"));
    functions.insert(atanh_F, QmuTranslation::translate("Functions", "atanh", "hyperbolic arcur tangens function"));
    functions.insert(log2_F, QmuTranslation::translate("Functions", "log2", "logarithm to the base 2"));
    functions.insert(log10_F, QmuTranslation::translate("Functions", "log10", "logarithm to the base 10"));
    functions.insert(log_F, QmuTranslation::translate("Functions", "log", "logarithm to the base 10"));
    functions.insert(ln_F, QmuTranslation::translate("Functions", "ln", "logarithm to base e (2.71828...)"));
    functions.insert(exp_F, QmuTranslation::translate("Functions", "exp", "e raised to the power of x"));
    functions.insert(sqrt_F, QmuTranslation::translate("Functions", "sqrt", "square root of a value"));
    functions.insert(sign_F, QmuTranslation::translate("Functions", "sign", "sign function -1 if x<0; 1 if x>0"));
    functions.insert(rint_F, QmuTranslation::translate("Functions", "rint", "round to nearest integer"));
    functions.insert(abs_F, QmuTranslation::translate("Functions", "abs", "absolute value"));
    functions.insert(min_F, QmuTranslation::translate("Functions", "min", "min of all arguments"));
    functions.insert(max_F, QmuTranslation::translate("Functions", "max", "max of all arguments"));
    functions.insert(sum_F, QmuTranslation::translate("Functions", "sum", "sum of all arguments"));
    functions.insert(avg_F, QmuTranslation::translate("Functions", "avg", "mean value of all arguments"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitPostfixOperators()
{
    postfixOperators.insert(cm_Oprt, QmuTranslation::translate("PostfixOperators", "cm", "centimeter"));
    postfixOperators.insert(mm_Oprt, QmuTranslation::translate("PostfixOperators", "mm", "millimeter"));
    postfixOperators.insert(in_Oprt, QmuTranslation::translate("PostfixOperators", "in", "inch"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitSTDescriptions()
{
    stDescriptions.insert("0", QmuTranslation::translate("STDescriptions",
                                                       "Standard figures of men 1st group, chest 100 cm",
                                                       "Standard table description"));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::MeasurementsFromUser translate measurement to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found measurement with same name.
 */
bool VApplication::MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if (token == i.value().translate())
        {
            newFormula.replace(position, token.length(), i.key());
            bias = token.length() - i.key().length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::VariablesFromUser translate variable to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found variable with same name.
 */
bool VApplication::VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if (token.indexOf( i.value().translate() ) == 0)
        {
            newFormula.replace(position, i.value().translate().length(), i.key());
            QString newToken = token;
            newToken.replace(0, i.value().translate().length(), i.key());
            bias = token.length() - newToken.length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::PostfixOperatorsFromUser translate postfix operator to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found postfix operator with same name.
 */
bool VApplication::PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = postfixOperators.constBegin();
    while (i != postfixOperators.constEnd())
    {
        if (token == i.value().translate())
        {
            newFormula.replace(position, token.length(), i.key());
            bias = token.length() - i.key().length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::FunctionsFromUser translate function name to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found function with same name.
 */
bool VApplication::FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = functions.constBegin();
    while (i != functions.constEnd())
    {
        if (token == i.value().translate())
        {
            newFormula.replace(position, token.length(), i.key());
            bias = token.length() - i.key().length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::VariablesToUser translate variable name to user.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found variable with same name.
 */
bool VApplication::VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if (token.indexOf( i.key() ) == 0)
        {
            newFormula.replace(position, i.key().length(), i.value().translate());

            QString newToken = token;
            newToken.replace(0, i.key().length(), i.value().translate());
            bias = token.length() - newToken.length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::CorrectionsPositions correct position tokens in expression after token translation.
 *
 * Because translated string can have different length compare to original need make correction after each translation.
 * If bias = 0 correction will not happens.
 *
 * @param position position currecnt token in expression
 * @param bias difference between original token length and translated
 * @param tokens all tokens
 * @param numbers all numbers
 */
void VApplication::CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens,
                                        QMap<int, QString> &numbers)
{
    if (bias == 0)
    {
        return;// Nothing to correct;
    }

    BiasTokens(position, bias, tokens);
    BiasTokens(position, bias, numbers);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::BiasTokens change position for each token that have position more then "position".
 * @param position token position
 * @param bias difference between original token length and translated
 * @param tokens all tokens
 */
void VApplication::BiasTokens(int position, int bias, QMap<int, QString> &tokens) const
{
    QMap<int, QString> newTokens;
    QMap<int, QString>::const_iterator i = tokens.constBegin();
    while (i != tokens.constEnd())
    {
        if (i.key()<= position)
        { // Tokens before position "position" did not change his positions.
            newTokens.insert(i.key(), i.value());
        }
        else
        {
            newTokens.insert(i.key()-bias, i.value());
        }
        ++i;
    }
    tokens = newTokens;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setPatternUnit(const Unit &patternUnit)
{
    _patternUnit = patternUnit;
    InitLineWidth();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::VarToUser(const QString &var) const
{
    if (measurements.contains(var))
    {
        return measurements.value(var).translate();
    }

    if (functions.contains(var))
    {
        return functions.value(var).translate();
    }

    if (postfixOperators.contains(var))
    {
        return postfixOperators.value(var).translate();
    }

    QString newVar = var;
    int bias = 0;
    if (VariablesToUser(newVar, 0, var, bias))
    {
        return newVar;
    }
    return newVar;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::VarFromUser(const QString &var) const
{
    QString newVar = var;
    int bias = 0;
    if (MeasurementsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (VariablesFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (PostfixOperatorsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (FunctionsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }
    return newVar;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::GuiText(const QString &measurement) const
{
    return guiTexts.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Description(const QString &measurement) const
{
    return descriptions.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::PostfixOperator(const QString &name) const
{
    return postfixOperators.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::FormulaFromUser replace all known tokens in formula to internal look. Also change decimal
 * separator in numbers.
 * @param formula expression that need translate
 * @return translated expression
 */
QString VApplication::FormulaFromUser(const QString &formula)
{
    QString newFormula = formula;// Local copy for making changes

    Calculator *cal = new Calculator(formula);// Eval formula
    QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
    QMap<int, QString> numbers = cal->GetNumbers();// All numbers in expression for changing decimal separator
    delete cal;

    QList<int> tKeys = tokens.keys();// Take all tokens positions
    QList<QString> tValues = tokens.values();
    for (int i = 0; i < tKeys.size(); ++i)
    {
        int bias = 0;
        if (MeasurementsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (VariablesFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (PostfixOperatorsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (FunctionsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QLocale loc = QLocale::system(); // User locale
    if (loc != QLocale(QLocale::C) && getSettings()->GetOsSeparator())
    {// User want use Os separator
        QList<int> nKeys = numbers.keys();// Positions for all numbers in expression
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            loc = QLocale::system();// From system locale
            bool ok = false;
            const qreal d = loc.toDouble(nValues.at(i), &ok);
            if (ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;//Leave with out translation
            }

            loc = QLocale(QLocale::C);// To internal locale
            const QString dStr = loc.toString(d);// Internal look for number
            newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
            const int bias = nValues.at(i).length() - dStr.length();
            if (bias != 0)
            {// Translated number has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                nKeys = numbers.keys();
                nValues = numbers.values();
            }
        }
    }

    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::FormulaToUser replace all known tokens in formula to user look. Also change decimal
 * separator in numbers.
 * @param formula expression that need translate
 * @return translated expression
 */
QString VApplication::FormulaToUser(const QString &formula)
{
    QString newFormula = formula;// Local copy for making changes

    QMap<int, QString> tokens;
    QMap<int, QString> numbers;
    try
    {
        Calculator *cal = new Calculator(formula, false);// Eval formula
        tokens = cal->GetTokens();// Tokens (variables, measurements)
        numbers = cal->GetNumbers();// All numbers in expression for changing decimal separator
        delete cal;
    }
    catch (qmu::QmuParserError &e)
    {
        qDebug() << "\nMath parser error:\n"
                 << "--------------------------------------\n"
                 << "Message:     " << e.GetMsg()  << "\n"
                 << "Expression:  " << e.GetExpr() << "\n"
                 << "--------------------------------------";
        return newFormula;
    }

    QList<int> tKeys = tokens.keys();
    QList<QString> tValues = tokens.values();
    for (int i = 0; i < tKeys.size(); ++i)
    {
        if (measurements.contains(tValues.at(i)))
        {
            newFormula.replace(tKeys.at(i), tValues.at(i).length(), measurements.value(tValues.at(i)).translate());
            int bias = tValues.at(i).length() - measurements.value(tValues.at(i)).translate().length();
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (functions.contains(tValues.at(i)))
        {
            newFormula.replace(tKeys.at(i), tValues.at(i).length(), functions.value(tValues.at(i)).translate());
            int bias = tValues.at(i).length() - functions.value(tValues.at(i)).translate().length();
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (postfixOperators.contains(tValues.at(i)))
        {
            newFormula.replace(tKeys.at(i), tValues.at(i).length(), postfixOperators.value(tValues.at(i)).translate());
            int bias = tValues.at(i).length() - postfixOperators.value(tValues.at(i)).translate().length();
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        int bias = 0;
        if (VariablesToUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QLocale loc = QLocale::system();// User locale
    if (loc != QLocale::C && getSettings()->GetOsSeparator())
    {// User want use Os separator
        QList<int> nKeys = numbers.keys();// Positions for all numbers in expression
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            loc = QLocale(QLocale::C);// From pattern locale
            bool ok = false;
            const qreal d = loc.toDouble(nValues.at(i), &ok);
            if (ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;//Leave with out translation
            }

            loc = QLocale::system();// To user locale
            const QString dStr = loc.toString(d);// Number string in user locale
            newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
            const int bias = nValues.at(i).length() - dStr.length();
            if (bias != 0)
            {// Translated number has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                nKeys = numbers.keys();
                nValues = numbers.values();
            }
        }
    }

    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
QWidget *VApplication::getMainWindow() const
{
    return mainWindow;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setMainWindow(QWidget *value)
{
    SCASSERT(value != nullptr)
    mainWindow = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::getOpeningPattern() const
{
    return openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setOpeningPattern()
{
    openingPattern = !openingPattern;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::OpenSettings get acsses to application settings.
 *
 * Because we can create object in constructor we open file separately.
 */
void VApplication::OpenSettings()
{
    settings = new VSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                             QApplication::applicationName(), this);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication::getSettings hide settings constructor.
 * @return pointer to class for acssesing to settings in ini file.
 */
VSettings *VApplication::getSettings()
{
    SCASSERT(settings != nullptr);
    return settings;
}

//---------------------------------------------------------------------------------------------------------------------
VMainGraphicsScene *VApplication::getCurrentScene() const
{
    SCASSERT(currentScene != nullptr);
    return currentScene;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setCurrentScene(VMainGraphicsScene *value)
{
    currentScene = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setOverrideCursor(const QString &pixmapPath, int hotX, int hotY)
{
#ifndef QT_NO_CURSOR
    QPixmap oldPixmap;
    if (QCursor *oldCursor = QGuiApplication::overrideCursor())
    {
        oldPixmap = oldCursor->pixmap();
    }
    QPixmap newPixmap(pixmapPath);

    QImage oldImage = oldPixmap.toImage();
    QImage newImage = newPixmap.toImage();

    if (oldImage != newImage )
    {
        QApplication::setOverrideCursor(QCursor(newPixmap, hotX, hotY));
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::restoreOverrideCursor(const QString &pixmapPath)
{
#ifndef QT_NO_CURSOR
    QPixmap oldPixmap;
    if (QCursor *oldCursor = QGuiApplication::overrideCursor())
    {
        oldPixmap = oldCursor->pixmap();
    }
    QPixmap newPixmap(pixmapPath);

    QImage oldImage = oldPixmap.toImage();
    QImage newImage = newPixmap.toImage();

    if (oldImage == newImage )
    {
        QApplication::restoreOverrideCursor();
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VApplication::LabelLanguages()
{
    QStringList list = QStringList() << "de" // German
                                     << "en" // English
                                     << "fr" // French
                                     << "ru" // Russian
                                     << "uk" // Ukrainian
                                     << "hr" // Croatian
                                     << "sr" // Serbian
                                     << "bs"; // Bosnian
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::STDescription(const QString &id) const
{
    if (stDescriptions.contains(id))
    {
        return stDescriptions.value(id).translate();
    }
    else
    {
        qDebug()<<"Unknown id number. Got"<<id;
    }
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::StartLogging()
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 3, 0))
    // In Qt 5.2 need manualy enable debug information for categories. This work
    // because Qt doesn't provide debug information for categories itself. And in this
    // case will show our messages. Another situation with Qt 5.3 that has many debug
    // messages itself. We don't need this information and can turn on later if need.
    // But here Qt already show our debug messages without enabling.
    QLoggingCategory::setFilterRules("*.debug=true\n");
#endif

    CreateLogDir();
    BeginLogging();
    ClearOldLogs();
#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    ClearOldReports();
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)
}

//---------------------------------------------------------------------------------------------------------------------
QTextStream *VApplication::LogFile()
{
    return out;
}

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
//---------------------------------------------------------------------------------------------------------------------
void VApplication::ClearOldReports() const
{
    const QString reportsDir = QString("%1/reports").arg(qApp->applicationDirPath());
    QDir reports(reportsDir);
    if (reports.exists())
    {
        QStringList filters{"*.log", "*.RPT"};
        QDir logsDir(reportsDir);
        logsDir.setNameFilters(filters);
        logsDir.setCurrent(reportsDir);

        const QStringList allFiles = logsDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        if (allFiles.isEmpty() == false)
        {
            const QDateTime now = QDateTime::currentDateTime();
            for (int i = 0; i < allFiles.size(); ++i)
            {
                QFileInfo info(allFiles.at(i));
                if (info.created().daysTo(now) > 30)
                {
                    QFile(allFiles.at(i)).remove();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::GatherLogs() const
{
    QTextStream *out = nullptr;
    QFile *log = new QFile(QString("%1/valentina.log").arg(LogDirPath()));
    if (log->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        out = new QTextStream(log);

        QStringList filters{"*.log"};
        QDir logsDir(LogDirPath());
        logsDir.setNameFilters(filters);
        logsDir.setCurrent(LogDirPath());

        const QStringList allFiles = logsDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        if (allFiles.isEmpty() == false)
        {
            for (int i = 0; i < allFiles.size(); ++i)
            {
                QFileInfo info(allFiles.at(i));
                if (info.fileName() == "valentina.log")
                {
                    continue;
                }
                QLockFile *logLock = new QLockFile(info.absoluteFilePath()+".lock");
                logLock->setStaleLockTime(0);
                if (TryLock(logLock))
                {
                    *out <<"--------------------------" << endl;
                    QFile logFile(info.absoluteFilePath());
                    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    {
                        QTextStream in(&logFile);
                        while (!in.atEnd())
                        {
                            *out << in.readLine() << endl;
                        }
                        logFile.close();
                    }
                    else
                    {
                        *out << "Log file error:" + logFile.errorString() << endl;
                    }
                }
                else
                {
                    *out << "Could not lock" << info.absoluteFilePath() << ".";
                }
                delete logLock;
            }
        }
        else
        {
            *out << "Could not find logs.";
        }
        log->close();
    }
    delete out;
    delete log;
}

//---------------------------------------------------------------------------------------------------------------------
// Catch exception and create report. Use if program build with Mingw compiler.
// See more about catcher https://github.com/jrfonseca/drmingw/blob/master/README.md
void VApplication::DrMingw()
{
    QFile drmingw("exchndl.dll");
    if(drmingw.exists())
    {// If don't want create reports just delete exchndl.dll from installer
        LoadLibrary(L"exchndl.dll");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::CollectReports() const
{
    // Seek file "binary_name.RPT"
    const QString reportName = QString("%1/%2.RPT").arg(applicationDirPath())
            .arg(QFileInfo(arguments().at(0)).baseName());
    QFile reportFile(reportName);
    if (reportFile.exists())
    { // Hooray we have found crash
        if (settings == nullptr)
        {
            return;// Settings was not opened.
        }

        if (settings->GetSendReportState())
        { // Try send report
            // Remove gist.json file before close app.
            connect(this, &VApplication::aboutToQuit, this, &VApplication::CleanGist, Qt::UniqueConnection);
            SendReport(reportName);
        }
        else
        { // Just collect report to /reports directory
            CollectReport(reportName);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::CollectReport(const QString &reportName) const
{
    const QString reportsDir = QString("%1/reports").arg(qApp->applicationDirPath());
    QDir reports(reportsDir);
    if (reports.exists() == false)
    {
        reports.mkpath("."); // Create directory for reports if need
    }

    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy.MM.dd-hh_mm_ss"));
    QString filename = QString("%1/reports/crash-%2.RPT").arg(qApp->applicationDirPath()).arg(timestamp);

    QFile reportFile(reportName);
    reportFile.copy(filename); // Collect new crash
    reportFile.remove(); // Clear after yourself

    filename = QString("%1/reports/log-%2.log").arg(qApp->applicationDirPath()).arg(timestamp);
    GatherLogs();
    QFile logFile(QString("%1/valentina.log").arg(LogDirPath()));
    logFile.copy(filename); // Collect log
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::CleanGist() const
{
    QFile gistFile(GistFileName);
    if (gistFile.exists())
    {
        gistFile.remove();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::SendReport(const QString &reportName) const
{
    QString content;
    QFile reportFile(reportName);
    if (reportFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = ReadFileForSending(reportFile);
        reportFile.close();
    }
    else
    {
        content = "RPT file error:" + reportFile.errorString() + "\r\n";
    }

    // Additional information
    content.append(QString("-------------------------------")+"\r\n");
    content.append(QString("Version:%1").arg(APP_VERSION)+"\r\n");
    content.append(QString("Build revision:%1").arg(BUILD_REVISION)+"\r\n");
    content.append(QString("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR)+"\r\n");
    content.append(QString("Built on %3 at %4").arg(__DATE__).arg(__TIME__)+"\r\n");
    content.append("\r\n");

    // Creating json with report
    // Example:
    //{
    //  "description":"Crash report",
    //  "public":"true",
    //  "files":{
    //      "valentina.RPT":{
    //          "content":"Report text here"
    //      }
    //  }
    //}

    // Useful to know when crash was created
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy/MM/dd hh:mm:ss:zzz"));
    const QString report = QString("Crash report was created %2").arg(timestamp);

    QJsonObject reportObject;
    reportObject.insert(QStringLiteral("description"), QJsonValue(report));
    reportObject.insert(QStringLiteral("public"), QJsonValue(QString("true")));

    content.append(QString("\r\n-------------------------------\r\n"));
    content.append(QString("Log:")+"\r\n");

    GatherLogs();
    QFile logFile(QString("%1/valentina.log").arg(LogDirPath()));
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        const QString logContent = ReadFileForSending(logFile);
        if (logContent.isEmpty())
        {
            content.append("Log file is empty.");
        }
        else
        {
            content.append(logContent);
        }
        logFile.close();
    }
    else
    {
        content.append("\r\n Log file error:" + logFile.errorString() + "\r\n");
    }

    const QString contentSection = QStringLiteral("content");
    QJsonObject contentObject;
    contentObject.insert(contentSection, QJsonValue(content));

    const QString filesSection = QStringLiteral("files");
    QJsonObject fileObject;
    fileObject.insert(QFileInfo(reportName).fileName(), QJsonValue(contentObject));
    reportObject.insert(filesSection, QJsonValue(fileObject));

    QFile gistFile(GistFileName);
    if (!gistFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug("Couldn't open gist file.");
        return;
    }

    // Save data to file
    QJsonDocument saveRep(reportObject);
    gistFile.write(saveRep.toJson());
    gistFile.close();

    QFile curlFile("curl.exe");
    if (curlFile.exists())
    {// Trying send report
        // Change token 28df778e0ef75e3724f7b9622fb70b9c69187779 if need
        QString arg = QString("curl.exe -k -H \"Authorization: bearer 28df778e0ef75e3724f7b9622fb70b9c69187779\" "
                              "-H \"Accept: application/json\" -H \"Content-type: application/json\" -X POST "
                              "--data @gist.json https://api.github.com/gists");
        QProcess::startDetached(arg);
        reportFile.remove();// Clear after yourself
    }
    else
    {// We can not send than just collect
        CollectReport(reportName);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::ReadFileForSending(QFile &file) const
{
    QString content;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        content.append(in.readLine()+"\r\n");// Windows end of line
    }
    return content;
}
#endif //defined(Q_OS_WIN) && defined(Q_CC_GNU)
