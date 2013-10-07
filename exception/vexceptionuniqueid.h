#ifndef VEXCEPTIONUNIQUEID_H
#define VEXCEPTIONUNIQUEID_H

#include "vexception.h"
#include <QDomElement>

class VExceptionUniqueId : public VException{
public:
    VExceptionUniqueId(const QString &what, const QDomElement &domElement);
    VExceptionUniqueId(const VExceptionUniqueId &e):VException(e), tagText(e.TagText()),
        tagName(e.TagName()), lineNumber(e.LineNumber()){}
    virtual ~VExceptionUniqueId() noexcept(true){}
    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    QString TagText() const {return tagText;}
    QString TagName() const {return tagName;}
    qint32 LineNumber() const {return lineNumber;}
protected:
    QString tagText;
    QString tagName;
    qint32  lineNumber;
};

#endif // VEXCEPTIONUNIQUEID_H
