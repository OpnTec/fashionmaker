#include "vcmdexport.h"
#include "dialogs/dialoglayoutsettings.h"
#include "dialogs/dialogsavelayout.h"
#include "xml/vdomdocument.h"
#include "../vformat/vmeasurements.h"

VCommandLinePtr VCommandLine::instance = nullptr;

const static auto LONG_OPTION_BASENAME       = QStringLiteral("basename");
const static auto SINGLE_OPTION_BASENAME     = QStringLiteral("b");

const static auto LONG_OPTION_DESTINATION    = QStringLiteral("destination");
const static auto SINGLE_OPTION_DESTINATION  = QStringLiteral("d");

const static auto LONG_OPTION_MEASUREFILE    = QStringLiteral("mfile");
const static auto SINGLE_OPTION_MEASUREFILE  = QStringLiteral("m");

const static auto LONG_OPTION_PAGETEMPLATE   = QStringLiteral("pageformat");
const static auto SINGLE_OPTION_PAGETEMPLATE = QStringLiteral("p");

const static auto LONG_OPTION_EXP2FORMAT     = QStringLiteral("format");
const static auto SINGLE_OPTION_EXP2FORMAT   = QStringLiteral("f");

const static auto LONG_OPTION_ROTATE         = QStringLiteral("rotate");
const static auto SINGLE_OPTION_ROTATE       = QStringLiteral("r");

const static auto LONG_OPTION_CROP           = QStringLiteral("crop");
const static auto SINGLE_OPTION_CROP         = QStringLiteral("c");

const static auto LONG_OPTION_UNITE          = QStringLiteral("unite");
const static auto SINGLE_OPTION_UNITE        = QStringLiteral("u");

const static auto LONG_OPTION_PAGEW          = QStringLiteral("pagew");
const static auto SINGLE_OPTION_PAGEW        = QStringLiteral("W");

const static auto LONG_OPTION_PAGEH          = QStringLiteral("pageh");
const static auto SINGLE_OPTION_PAGEH        = QStringLiteral("H");

const static auto LONG_OPTION_PAGEUNITS      = QStringLiteral("pageunits");
const static auto SINGLE_OPTION_PAGEUNITS    = QStringLiteral("U");

const static auto LONG_OPTION_SAVELENGTH     = QStringLiteral("savelen");
const static auto SINGLE_OPTION_SAVELENGTH   = QStringLiteral("S");

const static auto LONG_OPTION_SHIFTLENGTH    = QStringLiteral("shiftlen");
const static auto SINGLE_OPTION_SHIFTLENGTH  = QStringLiteral("s");

const static auto LONG_OPTION_SHIFTUNITS     = QStringLiteral("layounits");
const static auto SINGLE_OPTION_SHIFTUNITS   = QStringLiteral("l");

const static auto LONG_OPTION_GAPWIDTH       = QStringLiteral("gapwidth");
const static auto SINGLE_OPTION_GAPWIDTH     = QStringLiteral("G");

const static auto LONG_OPTION_GROUPPING      = QStringLiteral("groups");
const static auto SINGLE_OPTION_GROUPPING    = QStringLiteral("g");

const static auto LONG_OPTION_TEST           = QStringLiteral("test");
const static auto SINGLE_OPTION_TEST         = QStringLiteral("t");

const static auto LONG_OPTION_GRADATIONSIZE   = QStringLiteral("gsize");
const static auto SINGLE_OPTION_GRADATIONSIZE = QStringLiteral("x");

const static auto LONG_OPTION_GRADATIONHEIGHT   = QStringLiteral("gheight");
const static auto SINGLE_OPTION_GRADATIONHEIGHT = QStringLiteral("e");

#define translate(source) QCoreApplication::translate("VCommandLine", (source))

//such a tricky initialization is used, because it uses static functions which relay on static variables and order of
//initialization is not defined between compiled units. i.e. - segv is possible (I hit it when
//VDomDocument::UnitsHelpString() was crashing because of not inited string vars).
//---------------------------------------------------------------------------------------------------------------------
VCommandLine::VCommandLine() : parser()
  , optionsUsed ({
                 //keep in mind order here - that is how user will see it, so group-up for usability
                 //===================================================================================

                 {LONG_OPTION_BASENAME,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_BASENAME << LONG_OPTION_BASENAME,
                                    translate("The base filename of exported layout files. Use it to enable console "
                                              "export mode."),
                                    translate("The base filename of layout files"))},

                 {LONG_OPTION_DESTINATION,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_DESTINATION << LONG_OPTION_DESTINATION,
                                    translate("The path to output destination folder."),
                                    translate("The destination folder"))},

                 {LONG_OPTION_MEASUREFILE,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_MEASUREFILE << LONG_OPTION_MEASUREFILE,
                                    translate("Path to custom measure file (export mode)."),
                                    translate("The measure file"))},

                 {LONG_OPTION_EXP2FORMAT,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_EXP2FORMAT << LONG_OPTION_EXP2FORMAT,
                                    translate("Number corresponding to output format (default = 0, export mode): ") +
                                    DialogSaveLayout::MakeHelpFormatList(), translate("Format number"), "0")},

                 {LONG_OPTION_GRADATIONSIZE,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_GRADATIONSIZE << LONG_OPTION_GRADATIONSIZE,
                                    translate("Set size value a pattern file, that was opened with standard "
                                              "measurements (export mode). Valid values: %1cm.")
                                        .arg(VMeasurement::WholeListSizes(Unit::Cm).join(", ")),
                                    translate("The size value"))},

                 {LONG_OPTION_GRADATIONHEIGHT,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_GRADATIONHEIGHT << LONG_OPTION_GRADATIONHEIGHT,
                                    translate("Set height value a pattern file, that was opened with standard "
                                              "measurements (export mode). Valid values: %1cm.")
                                        .arg(VMeasurement::WholeListHeights(Unit::Cm).join(", ")),
                                    translate("The height value"))},

                 //===================================================================================

                 {LONG_OPTION_PAGETEMPLATE,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGETEMPLATE << LONG_OPTION_PAGETEMPLATE,
                                    translate("Number corresponding to page template (default = 0, export mode): ")+
                                    DialogLayoutSettings::MakeHelpTemplateList(), translate("Template number"), "0")},


                 {LONG_OPTION_PAGEW,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEW << LONG_OPTION_PAGEW,
                                    translate("Page width in current units like 12.0 (cannot be used with \"%1\", "
                                              "export mode).").arg(LONG_OPTION_PAGETEMPLATE),
                                    translate("The page width"))},

                 {LONG_OPTION_PAGEH,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEH << LONG_OPTION_PAGEH,
                                    translate("Page height in current units like 12.0 (cannot be used with \"%1\", "
                                              "export mode).").arg(LONG_OPTION_PAGETEMPLATE), ("The page height"))},

                 {LONG_OPTION_PAGEUNITS,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEUNITS << LONG_OPTION_PAGEUNITS,
                                    translate("Page height/width measure units (cannot be used with \"%1\", export "
                                              "mode): ")
                                        .arg(LONG_OPTION_PAGETEMPLATE) + VDomDocument::UnitsHelpString(),
                                    translate("The measure unit"))},

                 //===================================================================================

                 {LONG_OPTION_ROTATE,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_ROTATE << LONG_OPTION_ROTATE,
                                    translate("Rotation in degrees (one of predefined). Default (or 0) is no-rotate "
                                       "(export mode)."), translate("Angle"))},

                 {LONG_OPTION_CROP,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_CROP << LONG_OPTION_CROP,
                                    translate("Auto crop unused length (export mode)."))},

                 {LONG_OPTION_UNITE,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_UNITE << LONG_OPTION_UNITE,
                                    translate("Unite pages if possible (export mode)."))},

                 //===================================================================================

                 {LONG_OPTION_SAVELENGTH,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_SAVELENGTH << LONG_OPTION_SAVELENGTH,
                                    translate("Save length of the sheet if set. (export mode)."))},

                 {LONG_OPTION_SHIFTUNITS,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_SHIFTUNITS << LONG_OPTION_SHIFTUNITS,
                                    translate("Layout units (as paper's one except px, export mode)."),
                                    translate("The unit"))},

                 {LONG_OPTION_SHIFTLENGTH,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_SHIFTLENGTH << LONG_OPTION_SHIFTLENGTH,
                                    translate("Shift layout length measured in layout units (export mode)."),
                                    translate("Shift length"))},

                 {LONG_OPTION_GAPWIDTH,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_GAPWIDTH << LONG_OPTION_GAPWIDTH,
                                    translate("Gap width x2, measured in layout units. (export mode)."),
                                    translate("The gap width"))},

                 {LONG_OPTION_GROUPPING,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_GROUPPING << LONG_OPTION_GROUPPING,
                                    translate("Sets layout groupping (export mode): ")
                                    + DialogLayoutSettings::MakeGroupsHelp(), translate("Grouping type"), "2")},

                 {LONG_OPTION_TEST,
                 new QCommandLineOption(QStringList() << SINGLE_OPTION_TEST << LONG_OPTION_TEST,
                                    translate("Run the program in a test mode. The program this mode load a single "
                                              "pattern file and silently quit without showing the main window. The "
                                              "key have priority before key '%1'.").arg(LONG_OPTION_BASENAME))}
                 }),
    isGuiEnabled(false)
{
    parser.setApplicationDescription(translate("Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", translate("Pattern file."));

    QMap<QString, QCommandLineOption *>::const_iterator i = optionsUsed.constBegin();
    while (i != optionsUsed.constEnd())
    {
        parser.addOption(*i.value());
        ++i;
    }
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::Lo2Px(const QString &src, const DialogLayoutSettings &converter)
{
    //that is dirty-dirty hack ...eventually number is converted float <--> int 3 or 4 times including inside dialog ...
    // that will loose precision for sure
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
        bool x = parser.isSet(*optionsUsed.value(LONG_OPTION_PAGETEMPLATE));

        bool a = parser.isSet(*optionsUsed.value(LONG_OPTION_PAGEH));
        bool b = parser.isSet(*optionsUsed.value(LONG_OPTION_PAGEW));
        bool c = parser.isSet(*optionsUsed.value(LONG_OPTION_PAGEUNITS));

        if ((a || b || c) && x)
        {
            qCritical() << translate("Cannot use pageformat and page explicit size/units together.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        if ((a || b || c) && !(a && b && c))
        {
            qCritical() << translate("Page height, width, units must be used all 3 at once.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

    }

    //fixme: not really sure ...if shift length must be set with shift units ...or separated, will comment for now.
    //Uncomment if need them both only.

//    {
//        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
//        bool a = parser.isSet(optionsUsed.value(LONG_OPTION_SHIFTLENGTH));
//        bool b = parser.isSet(optionsUsed.value(LONG_OPTION_SHIFTUNITS));

//        if ((a || b) && !(a && b))
//        {
//            qCritical() << translate("Shift length must be used together with shift units.") << "\n";
//            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
//        }
//    }

    int rotateDegree = OptRotation();
    diag.SetRotate(rotateDegree != 0 );

    if (rotateDegree != 0)
    {
        if (!diag.SetIncrease(rotateDegree))
        {
            qCritical() << translate("Invalid rotation value. That must be one of predefined values.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    // if present units MUST be set before any other to keep conversions correct
    if (!diag.SelectTemplate(OptPaperSize()))
    {
        qCritical() << translate("Unknown page templated selected.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }

    if (parser.isSet(*optionsUsed.value(LONG_OPTION_PAGEH))) //at this point we already sure 3 are set or none
    {

        if (!diag.SelectPaperUnit(parser.value(*optionsUsed.value(LONG_OPTION_PAGEUNITS))))
        {
            qCritical() << translate("Unsupported paper units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        diag.SetPaperHeight (Pg2Px(parser.value(*optionsUsed.value(LONG_OPTION_PAGEH)), diag));
        diag.SetPaperWidth  (Pg2Px(parser.value(*optionsUsed.value(LONG_OPTION_PAGEW)), diag));
    }

    if (parser.isSet(*optionsUsed.value(LONG_OPTION_SHIFTUNITS)))
    {
        if (!diag.SelectLayoutUnit(parser.value(*optionsUsed.value(LONG_OPTION_SHIFTUNITS))))
        {
            qCritical() << translate("Unsupported layout units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    if (parser.isSet(*optionsUsed.value(LONG_OPTION_SHIFTLENGTH)))
    {

        diag.SetShift(Lo2Px(parser.value(*optionsUsed.value(LONG_OPTION_SHIFTLENGTH)), diag));
    }

    if (parser.isSet(*optionsUsed.value(LONG_OPTION_GAPWIDTH)))
    {
        diag.SetLayoutWidth(Lo2Px(parser.value(*optionsUsed.value(LONG_OPTION_GAPWIDTH)), diag));
    }

    diag.SetAutoCrop(parser.isSet(*optionsUsed.value(LONG_OPTION_CROP)));
    diag.SetUnitePages(parser.isSet(*optionsUsed.value(LONG_OPTION_UNITE)));
    diag.SetSaveLength(parser.isSet(*optionsUsed.value(LONG_OPTION_SAVELENGTH)));
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
    instance->isGuiEnabled = not (instance->IsExportEnabled() || instance->IsTestModeEnabled());

    return instance;
}

//---------------------------------------------------------------------------------------------------------------------
VCommandLine::~VCommandLine()
{
    qDeleteAll(optionsUsed.begin(), optionsUsed.end());
    optionsUsed.clear();
}

//------------------------------------------------------------------------------------------------------
void VCommandLine::Reset()
{
    instance.reset();
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsTestModeEnabled() const
{
    const bool r = parser.isSet(*optionsUsed.value(LONG_OPTION_TEST));
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("Test option can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportEnabled() const
{
    const bool r = parser.isSet(*optionsUsed.value(LONG_OPTION_BASENAME));
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("Export options can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
DialogLayoutSettings::PaperSizeTemplate VCommandLine::OptPaperSize() const
{
    int ppsize = 0;
    if (parser.isSet(*optionsUsed.value(LONG_OPTION_PAGETEMPLATE)))
    {
        ppsize = parser.value(*optionsUsed.value(LONG_OPTION_PAGETEMPLATE)).toInt();
    }
    return static_cast<DialogLayoutSettings::PaperSizeTemplate>(ppsize);
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::OptRotation() const
{
    int rotate = 0;
    if (parser.isSet(*optionsUsed.value(LONG_OPTION_ROTATE)))
    {
        rotate = parser.value(*optionsUsed.value(LONG_OPTION_ROTATE)).toInt();
    }

    return rotate;
}

//------------------------------------------------------------------------------------------------------
Cases VCommandLine::OptGroup() const
{
    int r = parser.value(*optionsUsed.value(LONG_OPTION_GROUPPING)).toInt();
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
    if (parser.isSet(*optionsUsed.value(LONG_OPTION_MEASUREFILE))
            && IsExportEnabled() //todo: don't want yet to allow user set measure file for general loading, because need to fix multiply opened windows as well
            )
    {
        measure = parser.value(*optionsUsed.value(LONG_OPTION_MEASUREFILE));
    }

    return measure;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptBaseName() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = parser.value(*optionsUsed.value(LONG_OPTION_BASENAME));
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptDestinationPath() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = parser.value(*optionsUsed.value(LONG_OPTION_DESTINATION));
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
int VCommandLine::OptExportType() const
{
    int r = 0;
    if (parser.isSet(*optionsUsed.value(LONG_OPTION_EXP2FORMAT)))
    {
        r = parser.value(*optionsUsed.value(LONG_OPTION_EXP2FORMAT)).toInt();
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommandLine::OptInputFileNames() const
{
    return parser.positionalArguments();
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsGuiEnabled() const
{
    return isGuiEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsSetGradationSize() const
{
    return parser.isSet(*optionsUsed.value(LONG_OPTION_GRADATIONSIZE));
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsSetGradationHeight() const
{
    return parser.isSet(*optionsUsed.value(LONG_OPTION_GRADATIONHEIGHT));
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationSize() const
{
    const QString size = parser.value(*optionsUsed.value(LONG_OPTION_GRADATIONSIZE));
    if (VMeasurement::IsGradationSizeValid(size))
    {
        return size;
    }
    else
    {
        qCritical() << translate("Invalid gradation size value.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationHeight() const
{
    const QString height = parser.value(*optionsUsed.value(LONG_OPTION_GRADATIONHEIGHT));
    if (VMeasurement::IsGradationHeightValid(height))
    {
        return height;
    }
    else
    {
        qCritical() << translate("Invalid gradation height value.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
}

#undef translate
