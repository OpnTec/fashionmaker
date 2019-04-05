/************************************************************************
 **
 **  @file   def.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2015
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

#include "def.h"

#include <QApplication>
#include <QChar>
#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QGuiApplication>
#include <QImage>
#include <QLatin1Char>
#include <QList>
#include <QMap>
#include <QMessageLogger>
#include <QObject>
#include <QPixmap>
#include <QPrinterInfo>
#include <QProcess>
#include <QRgb>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>
#include <QPixmapCache>
#include <QGraphicsItem>
#include <QGlobalStatic>

#include "vabstractapplication.h"
#include "vdatastreamenum.h"

const qreal   defCurveApproximationScale = 0.5;
const qreal   minCurveApproximationScale = 0.1;
const qreal   maxCurveApproximationScale = 10.0;

//functions
const QString degTorad_F = QStringLiteral("degTorad");
const QString radTodeg_F = QStringLiteral("radTodeg");
const QString sin_F   = QStringLiteral("sin");
const QString cos_F   = QStringLiteral("cos");
const QString tan_F   = QStringLiteral("tan");
const QString asin_F  = QStringLiteral("asin");
const QString acos_F  = QStringLiteral("acos");
const QString atan_F  = QStringLiteral("atan");
const QString sinh_F  = QStringLiteral("sinh");
const QString cosh_F  = QStringLiteral("cosh");
const QString tanh_F  = QStringLiteral("tanh");
const QString asinh_F = QStringLiteral("asinh");
const QString acosh_F = QStringLiteral("acosh");
const QString atanh_F = QStringLiteral("atanh");
const QString sinD_F   = QStringLiteral("sinD");
const QString cosD_F   = QStringLiteral("cosD");
const QString tanD_F   = QStringLiteral("tanD");
const QString asinD_F  = QStringLiteral("asinD");
const QString acosD_F  = QStringLiteral("acosD");
const QString atanD_F  = QStringLiteral("atanD");
const QString log2_F  = QStringLiteral("log2");
const QString log10_F = QStringLiteral("log10");
const QString log_F   = QStringLiteral("log");
const QString ln_F    = QStringLiteral("ln");
const QString exp_F   = QStringLiteral("exp");
const QString sqrt_F  = QStringLiteral("sqrt");
const QString sign_F  = QStringLiteral("sign");
const QString rint_F  = QStringLiteral("rint");
const QString r2cm_F  = QStringLiteral("r2cm");
const QString csrCm_F = QStringLiteral("csrCm");
const QString csrInch_F = QStringLiteral("csrInch");
const QString abs_F   = QStringLiteral("abs");
const QString min_F   = QStringLiteral("min");
const QString max_F   = QStringLiteral("max");
const QString sum_F   = QStringLiteral("sum");
const QString avg_F   = QStringLiteral("avg");
const QString fmod_F  = QStringLiteral("fmod");

const QStringList builInFunctions = QStringList() << degTorad_F
                                                  << radTodeg_F
                                                  << sin_F
                                                  << cos_F
                                                  << tan_F
                                                  << asin_F
                                                  << acos_F
                                                  << atan_F
                                                  << sinh_F
                                                  << cosh_F
                                                  << tanh_F
                                                  << asinh_F
                                                  << acosh_F
                                                  << atanh_F
                                                  << sinD_F
                                                  << cosD_F
                                                  << tanD_F
                                                  << asinD_F
                                                  << acosD_F
                                                  << atanD_F
                                                  << log2_F
                                                  << log10_F
                                                  << log_F
                                                  << ln_F
                                                  << exp_F
                                                  << sqrt_F
                                                  << sign_F
                                                  << rint_F
                                                  << r2cm_F
                                                  << csrCm_F
                                                  << csrInch_F
                                                  << abs_F
                                                  << min_F
                                                  << max_F
                                                  << sum_F
                                                  << avg_F
                                                  << fmod_F;

const QString pl_size          = QStringLiteral("size");
const QString pl_height        = QStringLiteral("height");
const QString pl_date          = QStringLiteral("date");
const QString pl_time          = QStringLiteral("time");
const QString pl_patternName   = QStringLiteral("patternName");
const QString pl_patternNumber = QStringLiteral("patternNumber");
const QString pl_author        = QStringLiteral("author");
const QString pl_customer      = QStringLiteral("customer");
const QString pl_userMaterial  = QStringLiteral("userMaterial");
const QString pl_pExt          = QStringLiteral("pExt");
const QString pl_pFileName     = QStringLiteral("pFileName");
const QString pl_mFileName     = QStringLiteral("mFileName");
const QString pl_mExt          = QStringLiteral("mExt");
const QString pl_pLetter       = QStringLiteral("pLetter");
const QString pl_pAnnotation   = QStringLiteral("pAnnotation");
const QString pl_pOrientation  = QStringLiteral("pOrientation");
const QString pl_pRotation     = QStringLiteral("pRotation");
const QString pl_pTilt         = QStringLiteral("pTilt");
const QString pl_pFoldPosition = QStringLiteral("pFoldPosition");
const QString pl_pName         = QStringLiteral("pName");
const QString pl_pQuantity     = QStringLiteral("pQuantity");
const QString pl_mFabric       = QStringLiteral("mFabric");
const QString pl_mLining       = QStringLiteral("mLining");
const QString pl_mInterfacing  = QStringLiteral("mInterfacing");
const QString pl_mInterlining  = QStringLiteral("mInterlining");
const QString pl_wCut          = QStringLiteral("wCut");
const QString pl_wOnFold       = QStringLiteral("wOnFold");

const QStringList labelTemplatePlaceholders = QStringList() << pl_size
                                                            << pl_height
                                                            << pl_date
                                                            << pl_time
                                                            << pl_patternName
                                                            << pl_patternNumber
                                                            << pl_author
                                                            << pl_customer
                                                            << pl_userMaterial
                                                            << pl_pExt
                                                            << pl_pFileName
                                                            << pl_mFileName
                                                            << pl_mExt
                                                            << pl_pLetter
                                                            << pl_pAnnotation
                                                            << pl_pOrientation
                                                            << pl_pRotation
                                                            << pl_pTilt
                                                            << pl_pFoldPosition
                                                            << pl_pName
                                                            << pl_pQuantity
                                                            << pl_mFabric
                                                            << pl_mLining
                                                            << pl_mInterfacing
                                                            << pl_mInterlining
                                                            << pl_wCut
                                                            << pl_wOnFold;

const QString cursorArrowOpenHand = QStringLiteral("://cursor/cursor-arrow-openhand.png");
const QString cursorArrowCloseHand = QStringLiteral("://cursor/cursor-arrow-closehand.png");

const QString unitMM   = QStringLiteral("mm");
const QString unitCM   = QStringLiteral("cm");
const QString unitINCH = QStringLiteral("inch");
const QString unitPX   = QStringLiteral("px");

const QString valentinaNamespace = QStringLiteral("valentina");

//---------------------------------------------------------------------------------------------------------------------
QPixmap QPixmapFromCache(const QString &pixmapPath)
{
    QPixmap pixmap;

    if (not QPixmapCache::find(pixmapPath, pixmap))
    {
        pixmap = QPixmap(pixmapPath);
        QPixmapCache::insert(pixmapPath, pixmap);
    }
    return pixmap;
}

//---------------------------------------------------------------------------------------------------------------------
void SetItemOverrideCursor(QGraphicsItem *item, const QString &pixmapPath, int hotX, int hotY)
{
#ifndef QT_NO_CURSOR
    SCASSERT(item != nullptr)
    item->setCursor(QCursor(QPixmapFromCache(pixmapPath), hotX, hotY));
#else
    Q_UNUSED(item)
    Q_UNUSED(pixmapPath)
    Q_UNUSED(hotX)
    Q_UNUSED(hotY)
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QStringList SupportedLocales()
{
    const QStringList locales = QStringList() << QStringLiteral("ru_RU")
                                              << QStringLiteral("uk_UA")
                                              << QStringLiteral("de_DE")
                                              << QStringLiteral("cs_CZ")
                                              << QStringLiteral("he_IL")
                                              << QStringLiteral("fr_FR")
                                              << QStringLiteral("it_IT")
                                              << QStringLiteral("nl_NL")
                                              << QStringLiteral("id_ID")
                                              << QStringLiteral("es_ES")
                                              << QStringLiteral("fi_FI")
                                              << QStringLiteral("en_US")
                                              << QStringLiteral("en_CA")
                                              << QStringLiteral("en_IN")
                                              << QStringLiteral("ro_RO")
                                              << QStringLiteral("zh_CN")
                                              << QStringLiteral("pt_BR")
                                              << QStringLiteral("el_GR")
                                              << QStringLiteral("pl_PL");

    return locales;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief strippedName the function call around curFile to exclude the path to the file.
 * @param fullFileName full path to the file.
 * @return file name.
 */
QString StrippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//---------------------------------------------------------------------------------------------------------------------
QString RelativeMPath(const QString &patternPath, const QString &absoluteMPath)
{
    if (patternPath.isEmpty() || absoluteMPath.isEmpty())
    {
        return absoluteMPath;
    }

    if (QFileInfo(absoluteMPath).isRelative())
    {
        return absoluteMPath;
    }

    return QFileInfo(patternPath).absoluteDir().relativeFilePath(absoluteMPath);
}

//---------------------------------------------------------------------------------------------------------------------
QString AbsoluteMPath(const QString &patternPath, const QString &relativeMPath)
{
    if (patternPath.isEmpty() || relativeMPath.isEmpty())
    {
        return relativeMPath;
    }

    if (QFileInfo(relativeMPath).isAbsolute())
    {
        return relativeMPath;
    }

    return QFileInfo(QFileInfo(patternPath).absoluteDir(), relativeMPath).absoluteFilePath();
}

//---------------------------------------------------------------------------------------------------------------------
QSharedPointer<QPrinter> PreparePrinter(const QPrinterInfo &info, QPrinter::PrinterMode mode)
{
    QPrinterInfo tmpInfo = info;
    if(tmpInfo.isNull() || tmpInfo.printerName().isEmpty())
    {
        const QStringList list = QPrinterInfo::availablePrinterNames();
        if(list.isEmpty())
        {
            return QSharedPointer<QPrinter>();
        }
        else
        {
            tmpInfo = QPrinterInfo::printerInfo(list.first());
        }
    }

    auto printer = QSharedPointer<QPrinter>(new QPrinter(tmpInfo, mode));
    printer->setResolution(static_cast<int>(PrintDPI));
    return printer;
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF GetMinPrinterFields(const QSharedPointer<QPrinter> &printer)
{
    QPageLayout layout = printer->pageLayout();
    layout.setUnits(QPageLayout::Millimeter);
    const QMarginsF minMargins = layout.minimumMargins();

    QMarginsF min;
    min.setLeft(UnitConvertor(minMargins.left(), Unit::Mm, Unit::Px));
    min.setRight(UnitConvertor(minMargins.right(), Unit::Mm, Unit::Px));
    min.setTop(UnitConvertor(minMargins.top(), Unit::Mm, Unit::Px));
    min.setBottom(UnitConvertor(minMargins.bottom(), Unit::Mm, Unit::Px));
    return min;
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF GetPrinterFields(const QSharedPointer<QPrinter> &printer)
{
    if (printer.isNull())
    {
        return QMarginsF();
    }

    qreal left = 0;
    qreal top = 0;
    qreal right = 0;
    qreal bottom = 0;
    printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
    // We can't use Unit::Px because our dpi in most cases is different
    QMarginsF def;
    def.setLeft(UnitConvertor(left, Unit::Mm, Unit::Px));
    def.setRight(UnitConvertor(right, Unit::Mm, Unit::Px));
    def.setTop(UnitConvertor(top, Unit::Mm, Unit::Px));
    def.setBottom(UnitConvertor(bottom, Unit::Mm, Unit::Px));
    return def;
}

//---------------------------------------------------------------------------------------------------------------------
QPixmap darkenPixmap(const QPixmap &pixmap)
{
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    const int imgh = img.height();
    const int imgw = img.width();
    for (int y = 0; y < imgh; ++y)
    {
        for (int x = 0; x < imgw; ++x)
        {
            int h, s, v;
            QRgb pixel = img.pixel(x, y);
            const int a = qAlpha(pixel);
            QColor hsvColor(pixel);
            hsvColor.getHsv(&h, &s, &v);
            s = qMin(100, s * 2);
            v = v / 2;
            hsvColor.setHsv(h, s, v);
            pixel = hsvColor.rgb();
            img.setPixel(x, y, qRgba(qRed(pixel), qGreen(pixel), qBlue(pixel), a));
        }
    }
    return QPixmap::fromImage(img);
}

//---------------------------------------------------------------------------------------------------------------------
void ShowInGraphicalShell(const QString &filePath)
{
#ifdef Q_OS_MAC
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \""+filePath+"\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#elif defined(Q_OS_WIN)
    QProcess::startDetached(QString("explorer /select, \"%1\"").arg(QDir::toNativeSeparators(filePath)));
#else
    const QString app = "xdg-open %d";
    QString cmd;
    for (int i = 0; i < app.size(); ++i)
    {
        QChar c = app.at(i);
        if (c == QLatin1Char('%') && i < app.size()-1)
        {
            c = app.at(++i);
            QString s;
            if (c == QLatin1Char('d'))
            {
                s = QLatin1Char('"') + QFileInfo(filePath).path() + QLatin1Char('"');
            }
            else if (c == QLatin1Char('%'))
            {
                s = c;
            }
            else
            {
                s = QLatin1Char('%');
                s += c;
            }
            cmd += s;
            continue;
        }
        cmd += c;
    }
    QProcess::startDetached(cmd);
#endif

}

//---------------------------------------------------------------------------------------------------------------------
bool IsOptionSet(int argc, char *argv[], const char *option)
{
    for (int i = 1; i < argc; ++i)
    {
        if (!qstrcmp(argv[i], option))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
// See issue #624. https://bitbucket.org/dismine/valentina/issues/624
void InitHighDpiScaling(int argc, char *argv[])
{
    /* For more info see: http://doc.qt.io/qt-5/highdpi.html */
    if (IsOptionSet(argc, argv, qPrintable(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING)))
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#else
        qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("1"));
#endif
    }
    else
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
#else
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", QByteArray("1"));
#endif
    }
}

const QString strOne   = QStringLiteral("one");
const QString strTwo   = QStringLiteral("two");
const QString strThree = QStringLiteral("three");

Q_GLOBAL_STATIC_WITH_ARGS(const QString, strTMark, (QLatin1String("tMark")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, strVMark, (QLatin1String("vMark")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, strVMark2, (QLatin1String("vMark2")))

//---------------------------------------------------------------------------------------------------------------------
QString PassmarkLineTypeToString(PassmarkLineType type)
{
    switch(type)
    {
        case PassmarkLineType::OneLine:
            return strOne;
        case PassmarkLineType::TwoLines:
            return strTwo;
        case PassmarkLineType::ThreeLines:
            return strThree;
        case PassmarkLineType::TMark:
            return *strTMark;
        case PassmarkLineType::VMark:
            return *strVMark;
        case PassmarkLineType::VMark2:
            return *strVMark2;
        default:
            break;
    }

    return strOne;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkLineType StringToPassmarkLineType(const QString &value)
{
    const QStringList values{strOne, strTwo, strThree, *strTMark, *strVMark, *strVMark2};

    switch(values.indexOf(value))
    {
        case 0: // strOne
            return PassmarkLineType::OneLine;
        case 1: // strTwo
            return PassmarkLineType::TwoLines;
        case 2: // strThree
            return PassmarkLineType::ThreeLines;
        case 3: // strTMark
            return PassmarkLineType::TMark;
        case 4: // strVMark
            return PassmarkLineType::VMark;
        case 5: // strVMark2
            return PassmarkLineType::VMark2;
        default:
            break;
    }
    return PassmarkLineType::OneLine;
}

const QString strStraightforward        = QStringLiteral("straightforward");
const QString strBisector               = QStringLiteral("bisector");
const QString strIntersection           = QStringLiteral("intersection");
const QString strIntersectionOnlyLeft   = QStringLiteral("intersectionLeft");
const QString strIntersectionOnlyRight  = QStringLiteral("intersectionRight");
const QString strIntersection2          = QStringLiteral("intersection2");
const QString strIntersection2OnlyLeft  = QStringLiteral("intersection2Left");
const QString strIntersection2OnlyRight = QStringLiteral("intersection2Right");
const QString strTypeIncrement          = QStringLiteral("increment");
const QString strTypeSeparator          = QStringLiteral("separator");

//---------------------------------------------------------------------------------------------------------------------
QString PassmarkAngleTypeToString(PassmarkAngleType type)
{
    switch(type)
    {
        case PassmarkAngleType::Straightforward:
            return strStraightforward;
        case PassmarkAngleType::Bisector:
            return strBisector;
        case PassmarkAngleType::Intersection:
            return strIntersection;
        case PassmarkAngleType::IntersectionOnlyLeft:
            return strIntersectionOnlyLeft;
        case PassmarkAngleType::IntersectionOnlyRight:
            return strIntersectionOnlyRight;
        case PassmarkAngleType::Intersection2:
            return strIntersection2;
        case PassmarkAngleType::Intersection2OnlyLeft:
            return strIntersection2OnlyLeft;
        case PassmarkAngleType::Intersection2OnlyRight:
            return strIntersection2OnlyRight;
        default:
            break;
    }

    return strStraightforward;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkAngleType StringToPassmarkAngleType(const QString &value)
{
    const QStringList values = QStringList() << strStraightforward
                                             << strBisector
                                             << strIntersection
                                             << strIntersectionOnlyLeft
                                             << strIntersectionOnlyRight
                                             << strIntersection2
                                             << strIntersection2OnlyLeft
                                             << strIntersection2OnlyRight;

    switch(values.indexOf(value))
    {
        case 0:
            return PassmarkAngleType::Straightforward;
        case 1:
            return PassmarkAngleType::Bisector;
        case 2:
            return PassmarkAngleType::Intersection;
        case 3:
            return PassmarkAngleType::IntersectionOnlyLeft;
        case 4:
            return PassmarkAngleType::IntersectionOnlyRight;
        case 5:
            return PassmarkAngleType::Intersection2;
        case 6:
            return PassmarkAngleType::Intersection2OnlyLeft;
        case 7:
            return PassmarkAngleType::Intersection2OnlyRight;
        default:
            break;
    }
    return PassmarkAngleType::Straightforward;
}


//---------------------------------------------------------------------------------------------------------------------
Unit StrToUnits(const QString &unit)
{
    const QStringList units = QStringList() << unitMM << unitCM << unitINCH << unitPX;
    Unit result = Unit::Cm;
    switch (units.indexOf(unit))
    {
        case 0:// mm
            result = Unit::Mm;
            break;
        case 2:// inch
            result = Unit::Inch;
            break;
        case 3:// px
            result = Unit::Px;
            break;
        case 1:// cm
        default:
            result = Unit::Cm;
            break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UnitsToStr translate unit to string.
 *
 * This method used when need write unit in xml file and for showing unit in dialogs.
 * @param unit curent unit
 * @param translate true if need show translated name. Default value false.
 * @return string reprezantation for unit.
 */
QString UnitsToStr(const Unit &unit, const bool translate)
{
    QString result;
    switch (unit)
    {
        case Unit::Mm:
            translate ? result = QObject::tr("mm") : result = unitMM;
            break;
        case Unit::Inch:
            translate ? result = QObject::tr("inch") : result = unitINCH;
            break;
        case Unit::Px:
            translate ? result = QObject::tr("px") : result = unitPX;
            break;
        case Unit::Cm:
        default:
            translate ? result = QObject::tr("cm") : result = unitCM;
            break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void InitLanguages(QComboBox *combobox)
{
    SCASSERT(combobox != nullptr)
    combobox->clear();

    QStringList fileNames;
    QDirIterator it(qApp->translationsPath(), QStringList("valentina_*.qm"), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        fileNames.append(it.fileName());
    }

    bool englishUS = false;
    const QString en_US = QStringLiteral("en_US");

    for (auto locale : fileNames)
    {
        // get locale extracted by filename           "valentina_de_De.qm"
        locale.truncate(locale.lastIndexOf('.'));  // "valentina_de_De"
        locale.remove(0, locale.indexOf('_') + 1); // "de_De"

        if (not englishUS)
        {
            englishUS = (en_US == locale);
        }

        QLocale loc = QLocale(locale);
        QString lang = loc.nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://flags", QLocale::countryToString(loc.country())));

        combobox->addItem(ico, lang, locale);
    }

    if (combobox->count() == 0 || not englishUS)
    {
        // English language is internal and doens't have own *.qm file.
        QIcon ico(QString("%1/%2.png").arg("://flags", QLocale::countryToString(QLocale::UnitedStates)));
        QString lang = QLocale(en_US).nativeLanguageName();
        combobox->addItem(ico, lang, en_US);
    }

    // set default translators and language checked
    qint32 index = combobox->findData(qApp->Settings()->GetLocale());
    if (index != -1)
    {
        combobox->setCurrentIndex(index);
    }
}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const CustomSARecord &record)
{
    out << record.startPoint;
    out << record.path;
    out << record.endPoint;
    out << record.reverse;
    out << record.includeType;
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, CustomSARecord &record)
{
    in >> record.startPoint;
    in >> record.path;
    in >> record.endPoint;
    in >> record.reverse;
    in >> record.includeType;
    return in;
}

//---------------------------------------------------------------------------------------------------------------------
QString IncrementTypeToString(IncrementType type)
{
    switch(type)
    {
        case IncrementType::Increment:
            return strTypeIncrement;
        case IncrementType::Separator:
            return strTypeSeparator;
        default:
            break;
    }

    return strTypeIncrement;
}

//---------------------------------------------------------------------------------------------------------------------
IncrementType StringToIncrementType(const QString &value)
{
    const QStringList values { strTypeIncrement, strTypeSeparator };

    switch(values.indexOf(value))
    {
        case 0:
            return IncrementType::Increment;
        case 1:
            return IncrementType::Separator;
        default:
            break;
    }
    return IncrementType::Increment;
}
