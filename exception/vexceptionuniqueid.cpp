#include "vexceptionuniqueid.h"

VExceptionUniqueId::VExceptionUniqueId(const QString &what, const QDomElement &domElement)
    :VException(what), tagText(QString()), tagName(QString()), lineNumber(-1){
    Q_ASSERT_X(!domElement.isNull(), Q_FUNC_INFO, "domElement is null");
    QTextStream stream(&tagText);
    domElement.save(stream, 4);
    tagName = domElement.tagName();
    lineNumber = domElement.lineNumber();
}

QString VExceptionUniqueId::ErrorMessage() const{
    QString error = QString("ExceptionUniqueId: %1").arg(what);
    return error;
}

QString VExceptionUniqueId::DetailedInformation() const{
    QString detail = QString("tag: %1 in line %2\nFull tag:\n%3").arg(tagName).arg(lineNumber).arg(tagText);
    return detail;
}
