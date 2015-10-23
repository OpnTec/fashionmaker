#ifndef VCMDEXPORT_H
#define VCMDEXPORT_H

#include <memory>
#include <vector>
#include <QTextStream>
#include <QCoreApplication>

#include "dialogs/dialoglayoutsettings.h"
#include "../vmisc/vsysexits.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../vmisc/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif

class VCommandLine;
typedef std::shared_ptr<VCommandLine> VCommandLinePtr;

//@brief: class used to install export command line options and parse their values
//QCommandLineParser* object must exists until this object alive
class VCommandLine
{
public:
    virtual ~VCommandLine();

    //@brief creates object and applies export related options to parser

    //@brief tests if user enabled test mode from cmd, throws exception if not exactly 1 input VAL file supplied in case test mode enabled
    bool IsTestModeEnabled() const;

    //@brief tests if user enabled export from cmd, throws exception if not exactly 1 input VAL file supplied in case export enabled
    bool IsExportEnabled() const;

    //@brief returns path to custom measure file or empty string
    QString OptMeasurePath() const;

    //@brief returns the base name of layout files or empty string if not set
    QString OptBaseName() const;

    //@brief returns the absolute path to output destination directory or path to application's current directory if not set
    QString OptDestinationPath() const;

    //@brief returns export type set, defaults 0 - svg
    int OptExportType() const;

    //generator creation is moved here ... because most options are for it only, so no need to create extra getters...
    //@brief creates VLayoutGenerator
    VLayoutGeneratorPtr DefaultGenerator() const;

    //@brief gets filenames which should be loaded
    QStringList OptInputFileNames() const;

    bool IsGuiEnabled()const;

    bool IsSetGradationSize() const;
    bool IsSetGradationHeight() const;

    QString OptGradationSize() const;
    QString OptGradationHeight() const;

protected:

    VCommandLine();

    //@brief returns DialogLayoutSettings::PaperSizeTemplate
    DialogLayoutSettings::PaperSizeTemplate  OptPaperSize() const;
    //@brief returns rotation in degrees or 0 if not set
    int OptRotation() const;

    Cases OptGroup() const;

    //@brief: called in destructor of application, so instance destroyed and new maybe created (never happen scenario though)
    static void Reset();

    //@brief called to create single object, by VApplication only
    static VCommandLinePtr Get(const QCoreApplication& app);

private:
    Q_DISABLE_COPY(VCommandLine)
    static VCommandLinePtr instance;
    QCommandLineParser parser;
    QMap<QString, QCommandLineOption *> optionsUsed;
    bool isGuiEnabled;
    friend class VApplication;

    static int Lo2Px(const QString& src, const DialogLayoutSettings& converter);
    static int Pg2Px(const QString& src, const DialogLayoutSettings& converter);
    
    static QMap<QString, QCommandLineOption *> InitOptionsUsed();
};

#endif // VCMDEXPORT_H
