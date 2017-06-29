#ifndef DRW_TEXTCODEC_H
#define DRW_TEXTCODEC_H

#include <string>

class QTextCodec;

class DRW_TextCodec
{
public:
    DRW_TextCodec();
    std::string fromUtf8(std::string s);
    std::string toUtf8(std::string s);
    int getVersion(){return version;}
    void setVersion(std::string *v, bool dxfFormat);
    void setVersion(int v, bool dxfFormat);
    void setCodePage(std::string *c, bool dxfFormat);
    void setCodePage(std::string c, bool dxfFormat){setCodePage(&c, dxfFormat);}
    std::string getCodePage(){return cp;}

private:
    std::string correctCodePage(const std::string& s);

private:
    int version;
    std::string cp;
    QTextCodec *conv;
};

#endif // DRW_TEXTCODEC_H
