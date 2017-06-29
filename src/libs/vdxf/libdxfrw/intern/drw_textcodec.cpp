#include "drw_textcodec.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <QString>
#include <QTextCodec>
#include "../drw_base.h"

DRW_TextCodec::DRW_TextCodec() {
    version = DRW::AC1021;
    conv = nullptr;
}

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

void DRW_TextCodec::setCodePage(std::string *c, bool dxfFormat){
    cp = correctCodePage(*c);
    if (version < DRW::AC1021)
    {
        if (cp == "UTF-8")
        { //DXF older than 2007 are write in win codepages
            cp = "ANSI_1252";
        }
        conv = QTextCodec::codecForName(cp.c_str());
    }
    else
    {
        if (dxfFormat)
        {
            conv = QTextCodec::codecForName("UTF-8");
        }
        else
        {
            conv = QTextCodec::codecForName("UTF-16");
        }
    }
}

std::string DRW_TextCodec::toUtf8(std::string s) {
    if (conv == nullptr)
    {
        return s;
    }

    const QString encodedString = conv->toUnicode(s.c_str());
    return encodedString.toStdString();
}

std::string DRW_TextCodec::fromUtf8(std::string s) {
    if (conv == nullptr)
    {
        return s;
    }

    const QByteArray encodedString = conv->fromUnicode(QString::fromStdString(s));
    return std::string(encodedString.constData());
}

std::string DRW_TextCodec::correctCodePage(const std::string& s) {
    //stringstream cause crash in OS/X, bug#3597944
    std::string cp=s;
    transform(cp.begin(), cp.end(), cp.begin(), toupper);
    //Latin/Thai
    if (cp=="ANSI_874" || cp=="CP874" || cp=="ISO8859-11" || cp=="TIS-620") {
        return "ANSI_874";
        //Central Europe and Eastern Europe
    } else if (cp=="ANSI_1250" || cp=="CP1250" || cp=="ISO8859-2") {
        return "ANSI_1250";
        //Cyrillic script
    } else if (cp=="ANSI_1251" || cp=="CP1251" || cp=="ISO8859-5" || cp=="KOI8-R" ||
               cp=="KOI8-U" || cp=="IBM 866") {
        return "ANSI_1251";
        //Western Europe
    } else if (cp=="ANSI_1252" || cp=="CP1252" || cp=="LATIN1" || cp=="ISO-8859-1" ||
               cp=="CP819" || cp=="CSISO" || cp=="IBM819" || cp=="ISO_8859-1" || cp=="APPLE ROMAN" ||
               cp=="ISO8859-1" || cp=="ISO8859-15" || cp=="ISO-IR-100" || cp=="L1" || cp=="IBM 850") {
        return "ANSI_1252";
        //Greek
    } else if (cp=="ANSI_1253" || cp=="CP1253" || cp=="iso8859-7") {
        return "ANSI_1253";
        //Turkish
    } else if (cp=="ANSI_1254" || cp=="CP1254" || cp=="iso8859-9" || cp=="iso8859-3") {
        return "ANSI_1254";
        //Hebrew
    } else if (cp=="ANSI_1255" || cp=="CP1255" || cp=="iso8859-8") {
        return "ANSI_1255";
        //Arabic
    } else if (cp=="ANSI_1256" || cp=="CP1256" || cp=="ISO8859-6") {
        return "ANSI_1256";
        //Baltic
    } else if (cp=="ANSI_1257" || cp=="CP1257" || cp=="ISO8859-4" || cp=="ISO8859-10" || cp=="ISO8859-13") {
        return "ANSI_1257";
        //Vietnamese
    } else if (cp=="ANSI_1258" || cp=="CP1258") {
        return "ANSI_1258";

        //Japanese
    } else if (cp=="ANSI_932" || cp=="SHIFT-JIS" || cp=="SHIFT_JIS" || cp=="CSSHIFTJIS" ||
               cp=="CSWINDOWS31J" || cp=="MS_KANJI" || cp=="X-MS-CP932" || cp=="X-SJIS" ||
               cp=="EUCJP" || cp=="EUC-JP" || cp=="CSEUCPKDFMTJAPANESE" || cp=="X-EUC" ||
               cp=="X-EUC-JP" || cp=="JIS7") {
        return "ANSI_932";
        //Chinese PRC GBK (XGB) simplified
    } else if (cp=="ANSI_936" || cp=="GBK" || cp=="GB2312" || cp=="CHINESE" || cp=="CN-GB" ||
               cp=="CSGB2312" || cp=="CSGB231280" || cp=="CSISO58BG231280" ||
               cp=="GB_2312-80" || cp=="GB231280" || cp=="GB2312-80" ||
               cp=="ISO-IR-58" || cp=="GB18030") {
        return "ANSI_936";
        //Korean
    } else if (cp=="ANSI_949" || cp=="EUCKR") {
        return "ANSI_949";
        //Chinese Big5 (Taiwan, Hong Kong SAR)
    } else if (cp=="ANSI_950" || cp=="BIG5" || cp=="CN-BIG5" || cp=="CSBIG5" ||
               cp=="X-X-BIG5" || cp=="BIG5-HKSCS") {
        return "ANSI_950";

//celtic
/*    } else if (cp=="ISO8859-14") {
       return "ISO8859-14";
    } else if (cp=="TSCII") {
        return "TSCII"; //tamil
    }*/

    } else if (cp=="UTF-8" || cp=="UTF8" || cp=="UTF8-BIT") {
        return "UTF-8";
    } else if (cp=="UTF-16" || cp=="UTF16" || cp=="UTF16-BIT") {
        return "UTF-16";
    }

    return "ANSI_1252";
}
