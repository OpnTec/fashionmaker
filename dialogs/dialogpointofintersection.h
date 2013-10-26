#ifndef DIALOGPOINTOFINTERSECTION_H
#define DIALOGPOINTOFINTERSECTION_H

#include "dialogtool.h"

namespace Ui {
class DialogPointOfIntersection;
}

class DialogPointOfIntersection : public DialogTool{
    Q_OBJECT   
public:
                   DialogPointOfIntersection(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                             QWidget *parent = 0);
                   ~DialogPointOfIntersection();
    inline QString getPointName() const {return pointName;}
    void           setPointName(const QString &value);
    inline qint64  getFirstPointId() const {return firstPointId;}
    void           setFirstPointId(const qint64 &value, const qint64 &id);
    inline qint64  getSecondPointId() const {return secondPointId;}
    void           setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void   ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogPointOfIntersection)
    Ui::DialogPointOfIntersection *ui;
    qint32         number;
    QString        pointName;
    qint64         firstPointId;
    qint64         secondPointId;
};

#endif // DIALOGPOINTOFINTERSECTION_H
