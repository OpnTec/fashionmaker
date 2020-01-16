#include "drw_textcodec.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <QString>
#include <QTextCodec>
#include "../drw_base.h"
#include "../vmisc/vabstractapplication.h"
#include "../ifc/exception/vexception.h"

namespace
{
QMap<QString, QStringList> QtCodecs()
{
    return QMap<QString, QStringList>
    {
        {"ANSI_874", {"ANSI_874", "CP874", "windows-874", "MS874", "x-windows-874", "TIS-620", "IBM1162",
                "x-IBM874"}}, // Latin/Thai
        {"ANSI_932", {"ANSI_932", "CP932", "SHIFT-JIS", "SHIFT_JIS", "CSSHIFTJIS", "CSWINDOWS31J", "MS_KANJI",
                "X-MS-CP932", "X-SJIS", "EUCJP", "EUC-JP", "CSEUCPKDFMTJAPANESE", "X-EUC", "X-EUC-JP", "IBM-943",
                "JIS7"}}, // Japanese
        {"ANSI_936", {"ANSI_936", "GBK", "CP936", "MS936", "Windows-936", "GB2312",
                "CHINESE"}}, // Chinese PRC GBK (XGB) simplified
        {"ANSI_949", {"ANSI_949", "Windows-949", "MS949", "CP949"}}, // Korean
        {"ANSI_950", {"ANSI_950", "BIG5", "windows-950-2000", "csBig5", "windows-950", "x-windows-950", "x-big5",
                "ms950"}}, // Chinese Big5 (Taiwan, Hong Kong SAR)
        {"ANSI_1250", {"ANSI_1250", "CP1250", "windows-1250", "ibm-1250_P100-1995",
                "ibm-1250"}}, //Central Europe and Eastern Europe
        {"ANSI_1251", {"ANSI_1251", "CP1251", "windows-1251", "ANSI1251", "ibm-5347_P100-1998",
                "ibm-5347"}}, // Cyrillic script
        {"ANSI_1252", {"ANSI_1252", "CP1252", "windows-1252", "LATIN1", "ISO-8859-1", "CP819", "CSISO", "IBM819",
                "ISO_8859-1", "APPLE ROMAN", "ISO8859-1", "ISO8859-15", "ISO-IR-100", "L1",
                "IBM 850", "850"}}, // Western Europe
        {"ANSI_1253", {"ANSI_1253", "CP1253", "windows-1253"}}, // Greek
        {"ANSI_1254", {"ANSI_1254", "CP1254", "windows-1254"}}, // Turkish
        {"ANSI_1255", {"ANSI_1255", "CP1255", "windows-1255"}}, // Hebrew
        {"ANSI_1256", {"ANSI_1256", "CP1256", "windows-1256", "x-windows-1256S"}}, // Arabic
        {"ANSI_1257", {"ANSI_1257", "CP1257", "windows-1257"}}, // Baltic
        {"ANSI_1258", {"ANSI_1258", "CP1258", "windows-1258"}}, // Vietnamese
        {"UTF-8", {"UTF-8", "UTF8", "UTF8-BIT"}},
        {"UTF-16", {"UTF-16", "UTF16", "UTF16-BIT"}},
    };
}
}

DRW_TextCodec::DRW_TextCodec()
    : version(DRW::AC1021),
      cp(),
      conv(nullptr)
{}

void DRW_TextCodec::setVersion(int v, bool dxfFormat){
    if (v == DRW::AC1009 || v == DRW::AC1006) {
        version = DRW::AC1009;
        cp = "ANSI_1252";
        setCodePage(&cp, dxfFormat);
    } else if (v == DRW::AC1012 || v == DRW::AC1014
             || v == DRW::AC1015 || v == DRW::AC1018) {
        version = DRW::AC1015;
//        if (cp.empty()) { //codepage not set, initialize
            cp = "ANSI_1252";
            setCodePage(&cp, dxfFormat);
//        }
    } else {
        version = DRW::AC1021;
        if (dxfFormat)
            cp = "UTF-8";//RLZ: can be UCS2 or UTF-16 16bits per char
        else
            cp = "UTF-16";//RLZ: can be UCS2 or UTF-16 16bits per char
        setCodePage(&cp, dxfFormat);
    }
}

void DRW_TextCodec::setVersion(std::string *v, bool dxfFormat){
    std::string versionStr = *v;
    if (versionStr == "AC1009" || versionStr == "AC1006") {
        setVersion(DRW::AC1009, dxfFormat);
    } else if (versionStr == "AC1012" || versionStr == "AC1014"
             || versionStr == "AC1015" || versionStr == "AC1018") {
        setVersion(DRW::AC1015, dxfFormat);
    }
    else
    {
        setVersion(DRW::AC1021, dxfFormat);
    }
}

void DRW_TextCodec::setCodePage(const std::string *c, bool dxfFormat){
    cp = correctCodePage(*c);
    if (version < DRW::AC1021)
    {
        if (cp == "UTF-8")
        { //DXF older than 2007 are write in win codepages
            cp = "ANSI_1252";
        }
        conv = DRW_TextCodec::CodecForName(QString::fromStdString(cp));
    }
    else
    {
        if (dxfFormat)
        {
            conv = DRW_TextCodec::CodecForName(QString::fromStdString("UTF-8"));
        }
        else
        {
            conv = DRW_TextCodec::CodecForName(QString::fromStdString("UTF-16"));
        }
    }

    if (conv == nullptr)
    {
        const QString errorMsg = QCoreApplication::translate("DRW_TextCodec", "No available codec for code page '%1'.")
                .arg(cp.c_str());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;

        if (version < DRW::AC1021 && cp == "UTF-8")
        {
            cp = "ANSI_1252";
            conv = DRW_TextCodec::CodecForName(QString::fromStdString(cp)); // Fallback to latin
        }
    }
}

QMap<QString, QStringList> DRW_TextCodec::DXFCodePageMap()
{
    return QMap<QString, QStringList>
    {
        {"ANSI_874", {"ANSI_874", "CP874", "ISO8859-11", "TIS-620"}}, // Latin/Thai
        {"ANSI_932", {"ANSI_932", "SHIFT-JIS", "SHIFT_JIS", "CSSHIFTJIS", "CSWINDOWS31J", "MS_KANJI", "X-MS-CP932",
                "X-SJIS", "EUCJP", "EUC-JP", "CSEUCPKDFMTJAPANESE", "X-EUC", "X-EUC-JP", "CP932",
                "JIS7"}}, // Japanese
        {"ANSI_936", {"ANSI_936", "GBK", "GB2312", "CHINESE", "CN-GB", "CSGB2312", "CSGB231280", "CSISO58BG231280",
                "GB_2312-80", "GB231280", "GB2312-80", "ISO-IR-58", "GB18030"}}, // Chinese PRC GBK (XGB) simplified
        {"ANSI_949", {"ANSI_949", "EUCKR"}}, // Korean
        {"ANSI_950", {"ANSI_950", "BIG5", "CN-BIG5", "CSBIG5", "X-X-BIG5",
                "BIG5-HKSCS"}}, // Chinese Big5 (Taiwan, Hong Kong SAR)
        {"ANSI_1250", {"ANSI_1250", "CP1250", "ISO8859-2"}}, //Central Europe and Eastern Europe
        {"ANSI_1251", {"ANSI_1251", "CP1251", "ISO8859-5", "KOI8-R", "KOI8-U", "IBM 866"}}, // Cyrillic script
        {"ANSI_1252", {"ANSI_1252", "CP1252", "LATIN1", "ISO-8859-1", "CP819", "CSISO", "IBM819", "ISO_8859-1",
                "APPLE ROMAN", "ISO8859-1", "ISO8859-15", "ISO-IR-100", "L1", "IBM 850"}}, // Western Europe
        {"ANSI_1253", {"ANSI_1253", "CP1253", "ISO8859-7"}}, // Greek
        {"ANSI_1254", {"ANSI_1254", "CP1254", "ISO8859-9", "iso8859-3"}}, // Turkish
        {"ANSI_1255", {"ANSI_1255", "CP1255", "ISO8859-8"}}, // Hebrew
        {"ANSI_1256", {"ANSI_1256", "CP1256", "ISO8859-6"}}, // Arabic
        {"ANSI_1257", {"ANSI_1257", "CP1257", "ISO8859-4", "ISO8859-10", "ISO8859-13"}}, // Baltic
        {"ANSI_1258", {"ANSI_1258", "CP1258"}}, // Vietnamese
        {"UTF-8", {"UTF-8", "UTF8", "UTF8-BIT"}},
        {"UTF-16", {"UTF-16", "UTF16", "UTF16-BIT"}},
    };
}

QTextCodec *DRW_TextCodec::CodecForName(const QString &name)
{
    QMap<QString, QStringList> knownCodecs = QtCodecs();
    if (knownCodecs.contains(name))
    {
        QStringList aliases = knownCodecs.value(name);
        for (auto &alias : aliases)
        {
            if (QTextCodec *codec = QTextCodec::codecForName(alias.toLatin1()))
            {
                return codec;
            }
        }
    }

    return nullptr;
}

std::string DRW_TextCodec::toUtf8(const std::string &s) {
    if (conv == nullptr)
    {
        return s;
    }

    const QString encodedString = conv->toUnicode(s.c_str());
    return encodedString.toStdString();
}

std::string DRW_TextCodec::fromUtf8(const std::string &s) {
    if (conv == nullptr)
    {
        return s;
    }

    const QByteArray encodedString = conv->fromUnicode(QString::fromStdString(s));
    return std::string(encodedString.constData());
}

std::string DRW_TextCodec::correctCodePage(const std::string& s) {
    //stringstream cause crash in OS/X, bug#3597944
    QString cp = QString::fromStdString(s);
    cp = cp.toUpper();
    QMap<QString, QStringList> codeMap = DRW_TextCodec::DXFCodePageMap();

    auto i = codeMap.constBegin();
    while (i != codeMap.constEnd())
    {
        if (i.value().contains(cp))
        {
            return i.key().toStdString();
        }
        ++i;
    }

    return "ANSI_1252";
}
