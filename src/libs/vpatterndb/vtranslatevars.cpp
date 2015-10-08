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

#define tr(source, disambiguation) QmuTranslation::translate("VTranslateVars", (source), (disambiguation))

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPatternMakingSystems()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation name;
    QmuTranslation author;
    QmuTranslation book;

    //=================================================================================================================
    name = tr("Bunka", "System name");
    author = tr("Bunka Fashion College", "Author name");
    book = tr("Fundamentals of Garment Design", "Book name");
    InitSystem(p0_S, name, author, book);
    //=================================================================================================================
    name = tr("Barnfield and Richard", "System name");
    author = tr("Jo Barnfield and Andrew Richards", "Author name");
    book = tr("Pattern Making Primer", "Book name");
    InitSystem(p1_S, name, author, book);
    //=================================================================================================================
    name = tr("Friendship/Women", "System name");
    author = tr("Elizabeth Friendship", "Author name");
    book = tr("Creating Historical Clothes - Pattern Cutting from the 16th to the 19th Centuries", "Book name");
    InitSystem(p2_S, name, author, book);
    //=================================================================================================================
    name = tr("Morris, K.", "System name");
    author = tr("Karen Morris", "Author name");
    book = tr("Sewing Lingerie that Fits", "Book name");
    InitSystem(p3_S, name, author, book);
    //=================================================================================================================
    name = tr("Castro", "System name");
    author = tr("Lucia Mors de Castro", "Author name");
    book = tr("Patternmaking in Practic", "Book name");
    InitSystem(p4_S, name, author, book);
    //=================================================================================================================
    name = tr("Kim & Uh", "System name");
    author = tr("Injoo Kim and Mykyung Uh", "Author name");
    book = tr("Apparel Making in Fashion Design", "Book name");
    InitSystem(p5_S, name, author, book);
    //=================================================================================================================
    name = tr("Waugh", "System name");
    author = tr("Norah Waugh", "Author name");
    book = tr("Corsets and Crinolines", "Book name");
    InitSystem(p6_S, name, author, book);
    //=================================================================================================================
    name = tr("Grimble", "System name");
    author = tr("Frances Grimble", "Author name");
    book = tr("Fashions of the Gilded Age", "Book name");
    InitSystem(p7_S, name, author, book);
    //=================================================================================================================
    name = tr("Thornton's International System", "System name");
    author = tr("ed. R. L. Shep", "Author name");
    book = tr("The Great War: Styles and Patterns of the 1910s", "Book name");
    InitSystem(p8_S, name, author, book);
    //=================================================================================================================
    name = tr("Hillhouse & Mansfield", "System name");
    author = tr("Marion S. Hillhouse and Evelyn A. Mansfield", "Author name");
    book = tr("Dress Design: Draping and Flat Pattern Making", "Book name");
    InitSystem(p9_S, name, author, book);
    //=================================================================================================================
    name = tr("Pivnick", "System name");
    author = tr("Esther Kaplan Pivnick", "Author name");
    book = tr("How to Design Beautiful Clothes: Designing and Pattern Making", "Book name");
    InitSystem(p10_S, name, author, book);
    //=================================================================================================================
    name = tr("Minister & Son", "System name");
    author = tr("Edward Minister & Son, ed. R. L. Shep", "Author name");
    book = tr("The Complete Guide to Practical Cutting (1853)", "Book name");
    InitSystem(p11_S, name, author, book);
    //=================================================================================================================
    name = tr("Strickland", "System name");
    author = tr("Gertrude Strickland", "Author name");
    book = tr("A Tailoring Manual", "Book name");
    InitSystem(p12_S, name, author, book);
    //=================================================================================================================
    name = tr("Loh & Lewis", "System name");
    author = tr("May Loh and Diehl Lewis", "Author name");
    book = tr("Patternless Fashion Design", "Book name");
    InitSystem(p13_S, name, author, book);
    //=================================================================================================================
    name = tr("Morris, F. R.", "System name");
    author = tr("F. R. Morris", "Author name");
    book = tr("Ladies Garment Cutting and Making", "Book name");
    InitSystem(p14_S, name, author, book);
    //=================================================================================================================
    name = tr("Mason", "System name");
    author = tr("Gertrude Mason", "Author name");
    book = tr("Gertrude Mason's Patternmaking Book", "Book name");
    InitSystem(p15_S, name, author, book);
    //=================================================================================================================
    name = tr("Kimata", "System name");
    author = tr("K. Kimata", "Author name");
    book = tr("K.Kimata's Simplified Drafting Book for Dressmaking", "Book name");
    InitSystem(p16_S, name, author, book);
    //=================================================================================================================
    name = tr("Master Designer", "System name");
    author = tr("The Master Designer (Chicago, IL)", "Author name");
    book = tr("Master Designer's System of Designing, Cutting and Grading", "Book name");
    InitSystem(p17_S, name, author, book);
    //=================================================================================================================
    name = tr("Kopp", "System name");
    author = tr("Ernestine Kopp, Vittorina Rolfo, Beatrice Zelin, Lee Gross", "Author name");
    book = tr("How to Draft Basic Patterns", "Book name");
    InitSystem(p18_S, name, author, book);
    //=================================================================================================================
    name = tr("Ekern", "System name");
    author = tr("Doris Ekern", "Author name");
    book = tr("Slacks Cut-to-Fit for Your Figure", "Book name");
    InitSystem(p19_S, name, author, book);
    //=================================================================================================================
    name = tr("Doyle", "System name");
    author = tr("Sarah J. Doyle", "Author name");
    book = tr("Sarah's Key to Pattern Drafting", "Book name");
    InitSystem(p20_S, name, author, book);
    //=================================================================================================================
    name = tr("Shelton", "System name");
    author = tr("Karla J. Shelton", "Author name");
    book = tr("Design and Sew Jeans", "Book name");
    InitSystem(p21_S, name, author, book);
    //=================================================================================================================
    name = tr("Lady Boutique", "System name");
    author = tr("Lady Boutique", "Author name");
    book = tr("Lady Boutique magazine (Japan)", "Book name");
    InitSystem(p22_S, name, author, book);
    //=================================================================================================================
    name = tr("Rohr", "System name");
    author = tr("M. Rohr", "Author name");
    book = tr("Pattern Drafting and Grading: Women's nd Misses' Garment Design", "Book name");
    InitSystem(p23_S, name, author, book);
    //=================================================================================================================
    name = tr("Moore", "System name");
    author = tr("Dorothy Moore", "Author name");
    book = tr("Dorothy Moore's Pattern Drafting and Dressmaking", "Book name");
    InitSystem(p24_S, name, author, book);
    //=================================================================================================================
    name = tr("Abling", "System name");
    author = tr("Bina Abling", "Author name");
    book = tr("Integrating Draping, Drafting and Drawing", "Book name");
    InitSystem(p25_S, name, author, book);
    //=================================================================================================================
    name = tr("Fukomoto", "System name");
    author = tr("Sue S. Fukomoto", "Author name");
    book = tr("Scientific Pattern Drafting as taught at Style Center School of Costume Design, Dressmaking and "
              "Millinery", "Book name");
    InitSystem(p26_S, name, author, book);
    //=================================================================================================================
    name = tr("Dressmaking International", "System name");
    author = tr("Dressmaking International", "Author name");
    book = tr("Dressmaking International magazine (Japan)", "Book name");
    InitSystem(p27_S, name, author, book);
    //=================================================================================================================
    name = tr("Erwin", "System name");
    author = tr("Mabel D. Erwin", "Author name");
    book = tr("Practical Dress Design", "Book name");
    InitSystem(p28_S, name, author, book);
    //=================================================================================================================
    name = tr("Gough", "System name");
    author = tr("E. L. G. Gough", "Author name");
    book = tr("Principles of Garment Cutting", "Book name");
    InitSystem(p29_S, name, author, book);
    //=================================================================================================================
    name = tr("Allemong", "System name");
    author = tr("Elizabeth M. Allemong", "Author name");
    book = tr("European Cut", "Book name");
    InitSystem(p30_S, name, author, book);
    //=================================================================================================================
    name = tr("McCunn", "System name");
    author = tr("Donald H. McCunn", "Author name");
    book = tr("How to Make Your Own Sewing Patterns", "Book name");
    InitSystem(p31_S, name, author, book);
    //=================================================================================================================
    name = tr("Zarapkar", "System name");
    author = tr("Shri K. R. Zarapkar and Shri Arvind K. Zarapkar", "Author name");
    book = tr("Zarapkar System of Cutting", "Book name");
    InitSystem(p32_S, name, author, book);
    //=================================================================================================================
    name = tr("Kunick", "System name");
    author = tr("Philip Kunick", "Author name");
    book = tr("Sizing, Pattern Construction and Grading for Women's and Children's Garments", "Book name");
    InitSystem(p33_S, name, author, book);
    //=================================================================================================================
    name = tr("Handford", "System name");
    author = tr("Jack Handford", "Author name");
    book = tr("Professional Patternmaking for Designers: Women's Wear, Men's Casual Wear", "Book name");
    InitSystem(p34_S, name, author, book);
    //=================================================================================================================
    name = tr("Davis", "System name");
    author = tr("R. I. Davis", "Author name");
    book = tr("Men's 17th & 18th Century Costume, Cut & Fashion", "Book name");
    InitSystem(p35_S, name, author, book);
    //=================================================================================================================
    name = tr("MacLochlainn", "System name");
    author = tr("Jason MacLochlainn", "Author name");
    book = tr("The Victorian Tailor: An Introduction to Period Tailoring", "Book name");
    InitSystem(p36_S, name, author, book);
    //=================================================================================================================
    name = tr("Joseph-Armstrong", "System name");
    author = tr("Helen Joseph-Armstrong", "Author name");
    book = tr("Patternmaking for Fashion Design", "Book name");
    InitSystem(p37_S, name, author, book);
    //=================================================================================================================
    name = tr("Supreme System", "System name");
    author = tr("Frederick T. Croonberg", "Author name");
    book = tr("The Blue Book of Men's Tailoring, Grand Edition of Supreme System for Producing Mens Garments (1907)",
              "Book name");
    InitSystem(p38_S, name, author, book);
    //=================================================================================================================
    name = tr("Sugino", "System name");
    author = tr("Dressmaking", "Author name");
    book = tr("Pattern Drafting Vols. I, II, III (Japan)", "Book name");
    InitSystem(p39_S, name, author, book);
    //=================================================================================================================
    name = tr("Centre Point System", "System name");
    author = tr("Louis Devere", "Author name");
    book = tr("The Handbook of Practical Cutting on the Centre Point System", "Book name");
    InitSystem(p40_S, name, author, book);
    //=================================================================================================================
    name = tr("Aldrich/Men", "System name");
    author = tr("Winifred Aldrich", "Author name");
    book = tr("Metric Pattern Cutting for Menswear", "Book name");
    InitSystem(p41_S, name, author, book);
    //=================================================================================================================
    name = tr("Aldrich/Women", "System name");
    author = tr("Winifred Aldrich", "Author name");
    book = tr("Metric Pattern Cutting for Women's Wear", "Book name");
    InitSystem(p42_S, name, author, book);
    //=================================================================================================================
    name = tr("Kershaw", "System name");
    author = tr("Gareth Kershaw", "Author name");
    book = tr("Patternmaking for Menswear", "Book name");
    InitSystem(p43_S, name, author, book);
    //=================================================================================================================
    name = tr("Gilewska", "System name");
    author = tr("Teresa Gilewska", "Author name");
    book = tr("Pattern-Drafting for Fashion: The Basics", "Book name");
    InitSystem(p44_S, name, author, book);
    //=================================================================================================================
    name = tr("Lo", "System name");
    author = tr("Dennic Chunman Lo", "Author name");
    book = tr("Pattern Cutting", "Book name");
    InitSystem(p45_S, name, author, book);
    //=================================================================================================================
    name = tr("Bray", "System name");
    author = tr("Natalie Bray", "Author name");
    book = tr("Dress Pattern Designing: The Basic Principles of Cut and Fit", "Book name");
    InitSystem(p46_S, name, author, book);
    //=================================================================================================================
    name = tr("Knowles/Men", "System name");
    author = tr("Lori A. Knowles", "Author name");
    book = tr("The Practical Guide to Patternmaking for Fashion Designers: Menswear", "Book name");
    InitSystem(p47_S, name, author, book);
    //=================================================================================================================
    name = tr("Friendship/Men", "System name");
    author = tr("Elizabeth Friendship", "Author name");
    book = tr("Pattern Cutting for Men's Costume", "Book name");
    InitSystem(p48_S, name, author, book);
    //=================================================================================================================
    name = tr("Brown", "System name");
    author = tr("P. Clement Brown", "Author name");
    book = tr("Art in Dress", "Book name");
    InitSystem(p49_S, name, author, book);
    //=================================================================================================================
    name = tr("Mitchell", "System name");
    author = tr("Jno. J. Mitchell", "Author name");
    book = tr("\"Standard\" Work on Cutting (Men's Garments) 1886: The Art and Science of Garment Cutting",
              "Book name");
    InitSystem(p50_S, name, author, book);
    //=================================================================================================================
    name = tr("GOST 17917-86", "System name");
    author = tr("Ministry of consumer industry of the USSR", "Author name");
    book = tr("Standard figure boys", "Book name");
    InitSystem(p51_S, name, author, book);
    //=================================================================================================================
    name = tr("Eddy", "System name");
    author = tr("Josephine F. Eddy and Elizabeth C. B. Wiley", "Author name");
    book = tr("Pattern and Dress Design", "Book name");
    InitSystem(p52_S, name, author, book);
    //=================================================================================================================
    name = tr("Knowles/Women", "System name");
    author = tr("Lori A. Knowles", "Author name");
    book = tr("Practical Guide to Patternmaking for Fashion Designers: Juniors, Misses, and Women", "Book name");
    InitSystem(p53_S, name, author, book);
    //=================================================================================================================
    name = tr("American Garment Cutter", "System name");
    author = tr("ed. R. L. Shep", "Author name");
    book = tr("The Great War: Styles and Patterns of the 1910s", "Book name");
    InitSystem(p54_S, name, author, book);
    //=================================================================================================================
    name = tr("None", "System name");
    author = tr("Valentina team", "Author name");
    book = tr("Valentina's internal standard", "Book name");
    InitSystem(p998_S, name, author, book);
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitVariables()
{
    variables.insert(line_, tr("Line_", "Left symbol _ in name"));
    variables.insert(angleLine_, tr("AngleLine_", "Left symbol _ in name"));
    variables.insert(arc_, tr("Arc_", "Left symbol _ in name"));
    variables.insert(spl_, tr("Spl_", "Left symbol _ in name"));
    variables.insert(splPath, tr("SplPath", "Do not add symbol _ to the end of name"));
    variables.insert(radiusArc_, tr("RadiusArc_", "Left symbol _ in name"));
    variables.insert(angle1Arc_, tr("Angle1Arc_", "Left symbol _ in name"));
    variables.insert(angle2Arc_, tr("Angle2Arc_", "Left symbol _ in name"));
    variables.insert(angle1Spl_, tr("Angle1Spl_", "Left symbol _ in name"));
    variables.insert(angle2Spl_, tr("Angle2Spl_", "Left symbol _ in name"));
    variables.insert(angle1SplPath, tr("Angle1SplPath", "Do not add symbol _ to the end of name"));
    variables.insert(angle2SplPath, tr("Angle2SplPath", "Do not add symbol _ to the end of name"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitFunctions()
{
    functions.insert(sin_F, tr("sin", "sine function"));
    functions.insert(cos_F, tr("cos", "cosine function"));
    functions.insert(tan_F, tr("tan", "tangens function"));
    functions.insert(asin_F, tr("asin", "arcus sine function"));
    functions.insert(acos_F, tr("acos", "arcus cosine function"));
    functions.insert(atan_F, tr("atan", "arcus tangens function"));
    functions.insert(sinh_F, tr("sinh", "hyperbolic sine function"));
    functions.insert(cosh_F, tr("cosh", "hyperbolic cosine"));
    functions.insert(tanh_F, tr("tanh", "hyperbolic tangens function"));
    functions.insert(asinh_F, tr("asinh", "hyperbolic arcus sine function"));
    functions.insert(acosh_F, tr("acosh", "hyperbolic arcus tangens function"));
    functions.insert(atanh_F, tr("atanh", "hyperbolic arcur tangens function"));
    functions.insert(log2_F, tr("log2", "logarithm to the base 2"));
    functions.insert(log10_F, tr("log10", "logarithm to the base 10"));
    functions.insert(log_F, tr("log", "logarithm to the base 10"));
    functions.insert(ln_F, tr("ln", "logarithm to base e (2.71828...)"));
    functions.insert(exp_F, tr("exp", "e raised to the power of x"));
    functions.insert(sqrt_F, tr("sqrt", "square root of a value"));
    functions.insert(sign_F, tr("sign", "sign function -1 if x<0; 1 if x>0"));
    functions.insert(rint_F, tr("rint", "round to nearest integer"));
    functions.insert(abs_F, tr("abs", "absolute value"));
    functions.insert(min_F, tr("min", "min of all arguments"));
    functions.insert(max_F, tr("max", "max of all arguments"));
    functions.insert(sum_F, tr("sum", "sum of all arguments"));
    functions.insert(avg_F, tr("avg", "mean value of all arguments"));
    functions.insert(fmod_F, tr("fmod", "Returns the floating-point remainder of numer/denom (rounded towards zero)"));
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateVars::InitPostfixOperators()
{
    postfixOperators.insert(cm_Oprt, tr("cm", "centimeter"));
    postfixOperators.insert(mm_Oprt, tr("mm", "millimeter"));
    postfixOperators.insert(in_Oprt, tr("in", "inch"));
}

#undef tr

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
