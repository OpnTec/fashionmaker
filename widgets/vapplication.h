#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include <QApplication>

class VApplication : public QApplication
{
    Q_OBJECT
public:
    VApplication(int &argc, char ** argv);
    virtual ~VApplication() { }
    virtual bool notify(QObject * receiver, QEvent * event);
};

#endif // VAPPLICATION_H
