#ifndef DRW_TEXTCODEC_H
#define DRW_TEXTCODEC_H

#include <string>
#include <QtGlobal>

class QTextCodec;
class QStringList;
template <class Key, class T> class QMap;

class DRW_TextCodec
{
public:
    DRW_TextCodec();
    std::string fromUtf8(const std::string &s);
    std::string toUtf8(const std::string &s);
    int getVersion() const {return version;}
    void setVersion(std::string *v, bool dxfFormat);
    void setVersion(int v, bool dxfFormat);
    void setCodePage(const std::string *c, bool dxfFormat);
    void setCodePage(const std::string &c, bool dxfFormat){setCodePage(&c, dxfFormat);}
    std::string getCodePage() const {return cp;}

    static QMap<QString, QStringList> DXFCodePageMap();
    static QTextCodec* CodecForName(const QString &name);

private:
    std::string correctCodePage(const std::string& s);

private:
    Q_DISABLE_COPY(DRW_TextCodec)
    int version;
    std::string cp;
    QTextCodec *conv;
};

#endif // DRW_TEXTCODEC_H
