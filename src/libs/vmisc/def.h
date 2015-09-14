/************************************************************************
 **
 **  @file   def.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
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

#ifndef DEF_H
#define DEF_H

#include <csignal>
#include <QtGlobal>
#include <QStringList>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif /* Q_OS_WIN */

#include "debugbreak.h"

#define SceneSize 50000
#define DefPointRadius 1.5//mm

enum class NodeDetail : char { Contour, Modeling };
enum class SceneObject : char { Point, Line, Spline, Arc, SplinePath, Detail, Unknown };
enum class MeasurementsType : char { Standard, Individual , Unknown};
enum class Unit : char { Mm = 0, Cm, Inch, Px, LAST_UNIT_DO_NOT_USE};
enum class Source : char { FromGui, FromFile, FromTool };

typedef unsigned char ToolVisHolderType;
enum class Tool : ToolVisHolderType
{
    Arrow,
    SinglePoint,
    DoublePoint,
    LinePoint,
    AbstractSpline,
    Cut,
    BasePoint,
    EndLine,
    Line,
    AlongLine,
    ShoulderPoint,
    Normal,
    Bisector,
    LineIntersect,
    Spline,
    CutSpline,
    CutArc,
    Arc,
    ArcWithLength,
    SplinePath,
    CutSplinePath,
    PointOfContact,
    Detail,
    NodePoint,
    NodeArc,
    NodeSpline,
    NodeSplinePath,
    Height,
    Triangle,
    LineIntersectAxis,
    PointOfIntersectionArcs,
    PointOfIntersectionCircles,
    CurveIntersectAxis,
    PointOfIntersection,
    PointFromCircleAndTangent,
    PointFromArcAndTangent,
    TrueDarts,
    UnionDetails, // 37
    LAST_ONE_DO_NOT_USE //add new stuffs above this, this constant must be last and never used
};

enum class Vis : ToolVisHolderType
{
    ControlPointSpline = static_cast<ToolVisHolderType>(Tool::LAST_ONE_DO_NOT_USE),
    GraphicsSimpleTextItem,
    SimpleSplinePath,
    SimplePoint,
    Line,
    Path,
    ToolAlongLine,
    ToolArc,
    ToolArcWithLength,
    ToolBisector,
    ToolCutArc,
    ToolEndLine,
    ToolHeight,
    ToolLine,
    ToolLineIntersect,
    ToolNormal,
    ToolPointOfContact,
    ToolPointOfIntersection,
    ToolPointOfIntersectionArcs,
    ToolPointOfIntersectionCircles,
    ToolPointFromCircleAndTangent,
    ToolPointFromArcAndTangent,
    ToolShoulderPoint,
    ToolSpline,
    ToolTriangle,
    ToolCutSpline,
    ToolSplinePath,
    ToolCutSplinePath,
    ToolLineIntersectAxis,
    ToolCurveIntersectAxis,
    ToolTrueDarts
};

enum class VarType : char { Measurement, Increment, LineLength, SplineLength, ArcLength, ArcRadius, LineAngle, ArcAngle,
                            SplineAngle, Unknown };

enum class GHeights : unsigned char { ALL,
                                      H92=92,   H98=98,   H104=104, H110=110, H116=116, H122=122, H128=128, H134=134,
                                      H140=140, H146=146, H152=152, H158=158, H164=164, H170=170, H176=176, H182=182,
                                      H188=188, H194=194};

enum class GSizes : unsigned char { ALL,
                                    S22=22, S24=24, S26=26, S28=28, S30=30, S32=32, S34=34, S36=36, S38=38, S40=40,
                                    S42=42, S44=44, S46=46, S48=48, S50=50, S52=52, S54=54, S56=56 };

/* QImage supports a maximum of 32768x32768 px images (signed short).
 * This follows from the condition: width * height * colordepth < INT_MAX (4 billion) -> 32768 * 32768 * 4 = 4 billion.
 * The second condition is of course that malloc is able to allocate the requested memory.
 *
 * If you really need bigger images you will have to use another wrapper or split into multiple QImage's.
 */
#define QIMAGE_MAX 32768

/*
 * This macros SCASSERT (for Stop and Continue Assert) will break into the debugger on the line of the assert and allow
 * you to continue afterwards should you choose to.
 * idea: Q_ASSERT no longer pauses debugger - http://qt-project.org/forums/viewthread/13148
 * Usefull links:
 * 1. What's the difference between __PRETTY_FUNCTION__, __FUNCTION__, __func__? -
 *    https://stackoverflow.com/questions/4384765/whats-the-difference-between-pretty-function-function-func
 *
 * 2. Windows Predefined Macros - http://msdn.microsoft.com/library/b0084kay.aspx
 *
 * 3. Windows DebugBreak function - http://msdn.microsoft.com/en-us/library/ms679297%28VS.85%29.aspx
 *
 * 4. Continue to debug after failed assertion on Linux? [C/C++] -
 * https://stackoverflow.com/questions/1721543/continue-to-debug-after-failed-assertion-on-linux-c-c
 */
#ifndef V_NO_ASSERT

#ifdef Q_CC_MSVC
#define V_PRETTY_FUNCTION __FUNCSIG__
#else // GCC/Clang
#define V_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif /*Q_CC_MSVC*/

#define SCASSERT(cond)                                      \
{                                                           \
    if (!(cond))                                            \
    {                                                       \
        qDebug("ASSERT: %s in %s (%s:%u)",                  \
            #cond, V_PRETTY_FUNCTION, __FILE__, __LINE__);  \
        debug_break();                                      \
    }                                                       \
}                                                           \

#else // define but disable this function if debugging is not set
#define SCASSERT(cond) qt_noop();
#endif /* V_NO_ASSERT */

#ifdef Q_CC_CLANG
#define V_FALLTHROUGH [[clang::fallthrough]];
#else
#define V_FALLTHROUGH
#endif

// measurements
// Need for standard table
extern const QString size_M;
// A
extern const QString height_M;                // A01
extern const QString heightNeckBack_M;        // A02
extern const QString heightScapula_M;         // A03
extern const QString heightArmpit_M;          // A04
extern const QString heightWaistSide_M;       // A05
extern const QString heightHip_M;             // A06
extern const QString heightGlutealFold_M;     // A07
extern const QString heightKnee_M;            // A08
extern const QString heightCalf_M;            // A09
extern const QString heightAnkleHigh_M;       // A10
extern const QString heightAnkle_M;           // A11
extern const QString heightHighhip_M;         // A12
extern const QString heightWaistFront_M;      // A13
extern const QString heightBustpoint_M;       // A14
extern const QString heightShoulderTip_M;     // A15
extern const QString heightNeckFront_M;       // A16
extern const QString heightNeckSide_M;        // A17
extern const QString heightNeckBackToKnee_M;  // A18
extern const QString heightWaistSideToKnee_M; // A19
extern const QString heightWaistSideToHip_M;  // A20
extern const QString heightKneeToAnkle_M;     // A21
// B
extern const QString widthShoulder_M;     // B01
extern const QString widthBust_M;         // B02
extern const QString widthWaist_M;        // B03
extern const QString widthHip_M;          // B04
extern const QString widthAbdomenToHip_M; // B05
// C
extern const QString indentNeckBack_M;  // C01
extern const QString indentWaistBack_M; // C02
extern const QString indentAnkleHigh_M; // C03
// D
extern const QString neckMidCirc_M;        // D01
extern const QString neckCirc_M;           // D02
extern const QString highbustCirc_M;       // D03
extern const QString bustCirc_M;           // D04
extern const QString lowbustCirc_M;        // D05
extern const QString ribCirc_M;            // D06
extern const QString waistCirc_M;          // D07
extern const QString highhipCirc_M;        // D08
extern const QString hipCirc_M;            // D09
extern const QString neckArcF_M;           // D10
extern const QString highbustArcF_M;       // D11
extern const QString bustArcF_M;           // D12
extern const QString lowbustArcF_M;        // D13
extern const QString ribArcF_M;            // D14
extern const QString waistArcF_M;          // D15
extern const QString highhipArcF_M;        // D16
extern const QString hipArcF_M;            // D17
extern const QString neckArcB_M;           // D18
extern const QString highbustArcB_M;       // D19
extern const QString bustArcB_M;           // D20
extern const QString lowbustArcB_M;        // D21
extern const QString ribArcB_M;            // D22
extern const QString waistArcB_M;          // D23
extern const QString highhipArcB_M;        // D24
extern const QString hipArcB_M;            // D25
extern const QString neckArcHalfF_M;       // D26
extern const QString highbustArcHalfF_M;   // D27
extern const QString bustArcHalfF_M;       // D28
extern const QString lowbustArcHalfF_M;    // D29
extern const QString ribArcHalfF_M;        // D30
extern const QString waistArcHalfF_M;      // D31
extern const QString highhipArcHalfF_M;    // D32
extern const QString hipArcHalfF_M;        // D33
extern const QString neckArcHalfB_M;       // D34
extern const QString highbustArcHalfB_M;   // D35
extern const QString bustArcHalfB_M;       // D36
extern const QString lowbustArcHalfB_M;    // D37
extern const QString ribArcHalfB_M;        // D38
extern const QString waistArcHalfB_M;      // D39
extern const QString highhipArcHalfB_M;    // D40
extern const QString hipArcHalfB_M;        // D41
extern const QString hipWithAbdomenArcF_M; // D42
extern const QString bodyArmfoldCirc_M;    // D43
extern const QString bodyBustCirc_M;       // D44
extern const QString bodyTorsoCirc_M;      // D45
// E
extern const QString neckFrontToWaistF_M;             // E01
extern const QString neckFrontToWaistFlatF_M;         // E02
extern const QString armpitToWaistSide_M;             // E03
extern const QString shoulderTipToWaistSideF_M;       // E04
extern const QString neckSideToWaistF_M;              // E05
extern const QString neckSideToWaistBustpointF_M;     // E06
extern const QString shoulderTipToWaistSideB_M;       // E07
extern const QString neckSideToWaistB_M;              // E08
extern const QString neckBackToWaistB_M;              // E09
extern const QString neckSideToWaistBladepointB_M;    // E10
extern const QString neckFrontToHighbustF_M;          // E11
extern const QString neckFrontToBustF_M;              // E12
extern const QString highbustToWaistF_M;              // E13
extern const QString bustToWaistF_M;                  // E14
extern const QString lowbustToWaistF_M;               // E15
extern const QString ribToWaistSide_M;                // E16
extern const QString neckBackToHighbustB_M;           // E17
extern const QString neckBackToBustB_M;               // E18
extern const QString highbustToWaistB_M;              // E19
extern const QString bustToWaistB_M;                  // E20
extern const QString lowbustToWaistB_M;               // E21
extern const QString shoulderTipToArmfoldF_M;         // E22
extern const QString neckSideToBustF_M;               // E23
extern const QString neckSideToHighbustF_M;           // E24
extern const QString shoulderCenterToHighbustF_M;     // E25
extern const QString shoulderTipToArmfoldB_M;         // E26
extern const QString neckSideToBustB_M;               // E27
extern const QString neckSideToHighbustB_M;           // E28
extern const QString shoulderCenterToHighbustB_M;     // E29
extern const QString waistToHighhipF_M;               // E30
extern const QString waistToHipF_M;                   // E31
extern const QString waistToHighhipSide_M;            // E32
extern const QString waistToHighhipB_M;               // E33
extern const QString waistToHipB_M;                   // E34
extern const QString waistToHipSide_M;                // E35
extern const QString shoulderSlopeNeckSideAngle_M;    // E36
extern const QString shoulderSlopeNeckSideLength_M;   // E37
extern const QString shoulderSlopeNeckBackAngle_M;    // E38
extern const QString shoulderSlopeNeckBackHeight_M;   // E39
extern const QString shoulderSlopeShoulderTipAngle_M; // E40
// F
extern const QString shoulderLength_M;                // F01
extern const QString shoulderWidthF_M;                // F02
extern const QString acrossChestF_M;                  // F03
extern const QString armfoldToArmfoldF_M;             // F04
extern const QString shoulderWidthB_M;                // F05
extern const QString acrossBackB_M;                   // F06
extern const QString armfoldToArmfoldB_M;             // F07
extern const QString shoulderTipToShoulderTipHalfF_M; // F08
extern const QString acrossChestHalfF_M;              // F09
extern const QString shoulderTipToShoulderTipHalfB_M; // F10
extern const QString acrossBackHalfB_M;               // F11
extern const QString neckFrontToShoulderTipF_M;       // F12
extern const QString neckBackToShoulderTipB_M;        // F13
extern const QString neckWidth_M;	                  // F14
// G
extern const QString bustpointToBustpoint_M;       // G01
extern const QString bustpointToNeckSide_M;        // G02
extern const QString bustpointToLowbust_M;         // G03
extern const QString bustpointToWaist_M;           // G04
extern const QString bustpointToBustpointHalf_M;   // G05
extern const QString bustpointToBustpointHalter_M; // G06
extern const QString bustpointToShoulderTip_M;     // G07
extern const QString bustpointToWaistFront_M;      // G08
// H
extern const QString shoulderTipToWaistFront_M;       // H01
extern const QString neckFrontToWaistSide_M;          // H02
extern const QString neckSideToWaistSideF_M;          // H03
extern const QString neckSideToArmfoldF_M;            // H04
extern const QString neckSideToArmpitF_M;             // H05
extern const QString neckSideToBustSideF_M;           // H06
extern const QString shoulderTipToWaistBack_M;        // H07
extern const QString neckBackToWaistSide_M;           // H08
extern const QString neckSideToWaistSideB_M;          // H09
extern const QString neckSideToArmfoldB_M;            // H10
extern const QString neckSideToArmpitB_M;             // H11
extern const QString neckSideToBustSideB_M;           // H12
extern const QString shoulderTipToWaistB_1inOffset_M; // H13
// I
extern const QString armShoulderTipToWristBent_M;   // I01
extern const QString armShoulderTipToElbowBent_M;   // I02
extern const QString armElbowToWristBent_M;         // I03
extern const QString armElbowCircBent_M;            // I04
extern const QString armShoulderTipToWrist_M;       // I05
extern const QString armShoulderTipToElbow_M;       // I06
extern const QString armElbowToWrist_M;             // I07
extern const QString armArmpitToWrist_M;            // I08
extern const QString armArmpitToElbow_M;            // I09
extern const QString armElbowToWristInside_M;       // I10
extern const QString armUpperCirc_M;                // I11
extern const QString armAboveElbowCirc_M;           // I12
extern const QString armElbowCirc_M;                // I13
extern const QString armLowerCirc_M;                // I14
extern const QString armWristCirc_M;                // I15
extern const QString armShoulderTipToArmfoldLine_M; // I16
extern const QString armscyeCirc_M;                 // I17
extern const QString armscyeLength_M;               // I18
extern const QString armscyeWidth_M;	            // I19
extern const QString armNeckSideToFingerTip_M;      // I20
extern const QString armNeckSideToWrist_M;          // I21
// J
extern const QString legCrotchToFloor_M;    // J01
extern const QString legWaistSideToFloor_M; // J02
extern const QString legWaistSideToKnee_M;  // J03
extern const QString legThighUpperCirc_M;	// J04
extern const QString legThighMidCirc_M;     // J05
extern const QString legKneeCirc_M;	        // J06
extern const QString legKneeSmallCirc_M;    // J07
extern const QString legCalfCirc_M;	        // J08
extern const QString legAnkleHighCirc_M;    // J09
extern const QString legAnkleCirc_M;        // J10
extern const QString legKneeCircBent_M;	    // J11
extern const QString legAnkleDiagCirc_M;    // J12
extern const QString legCrotchToAnkle_M;    // J13
extern const QString legWaistSideToAnkle_M; // J14

// K
extern const QString crotchLength_M;   // K01
extern const QString crotchLengthB_M;  // K02
extern const QString crotchLengthF_M;  // K03
extern const QString riseLengthSide_M; // K04
extern const QString riseLengthDiag_M; // K05
extern const QString riseLengthB_M;    // K06
extern const QString riseLengthF_M;    // K07
// L
extern const QString handPalmLength_M; // L01
extern const QString handLength_M;     // L02
extern const QString handPalmWidth_M;  // L03
extern const QString handPalmCirc_M;   // L04
extern const QString handCirc_M;	   // L05
// M
extern const QString footWidth_M;      // M01
extern const QString footLength_M;     // M02
extern const QString footCirc_M;       // M03
extern const QString footInstepCirc_M; // M04
// N
extern const QString headCirc_M;	        // N01
extern const QString headLength_M;          // N02
extern const QString headDepth_M;	        // N03
extern const QString headWidth_M;	        // N04
extern const QString headCrownToNeckBack_M; // N05
extern const QString headChinToNeckBack_M;  // N06
// O
extern const QString neckBackToWaistFront_M;	        // O01
extern const QString waistToWaistHalter_M;	            // O02
extern const QString waistNaturalCirc_M;	            // O03
extern const QString waistNaturalArcF_M;                // O04
extern const QString waistNaturalArcB_M;                // O05
extern const QString waistToNaturalWaistF_M;            // O06
extern const QString waistToNaturalWaistB_M;            // O07
extern const QString armNeckBackToElbowBent_M;          // O08
extern const QString armNeckBackToWristBent_M;          // O09
extern const QString armNeckSideToElbowBent_M;	        // O10
extern const QString armNeckSideToWristBent_M;	        // O11
extern const QString armAcrossBackCenterToElbowBent_M;  // O12
extern const QString armAcrossBackCenterToWristBent_M;  // O13
extern const QString armArmscyeBackCenterToWristBent_M; // O14
// P
extern const QString armfoldToArmfoldBust_M;                             // P01
extern const QString armfoldToBustFront_M;	                             // P02
extern const QString neckBackToBustFront_M;	                             // P03
extern const QString neckBackToArmfoldFront_M;	                         // P04
extern const QString neckBackToArmfoldFrontToWaistSide_M;                // P05
extern const QString highbustBackOverShoulderToArmfoldFront_M;           // P06
extern const QString highbustBackOverShoulderToWaistFront_M;             // P07
extern const QString neckBackToArmfoldFrontToNeckBack_M;                 // P08
extern const QString acrossBackCenterToArmfoldFrontToAcrossBackCenter_M; // P09
extern const QString neckBackToArmfoldFrontToHighbustBack_M;             // P10
extern const QString highbustBOverShoulderToHighbustF_M;	             // P11
extern const QString armscyeArc_M;	                                     // P12
// Q
extern const QString dartWidthShoulder_M; // Q01
extern const QString dartWidthBust_M;	  // Q02
extern const QString dartWidthWaist_M;    // Q03

QStringList ListGroupA();
QStringList ListGroupB();
QStringList ListGroupC();
QStringList ListGroupD();
QStringList ListGroupE();
QStringList ListGroupF();
QStringList ListGroupG();
QStringList ListGroupH();
QStringList ListGroupI();
QStringList ListGroupJ();
QStringList ListGroupK();
QStringList ListGroupL();
QStringList ListGroupM();
QStringList ListGroupN();
QStringList ListGroupO();
QStringList ListGroupP();
QStringList ListGroupQ();

// pattern making systems codes
extern const QString p0_S;
extern const QString p1_S;
extern const QString p2_S;
extern const QString p3_S;
extern const QString p4_S;
extern const QString p5_S;
extern const QString p6_S;
extern const QString p7_S;
extern const QString p8_S;
extern const QString p9_S;
extern const QString p10_S;
extern const QString p11_S;
extern const QString p12_S;
extern const QString p13_S;
extern const QString p14_S;
extern const QString p15_S;
extern const QString p16_S;
extern const QString p17_S;
extern const QString p18_S;
extern const QString p19_S;
extern const QString p20_S;
extern const QString p21_S;
extern const QString p22_S;
extern const QString p23_S;
extern const QString p24_S;
extern const QString p25_S;
extern const QString p26_S;
extern const QString p27_S;
extern const QString p28_S;
extern const QString p29_S;
extern const QString p30_S;
extern const QString p31_S;
extern const QString p32_S;
extern const QString p33_S;
extern const QString p34_S;
extern const QString p35_S;
extern const QString p36_S;
extern const QString p37_S;
extern const QString p38_S;
extern const QString p39_S;
extern const QString p40_S;
extern const QString p41_S;
extern const QString p42_S;
extern const QString p43_S;
extern const QString p44_S;
extern const QString p45_S;
extern const QString p46_S;
extern const QString p47_S;
extern const QString p48_S;
extern const QString p49_S;
extern const QString p50_S;
extern const QString p51_S;
extern const QString p52_S;
extern const QString p53_S;
extern const QString p54_S;

// functions
extern const QString sin_F;
extern const QString cos_F;
extern const QString tan_F;
extern const QString asin_F;
extern const QString acos_F;
extern const QString atan_F;
extern const QString sinh_F;
extern const QString cosh_F;
extern const QString tanh_F;
extern const QString asinh_F;
extern const QString acosh_F;
extern const QString atanh_F;
extern const QString log2_F;
extern const QString log10_F;
extern const QString log_F;
extern const QString ln_F;
extern const QString exp_F;
extern const QString sqrt_F;
extern const QString sign_F;
extern const QString rint_F;
extern const QString abs_F;
extern const QString min_F;
extern const QString max_F;
extern const QString sum_F;
extern const QString avg_F;
extern const QString fmod_F;

extern const QStringList builInFunctions;

// Postfix operators
extern const QString cm_Oprt;
extern const QString mm_Oprt;
extern const QString in_Oprt;

extern const QStringList builInPostfixOperators;

extern const QString cursorArrowOpenHand;
extern const QString cursorArrowCloseHand;

extern const QString degreeSymbol;

void SetOverrideCursor(const QString & pixmapPath, int hotX = -1, int hotY = -1);
void RestoreOverrideCursor(const QString & pixmapPath);

extern const qreal PrintDPI;

double ToPixel(double val, const Unit &unit);
double FromPixel(double pix, const Unit &unit);

qreal UnitConvertor(qreal value, const Unit &from, const Unit &to);

void CheckFactor(qreal &oldFactor, const qreal &Newfactor);

#endif // DEF_H
