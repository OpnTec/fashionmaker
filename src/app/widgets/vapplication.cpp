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
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionbadid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionwrongid.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QSettings>

#include <container/calculator.h>
#include <QtMath>

const qreal VApplication::PrintDPI = 96.0;

#define DefWidth 1.2//mm

//---------------------------------------------------------------------------------------------------------------------
VApplication::VApplication(int &argc, char **argv)
    : QApplication(argc, argv), _patternUnit(Valentina::Cm), _patternType(Pattern::Individual),
      _widthMainLine(DefWidth), _widthHairLine(DefWidth/3.0), measurements(QMap<QString, VTranslation>()),
      guiTexts(QMap<QString, VTranslation>()), descriptions(QMap<QString, VTranslation>()),
      variables(QMap<QString, VTranslation>()), functions(QMap<QString, VTranslation>()),
      postfixOperators(QMap<QString, VTranslation>()), undoStack(nullptr)
{
    undoStack = new QUndoStack(this);

    InitLineWidth();
    InitMeasurements();
    InitVariables();
    InitFunctions();
    InitPostfixOperators();
}

//---------------------------------------------------------------------------------------------------------------------
// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file. Program will be terminated."));
        abort();
    }
    catch (const VExceptionBadId &e)
    {
        e.CriticalMessageBox(tr("Error bad id. Program will be terminated."));
        abort();
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value. Program will be terminated."));
        abort();
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter. Program will be terminated."));
        abort();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id. Program will be terminated."));
        abort();
    }
    catch (const VException &e)
    {
        e.CriticalMessageBox(tr("Something's wrong!!"));
        return true;
    }
    catch (std::exception& e)
    {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::toPixel(double unit) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (unit / 25.4) * PrintDPI;
        break;
    case Valentina::Cm:
        result = ((unit * 10.0) / 25.4) * PrintDPI;
        break;
    case Valentina::Inch:
        result = unit * PrintDPI;
        break;
    default:
        break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::fromPixel(double pix) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (pix / PrintDPI) * 25.4;
        break;
    case Valentina::Cm:
        result = ((pix / PrintDPI) * 25.4) / 10.0;
        break;
    case Valentina::Inch:
        result = pix / PrintDPI;
        break;
    default:
        break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::pathToTables() const
{
    if (_patternType == Pattern::Individual)
    {
        return QStringLiteral("://tables/individual/individual.vit");
    }
    else
    {
        #ifdef Q_OS_WIN
            return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
        #else
            #ifdef QT_DEBUG
                return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
            #else
                QDir dir(QApplication::applicationDirPath() + QStringLiteral("/tables/standard"));
                if(dir.exist())
                {
                    return dir.absolutePath();
                }
                else
                {
                    return QStringLiteral("/usr/share/valentina/tables/standard");
                }
            #endif
        #endif
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::translationsPath() const
{
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
#else
#ifdef QT_DEBUG
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
    #else
        QDir dir(QApplication::applicationDirPath() + QStringLiteral("/translations"));
        if(dir.exist())
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
        case Valentina::Mm:
            _widthMainLine = DefWidth;
            break;
        case Valentina::Cm:
            _widthMainLine = DefWidth/10.0;
            break;
        case Valentina::Inch:
            _widthMainLine = DefWidth/25.4;
            break;
        default:
            _widthMainLine = DefWidth;
            break;
    }
    _widthHairLine = _widthMainLine/3.0;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitMeasurements()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    VTranslation m;
    VTranslation g;
    VTranslation d;

    //=================================================================================================================
    // head_and_neck
    m = VTranslation::translate("Measurements", "head_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Head girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around fullest part of Head", "Full measurement description");

    measurements.insert(headGirth_M, m);
    guiTexts.insert(headGirth_M, g);
    descriptions.insert(headGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "mid_neck_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Mid-neck girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around middle part of Neck", "Full measurement description");

    measurements.insert(midNeckGirth_M, m);
    guiTexts.insert(midNeckGirth_M, g);
    descriptions.insert(midNeckGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_base_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck Base girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Neck at base", "Full measurement description");

    measurements.insert(neckBaseGirth_M, m);
    guiTexts.insert(neckBaseGirth_M, g);
    descriptions.insert(neckBaseGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "head_and_neck_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Head and Neck length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Vertical Distance from Crown to Nape", "Full measurement description");

    measurements.insert(headAndNeckLength_M, m);
    guiTexts.insert(headAndNeckLength_M, g);
    descriptions.insert(headAndNeckLength_M, d);
    //=================================================================================================================
    // torso
    m = VTranslation::translate("Measurements", "center_front_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Center length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front Neck Center over tape at Bustline to Front Waist Center",
                                "Full measurement description");

    measurements.insert(centerFrontWaistLength_M, m);
    guiTexts.insert(centerFrontWaistLength_M, g);
    descriptions.insert(centerFrontWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "center_back_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Center length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back Neck Center to Back Waist Center",
                                "Full measurement description");

    measurements.insert(centerBackWaistLength_M, m);
    guiTexts.insert(centerBackWaistLength_M, g);
    descriptions.insert(centerBackWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder length", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to ShoulderTip", "Full measurement description");

    measurements.insert(shoulderLength_M, m);
    guiTexts.insert(shoulderLength_M, g);
    descriptions.insert(shoulderLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "side_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Side Waist length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Armpit to Waist side", "Full measurement description");

    measurements.insert(sideWaistLength_M, m);
    guiTexts.insert(sideWaistLength_M, g);
    descriptions.insert(sideWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "trunk_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Trunk length", "Full measurement name");
    d = VTranslation::translate("Measurements",
                                "Around Body from middle of Shoulder length to BustPoint to Crotch up back to beginning"
                                " point",
                                "Full measurement description");

    measurements.insert(trunkLength_M, m);
    guiTexts.insert(trunkLength_M, g);
    descriptions.insert(trunkLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Arms and Torso, at bicep level parallel to floor, with arms"
                                " hanging at the sides", "Full measurement description");

    measurements.insert(shoulderGirth_M, m);
    guiTexts.insert(shoulderGirth_M, g);
    descriptions.insert(shoulderGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "upper_chest_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Upper Chest girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Chest at Armfold level, will be parallel to floor across back,"
                                " will not be parallel to floor across front chest", "Full measurement description");

    measurements.insert(upperChestGirth_M, m);
    guiTexts.insert(upperChestGirth_M, g);
    descriptions.insert(upperChestGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "bust_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Bust girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around fullest part of Bust, parallel to floor",
                                "Full measurement description");

    measurements.insert(bustGirth_M, m);
    guiTexts.insert(bustGirth_M, g);
    descriptions.insert(bustGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "under_bust_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Under Bust girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Chest below the Bust, parallel to floor",
                                "Full measurement description");

    measurements.insert(underBustGirth_M, m);
    guiTexts.insert(underBustGirth_M, g);
    descriptions.insert(underBustGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Tie a string around smallest part of waist, keep string tied while"
                                " taking meaasurements. Not usually parallel to floor for front waist or back waist.",
                                "Full measurement description");

    measurements.insert(waistGirth_M, m);
    guiTexts.insert(waistGirth_M, g);
    descriptions.insert(waistGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "high_hip_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "HighHip girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around HighHip, parallel to floor", "Full measurement description");

    measurements.insert(highHipGirth_M, m);
    guiTexts.insert(highHipGirth_M, g);
    descriptions.insert(highHipGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hip_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hip girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Hip, parallel to floor", "Full measurement description");

    measurements.insert(hipGirth_M, m);
    guiTexts.insert(hipGirth_M, g);
    descriptions.insert(hipGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "upper_front_chest_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Upper Chest width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Across Front UpperChest, smallest width from armscye to armscye",
                                "Full measurement description");

    measurements.insert(upperFrontChestWidth_M, m);
    guiTexts.insert(upperFrontChestWidth_M, g);
    descriptions.insert(upperFrontChestWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_chest_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Chest width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Across Front Chest, from armfold to armfold",
                                "Full measurement description");

    measurements.insert(frontChestWidth_M, m);
    guiTexts.insert(frontChestWidth_M, g);
    descriptions.insert(frontChestWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "across_front_shoulder_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Across Shoulder width", "Full measurement name");
    d = VTranslation::translate("Measurements", "From ShoulderTip to ShoulderTip, across Front",
                                "Full measurement description");

    measurements.insert(acrossFrontShoulderWidth_M, m);
    guiTexts.insert(acrossFrontShoulderWidth_M, g);
    descriptions.insert(acrossFrontShoulderWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "across_back_shoulder_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Across Shoulder width", "Full measurement name");
    d = VTranslation::translate("Measurements", "From ShoulderTip to ShoulderTip, across Back",
                                "Full measurement description");

    measurements.insert(acrossBackShoulderWidth_M, m);
    guiTexts.insert(acrossBackShoulderWidth_M,g );
    descriptions.insert(acrossBackShoulderWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "upper_back_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Upper Chest width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Across Back UpperChest, smallest width from armscye to armscye",
                                "Full measurement description");

    measurements.insert(upperBackWidth_M, m);
    guiTexts.insert(upperBackWidth_M, g);
    descriptions.insert(upperBackWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Chest width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Across Back Chest, from armfold to armfold",
                                "Full measurement description");

    measurements.insert(backWidth_M, m);
    guiTexts.insert(backWidth_M, g);
    descriptions.insert(backWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "bustpoint_to_bustpoint",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "BustPoint to BustPoint", "Full measurement name");
    d = VTranslation::translate("Measurements", "Distance between BustPoints, across Chest",
                                "Full measurement description");

    measurements.insert(bustpointToBustpoint_M, m);
    guiTexts.insert(bustpointToBustpoint_M, g);
    descriptions.insert(bustpointToBustpoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "halter_bustpoint_to_bustpoint",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Halter Bustpoint to Bustpoint", "Full measurement name");
    d = VTranslation::translate("Measurements", "Distance from Bustpoint, behind neck, down to Bustpoint",
                                "Full measurement description");

    measurements.insert(halterBustpointToBustpoint_M, m);
    guiTexts.insert(halterBustpointToBustpoint_M, g);
    descriptions.insert(halterBustpointToBustpoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_bustpoint",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "NeckPoint to BustPoint", "Full measurement name");
    d = VTranslation::translate("Measurements", "From NeckPoint to BustPoint", "Full measurement description");

    measurements.insert(neckToBustpoint_M, m);
    guiTexts.insert(neckToBustpoint_M, g);
    descriptions.insert(neckToBustpoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "crotch_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Crotch length", "Full measurement name");
    d = VTranslation::translate("Measurements", "From Front Waist Center, down to crotch, up to Back Waist Center",
                                "Full measurement description");

    measurements.insert(crotchLength_M, m);
    guiTexts.insert(crotchLength_M, g);
    descriptions.insert(crotchLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "rise_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Rise height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Sit on hard chair, measure from side waist straight down to chair"
                                " bottom", "Full measurement description");

    measurements.insert(riseHeight_M, m);
    guiTexts.insert(riseHeight_M, g);
    descriptions.insert(riseHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_drop",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder Drop", "Full measurement name");
    d = VTranslation::translate("Measurements", "Vertical Distance from NeckPoint level to ShoulderTip level",
                                "Full measurement description");

    measurements.insert(shoulderDrop_M, m);
    guiTexts.insert(shoulderDrop_M, g);
    descriptions.insert(shoulderDrop_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_slope_degrees",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder Slope degrees", "Full measurement name");
    d = VTranslation::translate("Measurements", "Degrees of angle from NeckPoint to ShoulderTip – requires goniometer",
                                "Full measurement description");

    measurements.insert(shoulderSlopeDegrees_M, m);
    guiTexts.insert(shoulderSlopeDegrees_M, g);
    descriptions.insert(shoulderSlopeDegrees_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_shoulder_slope_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Shoulder Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "ShoulderTip to Front Waist Center", "Full measurement description");

    measurements.insert(frontShoulderSlopeLength_M, m);
    guiTexts.insert(frontShoulderSlopeLength_M, g);
    descriptions.insert(frontShoulderSlopeLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_shoulder_slope_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Shoulder Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "ShoulderTip to Back Waist Center", "Full measurement description");

    measurements.insert(backShoulderSlopeLength_M, m);
    guiTexts.insert(backShoulderSlopeLength_M, g);
    descriptions.insert(backShoulderSlopeLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_shoulder_to_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Full Length", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint straight down front chest to Waistline",
                                "Full measurement description");

    measurements.insert(frontShoulderToWaistLength_M, m);
    guiTexts.insert(frontShoulderToWaistLength_M, g);
    descriptions.insert(frontShoulderToWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_shoulder_to_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Full Length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back NeckPoint straight down back chest to Waistline",
                                "Full measurement description");

    measurements.insert(backShoulderToWaistLength_M, m);
    guiTexts.insert(backShoulderToWaistLength_M, g);
    descriptions.insert(backShoulderToWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_neck_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Neck arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to NeckPoint through Front Neck Center",
                                "Full measurement description");

    measurements.insert(frontNeckArc_M, m);
    guiTexts.insert(frontNeckArc_M, g);
    descriptions.insert(frontNeckArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_neck_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Neck arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to NeckPoint across Nape", "Full measurement description");

    measurements.insert(backNeckArc_M, m);
    guiTexts.insert(backNeckArc_M, g);
    descriptions.insert(backNeckArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_upper_chest_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "front_upper-bust_arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "front_upper-bust_arc", "Full measurement description");

    measurements.insert(frontUpperChestArc_M, m);
    guiTexts.insert(frontUpperChestArc_M, g);
    descriptions.insert(frontUpperChestArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_upper_chest_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back UpperBust arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back UpperBust side to side", "Full measurement description");

    measurements.insert(backUpperChestArc_M, m);
    guiTexts.insert(backUpperChestArc_M, g);
    descriptions.insert(backUpperChestArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_waist_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Waist arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front Waist side to side", "Full measurement description");

    measurements.insert(frontWaistArc_M, m);
    guiTexts.insert(frontWaistArc_M, g);
    descriptions.insert(frontWaistArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_waist_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Waist arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back Waist side to side", "Full measurement description");

    measurements.insert(backWaistArc_M, m);
    guiTexts.insert(backWaistArc_M, g);
    descriptions.insert(backWaistArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_upper_hip_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front UpperHip arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front UpperHip side to side", "Full measurement description");

    measurements.insert(frontUpperHipArc_M, m);
    guiTexts.insert(frontUpperHipArc_M, g);
    descriptions.insert(frontUpperHipArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_upper_hip_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back UpperHip arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back UpperHip side to side", "Full measurement description");

    measurements.insert(backUpperHipArc_M, m);
    guiTexts.insert(backUpperHipArc_M, g);
    descriptions.insert(backUpperHipArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_hip_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Hip arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front Hip side to side", "Full measurement description");

    measurements.insert(frontHipArc_M, m);
    guiTexts.insert(frontHipArc_M, g);
    descriptions.insert(frontHipArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_hip_arc",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Hip arc", "Full measurement name");
    d = VTranslation::translate("Measurements", "Back Hip side to side", "Full measurement description");

    measurements.insert(backHipArc_M, m);
    guiTexts.insert(backHipArc_M, g);
    descriptions.insert(backHipArc_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "chest_slope",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Chest Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to Front ArmfoldPoint", "Full measurement description");

    measurements.insert(chestSlope_M, m);
    guiTexts.insert(chestSlope_M, g);
    descriptions.insert(chestSlope_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_slope",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to Back ArmfoldPoint", "Full measurement description");

    measurements.insert(backSlope_M, m);
    guiTexts.insert(backSlope_M, g);
    descriptions.insert(backSlope_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_waist_slope",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front Waist Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint across Front Chest to Waist side",
                                "Full measurement description");

    measurements.insert(frontWaistSlope_M, m);
    guiTexts.insert(frontWaistSlope_M, g);
    descriptions.insert(frontWaistSlope_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "back_waist_slope",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Back Waist Balance", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint across Back Chest to Waist side",
                                "Full measurement description");

    measurements.insert(backWaistSlope_M, m);
    guiTexts.insert(backWaistSlope_M, g);
    descriptions.insert(backWaistSlope_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_neck_to_upper_chest_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front UpperChest height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front Neck Center straight down to UpperChest line",
                                "Full measurement description");

    measurements.insert(frontNeckToUpperChestHeight_M, m);
    guiTexts.insert(frontNeckToUpperChestHeight_M, g);
    descriptions.insert(frontNeckToUpperChestHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_neck_to_bust_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Bust height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front Neck Center straight down to Bust line",
                                "Full measurement description");

    measurements.insert(frontNeckToBustHeight_M, m);
    guiTexts.insert(frontNeckToBustHeight_M, g);
    descriptions.insert(frontNeckToBustHeight_M, d);
    //=================================================================================================================
    // arm
    m = VTranslation::translate("Measurements", "armscye_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Armscye Girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Armscye", "Full measurement description");

    measurements.insert(armscyeGirth_M, m);
    guiTexts.insert(armscyeGirth_M, g);
    descriptions.insert(armscyeGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "elbow_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Elbow Girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Elbow with elbow bent", "Full measurement description");

    measurements.insert(elbowGirth_M, m);
    guiTexts.insert(elbowGirth_M, g);
    descriptions.insert(elbowGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "upper_arm_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Upperarm Girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around UpperArm", "Full measurement description");

    measurements.insert(upperArmGirth_M, m);
    guiTexts.insert(upperArmGirth_M, g);
    descriptions.insert(upperArmGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "wrist_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Wrist girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Wrist", "Full measurement description");

    measurements.insert(wristGirth_M, m);
    guiTexts.insert(wristGirth_M, g);
    descriptions.insert(wristGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "scye_depth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Armscye depth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Nape straight down to UnderBust line (same as Back UpperBust height)",
                                "Full measurement description");

    measurements.insert(scyeDepth_M, m);
    guiTexts.insert(scyeDepth_M, g);
    descriptions.insert(scyeDepth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_and_arm_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder and Arm length", "Full measurement name");
    d = VTranslation::translate("Measurements", "NeckPoint to ShoulderTip to Wrist, with elbow bent and hand on hip",
                                "Full measurement description");

    measurements.insert(shoulderAndArmLength_M, m);
    guiTexts.insert(shoulderAndArmLength_M, g);
    descriptions.insert(shoulderAndArmLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "underarm_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Underarm length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Armpit to Wrist, with arm straight and hanging at side",
                                "Full measurement description");

    measurements.insert(underarmLength_M, m);
    guiTexts.insert(underarmLength_M, g);
    descriptions.insert(underarmLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "cervicale_to_wrist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Nape to wrist length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Nape to Wrist, with elbow bent and hand on hip",
                                "Full measurement description");

    measurements.insert(cervicaleToWristLength_M, m);
    guiTexts.insert(cervicaleToWristLength_M, g);
    descriptions.insert(cervicaleToWristLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_to_elbow_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Elbow length", "Full measurement name");
    d = VTranslation::translate("Measurements", "ShoulderTip to Elbow, with elbow bent and hand on hip",
                                "Full measurement description");

    measurements.insert(shoulderToElbowLength_M, m);
    guiTexts.insert(shoulderToElbowLength_M, g);
    descriptions.insert(shoulderToElbowLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "arm_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Arm length", "Full measurement name");
    d = VTranslation::translate("Measurements", "ShoulderTip to Wrist, with elbow bent and hand on hip",
                                "Full measurement description");

    measurements.insert(armLength_M, m);
    guiTexts.insert(armLength_M, g);
    descriptions.insert(armLength_M, d);
    //=================================================================================================================
    // hand
    m = VTranslation::translate("Measurements", "hand_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hand width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Hand side to side", "Full measurement description");

    measurements.insert(handWidth_M, m);
    guiTexts.insert(handWidth_M, g);
    descriptions.insert(handWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hand_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hand length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Hand Middle Finger tip to wrist", "Full measurement description");

    measurements.insert(handLength_M, m);
    guiTexts.insert(handLength_M, g);
    descriptions.insert(handLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hand_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hand girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Hand", "Full measurement description");

    measurements.insert(handGirth_M, m);
    guiTexts.insert(handGirth_M, g);
    descriptions.insert(handGirth_M, d);
    //=================================================================================================================
    // leg
    m = VTranslation::translate("Measurements", "thigh_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Thigh girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Thigh", "Full measurement description");

    measurements.insert(thighGirth_M, m);
    guiTexts.insert(thighGirth_M, g);
    descriptions.insert(thighGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "mid_thigh_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Midthigh girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around MidThigh", "Full measurement description");

    measurements.insert(midThighGirth_M, m);
    guiTexts.insert(midThighGirth_M, g);
    descriptions.insert(midThighGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "knee_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Knee girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Knee", "Full measurement description");

    measurements.insert(kneeGirth_M, m);
    guiTexts.insert(kneeGirth_M, g);
    descriptions.insert(kneeGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "calf_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Calf girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Calf", "Full measurement description");

    measurements.insert(calfGirth_M, m);
    guiTexts.insert(calfGirth_M, g);
    descriptions.insert(calfGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "ankle_girth",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Ankle girth", "Full measurement name");
    d = VTranslation::translate("Measurements", "Around Ankle", "Full measurement description");

    measurements.insert(ankleGirth_M, m);
    guiTexts.insert(ankleGirth_M, g);
    descriptions.insert(ankleGirth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "knee_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Knee height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Knee to Floor", "Full measurement description");

    measurements.insert(kneeHeight_M, m);
    guiTexts.insert(kneeHeight_M, g);
    descriptions.insert(kneeHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "ankle_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Ankle height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Ankle to Floor", "Full measurement description");

    measurements.insert(ankleHeight_M, m);
    guiTexts.insert(ankleHeight_M, g);
    descriptions.insert(ankleHeight_M, d);
    //=================================================================================================================
    // foot
    m = VTranslation::translate("Measurements", "foot_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Foot width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Widest part of Foot side to side", "Full measurement description");

    measurements.insert(footWidth_M, m);
    guiTexts.insert(footWidth_M, g);
    descriptions.insert(footWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "foot_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Foot length", "Full measurement name");
    d = VTranslation::translate("Measurements", "Tip of Longest Toe straight to back of heel",
                                "Full measurement description");

    measurements.insert(footLength_M, m);
    guiTexts.insert(footLength_M, g);
    descriptions.insert(footLength_M, d);
    //=================================================================================================================
    // heights
    m = VTranslation::translate("Measurements", "height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Total Height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Top of head to floor", "Full measurement description");

    measurements.insert(height_M, m);
    guiTexts.insert(height_M, g);
    descriptions.insert(height_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "cervicale_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Nape height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Nape to Floor", "Full measurement description");

    measurements.insert(cervicaleHeight_M, m);
    guiTexts.insert(cervicaleHeight_M, g);
    descriptions.insert(cervicaleHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "cervicale_to_knee_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Nape to knee height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Nape to Knee", "Full measurement description");

    measurements.insert(cervicaleToKneeHeight_M, m);
    guiTexts.insert(cervicaleToKneeHeight_M, g);
    descriptions.insert(cervicaleToKneeHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Waist side to floor", "Full measurement description");

    measurements.insert(waistHeight_M, m);
    guiTexts.insert(waistHeight_M, g);
    descriptions.insert(waistHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "high_hip_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "HighHip height", "Full measurement name");
    d = VTranslation::translate("Measurements", "HighHip side to Floor", "Full measurement description");

    measurements.insert(highHipHeight_M, m);
    guiTexts.insert(highHipHeight_M, g);
    descriptions.insert(highHipHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hip_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hip height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Hip side to Floor", "Full measurement description");

    measurements.insert(hipHeight_M, m);
    guiTexts.insert(hipHeight_M, g);
    descriptions.insert(hipHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_to_hip_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist to Hip height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Waist side to Hip", "Full measurement description");

    measurements.insert(waistToHipHeight_M, m);
    guiTexts.insert(waistToHipHeight_M, g);
    descriptions.insert(waistToHipHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_to_knee_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist to Knee height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Waist side to Knee", "Full measurement description");

    measurements.insert(waistToKneeHeight_M, m);
    guiTexts.insert(waistToKneeHeight_M, g);
    descriptions.insert(waistToKneeHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "crotch_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Crotch height/Inseam", "Full measurement name");
    d = VTranslation::translate("Measurements", "Crotch to Floor along inside leg", "Full measurement description");

    measurements.insert(crotchHeight_M, m);
    guiTexts.insert(crotchHeight_M, g);
    descriptions.insert(crotchHeight_M, d);
    //=================================================================================================================
    //extended
    m = VTranslation::translate("Measurements", "size", "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Size", "Full measurement name");
    d = VTranslation::translate("Measurements", "Size", "Full measurement description");

    measurements.insert(size_M, m);
    guiTexts.insert(size_M, g);
    descriptions.insert(size_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_front_neck_base_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height front neck base point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height of the point base of the neck in front",
                                "Full measurement description");

    measurements.insert(heightFrontNeckBasePoint_M, m);
    guiTexts.insert(heightFrontNeckBasePoint_M, g);
    descriptions.insert(heightFrontNeckBasePoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_base_neck_side_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height base neck side point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height of the base of the neck side point",
                                "Full measurement description");

    measurements.insert(heightBaseNeckSidePoint_M, m);
    guiTexts.insert(heightBaseNeckSidePoint_M, g);
    descriptions.insert(heightBaseNeckSidePoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_shoulder_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height shoulder point", "Full measurement name");
    d = VTranslation::translate("Measurements", "The height of the shoulder point", "Full measurement description");

    measurements.insert(heightShoulderPoint_M, m);
    guiTexts.insert(heightShoulderPoint_M, g);
    descriptions.insert(heightShoulderPoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_nipple_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height nipple point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height nipple point", "Full measurement description");

    measurements.insert(heightNipplePoint_M, m);
    guiTexts.insert(heightNipplePoint_M, g);
    descriptions.insert(heightNipplePoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_back_angle_axilla",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height back angle axilla", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height back angle axilla", "Full measurement description");

    measurements.insert(heightBackAngleAxilla_M, m);
    guiTexts.insert(heightBackAngleAxilla_M, g);
    descriptions.insert(heightBackAngleAxilla_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_scapular_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height scapular point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height scapular point", "Full measurement description");

    measurements.insert(heightScapularPoint_M, m);
    guiTexts.insert(heightScapularPoint_M, g);
    descriptions.insert(heightScapularPoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_under_buttock_folds",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height under buttock folds", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height under buttock folds", "Full measurement description");

    measurements.insert(heightUnderButtockFolds_M, m);
    guiTexts.insert(heightUnderButtockFolds_M, g);
    descriptions.insert(heightUnderButtockFolds_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hips_excluding_protruding_abdomen",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hips excluding protruding abdomen", "Full measurement name");
    d = VTranslation::translate("Measurements", "Hips excluding protruding abdomen", "Full measurement description");

    measurements.insert(hipsExcludingProtrudingAbdomen_M, m);
    guiTexts.insert(hipsExcludingProtrudingAbdomen_M, g);
    descriptions.insert(hipsExcludingProtrudingAbdomen_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "girth_foot_instep",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Girth foot instep", "Full measurement name");
    d = VTranslation::translate("Measurements", "Girth foot instep", "Full measurement description");

    measurements.insert(girthFootInstep_M, m);
    guiTexts.insert(girthFootInstep_M, g);
    descriptions.insert(girthFootInstep_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "side_waist_to_floor",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Side waist to floor", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the side waist to floor",
                                "Full measurement description");

    measurements.insert(sideWaistToFloor_M, m);
    guiTexts.insert(sideWaistToFloor_M, g);
    descriptions.insert(sideWaistToFloor_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_waist_to_floor",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front waist to floor", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the front waist to floor",
                                "Full measurement description");

    measurements.insert(frontWaistToFloor_M, m);
    guiTexts.insert(frontWaistToFloor_M, g);
    descriptions.insert(frontWaistToFloor_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "arc_through_groin_area",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Arc through groin area", "Full measurement name");
    d = VTranslation::translate("Measurements", "Arc through groin area", "Full measurement description");

    measurements.insert(arcThroughGroinArea_M, m);
    guiTexts.insert(arcThroughGroinArea_M, g);
    descriptions.insert(arcThroughGroinArea_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_to_plane_seat",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist to plane seat", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the waist to the plane seat",
                                "Full measurement description");

    measurements.insert(waistToPlaneSeat_M, m);
    guiTexts.insert(waistToPlaneSeat_M, g);
    descriptions.insert(waistToPlaneSeat_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_radial_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to radial point", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the base of the neck to the side of the radial"
                                " point", "Full measurement description");

    measurements.insert(neckToRadialPoint_M, m);
    guiTexts.insert(neckToRadialPoint_M, g);
    descriptions.insert(neckToRadialPoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_third_finger",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to third finger", "Full measurement name");
    d = VTranslation::translate("Measurements", "Distance from the base of the neck side point to the end of the third"
                                " finger", "Full measurement description");

    measurements.insert(neckToThirdFinger_M, m);
    guiTexts.insert(neckToThirdFinger_M, g);
    descriptions.insert(neckToThirdFinger_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_first_line_chest_circumference",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to first line chest circumference", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the base of the neck to the side of the first line"
                                " in front of chest circumference", "Full measurement description");

    measurements.insert(neckToFirstLineChestCircumference_M, m);
    guiTexts.insert(neckToFirstLineChestCircumference_M, g);
    descriptions.insert(neckToFirstLineChestCircumference_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_waist_length",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front waist length", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the base of the neck to the waist side front (waist"
                                " length in the front)", "Full measurement description");

    measurements.insert(frontWaistLength_M, m);
    guiTexts.insert(frontWaistLength_M, g);
    descriptions.insert(frontWaistLength_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "arc_through_shoulder_joint",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Arc through shoulder joint", "Full measurement name");
    d = VTranslation::translate("Measurements", "Arc through the highest point of the shoulder joint",
                                "Full measurement description");

    measurements.insert(arcThroughShoulderJoint_M, m);
    guiTexts.insert(arcThroughShoulderJoint_M, g);
    descriptions.insert(arcThroughShoulderJoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_back_line_chest_circumference",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to back line chest circumference", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the base of the neck to the back line of chest"
                                " circumference of the first and the second based on ledge vanes",
                                "Full measurement description");

    measurements.insert(neckToBackLineChestCircumference_M, m);
    guiTexts.insert(neckToBackLineChestCircumference_M, g);
    descriptions.insert(neckToBackLineChestCircumference_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_to_neck_side",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist to neck side", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the waist to the back base of the neck side point",
                                "Full measurement description");

    measurements.insert(waistToNeckSide_M, m);
    guiTexts.insert(waistToNeckSide_M, g);
    descriptions.insert(waistToNeckSide_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "arc_length_upper_body",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Arc length upper body", "Full measurement name");
    d = VTranslation::translate("Measurements", "Arc length of the upper body through the base of the neck side point",
                                "Full measurement description");

    measurements.insert(arcLengthUpperBody_M, m);
    guiTexts.insert(arcLengthUpperBody_M, g);
    descriptions.insert(arcLengthUpperBody_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "chest_width",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Chest width", "Full measurement name");
    d = VTranslation::translate("Measurements", "Chest width", "Full measurement description");

    measurements.insert(chestWidth_M, m);
    guiTexts.insert(chestWidth_M, g);
    descriptions.insert(chestWidth_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "anteroposterior_diameter_hands",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Anteroposterior diameter hands", "Full measurement name");
    d = VTranslation::translate("Measurements", "Anteroposterior diameter of the hands",
                                "Full measurement description");

    measurements.insert(anteroposteriorDiameterHands_M, m);
    guiTexts.insert(anteroposteriorDiameterHands_M, g);
    descriptions.insert(anteroposteriorDiameterHands_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_clavicular_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height clavicular point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Height clavicular point", "Full measurement description");

    measurements.insert(heightClavicularPoint_M, m);
    guiTexts.insert(heightClavicularPoint_M, g);
    descriptions.insert(heightClavicularPoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "height_armhole_slash",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Height armhole slash", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the point to the cervical level of the posterior"
                                " angle of the front armpit (underarm height oblique)", "Full measurement description");

    measurements.insert(heightArmholeSlash_M, m);
    guiTexts.insert(heightArmholeSlash_M, g);
    descriptions.insert(heightArmholeSlash_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "slash_shoulder_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Slash shoulder height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Slash shoulder height", "Full measurement description");

    measurements.insert(slashShoulderHeight_M, m);
    guiTexts.insert(slashShoulderHeight_M, g);
    descriptions.insert(slashShoulderHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_neck",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth neck", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth neck", "Full measurement description");

    measurements.insert(halfGirthNeck_M, m);
    guiTexts.insert(halfGirthNeck_M, g);
    descriptions.insert(halfGirthNeck_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_neck_for_shirts",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth neck for shirts", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth neck for shirts", "Full measurement description");

    measurements.insert(halfGirthNeckForShirts_M, m);
    guiTexts.insert(halfGirthNeckForShirts_M, g);
    descriptions.insert(halfGirthNeckForShirts_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_chest_first",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth chest first", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth chest first", "Full measurement description");

    measurements.insert(halfGirthChestFirst_M, m);
    guiTexts.insert(halfGirthChestFirst_M, g);
    descriptions.insert(halfGirthChestFirst_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_chest_second",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth chest second", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth chest second", "Full measurement description");

    measurements.insert(halfGirthChestSecond_M, m);
    guiTexts.insert(halfGirthChestSecond_M, g);
    descriptions.insert(halfGirthChestSecond_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_chest_third",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth chest third", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth chest third", "Full measurement description");

    measurements.insert(halfGirthChestThird_M, m);
    guiTexts.insert(halfGirthChestThird_M, g);
    descriptions.insert(halfGirthChestThird_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_waist",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth waist", "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth waist", "Full measurement description");

    measurements.insert(halfGirthWaist_M, m);
    guiTexts.insert(halfGirthWaist_M, g);
    descriptions.insert(halfGirthWaist_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_hips_considering_protruding_abdomen",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth hips considering protruding abdomen",
                                "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth hips considering protruding abdomen",
                                "Full measurement description");

    measurements.insert(halfGirthHipsConsideringProtrudingAbdomen_M, m);
    guiTexts.insert(halfGirthHipsConsideringProtrudingAbdomen_M, g);
    descriptions.insert(halfGirthHipsConsideringProtrudingAbdomen_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "half_girth_hips_excluding_protruding_abdomen",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Half girth hips excluding protruding abdomen",
                                "Full measurement name");
    d = VTranslation::translate("Measurements", "Half girth hips excluding protruding abdomen",
                                "Full measurement description");

    measurements.insert(halfGirthHipsExcludingProtrudingAbdomen_M, m);
    guiTexts.insert(halfGirthHipsExcludingProtrudingAbdomen_M, g);
    descriptions.insert(halfGirthHipsExcludingProtrudingAbdomen_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "girth_knee_flexed_feet",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Girth knee flexed feet", "Full measurement name");
    d = VTranslation::translate("Measurements", "Girth knee flexed feet", "Full measurement description");

    measurements.insert(girthKneeFlexedFeet_M, m);
    guiTexts.insert(girthKneeFlexedFeet_M, g);
    descriptions.insert(girthKneeFlexedFeet_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_transverse_diameter",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck transverse diameter", "Full measurement name");
    d = VTranslation::translate("Measurements", "Neck transverse diameter", "Full measurement description");

    measurements.insert(neckTransverseDiameter_M, m);
    guiTexts.insert(neckTransverseDiameter_M, g);
    descriptions.insert(neckTransverseDiameter_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "front_slash_shoulder_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Front slash shoulder height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Front slash shoulder height", "Full measurement description");

    measurements.insert(frontSlashShoulderHeight_M, m);
    guiTexts.insert(frontSlashShoulderHeight_M, g);
    descriptions.insert(frontSlashShoulderHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_front_waist_line",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to front waist line", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the base of the neck to the waist line front",
                                "Full measurement description");

    measurements.insert(neckToFrontWaistLine_M, m);
    guiTexts.insert(neckToFrontWaistLine_M, g);
    descriptions.insert(neckToFrontWaistLine_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "hand_vertical_diameter",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Hand vertical diameter", "Full measurement name");
    d = VTranslation::translate("Measurements", "Hand vertical diameter", "Full measurement description");

    measurements.insert(handVerticalDiameter_M, m);
    guiTexts.insert(handVerticalDiameter_M, g);
    descriptions.insert(handVerticalDiameter_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_knee_point",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to knee point", "Full measurement name");
    d = VTranslation::translate("Measurements", "Distance from neck to knee point", "Full measurement description");

    measurements.insert(neckToKneePoint_M, m);
    guiTexts.insert(neckToKneePoint_M, g);
    descriptions.insert(neckToKneePoint_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "waist_to_knee",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Waist to knee", "Full measurement name");
    d = VTranslation::translate("Measurements", "The distance from the waist to the knee",
                                "Full measurement description");

    measurements.insert(waistToKnee_M, m);
    guiTexts.insert(waistToKnee_M, g);
    descriptions.insert(waistToKnee_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "shoulder_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Shoulder height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Shoulder height", "Full measurement description");

    measurements.insert(shoulderHeight_M, m);
    guiTexts.insert(shoulderHeight_M, g);
    descriptions.insert(shoulderHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "head_height",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Head height", "Full measurement name");
    d = VTranslation::translate("Measurements", "Head height", "Full measurement description");

    measurements.insert(headHeight_M, m);
    guiTexts.insert(headHeight_M, g);
    descriptions.insert(headHeight_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "body_position",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Body position", "Full measurement name");
    d = VTranslation::translate("Measurements", "Body position", "Full measurement description");

    measurements.insert(bodyPosition_M, m);
    guiTexts.insert(bodyPosition_M, g);
    descriptions.insert(bodyPosition_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "arc_behind_shoulder_girdle",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Arc behind shoulder girdle", "Full measurement name");
    d = VTranslation::translate("Measurements", "Arc behind the shoulder girdle", "Full measurement description");

    measurements.insert(arcBehindShoulderGirdle_M, m);
    guiTexts.insert(arcBehindShoulderGirdle_M, g);
    descriptions.insert(arcBehindShoulderGirdle_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "neck_to_neck_base",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Neck to neck base", "Full measurement name");
    d = VTranslation::translate("Measurements", "Distance from neck point to point on the base of the neck side neck"
                                " girth measurement line", "Full measurement description");

    measurements.insert(neckToNeckBase_M, m);
    guiTexts.insert(neckToNeckBase_M, g);
    descriptions.insert(neckToNeckBase_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "depth_waist_first",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Depth waist first", "Full measurement name");
    d = VTranslation::translate("Measurements", "Depth waist first", "Full measurement description");

    measurements.insert(depthWaistFirst_M, m);
    guiTexts.insert(depthWaistFirst_M, g);
    descriptions.insert(depthWaistFirst_M, d);
    //=================================================================================================================
    m = VTranslation::translate("Measurements", "depth_waist_second",
                                "Short measurement name. Don't use math symbols in name!!!!");
    g = VTranslation::translate("Measurements", "Depth waist second", "Full measurement name");
    d = VTranslation::translate("Measurements", "Depth waist second", "Full measurement description");

    measurements.insert(depthWaistSecond_M, m);
    guiTexts.insert(depthWaistSecond_M, g);
    descriptions.insert(depthWaistSecond_M, d);
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitVariables()
{
    variables.insert(line_, VTranslation::translate("Variables", "Line_", "Left symbol _ in name"));
    variables.insert(angleLine_, VTranslation::translate("Variables", "AngleLine_", "Left symbol _ in name"));
    variables.insert(arc_, VTranslation::translate("Variables", "Arc_", "Left symbol _ in name"));
    variables.insert(spl_, VTranslation::translate("Variables", "Spl_", "Left symbol _ in name"));
    variables.insert(splPath, VTranslation::translate("Variables", "SplPath"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitFunctions()
{
    functions.insert(sin_F, VTranslation::translate("Functions", "sin", "sine function"));
    functions.insert(cos_F, VTranslation::translate("Functions", "cos", "cosine function"));
    functions.insert(tan_F, VTranslation::translate("Functions", "tan", "tangens function"));
    functions.insert(asin_F, VTranslation::translate("Functions", "asin", "arcus sine function"));
    functions.insert(acos_F, VTranslation::translate("Functions", "acos", "arcus cosine function"));
    functions.insert(atan_F, VTranslation::translate("Functions", "atan", "arcus tangens function"));
    functions.insert(sinh_F, VTranslation::translate("Functions", "sinh", "hyperbolic sine function"));
    functions.insert(cosh_F, VTranslation::translate("Functions", "cosh", "hyperbolic cosine"));
    functions.insert(tanh_F, VTranslation::translate("Functions", "tanh", "hyperbolic tangens function"));
    functions.insert(asinh_F, VTranslation::translate("Functions", "asinh", "hyperbolic arcus sine function"));
    functions.insert(acosh_F, VTranslation::translate("Functions", "acosh", "hyperbolic arcus tangens function"));
    functions.insert(atanh_F, VTranslation::translate("Functions", "atanh", "hyperbolic arcur tangens function"));
    functions.insert(log2_F, VTranslation::translate("Functions", "log2", "logarithm to the base 2"));
    functions.insert(log10_F, VTranslation::translate("Functions", "log10", "logarithm to the base 10"));
    functions.insert(log_F, VTranslation::translate("Functions", "log", "logarithm to the base 10"));
    functions.insert(ln_F, VTranslation::translate("Functions", "ln", "logarithm to base e (2.71828...)"));
    functions.insert(exp_F, VTranslation::translate("Functions", "exp", "e raised to the power of x"));
    functions.insert(sqrt_F, VTranslation::translate("Functions", "sqrt", "square root of a value"));
    functions.insert(sign_F, VTranslation::translate("Functions", "sign", "sign function -1 if x<0; 1 if x>0"));
    functions.insert(rint_F, VTranslation::translate("Functions", "rint", "round to nearest integer"));
    functions.insert(abs_F, VTranslation::translate("Functions", "abs", "absolute value"));
    functions.insert(min_F, VTranslation::translate("Functions", "min", "min of all arguments"));
    functions.insert(max_F, VTranslation::translate("Functions", "max", "max of all arguments"));
    functions.insert(sum_F, VTranslation::translate("Functions", "sum", "sum of all arguments"));
    functions.insert(avg_F, VTranslation::translate("Functions", "avg", "mean value of all arguments"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitPostfixOperators()
{
    postfixOperators.insert(cm_Oprt, VTranslation::translate("PostfixOperators", "cm", "centimeter"));
    postfixOperators.insert(mm_Oprt, VTranslation::translate("PostfixOperators", "mm", "millimeter"));
    postfixOperators.insert(in_Oprt, VTranslation::translate("PostfixOperators", "in", "inch"));
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, VTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if(token == i.value().translate())
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
bool VApplication::VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, VTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if(token.indexOf( i.value().translate() ) == 0)
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
bool VApplication::PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, VTranslation>::const_iterator i = postfixOperators.constBegin();
    while (i != postfixOperators.constEnd())
    {
        if(token == i.value().translate())
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
bool VApplication::FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, VTranslation>::const_iterator i = functions.constBegin();
    while (i != functions.constEnd())
    {
        if(token == i.value().translate())
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
bool VApplication::VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, VTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if(token.indexOf( i.key() ) == 0)
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

void VApplication::CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens,
                                        QMap<int, QString> &numbers)
{
    if(bias == 0)
    {
        return;
    }

    BiasTokens(position, bias, tokens);
    BiasTokens(position, bias, numbers);
}

void VApplication::BiasTokens(int position, int bias, QMap<int, QString> &tokens) const
{
    QMap<int, QString> newTokens;
    QMap<int, QString>::const_iterator i = tokens.constBegin();
    while (i != tokens.constEnd())
    {
        if(i.key()<= position)
        {
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
void VApplication::setPatternUnit(const Valentina::Units &patternUnit)
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
    if(VariablesToUser(newVar, 0, var, bias))
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
    if(MeasurementsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if(VariablesFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if(PostfixOperatorsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if(FunctionsFromUser(newVar, 0, var, bias))
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
QString VApplication::Variable(const QString &name) const
{
    return variables.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Function(const QString &name) const
{
    return functions.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::PostfixOperator(const QString &name) const
{
    return postfixOperators.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::FormulaFromUser(const QString &formula)
{
    QString newFormula = formula;

    Calculator *cal = new Calculator(formula);
    QMap<int, QString> tokens = cal->GetTokens();
    QMap<int, QString> numbers = cal->GetNumbers();
    delete cal;

    QList<int> tKeys = tokens.keys();
    QList<QString> tValues = tokens.values();
    for (int i = 0; i < tKeys.size(); ++i)
    {
        int bias = 0;
        if(MeasurementsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if(VariablesFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if(PostfixOperatorsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if(FunctionsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    bool osSeparatorValue = settings.value("configuration/osSeparator", 1).toBool();

    QLocale loc = QLocale::system();
    if(loc != QLocale(QLocale::C) && osSeparatorValue)
    {
        QList<int> nKeys = numbers.keys();
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            bool ok = false;
            qreal d = loc.toDouble(nValues.at(i), &ok);
            if (ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;
            }
            if (qFloor (d) < d)
            {
                QLocale loc = QLocale(QLocale::C);
                QString dStr = loc.toString(d);
                newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
                int bias = nValues.at(i).length() - dStr.length();
                if (bias != 0)
                {
                    CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                    nKeys = numbers.keys();
                    nValues = numbers.values();
                }
            }

        }
    }

    return newFormula;

}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::FormulaToUser(const QString &formula)
{
    QString newFormula = formula;

    QMap<int, QString> tokens;
    QMap<int, QString> numbers;
    try
    {
        Calculator *cal = new Calculator(formula, false);
        tokens = cal->GetTokens();
        numbers = cal->GetNumbers();
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
            {
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
            {
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
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        int bias = 0;
        if(VariablesToUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    bool osSeparatorValue = settings.value("configuration/osSeparator", 1).toBool();

    QLocale loc = QLocale::system();
    if(loc != QLocale::C && osSeparatorValue)
    {
        QList<int> nKeys = numbers.keys();
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            QLocale loc = QLocale(QLocale::C);
            bool ok = false;
            qreal d = loc.toDouble(nValues.at(i), &ok);
            if(ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;
            }
            if(qFloor (d) < d)
            {
                QLocale loc = QLocale::system();
                QString dStr = loc.toString(d);
                newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
                int bias = nValues.at(i).length() - dStr.length();

                if (bias != 0)
                {
                    CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                    nKeys = numbers.keys();
                    nValues = numbers.values();
                }
            }
        }
    }

    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
QUndoStack *VApplication::getUndoStack() const
{
    return undoStack;
}

