/************************************************************************
 **
 **  @file   vtranslatevars.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 6, 2015
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

#include "vtranslatevars.h"
#include "calculator.h"
#include "../vmisc/def.h"
#include "../vgeometry/vgeometrydef.h"

using namespace qmu;

//---------------------------------------------------------------------------------------------------------------------
VTranslateVars::VTranslateVars(bool osSeparator)
    :measurements(QMap<QString, QmuTranslation>()),
      PMSystemNames(QMap<QString, QmuTranslation>()),
      PMSystemAuthors(QMap<QString, QmuTranslation>()),
      PMSystemBooks(QMap<QString, QmuTranslation>()),
      guiTexts(QMap<QString, QmuTranslation>()),
      descriptions(QMap<QString, QmuTranslation>()),
      variables(QMap<QString, QmuTranslation>()),
      functions(QMap<QString, QmuTranslation>()),
      postfixOperators(QMap<QString, QmuTranslation>()),
      stDescriptions(QMap<QString, QmuTranslation>()),
      numbers(QMap<QString, QString>()),
      formulas(QMap<QString, QString>()),
      osSeparator(osSeparator)
{
    InitMeasurements();
    InitPatternMakingSystems();
    InitVariables();
    InitFunctions();
    InitPostfixOperators();
    InitSTDescriptions();
}

//---------------------------------------------------------------------------------------------------------------------
VTranslateVars::~VTranslateVars()
{}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitMeasurements()
{
    InitGroupA(); // Direct Height
    InitGroupB(); // Direct Width
    InitGroupC(); // Indentation
    InitGroupD(); // Circumference and Arc
    InitGroupE(); // Vertical
    InitGroupF(); // Horizontal
    InitGroupG(); // Bust
    InitGroupH(); // Balance
    InitGroupI(); // Arm
    InitGroupJ(); // Leg
    InitGroupK(); // Crotch and Rise
    InitGroupL(); // Hand
    InitGroupM(); // Foot
    InitGroupN(); // Head
    InitGroupO(); // Men & Tailoring
    InitGroupP(); // Historical & Specialty
    InitGroupQ(); // Patternmaking measurements
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPatternMakingSystems()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation name;
    QmuTranslation author;
    QmuTranslation book;

    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Bunka", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Bunka Fashion College", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Fundamentals of Garment Design", "Book name");
    InitSystem(p0_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Barnfield and Richard", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Jo Barnfield and Andrew Richards", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern Making Primer", "Book name");
    InitSystem(p1_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Friendship/Women", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Elizabeth Friendship", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Creating Historical Clothes - Pattern Cutting from "
                                     "the 16th to the 19th Centuries", "Book name");
    InitSystem(p2_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Morris, K.", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Karen Morris", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Sewing Lingerie that Fits", "Book name");
    InitSystem(p3_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Castro", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Lucia Mors de Castro", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Patternmaking in Practic", "Book name");
    InitSystem(p4_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Kim & Uh", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Injoo Kim and Mykyung Uh", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Apparel Making in Fashion Design", "Book name");
    InitSystem(p5_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Waugh", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Norah Waugh", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Corsets and Crinolines", "Book name");
    InitSystem(p6_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Grimble", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Frances Grimble", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Fashions of the Gilded Age", "Book name");
    InitSystem(p7_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Thornton's International System", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "ed. R. L. Shep", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Great War: Styles and Patterns of the 1910s",
                                     "Book name");
    InitSystem(p8_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Hillhouse & Mansfield", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Marion S. Hillhouse and Evelyn A. Mansfield",
                                       "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Dress Design: Draping and Flat Pattern Making",
                                     "Book name");
    InitSystem(p9_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Pivnick", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Esther Kaplan Pivnick", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "How to Design Beautiful Clothes: Designing and "
                                     "Pattern Making", "Book name");
    InitSystem(p10_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Minister & Son", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Edward Minister & Son, ed. R. L. Shep",
                                       "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Complete Guide to Practical Cutting (1853)",
                                     "Book name");
    InitSystem(p11_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Strickland", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Gertrude Strickland", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "A Tailoring Manual", "Book name");
    InitSystem(p12_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Loh & Lewis", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "May Loh and Diehl Lewis", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Patternless Fashion Design", "Book name");
    InitSystem(p13_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Morris, F. R.", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "F. R. Morris", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Ladies Garment Cutting and Making", "Book name");
    InitSystem(p14_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Mason", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Gertrude Mason", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Gertrude Mason's Patternmaking Book", "Book name");
    InitSystem(p15_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Kimata", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "K. Kimata", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "K.Kimata's Simplified Drafting Book for Dressmaking",
                                     "Book name");
    InitSystem(p16_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Master Designer", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "The Master Designer (Chicago, IL)", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Master Designer's System of Designing, Cutting and "
                                     "Grading", "Book name");
    InitSystem(p17_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Kopp", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Ernestine Kopp, Vittorina Rolfo, Beatrice Zelin, "
                                       "Lee Gross", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "How to Draft Basic Patterns", "Book name");
    InitSystem(p18_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Ekern", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Doris Ekern", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Slacks Cut-to-Fit for Your Figure", "Book name");
    InitSystem(p19_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Doyle", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Sarah J. Doyle", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Sarah's Key to Pattern Drafting", "Book name");
    InitSystem(p20_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Shelton", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Karla J. Shelton", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Design and Sew Jeans", "Book name");
    InitSystem(p21_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Lady Boutique", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Lady Boutique", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Lady Boutique magazine (Japan)", "Book name");
    InitSystem(p22_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Rohr", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "M. Rohr", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern Drafting and Grading: Women's nd Misses' "
                                     "Garment Design", "Book name");
    InitSystem(p23_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Moore", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Dorothy Moore", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Dorothy Moore's Pattern Drafting and Dressmaking",
                                     "Book name");
    InitSystem(p24_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "system_P25", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "system_P25", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "system_P25", "Book name");
    InitSystem(p25_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Fukomoto", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Sue S. Fukomoto", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Scientific Pattern Drafting as taught at Style Center "
                                     "School of Costume Design, Dressmaking and Millinery", "Book name");
    InitSystem(p26_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Dressmaking International", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Dressmaking International", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Dressmaking International magazine (Japan)",
                                     "Book name");
    InitSystem(p27_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Erwin", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Mabel D. Erwin", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Practical Dress Design", "Book name");
    InitSystem(p28_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Gough", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "E. L. G. Gough", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Principles of Garment Cutting", "Book name");
    InitSystem(p29_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Allemong", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Elizabeth M. Allemong", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "European Cut", "Book name");
    InitSystem(p30_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "McCunn", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Donald H. McCunn", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "How to Make Your Own Sewing Patterns", "Book name");
    InitSystem(p31_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Zarapkar", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Shri K. R. Zarapkar and Shri Arvind K. Zarapkar",
                                       "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Zarapkar System of Cutting", "Book name");
    InitSystem(p32_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Kunick", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Philip Kunick", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Sizing, Pattern Construction and Grading for Women's "
                                     "and Children's Garments", "Book name");
    InitSystem(p33_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Handford", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Jack Handford", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Professional Patternmaking for Designers: Women's "
                                     "Wear, Men's Casual Wear", "Book name");
    InitSystem(p34_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Davis", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "R. I. Davis", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Men's 17th & 18th Century Costume, Cut & Fashion",
                                     "Book name");
    InitSystem(p35_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "MacLochlainn", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Jason MacLochlainn", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Victorian Tailor: An Introduction to Period "
                                     "Tailoring", "Book name");
    InitSystem(p36_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Joseph-Armstrong", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Helen Joseph-Armstrong", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Patternmaking for Fashion Design", "Book name");
    InitSystem(p37_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Supreme System", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Frederick T. Croonberg", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Blue Book of Men's Tailoring, Grand Edition of "
                                     "Supreme System for Producing Mens Garments (1907)", "Book name");
    InitSystem(p38_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Sugino", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Dressmaking", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern Drafting Vols. I, II, III (Japan)",
                                     "Book name");
    InitSystem(p39_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Centre Point System", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Louis Devere", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Handbook of Practical Cutting on the Centre Point "
                                     "System", "Book name");
    InitSystem(p40_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Aldrich/Men", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Winifred Aldrich", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Metric Pattern Cutting for Menswear", "Book name");
    InitSystem(p41_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Aldrich/Women", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Winifred Aldrich", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Metric Pattern Cutting for Women's Wear", "Book name");
    InitSystem(p42_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Kershaw", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Gareth Kershaw", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Patternmaking for Menswear", "Book name");
    InitSystem(p43_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Gilewska", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Teresa Gilewska", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern-Drafting for Fashion: The Basics", "Book name");
    InitSystem(p44_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Lo", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Dennic Chunman Lo", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern Cutting", "Book name");
    InitSystem(p45_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Bray", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Natalie Bray", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Dress Pattern Designing: The Basic Principles of Cut "
                                     "and Fit", "Book name");
    InitSystem(p46_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Knowles/Men", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Lori A. Knowles", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Practical Guide to Patternmaking for Fashion "
                                     "Designers: Menswear", "Book name");
    InitSystem(p47_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Friendship/Men", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Elizabeth Friendship", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern Cutting for Men's Costume", "Book name");
    InitSystem(p48_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Brown", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "P. Clement Brown", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Art in Dress", "Book name");
    InitSystem(p49_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Mitchell", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Jno. J. Mitchell", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "\"Standard\" Work on Cutting (Men's Garments) 1886: "
                                     "The Art and Science of Garment Cutting", "Book name");
    InitSystem(p50_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "system_P51", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "system_P51", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "system_P51", "Book name");
    InitSystem(p51_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Eddy", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Josephine F. Eddy and Elizabeth C. B. Wiley",
                                       "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Pattern and Dress Design", "Book name");
    InitSystem(p52_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "Knowles/Women", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Lori A. Knowles", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Practical Guide to Patternmaking for Fashion "
                                     "Designers: Juniors, Misses, and Women", "Book name");
    InitSystem(p53_S, name, author, book);
    //=================================================================================================================
    name = QmuTranslation::translate("Pattern_making_systems", "American Garment Cutter", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "ed. R. L. Shep", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "The Great War: Styles and Patterns of the 1910s",
                                     "Book name");
    InitSystem(p54_S, name, author, book);
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitVariables()
{
    variables.insert(line_, QmuTranslation::translate("Variables", "Line_", "Left symbol _ in name"));
    variables.insert(angleLine_, QmuTranslation::translate("Variables", "AngleLine_", "Left symbol _ in name"));
    variables.insert(arc_, QmuTranslation::translate("Variables", "Arc_", "Left symbol _ in name"));
    variables.insert(spl_, QmuTranslation::translate("Variables", "Spl_", "Left symbol _ in name"));
    variables.insert(splPath, QmuTranslation::translate("Variables", "SplPath",
                                                        "Do not add symbol _ to the end of name"));
    variables.insert(radiusArc_, QmuTranslation::translate("Variables", "RadiusArc_", "Left symbol _ in name"));
    variables.insert(angle1Arc_, QmuTranslation::translate("Variables", "Angle1Arc_", "Left symbol _ in name"));
    variables.insert(angle2Arc_, QmuTranslation::translate("Variables", "Angle2Arc_", "Left symbol _ in name"));
    variables.insert(angle1Spl_, QmuTranslation::translate("Variables", "Angle1Spl_", "Left symbol _ in name"));
    variables.insert(angle2Spl_, QmuTranslation::translate("Variables", "Angle2Spl_", "Left symbol _ in name"));
    variables.insert(angle1SplPath, QmuTranslation::translate("Variables", "Angle1SplPath",
                                                             "Do not add symbol _ to the end of name"));
    variables.insert(angle2SplPath, QmuTranslation::translate("Variables", "Angle2SplPath",
                                                              "Do not add symbol _ to the end of name"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitFunctions()
{
    functions.insert(sin_F, QmuTranslation::translate("Functions", "sin", "sine function"));
    functions.insert(cos_F, QmuTranslation::translate("Functions", "cos", "cosine function"));
    functions.insert(tan_F, QmuTranslation::translate("Functions", "tan", "tangens function"));
    functions.insert(asin_F, QmuTranslation::translate("Functions", "asin", "arcus sine function"));
    functions.insert(acos_F, QmuTranslation::translate("Functions", "acos", "arcus cosine function"));
    functions.insert(atan_F, QmuTranslation::translate("Functions", "atan", "arcus tangens function"));
    functions.insert(sinh_F, QmuTranslation::translate("Functions", "sinh", "hyperbolic sine function"));
    functions.insert(cosh_F, QmuTranslation::translate("Functions", "cosh", "hyperbolic cosine"));
    functions.insert(tanh_F, QmuTranslation::translate("Functions", "tanh", "hyperbolic tangens function"));
    functions.insert(asinh_F, QmuTranslation::translate("Functions", "asinh", "hyperbolic arcus sine function"));
    functions.insert(acosh_F, QmuTranslation::translate("Functions", "acosh", "hyperbolic arcus tangens function"));
    functions.insert(atanh_F, QmuTranslation::translate("Functions", "atanh", "hyperbolic arcur tangens function"));
    functions.insert(log2_F, QmuTranslation::translate("Functions", "log2", "logarithm to the base 2"));
    functions.insert(log10_F, QmuTranslation::translate("Functions", "log10", "logarithm to the base 10"));
    functions.insert(log_F, QmuTranslation::translate("Functions", "log", "logarithm to the base 10"));
    functions.insert(ln_F, QmuTranslation::translate("Functions", "ln", "logarithm to base e (2.71828...)"));
    functions.insert(exp_F, QmuTranslation::translate("Functions", "exp", "e raised to the power of x"));
    functions.insert(sqrt_F, QmuTranslation::translate("Functions", "sqrt", "square root of a value"));
    functions.insert(sign_F, QmuTranslation::translate("Functions", "sign", "sign function -1 if x<0; 1 if x>0"));
    functions.insert(rint_F, QmuTranslation::translate("Functions", "rint", "round to nearest integer"));
    functions.insert(abs_F, QmuTranslation::translate("Functions", "abs", "absolute value"));
    functions.insert(min_F, QmuTranslation::translate("Functions", "min", "min of all arguments"));
    functions.insert(max_F, QmuTranslation::translate("Functions", "max", "max of all arguments"));
    functions.insert(sum_F, QmuTranslation::translate("Functions", "sum", "sum of all arguments"));
    functions.insert(avg_F, QmuTranslation::translate("Functions", "avg", "mean value of all arguments"));
    functions.insert(fmod_F, QmuTranslation::translate("Functions", "fmod",
                                         "Returns the floating-point remainder of numer/denom (rounded towards zero)"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPostfixOperators()
{
    postfixOperators.insert(cm_Oprt, QmuTranslation::translate("PostfixOperators", "cm", "centimeter"));
    postfixOperators.insert(mm_Oprt, QmuTranslation::translate("PostfixOperators", "mm", "millimeter"));
    postfixOperators.insert(in_Oprt, QmuTranslation::translate("PostfixOperators", "in", "inch"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitSTDescriptions()
{
    stDescriptions.insert("0", QmuTranslation::translate("STDescriptions",
                                                         "Standard figures of men 1st group, chest 100 cm",
                                                         "Standard table description"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitMeasurement(const QString &name, const QmuTranslation &m, const QmuTranslation &g,
                                     const QmuTranslation &d, const QString &number, const QString &formula)
{
    measurements.insert(name, m);
    guiTexts.insert(name, g);
    descriptions.insert(name, d);
    numbers.insert(name, number);
    formulas.insert(name, formula);
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitSystem(const QString &code, const QmuTranslation &name, const QmuTranslation &author,
                                const QmuTranslation &book)
{
    PMSystemNames.insert(code, name);
    PMSystemAuthors.insert(code, author);
    PMSystemBooks.insert(code, book);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CorrectionsPositions correct position tokens in expression after token translation.
 *
 * Because translated string can have different length compare to original need make correction after each translation.
 * If bias = 0 correction will not happens.
 *
 * @param position position currecnt token in expression
 * @param bias difference between original token length and translated
 * @param tokens all tokens
 * @param numbers all numbers
 */
void VTranslateVars::CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens,
                                          QMap<int, QString> &numbers) const
{
    if (bias == 0)
    {
        return;// Nothing to correct;
    }

    BiasTokens(position, bias, tokens);
    BiasTokens(position, bias, numbers);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasTokens change position for each token that have position more then "position".
 * @param position token position
 * @param bias difference between original token length and translated
 * @param tokens all tokens
 */
void VTranslateVars::BiasTokens(int position, int bias, QMap<int, QString> &tokens)
{
    QMap<int, QString> newTokens;
    QMap<int, QString>::const_iterator i = tokens.constBegin();
    while (i != tokens.constEnd())
    {
        if (i.key()<= position)
        { // Tokens before position "position" did not change his positions.
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
/**
 * @brief MeasurementsFromUser translate measurement to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found measurement with same name.
 */
bool VTranslateVars::MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if (token == i.value().translate())
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
/**
 * @brief VariablesFromUser translate variable to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found variable with same name.
 */
bool VTranslateVars::VariablesFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if (token.indexOf( i.value().translate() ) == 0)
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
/**
 * @brief PostfixOperatorsFromUser translate postfix operator to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found postfix operator with same name.
 */
bool VTranslateVars::PostfixOperatorsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = postfixOperators.constBegin();
    while (i != postfixOperators.constEnd())
    {
        if (token == i.value().translate())
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
/**
 * @brief FunctionsFromUser translate function name to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found function with same name.
 */
bool VTranslateVars::FunctionsFromUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = functions.constBegin();
    while (i != functions.constEnd())
    {
        if (token == i.value().translate())
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
/**
 * @brief VariablesToUser translate variable name to user.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found variable with same name.
 */
bool VTranslateVars::VariablesToUser(QString &newFormula, int position, const QString &token, int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = variables.constBegin();
    while (i != variables.constEnd())
    {
        if (token.indexOf( i.key() ) == 0)
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

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::VarToUser(const QString &var) const
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
    if (VariablesToUser(newVar, 0, var, bias))
    {
        return newVar;
    }
    return newVar;
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::VarFromUser(const QString &var) const
{
    QString newVar = var;
    int bias = 0;
    if (MeasurementsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (VariablesFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (PostfixOperatorsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }

    if (FunctionsFromUser(newVar, 0, var, bias))
    {
        return newVar;
    }
    return newVar;
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::PMSystemName(const QString &code) const
{
    return PMSystemNames.value(code).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::PMSystemAuthor(const QString &code) const
{
    return PMSystemAuthors.value(code).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::PMSystemBook(const QString &code) const
{
    return PMSystemBooks.value(code).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::MToUser(const QString &measurement) const
{
    return measurements.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::MFromUser(const QString &measurement) const
{
    QMap<QString, QmuTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if (measurement == i.value().translate())
        {
            return i.key();
        }
        ++i;
    }
    return measurement;
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::MNumber(const QString &measurement) const
{
    return numbers.value(measurement);
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::MFormula(const QString &measurement) const
{
    return formulas.value(measurement);
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::GuiText(const QString &measurement) const
{
    return guiTexts.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::Description(const QString &measurement) const
{
    return descriptions.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QString VTranslateVars::PostfixOperator(const QString &name) const
{
    return postfixOperators.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateVars::STDescription(const QString &id) const
{
    if (stDescriptions.contains(id))
    {
        return stDescriptions.value(id).translate();
    }
    else
    {
        qDebug()<<"Unknown id number. Got"<<id;
    }
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FormulaFromUser replace all known tokens in formula to internal look. Also change decimal
 * separator in numbers.
 * @param formula expression that need translate
 * @return translated expression
 */
QString VTranslateVars::FormulaFromUser(const QString &formula, bool osSeparator) const
{
    if (formula.isEmpty())
    {
        return formula;
    }
    QString newFormula = formula;// Local copy for making changes

    Calculator *cal = new Calculator(formula, osSeparator);// Eval formula
    QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
    QMap<int, QString> numbers = cal->GetNumbers();// All numbers in expression for changing decimal separator
    delete cal;

    QList<int> tKeys = tokens.keys();// Take all tokens positions
    QList<QString> tValues = tokens.values();
    for (int i = 0; i < tKeys.size(); ++i)
    {
        int bias = 0;
        if (MeasurementsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (VariablesFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (PostfixOperatorsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        if (FunctionsFromUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QLocale loc = QLocale::system(); // User locale
    if (loc != QLocale(QLocale::C) && osSeparator)
    {// User want use Os separator
        QList<int> nKeys = numbers.keys();// Positions for all numbers in expression
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            loc = QLocale::system();// From system locale
            bool ok = false;
            const qreal d = loc.toDouble(nValues.at(i), &ok);
            if (ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;//Leave with out translation
            }

            loc = QLocale(QLocale::C);// To internal locale
            const QString dStr = loc.toString(d);// Internal look for number
            newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
            const int bias = nValues.at(i).length() - dStr.length();
            if (bias != 0)
            {// Translated number has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                nKeys = numbers.keys();
                nValues = numbers.values();
            }
        }
    }

    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FormulaToUser replace all known tokens in formula to user look. Also change decimal
 * separator in numbers.
 * @param formula expression that need translate
 * @return translated expression
 */
QString VTranslateVars::FormulaToUser(const QString &formula) const
{
    if (formula.isEmpty())
    {
        return formula;
    }

    QString newFormula = formula;// Local copy for making changes

    QMap<int, QString> tokens;
    QMap<int, QString> numbers;
    try
    {
        Calculator *cal = new Calculator(formula, false);// Eval formula
        tokens = cal->GetTokens();// Tokens (variables, measurements)
        numbers = cal->GetNumbers();// All numbers in expression for changing decimal separator
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
            {// Translated token has different length than original. Position next tokens need to be corrected.
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
            {// Translated token has different length than original. Position next tokens need to be corrected.
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
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }

        int bias = 0;
        if (VariablesToUser(newFormula, tKeys.at(i), tValues.at(i), bias))
        {
            if (bias != 0)
            {// Translated token has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(tKeys.at(i), bias, tokens, numbers);
                tKeys = tokens.keys();
                tValues = tokens.values();
            }
            continue;
        }
    }

    QLocale loc = QLocale::system();// User locale
    if (loc != QLocale::C && osSeparator)
    {// User want use Os separator
        QList<int> nKeys = numbers.keys();// Positions for all numbers in expression
        QList<QString> nValues = numbers.values();
        for (int i = 0; i < nKeys.size(); ++i)
        {
            loc = QLocale(QLocale::C);// From pattern locale
            bool ok = false;
            const qreal d = loc.toDouble(nValues.at(i), &ok);
            if (ok == false)
            {
                qDebug()<<"Can't convert to double token"<<nValues.at(i);
                continue;//Leave with out translation
            }

            loc = QLocale::system();// To user locale
            const QString dStr = loc.toString(d);// Number string in user locale
            newFormula.replace(nKeys.at(i), nValues.at(i).length(), dStr);
            const int bias = nValues.at(i).length() - dStr.length();
            if (bias != 0)
            {// Translated number has different length than original. Position next tokens need to be corrected.
                CorrectionsPositions(nKeys.at(i), bias, tokens, numbers);
                nKeys = numbers.keys();
                nValues = numbers.values();
            }
        }
    }

    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupA()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Total", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Total", "Full measurement description.");
    InitMeasurement(height_M, m, g, d, "A01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Neck Back",
                                  "Full measurement description.");
    InitMeasurement(heightNeckBack_M, m, g, d, "A02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_scapula",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Scapula", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Scapula", "Full measurement description.");
    InitMeasurement(heightScapula_M, m, g, d, "A03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_armpit",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Armpit", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Armpit", "Full measurement description.");
    InitMeasurement(heightArmpit_M, m, g, d, "A04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Waist Side", "Full measurement description.");
    InitMeasurement(heightWaistSide_M, m, g, d, "A05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Hip", "Full measurement description.");
    InitMeasurement(heightHip_M, m, g, d, "A06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_gluteal_fold",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Gluteal Fold", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Gluteal Fold", "Full measurement description.");
    InitMeasurement(heightGlutealFold_M, m, g, d, "A07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Knee", "Full measurement description.");
    InitMeasurement(heightKnee_M, m, g, d, "A08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_calf",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Calf", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Calf", "Full measurement description.");
    InitMeasurement(heightCalf_M, m, g, d, "A09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_ankle_high",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Ankle High", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Ankle High", "Full measurement description.");
    InitMeasurement(heightAnkleHigh_M, m, g, d, "A10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Ankle", "Full measurement description.");
    InitMeasurement(heightAnkle_M, m, g, d, "A11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_highhip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Highhip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Highhip", "Full measurement description.");
    InitMeasurement(heightHighhip_M, m, g, d, "A12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Waist Front", "Full measurement description.");
    InitMeasurement(heightWaistFront_M, m, g, d, "A13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_bustpoint",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Bustpoint", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Bustpoint", "Full measurement description.");
    InitMeasurement(heightBustpoint_M, m, g, d, "A14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_shoulder_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Shoulder Tip", "Full measurement description.");
    InitMeasurement(heightShoulderTip_M, m, g, d, "A15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Neck Front", "Full measurement description.");
    InitMeasurement(heightNeckFront_M, m, g, d, "A16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Neck Side", "Full measurement description.");
    InitMeasurement(heightNeckSide_M, m, g, d, "A17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_back_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Back to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Neck Back to Knee", "Full measurement description.");
    InitMeasurement(heightNeckBackToKnee_M, m, g, d, "A18", "(height_neck_back - height_knee)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Waist Side to Knee",
                                  "Full measurement description.");
    InitMeasurement(heightWaistSideToKnee_M, m, g, d, "A19", "(height_waist_side - height_knee)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side_to_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side to Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Height Waist Side to Hip", "Full measurement description.");
    InitMeasurement(heightWaistSideToHip_M, m, g, d, "A20", "(height_waist_side - height_hip)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupB()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_shoulder",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Shoulder", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Width Shoulder", "Full measurement description.");
    InitMeasurement(widthShoulder_M, m, g, d, "B01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Width Bust", "Full measurement description.");
    InitMeasurement(widthBust_M, m, g, d, "B02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Width Waist", "Full measurement description.");
    InitMeasurement(widthWaist_M, m, g, d, "B03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Width Hip", "Full measurement description.");
    InitMeasurement(widthHip_M, m, g, d, "B04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_abdomen_to_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Abdomen to Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Width Abdomen to Hip", "Full measurement description.");
    InitMeasurement(widthAbdomenToHip_M, m, g, d, "B05");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupC()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "indent_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Indent Neck Back", "Full measurement description.");
    InitMeasurement(indentNeckBack_M, m, g, d, "C01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "indent_waist_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Indent Waist Back", "Full measurement description.");
    InitMeasurement(indentWaistBack_M, m, g, d, "C02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "indent_ankle_high",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Ankle High", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Indent Ankle High", "Full measurement description.");
    InitMeasurement(indentAnkleHigh_M, m, g, d, "C03");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupD()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_mid_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Middle Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Middle Circumference", "Full measurement description.");
    InitMeasurement(neckMidCirc_M, m, g, d, "D01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Circumference", "Full measurement description.");
    InitMeasurement(neckCirc_M, m, g, d, "D02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Circumference", "Full measurement description.");
    InitMeasurement(highbustCirc_M, m, g, d, "D03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Circumference", "Full measurement description.");
    InitMeasurement(bustCirc_M, m, g, d, "D04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust Circumference", "Full measurement description.");
    InitMeasurement(lowbustCirc_M, m, g, d, "D05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rib Circumference", "Full measurement description.");
    InitMeasurement(ribCirc_M, m, g, d, "D06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist Circumference", "Full measurement description.");
    InitMeasurement(waistCirc_M, m, g, d, "D07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highhip Circumference", "Full measurement description.");
    InitMeasurement(highhipCirc_M, m, g, d, "D08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip Circumference", "Full measurement description.");
    InitMeasurement(hipCirc_M, m, g, d, "D09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Arc (front)", "Full measurement description.");
    InitMeasurement(neckArcF_M, m, g, d, "D10", "(neck_circ - neck_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Arc (front)", "Full measurement description.");
    InitMeasurement(highbustArcF_M, m, g, d, "D11", "(highbust_circ - highbust_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Arc (front)", "Full measurement description.");
    InitMeasurement(bustArcF_M, m, g, d, "D12", "(bust_circ – bust_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust Arc (front)", "Full measurement description.");
    InitMeasurement(lowbustArcF_M, m, g, d, "D13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rib Arc (front)", "Full measurement description.");
    InitMeasurement(ribArcF_M, m, g, d, "D14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist Arc (front)", "Full measurement description.");
    InitMeasurement(waistArcF_M, m, g, d, "D15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highhip Arc (front)", "Full measurement description.");
    InitMeasurement(highhipArcF_M, m, g, d, "D16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip Arc (front)", "Full measurement description.");
    InitMeasurement(hipArcF_M, m, g, d, "D17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Arc (back)", "Full measurement description.");
    InitMeasurement(neckArcB_M, m, g, d, "D18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Arc (back)", "Full measurement description.");
    InitMeasurement(highbustArcB_M, m, g, d, "D19");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Arc (back)", "Full measurement description.");
    InitMeasurement(bustArcB_M, m, g, d, "D20");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust Arc (back)", "Full measurement description.");
    InitMeasurement(lowbustArcB_M, m, g, d, "D21", "(lowbust_circ - lowbust_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rib Arc (back)", "Full measurement description.");
    InitMeasurement(ribArcB_M, m, g, d, "D22", "(rib_circ - rib_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist Arc (back)", "Full measurement description.");
    InitMeasurement(waistArcB_M, m, g, d, "D23", "(waist_circ - waist_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highhip Arc (back)", "Full measurement description.");
    InitMeasurement(highhipArcB_M, m, g, d, "D24", "(highhip_circ - highhip_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip Arc (back)", "Full measurement description.");
    InitMeasurement(hipArcB_M, m, g, d, "D25", "(hip_circ - hip_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Arc, Half (front)", "Full measurement description.");
    InitMeasurement(neckArcHalfF_M, m, g, d, "D26", "(neck_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Arc, Half (front)", "Full measurement description.");
    InitMeasurement(highbustArcHalfF_M, m, g, d, "D27", "(highbust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Arc, Half (front)", "Full measurement description.");
    InitMeasurement(bustArcHalfF_M, m, g, d, "D28", "(bust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (front)", "Full measurement description.");
    InitMeasurement(lowbustArcHalfF_M, m, g, d, "D29", "(lowbust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rib Arc, Half (front)", "Full measurement description.");
    InitMeasurement(ribArcHalfF_M, m, g, d, "D30", "(rib_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist Arc, Half (front)", "Full measurement description.");
    InitMeasurement(waistArcHalfF_M, m, g, d, "D31", "(waist_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highhip Arc, Half (front)", "Full measurement description.");
    InitMeasurement(highhipArcHalfF_M, m, g, d, "D32", "(highhip_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip Arc, Half (front)", "Full measurement description.");
    InitMeasurement(hipArcHalfF_M, m, g, d, "D33", "(hip_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Arc, Half (back)", "Full measurement description.");
    InitMeasurement(neckArcHalfB_M, m, g, d, "D34", "(neck_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Arc, Half (back)", "Full measurement description.");
    InitMeasurement(highbustArcHalfB_M, m, g, d, "D35", "(highbust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Arc, Half (back)", "Full measurement description.");
    InitMeasurement(bustArcHalfB_M, m, g, d, "D36", "(bust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (back)", "Full measurement description.");
    InitMeasurement(lowbustArcHalfB_M, m, g, d, "D37", "(lowbust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rib Arc, Half (back)", "Full measurement description.");
    InitMeasurement(ribArcHalfB_M, m, g, d, "D38", "(rib_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist Arc, Half (back)", "Full measurement description.");
    InitMeasurement(waistArcHalfB_M, m, g, d, "D39", "(waist_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highhip Arc, Half (back)", "Full measurement description.");
    InitMeasurement(highhipArcHalfB_M, m, g, d, "D40", "(highhip_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip Arc, Half (back)", "Full measurement description.");
    InitMeasurement(hipArcHalfB_M, m, g, d, "D41", "(hip_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_with_abdomen_arc",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip with Abdomen Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hip with Abdomen Arc (front)", "Full measurement description.");
    InitMeasurement(hipWithAbdomenArcF_M, m, g, d, "D42");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_armfold_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference at Armfold", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Body Circumference at Armfold", "Full measurement description.");
    InitMeasurement(bodyArmfoldCirc_M, m, g, d, "D43");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_bust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference at Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Body Circumference at Bust", "Full measurement description.");
    InitMeasurement(bodyBustCirc_M, m, g, d, "D44");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_torso_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference around Torso", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Body Circumference around Torso", "Full measurement description.");
    InitMeasurement(bodyTorsoCirc_M, m, g, d, "D45");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupE()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Waist (front)", "Full measurement description.");
    InitMeasurement(neckFrontToWaistF_M, m, g, d, "E01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_flat_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist, Flat (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Waist, Flat (front)", "Full measurement description.");
    InitMeasurement(neckFrontToWaistFlatF_M, m, g, d, "E02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armpit_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armpit to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armpit to Waist Side", "Full measurement description.");
    InitMeasurement(armpitToWaistSide_M, m, g, d, "E03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (front)",
                                  "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideF_M, m, g, d, "E04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist (front)", "Full measurement description.");
    InitMeasurement(neckSideToWaistF_M, m, g, d, "E05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_bustpoint_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bustpoint (front)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bustpoint (front)",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistBustpointF_M, m, g, d, "E06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (back)", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideB_M, m, g, d, "E07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist (back)", "Full measurement description.");
    InitMeasurement(neckSideToWaistB_M, m, g, d, "E08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Waist Back", "Full measurement description.");
    InitMeasurement(neckBackToWaistB_M, m, g, d, "E09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_bladepoint_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bladepoint (Back)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bladepoint (Back)",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistBladepointB_M, m, g, d, "E10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (front)", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldF_M, m, g, d, "E11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust (front)", "Full measurement description.");
    InitMeasurement(neckSideToBustF_M, m, g, d, "E12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Highbust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Highbust (front)", "Full measurement description.");
    InitMeasurement(neckSideToHighbustF_M, m, g, d, "E13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_center_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (front), aimed at Bustpoint",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (front), aimed at Bustpoint",
                                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustF_M, m, g, d, "E14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Highbust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Highbust (front)", "Full measurement description.");
    InitMeasurement(neckFrontToHighbustF_M, m, g, d, "E15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_bust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Bust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Bust (front)", "Full measurement description.");
    InitMeasurement(neckFrontToBustF_M, m, g, d, "E16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust to Waist (front)", "Full measurement description.");
    InitMeasurement(lowbustToWaistF_M, m, g, d, "E17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (back)", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldB_M, m, g, d, "E18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust (back)", "Full measurement description.");
    InitMeasurement(neckSideToBustB_M, m, g, d, "E19");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Highbust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Highbust (back)", "Full measurement description.");
    InitMeasurement(neckSideToHighbustB_M, m, g, d, "E20");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_center_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (back), aimed through Bladepoint",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (back), aimed through Bladepoint",
                                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustB_M, m, g, d, "E21");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Highbust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Highbust (back)", "Full measurement description.");
    InitMeasurement(neckBackToHighbustB_M, m, g, d, "E22");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_bust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Bust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Bust (back)", "Full measurement description.");
    InitMeasurement(neckBackToBustB_M, m, g, d, "E23");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust to Waist (back)", "Full measurement description.");
    InitMeasurement(lowbustToWaistB_M, m, g, d, "E24");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (front)", "Full measurement description.");
    InitMeasurement(waistToHighhipF_M, m, g, d, "E25");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (front)", "Full measurement description.");
    InitMeasurement(waistToHipF_M, m, g, d, "E26");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (side)", "Full measurement description.");
    InitMeasurement(waistToHighhipSide_M, m, g, d, "E27");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (back)", "Full measurement description.");
    InitMeasurement(waistToHighhipB_M, m, g, d, "E28");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (back)", "Full measurement description.");
    InitMeasurement(waistToHipB_M, m, g, d, "E29");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (side)", "Full measurement description.");
    InitMeasurement(waistToHipSide_M, m, g, d, "E30");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_side_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope length from Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Slope length from Neck Side",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideLength_M, m, g, d, "E31");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_side_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope angle from Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Slope angle from Neck Side",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideAngle_M, m, g, d, "E32");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_back_height",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Height from Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Slope Height from Neck Back",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackHeight_M, m, g, d, "E33");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_back_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Neck Back",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackAngle_M, m, g, d, "E34");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_shoulder_tip_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Shoulder Tip",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeShoulderTipAngle_M, m, g, d, "E35");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement description.");
    InitMeasurement(highbustToWaistF_M, m, g, d, "E36", "(neck_front_to_waist - neck_front_to_highbust)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust to Waist (back)", "Full measurement description.");
    InitMeasurement(highbustToWaistB_M, m, g, d, "E37", "(neck_back_to_waist - neck_back_to_highbust)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupF()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Length", "Full measurement description.");
    InitMeasurement(shoulderLength_M, m, g, d, "F01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_width_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Width (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Width (front)", "Full measurement description.");
    InitMeasurement(shoulderWidthF_M, m, g, d, "F02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_chest_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Chest (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Across Chest (front)", "Full measurement description.");
    InitMeasurement(acrossChestF_M, m, g, d, "F03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement description.");
    InitMeasurement(armfoldToArmfoldF_M, m, g, d, "F04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_width_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement description.");
    InitMeasurement(shoulderWidthB_M, m, g, d, "F05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Width (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Width (back)", "Full measurement description.");
    InitMeasurement(acrossBackB_M, m, g, d, "F06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Back (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Across Back (back)", "Full measurement description.");
    InitMeasurement(armfoldToArmfoldB_M, m, g, d, "F07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_shoulder_tip_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Armfold (back)", "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfF_M, m, g, d, "F08", "(shoulder_tip_to_shoulder_tip_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_chest_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)",
                                  "Full measurement description.");
    InitMeasurement(acrossChestHalfF_M, m, g, d, "F09", "(across_chest_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_shoulder_tip_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)",
                                  "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfB_M, m, g, d, "F10", "(shoulder_tip_to_shoulder_tip_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)",
                                  "Full measurement description.");
    InitMeasurement(acrossBackHalfB_M, m, g, d, "F11", "(across_back_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_shoulder_tip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)",
                                  "Full measurement description.");
    InitMeasurement(neckFrontToShoulderTipF_M, m, g, d, "F12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_shoulder_tip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)",
                                  "Full measurement description.");
    InitMeasurement(neckBackToShoulderTipB_M, m, g, d, "F13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Shoulder Tip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Shoulder Tip (back)", "Full measurement description.");
    InitMeasurement(neckWidth_M, m, g, d, "F14");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupG()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint", "Full measurement description.");
    InitMeasurement(bustpointToBustpoint_M, m, g, d, "G01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_neck_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Neck Side", "Full measurement description.");
    InitMeasurement(bustpointToNeckSide_M, m, g, d, "G02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_lowbust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Lowbust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Lowbust", "Full measurement description.");
    InitMeasurement(bustpointToLowbust_M, m, g, d, "G03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement description.");
    InitMeasurement(bustpointToWaist_M, m, g, d, "G04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint_half",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalf_M, m, g, d, "G05", "(bustpoint_to_bustpoint/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint_halter",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint, Half", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint, Half", "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalter_M, m, g, d, "G06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_shoulder_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Shoulder Tip", "Full measurement description.");
    InitMeasurement(bustpointToShoulderTip_M, m, g, d, "G07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bustpoint to Waist Front", "Full measurement description.");
    InitMeasurement(bustpointToWaistFront_M, m, g, d, "G08");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupH()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Front", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistFront_M, m, g, d, "H01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Waist Side", "Full measurement description.");
    InitMeasurement(neckFrontToWaistSide_M, m, g, d, "H02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (front)", "Full measurement description.");
    InitMeasurement(neckSideToWaistSideF_M, m, g, d, "H03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Armfold (front)", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldF_M, m, g, d, "H04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armpit_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armpit (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Armpit (front)", "Full measurement description.");
    InitMeasurement(neckSideToArmpitF_M, m, g, d, "H05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (front)", "Full measurement description.");
    InitMeasurement(neckSideToBustSideF_M, m, g, d, "H06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Back", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistBack_M, m, g, d, "H07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Waist Side", "Full measurement description.");
    InitMeasurement(neckBackToWaistSide_M, m, g, d, "H08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (back)", "Full measurement description.");
    InitMeasurement(neckSideToWaistSideB_M, m, g, d, "H09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Armfold (back)", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldB_M, m, g, d, "H10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armpit_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armpit (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Armpit (back)", "Full measurement description.");
    InitMeasurement(neckSideToArmpitB_M, m, g, d, "H11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (back)", "Full measurement description.");
    InitMeasurement(neckSideToBustSideB_M, m, g, d, "H12");
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupI()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Wrist (bent)",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToWristBent_M, m, g, d, "I01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Elbow (bent)",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToElbowBent_M, m, g, d, "I02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist (bent)", "Full measurement description.");
    InitMeasurement(armElbowToWristBent_M, m, g, d, "I03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_circ_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Elbow circumference (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Elbow circumference (bent)", "Full measurement description.");
    InitMeasurement(armElbowCircBent_M, m, g, d, "I04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Wrist", "Full measurement description.");
    InitMeasurement(armShoulderTipToWrist_M, m, g, d, "I05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_elbow",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Elbow", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Elbow", "Full measurement description.");
    InitMeasurement(armShoulderTipToElbow_M, m, g, d, "I06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist", "Full measurement description.");
    InitMeasurement(armElbowToWrist_M, m, g, d, "I07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armpit_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Armpit to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Armpit to Wrist", "Full measurement description.");
    InitMeasurement(armArmpitToWrist_M, m, g, d, "I08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armpit_to_elbow",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Armpit to Elbow", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Armpit to Elbow", "Full measurement description.");
    InitMeasurement(armArmpitToElbow_M, m, g, d, "I09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist_inside",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist (inside)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Elbow to Wrist (inside)", "Full measurement description.");
    InitMeasurement(armElbowToWristInside_M, m, g, d, "I10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_upper_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Upper Arm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Upper Arm circumference", "Full measurement description.");
    InitMeasurement(armUpperCirc_M, m, g, d, "I11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_above_elbow_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Above Elbow circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Above Elbow circumference", "Full measurement description.");
    InitMeasurement(armAboveElbowCirc_M, m, g, d, "I12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Elbow circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Elbow circumference", "Full measurement description.");
    InitMeasurement(armElbowCirc_M, m, g, d, "I13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_lower_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Lower Arm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Lower Arm circumference", "Full measurement description.");
    InitMeasurement(armLowerCirc_M, m, g, d, "I14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_wrist_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Wrist circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Wrist circumference", "Full measurement description.");
    InitMeasurement(armWristCirc_M, m, g, d, "I15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_armfold_line",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Armfold line", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Shoulder Tip to Armfold line",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToArmfoldLine_M, m, g, d, "I16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armscye circumference", "Full measurement description.");
    InitMeasurement(armscyeCirc_M, m, g, d, "I17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye length, Shoulder Tip to Armpit vertical distance",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armscye length, Shoulder Tip to Armpit vertical distance",
                                  "Full measurement description.");
    InitMeasurement(armscyeLength_M, m, g, d, "I18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armscye width", "Full measurement description.");
    InitMeasurement(armscyeWidth_M, m, g, d, "I19");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_finger_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Side to Finger Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Side to Finger Tip", "Full measurement description.");
    InitMeasurement(armNeckSideToFingerTip_M, m, g, d, "I20",
                    "(shoulder_length + arm_shoulder_tip_to_wrist + hand_length)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm  - Neck Side to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm  - Neck Side to Wrist", "Full measurement description.");
    InitMeasurement(armNeckSideToWrist_M, m, g, d, "I21");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupJ()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_crotch_to_floor",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Crotch to floor (Inseam)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Crotch to floor (Inseam)", "Full measurement description.");
    InitMeasurement(legCrotchToFloor_M, m, g, d, "J01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_floor",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg >  Waist Side to floor (Outseam)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg >  Waist Side to floor (Outseam)",
                                  "Full measurement description.");
    InitMeasurement(legWaistSideToFloor_M, m, g, d, "J02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Waist Side to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Waist Side to Knee", "Full measurement description.");
    InitMeasurement(legWaistSideToKnee_M, m, g, d, "J03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_thigh_upper_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Thigh Upper circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Thigh Upper circumference", "Full measurement description.");
    InitMeasurement(legThighUpperCirc_M, m, g, d, "J04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_thigh_mid_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Thigh Middle circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Thigh Middle circumference", "Full measurement description.");
    InitMeasurement(legThighMidCirc_M, m, g, d, "J05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Knee circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Knee circumference", "Full measurement description.");
    InitMeasurement(legKneeCirc_M, m, g, d, "J06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_small_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Knee Small circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Knee Small circumference", "Full measurement description.");
    InitMeasurement(legKneeSmallCirc_M, m, g, d, "J07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_calf_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Calf circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Calf circumference", "Full measurement description.");
    InitMeasurement(legCalfCirc_M, m, g, d, "J08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_high_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Ankle High circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Ankle High circumference", "Full measurement description.");
    InitMeasurement(legAnkleHighCirc_M, m, g, d, "J09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Ankle circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Ankle circumference", "Full measurement description.");
    InitMeasurement(legAnkleCirc_M, m, g, d, "J10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_circ_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Knee circumference (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Knee circumference (bent)", "Full measurement description.");
    InitMeasurement(legKneeCircBent_M, m, g, d, "J11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_diag_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Ankle diagonal circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Ankle diagonal circumference",
                                  "Full measurement description.");
    InitMeasurement(legAnkleDiagCirc_M, m, g, d, "J12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_crotch_to_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Crotch to ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Crotch to ankle",
                                  "Full measurement description.");
    InitMeasurement(legCrotchToAnkle_M, m, g, d, "J13", "(leg_crotch_to_floor - height_ankle)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg > Waist Side to Ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg > Waist Side to Ankle",
                                  "Full measurement description.");
    InitMeasurement(legWaistSideToAnkle_M, m, g, d, "J14", "(leg_waist_side_to_floor - height_ankle)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupK()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length", "Full measurement description.");
    InitMeasurement(crotchLength_M, m, g, d, "K01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length (back)", "Full measurement description.");
    InitMeasurement(crotchLengthB_M, m, g, d, "K02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length (front)", "Full measurement description.");
    InitMeasurement(crotchLengthF_M, m, g, d, "K03", "(crotch_length - crotch_length_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (side)", "Full measurement description.");
    InitMeasurement(riseLengthSide_M, m, g, d, "K04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_diag",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (diagonal)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (diagonal)", "Full measurement description.");
    InitMeasurement(riseLengthDiag_M, m, g, d, "K05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (back)", "Full measurement description.");
    InitMeasurement(riseLengthB_M, m, g, d, "K06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (front)", "Full measurement description.");
    InitMeasurement(riseLengthF_M, m, g, d, "K07");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupL()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_palm_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm length", "Full measurement description.");
    InitMeasurement(handPalmLength_M, m, g, d, "L01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand length", "Full measurement description.");
    InitMeasurement(handLength_M, m, g, d, "L02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_palm_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm width", "Full measurement description.");
    InitMeasurement(handPalmWidth_M, m, g, d, "L03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_palm_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm circumference", "Full measurement description.");
    InitMeasurement(handPalmCirc_M, m, g, d, "L04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand circumference", "Full measurement description.");
    InitMeasurement(handCirc_M, m, g, d, "L05");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupM()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot width", "Full measurement description.");
    InitMeasurement(footWidth_M, m, g, d, "M01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot length", "Full measurement description.");
    InitMeasurement(footLength_M, m, g, d, "M02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot circumference", "Full measurement description.");
    InitMeasurement(footCirc_M, m, g, d, "M03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_instep_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot Instep circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot Instep circumference", "Full measurement description.");
    InitMeasurement(footInstepCirc_M, m, g, d, "M04");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupN()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head circumference", "Full measurement description.");
    InitMeasurement(headCirc_M, m, g, d, "N01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head length", "Full measurement description.");
    InitMeasurement(headLength_M, m, g, d, "N02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_depth",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head depth", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head depth", "Full measurement description.");
    InitMeasurement(headDepth_M, m, g, d, "N03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head width", "Full measurement description.");
    InitMeasurement(headWidth_M, m, g, d, "N04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_crown_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head Crown to Neck Back, vertical distance",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head Crown to Neck Back, vertical distance",
                                  "Full measurement description.");
    InitMeasurement(headCrownToNeckBack_M, m, g, d, "N05", "(height - height_neck_back)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_chin_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head > Chin to Neck Back, vertical distance",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head > Chin to Neck Back, vertical distance",
                                  "Full measurement description.");
    InitMeasurement(headChinToNeckBack_M, m, g, d, "N06", "(height - height_neck_back - head_length)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupO()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Waist Front", "Full measurement description.");
    InitMeasurement(neckBackToWaistFront_M, m, g, d, "O01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_waist_halter",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Waist Halter, around Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Waist Halter, around Neck Back",
                                  "Full measurement description.");
    InitMeasurement(waistToWaistHalter_M, m, g, d, "O02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist circumference", "Full measurement description.");
    InitMeasurement(waistNaturalCirc_M, m, g, d, "O03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist arc (front)", "Full measurement description.");
    InitMeasurement(waistNaturalArcF_M, m, g, d, "O04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist arc (back)", "Full measurement description.");
    InitMeasurement(waistNaturalArcB_M, m, g, d, "O05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_natural_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Natural Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Natural Waist (front)", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistF_M, m, g, d, "O06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_natural_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Natural Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Natural Waist (back)", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistB_M, m, g, d, "O07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_back_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Back to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Back to Elbow (bent)", "Full measurement description.");
    InitMeasurement(armNeckBackToElbowBent_M, m, g, d, "O08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_back_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Back to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Back to Wrist (bent)", "Full measurement description.");
    InitMeasurement(armNeckBackToWristBent_M, m, g, d, "O09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Side to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Side to Elbow (bent)", "Full measurement description.");
    InitMeasurement(armNeckSideToElbowBent_M, m, g, d, "O10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Side to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Side to Wrist (bent)", "Full measurement description.");
    InitMeasurement(armNeckSideToWristBent_M, m, g, d, "O11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_across_back_center_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Elbow (bent)",
                                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToElbowBent_M, m, g, d, "O12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_across_back_center_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Wrist (bent)",
                                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToWristBent_M, m, g, d, "O13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armscye_back_center_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Armscye Back Center to Wrist (bent)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Armscye Back Center to Wrist (bent)",
                                  "Full measurement description.");
    InitMeasurement(armArmscyeBackCenterToWristBent_M, m, g, d, "O14");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupP()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold curved through Bust Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Armfold curved through Bust Front",
                                  "Full measurement description.");
    InitMeasurement(armfoldToArmfoldBust_M, m, g, d, "P01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_bust_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Bust Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Bust Front", "Full measurement description.");
    InitMeasurement(armfoldToBustFront_M, m, g, d, "P02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_bust_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Bust Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Bust Front", "Full measurement description.");
    InitMeasurement(neckBackToBustFront_M, m, g, d, "P03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front", "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFront_M, m, g, d, "P04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Waist Side",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToWaistSide_M, m, g, d, "P05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_back_over_shoulder_to_armfold_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Armfold Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Armfold Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToArmfoldFront_M, m, g, d, "P06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_back_over_shoulder_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Waist Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Waist Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToWaistFront_M, m, g, d, "P07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Neck Back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToNeckBack_M, m, g, d, "P08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_center_to_armfold_front_to_across_back_center",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Back Center to Armfold Front to Across Back Center",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Across Back Center to Armfold Front to Across Back Center",
                                  "Full measurement description.");
    InitMeasurement(acrossBackCenterToArmfoldFrontToAcrossBackCenter_M, m, g, d, "P09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_highbust_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Highbust Back",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Highbust Back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToHighbustBack_M, m, g, d, "P10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_b_over_shoulder_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Highbust Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Highbust Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBOverShoulderToHighbustF_M, m, g, d, "P11",
                    "(mid_shoulder_to_highbust_f + mid_shoulder_to_highbust_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_arc",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye Arc through Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armscye Arc through Shoulder Tip", "Full measurement description.");
    InitMeasurement(armscyeArc_M, m, g, d, "P12");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitGroupQ()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "dart_width_shoulder",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Shoulder", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Shoulder", "Full measurement description.");
    InitMeasurement(dartWidthShoulder_M, m, g, d, "Q01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "dart_width_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Bust", "Full measurement description.");
    InitMeasurement(dartWidthBust_M, m, g, d, "Q02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "dart_width_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Waist", "Full measurement description.");
    InitMeasurement(dartWidthWaist_M, m, g, d, "Q03");
    //=================================================================================================================
}
