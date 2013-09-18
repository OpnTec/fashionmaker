#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "tablewindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                context.function);
        abort();
    }
}

int main(int argc, char *argv[]){
    qInstallMessageHandler(myMessageOutput);
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("valentina_" + QLocale::system().name(),".");
    app.installTranslator(&appTranslator);

    MainWindow w;
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    TableWindow table;
    QObject::connect(&w, &MainWindow::ModelChosen, &table, &TableWindow::ModelChosen);
    QObject::connect(&table, &TableWindow::closed, &w, &MainWindow::tableClosed);
    w.show();
    
    return app.exec();
}
