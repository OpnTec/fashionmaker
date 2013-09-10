#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include "ui_dialogpointofcontact.h"
#include "dialogtool.h"
#include "options.h"
#include "container/vcontainer.h"
#include "container/calculator.h"

class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
    
public:
    explicit DialogPointOfContact(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                  QWidget *parent = 0);
    QString getPointName() const;
    void setPointName(const QString &value);

    QString getRadius() const;
    void setRadius(const QString &value);

    qint64 getCenter() const;
    void setCenter(const qint64 &value, const qint64 &id);

    qint64 getFirstPoint() const;
    void setFirstPoint(const qint64 &value, const qint64 &id);

    qint64 getSecondPoint() const;
    void setSecondPoint(const qint64 &value, const qint64 &id);

public slots:
    virtual void            ChoosedObject(qint64 id, Scene::Type type);
    virtual void            DialogAccepted();
private:
    Ui::DialogPointOfContact ui;
    qint32                  number;
    QString                 pointName;
    QString                 radius;
    qint64                  center;
    qint64                  firstPoint;
    qint64                  secondPoint;
    DialogPointOfContact(const DialogPointOfContact &dialog);
    const DialogPointOfContact &operator=(const DialogPointOfContact& dialog);
};

#endif // DIALOGPOINTOFCONTACT_H
