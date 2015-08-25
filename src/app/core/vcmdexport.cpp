#include "vcmdexport.h"
#include "dialogs/dialoglayoutsettings.h"
#include "dialogs/dialogsavelayout.h"
#include "xml/vdomdocument.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../libs/vmisc/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif

VCommandLinePtr VCommandLine::instance = nullptr;

const static auto OPTION_OUTFILE      = QStringLiteral("outfile");
const static auto OPTION_MEASUREFILE  = QStringLiteral("mfile");
const static auto OPTION_PAGETEMPLATE = QStringLiteral("pageformat");
const static auto OPTION_EXP2FORMAT   = QStringLiteral("format");
const static auto OPTION_ROTATE       = QStringLiteral("rotate");
const static auto OPTION_CROP         = QStringLiteral("crop");
const static auto OPTION_UNITE        = QStringLiteral("unite");

const static auto OPTION_PAGEW        = QStringLiteral("pagew");
const static auto OPTION_PAGEH        = QStringLiteral("pageh");
const static auto OPTION_PAGEUNITS    = QStringLiteral("pageunits");

const static auto OPTION_SAVELENGTH   = QStringLiteral("savelen");
const static auto OPTION_SHIFTLENGTH  = QStringLiteral("shiftlen");
const static auto OPTION_SHIFTUNITS   = QStringLiteral("layounits");
const static auto OPTION_GAPWIDTH     = QStringLiteral("gapwidth");
const static auto OPTION_GROUPPING    = QStringLiteral("groups");

#define tr(A) QCoreApplication::translate("VCommandLine", (A))
//------------------------------------------------------------------------------------------------------

//such a tricky initialization is used, because it uses static functions which relay on static variables and order of initialization is not defined between compiled units.
//i.e. - segv is possible (I hit it when VDomDocument::UnitsHelpString() was crashing because of not inited string vars).
//------------------------------------------------------------------------------------------------------
VCommandLine::VCommandLine() : parser()
  , optionsUsed ({
                 //keep in mind order here - that is how user will see it, so group-up for usability
                 //===================================================================================

                 QCommandLineOption(OPTION_OUTFILE,
                                    tr("Path to output exported layout file. Use it to enable console export mode."),
                                    OPTION_OUTFILE),

                 QCommandLineOption(OPTION_MEASUREFILE,
                                    tr("Path to custom measure file (export mode)."),
                                    OPTION_MEASUREFILE),

                 QCommandLineOption(OPTION_EXP2FORMAT,
                                    tr("Number corresponding to output format (default = 0, export mode): ") +
                                    DialogSaveLayout::MakeHelpFormatList(), OPTION_EXP2FORMAT, "0"),

                 //===================================================================================

                 QCommandLineOption(OPTION_PAGETEMPLATE,
                                    tr("Number corresponding to page template (default = 0, export mode): ")+
                                    DialogLayoutSettings::MakeHelpTemplateList(), OPTION_PAGETEMPLATE, "0"),


                 QCommandLineOption(OPTION_PAGEW,
                                    tr("Page width in current units like 12.0 (cannot be used with \"")
                                    +OPTION_PAGETEMPLATE+tr("\", export mode)."), OPTION_PAGEW),

                 QCommandLineOption(OPTION_PAGEH,
                                    tr("Page height in current units like 12.0 (cannot be used with \"")
                                    +OPTION_PAGETEMPLATE+tr("\", export mode)."), OPTION_PAGEH),

                 QCommandLineOption(OPTION_PAGEUNITS,
                                    tr("Page height/width measure units (cannot be used with \"")+
                                    OPTION_PAGETEMPLATE+tr("\", export mode): ") + VDomDocument::UnitsHelpString(),
                                    OPTION_PAGEUNITS),

                 //===================================================================================

                 QCommandLineOption(OPTION_ROTATE,
                                    tr("Rotation in degrees (one of predefined). Default (or 0) is no-rotate (export mode)."),
                                    OPTION_ROTATE),

                 QCommandLineOption(OPTION_CROP,
                                    tr("Auto crop unused length (export mode).")),

                 QCommandLineOption(OPTION_UNITE,
                                    tr("Unite pages if possible (export mode).")),

                 //===================================================================================

                 QCommandLineOption(OPTION_SAVELENGTH,
                                    tr("Save length of the sheet if set. (export mode).")),

                 QCommandLineOption(OPTION_SHIFTUNITS,
                                    tr("Layout units (as paper's one except px, export mode)."),
                                    OPTION_SHIFTUNITS),

                 QCommandLineOption(OPTION_SHIFTLENGTH,
                                    tr("Shift layout length measured in layout units (export mode)."),
                                    OPTION_SHIFTLENGTH),

                 QCommandLineOption(OPTION_GAPWIDTH,
                                    tr("Gap width x2, measured in layout units. (export mode)."),
                                    OPTION_GAPWIDTH),

                 QCommandLineOption(OPTION_GROUPPING,
                                    tr("Sets layout groupping (export mode): ")
                                    + DialogLayoutSettings::MakeGroupsHelp(), OPTION_GROUPPING, "2"),
                 }),
    isGuiEnabled(false)
{
    parser.setApplicationDescription(QCoreApplication::translate("main", "Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Pattern file."));

    foreach(const auto& o, optionsUsed)
    {
        parser.addOption(o);
    }
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::Lo2Px(const QString &src, const DialogLayoutSettings &converter)
{
    //that is dirty-dirty hack ...eventually number is converted float <--> int 3 or 4 times including inside dialog ... that will loose precision for sure
    return converter.LayoutToPixels(src.toFloat());
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::Pg2Px(const QString& src, const DialogLayoutSettings& converter)
{
    return converter.PageToPixels(src.toFloat());
}

//------------------------------------------------------------------------------------------------------
VLayoutGeneratorPtr VCommandLine::DefaultGenerator() const
{
    //this functions covers all options found into layout setup dialog, nothing to add here, unless dialog extended

    VLayoutGeneratorPtr res(new VLayoutGenerator());
    DialogLayoutSettings diag(res.get(), nullptr, true);

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool x = parser.isSet(OPTION_PAGETEMPLATE);

        bool a = parser.isSet(OPTION_PAGEH);
        bool b = parser.isSet(OPTION_PAGEW);
        bool c = parser.isSet(OPTION_PAGEUNITS);

        if ((a || b || c) && x)
        {
            Error(tr("Cannot use pageformat and page explicit size/units together."));
        }

        if ((a || b || c) && !(a && b && c))
        {
            Error(tr("Page height, width, units must be used all 3 at once."));
        }

    }

    //fixme: not really sure ...if shift length must be set with shift units ...or separated, will comment for now. Uncomment if need them both only.

//    {
//        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
//        bool a = parser.isSet(OPTION_SHIFTLENGTH);
//        bool b = parser.isSet(OPTION_SHIFTUNITS);

//        if ((a || b) && !(a && b))
//        {
//            Error(tr("Shift length must be used together with shift units."));
//        }
//    }

    int rotateDegree = OptRotation();
    diag.SetRotate(rotateDegree != 0 );

    if (rotateDegree != 0)
    {
        if (!diag.SetIncrease(rotateDegree))
        {
            Error(tr("Invalid rotation value. That must be one of predefined values."));
        }
    }

    // if present units MUST be set before any other to keep conversions correct
    if (!diag.SelectTemplate(OptPaperSize()))
    {
        Error(tr("Unknown page templated selected."));
    }

    if (parser.isSet(OPTION_PAGEH)) //at this point we already sure 3 are set or none
    {

        if (!diag.SelectPaperUnit(parser.value(OPTION_PAGEUNITS)))
        {
            Error(tr("Unsupported paper units."));
        }

        diag.SetPaperHeight (Pg2Px(parser.value(OPTION_PAGEH), diag));
        diag.SetPaperWidth  (Pg2Px(parser.value(OPTION_PAGEW), diag));
    }

    if (parser.isSet(OPTION_SHIFTUNITS))
    {
        if (!diag.SelectLayoutUnit(parser.value(OPTION_SHIFTUNITS)))
        {
            Error(tr("Unsupported layout units."));
        }
    }

    if (parser.isSet(OPTION_SHIFTLENGTH))
    {

        diag.SetShift(Lo2Px(parser.value(OPTION_SHIFTLENGTH), diag));
    }

    if (parser.isSet(OPTION_GAPWIDTH))
    {
        diag.SetLayoutWidth(Lo2Px(parser.value(OPTION_GAPWIDTH), diag));
    }

    diag.SetAutoCrop(parser.isSet(OPTION_CROP));
    diag.SetUnitePages(parser.isSet(OPTION_UNITE));
    diag.SetSaveLength(parser.isSet(OPTION_SAVELENGTH));
    diag.SetGroup(OptGroup());



    diag.DialogAccepted(); // filling VLayoutGenerator

    return res;
}

//------------------------------------------------------------------------------------------------------
VCommandLinePtr VCommandLine::Get(const QCoreApplication& app)
{
    if (instance == nullptr)
    {
        instance.reset(new VCommandLine());
    }
    instance->parser.process(app);

    //fixme: in case of additional options/modes which will need to disable GUI - add it here too
    instance->isGuiEnabled = !instance->IsExportEnabled();

    return instance;
}

//------------------------------------------------------------------------------------------------------
NORET_ATTR void VCommandLine::Error(const QString &text) const
{
    qStdErr() << text << "\n";
    const_cast<VCommandLine*>(this)->parser.showHelp(FAILED_HELP_SHOWN_STATUS);
}

//------------------------------------------------------------------------------------------------------
void VCommandLine::Reset()
{
    instance.reset();
}

//------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportEnabled() const
{
    bool r = parser.isSet(OPTION_OUTFILE);
    if (r && parser.positionalArguments().size() != 1)
    {
        Error(tr("Export options can be used with single input file only."));
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
DialogLayoutSettings::PaperSizeTemplate VCommandLine::OptPaperSize() const
{
    int ppsize = 0;
    if (parser.isSet(OPTION_PAGETEMPLATE))
    {
        ppsize = parser.value(OPTION_PAGETEMPLATE).toInt();
    }
    return static_cast<DialogLayoutSettings::PaperSizeTemplate>(ppsize);
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::OptRotation() const
{
    int rotate = 0;
    if (parser.isSet(OPTION_ROTATE))
    {
        rotate = parser.value(OPTION_ROTATE).toInt();
    }

    return rotate;
}

//------------------------------------------------------------------------------------------------------
Cases VCommandLine::OptGroup() const
{
    int r = parser.value(OPTION_GROUPPING).toInt();
    if ( r < 0 || r >= static_cast<int>(Cases::UnknownCase))
    {
        r = 0;
    }
    return static_cast<Cases>(r);
}

//------------------------------------------------------------------------------------------------------
QString VCommandLine::OptMeasurePath() const
{
    QString measure;
    if (parser.isSet(OPTION_MEASUREFILE)
            && IsExportEnabled() //todo: don't want yet to allow user set measure file for general loading, because need to fix multiply opened windows as well
            )
    {
        measure = parser.value(OPTION_MEASUREFILE);
    }

    return measure;
}

//------------------------------------------------------------------------------------------------------
QString VCommandLine::OptExportPath() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = parser.value(OPTION_OUTFILE);
    }

    return path;
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::OptExportType() const
{
    int r = 0;
    if (parser.isSet(OPTION_EXP2FORMAT))
    {
        r = parser.value(OPTION_EXP2FORMAT).toInt();
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommandLine::OptInputFileNames() const
{
    return parser.positionalArguments();
}

//------------------------------------------------------------------------------------------------------
bool VCommandLine::IsGuiEnabled() const
{
    return isGuiEnabled;
}

//------------------------------------------------------------------------------------------------------
#undef tr
