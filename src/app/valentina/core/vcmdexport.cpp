/************************************************************************
 **
 **  @file   vcmdexport.cpp
 **  @author Alex Zaharov <alexzkhr@gmail.com>
 **  @date   25 8, 2015
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

#include "vcmdexport.h"
#include "../dialogs/dialoglayoutsettings.h"
#include "../dialogs/dialogsavelayout.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vformat/vmeasurements.h"
#include "../vmisc/commandoptions.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/dialogs/dialogexporttocsv.h"
#include "../vlayout/vlayoutgenerator.h"
#include "../vpatterndb/variables/vmeasurement.h"
#include <QDebug>
#include <QTextCodec>

VCommandLinePtr VCommandLine::instance = nullptr;

#define translate(context, source) QCoreApplication::translate((context), (source))

namespace
{
//---------------------------------------------------------------------------------------------------------------------
qreal Lo2Px(const QString &src, const DialogLayoutSettings &converter)
{
    return converter.LayoutToPixels(src.toDouble());
}

//---------------------------------------------------------------------------------------------------------------------
qreal Pg2Px(const QString& src, const DialogLayoutSettings& converter)
{
    return converter.PageToPixels(src.toDouble());
}
} // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
VCommandLine::VCommandLine() : parser(), isGuiEnabled(false)
{
    parser.setApplicationDescription(translate("VCommandLine", "Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QStringLiteral("filename"), translate("VCommandLine", "Pattern file."));

    InitCommandLineOptions();
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractLayoutDialog::PaperSizeTemplate VCommandLine::FormatSize(const QString &key) const
{
    int ppsize = 0;
    if (IsOptionSet(key))
    {
        ppsize = OptionValue(key).toInt();
    }
    return static_cast<VAbstractLayoutDialog::PaperSizeTemplate>(ppsize);
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutGeneratorPtr VCommandLine::DefaultGenerator() const
{
    //this functions covers all options found into layout setup dialog, nothing to add here, unless dialog extended

    VLayoutGeneratorPtr res(new VLayoutGenerator());
    DialogLayoutSettings diag(res.get(), nullptr, true);

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool x = IsOptionSet(LONG_OPTION_PAGETEMPLATE);

        bool a = IsOptionSet(LONG_OPTION_PAGEH);
        bool b = IsOptionSet(LONG_OPTION_PAGEW);
        bool c = IsOptionSet(LONG_OPTION_PAGEUNITS);

        if ((a || b) && x)
        {
            qCritical() << translate("VCommandLine", "Cannot use pageformat and page explicit size together.")
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        if ((a || b || c) && !(a && b && c))
        {
            qCritical() << translate("VCommandLine", "Page height, width, units must be used all 3 at once.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = IsOptionSet(LONG_OPTION_GAPWIDTH);
        bool b = IsOptionSet(LONG_OPTION_SHIFTUNITS);

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Gap width must be used together with shift units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    auto CheckKey = [this](const QString &key, const QString &message)
    {
        bool a = IsOptionSet(key);
        bool b = IsOptionSet(LONG_OPTION_PAGEUNITS);

        if ((a || b) && !(a && b))
        {
            qCritical() << message << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    };

    if (not IsOptionSet(LONG_OPTION_IGNORE_MARGINS))
    {
        CheckKey(LONG_OPTION_LEFT_MARGIN,
                 translate("VCommandLine", "Left margin must be used together with page units."));
        CheckKey(LONG_OPTION_RIGHT_MARGIN,
                 translate("VCommandLine", "Right margin must be used together with page units."));
        CheckKey(LONG_OPTION_TOP_MARGIN,
                 translate("VCommandLine", "Top margin must be used together with page units."));
        CheckKey(LONG_OPTION_BOTTOM_MARGIN,
                 translate("VCommandLine", "Bottom margin must be used together with page units."));
    }

    if (static_cast<LayoutExportFormats>(OptExportType()) == LayoutExportFormats::PDFTiled)
    {
        CheckKey(LONG_OPTION_TILED_PDF_LEFT_MARGIN,
                 translate("VCommandLine", "Tiled left margin must be used together with page units."));
        CheckKey(LONG_OPTION_TILED_PDF_RIGHT_MARGIN,
                 translate("VCommandLine", "Tiled right margin must be used together with page units."));
        CheckKey(LONG_OPTION_TILED_PDF_TOP_MARGIN,
                 translate("VCommandLine", "Tiled top margin must be used together with page units."));
        CheckKey(LONG_OPTION_TILED_PDF_BOTTOM_MARGIN,
                 translate("VCommandLine", "Tiled bottom margin must be used together with page units."));
    }

    // if present units MUST be set before any other to keep conversions correct
    if (!diag.SelectTemplate(OptPaperSize()))
    {
        qCritical() << translate("VCommandLine", "Unknown page templated selected.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }

    if (IsOptionSet(LONG_OPTION_PAGEH))
    { //at this point we already sure 3 are set or none

        if (!diag.SelectPaperUnit(OptionValue(LONG_OPTION_PAGEUNITS)))
        {
            qCritical() << translate("VCommandLine", "Unsupported paper units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        diag.SetPaperHeight (Pg2Px(OptionValue(LONG_OPTION_PAGEH), diag));
        diag.SetPaperWidth  (Pg2Px(OptionValue(LONG_OPTION_PAGEW), diag));
    }
    else
    { // Not explicit page size
        if (IsOptionSet(LONG_OPTION_LANDSCAPE_ORIENTATION))
        {
            diag.EnableLandscapeOrientation();
        }
    }

    if (IsOptionSet(LONG_OPTION_SHIFTUNITS))
    {
        if (!diag.SelectLayoutUnit(OptionValue(LONG_OPTION_SHIFTUNITS)))
        {
            qCritical() << translate("VCommandLine", "Unsupported layout units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    if (IsOptionSet(LONG_OPTION_GAPWIDTH))
    {
        diag.SetLayoutWidth(Lo2Px(OptionValue(LONG_OPTION_GAPWIDTH), diag));
    }

    diag.SetAutoCrop(IsOptionSet(LONG_OPTION_CROP));
    diag.SetUnitePages(IsOptionSet(LONG_OPTION_UNITE));
    diag.SetSaveLength(IsOptionSet(LONG_OPTION_SAVELENGTH));
    diag.SetGroup(OptGroup());

    if (IsOptionSet(LONG_OPTION_IGNORE_MARGINS))
    {
        diag.SetIgnoreAllFields(true);
    }

    QMarginsF margins = diag.GetFields();

    if (IsOptionSet(LONG_OPTION_LEFT_MARGIN))
    {
        margins.setLeft(Pg2Px(OptionValue(LONG_OPTION_LEFT_MARGIN), diag));
    }

    if (IsOptionSet(LONG_OPTION_RIGHT_MARGIN))
    {
        margins.setRight(Pg2Px(OptionValue(LONG_OPTION_RIGHT_MARGIN), diag));
    }

    if (IsOptionSet(LONG_OPTION_TOP_MARGIN))
    {
        margins.setTop(Pg2Px(OptionValue(LONG_OPTION_TOP_MARGIN), diag));
    }

    if (IsOptionSet(LONG_OPTION_BOTTOM_MARGIN))
    {
        margins.setBottom(Pg2Px(OptionValue(LONG_OPTION_BOTTOM_MARGIN), diag));
    }

    diag.SetFields(margins);
    diag.SetFollowGrainline(IsOptionSet(LONG_OPTION_FOLLOW_GRAINLINE));
    diag.SetManualPriority(IsOptionSet(LONG_OPTION_MANUAL_PRIORITY));
    diag.SetNestQuantity(IsOptionSet(LONG_OPTION_NEST_QUANTITY));
    diag.SetNestingTime(OptNestingTime());
    diag.SetEfficiencyCoefficient(OptEfficiencyCoefficient());

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
    instance->isGuiEnabled = not (instance->IsExportEnabled()
                                  || instance->IsTestModeEnabled()
                                  || instance->IsExportFMEnabled());

    return instance;
}

//------------------------------------------------------------------------------------------------------
void VCommandLine::Reset()
{
    instance.reset();
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsTestModeEnabled() const
{
    const bool r = IsOptionSet(LONG_OPTION_TEST);
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("VCommandLine", "Test option can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsPedantic() const
{
    // Pedantic doesn't work in GUI mode
    return IsGuiEnabled() ? false : IsOptionSet(LONG_OPTION_PENDANTIC);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsNoScalingEnabled() const
{
    return IsOptionSet(LONG_OPTION_NO_HDPI_SCALING);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportEnabled() const
{
    const bool r = IsOptionSet(LONG_OPTION_BASENAME);
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("VCommandLine", "Export options can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportFMEnabled() const
{
    const bool r = IsOptionSet(LONG_OPTION_CSVEXPORTFM);
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("VCommandLine", "Export options can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
VAbstractLayoutDialog::PaperSizeTemplate VCommandLine::OptPaperSize() const
{
    return FormatSize(LONG_OPTION_PAGETEMPLATE);
}

//------------------------------------------------------------------------------------------------------
Cases VCommandLine::OptGroup() const
{
    int r = OptionValue(LONG_OPTION_GROUPPING).toInt();
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
    if (IsOptionSet(LONG_OPTION_MEASUREFILE)
            && (IsExportEnabled() || IsTestModeEnabled()))
            //todo: don't want yet to allow user set measure file for general loading,
            //because need to fix multiply opened windows as well
    {
        measure = OptionValue(LONG_OPTION_MEASUREFILE);
    }

    return measure;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptBaseName() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = OptionValue(LONG_OPTION_BASENAME);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptDestinationPath() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = OptionValue(LONG_OPTION_DESTINATION);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
int VCommandLine::OptExportType() const
{
    int r = 0;
    if (IsOptionSet(LONG_OPTION_EXP2FORMAT))
    {
        r = OptionValue(LONG_OPTION_EXP2FORMAT).toInt();
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsBinaryDXF() const
{
    return IsOptionSet(LONG_OPTION_BINARYDXF);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsTextAsPaths() const
{
    return IsOptionSet(LONG_OPTION_TEXT2PATHS);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportOnlyDetails() const
{
    return IsOptionSet(LONG_OPTION_EXPORTONLYDETAILS);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsCSVWithHeader() const
{
    return IsOptionSet(LONG_OPTION_CSVWITHHEADER);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptExportSuchDetails() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = OptionValue(LONG_OPTION_EXPORTSUCHDETAILS);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptCSVCodecName() const
{
    return OptionValue(LONG_OPTION_CSVCODEC);
}

//---------------------------------------------------------------------------------------------------------------------
QChar VCommandLine::OptCSVSeparator() const
{
    const QString value = OptionValue(LONG_OPTION_CSVSEPARATOR);
    return not value.isEmpty() ? value.at(0) : QChar();
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptExportFMTo() const
{
    return OptionValue(LONG_OPTION_CSVEXPORTFM);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<int, QString> VCommandLine::OptUserMaterials() const
{
    QMap<int, QString> userMaterials;
    const QStringList values = OptionValues(LONG_OPTION_USER_MATERIAL);
    for(auto &value : values)
    {
        const QStringList parts = value.split('@');
        if (parts.size() != 2)
        {
            qCritical() << translate("VCommandLine", "Invalid user material '%1'. Separator is missing.").arg(value)
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        bool ok = false;
        const int number = parts.first().toInt(&ok);

        if (not ok or number < 1 or number > userMaterialPlaceholdersQuantity)
        {
            qCritical() << translate("VCommandLine", "Invalid user material '%1'. Wrong material number.").arg(value)
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        userMaterials.insert(number, parts.last());
    }

    return userMaterials;
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
    return IsOptionSet(LONG_OPTION_GRADATIONSIZE);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsSetGradationHeight() const
{
    return IsOptionSet(LONG_OPTION_GRADATIONHEIGHT);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationSize() const
{
    const QString size = OptionValue(LONG_OPTION_GRADATIONSIZE);
    if (VMeasurement::IsGradationSizeValid(size))
    {
        return size;
    }

    qCritical() << translate("VCommandLine", "Invalid gradation size value.") << "\n";
    const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationHeight() const
{
    const QString height = OptionValue(LONG_OPTION_GRADATIONHEIGHT);
    if (VMeasurement::IsGradationHeightValid(height))
    {
        return height;
    }

    qCritical() << translate("VCommandLine", "Invalid gradation height value.") << "\n";
    const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VCommandLine::TiledPageMargins() const
{
    QMarginsF margins(10, 10, 10, 10); // mm

    if (not IsOptionSet(LONG_OPTION_LEFT_MARGIN))
    {
        return margins;
    }

    const QString value = OptionValue(LONG_OPTION_LEFT_MARGIN);
    const QStringList supportedUnits = QStringList() << unitMM << unitCM << unitINCH;
    if (not supportedUnits.contains(value))
    {
        qCritical() << translate("VCommandLine", "Unsupported paper units.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    const Unit unit = StrToUnits(value);

    if (IsOptionSet(LONG_OPTION_LEFT_MARGIN))
    {
        margins.setLeft(UnitConvertor(OptionValue(LONG_OPTION_LEFT_MARGIN).toDouble(), unit, Unit::Mm));
    }

    if (IsOptionSet(LONG_OPTION_RIGHT_MARGIN))
    {
        margins.setRight(UnitConvertor(OptionValue(LONG_OPTION_RIGHT_MARGIN).toDouble(), unit, Unit::Mm));
    }

    if (IsOptionSet(LONG_OPTION_TOP_MARGIN))
    {
        margins.setTop(UnitConvertor(OptionValue(LONG_OPTION_TOP_MARGIN).toDouble(), unit, Unit::Mm));
    }

    if (IsOptionSet(LONG_OPTION_BOTTOM_MARGIN))
    {
        margins.setBottom(UnitConvertor(OptionValue(LONG_OPTION_BOTTOM_MARGIN).toDouble(), unit, Unit::Mm));
    }

    return margins;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractLayoutDialog::PaperSizeTemplate VCommandLine::OptTiledPaperSize() const
{
    return FormatSize(LONG_OPTION_TILED_PDF_PAGE_TEMPLATE);
}

//---------------------------------------------------------------------------------------------------------------------
PageOrientation VCommandLine::OptTiledPageOrientation() const
{
    return static_cast<PageOrientation>(not IsOptionSet(LONG_OPTION_TILED_PDF_LANDSCAPE));
}

//---------------------------------------------------------------------------------------------------------------------
void VCommandLine::InitCommandLineOptions()
{
    //keep in mind order here - that is how user will see it, so group-up for usability
    //=================================================================================================================
    parser.addOptions({
        {{SINGLE_OPTION_BASENAME, LONG_OPTION_BASENAME},
         translate("VCommandLine", "The base filename of exported layout files. Use it to enable console export mode."),
         translate("VCommandLine", "The base filename of layout files")},
        {{SINGLE_OPTION_DESTINATION, LONG_OPTION_DESTINATION},
         translate("VCommandLine", "The path to output destination folder. By default the directory at which the "
         "application was started."),
         translate("VCommandLine", "The destination folder")},
        {{SINGLE_OPTION_MEASUREFILE, LONG_OPTION_MEASUREFILE},
         translate("VCommandLine", "Path to custom measure file (export mode)."),
         translate("VCommandLine", "The measure file")},
        {{SINGLE_OPTION_NESTING_TIME, LONG_OPTION_NESTING_TIME},
         translate("VCommandLine", "<Time> in minutes given for the algorithm to find best layout. Time must be in "
         "range from 1 minute to 60 minutes. Default value 1 minute."),
         translate("VCommandLine", "Time")},
        {LONG_OPTION_EFFICIENCY_COEFFICIENT,
         translate("VCommandLine", "Set layout efficiency <coefficient>. Layout efficiency coefficient is the ratio of "
         "the area occupied by the pieces to the bounding rect of all pieces. If nesting reaches required level the "
         "process stops. If value is 0 no check will be made. Сoefficient must be in range from 0 to 100. Default "
         "value 0."),
         translate("VCommandLine", "Coefficient")},
        {{SINGLE_OPTION_EXP2FORMAT, LONG_OPTION_EXP2FORMAT},
         translate("VCommandLine", "Number corresponding to output format (default = 0, export mode):") +
         DialogSaveLayout::MakeHelpFormatList(),
         translate("VCommandLine", "Format number"), QChar('0')},
        {LONG_OPTION_BINARYDXF, translate("VCommandLine", "Export dxf in binary form.")},
        {LONG_OPTION_TEXT2PATHS, translate("VCommandLine", "Export text as paths.")},
        {LONG_OPTION_EXPORTONLYDETAILS,
         translate("VCommandLine", "Export only details. Export details as they positioned in the details mode. Any "
         "layout related options will be ignored.")},
        {LONG_OPTION_EXPORTSUCHDETAILS,
         translate("VCommandLine", "Export only details that match a piece name regex."),
         translate("VCommandLine", "The name regex")},
        {{SINGLE_OPTION_GRADATIONSIZE, LONG_OPTION_GRADATIONSIZE},
         translate("VCommandLine", "Set size value for pattern file, that was opened with multisize measurements "
         "(export mode). Valid values: %1cm.").arg(VMeasurement::WholeListSizes(Unit::Cm) .join(QStringLiteral(", "))),
         translate("VCommandLine", "The size value")},
        {{SINGLE_OPTION_GRADATIONHEIGHT, LONG_OPTION_GRADATIONHEIGHT},
         translate("VCommandLine", "Set height value for pattern file, that was opened with multisize measurements "
         "(export mode). Valid values: %1cm.").arg(VMeasurement::WholeListHeights(Unit::Cm).join(QStringLiteral(", "))),
         translate("VCommandLine", "The height value")},
        {LONG_OPTION_USER_MATERIAL,
         translate("VCommandLine", "Use this option to override user material defined in pattern. The value must be in "
         "form <number>@<user matrial name>. The number should be in range from 1 to %1. For example, 1@Fabric2. The "
         "key can be used multiple times. Has no effect in GUI mode.").arg(userMaterialPlaceholdersQuantity),
         translate("VCommandLine", "User material")},
    //=================================================================================================================
        {{SINGLE_OPTION_PAGETEMPLATE, LONG_OPTION_PAGETEMPLATE},
         translate("VCommandLine", "Number corresponding to layout page template (default = 0, export mode):") +
         DialogLayoutSettings::MakeHelpTemplateList(),
         translate("VCommandLine", "Template number"), QChar('0')},
        {LONG_OPTION_LANDSCAPE_ORIENTATION,
         translate("VCommandLine", "Switch page template orientation to landscape (export mode). This option has "
         "effect only for one of predefined page templates.")},
        {{SINGLE_OPTION_PAGEW, LONG_OPTION_PAGEW},
         translate("VCommandLine", "Page width in current units like 12.0 (cannot be used with \"%1\", export mode).")
         .arg(LONG_OPTION_PAGETEMPLATE),
         translate("VCommandLine", "The page width")},
        {{SINGLE_OPTION_PAGEH, LONG_OPTION_PAGEH},
         translate("VCommandLine", "Page height in current units like 12.0 (cannot be used with \"%1\", export mode).")
         .arg(LONG_OPTION_PAGETEMPLATE),
         translate("VCommandLine", "The page height")},
        {{SINGLE_OPTION_PAGEUNITS, LONG_OPTION_PAGEUNITS},
         translate("VCommandLine", "Page measure units (export mode). Valid values: %1.")
         .arg(VDomDocument::UnitsHelpString()),
         translate("VCommandLine", "The measure unit")},
        {{SINGLE_OPTION_IGNORE_MARGINS, LONG_OPTION_IGNORE_MARGINS},
         translate("VCommandLine", "Ignore printer margins (export mode). Use if need full paper space. In case of "
         "later printing you must account for the margins himself.")},
        {{SINGLE_OPTION_LEFT_MARGIN, LONG_OPTION_LEFT_MARGIN},
         translate("VCommandLine", "Page left margin in current units like 3.0 (export mode). If not set will be used "
         "value from default printer. Or 0 if none printers was found."),
         translate("VCommandLine", "The left margin")},
        {{SINGLE_OPTION_RIGHT_MARGIN, LONG_OPTION_RIGHT_MARGIN},
         translate("VCommandLine", "Page right margin in current units like 3.0 (export mode). If not set will be used "
         "value from default printer. Or 0 if none printers was found."),
         translate("VCommandLine", "The right margin")},
        {{SINGLE_OPTION_TOP_MARGIN, LONG_OPTION_TOP_MARGIN},
         translate("VCommandLine", "Page top margin in current units like 3.0 (export mode). If not set will be used "
         "value from default printer. Or 0 if none printers was found."),
         translate("VCommandLine", "The top margin")},
        {{SINGLE_OPTION_BOTTOM_MARGIN, LONG_OPTION_BOTTOM_MARGIN},
         translate("VCommandLine", "Page bottom margin in current units like 3.0 (export mode). If not set will be "
         "used value from default printer. Or 0 if none printers was found."),
         translate("VCommandLine", "The bottom margin")},
    //=================================================================================================================
        {LONG_OPTION_FOLLOW_GRAINLINE,
         translate("VCommandLine", "Order detail to follow grainline direction (export mode).")},
        {LONG_OPTION_MANUAL_PRIORITY,
         translate("VCommandLine", "Follow manual priority over priority by square (export mode).")},
        {LONG_OPTION_NEST_QUANTITY,
         translate("VCommandLine", "Nest quantity copies of each piece (export mode).")},
        {{SINGLE_OPTION_CROP, LONG_OPTION_CROP},
         translate("VCommandLine", "Auto crop unused length (export mode).")},
        {{SINGLE_OPTION_UNITE, LONG_OPTION_UNITE},
         translate("VCommandLine", "Unite pages if possible (export mode). Maximum value limited by QImage that "
         "supports only a maximum of 32768x32768 px images.")},
    //=================================================================================================================
        {{SINGLE_OPTION_SAVELENGTH, LONG_OPTION_SAVELENGTH},
         translate("VCommandLine", "Save length of the sheet if set (export mode). The option tells the program to use "
         "as much as possible width of sheet. Quality of a layout can be worse when this option was used.")},
        {{SINGLE_OPTION_SHIFTUNITS, LONG_OPTION_SHIFTUNITS},
         translate("VCommandLine", "Layout units (as paper's one except px, export mode)."),
         translate("VCommandLine", "The unit")},
        {{SINGLE_OPTION_GAPWIDTH, LONG_OPTION_GAPWIDTH},
         translate("VCommandLine", "The layout gap width x2, measured in layout units (export mode). Set distance "
         "between details and a detail and a sheet."),
         translate("VCommandLine", "The gap width")},
        {{SINGLE_OPTION_GROUPPING, LONG_OPTION_GROUPPING},
         translate("VCommandLine", "Sets layout groupping cases (export mode): %1.")
         .arg(DialogLayoutSettings::MakeGroupsHelp()),
         translate("VCommandLine", "Grouping type"), QChar('2')},
        {{SINGLE_OPTION_TEST, LONG_OPTION_TEST},
         translate("VCommandLine", "Run the program in a test mode. The program in this mode loads a single pattern "
         "file and silently quit without showing the main window. The key have priority before key '%1'.")
         .arg(LONG_OPTION_BASENAME)},
        {LONG_OPTION_PENDANTIC,
         translate("VCommandLine", "Make all parsing warnings into errors. Have effect only in console mode. Use to "
         "force Valentina to immediately terminate if a pattern contains a parsing warning.")},
        {LONG_OPTION_NO_HDPI_SCALING,
         translate("VCommandLine", "Disable high dpi scaling. Call this option if has problem with scaling (by default "
         "scaling enabled). Alternatively you can use the %1 environment variable.")
         .arg(QStringLiteral("QT_AUTO_SCREEN_SCALE_FACTOR=0"))},
    //=================================================================================================================
        {LONG_OPTION_CSVWITHHEADER,
         translate("VCommandLine", "Export to csv with header. By default disabled.")},
        {LONG_OPTION_CSVCODEC,
         translate("VCommandLine", "Specify codec that will be used to save data. List of supported codecs provided by "
         "Qt. Default value depend from system. On Windows, the codec will be based on a system locale. On Unix "
         "systems, the codec will might fall back to using the iconv library if no builtin codec for the locale can be "
         "found. Valid values for this installation:") + DialogExportToCSV::MakeHelpCodecsList(),
         translate("VCommandLine", "Codec name"), QString(QTextCodec::codecForLocale()->name())},
        {LONG_OPTION_CSVSEPARATOR,
         translate("VCommandLine", "Specify csv separator character. Default value is '%1'. Valid characters:")
         .arg(VCommonSettings::GetDefCSVSeparator()) + DialogExportToCSV::MakeHelpSeparatorList(),
         translate("VCommandLine", "Separator character"), QString(VCommonSettings::GetDefCSVSeparator())},
        {LONG_OPTION_CSVEXPORTFM,
         translate("VCommandLine", "Calling this command enable exporting final measurements. Specify path to csv file "
         "with final measurements. The path must contain path to directory and name of file. It can be absolute or "
         "relatetive. In case of relative path will be used current working directory to calc a destination path."),
         translate("VCommandLine", "Path to csv file")},
    //=================================================================================================================
        {LONG_OPTION_TILED_PDF_PAGE_TEMPLATE,
         translate("VCommandLine", "Number corresponding to tiled pdf page template (default = 0, export mode with "
         "tiled pdf format):") + DialogLayoutSettings::MakeHelpTiledPdfTemplateList(),
         translate("VCommandLine", "Template number"), QChar('0')},
        {LONG_OPTION_TILED_PDF_LEFT_MARGIN,
         translate("VCommandLine","Tiled page left margin in current units like 3.0 (export mode). If not set will be "
         "used default value 1 cm."),
         translate("VCommandLine", "The left margin")},
        {LONG_OPTION_TILED_PDF_RIGHT_MARGIN,
         translate("VCommandLine", "Tiled page right margin in current units like 3.0 (export mode). If not set will "
         "be used default value 1 cm."),
         translate("VCommandLine", "The right margin")},
        {LONG_OPTION_TILED_PDF_TOP_MARGIN,
         translate("VCommandLine", "Tiled page top margin in current units like 3.0 (export mode). If not set will be "
         "used value default value 1 cm."),
         translate("VCommandLine", "The top margin")},
        {LONG_OPTION_TILED_PDF_BOTTOM_MARGIN,
         translate("VCommandLine", "Tiled page bottom margin in current units like 3.0 (export mode). If not set will "
         "be used value default value 1 cm."),
         translate("VCommandLine", "The bottom margin")},
        {LONG_OPTION_TILED_PDF_LANDSCAPE,
         translate("VCommandLine", "Set tiled page orienatation to landscape (export mode). Default value if not set "
         "portrait.")}
    });
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsOptionSet(const QString &option) const
{
    return parser.isSet(option);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptionValue(const QString &option) const
{
    return parser.value(option);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommandLine::OptionValues(const QString &option) const
{
    return parser.values(option);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommandLine::OptNestingTime() const
{
    int time = VSettings::GetDefNestingTime();
    if (IsOptionSet(LONG_OPTION_NESTING_TIME))
    {
        bool ok = false;
        time = OptionValue(LONG_OPTION_NESTING_TIME).toInt(&ok);

        if (not ok || time < 1 || time > 60)
        {
            qCritical() << translate("VCommandLine", "Time must be in range from 1 minute to 60 minutes.")
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    return time;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCommandLine::OptEfficiencyCoefficient() const
{
    qreal coefficient = VSettings::GetDefEfficiencyCoefficient();
    if (IsOptionSet(LONG_OPTION_EFFICIENCY_COEFFICIENT))
    {
        bool ok = false;
        coefficient = OptionValue(LONG_OPTION_EFFICIENCY_COEFFICIENT).toDouble(&ok);

        if (not ok || coefficient < 0 || coefficient > 100)
        {
            qCritical() << translate("VCommandLine", "Сoefficient must be in range from 0 to 100.")
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    return coefficient;
}

#undef translate
