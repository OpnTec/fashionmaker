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
#include "../qmuparser/qmutokenparser.h"
#include "../ifc/ifcdef.h"

using namespace qmu;

//---------------------------------------------------------------------------------------------------------------------
VTranslateVars::VTranslateVars(bool osSeparator)
    :VTranslateMeasurements(),
      PMSystemNames(QMap<QString, QmuTranslation>()),
      PMSystemAuthors(QMap<QString, QmuTranslation>()),
      PMSystemBooks(QMap<QString, QmuTranslation>()),
      variables(QMap<QString, QmuTranslation>()),
      functions(QMap<QString, QmuTranslation>()),
      postfixOperators(QMap<QString, QmuTranslation>()),
      stDescriptions(QMap<QString, QmuTranslation>()),
      osSeparator(osSeparator)
{
    InitPatternMakingSystems();
    InitVariables();
    InitFunctions();
    InitPostfixOperators();
}

//---------------------------------------------------------------------------------------------------------------------
VTranslateVars::~VTranslateVars()
{}

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
    name = QmuTranslation::translate("Pattern_making_systems", "Abling", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Bina Abling", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Integrating Draping, Drafting and Drawing",
                                     "Book name");
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
    name = QmuTranslation::translate("Pattern_making_systems", "GOST 17917-86", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Ministry of consumer industry of the USSR",
                                       "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Standard figure boys", "Book name");
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
    name = QmuTranslation::translate("Pattern_making_systems", "None", "System name");
    author = QmuTranslation::translate("Pattern_making_systems", "Valentina team", "Author name");
    book = QmuTranslation::translate("Pattern_making_systems", "Valentina's internal standard", "Book name");
    InitSystem(p998_S, name, author, book);
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
// cppcheck-suppress unusedFunction
QString VTranslateVars::PostfixOperator(const QString &name) const
{
    return postfixOperators.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FormulaFromUser replace all known tokens in formula to internal look. Also change decimal
 * separator in numbers.
 * @param formula expression that need translate
 * @throw qmu::QmuParserError in case of a wrong expression
 * @return translated expression
 */
QString VTranslateVars::FormulaFromUser(const QString &formula, bool osSeparator) const
{
    if (formula.isEmpty())
    {
        return formula;
    }
    QString newFormula = formula;// Local copy for making changes

    QmuTokenParser *cal = new QmuTokenParser(formula, osSeparator);// Eval formula
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
        QmuTokenParser *cal = new QmuTokenParser(formula, false);// Eval formula
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
void VTranslateVars::Retranslate()
{
    VTranslateMeasurements::Retranslate();

    PMSystemNames.clear();
    PMSystemAuthors.clear();
    PMSystemBooks.clear();
    variables.clear();
    functions.clear();
    postfixOperators.clear();
    stDescriptions.clear();

    InitPatternMakingSystems();
    InitVariables();
    InitFunctions();
    InitPostfixOperators();
}
