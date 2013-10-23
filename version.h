#ifndef VERSION_H
#define VERSION_H

#include <QtCore/QString>

extern const int MAJOR_VERSION = 0;
extern const int MINOR_VERSION = 1;
extern const int DEBUG_VERSION = 0;

extern const QString APP_VERSION(QStringLiteral("%1.%2.%3").arg(MAJOR_VERSION)
                                                           .arg(MINOR_VERSION)
                                                           .arg(DEBUG_VERSION));
extern const QString WARRANTY(QT_TR_NOOP("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE."));
#endif // VERSION_H
