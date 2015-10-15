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

#define translate(source, disambiguation) QmuTranslation::translate("VTranslateVars", (source), (disambiguation))

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPatternMakingSystems()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation name;
    QmuTranslation author;
    QmuTranslation book;

    //=================================================================================================================
    name = translate("Bunka", "System name");
    author = translate("Bunka Fashion College", "Author name");
    book = translate("Fundamentals of Garment Design", "Book name");
    InitSystem(p0_S, name, author, book);
    //=================================================================================================================
    name = translate("Barnfield and Richard", "System name");
    author = translate("Jo Barnfield and Andrew Richards", "Author name");
    book = translate("Pattern Making Primer", "Book name");
    InitSystem(p1_S, name, author, book);
    //=================================================================================================================
    name = translate("Friendship/Women", "System name");
    author = translate("Elizabeth Friendship", "Author name");
    book = translate("Creating Historical Clothes - Pattern Cutting from the 16th to the 19th Centuries", "Book name");
    InitSystem(p2_S, name, author, book);
    //=================================================================================================================
    name = translate("Morris, K.", "System name");
    author = translate("Karen Morris", "Author name");
    book = translate("Sewing Lingerie that Fits", "Book name");
    InitSystem(p3_S, name, author, book);
    //=================================================================================================================
    name = translate("Castro", "System name");
    author = translate("Lucia Mors de Castro", "Author name");
    book = translate("Patternmaking in Practic", "Book name");
    InitSystem(p4_S, name, author, book);
    //=================================================================================================================
    name = translate("Kim & Uh", "System name");
    author = translate("Injoo Kim and Mykyung Uh", "Author name");
    book = translate("Apparel Making in Fashion Design", "Book name");
    InitSystem(p5_S, name, author, book);
    //=================================================================================================================
    name = translate("Waugh", "System name");
    author = translate("Norah Waugh", "Author name");
    book = translate("Corsets and Crinolines", "Book name");
    InitSystem(p6_S, name, author, book);
    //=================================================================================================================
    name = translate("Grimble", "System name");
    author = translate("Frances Grimble", "Author name");
    book = translate("Fashions of the Gilded Age", "Book name");
    InitSystem(p7_S, name, author, book);
    //=================================================================================================================
    name = translate("Thornton's International System", "System name");
    author = translate("ed. R. L. Shep", "Author name");
    book = translate("The Great War: Styles and Patterns of the 1910s", "Book name");
    InitSystem(p8_S, name, author, book);
    //=================================================================================================================
    name = translate("Hillhouse & Mansfield", "System name");
    author = translate("Marion S. Hillhouse and Evelyn A. Mansfield", "Author name");
    book = translate("Dress Design: Draping and Flat Pattern Making", "Book name");
    InitSystem(p9_S, name, author, book);
    //=================================================================================================================
    name = translate("Pivnick", "System name");
    author = translate("Esther Kaplan Pivnick", "Author name");
    book = translate("How to Design Beautiful Clothes: Designing and Pattern Making", "Book name");
    InitSystem(p10_S, name, author, book);
    //=================================================================================================================
    name = translate("Minister & Son", "System name");
    author = translate("Edward Minister & Son, ed. R. L. Shep", "Author name");
    book = translate("The Complete Guide to Practical Cutting (1853)", "Book name");
    InitSystem(p11_S, name, author, book);
    //=================================================================================================================
    name = translate("Strickland", "System name");
    author = translate("Gertrude Strickland", "Author name");
    book = translate("A Tailoring Manual", "Book name");
    InitSystem(p12_S, name, author, book);
    //=================================================================================================================
    name = translate("Loh & Lewis", "System name");
    author = translate("May Loh and Diehl Lewis", "Author name");
    book = translate("Patternless Fashion Design", "Book name");
    InitSystem(p13_S, name, author, book);
    //=================================================================================================================
    name = translate("Morris, F. R.", "System name");
    author = translate("F. R. Morris", "Author name");
    book = translate("Ladies Garment Cutting and Making", "Book name");
    InitSystem(p14_S, name, author, book);
    //=================================================================================================================
    name = translate("Mason", "System name");
    author = translate("Gertrude Mason", "Author name");
    book = translate("Gertrude Mason's Patternmaking Book", "Book name");
    InitSystem(p15_S, name, author, book);
    //=================================================================================================================
    name = translate("Kimata", "System name");
    author = translate("K. Kimata", "Author name");
    book = translate("K.Kimata's Simplified Drafting Book for Dressmaking", "Book name");
    InitSystem(p16_S, name, author, book);
    //=================================================================================================================
    name = translate("Master Designer", "System name");
    author = translate("The Master Designer (Chicago, IL)", "Author name");
    book = translate("Master Designer's System of Designing, Cutting and Grading", "Book name");
    InitSystem(p17_S, name, author, book);
    //=================================================================================================================
    name = translate("Kopp", "System name");
    author = translate("Ernestine Kopp, Vittorina Rolfo, Beatrice Zelin, Lee Gross", "Author name");
    book = translate("How to Draft Basic Patterns", "Book name");
    InitSystem(p18_S, name, author, book);
    //=================================================================================================================
    name = translate("Ekern", "System name");
    author = translate("Doris Ekern", "Author name");
    book = translate("Slacks Cut-to-Fit for Your Figure", "Book name");
    InitSystem(p19_S, name, author, book);
    //=================================================================================================================
    name = translate("Doyle", "System name");
    author = translate("Sarah J. Doyle", "Author name");
    book = translate("Sarah's Key to Pattern Drafting", "Book name");
    InitSystem(p20_S, name, author, book);
    //=================================================================================================================
    name = translate("Shelton", "System name");
    author = translate("Karla J. Shelton", "Author name");
    book = translate("Design and Sew Jeans", "Book name");
    InitSystem(p21_S, name, author, book);
    //=================================================================================================================
    name = translate("Lady Boutique", "System name");
    author = translate("Lady Boutique", "Author name");
    book = translate("Lady Boutique magazine (Japan)", "Book name");
    InitSystem(p22_S, name, author, book);
    //=================================================================================================================
    name = translate("Rohr", "System name");
    author = translate("M. Rohr", "Author name");
    book = translate("Pattern Drafting and Grading: Women's nd Misses' Garment Design", "Book name");
    InitSystem(p23_S, name, author, book);
    //=================================================================================================================
    name = translate("Moore", "System name");
    author = translate("Dorothy Moore", "Author name");
    book = translate("Dorothy Moore's Pattern Drafting and Dressmaking", "Book name");
    InitSystem(p24_S, name, author, book);
    //=================================================================================================================
    name = translate("Abling", "System name");
    author = translate("Bina Abling", "Author name");
    book = translate("Integrating Draping, Drafting and Drawing", "Book name");
    InitSystem(p25_S, name, author, book);
    //=================================================================================================================
    name = translate("Fukomoto", "System name");
    author = translate("Sue S. Fukomoto", "Author name");
    book = translate("Scientific Pattern Drafting as taught at Style Center School of Costume Design, Dressmaking and "
                     "Millinery", "Book name");
    InitSystem(p26_S, name, author, book);
    //=================================================================================================================
    name = translate("Dressmaking International", "System name");
    author = translate("Dressmaking International", "Author name");
    book = translate("Dressmaking International magazine (Japan)", "Book name");
    InitSystem(p27_S, name, author, book);
    //=================================================================================================================
    name = translate("Erwin", "System name");
    author = translate("Mabel D. Erwin", "Author name");
    book = translate("Practical Dress Design", "Book name");
    InitSystem(p28_S, name, author, book);
    //=================================================================================================================
    name = translate("Gough", "System name");
    author = translate("E. L. G. Gough", "Author name");
    book = translate("Principles of Garment Cutting", "Book name");
    InitSystem(p29_S, name, author, book);
    //=================================================================================================================
    name = translate("Allemong", "System name");
    author = translate("Elizabeth M. Allemong", "Author name");
    book = translate("European Cut", "Book name");
    InitSystem(p30_S, name, author, book);
    //=================================================================================================================
    name = translate("McCunn", "System name");
    author = translate("Donald H. McCunn", "Author name");
    book = translate("How to Make Your Own Sewing Patterns", "Book name");
    InitSystem(p31_S, name, author, book);
    //=================================================================================================================
    name = translate("Zarapkar", "System name");
    author = translate("Shri K. R. Zarapkar and Shri Arvind K. Zarapkar", "Author name");
    book = translate("Zarapkar System of Cutting", "Book name");
    InitSystem(p32_S, name, author, book);
    //=================================================================================================================
    name = translate("Kunick", "System name");
    author = translate("Philip Kunick", "Author name");
    book = translate("Sizing, Pattern Construction and Grading for Women's and Children's Garments", "Book name");
    InitSystem(p33_S, name, author, book);
    //=================================================================================================================
    name = translate("Handford", "System name");
    author = translate("Jack Handford", "Author name");
    book = translate("Professional Patternmaking for Designers: Women's Wear, Men's Casual Wear", "Book name");
    InitSystem(p34_S, name, author, book);
    //=================================================================================================================
    name = translate("Davis", "System name");
    author = translate("R. I. Davis", "Author name");
    book = translate("Men's 17th & 18th Century Costume, Cut & Fashion", "Book name");
    InitSystem(p35_S, name, author, book);
    //=================================================================================================================
    name = translate("MacLochlainn", "System name");
    author = translate("Jason MacLochlainn", "Author name");
    book = translate("The Victorian Tailor: An Introduction to Period Tailoring", "Book name");
    InitSystem(p36_S, name, author, book);
    //=================================================================================================================
    name = translate("Joseph-Armstrong", "System name");
    author = translate("Helen Joseph-Armstrong", "Author name");
    book = translate("Patternmaking for Fashion Design", "Book name");
    InitSystem(p37_S, name, author, book);
    //=================================================================================================================
    name = translate("Supreme System", "System name");
    author = translate("Frederick T. Croonberg", "Author name");
    book = translate("The Blue Book of Men's Tailoring, Grand Edition of Supreme System for Producing Mens Garments "
                     "(1907)", "Book name");
    InitSystem(p38_S, name, author, book);
    //=================================================================================================================
    name = translate("Sugino", "System name");
    author = translate("Dressmaking", "Author name");
    book = translate("Pattern Drafting Vols. I, II, III (Japan)", "Book name");
    InitSystem(p39_S, name, author, book);
    //=================================================================================================================
    name = translate("Centre Point System", "System name");
    author = translate("Louis Devere", "Author name");
    book = translate("The Handbook of Practical Cutting on the Centre Point System", "Book name");
    InitSystem(p40_S, name, author, book);
    //=================================================================================================================
    name = translate("Aldrich/Men", "System name");
    author = translate("Winifred Aldrich", "Author name");
    book = translate("Metric Pattern Cutting for Menswear", "Book name");
    InitSystem(p41_S, name, author, book);
    //=================================================================================================================
    name = translate("Aldrich/Women", "System name");
    author = translate("Winifred Aldrich", "Author name");
    book = translate("Metric Pattern Cutting for Women's Wear", "Book name");
    InitSystem(p42_S, name, author, book);
    //=================================================================================================================
    name = translate("Kershaw", "System name");
    author = translate("Gareth Kershaw", "Author name");
    book = translate("Patternmaking for Menswear", "Book name");
    InitSystem(p43_S, name, author, book);
    //=================================================================================================================
    name = translate("Gilewska", "System name");
    author = translate("Teresa Gilewska", "Author name");
    book = translate("Pattern-Drafting for Fashion: The Basics", "Book name");
    InitSystem(p44_S, name, author, book);
    //=================================================================================================================
    name = translate("Lo", "System name");
    author = translate("Dennic Chunman Lo", "Author name");
    book = translate("Pattern Cutting", "Book name");
    InitSystem(p45_S, name, author, book);
    //=================================================================================================================
    name = translate("Bray", "System name");
    author = translate("Natalie Bray", "Author name");
    book = translate("Dress Pattern Designing: The Basic Principles of Cut and Fit", "Book name");
    InitSystem(p46_S, name, author, book);
    //=================================================================================================================
    name = translate("Knowles/Men", "System name");
    author = translate("Lori A. Knowles", "Author name");
    book = translate("The Practical Guide to Patternmaking for Fashion Designers: Menswear", "Book name");
    InitSystem(p47_S, name, author, book);
    //=================================================================================================================
    name = translate("Friendship/Men", "System name");
    author = translate("Elizabeth Friendship", "Author name");
    book = translate("Pattern Cutting for Men's Costume", "Book name");
    InitSystem(p48_S, name, author, book);
    //=================================================================================================================
    name = translate("Brown", "System name");
    author = translate("P. Clement Brown", "Author name");
    book = translate("Art in Dress", "Book name");
    InitSystem(p49_S, name, author, book);
    //=================================================================================================================
    name = translate("Mitchell", "System name");
    author = translate("Jno. J. Mitchell", "Author name");
    book = translate("\"Standard\" Work on Cutting (Men's Garments) 1886: The Art and Science of Garment Cutting",
                     "Book name");
    InitSystem(p50_S, name, author, book);
    //=================================================================================================================
    name = translate("GOST 17917-86", "System name");
    author = translate("Ministry of consumer industry of the USSR", "Author name");
    book = translate("Standard figure boys", "Book name");
    InitSystem(p51_S, name, author, book);
    //=================================================================================================================
    name = translate("Eddy", "System name");
    author = translate("Josephine F. Eddy and Elizabeth C. B. Wiley", "Author name");
    book = translate("Pattern and Dress Design", "Book name");
    InitSystem(p52_S, name, author, book);
    //=================================================================================================================
    name = translate("Knowles/Women", "System name");
    author = translate("Lori A. Knowles", "Author name");
    book = translate("Practical Guide to Patternmaking for Fashion Designers: Juniors, Misses, and Women", "Book name");
    InitSystem(p53_S, name, author, book);
    //=================================================================================================================
    name = translate("American Garment Cutter", "System name");
    author = translate("ed. R. L. Shep", "Author name");
    book = translate("The Great War: Styles and Patterns of the 1910s", "Book name");
    InitSystem(p54_S, name, author, book);
    //=================================================================================================================
    name = translate("None", "System name");
    author = translate("Valentina team", "Author name");
    book = translate("Valentina's internal standard", "Book name");
    InitSystem(p998_S, name, author, book);
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitVariables()
{
    variables.insert(line_, translate("Line_", "Left symbol _ in name"));
    variables.insert(angleLine_, translate("AngleLine_", "Left symbol _ in name"));
    variables.insert(arc_, translate("Arc_", "Left symbol _ in name"));
    variables.insert(spl_, translate("Spl_", "Left symbol _ in name"));
    variables.insert(splPath, translate("SplPath", "Do not add symbol _ to the end of name"));
    variables.insert(radiusArc_, translate("RadiusArc_", "Left symbol _ in name"));
    variables.insert(angle1Arc_, translate("Angle1Arc_", "Left symbol _ in name"));
    variables.insert(angle2Arc_, translate("Angle2Arc_", "Left symbol _ in name"));
    variables.insert(angle1Spl_, translate("Angle1Spl_", "Left symbol _ in name"));
    variables.insert(angle2Spl_, translate("Angle2Spl_", "Left symbol _ in name"));
    variables.insert(angle1SplPath, translate("Angle1SplPath", "Do not add symbol _ to the end of name"));
    variables.insert(angle2SplPath, translate("Angle2SplPath", "Do not add symbol _ to the end of name"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitFunctions()
{
    functions.insert(sin_F, translate("sin", "sine function"));
    functions.insert(cos_F, translate("cos", "cosine function"));
    functions.insert(tan_F, translate("tan", "tangens function"));
    functions.insert(asin_F, translate("asin", "arcus sine function"));
    functions.insert(acos_F, translate("acos", "arcus cosine function"));
    functions.insert(atan_F, translate("atan", "arcus tangens function"));
    functions.insert(sinh_F, translate("sinh", "hyperbolic sine function"));
    functions.insert(cosh_F, translate("cosh", "hyperbolic cosine"));
    functions.insert(tanh_F, translate("tanh", "hyperbolic tangens function"));
    functions.insert(asinh_F, translate("asinh", "hyperbolic arcus sine function"));
    functions.insert(acosh_F, translate("acosh", "hyperbolic arcus tangens function"));
    functions.insert(atanh_F, translate("atanh", "hyperbolic arcur tangens function"));
    functions.insert(log2_F, translate("log2", "logarithm to the base 2"));
    functions.insert(log10_F, translate("log10", "logarithm to the base 10"));
    functions.insert(log_F, translate("log", "logarithm to the base 10"));
    functions.insert(ln_F, translate("ln", "logarithm to base e (2.71828...)"));
    functions.insert(exp_F, translate("exp", "e raised to the power of x"));
    functions.insert(sqrt_F, translate("sqrt", "square root of a value"));
    functions.insert(sign_F, translate("sign", "sign function -1 if x<0; 1 if x>0"));
    functions.insert(rint_F, translate("rint", "round to nearest integer"));
    functions.insert(abs_F, translate("abs", "absolute value"));
    functions.insert(min_F, translate("min", "min of all arguments"));
    functions.insert(max_F, translate("max", "max of all arguments"));
    functions.insert(sum_F, translate("sum", "sum of all arguments"));
    functions.insert(avg_F, translate("avg", "mean value of all arguments"));
    functions.insert(fmod_F, translate("fmod",
                                       "Returns the floating-point remainder of numer/denom (rounded towards zero)"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPostfixOperators()
{
    postfixOperators.insert(cm_Oprt, translate("cm", "centimeter"));
    postfixOperators.insert(mm_Oprt, translate("mm", "millimeter"));
    postfixOperators.insert(in_Oprt, translate("in", "inch"));
}

#undef translate

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
