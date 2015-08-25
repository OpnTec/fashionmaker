#ifndef VCMDEXPORT_H
#define VCMDEXPORT_H
#include <memory>
#include <vector>
#include <iostream>

#include "dialogs/dialoglayoutsettings.h"


constexpr auto GENERAL_ERROR_STATUS        = 255;
constexpr auto INVALID_PARAMS_STATUS       = 254;
constexpr auto FAILED_TO_GEN_LAYOUT_STATUS = 253;
constexpr auto FAILED_HELP_SHOWN_STATUS    = 250;
constexpr auto FAILED_GEN_BASE_STATUS      = 240;

#define NORET_ATTR Q_NORETURN


//---------------------------------------------------------------------------------------------------------------------
inline QTextStream& qStdErr()
{
    static QTextStream ts( stderr );
    return ts;
}

//---------------------------------------------------------------------------------------------------------------------
NORET_ATTR inline void AppAbort(const QString& text, int code = GENERAL_ERROR_STATUS)
{
    //well ..std::runtime_error was leading to zombies in memory and a lot of dumping all the time ...better to do just exit
    //possibly compiler do not have -fexceptions set
    qStdErr() << text << "\n";
    std::exit(code);
}
//---------------------------------------------------------------------------------------------------------------------


class VCommandLine;
typedef std::shared_ptr<VCommandLine> VCommandLinePtr;

//@brief: class used to install export command line options and parse their values
//QCommandLineParser* object must exists until this object alive
class VCommandLine
{
private:
    static VCommandLinePtr instance;
    QCommandLineParser parser;
    const std::vector<QCommandLineOption> optionsUsed;
    bool isGuiEnabled;
    friend class VApplication;

    static int Lo2Px(const QString& src, const DialogLayoutSettings& converter);
    static int Pg2Px(const QString& src, const DialogLayoutSettings& converter);
protected:

    explicit VCommandLine();


    //@brief returns DialogLayoutSettings::PaperSizeTemplate
    DialogLayoutSettings::PaperSizeTemplate  OptPaperSize() const;
    //@brief returns rotation in degrees or 0 if not set
    int OptRotation() const;

    Cases OptGroup() const;

    //@brief convinient shortcut to show error and help and exit
    NORET_ATTR void Error(const QString& text) const;

    //@brief: called in destructor of application, so instance destroyed and new maybe created (never happen scenario though)
    static void Reset();

    //@brief called to create single object, by VApplication only
    static VCommandLinePtr Get(const QCoreApplication& app);

public:
    VCommandLine(const VCommandLine&) = delete;
    VCommandLine& operator =(const VCommandLine&) = delete;
    virtual ~VCommandLine(){}

    //@brief creates object and applies export related options to parser

    //@brief tests if user enabled export from cmd, throws exception if not exactly 1 input VAL file supplied in case export enabled
    bool IsExportEnabled() const;

    //@brief returns path to custom measure file or empty string
    QString OptMeasurePath() const;

    //@brief returns export path or empty string if not set
    QString OptExportPath() const;

    //@brief returns export type set, defaults 0 - svg
    int OptExportType() const;

    //generator creation is moved here ... because most options are for it only, so no need to create extra getters...
    //@brief creates VLayoutGenerator
    VLayoutGeneratorPtr DefaultGenerator() const;

    //@brief gets filenames which should be loaded
    QStringList OptInputFileNames() const;

    bool IsGuiEnabled()const;
};

#endif // VCMDEXPORT_H
