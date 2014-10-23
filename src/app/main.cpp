/************************************************************************
 **
 **  @file   main.cpp
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

#include "mainwindow.h"
#include "core/vapplication.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QThread>

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 1)
    #include "core/qcommandlineparser.h"
#else
    #include <QCommandLineParser>
#endif

#include <QtXml>
#include <QLibraryInfo>
#include "tablewindow.h"
#include "version.h"

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
        QByteArray localMsg = msg.toLocal8Bit();
        QMessageBox messageBox;
        switch (type)
        {
            case QtDebugMsg:
                fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                return;
            case QtWarningMsg:
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.setInformativeText(msg);
                messageBox.setStandardButtons(QMessageBox::Ok);
                fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                messageBox.exec();
                break;
            case QtCriticalMsg:
                messageBox.setIcon(QMessageBox::Critical);
                messageBox.setInformativeText(msg);
                messageBox.setStandardButtons(QMessageBox::Ok);
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                messageBox.exec();
                abort();
            case QtFatalMsg:
                messageBox.setIcon(QMessageBox::Critical);
                messageBox.setInformativeText(msg);
                messageBox.setStandardButtons(QMessageBox::Ok);
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                messageBox.exec();
                abort();
            default:
                break;
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
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(cursor);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(theme);

    QT_REQUIRE_VERSION(argc, argv, "5.0.2");

    VApplication app(argc, argv);
#ifdef QT_DEBUG
    // Because our "noisy" message handler uses the GUI subsystem for message
    // boxes, we can't install it until after the QApplication is constructed.  But it
    // is good to be the very next thing to run, to start catching warnings ASAP.
    {
        qInstallMessageHandler(noisyFailureMsgHandler);
    }
#endif
    app.setApplicationDisplayName(VER_PRODUCTNAME_STR);
    app.setApplicationName(VER_INTERNALNAME_STR);
    app.setOrganizationName(VER_COMPANYNAME_STR);
    app.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    // Setting the Application version
    app.setApplicationVersion(APP_VERSION);

    app.OpenSettings();

    QString checkedLocale = qApp->getSettings()->value("configuration/locale", QLocale::system().name()).toString();

    QTranslator qtTranslator;
#if defined(Q_OS_WIN)
    qtTranslator.load("qt_" + checkedLocale, QCoreApplication::applicationDirPath());
#else
    qtTranslator.load("qt_" + checkedLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("valentina_" + checkedLocale, qApp->translationsPath());
    app.installTranslator(&appTranslator);

    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    if (QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK) == false)
    {
        //If there is no default working icon theme then we should
        //use an icon theme that we provide via a .qrc file
        //This case happens under Windows and Mac OS X
        //This does not happen under GNOME or KDE
        QIcon::setThemeName("win.icon.theme");
    }

    MainWindow w;
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    app.setMainWindow(&w);
    TableWindow table;
    QObject::connect(&w, &MainWindow::ModelChosen, &table, &TableWindow::ModelChosen);
    QObject::connect(&table, &TableWindow::closed, &w, &MainWindow::tableClosed);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Pattern file."));
    parser.process(app);
    const QStringList args = parser.positionalArguments();

    //Before we load pattern show window.
    w.show();

    for (int i=0;i<args.size();++i)
    {
        w.LoadPattern(args.at(i));
    }

    return app.exec();
}
