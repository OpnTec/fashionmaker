/************************************************************************
 **
 **  @file   vcmdexport.cpp
 **  @author Alex Zaharov <alexzkhr@gmail.com>
 **  @date   25 8, 2015
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

#include "vcmdexport.h"
#include "../dialogs/dialoglayoutsettings.h"
#include "../dialogs/dialogsavelayout.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vformat/vmeasurements.h"
#include "../vmisc/commandoptions.h"
#include "../vmisc/vsettings.h"
#include <QDebug>

VCommandLinePtr VCommandLine::instance = nullptr;

#define translate(context, source) QCoreApplication::translate((context), (source))

//---------------------------------------------------------------------------------------------------------------------
VCommandLine::VCommandLine() : parser(), optionsUsed(), optionsIndex(), isGuiEnabled(false)
{
    parser.setApplicationDescription(translate("VCommandLine", "Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", translate("VCommandLine", "Pattern file."));

    InitOptions(optionsUsed, optionsIndex);

    VCommandLineOptions::const_iterator i = optionsUsed.constBegin();
    while (i != optionsUsed.constEnd())
    {
        parser.addOption(*((*i)));
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCommandLine::Lo2Px(const QString &src, const DialogLayoutSettings &converter)
{
    return converter.LayoutToPixels(src.toDouble());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCommandLine::Pg2Px(const QString& src, const DialogLayoutSettings& converter)
{
    return converter.PageToPixels(src.toDouble());
}

//---------------------------------------------------------------------------------------------------------------------
void VCommandLine::InitOptions(VCommandLineOptions &options, QMap<QString, int> &optionsIndex)
{
    int index = 0;
    //keep in mind order here - that is how user will see it, so group-up for usability
    //=================================================================================================================
    optionsIndex.insert(LONG_OPTION_BASENAME, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_BASENAME << LONG_OPTION_BASENAME,
                                          translate("VCommandLine", "The base filename of exported layout files. Use "
                                                                    "it to enable console export mode."),
                                          translate("VCommandLine", "The base filename of layout files")));

    optionsIndex.insert(LONG_OPTION_DESTINATION, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_DESTINATION << LONG_OPTION_DESTINATION,
                                          translate("VCommandLine", "The path to output destination folder. By "
                                                    "default the directory at which the application was started."),
                                          translate("VCommandLine", "The destination folder")));

    optionsIndex.insert(LONG_OPTION_MEASUREFILE, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_MEASUREFILE << LONG_OPTION_MEASUREFILE,
                                          translate("VCommandLine", "Path to custom measure file (export mode)."),
                                          translate("VCommandLine", "The measure file")));

    optionsIndex.insert(LONG_OPTION_EXP2FORMAT, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_EXP2FORMAT << LONG_OPTION_EXP2FORMAT,
                                          translate("VCommandLine", "Number corresponding to output format (default = "
                                                                    "0, export mode):") +
                                                                    DialogSaveLayout::MakeHelpFormatList(),
                                          translate("VCommandLine", "Format number"), "0"));

    optionsIndex.insert(LONG_OPTION_GRADATIONSIZE, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_GRADATIONSIZE << LONG_OPTION_GRADATIONSIZE,
                                          translate("VCommandLine", "Set size value a pattern file, that was opened "
                                                                    "with standard measurements (export mode). Valid "
                                                                    "values: %1cm.")
                                                                .arg(VMeasurement::WholeListSizes(Unit::Cm).join(", ")),
                                          translate("VCommandLine", "The size value")));

    optionsIndex.insert(LONG_OPTION_GRADATIONHEIGHT, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_GRADATIONHEIGHT << LONG_OPTION_GRADATIONHEIGHT,
                                          translate("VCommandLine", "Set height value a pattern file, that was opened "
                                                                    "with standard measurements (export mode). Valid "
                                                                    "values: %1cm.")
                                                              .arg(VMeasurement::WholeListHeights(Unit::Cm).join(", ")),
                                          translate("VCommandLine", "The height value")));

    //=================================================================================================================
    optionsIndex.insert(LONG_OPTION_PAGETEMPLATE, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGETEMPLATE << LONG_OPTION_PAGETEMPLATE,
                                          translate("VCommandLine", "Number corresponding to page template (default = "
                                                                    "0, export mode):") +
                                                                    DialogLayoutSettings::MakeHelpTemplateList(),
                                          translate("VCommandLine", "Template number"), "0"));

    optionsIndex.insert(LONG_OPTION_PAGEW, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEW << LONG_OPTION_PAGEW,
                                          translate("VCommandLine", "Page width in current units like 12.0 (cannot be "
                                                                    "used with \"%1\", export mode).")
                                                                    .arg(LONG_OPTION_PAGETEMPLATE),
                                          translate("VCommandLine", "The page width")));

    optionsIndex.insert(LONG_OPTION_PAGEH, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEH << LONG_OPTION_PAGEH,
                                          translate("VCommandLine", "Page height in current units like 12.0 (cannot "
                                                                    "be used with \"%1\", export mode).")
                                                                    .arg(LONG_OPTION_PAGETEMPLATE),
                                          ("The page height")));

    optionsIndex.insert(LONG_OPTION_PAGEUNITS, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_PAGEUNITS << LONG_OPTION_PAGEUNITS,
                                          translate("VCommandLine", "Page height/width measure units (cannot be used "
                                                    "with \"%1\", export mode). Valid values: %2.")
                                                    .arg(LONG_OPTION_PAGETEMPLATE).arg(VDomDocument::UnitsHelpString()),
                                          translate("VCommandLine", "The measure unit")));

    optionsIndex.insert(LONG_OPTION_IGNORE_MARGINS, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_IGNORE_MARGINS << LONG_OPTION_IGNORE_MARGINS,
                                          translate("VCommandLine",
                                                    "Ignore margins printing (export mode). Disable value keys: "
                                                    "\"%1\", \"%2\", \"%3\", \"%4\". Set all margins to 0.")
                                          .arg(LONG_OPTION_LEFT_MARGIN).arg(LONG_OPTION_RIGHT_MARGIN)
                                          .arg(LONG_OPTION_TOP_MARGIN).arg(LONG_OPTION_BOTTOM_MARGIN)));

    optionsIndex.insert(LONG_OPTION_LEFT_MARGIN, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_LEFT_MARGIN << LONG_OPTION_LEFT_MARGIN,
                                          translate("VCommandLine",
                                                    "Page left margin in current units like 3.0 (export mode). If "
                                                    "not set will be used value from default printer. Or 0 if none "
                                                    "printers was found. Value will be ignored if key \"%1\" is used.")
                                          .arg(LONG_OPTION_IGNORE_MARGINS),
                                          ("The left margin")));

    optionsIndex.insert(LONG_OPTION_RIGHT_MARGIN, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_RIGHT_MARGIN << LONG_OPTION_RIGHT_MARGIN,
                                          translate("VCommandLine",
                                                    "Page right margin in current units like 3.0 (export mode). If "
                                                    "not set will be used value from default printer. Or 0 if none "
                                                    "printers was found. Value will be ignored if key \"%1\" is used.")
                                          .arg(LONG_OPTION_IGNORE_MARGINS),
                                          ("The right margin")));

    optionsIndex.insert(LONG_OPTION_TOP_MARGIN, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_TOP_MARGIN << LONG_OPTION_TOP_MARGIN,
                                          translate("VCommandLine",
                                                    "Page top margin in current units like 3.0 (export mode). If "
                                                    "not set will be used value from default printer. Or 0 if none "
                                                    "printers was found. Value will be ignored if key \"%1\" is used.")
                                          .arg(LONG_OPTION_IGNORE_MARGINS),
                                          ("The top margin")));

    optionsIndex.insert(LONG_OPTION_BOTTOM_MARGIN, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_BOTTOM_MARGIN << LONG_OPTION_BOTTOM_MARGIN,
                                          translate("VCommandLine",
                                                    "Page bottom margin in current units like 3.0 (export mode). If "
                                                    "not set will be used value from default printer. Or 0 if none "
                                                    "printers was found. Value will be ignored if key \"%1\" is used.")
                                          .arg(LONG_OPTION_IGNORE_MARGINS),
                                          ("The bottom margin")));

    //=================================================================================================================
    optionsIndex.insert(LONG_OPTION_ROTATE, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_ROTATE << LONG_OPTION_ROTATE,
                                          translate("VCommandLine", "Rotation in degrees (one of predefined, "
                                                    "export mode). Default value is 180. 0 is no-rotate. Valid values: "
                                                    "%1. Each value show how many times details will be rotated. For "
                                                    "example 180 mean two times (360/180=2) by 180 degree.")
                                          .arg("0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 30, 36, 40, 45, "
                                               "60, 72, 90, 180"),
                                          translate("VCommandLine", "Angle")));

    optionsIndex.insert(LONG_OPTION_CROP, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_CROP << LONG_OPTION_CROP,
                                          translate("VCommandLine", "Auto crop unused length (export mode).")));

    optionsIndex.insert(LONG_OPTION_UNITE, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_UNITE << LONG_OPTION_UNITE,
                                          translate("VCommandLine", "Unite pages if possible (export mode). Maximum "
                                                    "value limited by QImage that supports only a maximum of "
                                                    "32768x32768 px images.")));

    //=================================================================================================================
    optionsIndex.insert(LONG_OPTION_SAVELENGTH, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_SAVELENGTH << LONG_OPTION_SAVELENGTH,
                                          translate("VCommandLine",
                                                    "Save length of the sheet if set (export mode). The option tells "
                                                    "the program to use as much as possible width of sheet. Quality "
                                                    "of a layout can be worse when this option was used.")));

    optionsIndex.insert(LONG_OPTION_SHIFTUNITS, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_SHIFTUNITS << LONG_OPTION_SHIFTUNITS,
                                          translate("VCommandLine",
                                                    "Layout units (as paper's one except px, export mode)."),
                                          translate("VCommandLine", "The unit")));

    optionsIndex.insert(LONG_OPTION_SHIFTLENGTH, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_SHIFTLENGTH << LONG_OPTION_SHIFTLENGTH,
                                          translate("VCommandLine", "Shift/Offset layout length measured in layout "
                                                    "units (export mode). The option show how many points along edge "
                                                    "will be used in creating a layout."),
                                          translate("VCommandLine", "Shift/Offset length")));

    optionsIndex.insert(LONG_OPTION_GAPWIDTH, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_GAPWIDTH << LONG_OPTION_GAPWIDTH,
                                          translate("VCommandLine",
                                                    "The layout gap width x2, measured in layout units (export mode). "
                                                    "Set distance between details and a detail and a sheet."),
                                          translate("VCommandLine", "The gap width")));

    optionsIndex.insert(LONG_OPTION_GROUPPING, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_GROUPPING << LONG_OPTION_GROUPPING,
                                          translate("VCommandLine", "Sets layout groupping cases (export mode): %1.")
                                          .arg(DialogLayoutSettings::MakeGroupsHelp()),
                                          translate("VCommandLine", "Grouping type"), "2"));

    optionsIndex.insert(LONG_OPTION_TEST, index++);
    options.append(new QCommandLineOption(QStringList() << SINGLE_OPTION_TEST << LONG_OPTION_TEST,
                                          translate("VCommandLine", "Run the program in a test mode. The program in "
                                                    "this mode loads a single pattern file and silently quit without "
                                                    "showing the main window. The key have priority before key '%1'.")
                                                    .arg(LONG_OPTION_BASENAME)));
}

//------------------------------------------------------------------------------------------------------
VLayoutGeneratorPtr VCommandLine::DefaultGenerator() const
{
    //this functions covers all options found into layout setup dialog, nothing to add here, unless dialog extended

    VLayoutGeneratorPtr res(new VLayoutGenerator());
    DialogLayoutSettings diag(res.get(), nullptr, true);

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool x = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGETEMPLATE)));

        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEH)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEW)));
        bool c = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)));

        if ((a || b || c) && x)
        {
            qCritical() << translate("VCommandLine", "Cannot use pageformat and page explicit size/units together.")
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
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTLENGTH)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Shift/Offset length must be used together with shift units.")
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GAPWIDTH)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Gap width must be used together with shift units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_LEFT_MARGIN)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Left margin must be used together with page units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_RIGHT_MARGIN)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Right margin must be used together with page units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_TOP_MARGIN)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Top margin must be used together with page units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    {
        //just anonymous namespace ...don' like to have a,b,c,d everywhere defined
        bool a = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_BOTTOM_MARGIN)));
        bool b = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)));

        if ((a || b) && !(a && b))
        {
            qCritical() << translate("VCommandLine", "Bottom margin must be used together with page units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    const int rotateDegree = OptRotation();
    diag.SetRotate(rotateDegree != 0 ); // 0 disable rotation

    if (rotateDegree != VSettings::GetDefLayoutRotationIncrease())//Value by default
    {
        if (not diag.SetIncrease(rotateDegree))
        {
            qCritical() << translate("VCommandLine", "Invalid rotation value. That must be one of predefined values.")
                        << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    // if present units MUST be set before any other to keep conversions correct
    if (!diag.SelectTemplate(OptPaperSize()))
    {
        qCritical() << translate("VCommandLine", "Unknown page templated selected.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }

    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEH))))
    { //at this point we already sure 3 are set or none

        if (!diag.SelectPaperUnit(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEUNITS)))))
        {
            qCritical() << translate("VCommandLine", "Unsupported paper units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }

        diag.SetPaperHeight (Pg2Px(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEH))), diag));
        diag.SetPaperWidth  (Pg2Px(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGEW))), diag));
    }

    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTUNITS))))
    {
        if (!diag.SelectLayoutUnit(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTUNITS)))))
        {
            qCritical() << translate("VCommandLine", "Unsupported layout units.") << "\n";
            const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
        }
    }

    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTLENGTH))))
    {

        diag.SetShift(Lo2Px(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SHIFTLENGTH))), diag));
    }

    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GAPWIDTH))))
    {
        diag.SetLayoutWidth(Lo2Px(parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GAPWIDTH))), diag));
    }

    diag.SetAutoCrop(parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_CROP))));
    diag.SetUnitePages(parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_UNITE))));
    diag.SetSaveLength(parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_SAVELENGTH))));
    diag.SetGroup(OptGroup());

    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_IGNORE_MARGINS))))
    {
        diag.SetIgnoreAllFields(true);
    }
    else
    {
        QMarginsF margins = diag.GetFields();

        {
            const QCommandLineOption *option = optionsUsed.value(optionsIndex.value(LONG_OPTION_LEFT_MARGIN));
            if (parser.isSet(*option))
            {
                margins.setLeft(Pg2Px(parser.value(*option), diag));
            }
        }

        {
            const QCommandLineOption *option = optionsUsed.value(optionsIndex.value(LONG_OPTION_RIGHT_MARGIN));
            if (parser.isSet(*option))
            {
                margins.setRight(Pg2Px(parser.value(*option), diag));
            }
        }

        {
            const QCommandLineOption *option = optionsUsed.value(optionsIndex.value(LONG_OPTION_TOP_MARGIN));
            if (parser.isSet(*option))
            {
                margins.setTop(Pg2Px(parser.value(*option), diag));
            }
        }

        {
            const QCommandLineOption *option = optionsUsed.value(optionsIndex.value(LONG_OPTION_BOTTOM_MARGIN));
            if (parser.isSet(*option))
            {
                margins.setBottom(Pg2Px(parser.value(*option), diag));
            }
        }
        diag.SetFields(margins);
    }

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
    const bool r = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_TEST)));
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("VCommandLine", "Test option can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
bool VCommandLine::IsExportEnabled() const
{
    const bool r = parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_BASENAME)));
    if (r && parser.positionalArguments().size() != 1)
    {
        qCritical() << translate("VCommandLine", "Export options can be used with single input file only.") << "/n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
    return r;
}

//------------------------------------------------------------------------------------------------------
DialogLayoutSettings::PaperSizeTemplate VCommandLine::OptPaperSize() const
{
    int ppsize = 0;
    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGETEMPLATE))))
    {
        ppsize = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_PAGETEMPLATE))).toInt();
    }
    return static_cast<DialogLayoutSettings::PaperSizeTemplate>(ppsize);
}

//------------------------------------------------------------------------------------------------------
int VCommandLine::OptRotation() const
{
    int rotate = VSettings::GetDefLayoutRotationIncrease();
    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_ROTATE))))
    {
        bool ok = false;
        rotate = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_ROTATE))).toInt(&ok);

        if (not ok)
        {
            rotate = VSettings::GetDefLayoutRotationIncrease();
        }
    }

    return rotate;
}

//------------------------------------------------------------------------------------------------------
Cases VCommandLine::OptGroup() const
{
    int r = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GROUPPING))).toInt();
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
    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_MEASUREFILE)))
            && (IsExportEnabled() || IsTestModeEnabled()))
            //todo: don't want yet to allow user set measure file for general loading,
            //because need to fix multiply opened windows as well
    {
        measure = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_MEASUREFILE)));
    }

    return measure;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptBaseName() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_BASENAME)));
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptDestinationPath() const
{
    QString path;
    if (IsExportEnabled())
    {
        path = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_DESTINATION)));
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
int VCommandLine::OptExportType() const
{
    int r = 0;
    if (parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_EXP2FORMAT))))
    {
        r = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_EXP2FORMAT))).toInt();
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
    return parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GRADATIONSIZE)));
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommandLine::IsSetGradationHeight() const
{
    return parser.isSet(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GRADATIONHEIGHT)));
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationSize() const
{
    const QString size = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GRADATIONSIZE)));
    if (VMeasurement::IsGradationSizeValid(size))
    {
        return size;
    }
    else
    {
        qCritical() << translate("VCommandLine", "Invalid gradation size value.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommandLine::OptGradationHeight() const
{
    const QString height = parser.value(*optionsUsed.value(optionsIndex.value(LONG_OPTION_GRADATIONHEIGHT)));
    if (VMeasurement::IsGradationHeightValid(height))
    {
        return height;
    }
    else
    {
        qCritical() << translate("VCommandLine", "Invalid gradation height value.") << "\n";
        const_cast<VCommandLine*>(this)->parser.showHelp(V_EX_USAGE);
    }
}

#undef translate
